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
        std::unique_ptr<SettlementRenderer> renderer_;

    public:
        SettlementManager()
            : renderer_(std::make_unique<SettlementRenderer>())
        {
            if (!renderer_) {
                PAXS_ERROR("Failed to allocate SettlementRenderer");
            }
        }

        void render() const override {
            if (!visible_ || !renderer_) return;
            renderer_->render();
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }

        bool isVisible() const override {
            return visible_;
        }

        void setVisible(bool visible) override {
            visible_ = visible;
            if (renderer_) {
                renderer_->setVisible(visible);
            }
        }

        /// @brief 描画パラメータを設定
        void setDrawParams(
            double jdn,
            paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>& agents,
            const std::vector<GridType4>& marriage_pos_list,
            double map_view_width, double map_view_height,
            double map_view_center_x, double map_view_center_y,
            std::size_t select_draw, bool is_line, bool is_arrow
        ) {
            if (!renderer_) return;

            renderer_->setDrawParams(
                jdn,
                agents,
                marriage_pos_list,
                map_view_width, map_view_height,
                map_view_center_x, map_view_center_y,
                select_draw, is_line, is_arrow
            );
        }
    };
}

#endif // !PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP
