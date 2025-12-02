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

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/InteractiveUIComponent.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/GitHubButton.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/MenuBarToggleButton.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/MenuSystem.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>

#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief アプリ上部のメニューバー
    class MenuBar : public InteractiveUIComponent {
    private:
        static constexpr int github_button_margin = 8;

    public:
        MenuBar(const paxs::FeatureVisibilityManager& visible_manager) :
            language_selector_(paxs::MurMur3::calcHash("Language"),
                paxg::Vec2i{ 3000, 0 },
                paxs::PulldownDisplayType::SelectedValue,
                true
            ) {
            subscribeToEvents();

            // Locales から登録されている言語リストを取得
            language_selector_.setItemsKey(Fonts().getOrderedLocales());

            github_button_.init(language_selector_);

            // 言語選択のコールバックを設定（キーベース）
            language_selector_.setOnSelectionChanged([this](std::uint_least32_t key, bool is_selected) {
                (void)is_selected;
                handleLanguageChanged(key);
                });

            // メニューバーにメニュー項目を追加（FontSystem経由）
            menu_system.add(paxs::MenuBarKeys::view_menu_hashes,
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                MenuBarType::view);
            menu_system.add(paxs::MenuBarKeys::place_names_menu_hashes,
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                MenuBarType::place_names);
            menu_system.add(paxs::MenuBarKeys::item_menu_hashes,
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                MenuBarType::item);
            menu_system.add(paxs::MenuBarKeys::structure_menu_hashes,
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                MenuBarType::structure);
            menu_system.add(paxs::MenuBarKeys::genome_menu_hashes,
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                MenuBarType::genomes);
            menu_system.add(paxs::MenuBarKeys::map_menu_hashes,
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                MenuBarType::map);

            // FeatureVisibilityManagerから初期状態を読み込んでメニューに反映
            initializeMenuFromVisibility(visible_manager);

            // 各メニューにコールバックを設定
            setupMenuCallbacks();

            calculateLayout();
            menu_system.updateMenuWidth();
        }

        /// @brief ヘッダーの高さを取得
        int getHeight() const {
            return language_selector_.getRect().height();
        }

        /// @brief 言語変更時のハンドラー（コールバック駆動、キーベース）
        /// @brief Language change handler (callback-driven, key-based)
        /// @param language_key 選択された言語のキー / Selected language key
        void handleLanguageChanged(std::uint_least32_t language_key) {
            // EventBus経由で言語変更コマンドを発行（キーのみ）
            paxs::EventBus::getInstance().publish(LanguageChangeCommandEvent(language_key));
        }

        /// @brief メニュー項目トグル時のハンドラー（コールバック駆動）
        void handleMenuItemToggled(const paxs::MenuBarType menu_type, std::size_t item_index, bool is_checked) {
            // item_indexはDropDownMenuの内部インデックス（0はヘッダー、1以降が実際の項目）
            // 実際の項目インデックスに変換するため-1する
            const std::size_t actual_index = (item_index > 0) ? (item_index - 1) : 0;

            if (menu_type == paxs::MenuBarType::view) {
                if (actual_index < paxs::MenuBarKeys::view_menu_items.size()) {
                    paxs::EventBus::getInstance().publish(FeatureVisibilityChangeCommandEvent(
                        static_cast<std::uint_least32_t>(paxs::MenuBarKeys::view_menu_items[actual_index]),
                        is_checked
                    ));
                }
            }
            else if (menu_type == paxs::MenuBarType::place_names) {
                if (actual_index < paxs::MenuBarKeys::place_names_menu_items.size()) {
                    paxs::EventBus::getInstance().publish(FeatureVisibilityChangeCommandEvent(
                        static_cast<std::uint_least32_t>(paxs::MenuBarKeys::place_names_menu_items[actual_index]),
                        is_checked
                    ));
                }
            }
            else if (menu_type == paxs::MenuBarType::item) {
                if (actual_index < paxs::MenuBarKeys::item_menu_items.size()) {
                    paxs::EventBus::getInstance().publish(FeatureVisibilityChangeCommandEvent(
                        static_cast<std::uint_least32_t>(paxs::MenuBarKeys::item_menu_items[actual_index]),
                        is_checked
                    ));
                }
            }
            else if (menu_type == paxs::MenuBarType::structure) {
                if (actual_index < paxs::MenuBarKeys::structure_menu_items.size()) {
                    paxs::EventBus::getInstance().publish(FeatureVisibilityChangeCommandEvent(
                        static_cast<std::uint_least32_t>(paxs::MenuBarKeys::structure_menu_items[actual_index]),
                        is_checked
                    ));
                }
            }
            else if (menu_type == paxs::MenuBarType::genomes) {
                if (actual_index < paxs::MenuBarKeys::genome_menu_items.size()) {
                    paxs::EventBus::getInstance().publish(FeatureVisibilityChangeCommandEvent(
                        static_cast<std::uint_least32_t>(paxs::MenuBarKeys::genome_menu_items[actual_index]),
                        is_checked
                    ));
                }
            }
            else if (menu_type == paxs::MenuBarType::map) {
                if (actual_index < paxs::MenuBarKeys::map_menu_items.size()) {
                    paxs::EventBus::getInstance().publish(FeatureVisibilityChangeCommandEvent(
                        static_cast<std::uint_least32_t>(paxs::MenuBarKeys::map_menu_items[actual_index]),
                        is_checked
                    ));
                }
            }
        }

        /// @brief メニューコールバックをセットアップ
        void setupMenuCallbacks() {
            // View メニューのコールバック
            paxs::DropDownMenu* view_menu = menu_system.getDropDownMenu(paxs::MenuBarType::view);
            if (view_menu != nullptr) {
                view_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(paxs::MenuBarType::view, index, is_checked);
                    });
            }

            // Place Names メニューのコールバック
            paxs::DropDownMenu* place_names_menu = menu_system.getDropDownMenu(paxs::MenuBarType::place_names);
            if (place_names_menu != nullptr) {
                place_names_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(paxs::MenuBarType::place_names, index, is_checked);
                    });
            }

            // Item メニューのコールバック
            paxs::DropDownMenu* item_menu = menu_system.getDropDownMenu(paxs::MenuBarType::item);
            if (item_menu != nullptr) {
                item_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(paxs::MenuBarType::item, index, is_checked);
                    });
            }

            // Structure メニューのコールバック
            paxs::DropDownMenu* structure_menu = menu_system.getDropDownMenu(paxs::MenuBarType::structure);
            if (structure_menu != nullptr) {
                structure_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(paxs::MenuBarType::structure, index, is_checked);
                    });
            }

            // Genome メニューのコールバック
            paxs::DropDownMenu* genome_menu = menu_system.getDropDownMenu(paxs::MenuBarType::genomes);
            if (genome_menu != nullptr) {
                genome_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(paxs::MenuBarType::genomes, index, is_checked);
                    });
            }

            // Map メニューのコールバック
            paxs::DropDownMenu* map_menu = menu_system.getDropDownMenu(paxs::MenuBarType::map);
            if (map_menu != nullptr) {
                map_menu->setOnItemToggled([this](std::size_t index, bool is_checked) {
                    handleMenuItemToggled(paxs::MenuBarType::map, index, is_checked);
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

        Rect<int> getRect() const override {
            return { 0, 0,
                paxg::Window::width(),
                getHeight()
            };
        }

        bool isHit(const paxs::Vector2<int>& pos) const override {
            if (!isVisible()) return false;
            if (getRect().contains(pos)) return true;
            if (menu_system.isHit(pos)) return true;
            return language_selector_.isHit(pos);
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // メニューバーのマウス入力処理
            if (menu_system.isHit(event.pos)) {
                return menu_system.handleEvent(event);
            }

            // 言語選択プルダウンのマウス入力処理
            if (language_selector_.isHit(event.pos)) {
                return language_selector_.handleEvent(event);
            }

            // GitHubボタンのマウス入力処理
            if (github_button_.isHit(event.pos)) {
                return github_button_.handleEvent(event);
            }

            // Headerの範囲内か
            if (getRect().contains(event.pos)) {
                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::NotHandled();
        }

        RenderLayer getLayer() const override { return RenderLayer::MenuBar; }
        bool isVisible() const override { return true; }
        void setPos(const Vector2<int>& /*pos*/) override {}
        const char* getName() const override { return "MenuBar"; }

    private:
        paxs::Pulldown language_selector_;
        paxs::MenuSystem menu_system;
        paxs::GitHubButton github_button_;

        /// @brief イベント購読を設定
        /// @brief Subscribe to events
        void subscribeToEvents() {
            // 言語変更イベントを購読してメニューの言語を更新
            EventBus::getInstance().subscribe<LanguageChangedEvent>(
                [this](const LanguageChangedEvent&) {
                    menu_system.updateMenuWidth();
                    language_selector_.updateLayout();  // 言語選択プルダウンのレイアウトを更新
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

        /// @brief レイアウトを計算
        void calculateLayout() {
            // 言語選択プルダウンを右端に配置
            language_selector_.setPos(Vector2<int>{
                static_cast<int>(paxg::Window::width() - language_selector_.getRect().width()),
                    0
            });
            github_button_.setPos(Vector2<int>{
                language_selector_.getRect().x() - github_button_.getRect().width() - github_button_margin,
                    (language_selector_.getRect().height() - github_button_.getRect().height()) / 2
            });
        }

        /// @brief FeatureVisibilityManagerからメニューの初期状態を読み込む
        /// @brief Load initial menu state from FeatureVisibilityManager
        void initializeMenuFromVisibility(const paxs::FeatureVisibilityManager& visible_manager) {
            // View メニューの状態を初期化
            paxs::DropDownMenu* view_menu = menu_system.getDropDownMenu(paxs::MenuBarType::view);
            if (view_menu != nullptr) {
                for (std::size_t i = 0; i < paxs::MenuBarKeys::view_menu_items.size(); ++i) {
                    view_menu->setIsItems(i, visible_manager.isVisible(paxs::MenuBarKeys::view_menu_items[i]));
                }
            }
            else {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'view' menu not found.");
            }

            // Map メニューの状態を初期化
            paxs::DropDownMenu* map_menu = menu_system.getDropDownMenu(paxs::MenuBarType::map);
            if (map_menu != nullptr) {
                for (std::size_t i = 0; i < paxs::MenuBarKeys::map_menu_items.size(); ++i) {
                    map_menu->setIsItems(i, visible_manager.isVisible(paxs::MenuBarKeys::map_menu_items[i]));
                }
            }
            else {
                PAXS_WARNING("MenuBar::initializeMenuFromVisibility: 'map' menu not found.");
            }
        }

    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_HEADER_PANEL_HPP
