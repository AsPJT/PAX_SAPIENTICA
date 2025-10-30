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
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/CalendarUILayout.hpp>
#include <PAX_MAHOROBA/IUIWidget.hpp>
#include <PAX_MAHOROBA/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/TouchStateManager.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif

namespace paxs {

    // デバッグ情報パネルを表示するクラス
    class DebugInfoPanel : public IUIWidget {
    public:
        // 初期化（LanguageFontsへの参照を設定）
        void init(paxs::LanguageFonts& fonts) {
            language_fonts_ = &fonts;
            visible_ = true;
        }

        // 可視性の設定・取得
        void setVisible(bool visible) { visible_ = visible; }
        bool isVisible() const { return visible_; }

        // マップ情報とシミュレーション統計を描画
        void renderMapAndSimulationInfo(
            MapViewport& map_viewport,
            int debug_start_y,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            const paxs::GraphicVisualizationList& visible
#ifdef PAXS_USING_SIMULATOR
            , std::unique_ptr<paxs::SettlementSimulator>& simulator
#endif
        ) {
            if (!visible_ || language_fonts_ == nullptr) return;

            // 暦描画フォントを指定
            paxg::Font* one_font = language_fonts_->getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
            if (one_font == nullptr) return;

            // マップの拡大率
            (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
            (*one_font).drawTopRight(((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                reinterpret_cast<const char*>(u8"拡大率") :
                "Zoom rate"),
                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y), paxg::Color(0, 0, 0));
            (*one_font).drawTopRight(std::to_string(map_viewport.getHeight()),
                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 30), paxg::Color(0, 0, 0));

#ifdef PAXS_USING_SIMULATOR
            if (simulator == nullptr) {
#else
            {
#endif
                if (visible[MurMur3::calcHash(8, "Simulation")]) {
                    (*one_font).drawTopRight(std::to_string(map_viewport.getCenterX()),
                        paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 60), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(map_viewport.getCenterY()),
                        paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 90), paxg::Color(0, 0, 0));
                }
            }
#ifdef PAXS_USING_SIMULATOR
            if (simulator != nullptr) {
                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                (*one_font).drawTopRight(
                    ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"人口数") :
                        "Population"),
                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 60), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::to_string(simulator->cgetPopulationNum()),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 60), paxg::Color(0, 0, 0));

                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                (*one_font).drawTopRight(
                    ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"集落数") :
                        "Settlements"),
                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 90), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::to_string(simulator->cgetSettlement()),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 90), paxg::Color(0, 0, 0));

                (*one_font).drawTopRight(
                    ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"渡来数") :
                        "Total Immigrants"),
                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 120), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::to_string(simulator->emigrationSize()),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 120), paxg::Color(0, 0, 0));

                (*one_font).drawTopRight(
                    ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"全ての処理時間（秒）") :
                        "All Processing Time [s]"),
                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 150), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::to_string(simulator->cgetProcessingTime()),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 150), paxg::Color(0, 0, 0));

                (*one_font).drawTopRight(
                    ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"集団移動の処理時間（秒）") :
                        "Move Processing Time [s]"),
                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 180), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::to_string(simulator->cgetMoveProcessingTime()),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 180), paxg::Color(0, 0, 0));

                (*one_font).drawTopRight(
                    ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"婚姻の処理時間（秒）") :
                        "Marriage Processing Time [s]"),
                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 210), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::to_string(simulator->cgetMarriageProcessingTime()),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 210), paxg::Color(0, 0, 0));
            }
