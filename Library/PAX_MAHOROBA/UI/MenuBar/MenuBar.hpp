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
#include <PAX_GRAPHICA/System.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

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

    /// @brief アプリ上部のUI（メニュー + 言語選択）を管理
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
        }

        void init() {
            // GitHubボタンを初期化
            github_button_.setLanguageSelector(&language_selector_);

            // メニューバーにメニュー項目を追加（FontSystem経由）
            menu_bar_.add(paxs::MenuBarKeys::VIEW_MENU_HASHES,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("view"));
            menu_bar_.add(paxs::MenuBarKeys::FEATURE_MENU_HASHES,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("place_names"));
            menu_bar_.add(paxs::MenuBarKeys::MAP_MENU_HASHES,
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                          static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                          MurMur3::calcHash("map"));

            calculateLayout();
        }

        /// @brief GitHubボタンの取得（MouseEventRouterへの登録用）
        /// @brief Get GitHub button (for MouseEventRouter registration)
        paxs::GitHubButton& getGitHubButton() {
            return github_button_;
        }

        /// @brief レイアウトを計算
        void calculateLayout() const {
            // 言語選択プルダウンを右端に配置
            language_selector_.setPos(paxg::Vec2i{
                static_cast<int>(paxg::Window::width() - language_selector_.getRect().w()),
                0
            });
        }

        /// @brief ヘッダーの高さを取得
        float getHeight() const {
            return language_selector_.getRect().h();
        }

        /// @brief メニューバーの取得（読み取り専用）
        const MenuSystem& getMenuSystem() const {
            return menu_bar_;
        }

        /// @brief メニューバーの取得（変更可能）- TileManager用
        MenuSystem& getMenuSystem() {
            return menu_bar_;
        }

        /// @brief 言語選択のインデックスを取得
        std::size_t getLanguageIndex() const {
            return language_selector_.getIndex();
        }

        /// @brief 言語選択のキーを取得
        std::uint_least32_t getLanguageKey() const {
            return language_selector_.getKey();
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
            paxs::DropDownMenu* view_menu = menu_bar_.getDropDownMenu(MurMur3::calcHash("view"));
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

        /// @brief メニュー状態を可視性に反映（毎フレーム呼び出し）
        void syncVisibilityFromMenu(paxs::FeatureVisibilityManager* visible_manager) {
            // View メニューの状態を同期
            paxs::DropDownMenu* view_menu = menu_bar_.getDropDownMenu(MurMur3::calcHash("view"));
            if (!view_menu) {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'view' menu not found.");
                return;
            }
            visible_manager->setVisibility(MurMur3::calcHash("Calendar"), view_menu->getIsItems(std::size_t(0)));
            visible_manager->setVisibility(MurMur3::calcHash("Map"), view_menu->getIsItems(std::size_t(1)));
            visible_manager->setVisibility(MurMur3::calcHash("UI"), view_menu->getIsItems(std::size_t(2)));
            visible_manager->setVisibility(MurMur3::calcHash("Simulation"), view_menu->getIsItems(std::size_t(3)));
            visible_manager->setVisibility(MurMur3::calcHash("License"), view_menu->getIsItems(std::size_t(4)));
            visible_manager->setVisibility(MurMur3::calcHash("Debug"), view_menu->getIsItems(std::size_t(5)));
            visible_manager->setVisibility(MurMur3::calcHash("3D"), view_menu->getIsItems(std::size_t(6)));

            // Place Names メニューの状態を同期
            paxs::DropDownMenu* place_names_menu = menu_bar_.getDropDownMenu(MurMur3::calcHash("place_names"));
            if (!place_names_menu) {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'place_names' menu not found.");
                return;
            }
            visible_manager->setVisibility(MurMur3::calcHash("place_name"), place_names_menu->getIsItems(std::size_t(0)));
            visible_manager->setVisibility(MurMur3::calcHash("site"), place_names_menu->getIsItems(std::size_t(1)));
            visible_manager->setVisibility(MurMur3::calcHash("tumulus"), place_names_menu->getIsItems(std::size_t(2)));
            visible_manager->setVisibility(MurMur3::calcHash("dolmen"), place_names_menu->getIsItems(std::size_t(3)));
            visible_manager->setVisibility(MurMur3::calcHash("kamekanbo"), place_names_menu->getIsItems(std::size_t(4)));
            visible_manager->setVisibility(MurMur3::calcHash("stone_coffin"), place_names_menu->getIsItems(std::size_t(5)));
            visible_manager->setVisibility(MurMur3::calcHash("doken"), place_names_menu->getIsItems(std::size_t(6)));
            visible_manager->setVisibility(MurMur3::calcHash("dotaku"), place_names_menu->getIsItems(std::size_t(7)));
            visible_manager->setVisibility(MurMur3::calcHash("bronze_mirror"), place_names_menu->getIsItems(std::size_t(8)));
            visible_manager->setVisibility(MurMur3::calcHash("human_bone"), place_names_menu->getIsItems(std::size_t(9)));
            visible_manager->setVisibility(MurMur3::calcHash("mtdna"), place_names_menu->getIsItems(std::size_t(10)));
            visible_manager->setVisibility(MurMur3::calcHash("ydna"), place_names_menu->getIsItems(std::size_t(11)));

            // Map メニューの状態を同期
            paxs::DropDownMenu* map_menu = menu_bar_.getDropDownMenu(MurMur3::calcHash("map"));
            if (!map_menu) {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'map' menu not found.");
                return;
            }
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_base"), map_menu->getIsItems(std::size_t(0)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_land_and_sea"), map_menu->getIsItems(std::size_t(1)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_land_and_water"), map_menu->getIsItems(std::size_t(2)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_soil"), map_menu->getIsItems(std::size_t(3)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_soil_temperature"), map_menu->getIsItems(std::size_t(4)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_ryosei_country"), map_menu->getIsItems(std::size_t(5)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_ryosei_line"), map_menu->getIsItems(std::size_t(6)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_slope"), map_menu->getIsItems(std::size_t(7)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"), map_menu->getIsItems(std::size_t(8)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"), map_menu->getIsItems(std::size_t(9)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_line1"), map_menu->getIsItems(std::size_t(10)));
            visible_manager->setVisibility(MurMur3::calcHash("menu_bar_map_line2"), map_menu->getIsItems(std::size_t(11)));
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
            menu_bar_.render();
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
            if (menu_bar_.isHit(x, y)) return true;
            if (language_selector_.isHit(x, y)) return true;
            const paxg::Rect rect = getRect();
            return rect.contains(static_cast<float>(x), static_cast<float>(y));
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // メニューバーのマウス入力処理
            if (menu_bar_.isHit(event.x, event.y)) {
                return menu_bar_.handleEvent(event);
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
        // 状態管理
        bool visible_ = true;

        // 子ウィジェット
        mutable paxs::Pulldown language_selector_;
        mutable paxs::MenuSystem menu_bar_;
        mutable paxs::GitHubButton github_button_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_HEADER_PANEL_HPP
