/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_ENTITY_SETTLEMENT_HPP
#define PAX_SAPIENTICA_SIMULATION_ENTITY_SETTLEMENT_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include <unordered_map>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Simulation/Algorithm/AStar.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/JapanProvinces.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/KanakumaLifeSpan.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/SettlementAgent.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/UniqueIdentification.hpp>

namespace paxs {

    /// @brief Settlement data class
    /// @brief 集落データクラス
    class SettlementData {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Agent = paxs::SettlementAgent;

        // Constructor
        explicit SettlementData(std::uint_least32_t id_) noexcept : id(id_) {}

    private:
        // Data members
        std::uint_least32_t id = 0;
        Vector2 position{};
        Vector2 old_position{ -1, -1 };
        std::vector<Vector2> positions{};
        std::vector<Agent> agents{};
        bool is_moved = false;
        std::uint_least32_t bronze = 0;

        // ✅ エージェントID→インデックスのマップ（O(1)検索）
        std::unordered_map<HumanIndexType, std::size_t> agent_id_to_index_;

    public:

        // Getters
        constexpr std::uint_least32_t getId() const noexcept { return id; }
        Vector2 getPosition() const noexcept { return position; }
        Vector2& getPositionMutable() noexcept { return position; }
        Vector2 getOldPosition() const noexcept { return old_position; }
        const std::vector<Vector2>& getPositions() const noexcept { return positions; }
        std::vector<Vector2>& getPositionsMutable() noexcept { return positions; }
        bool getIsMoved() const noexcept { return is_moved; }
        std::uint_least32_t getBronze() const noexcept { return bronze; }
        std::size_t getPopulation() const noexcept { return agents.size(); }

        // Setters
        void setPosition(const Vector2& position_) noexcept {
            old_position = position;
            position = position_;
        }
        void setPositions(const std::vector<Vector2>& positions_) noexcept {
            positions = positions_;
        }
        void clearOldPosition() noexcept {
            old_position = Vector2(-1, -1);
            positions.resize(0);
        }
        void setIsMoved(bool is_moved_) noexcept {
            is_moved = is_moved_;
        }
        void setBronze(std::uint_least32_t bronze_) noexcept {
            bronze = bronze_;
        }

        // ✅ Agent management (最適化版)
        void addAgent(const Agent& agent) noexcept {
            agents.emplace_back(agent);
            agent_id_to_index_[agent.getId()] = agents.size() - 1;
        }

        void addAgents(const std::vector<Agent>& agents_) noexcept {
            const std::size_t start_index = agents.size();
            agents.insert(agents.end(), agents_.begin(), agents_.end());
            // マップ更新
            for (std::size_t i = 0; i < agents_.size(); ++i) {
                agent_id_to_index_[agents[start_index + i].getId()] = start_index + i;
            }
        }

        void clearAgents() noexcept {
            agents.clear();
            agent_id_to_index_.clear();
        }

        void resizeAgents(std::size_t size_) noexcept {
            agents.resize(size_);
            rebuildAgentIndex();
        }

        void setAgents(const std::vector<Agent>& agents_) noexcept {
            agents = agents_;
            rebuildAgentIndex();
        }

        void setAgent(const Agent& agent_, std::size_t index_) noexcept {
            // 古いIDを削除
            if (index_ < agents.size()) {
                agent_id_to_index_.erase(agents[index_].getId());
            }
            agents[index_] = agent_;
            agent_id_to_index_[agent_.getId()] = index_;
        }

        // ✅ O(1)でエージェントを検索
        Agent* findAgentById(HumanIndexType id) noexcept {
            auto it = agent_id_to_index_.find(id);
            if (it == agent_id_to_index_.end()) return nullptr;
            if (it->second >= agents.size()) return nullptr; // 安全性チェック
            return &agents[it->second];
        }

        const Agent* findAgentById(HumanIndexType id) const noexcept {
            auto it = agent_id_to_index_.find(id);
            if (it == agent_id_to_index_.end()) return nullptr;
            if (it->second >= agents.size()) return nullptr;
            return &agents[it->second];
        }

        // ✅ エージェント削除（インデックス指定）
        void removeAgentAt(std::size_t index) noexcept {
            if (index >= agents.size()) return;

            // 削除するエージェントのIDをマップから削除
            agent_id_to_index_.erase(agents[index].getId());

            // 最後の要素と入れ替え
            if (index != agents.size() - 1) {
                agents[index] = agents.back();
                // 移動したエージェントのインデックスを更新
                agent_id_to_index_[agents[index].getId()] = index;
            }
            agents.pop_back();
        }

        void deleteAgent(std::uint_least32_t id_) noexcept {
            auto it = agent_id_to_index_.find(id_);
            if (it != agent_id_to_index_.end()) {
                removeAgentAt(it->second);
            }
        }

        Agent& getAgent(std::uint_least32_t id_) noexcept {
            Agent* agent = findAgentById(id_);
            if (agent == nullptr) {
                PAXS_ERROR("Agent not found: " + std::to_string(id_));
                return *agents.begin();
            }
            return *agent;
        }