#endif
        }

        // 考古学的遺物の型式情報を描画
        void renderArchaeologicalInfo(
            const paxs::Koyomi& koyomi,
            const paxs::CalendarUILayout& ui_layout,
            int debug_start_y,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text
        ) {
            if (!visible_ || language_fonts_ == nullptr) return;

            // 暦描画フォントを指定
            paxg::Font* one_font = language_fonts_->getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
            if (one_font == nullptr) return;

            std::string dotaku = "";
            std::string doken = "";

            static std::array<int, 12> yayoi_year = { {
            -230, -190, -150, -100, -50, -20, 10, 40, 80, 120, 160, 200
            } };
            static std::array<std::string, 12> dotaku_name = { {
                    "",
                    reinterpret_cast<const char*>(u8"Ⅰ式　菱環紐1式"),
    reinterpret_cast<const char*>(u8"Ⅰ式　菱環紐2式"),
    reinterpret_cast<const char*>(u8"Ⅱ式　外縁付鈕1式"),
    reinterpret_cast<const char*>(u8"Ⅱ式　外縁付鈕2式"),
    reinterpret_cast<const char*>(u8"Ⅲ式　扁平鈕1式"),
    reinterpret_cast<const char*>(u8"Ⅲ式　扁平鈕2式"),
    reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕1式"),
    reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕2式"),
    reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕3式"),
    reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕4式"),
    reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕5式")
                        } };
            static std::array<std::string, 12> doken_name = { {
                    "",
                    reinterpret_cast<const char*>(u8"細形"),
    reinterpret_cast<const char*>(u8""),
    reinterpret_cast<const char*>(u8"中細形a類"),
    reinterpret_cast<const char*>(u8"中細形b類"),
    reinterpret_cast<const char*>(u8"中細形c類／平形Ⅰ"),
    reinterpret_cast<const char*>(u8"中広形／平形Ⅱ"),
    reinterpret_cast<const char*>(u8""),
    reinterpret_cast<const char*>(u8""),
    reinterpret_cast<const char*>(u8""),
    reinterpret_cast<const char*>(u8""),
    reinterpret_cast<const char*>(u8"")
                        } };

            std::string sueki_nakamura = "";
            std::string sueki_tanabe = "";

            static std::array<int, 18> sueki_year = { {
            1380,1390,1410,1430,1440,1450,1460,1470,1490,1500,1520,1530,1550,1560,1590,1620,1645,1670
            } };
            static std::array<std::string, 18> sueki_name = { {
    "","TG232","TK73","TK216","ON46","ON46/TK208","TK208","TK23","TK23/TK47","TK47",
    "MT15","TK10","TK10/MT85","MT85","TK43","TK209",reinterpret_cast<const char*>(u8"TK217古"),reinterpret_cast<const char*>(u8"TK217新")
                        } };
            static std::array<std::string, 18> sueki_nakamura_name = { {
    "",reinterpret_cast<const char*>(u8"I-1前"),reinterpret_cast<const char*>(u8"I-1後"),"I-2","I-3","I-3","I-3","I-4","I-4/I-5","I-5",
    "II-1","II-2","II-2/II-3","II-3","II-4","II-5","II-6","III-1"
                        } };
#ifndef PAXS_USING_SIMULATOR
            {
                int date_year = 0;
                std::visit([&](const auto& x) { date_year = int(x.cgetYear()); }, koyomi.date_list[1].date);
                for (std::size_t i = 0; i < sueki_year.size(); ++i) {
                    if (date_year < sueki_year[i]) {
                        sueki_tanabe = sueki_name[i];
                        sueki_nakamura = sueki_nakamura_name[i];
                        break;
                    }
                }
                dotaku = "";
                for (std::size_t i = 0; i < yayoi_year.size(); ++i) {
                    if (date_year < yayoi_year[i]) {
                        dotaku = dotaku_name[i];
                        doken = doken_name[i];
                        break;
                    }
                }
            }
            if (dotaku.size() != 0) {
                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                (*one_font).draw(reinterpret_cast<const char*>(u8"銅鐸 型式"),
                    paxg::Vec2i(ui_layout.rect_start_x + 10, debug_start_y + 140), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::string(dotaku),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 140), paxg::Color(0, 0, 0));
            }
            if (doken.size() != 0) {
                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                (*one_font).draw(reinterpret_cast<const char*>(u8"銅剣 型式"),
                    paxg::Vec2i(ui_layout.rect_start_x + 10, debug_start_y + 170), paxg::Color(0, 0, 0));
                (*one_font).drawTopRight(
                    std::string(doken),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 170), paxg::Color(0, 0, 0));
            }

            const std::string* sueki_nakamura_str = language_text.getStringPtr(koyomi.sueki_nakamura_key, select_language.cgetKey());
            if (sueki_nakamura.size() != 0) {
                if (sueki_nakamura_str != nullptr) {
                    (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                    (*one_font).draw(*sueki_nakamura_str,
                        paxg::Vec2i(ui_layout.rect_start_x + 10, debug_start_y + 140), paxg::Color(0, 0, 0));
                }
                (*one_font).drawTopRight(
                    std::string(sueki_nakamura),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 140), paxg::Color(0, 0, 0));
            }
            if (sueki_tanabe.size() != 0) {
                const std::string* sueki_tanabe_str = language_text.getStringPtr(koyomi.sueki_tanabe_key, select_language.cgetKey());
                if (sueki_nakamura_str != nullptr) {
                    (*one_font).draw(*sueki_tanabe_str,
                        paxg::Vec2i(ui_layout.rect_start_x + 10, debug_start_y + 170), paxg::Color(0, 0, 0));
                }
                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                (*one_font).drawTopRight(
                    std::string(sueki_tanabe),
                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 170), paxg::Color(0, 0, 0));
            }
#endif
        }

    private:
        paxs::LanguageFonts* language_fonts_ = nullptr;
        bool visible_ = true;
        bool enabled_ = true;
        paxg::Vec2i pos_{0, 0};

    public:
        // IUIWidget インターフェースの実装
        void update(paxs::TouchStateManager& tm) override {
            // DebugInfoPanelは入力処理を行わないため、空実装
            (void)tm;
        }

        void draw() override {
            // DebugInfoPanelは2つのrenderメソッドがあるため、
            // UIManagerから直接render呼び出しを継続
            // draw()は空実装（将来的な統合のためのプレースホルダー）
        }

        paxg::Rect getRect() const override {
            // デバッグパネルの領域を返す
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(pos_.y()),
                static_cast<float>(paxg::Window::width()),
                300.0f // おおよその高さ
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
        }

        // setVisible/isVisibleは既に実装済み（互換性維持）
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }
    };

}

#endif // !PAX_MAHOROBA_DEBUG_INFO_PANEL_HPP
