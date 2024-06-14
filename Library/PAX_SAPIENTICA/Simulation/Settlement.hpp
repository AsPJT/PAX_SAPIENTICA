/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SETTLEMENT_HPP
#define PAX_SAPIENTICA_SETTLEMENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstdint>

#include <algorithm>
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

// M_PI が定義されていない場合
#ifndef M_PI
#define M_PI 3.141592653589
#endif

namespace paxs {

    namespace settlement {

        // std::sqrt(2 * M_PI)
        constexpr double sqrt_2_x_pi = static_cast<double>(2.506628275);

        // 結婚・出産の定数
        constexpr double sigma = 0.25;
        constexpr double sigma_p_2 = sigma * sigma;
        constexpr double sigma_p_2_x_2 = (2.0 * sigma_p_2);

        // sigma * sqrt_2_x_pi
        constexpr double sigma_x_sqrt_2_x_pi = sigma * sqrt_2_x_pi;
    }

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
            position = position_;
        }

        /// @brief Get the position of the settlement.
        /// @brief 集落の座標を取得
        Vector2 getPosition() const noexcept { return position; }

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
        const std::vector<Agent> cgetAgents() const noexcept { return agents; }

        /// @brief Marriage.
        /// @brief 婚姻
        void marriage(std::vector<Settlement*> close_settlements,
            std::function<void(const paxs::SettlementAgent, const std::uint_least32_t, const Vector2)> add_agent,
            std::function<void(const std::uint64_t, const std::uint_least32_t, const Vector2)> delete_agent
        ) noexcept {
            // 結婚の条件を満たすエージェントを取得
            std::vector<std::size_t> marriageable_female_index;
            for (std::size_t i = 0; i < agents.size(); ++i) {
                // 結婚可能かどうか
                if (agents[i].isAbleToMarriage() && agents[i].getGender() == female_value) {
                    if (!isMarried(agents[i].getAge())) continue;
                    // 妊娠していたら婚姻しない（婚姻可能と定義すると再婚者のデータで上書きされ子供への継承が不自然になる）
                    if (agents[i].getBirthIntervalCount() > 0) continue;

                    marriageable_female_index.emplace_back(i);
                }
            }

            // 結婚の条件を満たすエージェントがいない
            if (marriageable_female_index.empty()) {
                return;
            }

            // 近隣の集落を探す
            for (std::size_t i = 0; i < close_settlements.size();) {
                if (close_settlements[i]->getPosition().distance(position) > SimulationConstants::getInstance()->marriage_search_range) {
                    close_settlements[i] = close_settlements.back(); // 同義 it = close_settlements.erase(it);
                    close_settlements.pop_back();
                }
                else {
                    ++i;
                }
            }

            // 自分の集落を含めて、近くに集落がない
            if (close_settlements.empty()) {
                PAXS_ERROR("Settlement not found.");
                return;
            }

            // idで自分を探す
            auto it = std::find_if(close_settlements.begin(), close_settlements.end(), [this](const Settlement* const settlement) { return settlement->getId() == id; });
            if (it == close_settlements.end()) {
                PAXS_ERROR("Settlement not found.");
                return;
            }

            // エージェントIDと集落IDのペアを作成
            std::vector<std::pair<std::uint_least32_t, std::uint_least32_t>> male_settlement_pair;
            for (auto& close_settlement : close_settlements) {
                for (auto& agent : close_settlement->cgetAgents()) {
                    if (agent.isAbleToMarriage() && agent.getGender() == male_value) {
                        male_settlement_pair.emplace_back(agent.getId(), close_settlement->getId());
                    }
                }
            }

            // 女性と男性の組み合わせをランダムに選択
            RandomSelector selector(gen);

            // first: 女性のインデックス, second: 男性のインデックス
            const auto marriageable_agents_index_pair = selector.select(marriageable_female_index.size(), male_settlement_pair.size());

            // 居住婚
            bool is_matrilocality = false;

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
                is_matrilocality = (SimulationConstants::getInstance()->maternal_residence_probability >= SimulationConstants::getInstance()->random_dist_f32(*gen));
            }

            // シミュレーションの設定で母方に移住するか父方に移住するかを決める
            // 母方の場合
            // if (isMatrilocality()) {
            for (std::size_t i = 0; i < marriageable_agents_index_pair.size(); ++i) {
                std::pair<std::size_t, std::size_t> index_pair = marriageable_agents_index_pair[i];
                std::uint_least32_t male_id = male_settlement_pair[index_pair.second].first;
                std::uint_least32_t male_settlement_id = male_settlement_pair[index_pair.second].second;

                bool is_found = false;
                Vector2 male_settlement_position;
                for (std::size_t j = 0; j < close_settlements.size(); ++j) {
                    if (close_settlements[j]->getId() == male_settlement_id) {

                        if (index_pair.first >= marriageable_female_index.size()) {
                            PAXS_ERROR("The FIRST of index_pair is larger than the size of marriageable_female_index.");
                            continue;
                        }
                        if (marriageable_female_index[index_pair.first] >= agents.size()) {
                            PAXS_ERROR("marriageable_female_index is larger than the size of AGENTS.");
                            continue;
                        }
                        male_settlement_position = close_settlements[j]->getPosition();
                        if (is_matrilocality) {
                            Agent male_ = close_settlements[j]->getAgentCopy(male_id);
                            Agent& female_ = agents[marriageable_female_index[index_pair.first]];

                            female_.marry(male_id, male_.cgetGenome(), male_.cgetFarming(), male_.cgetHunterGatherer());
                            const HumanIndexType female_id = female_.getId();

                            male_.marry(female_id, female_.cgetGenome(), female_.cgetFarming(), female_.cgetHunterGatherer());
                            agents.emplace_back(male_);
                        }
                        else {
                            Agent& male_ = close_settlements[j]->getAgent(male_id);
                            Agent female_ = agents[marriageable_female_index[index_pair.first]];

                            female_.marry(male_id, male_.cgetGenome(), male_.cgetFarming(), male_.cgetHunterGatherer());
                            const HumanIndexType female_id = female_.getId();

                            male_.marry(female_id, female_.cgetGenome(), female_.cgetFarming(), female_.cgetHunterGatherer());

                            male_settlement_position /= SimulationConstants::getInstance()->grid_length;
                            add_agent(female_, male_settlement_id, male_settlement_position);
                        }
                        is_found = true;
                        break;
                    }
                }

                if (!is_found) {
                    PAXS_ERROR("Settlement not found.");
                    continue;
                }

                if (is_matrilocality) {
                    male_settlement_position /= SimulationConstants::getInstance()->grid_length;
                    delete_agent(male_id, male_settlement_id, male_settlement_position);
                }
                else {
                    for (std::size_t j = i + 1; j < marriageable_agents_index_pair.size(); ++j) {
                        if (marriageable_female_index[marriageable_agents_index_pair[j].first] < marriageable_female_index[index_pair.first]) continue;
                        marriageable_female_index[marriageable_agents_index_pair[j].first] -= 1;
                    }
                    auto& id_ = marriageable_female_index[index_pair.first];
                    agents.erase(agents.begin() + id_);
                }
            }
        }

        /// @brief Pre update.
        /// @brief 事前更新
        void preUpdate(KanakumaLifeSpan& kanakuma_life_span) noexcept {
            birth(kanakuma_life_span);
            //emigration(kanakuma_life_span);
        }

        /// @brief On update.
        /// @brief 更新
        void onUpdate() noexcept {
            ageUpdate();
            death();
            is_moved = false;
        }

        /// @brief Move.
        /// @brief 移動
        /// @return 集落グリッドを移動したかどうか
        std::tuple<std::uint_least32_t, Vector2, Vector2> move(std::mt19937& engine, int move_probability) noexcept {
            Vector2 current_key;
            Vector2 target_key;

            // 確率で移動
            std::uniform_int_distribution<> dist(0, SimulationConstants::getInstance()->move_probability_normalization_coefficient);
            if (dist(engine) > move_probability) return { 0, Vector2(), Vector2() };

            // 座標を移動
            // 移動距離0~max_move_distance
            std::uniform_int_distribution<> move_dist(SimulationConstants::getInstance()->min_move_distance, SimulationConstants::getInstance()->max_move_distance);
            std::uniform_real_distribution<float> theta_dist(0.0f, static_cast<float>(2.0 * M_PI));

            Vector2 current_position = position;
            Vector2 target_position = current_position;

            // 小さい領域のシミュレーションでは無限ループする可能性がある
            while (target_position == current_position || !environment->isLive(target_position)) {
                float theta = theta_dist(engine);
                int distance = move_dist(engine);
                target_position = current_position + Vector2(static_cast<GridType>(std::cos(theta) * distance), static_cast<GridType>(std::sin(theta) * distance));
            }

            current_key = current_position / SimulationConstants::getInstance()->grid_length;
            target_key = target_position / SimulationConstants::getInstance()->grid_length;

            if (current_key == target_key) return { 0, Vector2(), Vector2() };

            is_moved = true;

            return { id, current_key, target_key };
        }

        /// @brief Get the is_moved.
        /// @brief 移動したかどうかを取得
        bool isMoved() const noexcept { return is_moved; }

        /// @brief Get the population.
        /// @brief 人口を取得
        std::size_t getPopulation() const noexcept { return agents.size(); }

        /// @brief Get the weight population.
        /// @brief 重み人口を取得
        double getPopulationWeight() const noexcept {
            double population_weight = 0;

            for (std::size_t i = 0; i < agents.size(); ++i) {
                if (agents[i].cgetFarming() > 0) { // 農耕
                    population_weight += SimulationConstants::getInstance()->max_farming_settlement_weight;
                }
                else { // 狩猟採集
                    population_weight += SimulationConstants::getInstance()->max_hunter_gatherer_settlement_weight;
                }
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

        /// @brief Get the most mtDNA.
        /// @brief 最多 mtDNA を取得
        std::size_t getMostMtDNA() const noexcept {
            //std::vector<std::uint_least8_t> dna_list{};
            //std::vector<std::uint_least8_t> dna_count_list{};

            std::size_t mtdna_max = 0;

            for (std::size_t i = 0; i < agents.size(); ++i) {
                //for (std::size_t i = 0; i < agents.size(); ++i) {
                    //if (dna == agents[i].cgetGenome().getMtDNA())
                //}
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
                if (agent.isMarried() && agent.getGender() == female_value) {
                    auto it = std::find_if(new_settlement_agents.begin(), new_settlement_agents.end(), [agent](const Agent& a) { return a.getId() == agent.getPartnerId(); });
                    if (it != new_settlement_agents.end()) {
                        agents.emplace_back(*it);
                        (*it) = new_settlement_agents.back(); // 同義 new_settlement_agents.erase(it);
                        new_settlement_agents.pop_back();
                    }
                }
            }

            for (Agent& agent : new_settlement_agents) {
                if (agent.isMarried() && agent.getGender() == female_value) {
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
                            SimulationConstants::getInstance()->farming_stillbirth_rate :
                            SimulationConstants::getInstance()->hunter_gatherer_stillbirth_rate;

                        // 死産率 100 ％の場合は出産しない
                        if (stillbirth_rate >= 1.0f) continue;
                        else if (stillbirth_rate > 0.0f) {
                            // 死産
                            if (SimulationConstants::getInstance()->random_dist_f32(*gen) < stillbirth_rate) continue;
                        }
                        // TODO: 直す
                        //if (!agent.isMarried()) continue;
                        Genome genome = Genome::generateFromParents(*gen, agent.cgetGenome(), agent.cgetPartnerGenome());
                        children.emplace_back(Agent(
                            UniqueIdentification<HumanIndexType>::generate(),
                            //0, // TODO: 名前ID
                            0,
                            kanakuma_life_span.setLifeSpan(genome.getGender(), *gen),
                            genome,
                            (agent.cgetFarming() > 0 && agent.cgetPartnerFarming() > 0) ? 255 :(
                                (agent.cgetFarming() == 0 && agent.cgetPartnerFarming() == 0) ? 0 : (
                                (SimulationConstants::getInstance()->random_dist_f32(*gen) < SimulationConstants::getInstance()->child_agriculture_priority) ? 255 : 0
                            )),
                            //(((*gen)() % 2) == 0) ? agent.cgetFarming() : agent.cgetPartnerFarming(),
                            (((*gen)() % 2) == 0) ? agent.cgetHunterGatherer() : agent.cgetPartnerHunterGatherer()
                        ));
                    }
                }
                // 出産可能かどうか
                else if (agent.isAbleToGiveBirth() && isAbleToGiveBirth(agent.getAge())) {
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
                const AgeType set_lifespan = kanakuma_life_span.setLifeSpan(genome.getGender(), *gen);

                std::uniform_int_distribution<> lifespan_dist{
                    (std::min)(18 * SimulationConstants::getInstance()->steps_per_year + 1, static_cast<int>(set_lifespan - 1)),
                    static_cast<int>(set_lifespan - 1) }; // 性別の乱数分布

                agents.emplace_back(Agent(
                    UniqueIdentification<HumanIndexType>::generate(),
                    //0, // TODO: 名前ID
                    static_cast<AgeType>(lifespan_dist(*gen)),
                    set_lifespan,
                    genome,
                    255, // ((gen() % 2) == 0) ? agent.cgetFarming() : agent.cgetPartnerFarming(),
                    0 // ((gen() % 2) == 0) ? agent.cgetHunterGatherer() : agent.cgetPartnerHunterGatherer()
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

        /// @brief Death.
        /// @brief 死亡
        void death() noexcept {
            for (auto i = 0; i < agents.size();) {
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

        /// @brief Is the agent married?
        /// @brief 確率で結婚するかどうかを返す
        bool isMarried(const double age) noexcept {
            auto x = [](double age) { return (age - 13) / 8.5; };
            auto weight = [=](double age) {
                return std::exp(-std::pow(std::log(x(age)), 2) / settlement::sigma_p_2_x_2) / (x(age) * settlement::sigma_x_sqrt_2_x_pi);
                };

            const double threshold = static_cast<double>(weight(age)) * (0.98f / 101.8f);

            return SimulationConstants::getInstance()->random_dist(*gen) < threshold;
        }

        /// @brief Is able to give birth?
        /// @brief 確率で出産するかどうかを返す
        bool isAbleToGiveBirth(const double age) noexcept {
            auto x = [](double age) { return (age - 14) / 8.5; };
            auto weight = [=](double age) {
                return std::exp(-std::pow(std::log(x(age)), 2) / settlement::sigma_p_2_x_2) / (x(age) * settlement::sigma_x_sqrt_2_x_pi);
                };

            const double threshold = static_cast<double>(weight(age)) * (16.0f / 101.8f);

            return SimulationConstants::getInstance()->random_dist(*gen) < threshold;
        }
    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_HPP