        const Agent& cgetAgent(std::uint_least32_t id_) const noexcept {
            const Agent* agent = findAgentById(id_);
            if (agent == nullptr) {
                PAXS_ERROR("Agent not found: " + std::to_string(id_));
                return *agents.begin();
            }
            return *agent;
        }

        std::vector<Agent>& getAgents() noexcept { return agents; }
        const std::vector<Agent>& cgetAgents() const noexcept { return agents; }

        // Calculation methods (const, no state change)
        double getPopulationWeight() const noexcept {
            const double ac_weight = SimulationConstants::getInstance().max_agricultural_settlement_weight;
            const double hg_weight = SimulationConstants::getInstance().max_hunter_gatherer_settlement_weight;

            double population_weight = 0;
            for (const auto& agent : agents) {
                population_weight += (agent.cgetFarming() > 0) ? ac_weight : hg_weight;
            }
            return population_weight;
        }

        std::size_t getFarmingPopulation() const noexcept {
            std::size_t farming_population = 0;
            for (std::size_t i = 0; i < agents.size(); ++i) {
                if (agents[i].cgetFarming() > 0) {
                    ++farming_population;
                }
            }
            return farming_population;
        }

        double getSNP() const noexcept {
            if (agents.empty()) return 0.0;
            std::uint_least64_t snp = 0;
            for (std::size_t i = 0; i < agents.size(); ++i) {
                snp += static_cast<std::uint_least64_t>(agents[i].cgetGenome().getSNP());
            }
            return static_cast<double>(snp) / static_cast<double>(agents.size()) / 255.0;
        }

        std::uint_least8_t getLanguage() const noexcept {
            if (agents.empty()) {
                return 0;
            }

            std::array<std::uint_least8_t, 256> language{};
            for (const auto& agent : agents) {
                ++language[agent.cgetLanguage()];
            }

            std::size_t max_count = 0;
            std::size_t max_index = 0;
            for (std::size_t i = 0; i < language.size(); ++i) {
                if (language[i] > max_count) {
                    max_count = language[i];
                    max_index = i;
                }
            }
            return static_cast<std::uint_least8_t>(max_index);
        }

        std::size_t getMostMtDNA() const noexcept {
            if (agents.empty()) return 0;
            std::size_t mtdna_max = 0;
            for (std::size_t i = 0; i < agents.size(); ++i) {
                mtdna_max += agents[i].cgetGenome().getMtDNA();
            }
            return mtdna_max / agents.size();
        }

    private:
        // インデックスマップを再構築
        void rebuildAgentIndex() {
            agent_id_to_index_.clear();
            for (std::size_t i = 0; i < agents.size(); ++i) {
                agent_id_to_index_[agents[i].getId()] = i;
            }
        }
    };

    class Settlement {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Agent = paxs::SettlementAgent;

        explicit Settlement(const std::uint_least32_t id, std::mt19937& gen_, const std::shared_ptr<Environment> env) noexcept : data_(id), gen(&gen_), environment(env) {}

        /// @brief Get the uuid.
        /// @brief idを取得
        constexpr std::uint_least32_t getId() const noexcept { return data_.getId(); }

        /// @brief Add an agent to the settlement.
        /// @brief 集落にエージェントを追加
        void addAgent(const Agent& agent) noexcept { data_.addAgent(agent); }

        /// @brief Add agents to the settlement.
        /// @brief 集落にエージェントを追加
        void addAgents(const std::vector<Agent>& agents_) noexcept {
            data_.addAgents(agents_);
        }

        /// @brief Set the agents.
        /// @brief エージェントを消去
        void clearAgents() noexcept {
            data_.clearAgents();
        }

        /// @brief Set the agents.
        /// @brief エージェントを消去
        void resizeAgents(const std::size_t size_) noexcept {
            data_.resizeAgents(size_);
        }

        /// @brief Set the agents.
        /// @brief エージェントを設定
        void setAgents(const std::vector<Agent>& agents_) noexcept {
            data_.setAgents(agents_);
        }

        /// @brief Set the agents.
        /// @brief エージェントを設定
        void setAgent(const Agent& agent_, const std::size_t index_) noexcept {
            data_.setAgent(agent_, index_);
        }

        /// @brief Delete the agent.
        /// @brief エージェントを削除する
        void deleteAgent(const std::uint_least32_t id_) noexcept {
            data_.deleteAgent(id_);
        }

        /// @brief Set the position of the settlement.
        /// @brief 集落の座標を設定
        void setPosition(const Vector2& position_) noexcept {
            data_.setPosition(position_);
        }
        /// @brief
        /// @brief 集落の過去の座標を消去
        void clearOldPosition() noexcept {
            data_.clearOldPosition();
        }

