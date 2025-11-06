/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP
#define PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include <PAX_MAHOROBA/Map/Location/SettlementRenderer.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/Simulation/SettlementGrid.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief 集落の描画を管理するクラス
    /// @brief Class to manage settlement rendering
    class SettlementManager : public IRenderable {
    private:
        bool visible_ = true;

        double cached_jdn_ = 0.0;
        const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* cached_agents_ = nullptr;
        const std::vector<GridType4>* cached_marriage_pos_list_ = nullptr;
        double cached_map_view_width_ = 0.0;
        double cached_map_view_height_ = 0.0;
        double cached_map_view_center_x_ = 0.0;
        double cached_map_view_center_y_ = 0.0;
        std::size_t select_draw_ = 1;
        bool is_line_ = false;
        bool is_arrow_ = true;


    public:
        SettlementManager() = default;

        void render() const override {
            if (!visible_) return;
            if (!cached_agents_ || !cached_marriage_pos_list_) return;
            SettlementRenderer::draw(cached_jdn_, cached_agents_, cached_marriage_pos_list_,
                cached_map_view_width_, cached_map_view_height_,
                cached_map_view_center_x_, cached_map_view_center_y_,
                select_draw_, is_line_, is_arrow_);
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }
        bool isVisible() const override {
            return visible_;
        }
        void setVisible(bool visible) override {
            visible_ = visible;
        }

        /// @brief 描画パラメータを設定
        void setDrawParams(
            const double jdn,
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>& agents,
            const std::vector<GridType4>& marriage_pos_list,
            const double map_view_width, const double map_view_height,
            const double map_view_center_x, const double map_view_center_y,
            const std::size_t select_draw, const bool is_line, const bool is_arrow
        ) {
            cached_jdn_ = jdn;
            cached_agents_ = &agents;
            cached_marriage_pos_list_ = &marriage_pos_list;
            cached_map_view_width_ = map_view_width;
            cached_map_view_height_ = map_view_height;
            cached_map_view_center_x_ = map_view_center_x;
            cached_map_view_center_y_ = map_view_center_y;
            select_draw_ = select_draw;
            is_line_ = is_line;
            is_arrow_ = is_arrow;
        }
    };
}

#endif // !PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP
