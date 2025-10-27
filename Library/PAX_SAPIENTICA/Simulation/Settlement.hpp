/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SETTLEMENT_HPP
#define PAX_SAPIENTICA_SETTLEMENT_HPP

#include <cmath>
#include <cstdint>

#include <algorithm>
#include <deque>
#include <functional>
#include <memory>
#include <random>

#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/RandomSelector.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/KanakumaLifeSpan.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementAgent.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/UniqueIdentification.hpp>

namespace paxs {

    namespace settlement {

        // 結婚・出産の定数
        constexpr double sigma = 0.25;
        constexpr double sigma_p_2 = sigma * sigma;
        constexpr double sigma_p_2_x_2 = (2.0 * sigma_p_2);

        // sigma * sqrt_2_x_pi
        constexpr double sigma_x_sqrt_2_x_pi = sigma * sqrt_2_x_pi;
    }

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

    class AStar {
        using AStarVec2 = paxs::Vector2<GridType>;
    private:
        // 始点 元の座標
        AStarVec2 start_vec2_original{};
        // 終点 元の座標
        AStarVec2 end_vec2_original{};
        // 始点
        AStarVec2 start_vec2{};
        // 終点
        AStarVec2 end_vec2{};
        // 範囲
        AStarVec2 min_range{};
        AStarVec2 max_range{};

        GridType z{};

        std::deque<AStarNode> open{};
        std::deque<AStarNode> closed{};

    public:
        AStar(const AStarVec2& start_vec2_, const AStarVec2& end_vec2_, const GridType z_) noexcept
            :start_vec2_original(start_vec2_), end_vec2_original(end_vec2_),
            start_vec2(start_vec2_ / z_), end_vec2(end_vec2_ / z_),
            z(z_) {
            min_range = AStarVec2{
                (std::min)(start_vec2.x, end_vec2.x) - std::abs(start_vec2.x - end_vec2.x) / 2,
                (std::min)(start_vec2.y, end_vec2.y) - std::abs(start_vec2.y - end_vec2.y) / 2
            };
            max_range = AStarVec2{
                (std::max)(start_vec2.x, end_vec2.x) + std::abs(start_vec2.x - end_vec2.x) / 2,
                (std::max)(start_vec2.y, end_vec2.y) + std::abs(start_vec2.y - end_vec2.y) / 2
            };
        }

        constexpr GridType calculateDistance(const AStarVec2& vec2_) const noexcept {
            const GridType x{ end_vec2.x - vec2_.x };
            const GridType y{ end_vec2.y - vec2_.y };
            return x * x + y * y;
        }
        bool isRange(const AStarVec2& vec2_) const noexcept {
            return vec2_.x >= min_range.x
                && vec2_.y >= min_range.y
                && vec2_.x < max_range.x
                && vec2_.y < max_range.y;
        }

        bool existPoint(const AStarVec2& vec2_, const double cost_) noexcept {
            auto i{ std::find(closed.begin(), closed.end(), vec2_) };
            if (i != closed.end()) {
                if ((*i).cost + (*i).distance < cost_) return true;
                else {
                    closed.erase(i);
                    return false;
                }
            }
            i = std::find(open.begin(), open.end(), vec2_);
            if (i != open.end()) {
                if ((*i).cost + (*i).distance < cost_) return true;
                else {
                    open.erase(i);
                    return false;
                }
            }
            return false;
        }

