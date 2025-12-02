/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_ALGORITHM_ASTAR_HPP
#define PAX_SAPIENTICA_SIMULATION_ALGORITHM_ASTAR_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <memory>
#include <vector>

#include <PAX_SAPIENTICA/Core/Math/Math.hpp>
#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>

namespace paxs {

    /// @brief A* pathfinding node
    /// @brief A*経路探索のノード
    struct AStarNode {
        using AStarVec2 = paxs::Vector2<GridType>;
        //位置
        AStarVec2 position{};
        //親ノード
        AStarVec2 parent_node{};
        //距離
        GridType distance{};
        //コスト
        double cost{};

        constexpr AStarNode() noexcept = default;
        constexpr AStarNode(const AStarVec2& pos_, const AStarVec2& parent_node_, const GridType distance_, const double cost_) noexcept
            :position(pos_), parent_node(parent_node_), distance(distance_), cost(cost_) {}
        bool operator==(const AStarNode& node_) const noexcept {
            return position == node_.position;
        }
        bool operator==(const AStarVec2& vec2_) const noexcept {
            return position == vec2_;
        }
    };

    /// @brief A* pathfinding algorithm
    /// @brief A*経路探索アルゴリズム
    class AStar {
        using AStarVec2 = paxs::Vector2<GridType>;
    private:
        // 始点 元の座標
        AStarVec2 start_vec2_original{};
        // 始点
        AStarVec2 start_vec2{};
        // 終点
        AStarVec2 end_vec2{};

        GridType z{};

        // 範囲
        Range<AStarVec2> search_range{};

        std::deque<AStarNode> open{};
        std::deque<AStarNode> closed{};

    public:
        AStar(const AStarVec2& start_vec2_, const AStarVec2& end_vec2_, const GridType z_) noexcept
            :start_vec2_original(start_vec2_),
            start_vec2(start_vec2_ / z_), end_vec2(end_vec2_ / z_),
            z(z_),
            search_range(
                AStarVec2{
                    (std::min)(start_vec2.x, end_vec2.x) - std::abs(start_vec2.x - end_vec2.x) / 2,
                    (std::min)(start_vec2.y, end_vec2.y) - std::abs(start_vec2.y - end_vec2.y) / 2
                },
                AStarVec2{
                    (std::max)(start_vec2.x, end_vec2.x) + std::abs(start_vec2.x - end_vec2.x) / 2,
                    (std::max)(start_vec2.y, end_vec2.y) + std::abs(start_vec2.y - end_vec2.y) / 2
                }
            ) {
        }

        constexpr GridType calculateDistance(const AStarVec2& vec2_) const noexcept {
            const GridType x{ end_vec2.x - vec2_.x };
            const GridType y{ end_vec2.y - vec2_.y };
            return x * x + y * y;
        }

        bool existPoint(const AStarVec2& vec2_, const double cost_) noexcept {
            auto i = std::find(closed.begin(), closed.end(), vec2_);
            if (i != closed.end()) {
                if ((*i).cost + (*i).distance < cost_) {
                    return true;
                }
                closed.erase(i);
                return false;
            }
            i = std::find(open.begin(), open.end(), vec2_);
            if (i == open.end()) {
                return false;
            }
            if ((*i).cost + (*i).distance < cost_) {
                return true;
            }
            open.erase(i);
            return false;
        }

        double calcCost(std::shared_ptr<Environment>& environment, const AStarVec2& neighbour_z) const noexcept {
            // 海の通行コスト
            if (!environment->isLand(neighbour_z)) {
                return SimulationConstants::getInstance().ocean_cost;
            }
            // 海岸の通行コスト
            if (environment->isCoast(neighbour_z)) {
                return SimulationConstants::getInstance().coast_cost;
            }
            const double land_cost = SimulationConstants::getInstance().land_cost;
            if (environment->getSlope(neighbour_z) < 6 /*0.1*/) return land_cost;
            if (environment->getSlope(neighbour_z) < 100/*5*/) return 1.8 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) < 133/*10*/) return 2.1 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) < 154/*15*/) return 2.5 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) < 169/*20*/) return 3.0 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) < 181/*25*/) return 3.6 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) < 191/*30*/) return 4.3 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) < 199/*35*/) return 5.1 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) < 206/*40*/) return 6.0 / 1.5 * land_cost;
            if (environment->getSlope(neighbour_z) <= 250/*90*/) return 100 * 34.7 / 1.5 * land_cost;

            return SimulationConstants::getInstance().land_cost;
        }

        bool cellOpen(const AStarNode& node_, std::shared_ptr<Environment>& environment) noexcept {
            for (std::int_fast32_t x{}; x < 8; ++x) {
                const AStarVec2 neighbour = AStarVec2{ node_.position.x + astar_adjacent_cell[x].x,
                node_.position.y + astar_adjacent_cell[x].y };
                if (neighbour == end_vec2) return true;

                const AStarVec2 neighbour_z = neighbour * z;
                if (!search_range.contains(neighbour)) continue;
                // コスト計算
                const double node_cost = calcCost(environment, neighbour_z) * ((x <= 3 /* 斜め移動 */) ? MathConstants<double>::sqrt2() : 1.0) + node_.cost;
                const GridType distance = calculateDistance(neighbour);
                if (existPoint(neighbour, node_cost + distance)) continue;
                open.emplace_back(AStarNode(neighbour, node_.position, distance, node_cost));
            }
            return false;
        }

        bool search(std::shared_ptr<Environment>& environment) noexcept {
            open.emplace_back(AStarNode(start_vec2, AStarVec2{}, calculateDistance(start_vec2), 0));
            while (!open.empty()) {
                const AStarNode nn{ open.front() };
                open.pop_front();
                closed.emplace_back(nn);
                if (cellOpen(nn, environment)) return true;
            }
            return false;
        }

        double getCost() const noexcept {
            return closed.back().cost;
        }

        void setPath(std::vector<AStarVec2>& path_) noexcept {
            path_.resize(0);
            const AStarVec2 sub = AStarVec2{
                std::abs(start_vec2_original.x - start_vec2.x * z) / 2,
                std::abs(start_vec2_original.y - start_vec2.y * z) / 2
            };
            path_.emplace_back(closed.back().position * z + sub);
            AStarVec2 parent_node{ closed.back().parent_node };
            for (auto&& i{ closed.rbegin() }; i != closed.rend(); ++i) {
                if (!((*i).position == parent_node) || ((*i).position == start_vec2)) continue;
                path_.emplace_back((*i).position * z + sub);
                parent_node = (*i).parent_node;
            }
            // path_.emplace_back(start_vec2_original);
        }
    };

}

#endif // !PAX_SAPIENTICA_SIMULATION_ALGORITHM_ASTAR_HPP