        /// @brief Get the position of the settlement.
        /// @brief 集落の座標を取得
        Vector2 getPosition() const noexcept { return data_.getPosition(); }
        Vector2 getOldPosition() const noexcept { return data_.getOldPosition(); }
        const std::vector<Vector2>& getPositions() const noexcept { return data_.getPositions(); }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        /// @param id The agent's id. エージェントのID
        Agent& getAgent(const std::uint_least32_t id_) noexcept {
            return data_.getAgent(id_);
        }

        /// @brief Get the agents.
        /// @brief エージェントを取得
        // TODO: 将来的にはcgetAgents()のみを残す
        std::vector<Agent>& getAgents() noexcept { return data_.getAgents(); }
        const Agent& cgetAgent(const std::uint_least32_t id_) const noexcept {
            return data_.cgetAgent(id_);
        }

        /// @brief Get the agents.
        /// @brief エージェントを取得
        const std::vector<Agent>& cgetAgents() const noexcept { return data_.cgetAgents(); }

        /// @brief 婚姻可能エージェントのキャッシュを更新
        void updateMarriageCache() const {
            if (marriage_cache_.is_valid) return;

            marriage_cache_.marriageable_male_indices.clear();
            marriage_cache_.marriageable_female_indices.clear();

            const std::vector<Agent>& agents = data_.cgetAgents();
            for (std::size_t i = 0; i < agents.size(); ++i) {
                if (agents[i].getLifeSpan() == 0 || !agents[i].isAbleToMarriage()) continue;

                if (agents[i].isMale()) {
                    marriage_cache_.marriageable_male_indices.push_back(i);
                }
                else if (agents[i].isFemale() && agents[i].getBirthIntervalCount() == 0) {
                    marriage_cache_.marriageable_female_indices.push_back(i);
                }
            }
            marriage_cache_.is_valid = true;
        }

        /// @brief 婚姻キャッシュを無効化
        void invalidateMarriageCache() {
            marriage_cache_.is_valid = false;
        }

        bool marriagePair(
            Marriage3& male_settlement_pair,
            std::discrete_distribution<std::size_t>& csl_dist,
            const std::vector<std::vector<Settlement>*>& close_settlements_list,
            const bool female_is_agricultural,
            JapanProvinces* japan_provinces_ptr
        ) {
            // 近隣集落グリッドを選択
            std::size_t i = csl_dist(*gen);
            if (close_settlements_list.size() <= i) return false;
            const std::vector<Settlement>& close_settlements = (*close_settlements_list[i]);

            if (close_settlements.empty()) return false;

            // 集落を選択
            std::uniform_int_distribution<int> settlement_dist(0, static_cast<int>(close_settlements.size() - 1));
            const std::size_t j = static_cast<std::size_t>(settlement_dist(*gen));
            Settlement& close_settlement = const_cast<Settlement&>(close_settlements[j]);

            // 距離チェック
            if (close_settlement.getPosition().distance_pow2(data_.getPosition()) >
                SimulationConstants::getInstance().marriage_search_range_pow2) return false;

            // ✅ キャッシュから婚姻可能な男性のみ取得
            close_settlement.updateMarriageCache();
            const auto& male_indices = close_settlement.marriage_cache_.marriageable_male_indices;

            if (male_indices.empty()) return false;

            // ランダムに男性を選択
            std::uniform_int_distribution<> male_dist(0, static_cast<int>(male_indices.size()) - 1);
            std::size_t k = male_indices[male_dist(*gen)];

            const std::vector<Agent>& close_agent = close_settlement.cgetAgents();
            const bool male_is_agricultural = (close_agent[k].cgetFarming() > 0);

            // ✅ 農耕民の婚姻時の地域チェック
            if (japan_provinces_ptr != nullptr) {
                // 女性が農耕民の場合
                if (female_is_agricultural) {
                    const std::uint_least8_t female_district_id = environment->template getData<std::uint_least8_t>(
                        SimulationConstants::getInstance().district_key, data_.getPosition());
                    const District& female_district = japan_provinces_ptr->getDistrict(female_district_id);
                    if (female_district.agricultural_capable == 0) {
                        return false; // 農耕不可能な地域
                    }
                }

                // 男性が農耕民の場合
                if (male_is_agricultural) {
                    const std::uint_least8_t male_district_id = environment->template getData<std::uint_least8_t>(
                        SimulationConstants::getInstance().district_key, close_settlement.getPosition());
                    const District& male_district = japan_provinces_ptr->getDistrict(male_district_id);
                    if (male_district.agricultural_capable == 0) {
                        return false; // 農耕不可能な地域
                    }
                }

                // 母方居住婚の移動先チェック
                if (SimulationConstants::getInstance().maternal_residence_probability > 0.0f && male_is_agricultural) {
                    const std::uint_least8_t female_district_id = environment->template getData<std::uint_least8_t>(
                        SimulationConstants::getInstance().district_key, data_.getPosition());
                    const District& female_district = japan_provinces_ptr->getDistrict(female_district_id);
                    if (female_district.agricultural_capable == 0) {
                        return false; // 移動先が農耕不可能
                    }
                }

                // 父方居住婚の移動先チェック
                if (SimulationConstants::getInstance().maternal_residence_probability < 1.0f && female_is_agricultural) {
                    const std::uint_least8_t male_district_id = environment->template getData<std::uint_least8_t>(
                        SimulationConstants::getInstance().district_key, close_settlement.getPosition());
                    const District& male_district = japan_provinces_ptr->getDistrict(male_district_id);
                    if (male_district.agricultural_capable == 0) {
                        return false; // 移動先が農耕不可能
                    }
                }
            }

            male_settlement_pair = Marriage3(
                static_cast<std::uint_least32_t>(k),
                static_cast<std::uint_least32_t>(j),
                static_cast<std::uint_least32_t>(i),
                close_agent[k].cgetFarming()
            );
            return true;
        }