        double calcCost(std::shared_ptr<Environment>& environment, const AStarVec2& neighbour_z) const noexcept {
            // 海の通行コスト
            if (!environment->isLand(neighbour_z)) return SimulationConstants::getInstance()->ocean_cost;
            // 海岸の通行コスト
            else if (environment->isCoast(neighbour_z)) return SimulationConstants::getInstance()->coast_cost;
            else {
                const double lc = SimulationConstants::getInstance()->land_cost;
                if (environment->getSlope(neighbour_z) < 6 /*0.1*/) return lc;
                else if (environment->getSlope(neighbour_z) < 100/*5*/) return 1.8 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) < 133/*10*/) return 2.1 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) < 154/*15*/) return 2.5 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) < 169/*20*/) return 3.0 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) < 181/*25*/) return 3.6 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) < 191/*30*/) return 4.3 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) < 199/*35*/) return 5.1 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) < 206/*40*/) return 6.0 / 1.5 * lc;
                else if (environment->getSlope(neighbour_z) <= 250/*90*/) return 100 * 34.7 / 1.5 * lc;
            }
            return SimulationConstants::getInstance()->land_cost;
        }

        bool cellOpen(const AStarNode& node_, std::shared_ptr<Environment>& environment) noexcept {
            for (std::int_fast32_t x{}; x < 8; ++x) {
                const AStarVec2 neighbour = AStarVec2{ node_.position.x + astar_adjacent_cell[x].x,
                node_.position.y + astar_adjacent_cell[x].y };
                if (neighbour == end_vec2) return true;

                const AStarVec2 neighbour_z = neighbour * z;
                if (!isRange(neighbour)) continue;
                // コスト計算
                const double node_cost = calcCost(environment, neighbour_z) * ((x <= 3 /* 斜め移動 */) ? 1.41421356237 : 1.0) + node_.cost;
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
            // path_.emplace_back(end_vec2_original);
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

    class Settlement {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Agent = paxs::SettlementAgent;

        explicit Settlement(const std::uint_least32_t id, std::mt19937& gen_, const std::shared_ptr<Environment> env) noexcept : id(id), gen(&gen_), environment(env) {}

        /// @brief Get the uuid.
        /// @brief idを取得
        constexpr std::uint_least32_t getId() const noexcept { return id; }

        /// @brief Add an agent to the settlement.
        /// @brief 集落にエージェントを追加
        void addAgent(const Agent& agent) noexcept { agents.emplace_back(agent); }

        /// @brief Add agents to the settlement.
        /// @brief 集落にエージェントを追加
        void addAgents(const std::vector<Agent>& agents_) noexcept {
            this->agents.insert(this->agents.end(), agents_.begin(), agents_.end());
        }

        /// @brief Set the agents.
        /// @brief エージェントを消去
        void clearAgents() noexcept {
            this->agents.clear();
        }

        /// @brief Set the agents.
        /// @brief エージェントを消去
        void resizeAgents(const std::size_t size_) noexcept {
            this->agents.resize(size_);
        }

        /// @brief Set the agents.
        /// @brief エージェントを設定
        void setAgents(const std::vector<Agent>& agents_) noexcept {
            this->agents = agents_;
        }

        /// @brief Set the agents.
        /// @brief エージェントを設定
        void setAgent(const Agent& agent_, const std::size_t index_) noexcept {
            this->agents[index_] = agent_;
        }

        /// @brief Delete the agent.
        /// @brief エージェントを削除する
        void deleteAgent(const std::uint_least32_t id_) noexcept {
            agents.erase(std::remove_if(agents.begin(), agents.end(), [id_](const Agent& agent) { return agent.getId() == id_; }), agents.end());
        }

        /// @brief Set the position of the settlement.
        /// @brief 集落の座標を設定
        void setPosition(const Vector2& position_) noexcept {
            old_position = position;
            position = position_;
        }
        /// @brief 
        /// @brief 集落の過去の座標を消去
        void clearOldPosition() noexcept {
            old_position = Vector2(-1, -1);
            positions.resize(0);
        }

        /// @brief Get the position of the settlement.
        /// @brief 集落の座標を取得
        Vector2 getPosition() const noexcept { return position; }
        Vector2 getOldPosition() const noexcept { return old_position; }
        const std::vector<Vector2>& getPositions() const noexcept { return positions; }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        /// @param id The agent's id. エージェントのID
        Agent& getAgent(const std::uint_least32_t id_) noexcept {
            auto it = std::find_if(agents.begin(), agents.end(), [id_](const Agent& agent) { return agent.getId() == id_; });
            if (it == agents.end()) {
                const std::string message = "Agent not found.";
                PAXS_ERROR(message);
                return *agents.begin();
            }
            return *it;
        }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        const Agent& cgetAgent(const std::uint_least32_t id_) const noexcept {
            auto it = std::find_if(agents.begin(), agents.end(), [id_](const Agent& agent) { return agent.getId() == id_; });
            if (it == agents.end()) {
                const std::string message = "Agent not found.";
                PAXS_ERROR(message);
                return *agents.begin();
            }
            return *it;
        }

        /// @brief Get the agent copy.
        /// @brief エージェントを取得
        Agent getAgentCopy(const std::uint_least32_t id_) const noexcept {
            auto it = std::find_if(agents.begin(), agents.end(), [id_](const Agent& agent) { return agent.getId() == id_; });
            if (it == agents.end()) {
                const std::string message = "Agent not found.";
                PAXS_ERROR(message);
                return *agents.begin();
            }
            return *it;
        }

        /// @brief Get the agents.
        /// @brief エージェントを取得
        std::vector<Agent>& getAgents() noexcept { return agents; }

        /// @brief Get the agents.
        /// @brief エージェントを取得
        const std::vector<Agent>& cgetAgents() const noexcept { return agents; }

        bool marriagePair(
            Marriage3& male_settlement_pair,
            std::discrete_distribution<std::size_t>& csl_dist,
            const std::vector<std::vector<Settlement>*>& close_settlements_list
        ) {
            // 近隣の集落を探し、エージェントIDと集落IDのペアを作成
            std::size_t i = csl_dist(*gen);
            if (close_settlements_list.size() <= i) return false; // 配列外
            const std::vector<Settlement>& close_settlements = (*close_settlements_list[i]);

            const std::size_t close_settlements_size = close_settlements.size();
            if (close_settlements_size == 0) return false; // 集落が無かったので失敗

            std::uniform_int_distribution<int> dist(0, static_cast<int>(close_settlements_size - 1));
            const std::size_t j = static_cast<std::size_t>(dist(*gen));
            const Settlement& close_settlement = close_settlements[j];
            if (close_settlement.getPosition().distance_pow2(position) > SimulationConstants::getInstance()->marriage_search_range_pow2) return false; // 婚姻距離内に集落が無いため失敗

            const std::vector<Agent>& close_agent = close_settlement.cgetAgents();
            for (std::size_t k = 0; k < close_agent.size(); ++k) {
                if (close_agent[k].isMale() && close_agent[k].getLifeSpan() != 0 && close_agent[k].isAbleToMarriage()) {
                    // 婚姻可能なエージェントを発見
                    male_settlement_pair = Marriage3{
                            static_cast<std::uint_least32_t>(k),
                        static_cast<std::uint_least32_t>(j),
                        static_cast<std::uint_least32_t>(i),
                        close_agent[k].cgetFarming()
                    };
                    return true; // 成功
                }
            }
            return false; // 失敗
        }

        /// @brief Marriage.
        /// @brief 婚姻
        void marriage(
            const std::vector<std::vector<Settlement>*>& close_settlements_list,
            std::vector<GridType4>& marriage_pos_list
        ) noexcept {
            if (close_settlements_list.size() == 0) return;

            // 居住婚
            bool is_matrilocality = false;
            // 選択居住婚
            bool is_select_matrilocality = false;

            // 母方居住婚
            if (SimulationConstants::getInstance()->maternal_residence_probability >= 1.0f) {
                is_matrilocality = true;
            }
            // 父方居住婚
            else if (SimulationConstants::getInstance()->maternal_residence_probability <= 0.0f) {
                is_matrilocality = false;
            }
            // 選択居住婚
            else {
                is_select_matrilocality = true;
            }

            // 各集落の選定重み
            std::vector<std::size_t> close_settlements_list_probabilities;
            std::discrete_distribution<std::size_t> csl_dist;

            // 結婚の条件を満たすエージェントを取得
            for (std::size_t i = 0; i < agents.size(); ++i) {
                Agent& female = agents[i];
                // 結婚可能かどうか
                if (female.isFemale() && female.isAbleToMarriage()) {
                    // 妊娠していたら婚姻しない（婚姻可能と定義すると再婚者のデータで上書きされ子供への継承が不自然になる）
                    if (female.getBirthIntervalCount() > 0) continue;
                    // 婚姻するか乱数で決定
                    if (!isMarried(female.getAgeSizeT(), female.cgetFarming() > 0)) continue;

                    // 集落グリッドを重み付け
                    if (close_settlements_list_probabilities.size() == 0) {
                        for (const auto& close_settlements : close_settlements_list) {
                            close_settlements_list_probabilities.emplace_back(close_settlements->size());
                        }
                        csl_dist = std::discrete_distribution<std::size_t>(
                            close_settlements_list_probabilities.begin(),
                            close_settlements_list_probabilities.end()
                        );
                    }

                    Marriage3 male_settlement_pair{};
                    bool pair_result = false; // ペアが見つかったか？
                    for (std::size_t pair_loop = 0; pair_loop < 50/*婚姻相手を見つけるループの回数*/ && !pair_result; ++pair_loop) {
                        pair_result = marriagePair(
                            male_settlement_pair, csl_dist, close_settlements_list
                        );
                    }
                    // ペアが見つからなかったので婚姻を諦める
                    if (!pair_result) continue;

                    // ペアが見つかった場合
                    const std::uint_least32_t pair_agent_index = male_settlement_pair.first;
                    const std::uint_least32_t pair_settlement_index = male_settlement_pair.second;
                    Settlement& pair_settlement = (*(close_settlements_list[male_settlement_pair.third]))[pair_settlement_index];
                    Agent& male = pair_settlement.getAgents()[pair_agent_index];
                    // 選択居住婚の場合はどちらの住居に移動するか乱数で決定する
                    if (is_select_matrilocality) {
                        is_matrilocality = (SimulationConstants::getInstance()->maternal_residence_probability >= SimulationConstants::getInstance()->random_dist_f32(*gen));
                    }
                    // シミュレーションの設定で母方に移住するか父方に移住するかを決める
                    // 母方の場合
                    if (is_matrilocality) {
                        Agent male_copy = male;
                        male.setLifeSpan(0);
                        male.setPartnerId(0);
                        female.marry(male_copy.getId(), male_copy.cgetGenome(), male_copy.cgetFarming(), male_copy.cgetHunterGatherer(), male_copy.cgetLanguage());
                        male_copy.marry(female.getId(), female.cgetGenome(), female.cgetFarming(), female.cgetHunterGatherer(), female.cgetLanguage());
                        marriage_pos_list.emplace_back(GridType4{ pair_settlement.position.x, pair_settlement.position.y, position.x, position.y, is_matrilocality });
                        agents.emplace_back(male_copy);
                    }
                    // 父方の場合
                    else {
                        Agent female_copy = female;
                        female.setLifeSpan(0);
                        female.setPartnerId(0);
                        female_copy.marry(male.getId(), male.cgetGenome(), male.cgetFarming(), male.cgetHunterGatherer(), male.cgetLanguage());
                        male.marry(female_copy.getId(), female_copy.cgetGenome(), female_copy.cgetFarming(), female_copy.cgetHunterGatherer(), female_copy.cgetLanguage());
                        marriage_pos_list.emplace_back(GridType4{ position.x, position.y, pair_settlement.position.x, pair_settlement.position.y, is_matrilocality });
                        pair_settlement.getAgents().emplace_back(female_copy);
                    }
                }
            }
        }

        /// @brief Pre update.
        /// @brief 事前更新
        void preUpdate(KanakumaLifeSpan& kanakuma_life_span) noexcept {
            birth(kanakuma_life_span);
        }

        /// @brief Death.
        /// @brief 死亡
        void death() noexcept {
            for (auto i = 0; i < agents.size();) {
                // 年齢を１増やす
                agents[i].incrementAge();
                // もし死んでいなかったら次のエージェントを見る
                if (!(agents[i].isDead())) {
                    ++i;
                    continue;
                }

                const HumanIndexType partner_id = agents[i].getPartnerId();
                if (partner_id != 0) {
                    auto partnerIt = std::find_if(agents.begin(), agents.end(), [partner_id](const Agent& agent) { return agent.getId() == partner_id; });
                    if (partnerIt != agents.end()) {
                        partnerIt->divorce();
                    }
                }
                agents[i] = agents.back(); // 同義 it = agents.erase(it);
                agents.pop_back();
            }
        }

        // A* の経路探索
        void moveAStar(std::mt19937& engine, District& district_, const Vector2& current_position, Vector2& target_position) noexcept {
            double cost = -1.0;
            const int distance = SimulationConstants::getInstance()->move_dist(engine);

            const GridType cw = /*environment->getSlopeCellWidth() * */SimulationConstants::getInstance()->move_astar_distance;
            const Vector2 cp_cw = current_position / cw;
#ifdef _OPENMP
            double max_cost = 1.0;
            const int loop = SimulationConstants::getInstance()->move_astar_loop;
            std::vector<double> cost_list(loop);
            std::vector<Vector2> pos_list(loop);
            std::vector<Vector2> move_list(loop);
            std::vector<std::uint_least8_t> slope_list(loop);
            std::vector<std::vector<Vector2>> route_list(loop);

            for (int i = 0; i < loop; ++i) {
                move_list[i] = calcMovePosition(engine, district_, current_position, distance);
                slope_list[i] = environment->getSlope(move_list[i]);
            }
            {
                target_position = move_list[0];
                const Vector2 mp_cw = move_list[0] / cw;
                if (cp_cw == mp_cw) return; // 同じ座標なので AStar 不可能
                // 隣接座標なので AStar 不可能
                else if (std::abs(cp_cw.x - mp_cw.x) <= 1 && std::abs(cp_cw.y - mp_cw.y) <= 1) return;
            }
#pragma omp parallel for
            for (int i = 0; i < loop; ++i) {
#else
            for (std::uint_least32_t i = 0; i < SimulationConstants::getInstance()->move_astar_loop; ++i) {
#endif
#ifdef _OPENMP
                const Vector2 move_position = move_list[i];
#else
                const Vector2 move_position = calcMovePosition(engine, district_, current_position, distance);
                // 移動先が今の位置ならやり直し（可住地が見つからなかった）
                if (move_position == current_position) continue;
                const Vector2 mp_cw = move_position / cw;
                if (cp_cw == mp_cw) break; // 同じ座標なので AStar 不可能
                // 隣接座標なので AStar 不可能
                else if (std::abs(cp_cw.x - mp_cw.x) <= 1 && std::abs(cp_cw.y - mp_cw.y) <= 1) break;
#endif
                AStar astar(current_position, move_position, cw);
                astar.search(environment);
#ifdef _OPENMP
                pos_list[i] = move_position;
                cost_list[i] = astar.getCost();
                astar.setPath(route_list[i]);
#else
                // 最初の場合または以前よりもコストが低い場合は上書きする
                if (cost == -1.0 || cost > astar.getCost()) {
                    target_position = move_position;
                    cost = astar.getCost();
                    // 経路を設定
                    astar.setPath(positions);
                }
#endif

            }
#ifdef _OPENMP
            int index_num = 0;
            for (int i = 0; i < loop; ++i) {
                if (max_cost < cost_list[i]) {
                    max_cost = cost_list[i];
                }
            }
            for (int i = 0; i < loop; ++i) {
                // 傾斜含むコスト
                const double slope_cost = cost_list[i] + max_cost * ((slope_list[i] <= 129) ? 0 : slope_list[i] / 250.0) /* 傾斜の 9.25334 度以下の土地を優先 */;
                if (cost == -1.0 || cost > slope_cost) {
                    cost = slope_cost;
                    index_num = i;
                }
            }
            target_position = pos_list[index_num];
            positions = route_list[index_num];
#endif
        }

        // 移動先を計算する
        Vector2 calcMovePosition(std::mt19937& engine, District& district_, const Vector2& current_position, const int distance) const noexcept {
            Vector2 target_position = current_position;
            std::uint_least32_t  loop_count = 0; // 無限ループ回避用のループ上限値
            // 小さい領域のシミュレーションでは無限ループする可能性がある
            while (target_position == current_position || !environment->isLive(target_position)) {
                // 無限ループに陥った場合は無視
                if (loop_count >= SimulationConstants::getInstance()->move_redo) return current_position;

                // 移動距離が偏りのある方向を指定する距離以上か判定し、方向を格納する
                const double theta = (distance >= static_cast<int>(district_.direction_min_distance)) ?
                    SimulationConstants::getInstance()->theta_dist_array[district_.direction_dist(engine)](engine) :
                    SimulationConstants::getInstance()->theta_dist(engine);
                target_position = current_position + Vector2(static_cast<GridType>(std::cos(theta) * distance), static_cast<GridType>(std::sin(theta) * distance));
                ++loop_count;
            }
            return target_position;
        }

        /// @brief Move.
        /// @brief 移動
        /// @return 集落グリッドを移動したかどうか
        std::tuple<std::uint_least32_t, Vector2, Vector2> move(std::mt19937& engine, District& district_) noexcept {
            // 確率で移動
            if (SimulationConstants::getInstance()->random_dist(engine) > SimulationConstants::getInstance()->move_probability) return { 0, Vector2(), Vector2() };

            Vector2 current_key;
            Vector2 target_key;

            // 座標を移動
            // 移動距離0~max_move_distance

            Vector2 current_position = position;
            Vector2 target_position = position;

            // A* を使った方法
            if(SimulationConstants::getInstance()->move_method == MurMur3::calcHash("astar") && SimulationConstants::getInstance()->move_astar_loop >= 1){
                moveAStar(engine, district_, current_position, target_position);
            }
            else {
                std::uint_least32_t  loop_count = 0; // 無限ループ回避用のループ上限値
                // 小さい領域のシミュレーションでは無限ループする可能性がある
                while (target_position == current_position || !environment->isLive(target_position)) {
                    // 無限ループに陥った場合は無視
                    if (loop_count >= SimulationConstants::getInstance()->move_redo) return { 0, Vector2(), Vector2() };
                    int distance = SimulationConstants::getInstance()->move_dist(engine);

                    // 移動距離が偏りのある方向を指定する距離以上か判定し、方向を格納する
                    const double theta = (distance >= static_cast<int>(district_.direction_min_distance)) ?
                        SimulationConstants::getInstance()->theta_dist_array[district_.direction_dist(engine)](engine) :
                        SimulationConstants::getInstance()->theta_dist(engine);
                    target_position = current_position + Vector2(static_cast<GridType>(std::cos(theta) * distance), static_cast<GridType>(std::sin(theta) * distance));
                    ++loop_count;
                }
            }
            current_key = current_position / SimulationConstants::getInstance()->cell_group_length;
            target_key = target_position / SimulationConstants::getInstance()->cell_group_length;

            if (current_key == target_key) return { 0, Vector2(), Vector2() };

            is_moved = true;

            return { id, current_key, target_key };
        }

        /// @brief Get the is_moved.
        /// @brief 移動したかどうかを取得
        bool isMoved() const noexcept { return is_moved; }

        void setIsMoved(const bool is_moved_) noexcept {
            is_moved = is_moved_;
        }

        /// @brief Get the Bronze.
        /// @brief 青銅を取得
        std::uint_least32_t getBronze() const noexcept {
            return bronze;
        }
        void setBronze(const std::uint_least32_t bronze_) noexcept {
            bronze = bronze_;
        }

        /// @brief Get the population.
        /// @brief 人口を取得
        std::size_t getPopulation() const noexcept { return agents.size(); }

        /// @brief Get the weight population.
        /// @brief 重み人口を取得
        double getPopulationWeight() const noexcept {
            // 農耕文化の重み
            const double ac_weight = SimulationConstants::getInstance()->max_agricultural_settlement_weight;
            // 狩猟採集文化の重み
            const double hg_weight = SimulationConstants::getInstance()->max_hunter_gatherer_settlement_weight;

            double population_weight = 0;
            for (const auto& agent : agents) {
                population_weight += (agent.cgetFarming() > 0) ? ac_weight : hg_weight;
            }
            return population_weight;
        }

        /// @brief Get the population.
        /// @brief 渡来人口を取得
        std::size_t getFarmingPopulation() const noexcept {
            std::size_t farming_population = 0;

            for (std::size_t i = 0; i < agents.size(); ++i) {
                if (agents[i].cgetFarming() > 0) {
                    ++farming_population;
                }
            }
            return farming_population;
        }

        /// @brief Get the SNP.
        /// @brief SNP を取得
        double getSNP() const noexcept {
            std::uint_least64_t snp = 0;

            for (std::size_t i = 0; i < agents.size(); ++i) {
                    snp += static_cast<std::uint_least64_t>(agents[i].cgetGenome().getSNP());
            }
            return static_cast<double>(snp) / static_cast<double>(agents.size()) / 255.0;
        }

        /// @brief Get the Language.
        /// @brief 言語を取得
        std::uint_least8_t getLanguage() const noexcept {
            static std::uint_least8_t language[256];
            for (std::size_t i = 0; i < 256; ++i) {
                language[i] = 0;
            }
            for (std::size_t i = 0; i < agents.size(); ++i) {
                ++language[agents[i].cgetLanguage()];
            }
            std::size_t max_count = 0;
            std::size_t max_index = 0;
            for (std::size_t i = 0; i < 256; ++i) {
                if (language[i] > max_count) {
                    max_count = language[i];
                    max_index = i;
                }
            }
            return static_cast<std::uint_least8_t>(max_index);
        }

        /// @brief Get the most mtDNA.
        /// @brief 最多 mtDNA を取得
        std::size_t getMostMtDNA() const noexcept {
            std::size_t mtdna_max = 0;

            for (std::size_t i = 0; i < agents.size(); ++i) {
                mtdna_max += agents[i].cgetGenome().getMtDNA();
            }
            return mtdna_max / agents.size();
        }

        /// @brief Divide the settlement.
        /// @brief 集落を分割
        Settlement divide() noexcept {
            //　とりあえず、エージェントを半分に分ける
            std::vector<Agent> new_settlement_agents = std::vector<Agent>(agents.begin() + agents.size() / 2, agents.end());
            agents.resize(agents.size() / 2); // 同義 agents.erase(agents.begin() + agents.size() / 2, agents.end());

            // パートナー同士は同じ集落に振り分ける
            for (Agent& agent : agents) {
                if (agent.isMarried() && agent.isFemale()) {
                    auto it = std::find_if(new_settlement_agents.begin(), new_settlement_agents.end(), [agent](const Agent& a) { return a.getId() == agent.getPartnerId(); });
                    if (it != new_settlement_agents.end()) {
                        agents.emplace_back(*it);
                        (*it) = new_settlement_agents.back(); // 同義 new_settlement_agents.erase(it);
                        new_settlement_agents.pop_back();
                    }
                }
            }

            for (Agent& agent : new_settlement_agents) {
                if (agent.isMarried() && agent.isFemale()) {
                    auto it = std::find_if(agents.begin(), agents.end(), [agent](const Agent& a) { return a.getId() == agent.getPartnerId(); });
                    if (it != agents.end()) {
                        new_settlement_agents.emplace_back(*it);
                        (*it) = agents.back(); // 同義 agents.erase(it);
                        agents.pop_back();
                    }
                }
            }

            // 新しい集落を作成
            Settlement new_settlement = Settlement(UniqueIdentification<std::uint_least32_t>::generate(), *gen, environment);
            new_settlement.setAgents(new_settlement_agents);
            return new_settlement;
        }
    private:
        /// @brief 既に移動したかどうか
        bool is_moved = false;
        /// @brief 集落id
        std::uint_least32_t id = 0;
        /// @brief 集落の座標
        Vector2 position{};

        std::mt19937* gen{}; // 乱数生成器
        

        std::shared_ptr<Environment> environment{}; // 環境
        /// @brief エージェントの配列
        std::vector<Agent> agents{};

        std::uint_least32_t bronze = 0; // 青銅
        /// @brief 集落の移動前の座標
        Vector2 old_position{ -1,-1 };
        std::vector<Vector2> positions{};

        /// @brief Birth.
        /// @brief 出産
        void birth(KanakumaLifeSpan& kanakuma_life_span) noexcept {
            std::vector<Agent> children;
            for (auto& agent : agents) {
                if (agent.getBirthIntervalCount() > 0) {
                    std::uint_least8_t count = agent.decrementBirthIntervalCount();
                    if (count == 0) {
                        // 生業文化別の死産率を格納
                        const double stillbirth_rate = (agent.cgetFarming() > 0) ?
                            SimulationConstants::getInstance()->agricultural_stillbirth_rate :
                            SimulationConstants::getInstance()->hunter_gatherer_stillbirth_rate;

                        // 死産率 100 ％の場合は出産しない
                        if (stillbirth_rate >= 1.0f) continue;
                        else if (stillbirth_rate > 0.0f) {
                            // 死産
                            if (SimulationConstants::getInstance()->random_dist_f32(*gen) < stillbirth_rate) continue;
                        }
                        const std::uint_least8_t farming =
                            // 両親が農耕文化であれば両親の半分の値を引き継ぐ
                            (agent.cgetFarming() > 0 && agent.cgetPartnerFarming() > 0) ?
                            static_cast<std::uint_least8_t>((int(agent.cgetFarming()) + int(agent.cgetPartnerFarming())) / 2)
                            // 両親が共に農耕文化を持たない場合は 0
                            : ((agent.cgetFarming() == 0 && agent.cgetPartnerFarming() == 0) ? 0
                                // 片親が農耕文化を持つ場合は乱数
                                : ((SimulationConstants::getInstance()->random_dist_f32(*gen) < SimulationConstants::getInstance()->child_agriculture_priority) ?
                                    // 農耕文化を持つ親から値を引き継ぐ
                                    ((agent.cgetFarming() == 0) ? agent.cgetPartnerFarming() : agent.cgetFarming()) :
                                    0
                                ));
                        const Genome genome = Genome::generateFromParents(*gen, agent.cgetGenome(), agent.cgetPartnerGenome());
                        children.emplace_back(Agent(
                            UniqueIdentification<HumanIndexType>::generate(),
                            0,
                            kanakuma_life_span.setLifeSpan(farming > 0, genome.isMale(), *gen),
                            genome,
                            farming,
                            (((*gen)() % 2) == 0) ? agent.cgetHunterGatherer() : agent.cgetPartnerHunterGatherer(),
                            (((*gen)() % 2) == 0) ? agent.cgetLanguage() : agent.cgetPartnerLanguage()
                        ));
                    }
                }
                // 出産可能かどうか
                else if (agent.isAbleToGiveBirth() && isAbleToGiveBirth(agent.getAgeSizeT(), agent.cgetFarming() > 0)) {
                    agent.setBirthIntervalCount(SimulationConstants::getInstance()->birth_interval);
                }

            }
            // 新しい赤ちゃんがいない場合はエージェントを追加をしない
            if (children.size() == 0) return;
            agents.insert(agents.end(), children.begin(), children.end());
        }

        /// @brief Emigration.
        /// @brief 渡来
        void emigration(KanakumaLifeSpan& kanakuma_life_span) noexcept {
            if (agents.size() >= 60) {

                Genome genome = Genome::generateRandom(*gen);
                const AgeType set_lifespan = kanakuma_life_span.setLifeSpan(true, genome.isMale(), *gen);

                std::uniform_int_distribution<> lifespan_dist{
                    (std::min)(18 * SimulationConstants::getInstance()->steps_per_year + 1, static_cast<int>(set_lifespan - 1)),
                    static_cast<int>(set_lifespan - 1) }; // 性別の乱数分布

                agents.emplace_back(Agent(
                    UniqueIdentification<HumanIndexType>::generate(),
                    static_cast<AgeType>(lifespan_dist(*gen)),
                    set_lifespan,
                    genome,
                    255, // ((gen() % 2) == 0) ? agent.cgetFarming() : agent.cgetPartnerFarming(),
                    0, // ((gen() % 2) == 0) ? agent.cgetHunterGatherer() : agent.cgetPartnerHunterGatherer()
                    255
                ));
            }
        }

        /// @brief Age update.
        /// @brief 年齢更新
        void ageUpdate() noexcept {
#ifdef _OPENMPa
            const int agent_size = static_cast<int>(agents.size());
#pragma omp parallel for
            for (int i = 0; i < agent_size; ++i) {
                agents[i].incrementAge();
            }
#else
            for (auto& agent : agents) {
                agent.incrementAge();
            }
#endif
        }

        /// @brief Is the agent married?
        /// @brief 確率で結婚するかどうかを返す
        bool isMarried(const std::size_t age, const bool is_agricultural) noexcept {
            // 婚姻可能年齢の上限値以上だったら結婚しない
            const double threshold = SimulationConstants::getInstance()->getMarriageProbability(age, is_agricultural);
            if (threshold == 0.0) return false;
            if (threshold >= 1.0) return true;
            return SimulationConstants::getInstance()->random_dist(*gen) < threshold;
        }

        /// @brief Is able to give birth?
        /// @brief 確率で出産するかどうかを返す
        bool isAbleToGiveBirth(const std::size_t age, const bool is_agricultural) noexcept {
            const double threshold = SimulationConstants::getInstance()->getChildbearingProbability(age, is_agricultural);
            if (threshold == 0.0) return false;
            if (threshold >= 1.0) return true;
            return SimulationConstants::getInstance()->random_dist(*gen) < threshold;
        }
    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_HPP
