/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_PRESENTER_HPP
#define PAX_MAHOROBA_MAP_PRESENTER_HPP

#include <memory>

#include <PAX_MAHOROBA/LocationPoint.hpp>
#include <PAX_MAHOROBA/MapRenderer.hpp>
#include <PAX_MAHOROBA/MapViewport.hpp>
#include <PAX_MAHOROBA/PersonLocation.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/Map/MapModel.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Simulation/AgentLocation.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

namespace paxs {

// 前方宣言
class UIManager;
class Koyomi;

/// @brief 地図のプレゼンター（ModelとRendererの調整役）
/// @brief Map Presenter (Coordinator between Model and Renderer)
class MapPresenter {
public:
    MapPresenter() = default;

    /// @brief 初期化
    /// @brief Initialize
    void init() {
        // LocationPoint の初期化は後で必要になったら追加
    }

    /// @brief UIManagerへの参照を設定
    /// @brief Set reference to UIManager
    /// @param ui_manager UIManager
    void setUIManager(UIManager* ui_manager) {
        renderer_.setUIManager(ui_manager);
    }

    /// @brief 更新処理（描画含む）
    /// @brief Update and render
    /// @param map_viewport 地図ビューポート
    /// @param select_language 選択言語
    /// @param koyomi カレンダー
    /// @param ui_manager UIマネージャー
    /// @param simulator シミュレーター（オプション）
    /// @param visible 可視性リスト
    void update(
        MapViewport& map_viewport,
        const SelectLanguage& select_language,
        const paxs::Koyomi& koyomi,
        paxs::UIManager& ui_manager,
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator>& simulator,
        AgentLocation& agent_location,
#endif
        PlaceNameLocation& place_name_location,
        PersonNameLocation& person_name_location,
        GraphicVisualizationList& visible
    ) {
        // Modelで計算された値を取得
        const double width = map_viewport.getWidth();
        const double height = map_viewport.getHeight();
        const double center_x = map_viewport.getCenterX();
        const double center_y = map_viewport.getCenterY();
        const double julian_day = koyomi.jdn.cgetDay();

        // フォントを指定（SelectLanguageに応じた主フォント）
        paxg::Font* main_font = ui_manager.string_viewer.language_fonts.getAndAdd(
            select_language.cgetKey(),
            static_cast<std::uint_least8_t>(ui_manager.koyomi_font_size),
            static_cast<std::uint_least8_t>(ui_manager.koyomi_font_buffer_thickness_size)
        );

        // 地名を描画
        renderer_.drawPlaceNames(
            place_name_location,
            visible,
            julian_day,
            width,
            height,
            center_x,
            center_y,
            (main_font == nullptr) ? ui_manager.string_viewer.pin_font : (*main_font),
            ui_manager.string_viewer.en_font,
            ui_manager.string_viewer.pin_font
        );

        // 人名を描画
        renderer_.drawPersonNames(
            person_name_location,
            julian_day,
            width,
            height,
            center_x,
            center_y,
            (main_font == nullptr) ? ui_manager.string_viewer.pin_font : (*main_font),
            ui_manager.string_viewer.en_font,
            ui_manager.string_viewer.pin_font
        );

#ifdef PAXS_USING_SIMULATOR
        if (simulator) {
            // エージェントを描画
            renderer_.drawAgents(
                agent_location,
                julian_day,
                simulator->getSettlementGrids(),
                simulator->getMarriagePosList(),
                width,
                height,
                center_x,
                center_y
            );

            // エージェントのテキストを描画
            renderer_.drawAgentText(agent_location);
        }
#endif
    }

private:
    paxs::map::MapModel model_;   ///< ドメインモデル（ロジック）
    MapRenderer renderer_;         ///< レンダラー（描画）
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_PRESENTER_HPP
