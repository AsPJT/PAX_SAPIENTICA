/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP
#define PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP

#include <vector>

#include <PAX_MAHOROBA/Map/Location/SettlementRenderer.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/SettlementGrid.hpp>

namespace paxs {

    /// @brief 集落の描画を管理するクラス
    /// @brief Class to manage settlement rendering
    class SettlementManager : public IRenderable {
    private:
        double cached_jdn_ = 0.0;
        const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* cached_agents_ = nullptr;
        const std::vector<GridType4>* cached_marriage_pos_list_ = nullptr;
        Vector2<double> cached_map_view_size_{0.0, 0.0};
        Vector2<double> cached_map_view_center_{0.0, 0.0};
        std::size_t select_draw_ = 1;
        bool is_line_ = false;
        bool is_arrow_ = true;


    public:
        SettlementManager() = default;

        void render() const override {
            if (!isVisible() || (cached_agents_ == nullptr) || (cached_marriage_pos_list_ == nullptr)) {
                return;
            }
            SettlementRenderer::draw(cached_jdn_, cached_agents_, cached_marriage_pos_list_,
                cached_map_view_size_, cached_map_view_center_,
                select_draw_, is_line_, is_arrow_);
        }

        /// @brief 描画パラメータを設定
        void setDrawParams(
            const double jdn,
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>& agents,
            const std::vector<GridType4>& marriage_pos_list,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center,
            const std::size_t select_draw, const bool is_line, const bool is_arrow
        ) {
            cached_jdn_ = jdn;
            cached_agents_ = &agents;
            cached_marriage_pos_list_ = &marriage_pos_list;
            cached_map_view_size_ = map_view_size;
            cached_map_view_center_ = map_view_center;
            select_draw_ = select_draw;
            is_line_ = is_line;
            is_arrow_ = is_arrow;
        }

        /// @brief キャッシュをクリア（シミュレーションクリア時に呼ぶ）
        /// @brief Clear cache (call when simulation is cleared)
        void clearCache() {
            cached_agents_ = nullptr;
            cached_marriage_pos_list_ = nullptr;
            cached_jdn_ = 0.0;
            cached_map_view_size_ = Vector2<double>(0.0, 0.0);
            cached_map_view_center_ = Vector2<double>(0.0, 0.0);
        }

        RenderLayer getLayer() const override { return RenderLayer::MapContent; }
        // TODO: isVisible() の実装を検討
        bool isVisible() const override { return true; }

    };
}

#endif // !PAX_MAHOROBA_SETTLEMENT_MANAGER_HPP