        /// @brief Marriage.
        /// @brief 婚姻
        void marriage(
            const std::vector<std::vector<Settlement>*>& close_settlements_list,
            std::vector<GridType4>& marriage_pos_list,
            JapanProvinces* japan_provinces_ptr = nullptr
        ) noexcept {
            if (close_settlements_list.empty()) return;

            // ✅ 婚姻可能エージェントのキャッシュを更新
            updateMarriageCache();

            // 居住婚の判定
            bool is_matrilocality = false;
            bool is_select_matrilocality = false;

            if (SimulationConstants::getInstance().maternal_residence_probability >= 1.0f) {
                is_matrilocality = true;
            }
            else if (SimulationConstants::getInstance().maternal_residence_probability <= 0.0f) {
                is_matrilocality = false;
            }
            else {
                is_select_matrilocality = true;
            }

            // 各集落の選定重み
            std::vector<std::size_t> close_settlements_list_probabilities;
            std::discrete_distribution<std::size_t> csl_dist;

            // ✅ 婚姻可能な女性のみループ
            std::vector<Agent>& agents = data_.getAgents();
            for (std::size_t idx : marriage_cache_.marriageable_female_indices) {
                Agent& female = agents[idx];

                // 婚姻確率チェック
                if (!isMarried(female.getAgeSizeT(), female.cgetFarming() > 0)) continue;

                // ✅ 集落グリッドを重み付け（初回のみ）
                if (close_settlements_list_probabilities.empty()) {
                    for (const auto& close_settlements : close_settlements_list) {
                        close_settlements_list_probabilities.emplace_back(close_settlements->size());
                    }
                    csl_dist = std::discrete_distribution<std::size_t>(
                        close_settlements_list_probabilities.begin(),
                        close_settlements_list_probabilities.end()
                    );
                }

                Marriage3 male_settlement_pair{};
                bool pair_result = false;
                const bool female_is_agricultural = (female.cgetFarming() > 0);

                // ✅ 最大50回試行
                for (std::size_t pair_loop = 0; pair_loop < 50 && !pair_result; ++pair_loop) {
                    pair_result = marriagePair(male_settlement_pair, csl_dist, close_settlements_list,
                        female_is_agricultural, japan_provinces_ptr);
                }

                if (!pair_result) continue; // ペアが見つからなかった

                // ✅ 婚姻処理
                const std::uint_least32_t pair_agent_index = male_settlement_pair.first;
                const std::uint_least32_t pair_settlement_index = male_settlement_pair.second;
                Settlement& pair_settlement = (*(close_settlements_list[male_settlement_pair.third]))[pair_settlement_index];
                Agent& male = pair_settlement.getAgents()[pair_agent_index];

                // 選択居住婚の判定
                if (is_select_matrilocality) {
                    is_matrilocality = (SimulationConstants::getInstance().maternal_residence_probability >=
                        SimulationConstants::getInstance().random_dist_f32(*gen));
                }

                // 母方居住婚
                if (is_matrilocality) {
                    Agent male_copy = male;
                    male.setLifeSpan(0);
                    male.setPartnerId(0);
                    female.marry(male_copy.getId(), male_copy.cgetGenome(), male_copy.cgetFarming(),
                        male_copy.cgetHunterGatherer(), male_copy.cgetLanguage());
                    male_copy.marry(female.getId(), female.cgetGenome(), female.cgetFarming(),
                        female.cgetHunterGatherer(), female.cgetLanguage());
                    marriage_pos_list.emplace_back(GridType4{
                        pair_settlement.data_.getPosition().x, pair_settlement.data_.getPosition().y,
                        data_.getPosition().x, data_.getPosition().y,
                        is_matrilocality
                        });
                    data_.addAgent(male_copy);
                    pair_settlement.invalidateMarriageCache(); // ✅ 相手集落のキャッシュ無効化
                }
                // 父方居住婚
                else {
                    Agent female_copy = female;
                    female.setLifeSpan(0);
                    female.setPartnerId(0);
                    female_copy.marry(male.getId(), male.cgetGenome(), male.cgetFarming(),
                        male.cgetHunterGatherer(), male.cgetLanguage());
                    male.marry(female_copy.getId(), female_copy.cgetGenome(), female_copy.cgetFarming(),
                        female_copy.cgetHunterGatherer(), female_copy.cgetLanguage());
                    marriage_pos_list.emplace_back(GridType4{
                        data_.getPosition().x, data_.getPosition().y,
                        pair_settlement.data_.getPosition().x, pair_settlement.data_.getPosition().y,
                        is_matrilocality
                        });
                    pair_settlement.getAgents().emplace_back(female_copy);
                    pair_settlement.invalidateMarriageCache(); // ✅ 相手集落のキャッシュ無効化
                }
            }

            invalidateMarriageCache(); // ✅ 自集落のキャッシュ無効化
        }

