/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_HEADER_PANEL_HPP
#define PAX_MAHOROBA_UI_HEADER_PANEL_HPP

#include <array>

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
    private:
        // UI配置定数
        static constexpr int github_button_margin = 8;

    public:
        /// @brief コンストラクタ
        MenuBar() :
            language_selector_(
                paxs::LanguageDomain::UI,
                paxg::Vec2i{3000, 0},
                paxs::PulldownDisplayType::SelectedValue,
                true
            )
        {
            language_selector_.setItemsKey(paxs::LanguageKeys::ALL_LANGUAGE_HASHES);

            // GitHubボタンを初期化
            github_button_.init(language_selector_);

            // 言語選択のコールバックを設定
            language_selector_.setOnSelectionChanged([this](std::size_t index, bool is_selected) {
                (void)is_selected;
                handleLanguageChanged(index);
            });

            // メニューバーにメニュー項目を追加（FontSystem経由）
            menu_system.add(paxs::MenuBarKeys::view_menu_hashes,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("view"));
            menu_system.add(paxs::MenuBarKeys::feature_menu_hashes,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("place_names"));
            menu_system.add(paxs::MenuBarKeys::map_menu_hashes,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("map"));

            // 各メニューにコールバックを設定
            setupMenuCallbacks();

            calculateLayout();
            menu_system.updateMenuWidth();
        }

        /// @brief レイアウトを計算
        void calculateLayout() {
            // 言語選択プルダウンを右端に配置
            language_selector_.setPos(paxg::Vec2i{
                static_cast<int>(paxg::Window::width() - language_selector_.getRect().w()),
                0
            });
            github_button_.setPos(paxg::Vec2i{
                static_cast<int>(language_selector_.getRect().x() - github_button_.getRect().w() - github_button_margin),
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
            visible_manager->emplace(ViewMenu::calendar, true); // 暦
            visible_manager->emplace(ViewMenu::map, true); // 地図
            visible_manager->emplace(ViewMenu::ui, true); // UI
            visible_manager->emplace(ViewMenu::simulation, true); // シミュレーション
            visible_manager->emplace(ViewMenu::license, false); // ライセンス
            visible_manager->emplace(ViewMenu::debug, false); // デバッグ
            visible_manager->emplace(ViewMenu::view_3d, false); // 360度写真

            visible_manager->emplace(MapLayersMenu::land_and_water, false); // 陸水境界
            visible_manager->emplace(MapLayersMenu::soil, false); // 土壌
            visible_manager->emplace(MapLayersMenu::ryosei_line, true); // 陸生国境界線
            visible_manager->emplace(MapLayersMenu::slope, true); // 傾斜
            visible_manager->emplace(MapLayersMenu::line2, false); // 線2

            // View メニューの状態を初期化
            paxs::DropDownMenu* view_menu = menu_system.getDropDownMenu(MurMur3::calcHash("view"));
            if (!view_menu) {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'view' menu not found.");
                return;
            }
            view_menu->setIsItems(std::size_t(0), visible_manager->isVisible(ViewMenu::calendar));
            view_menu->setIsItems(std::size_t(1), visible_manager->isVisible(ViewMenu::map));
            view_menu->setIsItems(std::size_t(2), visible_manager->isVisible(ViewMenu::ui));
            view_menu->setIsItems(std::size_t(3), visible_manager->isVisible(ViewMenu::simulation));
            view_menu->setIsItems(std::size_t(4), visible_manager->isVisible(ViewMenu::license));
            view_menu->setIsItems(std::size_t(5), visible_manager->isVisible(ViewMenu::debug));
            view_menu->setIsItems(std::size_t(6), visible_manager->isVisible(ViewMenu::view_3d));

            // Map メニューの状態を初期化
            paxs::DropDownMenu* map_menu = menu_system.getDropDownMenu(MurMur3::calcHash("map"));
            if (!map_menu) {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'map' menu not found.");
                return;
            }
            map_menu->setIsItems(std::size_t(0), visible_manager->isVisible(MapLayersMenu::land_and_water));
            map_menu->setIsItems(std::size_t(1), visible_manager->isVisible(MapLayersMenu::soil));
            map_menu->setIsItems(std::size_t(2), visible_manager->isVisible(MapLayersMenu::ryosei_line));
            map_menu->setIsItems(std::size_t(3), visible_manager->isVisible(MapLayersMenu::slope));
            map_menu->setIsItems(std::size_t(4), visible_manager->isVisible(MapLayersMenu::line2));
        }

        /// @brief 言語変更時のハンドラー（コールバック駆動）
        /// @brief Language change handler (callback-driven)
        void handleLanguageChanged(std::size_t new_index) {
            if (!app_state_manager_) return;

            const std::uint_least32_t language_key = language_selector_.getKey();
            // AppStateManager経由で言語変更イベントを発行
            app_state_manager_->setLanguage(static_cast<std::uint_least8_t>(new_index), language_key);
        }

        /// @brief メニュー項目トグル時のハンドラー（コールバック駆動）
        /// @brief Menu item toggle handler (callback-driven)
        void handleMenuItemToggled(const std::uint_least32_t menu_key, std::size_t item_index, bool is_checked) {
            if (!app_state_manager_) return;

            // メニューキーに応じて適切な機能キーを取得し、可視性を更新
            // item_indexはDropDownMenuの内部インデックス（0はヘッダー、1以降が実際の項目）
            // 実際の項目インデックスに変換するため-1する
            const std::size_t actual_index = (item_index > 0) ? (item_index - 1) : 0;

            if (menu_key == MurMur3::calcHash("view")) {
                // View メニュー
                const std::array<ViewMenu, 7> view_items = {
                    ViewMenu::calendar,
                    ViewMenu::map,
                    ViewMenu::ui,
                    ViewMenu::simulation,
                    ViewMenu::license,
                    ViewMenu::debug,
                    ViewMenu::view_3d
                };
                if (actual_index < view_items.size()) {
                    app_state_manager_->setFeatureVisibility(
                        static_cast<std::uint_least32_t>(view_items[actual_index]), is_checked);
                }
            }
            else if (menu_key == MurMur3::calcHash("place_names")) {
                // Place Names メニュー
                const std::array<PlaceNamesMenu, 12> place_name_items = {
                    PlaceNamesMenu::place_name,
                    PlaceNamesMenu::site,
                    PlaceNamesMenu::tumulus,
                    PlaceNamesMenu::dolmen,
                    PlaceNamesMenu::kamekanbo,
                    PlaceNamesMenu::stone_coffin,
                    PlaceNamesMenu::doken,
                    PlaceNamesMenu::dotaku,
                    PlaceNamesMenu::bronze_mirror,
                    PlaceNamesMenu::human_bone,
                    PlaceNamesMenu::mtdna,
                    PlaceNamesMenu::ydna
                };
                if (actual_index < place_name_items.size()) {
                    app_state_manager_->setFeatureVisibility(
                        static_cast<std::uint_least32_t>(place_name_items[actual_index]), is_checked);
                }
            }
            else if (menu_key == MurMur3::calcHash("map")) {
                // Map メニュー
                const std::array<MapLayersMenu, 5> map_layer_items = {
                    MapLayersMenu::land_and_water,
                    MapLayersMenu::soil,
                    MapLayersMenu::ryosei_line,
                    MapLayersMenu::slope,
                    MapLayersMenu::line2
                };
                if (actual_index < map_layer_items.size()) {
                    app_state_manager_->setFeatureVisibility(
                        static_cast<std::uint_least32_t>(map_layer_items[actual_index]), is_checked);
                }
            }
        }

        /// @brief メニューコールバックをセットアップ
        /// @brief Setup menu callbacks
        void setupMenuCallbacks() {
            // View メニューのコールバック
            paxs::DropDownMenu* view_menu = menu_system.getDropDownMenu(MurMur3::calcHash("view"));
            if (view_menu) {
                view_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(MurMur3::calcHash("view"), index, is_checked);
                });
            }

            // Place Names メニューのコールバック
            paxs::DropDownMenu* place_names_menu = menu_system.getDropDownMenu(MurMur3::calcHash("place_names"));
            if (place_names_menu) {
                place_names_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(MurMur3::calcHash("place_names"), index, is_checked);
                });
            }

            // Map メニューのコールバック
            paxs::DropDownMenu* map_menu = menu_system.getDropDownMenu(MurMur3::calcHash("map"));
            if (map_menu) {
                map_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(MurMur3::calcHash("map"), index, is_checked);
                });
            }
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
            menu_system.render();
            language_selector_.render();

            // GitHubボタンを描画
            github_button_.render();
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                0.f,
                0.f,
                static_cast<float>(paxg::Window::width()),
                getHeight()
            };
        }

        bool isHit(int x, int y) const override {
            if (!isVisible()) return false;
            if (getRect().contains(static_cast<float>(x), static_cast<float>(y))) return true;
            if (menu_system.isHit(x, y)) return true;
            return language_selector_.isHit(x, y);
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

        RenderLayer getLayer() const override { return RenderLayer::MenuBar; }
        bool isVisible() const override { return true; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
        const char* getName() const override { return "MenuBar"; }

    private:
        /// @brief イベント購読を設定
        /// @brief Subscribe to events
        void subscribeToEvents() {
            // 言語変更イベントを購読してメニューの言語を更新
            EventBus::getInstance().subscribe<LanguageChangedEvent>(
                [this](const LanguageChangedEvent&) {
                    menu_system.updateMenuWidth();
                    calculateLayout();
                }
            );

            // ウィンドウリサイズイベントを購読してレイアウトを再計算
            EventBus::getInstance().subscribe<WindowResizedEvent>(
                [this](const WindowResizedEvent&) {
                    calculateLayout();
                }
            );
        }

        // 状態管理
        bool visible_ = true;
        AppStateManager* app_state_manager_ = nullptr;
        std::size_t previous_language_index_ = 0;
        bool events_subscribed_ = false;

        // 子ウィジェット
        paxs::Pulldown language_selector_;
        paxs::MenuSystem menu_system;
        paxs::GitHubButton github_button_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_HEADER_PANEL_HPP
