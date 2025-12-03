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
#include <array>
#include <cmath>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <PAX_SAPIENTICA/Core/Math/Math.hpp>
#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    // ✅ 8方向の隣接セル定義（直線4方向+斜め4方向）
    static constexpr std::array<Vector2<GridType>, 8> astar_adjacent_cell = { {
        {1, 0},    // 東 (x=0, 直線)
        {0, 1},    // 南 (x=1, 直線)
        {-1, 0},   // 西 (x=2, 直線)
        {0, -1},   // 北 (x=3, 直線)
        {1, 1},    // 南東 (x=4, 斜め)
        {-1, 1},   // 南西 (x=5, 斜め)
        {-1, -1},  // 北西 (x=6, 斜め)
        {1, -1}    // 北東 (x=7, 斜め)
    } };

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
            :position(pos_), parent_node(parent_node_), distance(distance_), cost(cost_) {
        }
        bool operator==(const AStarNode& node_) const noexcept {
            return position == node_.position;
        }
        bool operator==(const AStarVec2& vec2_) const noexcept {
            return position == vec2_;
        }

        /// @brief Priority Queue用の比較演算子（f値が小さい方が優先）
        bool operator>(const AStarNode& other) const noexcept {
            return (cost + distance) > (other.cost + other.distance);
        }
    };

    /// @brief Vector2のハッシュ関数a
    struct Vec2Hash {
        std::size_t operator()(const paxs::Vector2<GridType>& v) const noexcept {
            std::size_t seed = 0;
            // xとyを混ぜ合わせる一般的なハッシュ結合
            seed ^= std::hash<GridType>{}(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<GridType>{}(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };

    /// @brief A* pathfinding algorithm
        /// @brief A*経路探索アルゴリズム
    class AStar {
        using AStarVec2 = paxs::Vector2<GridType>;
    private:
        // 始点 元の座標
        AStarVec2 start_vec2_original{};
        // 終点 元の座標 (★追加: 正確な終点を保持)
        AStarVec2 end_vec2_original{};

        // 始点 (グリッド)
        AStarVec2 start_vec2{};
        // 終点 (グリッド)
        AStarVec2 end_vec2{};

        GridType z{};

        // 範囲
        Range<AStarVec2> search_range{};

        std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> open{};
        std::unordered_set<AStarVec2, Vec2Hash> closed_set{};
        std::unordered_map<AStarVec2, AStarNode, Vec2Hash> all_nodes{};
        std::unordered_map<AStarVec2, double, Vec2Hash> open_costs{};

    public:
        // コンストラクタ: 探索範囲を2倍に拡張
        AStar(const AStarVec2& start_vec2_, const AStarVec2& end_vec2_, const GridType z_) noexcept
            :start_vec2_original(start_vec2_), end_vec2_original(end_vec2_),
            start_vec2(start_vec2_ / z_), end_vec2(end_vec2_ / z_),
            z(z_),
            search_range(
                AStarVec2{
                    // 最小X - マージンX (幅の半分、ただし最低5は確保)
                    (std::min)(start_vec2.x, end_vec2.x) - (std::max)(std::abs(start_vec2.x - end_vec2.x) / 2, 5),
                    // 最小Y - マージンY (高さの半分、ただし最低5は確保)
                    (std::min)(start_vec2.y, end_vec2.y) - (std::max)(std::abs(start_vec2.y - end_vec2.y) / 2, 5)
                },
                AStarVec2{
                    // 最大X + マージンX
                    (std::max)(start_vec2.x, end_vec2.x) + (std::max)(std::abs(start_vec2.x - end_vec2.x) / 2, 5),
                    // 最大Y + マージンY
                    (std::max)(start_vec2.y, end_vec2.y) + (std::max)(std::abs(start_vec2.y - end_vec2.y) / 2, 5)
                }
            ) {
            // ... (メモリ確保部分はそのまま) ...
            const std::size_t dx = static_cast<std::size_t>(std::abs(start_vec2.x - end_vec2.x));
            const std::size_t dy = static_cast<std::size_t>(std::abs(start_vec2.y - end_vec2.y));
            const std::size_t estimated_size = dx * dy;

            if (estimated_size > 0) {
                all_nodes.reserve(estimated_size / 2);
                open_costs.reserve(estimated_size / 4);
                closed_set.reserve(estimated_size / 2);
            }
        }

        // ... (calculateDistance, existPoint, calcCost, cellOpen, search, getCost は変更なし) ...
        constexpr GridType calculateDistance(const AStarVec2& vec2_) const noexcept {
            const GridType x{ end_vec2.x - vec2_.x };
            const GridType y{ end_vec2.y - vec2_.y };
            return x * x + y * y;
        }
        bool existPoint(const AStarVec2& vec2_, const double new_f_value) noexcept {
            if (closed_set.count(vec2_)) return true;
            auto it = open_costs.find(vec2_);
            if (it != open_costs.end()) {
                if (it->second <= new_f_value) return true;
                it->second = new_f_value;
            }
            return false;
        }
        double calcCost(std::shared_ptr<Environment>& environment, const AStarVec2& neighbour_z) const noexcept {
            if (!environment->isLand(neighbour_z)) return SimulationConstants::getInstance().ocean_cost;
            if (environment->isCoast(neighbour_z)) return SimulationConstants::getInstance().coast_cost;
            const double land_cost = SimulationConstants::getInstance().land_cost;
            const std::uint_least8_t slope = environment->getSlope(neighbour_z);
            if (slope < 6) return land_cost;
            if (slope < 100) return 25 * land_cost;
            if (slope < 133) return 100 * land_cost;
            if (slope < 154) return 15 * 15 * land_cost;
            if (slope < 169) return 400 * land_cost;
            if (slope < 181) return 25 * 25 * land_cost;
            if (slope < 191) return 30 * 30 * land_cost;
            if (slope < 199) return 35 * 35 * land_cost;
            if (slope < 206) return 40 * 40 * land_cost;
            if (slope <= 250) return 90 * 90 * land_cost;
            return land_cost;
        }
        bool cellOpen(const AStarNode& node_, std::shared_ptr<Environment>& environment) noexcept {
            static constexpr double sqrt2 = MathConstants<double>::sqrt2();
            for (std::int_fast32_t x{}; x < 8; ++x) {
                const AStarVec2 neighbour = AStarVec2{
                    node_.position.x + astar_adjacent_cell[x].x,
                    node_.position.y + astar_adjacent_cell[x].y
                };
                if (neighbour == end_vec2) {
                    const AStarVec2 neighbour_z = neighbour * z;
                    const double move_cost = (x >= 4) ? sqrt2 : 1.0;
                    const double node_cost = calcCost(environment, neighbour_z) * move_cost + node_.cost;
                    const GridType distance = calculateDistance(neighbour);
                    AStarNode end_node(neighbour, node_.position, distance, node_cost);
                    all_nodes[neighbour] = end_node;
                    return true;
                }
                const AStarVec2 neighbour_z = neighbour * z;
                if (!search_range.contains(neighbour)) continue;
                const double move_cost = (x >= 4) ? sqrt2 : 1.0;
                const double node_cost = calcCost(environment, neighbour_z) * move_cost + node_.cost;
                const GridType distance = calculateDistance(neighbour);
                const double f_value = node_cost + distance;
                if (existPoint(neighbour, f_value)) continue;
                AStarNode new_node(neighbour, node_.position, distance, node_cost);
                open.push(new_node);
                open_costs[neighbour] = f_value;
                all_nodes[neighbour] = new_node;
            }
            return false;
        }
        bool search(std::shared_ptr<Environment>& environment) noexcept {
            AStarNode start_node(start_vec2, AStarVec2{}, calculateDistance(start_vec2), 0);
            open.push(start_node);
            open_costs[start_vec2] = start_node.distance;
            all_nodes[start_vec2] = start_node;
            while (!open.empty()) {
                const AStarNode current = open.top();
                open.pop();
                open_costs.erase(current.position);
                if (closed_set.count(current.position)) continue;
                closed_set.insert(current.position);
                if (cellOpen(current, environment)) return true;
            }
            return false;
        }
        double getCost() const noexcept {
            auto it = all_nodes.find(end_vec2);
            if (it != all_nodes.end()) return it->second.cost;
            return 0.0;
        }

        // ★setPathのロジックを修正 (Stick & Loop対策・完全版)
        void setPath(std::vector<AStarVec2>& path_) noexcept {
            path_.clear();

            // ---------------------------------------------------------
            // 1. まずグリッド上の座標だけをリストアップする (End -> Start)
            // ---------------------------------------------------------
            std::vector<AStarVec2> grid_path;
            AStarVec2 current = end_vec2;

            // 安全装置
            int safety_count = 0;
            const int max_count = 100000;

            bool path_found = false;

            while (true) {
                grid_path.push_back(current);

                // 始点に到達したら成功
                if (current == start_vec2) {
                    path_found = true;
                    break;
                }

                // 親ノードを検索
                auto it = all_nodes.find(current);
                if (it == all_nodes.end()) {
                    // 親が見つからない＝経路が断絶している
                    // ここで break しないと、探索失敗時に変な場所へ飛ぶ
                    break;
                }

                AStarVec2 parent = it->second.parent_node;

                // 【重要】棒対策: 親ノードが (0,0) で、かつ始点が (0,0) でない場合、
                // それは初期化されていないノードへの不正アクセスなので終了する
                if (parent.x == 0 && parent.y == 0 && (start_vec2.x != 0 || start_vec2.y != 0)) {
                    break;
                }

                current = parent;

                if (++safety_count > max_count) break;
            }

            // 経路が見つからなかった、あるいは断絶した場合は、始点と終点だけを直結する
            // (これで変な方向への棒が出るのを防ぎます)
            if (!path_found || grid_path.empty()) {
                path_.emplace_back(start_vec2_original);
                path_.emplace_back(end_vec2_original);
                return;
            }

            // End -> Start の順で取れているので、Start -> End に直す
            std::reverse(grid_path.begin(), grid_path.end());


            // ---------------------------------------------------------
            // 2. グリッド座標をワールド座標に変換しつつ、始点・終点付近の重複を除去
            // ---------------------------------------------------------
            const AStarVec2 center_offset = AStarVec2{ z / 2, z / 2 };

            // ループ対策:
            // 正確な始点/終点から「z (1グリッド分)」以内のグリッド中心点は
            // スプライン制御点として近すぎるため、間引いてパスを安定させる。
            const GridType skip_dist_sq = z;

            // A. 正確な始点を追加
            path_.emplace_back(start_vec2_original);

            // B. 中間のグリッド点を追加（始点・終点に近すぎるものはスキップ）
            for (const auto& grid_pos : grid_path) {
                const AStarVec2 world_pos = grid_pos * z + center_offset;

                // 始点からの距離チェック
                GridType d_start_x = world_pos.x - start_vec2_original.x;
                GridType d_start_y = world_pos.y - start_vec2_original.y;
                if (d_start_x * d_start_x + d_start_y * d_start_y < skip_dist_sq) {
                    continue; // 始点に近すぎるグリッド中心は無視
                }

                // 終点からの距離チェック
                GridType d_end_x = world_pos.x - end_vec2_original.x;
                GridType d_end_y = world_pos.y - end_vec2_original.y;
                if (d_end_x * d_end_x + d_end_y * d_end_y < skip_dist_sq) {
                    continue; // 終点に近すぎるグリッド中心は無視
                }

                path_.emplace_back(world_pos);
            }

            // C. 正確な終点を追加
            path_.emplace_back(end_vec2_original);

            // 【最終チェック】フィルタリングで中間点が消えすぎて点が2つ未満になったら
            // 強制的に始点・終点のみの直線にする
            if (path_.size() < 2) {
                path_.clear();
                path_.emplace_back(start_vec2_original);
                path_.emplace_back(end_vec2_original);
            }
        }
    };

}

#endif // !PAX_SAPIENTICA_SIMULATION_ALGORITHM_ASTAR_HPP