        /// @brief Pre update.
        /// @brief 事前更新
        void preUpdate(KanakumaLifeSpan& kanakuma_life_span, JapanProvinces* japan_provinces_ptr = nullptr) noexcept {
            birth(kanakuma_life_span, japan_provinces_ptr);
            invalidateMarriageCache(); // ✅ 出産後にキャッシュ無効化
        }

        /// @brief Death.
        /// @brief 死亡
        void death() noexcept {
            std::vector<Agent>& agents_ref = data_.getAgents();
            for (std::size_t i = 0; i < agents_ref.size();) {
                agents_ref[i].incrementAge();
                if (!(agents_ref[i].isDead())) {
                    ++i;
                    continue;
                }

                const HumanIndexType partner_id = agents_ref[i].getPartnerId();
                if (partner_id != 0) {
                    // ✅ O(1)でパートナーを検索
                    Agent* partner = data_.findAgentById(partner_id);
                    if (partner) {
                        partner->divorce();
                    }
                }

                // ✅ O(1)で削除（インデックス管理込み）
                data_.removeAgentAt(i);
                // インデックスはそのまま（removeAgentAtで最後の要素が移動するため）
            }
            invalidateMarriageCache(); // ✅ 死亡後にキャッシュ無効化
        }

        // 移動先を計算する（moveAStarより前に定義）
        Vector2 calcMovePosition(std::mt19937& engine, District& district_, const Vector2& current_position, const int distance,
            const bool has_agricultural_agents, JapanProvinces* japan_provinces_ptr) const noexcept {
            Vector2 target_position = current_position;
            std::uint_least32_t loop_count = 0;

            while (loop_count < SimulationConstants::getInstance().move_redo) {
                if (loop_count >= SimulationConstants::getInstance().move_redo) return current_position;

                const double theta = (distance >= static_cast<int>(district_.direction_min_distance)) ?
                    SimulationConstants::getInstance().theta_dist_array[district_.direction_dist(engine)](engine) :
                    SimulationConstants::getInstance().theta_dist(engine);
                target_position = current_position + Vector2(static_cast<GridType>(std::cos(theta) * distance), static_cast<GridType>(std::sin(theta) * distance));

                // 基本的な可住地チェック
                if (target_position == current_position || !environment->isLive(target_position)) {
                    ++loop_count;
                    continue;
                }

                // 農耕民がいる場合、農耕不可能地域への移動を禁止
                if (has_agricultural_agents && japan_provinces_ptr != nullptr) {
                    const std::uint_least8_t target_district_id = environment->template getData<std::uint_least8_t>(
                        SimulationConstants::getInstance().district_key, target_position);
                    const District& target_district = japan_provinces_ptr->getDistrict(target_district_id);
                    if (target_district.agricultural_capable == 0) {
                        ++loop_count;
                        continue;
                    }
                }

                // すべてのチェックをパスした場合のみループを抜ける
                break;
            }

            // 移動先が見つからなかった場合は現在位置を返す
            if (loop_count >= SimulationConstants::getInstance().move_redo) {
                return current_position;
            }
            return target_position;
        }

