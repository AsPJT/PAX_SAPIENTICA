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
#include <PAX_SAPIENTICA/UnorderedMap.hpp>

namespace paxs {

    /// @brief 集落の描画を管理するクラス (Application Layer)
    /// @brief Class to manage settlement rendering (Application Layer)
    ///
    /// PersonNameManager/PlaceNameManagerと同じ構造で統一
    /// Unified structure with PersonNameManager/PlaceNameManager
    class SettlementManager : public IRenderable {
    public:
        SettlementManager()
            : renderer_(std::make_unique<SettlementRenderer>())
        {
            if (!renderer_) {
                PAXS_ERROR("Failed to allocate SettlementRenderer");
            }
        }

        // IRenderable の実装
        // IRenderable implementation

        /// @brief レンダリング処理
        /// @brief Render
        void render() override {
            if (!visible_) return;
            if (!renderer_) return;

            // 描画処理をレンダラーに委譲
            renderer_->render();
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }

        /// @brief 可視性を取得
        /// @brief Get visibility
        bool isVisible() const override {
            return visible_;
        }

        /// @brief 可視性を設定
        /// @brief Set visibility
        void setVisible(bool visible) override {
            visible_ = visible;
            if (renderer_) {
                renderer_->setVisible(visible);
            }
        }

        /// @brief 描画パラメータを設定(MapContentLayer から呼び出される)
        /// @brief Set drawing parameters (called from MapContentLayer)
        void setDrawParams(
            double jdn,
            paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>& agents,
            const std::vector<GridType4>& marriage_pos_list,
            double map_view_width, double map_view_height,
            double map_view_center_x, double map_view_center_y,
            std::size_t select_draw, bool is_line, bool is_arrow
        ) {
            if (!renderer_) return;

            // レンダラーに描画パラメータを渡す
            renderer_->setDrawParams(
                jdn,
                agents,
                marriage_pos_list,
                map_view_width, map_view_height,
                map_view_center_x, map_view_center_y,
                select_draw, is_line, is_arrow
            );
        }

        /// @brief SettlementRendererへの参照を取得
        /// @brief Get reference to SettlementRenderer
        SettlementRenderer* getRenderer() {
            return renderer_.get();
        }

    private:
        // 可視性管理
        bool visible_ = true;

        // 描画処理を担当
        std::unique_ptr<SettlementRenderer> renderer_;
    };
}

#endif // !PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP
