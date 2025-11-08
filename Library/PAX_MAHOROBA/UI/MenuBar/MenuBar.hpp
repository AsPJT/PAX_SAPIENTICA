/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_HEADER_PANEL_HPP
#define PAX_MAHOROBA_UI_HEADER_PANEL_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/GitHubButton.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/MenuSystem.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>

#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>
#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief アプリ上部のメニューバー
    class MenuBar : public IWidget{
    public:
        /// @brief コンストラクタ
        MenuBar() :
            language_selector_(
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                paxs::LanguageDomain::UI,
                paxg::Vec2i{3000, 0},
                paxs::PulldownDisplayType::SelectedValue,
                true
            )
        {
            language_selector_.setItemsKey(paxs::LanguageKeys::ALL_LANGUAGE_HASHES);

            // GitHubボタンを初期化
            github_button_.init(language_selector_);

            // メニューバーにメニュー項目を追加（FontSystem経由）
            menu_system.add(paxs::MenuBarKeys::VIEW_MENU_HASHES,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("view"));
            menu_system.add(paxs::MenuBarKeys::FEATURE_MENU_HASHES,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("place_names"));
            menu_system.add(paxs::MenuBarKeys::MAP_MENU_HASHES,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("map"));

            calculateLayout();
            menu_system.updateMenuWidth();
        }

        /// @brief レイアウトを計算
        void calculateLayout() const {
            // 言語選択プルダウンを右端に配置
            language_selector_.setPos(paxg::Vec2i{
                static_cast<int>(paxg::Window::width() - language_selector_.getRect().w()),
                0
            });
            github_button_.setPos(paxg::Vec2i{
                static_cast<int>(language_selector_.getRect().x() - github_button_.getRect().w() - 8),
                static_cast<int>((language_selector_.getRect().h() - github_button_.getRect().h()) / 2)
            });
        }

        /// @brief ヘッダーの高さを取得
        float getHeight() const {
            return language_selector_.getRect().h();
        }

        /// @brief メニューバーの取得（読み取り専用）
        const MenuSystem& getMenuSystem() const {
            return menu_system;
        }

        /// @brief メニューバーの取得（変更可能）- TileManager用
        MenuSystem& getMenuSystem() {
            return menu_system;
        }

        /// @brief 言語選択のインデックスを取得
        std::size_t getLanguageIndex() const {
            return language_selector_.getIndex();
        }

        /// @brief 言語選択のキーを取得
        std::uint_least32_t getLanguageKey() const {
            return language_selector_.getKey();
        }

        /// @brief AppStateManagerを設定
        /// @brief Set AppStateManager
        void setAppStateManager(AppStateManager* app_state_manager) {
            app_state_manager_ = app_state_manager;
            // AppStateManagerが設定されたら、イベント購読を開始
            if (app_state_manager_ && !events_subscribed_) {
                subscribeToEvents();
                events_subscribed_ = true;
            }
        }

        /// @brief 可視性状態を反映
        void initializeVisibility(paxs::FeatureVisibilityManager* visible_manager) {
            // 可視性の初期化
            visible_manager->emplace(MurMur3::calcHash("Calendar"), true); // 暦
            visible_manager->emplace(MurMur3::calcHash("Map"), true); // 地図
            visible_manager->emplace(MurMur3::calcHash("UI"), true); // UI
            visible_manager->emplace(MurMur3::calcHash("Simulation"), true); // シミュレーション
            visible_manager->emplace(MurMur3::calcHash("License"), false); // ライセンス
            visible_manager->emplace(MurMur3::calcHash("Debug"), false); // デバッグ
            visible_manager->emplace(MurMur3::calcHash("3D"), false); // 360度写真

            // View メニューの状態を初期化
            paxs::DropDownMenu* view_menu = menu_system.getDropDownMenu(MurMur3::calcHash("view"));
            if (!view_menu) {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'view' menu not found.");
                return;
            }
            view_menu->setIsItems(std::size_t(0), visible_manager->isVisible(MurMur3::calcHash("Calendar")));
            view_menu->setIsItems(std::size_t(1), visible_manager->isVisible(MurMur3::calcHash("Map")));
            view_menu->setIsItems(std::size_t(2), visible_manager->isVisible(MurMur3::calcHash("UI")));
            view_menu->setIsItems(std::size_t(3), visible_manager->isVisible(MurMur3::calcHash("Simulation")));
            view_menu->setIsItems(std::size_t(4), visible_manager->isVisible(MurMur3::calcHash("License")));
            view_menu->setIsItems(std::size_t(5), visible_manager->isVisible(MurMur3::calcHash("Debug")));
            view_menu->setIsItems(std::size_t(6), visible_manager->isVisible(MurMur3::calcHash("3D")));
        }

        /// @brief 言語変更を検出してイベントを発行
        /// @brief Detect language change and publish event
        void updateLanguage() const {
            if (!app_state_manager_) return;

            const std::size_t current_index = language_selector_.getIndex();

            if (previous_language_index_ != current_index) {
                previous_language_index_ = current_index;
                const std::uint_least32_t language_key = language_selector_.getKey();
                // AppStateManager経由で言語変更イベントを発行（インデックスとキーの両方を渡す）
                app_state_manager_->setLanguage(static_cast<std::uint_least8_t>(current_index), language_key);
            }
        }

        /// @brief メニュー状態を可視性に反映（毎フレーム呼び出し）
        /// @note AppStateManager経由で可視性を更新
        void syncVisibilityFromMenu() {
            if (!app_state_manager_) return;

            // View メニューの状態を同期
            const paxs::DropDownMenu* view_menu = menu_system.cgetDropDownMenu(MurMur3::calcHash("view"));
            if (!view_menu) {
                PAXS_WARNING("MenuBar::syncVisibilityFromMenu: 'view' menu not found.");
                return;
            }
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("Calendar"), view_menu->getIsItems(std::size_t(0)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("Map"), view_menu->getIsItems(std::size_t(1)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("UI"), view_menu->getIsItems(std::size_t(2)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("Simulation"), view_menu->getIsItems(std::size_t(3)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("License"), view_menu->getIsItems(std::size_t(4)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("Debug"), view_menu->getIsItems(std::size_t(5)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("3D"), view_menu->getIsItems(std::size_t(6)));

            // Place Names メニューの状態を同期
            const paxs::DropDownMenu* place_names_menu = menu_system.cgetDropDownMenu(MurMur3::calcHash("place_names"));
            if (!place_names_menu) {
                PAXS_WARNING("MenuBar::syncVisibilityFromMenu: 'place_names' menu not found.");
                return;
            }
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("place_name"), place_names_menu->getIsItems(std::size_t(0)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("site"), place_names_menu->getIsItems(std::size_t(1)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("tumulus"), place_names_menu->getIsItems(std::size_t(2)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("dolmen"), place_names_menu->getIsItems(std::size_t(3)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("kamekanbo"), place_names_menu->getIsItems(std::size_t(4)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("stone_coffin"), place_names_menu->getIsItems(std::size_t(5)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("doken"), place_names_menu->getIsItems(std::size_t(6)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("dotaku"), place_names_menu->getIsItems(std::size_t(7)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("bronze_mirror"), place_names_menu->getIsItems(std::size_t(8)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("human_bone"), place_names_menu->getIsItems(std::size_t(9)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("mtdna"), place_names_menu->getIsItems(std::size_t(10)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("ydna"), place_names_menu->getIsItems(std::size_t(11)));

            // Map メニューの状態を同期
            const paxs::DropDownMenu* map_menu = menu_system.cgetDropDownMenu(MurMur3::calcHash("map"));
            if (!map_menu) {
                PAXS_WARNING("MenuBar::syncVisibilityFromMenu: 'map' menu not found.");
                return;
            }
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_base"), map_menu->getIsItems(std::size_t(0)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_land_and_sea"), map_menu->getIsItems(std::size_t(1)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_land_and_water"), map_menu->getIsItems(std::size_t(2)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_soil"), map_menu->getIsItems(std::size_t(3)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_soil_temperature"), map_menu->getIsItems(std::size_t(4)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_ryosei_country"), map_menu->getIsItems(std::size_t(5)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_ryosei_line"), map_menu->getIsItems(std::size_t(6)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_slope"), map_menu->getIsItems(std::size_t(7)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"), map_menu->getIsItems(std::size_t(8)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"), map_menu->getIsItems(std::size_t(9)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_line1"), map_menu->getIsItems(std::size_t(10)));
            app_state_manager_->setFeatureVisibility(MurMur3::calcHash("menu_bar_map_line2"), map_menu->getIsItems(std::size_t(11)));
        }

        void render() const override {
            if (!isVisible()) {
                return;
            }

            // 背景を描画
            paxg::Rect background_rect = getRect();
            background_rect.drawShadow({ 0, 1 }, 2, 1).draw(paxg::Color{ 243, 243, 243 });
            background_rect.drawFrame(1, 0, paxg::Color{ 128, 128, 128 });

            // メニューバーと言語選択を描画
            calculateLayout();
            menu_system.render();
            language_selector_.render();

            // GitHubボタンを描画
            github_button_.render();
        }

        void setVisible(bool visible) override { (void)visible; }
        bool isVisible() const override { return true; }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        /// @note ヘッダーパネルは常に最前面（Header）で描画される
        RenderLayer getLayer() const override {
            return RenderLayer::MenuBar;
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                0.f,
                0.f,
                static_cast<float>(paxg::Window::width()),
                getHeight()
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            (void)pos;
        }

        void setEnabled(bool enabled) override {
            (void)enabled;
        }

        bool isEnabled() const override {
            return true;
        }

        const char* getName() const override {
            return "MenuBar";
        }

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            if (menu_system.isHit(x, y)) return true;
            if (language_selector_.isHit(x, y)) return true;
            const paxg::Rect rect = getRect();
            return rect.contains(static_cast<float>(x), static_cast<float>(y));
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // メニューバーのマウス入力処理
            if (menu_system.isHit(event.x, event.y)) {
                return menu_system.handleEvent(event);
            }

            // 言語選択プルダウンのマウス入力処理
            if (language_selector_.isHit(event.x, event.y)) {
                return language_selector_.handleEvent(event);
            }

            // GitHubボタンのマウス入力処理
            if (github_button_.isHit(event.x, event.y)) {
                return github_button_.handleEvent(event);
            }

            // Headerの範囲内か
            if (getRect().contains(static_cast<float>(event.x), static_cast<float>(event.y))) {
                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::NotHandled();
        }

    private:
        /// @brief イベント購読を設定
        /// @brief Subscribe to events
        void subscribeToEvents() {
            // 言語変更イベントを購読してメニューの言語を更新
            EventBus::getInstance().subscribe<LanguageChangedEvent>(
                [this](const LanguageChangedEvent&) {
                    // メニュー幅のみ更新（レイアウトは render() で毎フレーム計算される）
                    menu_system.updateMenuWidth();
                }
            );
        }

        // 状態管理
        bool visible_ = true;
        AppStateManager* app_state_manager_ = nullptr;
        mutable std::size_t previous_language_index_ = 0;
        bool events_subscribed_ = false;

        // 子ウィジェット
        mutable paxs::Pulldown language_selector_;
        mutable paxs::MenuSystem menu_system;
        mutable paxs::GitHubButton github_button_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_HEADER_PANEL_HPP