        // A* の経路探索
        void moveAStar(std::mt19937& engine, District& district_, const Vector2& current_position, Vector2& target_position,
            const bool has_agricultural_agents, JapanProvinces* japan_provinces_ptr) noexcept {
            double cost = -1.0;
            const int distance = SimulationConstants::getInstance().move_dist(engine);

            const GridType cw = SimulationConstants::getInstance().move_astar_distance;
            const Vector2 cp_cw = current_position / cw;

#ifdef _OPENMP
            double max_cost = 1.0;
            const int loop = SimulationConstants::getInstance().move_astar_loop;
            std::vector<double> cost_list(loop);
            std::vector<Vector2> pos_list(loop);
            std::vector<Vector2> move_list(loop);
            std::vector<std::uint_least8_t> slope_list(loop);
            std::vector<std::vector<Vector2>> route_list(loop);

            // ✅ 有効な移動先のみを収集
            int valid_count = 0;
            for (int i = 0; i < loop; ++i) {
                Vector2 candidate = calcMovePosition(engine, district_, current_position, distance, has_agricultural_agents, japan_provinces_ptr);

                // ✅ 移動先が現在位置と異なるかチェック
                if (candidate == current_position) continue;

                const Vector2 mp_cw = candidate / cw;

                // ✅ スケール後の座標が同じまたは隣接の場合はスキップ
                if (cp_cw == mp_cw || (std::abs(cp_cw.x - mp_cw.x) <= 1 && std::abs(cp_cw.y - mp_cw.y) <= 1)) {
                    continue;
                }

                move_list[valid_count] = candidate;
                slope_list[valid_count] = environment->getSlope(candidate);
                ++valid_count;

                if (valid_count >= loop) break;
            }

            // ✅ 有効な移動先がない場合は早期リターン
            if (valid_count == 0) {
                PAXS_WARNING("No valid move positions found for A*");
                return;
            }

            // 最初の有効な移動先をデフォルトに設定
            target_position = move_list[0];

#pragma omp parallel for
            for (int i = 0; i < valid_count; ++i) {
#else
            // ✅ 非OpenMP版も同様に修正
            int valid_attempts = 0;
            for (std::uint_least32_t i = 0; i < SimulationConstants::getInstance().move_astar_loop; ++i) {
#endif
#ifdef _OPENMP
                const Vector2 move_position = move_list[i];
#else
                Vector2 move_position = calcMovePosition(engine, district_, current_position, distance, has_agricultural_agents, japan_provinces_ptr);

                // ✅ 移動先が現在位置と同じならスキップ
                if (move_position == current_position) continue;

                const Vector2 mp_cw = move_position / cw;

                // ✅ スケール後の座標が同じまたは隣接の場合はスキップ
                if (cp_cw == mp_cw || (std::abs(cp_cw.x - mp_cw.x) <= 1 && std::abs(cp_cw.y - mp_cw.y) <= 1)) {
                    continue;
                }

                ++valid_attempts;
#endif
                AStar astar(current_position, move_position, cw);
                if (!astar.search(environment)) {
                    // 経路が見つからない場合はスキップ
                    continue;
                }

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
                    astar.setPath(data_.getPositionsMutable());
                }
#endif
            }

#ifdef _OPENMP
            // ✅ 有効な経路の中から最良のものを選択
            int index_num = -1;
            for (int i = 0; i < valid_count; ++i) {
                if (route_list[i].empty()) continue; // 経路が見つからなかった

                if (max_cost < cost_list[i]) {
                    max_cost = cost_list[i];
                }
            }

            for (int i = 0; i < valid_count; ++i) {
                if (route_list[i].empty()) continue;

                // 傾斜含むコスト
                const double slope_cost = cost_list[i] + max_cost * ((slope_list[i] <= 129) ? 0 : slope_list[i] / 250.0);
                if (cost == -1.0 || cost > slope_cost) {
                    cost = slope_cost;
                    index_num = i;
                }
            }

            if (index_num >= 0) {
                target_position = pos_list[index_num];
                data_.setPositions(route_list[index_num]);
            }
            else {
                PAXS_WARNING("No valid A* path found");
            }
#else
            if (cost == -1.0) {
                PAXS_WARNING("No valid A* path found (non-OpenMP)");
            }
#endif
            }

        /// @brief Move.
        /// @brief 移動
        /// @return 集落グリッドを移動したかどうか
        std::tuple<std::uint_least32_t, Vector2, Vector2> move(std::mt19937 & engine, District & district_, JapanProvinces * japan_provinces_ptr = nullptr) noexcept {
            // 確率で移動
            if (SimulationConstants::getInstance().random_dist(engine) > SimulationConstants::getInstance().move_probability) return { 0, Vector2(), Vector2() };

            Vector2 current_key;
            Vector2 target_key;

            // 座標を移動
            // 移動距離0~max_move_distance

            Vector2 current_position = data_.getPosition();
            Vector2 target_position = data_.getPosition();

            // 集落に農耕民がいるかチェック
            const bool has_agricultural_agents = (getFarmingPopulation() > 0);

            // A* を使った方法
            if (SimulationConstants::getInstance().move_method == MurMur3::calcHash("astar") && SimulationConstants::getInstance().move_astar_loop >= 1) {
                moveAStar(engine, district_, current_position, target_position, has_agricultural_agents, japan_provinces_ptr);
            }
            else {
                std::uint_least32_t loop_count = 0;
                while (loop_count < SimulationConstants::getInstance().move_redo) {
                    int distance = SimulationConstants::getInstance().move_dist(engine);

                    // 移動距離が偏りのある方向を指定する距離以上か判定し、方向を格納する
                    const double theta = (distance >= static_cast<int>(district_.direction_min_distance)) ?
                        SimulationConstants::getInstance().theta_dist_array[district_.direction_dist(engine)](engine) :
                        SimulationConstants::getInstance().theta_dist(engine);
                    target_position = current_position + Vector2(static_cast<GridType>(std::cos(theta) * distance), static_cast<GridType>(std::sin(theta) * distance));

                    // 基本的な可住地チェック
                    if (target_position == current_position || !environment->isLive(target_position)) {
                        ++loop_count;
                        continue;
                    }

                    // 農耕民がいる場合、移動先が農耕不可能地域なら移動不可
                    if (has_agricultural_agents && japan_provinces_ptr != nullptr) {
                        const std::uint_least8_t target_district_id = environment->template getData<std::uint_least8_t>(
                            SimulationConstants::getInstance().district_key, target_position);
                        const District& target_district = japan_provinces_ptr->getDistrict(target_district_id);
                        if (target_district.agricultural_capable == 0) {
                            ++loop_count;
                            continue;
                        }
                    }

                    // すべてのチェックをパスした場合のみループを抜ける
                    break;
                }

                // 移動先が見つからなかった場合は移動しない
                if (loop_count >= SimulationConstants::getInstance().move_redo) {
                    return { 0, Vector2(), Vector2() };
                }
            }
            current_key = current_position / SimulationConstants::getInstance().cell_group_length;
            target_key = target_position / SimulationConstants::getInstance().cell_group_length;

            if (current_key == target_key) {
                return { 0, Vector2(), Vector2() };
            }

            data_.setIsMoved(true);

            return { data_.getId(), current_key, target_key };
        }

