/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
#define PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP

#include <array>
#include <cmath>
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif

namespace paxs {

    /// @brief デバッグ情報パネルを表示するクラス
    /// @brief Debug information panel class
    class DebugInfoPanel : public IWidget {
    private:
        paxs::LanguageFonts* language_fonts_ = nullptr;
        PanelBackground background_;  // 背景
        bool visible_ = true;
        bool enabled_ = true;
        paxg::Vec2i pos_{10, 0};  // 左下の位置（Y座標は後で設定）
        int panel_width_ = 300;   // パネル幅
        int panel_height_ = 250;  // パネル高さ

    public:
        /// @brief 初期化（LanguageFontsへの参照を設定）
        /// @brief Initialize (set reference to LanguageFonts)
        void init(paxs::LanguageFonts& fonts) {
            language_fonts_ = &fonts;
            visible_ = true;

            // 背景を初期化（左下に配置）
            updateBackgroundPosition();
        }

        /// @brief 背景の位置を更新
        /// @brief Update background position
        void updateBackgroundPosition() {
            int y = paxg::Window::height() - panel_height_ - 10;  // 左下
            pos_ = paxg::Vec2i(pos_.x(), y);
        }

        // IWidget インターフェースの実装（コンポーネント情報）
        const char* getName() const override {
            return "DebugInfoPanel";
        }

        /// @brief レンダリングレイヤーを取得（背景レイヤー）
        /// @brief Get rendering layer (background layer)
        RenderLayer getLayer() const override {
            return RenderLayer::UIBackground;
        }

        bool isAvailable() const override {
            return true;
        }

        // setEnabled/isEnabledは下部で実装済み

        // 可視性の設定・取得
        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        /// @brief マップ情報とシミュレーション統計を描画
        /// @brief Render map information and simulation statistics
        void renderMapAndSimulationInfo(
            MapViewport& map_viewport,
            const SelectLanguage& select_language,
            const paxs::Language& language_text
#ifdef PAXS_USING_SIMULATOR
            , std::unique_ptr<paxs::SettlementSimulator>& simulator
#endif
            , const paxs::Koyomi* koyomi = nullptr
            , bool is_simulator_active = false
        ) {
            if (!visible_ || language_fonts_ == nullptr) return;

            // 背景位置を更新（ウィンドウサイズ変更に対応）
            updateBackgroundPosition();

            // フォントを取得
            paxg::Font* font = language_fonts_->getAndAdd(
                select_language.cgetKey(),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
            );
            if (font == nullptr) return;

            font->setOutline(0, 0.6, paxg::Color(255, 255, 255));

            int text_x = pos_.x() + 15;  // パネル内の左端
            int text_y = pos_.y() + 15;  // パネル内の上端
            int line_height = 25;
            int current_line = 0;

            // タイトル
            font->draw(
                (select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"デバッグ情報") : "Debug Info",
                paxg::Vec2i(text_x, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // マップの拡大率
            font->draw(
                (select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"拡大率: ") : "Zoom: ",
                paxg::Vec2i(text_x, text_y + line_height * current_line),
                paxg::Color(0, 0, 0)
            );
            font->draw(
                std::to_string(map_viewport.getHeight()),
                paxg::Vec2i(text_x + 100, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // XYZ Tiles Z拡大率
            const int z_magnification = static_cast<int>(-std::log2(map_viewport.getHeight()) + 12.5);
            const std::string* xyz_label_ptr = language_text.getStringPtr(
                MurMur3::calcHash("debug_xyz_tiles_z"),
                select_language.cgetKey()
            );
            if (xyz_label_ptr != nullptr) {
                font->draw(
                    (*xyz_label_ptr + ":").c_str(),
                    paxg::Vec2i(text_x, text_y + line_height * current_line),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(z_magnification),
                paxg::Vec2i(text_x + 130, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

#ifdef PAXS_USING_SIMULATOR
            if (simulator != nullptr) {
                // 人口数
                font->draw(
                    (select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"人口: ") : "Population: ",
                    paxg::Vec2i(text_x, text_y + line_height * current_line),
                    paxg::Color(0, 0, 0)
                );
                font->draw(
                    std::to_string(simulator->cgetPopulationNum()),
                    paxg::Vec2i(text_x + 130, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );

                // 集落数
                font->draw(
                    (select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"集落: ") : "Settlements: ",
                    paxg::Vec2i(text_x, text_y + line_height * current_line),
                    paxg::Color(0, 0, 0)
                );
                font->draw(
                    std::to_string(simulator->cgetSettlement()),
                    paxg::Vec2i(text_x + 130, text_y + line_height * current_line++),
                    paxg::Color(0, 0, 0)
                );
            }
#endif

            // 大きな年号を描画（シミュレーション非アクティブ時のみ）
            if (koyomi != nullptr && !is_simulator_active && !koyomi->date_list.empty()) {
                // グレゴリオ暦の年を取得（date_list[1]がグレゴリオ暦）
                if (koyomi->date_list.size() > 1) {
                    int date_year = 0;
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                    }, koyomi->date_list[1].date);

                    if (date_year > 0) {
                        // 大きな年号フォントを取得（通常の3倍サイズ）
                        paxg::Font* big_year_font = language_fonts_->getAndAdd(
                            select_language.cgetKey(),
                            static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE * 3),
                            static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
                        );
                        if (big_year_font != nullptr) {
                            big_year_font->setOutline(0, 0.6, paxg::Color(255, 255, 255));

                            // パネル内の下部に配置
                            int big_year_x = text_x;
                            int big_year_y = pos_.y() + panel_height_ - 80;  // パネル下部から80px上

                            big_year_font->draw(
                                std::to_string(date_year),
                                paxg::Vec2i(big_year_x, big_year_y),
                                paxg::Color(0, 0, 0)
                            );
                        }
                    }
                }
            }
        }

    public:
        // IWidget インターフェースの実装
        bool handleInput(const InputEvent& event) override {
            // DebugInfoPanelは入力処理を行わない
            (void)event;
            return false;
        }

        void render() override {
            if (!visible_) return;

            // 背景を描画（バッチ描画に登録）
            background_.draw(
                pos_.x(), pos_.y(),
                panel_width_, panel_height_,
                10,  // 角丸半径
                paxg::Color{255, 255, 255},  // 背景色
                RenderLayer::UIBackground
            );
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(pos_.y()),
                static_cast<float>(panel_width_),
                static_cast<float>(panel_height_)
            };
        }

        /// @brief 背景を取得（UILayerのバッチ描画用）
        /// @brief Get background (for UILayer batch rendering)
        PanelBackground& getBackground() {
            return background_;
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
        }

        // setEnabled/isEnabledは下部で実装済み
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }
    };

}

#endif // !PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