        /// @brief Get the is_moved.
        /// @brief 移動したかどうかを取得
        bool isMoved() const noexcept { return data_.getIsMoved(); }

        void setIsMoved(const bool is_moved_) noexcept {
            data_.setIsMoved(is_moved_);
        }

        /// @brief Get the Bronze.
        /// @brief 青銅を取得
        std::uint_least32_t getBronze() const noexcept {
            return data_.getBronze();
        }
        void setBronze(const std::uint_least32_t bronze_) noexcept {
            data_.setBronze(bronze_);
        }

        /// @brief Get the population.
        /// @brief 人口を取得
        std::size_t getPopulation() const noexcept { return data_.getPopulation(); }

        /// @brief Get the weight population.
        /// @brief 重み人口を取得
        double getPopulationWeight() const noexcept {
            return data_.getPopulationWeight();
        }

        /// @brief Get the population.
        /// @brief 渡来人口を取得
        std::size_t getFarmingPopulation() const noexcept {
            return data_.getFarmingPopulation();
        }

        /// @brief Get the SNP.
        /// @brief SNP を取得
        double getSNP() const noexcept {
            return data_.getSNP();
        }

        /// @brief Get the Language.
        /// @brief 言語を取得
        std::uint_least8_t getLanguage() const noexcept {
            return data_.getLanguage();
        }

        /// @brief Get the most mtDNA.
        /// @brief 最多 mtDNA を取得
        std::size_t getMostMtDNA() const noexcept {
            return data_.getMostMtDNA();
        }

        /// @brief Divide the settlement.
        /// @brief 集落を分割
        Settlement divide() noexcept {
            std::vector<Agent>& agents_ref = data_.getAgents();
            std::vector<Agent> new_settlement_agents = std::vector<Agent>(
                agents_ref.begin() + agents_ref.size() / 2, agents_ref.end());
            agents_ref.resize(agents_ref.size() / 2);

            // ✅ 一時的なインデックスマップを構築 O(n)
            std::unordered_map<HumanIndexType, std::size_t> new_agents_index;
            for (std::size_t i = 0; i < new_settlement_agents.size(); ++i) {
                new_agents_index[new_settlement_agents[i].getId()] = i;
            }

            std::unordered_map<HumanIndexType, std::size_t> current_agents_index;
            for (std::size_t i = 0; i < agents_ref.size(); ++i) {
                current_agents_index[agents_ref[i].getId()] = i;
            }

            // ✅ パートナー同士を同じ集落に O(n)
            const std::size_t original_agents_ref_size = agents_ref.size();
            for (std::size_t i = 0; i < original_agents_ref_size; ++i) {
                const Agent& agent = agents_ref[i];
                if (agent.isMarried() && agent.isFemale()) {
                    auto it = new_agents_index.find(agent.getPartnerId());
                    if (it != new_agents_index.end()) {
                        std::size_t partner_idx = it->second;
                        agents_ref.emplace_back(new_settlement_agents[partner_idx]);
                        current_agents_index[new_settlement_agents[partner_idx].getId()] = agents_ref.size() - 1;

                        // 削除処理
                        new_settlement_agents[partner_idx] = new_settlement_agents.back();
                        if (partner_idx < new_settlement_agents.size() - 1) {
                            new_agents_index[new_settlement_agents[partner_idx].getId()] = partner_idx;
                        }
                        new_agents_index.erase(it);
                        new_settlement_agents.pop_back();
                    }
                }
            }

            // 逆方向も同様に処理
            const std::size_t original_new_settlement_agents_size = new_settlement_agents.size();
            for (std::size_t i = 0; i < original_new_settlement_agents_size; ++i) {
                const Agent& agent = new_settlement_agents[i];
                if (agent.isMarried() && agent.isFemale()) {
                    auto it = current_agents_index.find(agent.getPartnerId());
                    if (it != current_agents_index.end()) {
                        std::size_t partner_idx = it->second;
                        new_settlement_agents.emplace_back(agents_ref[partner_idx]);
                        new_agents_index[agents_ref[partner_idx].getId()] = new_settlement_agents.size() - 1;

                        // 削除処理
                        agents_ref[partner_idx] = agents_ref.back();
                        if (partner_idx < agents_ref.size() - 1) {
                            current_agents_index[agents_ref[partner_idx].getId()] = partner_idx;
                        }
                        current_agents_index.erase(it);
                        agents_ref.pop_back();
                    }
                }
            }

            // 新しい集落を作成
            Settlement new_settlement = Settlement(UniqueIdentification<std::uint_least32_t>::generate(), *gen, environment);
            new_settlement.setAgents(new_settlement_agents);
            return new_settlement;
        }

    private:
        /// @brief Settlement data
        /// @brief 集落データ
        SettlementData data_;

        std::mt19937* gen{}; // 乱数生成器
        std::shared_ptr<Environment> environment{}; // 環境

        // 婚姻可能エージェントのキャッシュ
        struct MarriageCache {
            std::vector<std::size_t> marriageable_male_indices;
            std::vector<std::size_t> marriageable_female_indices;
            bool is_valid = false;
        };
        mutable MarriageCache marriage_cache_;

        /// @brief Birth.
        /// @brief 出産
        void birth(KanakumaLifeSpan & kanakuma_life_span, JapanProvinces * japan_provinces_ptr = nullptr) noexcept {
            std::vector<Agent> children;
            std::vector<Agent>& agents_ref = data_.getAgents();

            // 集落の位置の地区IDと農耕可能性をチェック
            bool is_agricultural_capable = true;
            if (japan_provinces_ptr != nullptr) {
                const std::uint_least8_t current_district_id = environment->template getData<std::uint_least8_t>(
                    SimulationConstants::getInstance().district_key, data_.getPosition());
                const District& current_district = japan_provinces_ptr->getDistrict(current_district_id);
                is_agricultural_capable = (current_district.agricultural_capable != 0);
            }

            for (auto& agent : agents_ref) {
                if (agent.getBirthIntervalCount() > 0) {
                    std::uint_least8_t count = agent.decrementBirthIntervalCount();
                    if (count == 0) {
                        // 生業文化別の死産率を格納
                        const double stillbirth_rate = (agent.cgetFarming() > 0) ?
                            SimulationConstants::getInstance().agricultural_stillbirth_rate :
                            SimulationConstants::getInstance().hunter_gatherer_stillbirth_rate;

                        // 死産率 100 ％の場合は出産しない
                        if (stillbirth_rate >= 1.0f) continue;
                        else if (stillbirth_rate > 0.0f) {
                            // 死産
                            if (SimulationConstants::getInstance().random_dist_f32(*gen) < stillbirth_rate) continue;
                        }
                        std::uint_least8_t farming =
                            // 両親が農耕文化であれば両親の半分の値を引き継ぐ
                            (agent.cgetFarming() > 0 && agent.cgetPartnerFarming() > 0) ?
                            static_cast<std::uint_least8_t>((int(agent.cgetFarming()) + int(agent.cgetPartnerFarming())) / 2)
                            // 両親が共に農耕文化を持たない場合は 0
                            : ((agent.cgetFarming() == 0 && agent.cgetPartnerFarming() == 0) ? 0
                                // 片親が農耕文化を持つ場合は乱数
                                : ((SimulationConstants::getInstance().random_dist_f32(*gen) < SimulationConstants::getInstance().child_agriculture_priority) ?
                                    // 農耕文化を持つ親から値を引き継ぐ
                                    ((agent.cgetFarming() == 0) ? agent.cgetPartnerFarming() : agent.cgetFarming()) :
                                    0
                                    ));

                        // 農耕不可能な地域では子供の農耕変数を0に設定
                        if (!is_agricultural_capable) {
                            farming = 0;
                        }

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
                    agent.setBirthIntervalCount(SimulationConstants::getInstance().birth_interval);
                }

            }
            // 新しい赤ちゃんがいない場合はエージェントを追加をしない
            if (children.size() == 0) return;
            data_.addAgents(children);
        }

        /// @brief Is the agent married?
        /// @brief 確率で結婚するかどうかを返す
        bool isMarried(const std::size_t age, const bool is_agricultural) noexcept {
            // 婚姻可能年齢の上限値以上だったら結婚しない
            const double threshold = SimulationConstants::getInstance().getMarriageProbability(age, is_agricultural);
            if (threshold == 0.0) return false;
            if (threshold >= 1.0) return true;
            return SimulationConstants::getInstance().random_dist(*gen) < threshold;
        }

        /// @brief Is able to give birth?
        /// @brief 確率で出産するかどうかを返す
        bool isAbleToGiveBirth(const std::size_t age, const bool is_agricultural) noexcept {
            const double threshold = SimulationConstants::getInstance().getChildbearingProbability(age, is_agricultural);
            if (threshold == 0.0) return false;
            if (threshold >= 1.0) return true;
            return SimulationConstants::getInstance().random_dist(*gen) < threshold;
        }
        };

    }

#endif // !PAX_SAPIENTICA_SIMULATION_ENTITY_SETTLEMENT_HPP
