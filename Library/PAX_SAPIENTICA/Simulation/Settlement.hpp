/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
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
#include <PAX_SAPIENTICA/Simulation/Object.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementAgent.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/UniqueIdentification.hpp>

// M_PI が定義されていない場合
#ifndef M_PI
#define M_PI 3.141592653589
#endif

namespace paxs {

    template <typename GridType>
    class Settlement {
    public:
        using Environment = paxs::Environment<GridType>;
        using Vector2 = paxs::Vector2<GridType>;
        using Object = paxs::Object<GridType>;
        using Agent = paxs::SettlementAgent<GridType>;

        constexpr explicit Settlement(const std::uint_least32_t id, const unsigned seed, const std::shared_ptr<Environment> env) noexcept : id(id), gen(seed), environment(env) {}

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
        void deleteAgent(const std::uint_least64_t id_) noexcept {
            agents.erase(std::remove_if(agents.begin(), agents.end(), [id_](const Agent& agent) { return agent.getId() == id_; }), agents.end());
        }

        /// @brief Set the position of the settlement.
        /// @brief 集落の座標を設定
        void setPosition(const Vector2& position) noexcept {
            positions = { position };
        }

        /// @brief Get the position of the settlement.
        /// @brief 集落の座標を取得
        Vector2 getPosition() const noexcept { return positions.back(); }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        /// @param id The agent's id. エージェントのID
        Agent getAgent(const std::uint_least64_t id_) const {
            auto it = std::find_if(agents.begin(), agents.end(), [id_](const Agent& agent) { return agent.getId() == id_; });
            if (it == agents.end()) {
                paxs::Logger logger("Save/error_log.txt");
                const std::string message = "Agent not found.";
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
            return *it;
        }

        /// @brief Get the agents.
        /// @brief エージェントを取得
        std::vector<Agent>& getAgents() const noexcept { return agents; }

        /// @brief Get the agents.
        /// @brief エージェントを取得
        std::vector<Agent> cgetAgents() const noexcept {
            std::vector<Agent> const_agents;
            for (const auto& agent : agents) {
                const_agents.emplace_back(agent);
            }
            return const_agents;
        }

        /// @brief Marriage.
        /// @brief 婚姻
        void marriage(std::vector<Settlement>& settlements) noexcept {
            // 結婚の条件を満たすエージェントを取得
            std::vector<std::size_t> marriageable_agents_index;
            for (std::size_t i = 0; i < agents.size(); ++i) {
                // 結婚可能かどうか
                if (agents[i].isAbleToMarriage() && agents[i].getGender() == female) {
                    if (!isMarried(agents[i].getAge())) continue;

                    marriageable_agents_index.emplace_back(i);
                }
            }

            // 結婚の条件を満たすエージェントがいない
            if (marriageable_agents_index.empty()) {
                return;
            }

            // 結婚相手を探す
            std::vector<std::size_t> close_settlements_index_list;
            for (std::size_t i = 0; i < settlements.size(); ++i) {
                if (settlements[i].getPosition().distance(positions[0]) < 10.0f) close_settlements_index_list.emplace_back(i);
            }

            // 自分の集落を含めて、近くに集落がない
            if (close_settlements_index_list.empty()) {
                Logger logger("Save/error_log.txt");
                const std::string message = "No close settlements.";
                logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                return;
            }

            // エージェントIDと集落IDのペアを作成
            std::vector<std::pair<std::uint_least64_t, std::uint_least32_t>> agent_settlement_pair;
            for (std::size_t close_settlement_index : close_settlements_index_list) {
                for (auto& agent : settlements[close_settlement_index].cgetAgents()) {
                    if (agent.isAbleToMarriage() && agent.getGender() == male) {
                        agent_settlement_pair.emplace_back(agent.getId(), settlements[close_settlement_index].getId());
                    }
                }
            }

            // 女性と男性の組み合わせをランダムに選択
            RandomSelector selector;

            // first: 女性のインデックス, second: 男性のインデックス
            const auto marriageable_agents_index_pair = selector.select(marriageable_agents_index.size(), agent_settlement_pair.size());

            // シミュレーションの設定で母方に移住するか父方に移住するかを決める
            // 母方の場合
            // if (isMatrilocality()) {
            if (true) {
                for (std::size_t i = 0; i < marriageable_agents_index_pair.size(); ++i) {
                    std::pair<std::size_t, std::size_t> pair = marriageable_agents_index_pair[i];
                    std::uint_least64_t male_id = marriageable_agents_index_pair[pair.second].first;
                    std::uint_least32_t settlement_id = static_cast<std::uint_least32_t>(marriageable_agents_index_pair[pair.second].second);

                    bool is_found = false;
                    for (std::size_t j = 0; j < settlements.size(); ++j) {
                        if (settlements[j].getId() == settlement_id) {
                            agents[marriageable_agents_index_pair[pair.first].first].marry(male_id);
                            const std::uint_least64_t female_id = agents[marriageable_agents_index_pair[pair.first].first].getId();

                            Agent male_ = settlements[j].getAgent(male_id);
                            male_.marry(female_id);
                            agents.emplace_back(male_);
                            settlements[j].deleteAgent(male_id);

                            is_found = true;
                            break;
                        }
                    }

                    if (!is_found) {
                        Logger logger("Save/error_log.txt");
                        const std::string message = "Settlement not found.";
                        logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                    } else {
                        std::cout << "marriage " << agents[marriageable_agents_index_pair[pair.first].first].getId() << std::endl;
                    }
                }
            }
            else {
                // 父方の場合
                for (std::size_t i = 0; i < marriageable_agents_index_pair.size(); ++i) {
                    std::pair<std::size_t, std::size_t> pair = marriageable_agents_index_pair[i];
                    std::uint_least64_t male_id = marriageable_agents_index_pair[pair.second].first;
                    std::uint_least32_t settlement_id = static_cast<std::uint_least32_t>(marriageable_agents_index_pair[pair.second].second);

                    bool is_found = false;
                    for (std::size_t j = 0; j < settlements.size(); ++j) {
                        if (settlements[j].getId() == settlement_id) {
                            agents[marriageable_agents_index_pair[pair.first].first].marry(male_id);
                            const std::uint_least64_t female_id = agents[marriageable_agents_index_pair[pair.first].first].getId();
                            settlements[j].getAgent(male_id).marry(female_id);

                            settlements[j].addAgent(agents[marriageable_agents_index_pair[pair.first].first]);
                            deleteAgent(marriageable_agents_index_pair[pair.first].first);

                            is_found = true;
                            break;
                        }
                    }

                    if (!is_found) {
                        Logger logger("Save/error_log.txt");
                        const std::string message = "Settlement not found.";
                        logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                    }
                }
            }
        }

        /// @brief Pre update.
        /// @brief 事前更新
        void preUpdate() noexcept {
            birth();
            emigration();
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
        std::tuple<std::uint_least32_t, Vector2, Vector2> move(std::mt19937& engine, int move_probability) {
            Vector2 current_key;
            Vector2 target_key;
            try {
                // 確率で移動
                std::uniform_int_distribution<> dist(0, move_probability_normalization_coefficient);
                if (dist(engine) > move_probability) return { 0, Vector2(), Vector2() };

                // 座標を移動
                // 移動距離0~max_move_distance
                std::uniform_int_distribution<> move_dist(min_move_distance, max_move_distance);

                Vector2 current_position = positions.front();
                Vector2 target_position = current_position;

                while (target_position == current_position || !environment->isLive(target_position)) {
                    float theta = std::uniform_real_distribution<float>(0.0f, 2.0f * M_PI)(engine);
                    int distance = move_dist(engine);
                    target_position = current_position + Vector2(static_cast<GridType>(std::cos(theta) * distance), static_cast<GridType>(std::sin(theta) * distance));
                }

                current_key = current_position / grid_length;
                target_key = target_position / grid_length;

                if (current_key == target_key) return { 0, Vector2(), Vector2() };

                is_moved = true;
                positions = { target_position };
            }
            catch (const std::exception& e) {
                Logger logger("Save/error_log.txt");
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, e.what());
                throw e;
            }
            return { id, current_key, target_key };
        }

        /// @brief Get the is_moved.
        /// @brief 移動したかどうかを取得
        bool isMoved() const noexcept { return is_moved; }

        /// @brief Get the population.
        /// @brief 人口を取得
        std::size_t getPopulation() const noexcept { return agents.size(); }
    private:
        std::shared_ptr<Environment> environment; // 環境
        /// @brief 集落id
        std::uint_least32_t id;
        /// @brief エージェントの配列
        std::vector<Agent> agents;
        /// @brief 集落の座標
        std::vector<Vector2> positions;
        /// @brief 既に移動したかどうか
        bool is_moved = false;

        std::mt19937 gen; // 乱数生成器
        std::uniform_int_distribution<> gender_dist{ 0, 1 }; // 性別の乱数分布
        std::uniform_real_distribution<> random_dist{ 0.0f, 1.0f }; // 乱数分布
        std::uniform_int_distribution<> life_exp_dist{ 50, 100 }; // 寿命の乱数分布

        /// @brief Birth.
        /// @brief 出産
        void birth() noexcept {
            std::vector<Agent> children;
            for (const auto& agent : agents) {
                // 出産可能かどうか
                if (!agent.isAbleToGiveBirth() || !isAbleToGiveBirth(agent.getAge())) continue;

                children.emplace_back(Agent(
                    UniqueIdentification<std::uint_least64_t>::generate(),
                    0, // TODO: 名前ID
                    static_cast<std::uint_least8_t>(gender_dist(gen)),
                    0,
                    static_cast<std::uint_least8_t>(life_exp_dist(gen)),
                    environment
                ));
            }
            agents.insert(agents.end(), children.begin(), children.end());
        }

        /// @brief Emigration.
        /// @brief 渡来
        void emigration() noexcept {
            // TODO: 渡来
        }

        /// @brief Age update.
        /// @brief 年齢更新
        void ageUpdate() noexcept {
            for (auto& agent : agents) {
                agent.incrementAge();
            }
        }

        /// @brief Death.
        /// @brief 死亡
        void death() noexcept {
            agents.erase(std::remove_if(agents.begin(), agents.end(), [](const Agent& agent) { return agent.isDead(); }), agents.end());
        }

        /// @brief Is the agent married?
        /// @brief 確率で結婚するかどうかを返す
        bool isMarried(float age) noexcept {
            const float sigma = 0.25f;
            auto x = [](float age) { return (age - 13) / 8.5f; };
            auto weight = [=](float age) {
                return std::exp(-std::pow(std::log(x(age)), 2) / (2 * std::pow(sigma, 2))) / (x(age) * sigma * std::sqrt(2 * M_PI));
                };

            float threshold = 0.98 * weight(age) / 101.8f;

            return random_dist(gen) < threshold;
        }

        /// @brief Is able to give birth?
        /// @brief 確率で出産するかどうかを返す
        bool isAbleToGiveBirth(float age) noexcept {
            const float sigma = 0.25f;
            auto x = [](float age) { return (age - 14) / 8.5f; };
            auto weight = [=](float age) {
                return std::exp(-std::pow(std::log(x(age)), 2) / (2 * std::pow(sigma, 2))) / (x(age) * sigma * std::sqrt(2 * M_PI));
                };

            float threshold = 16 * weight(age) / 101.8;

            return random_dist(gen) < threshold;
        }
    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_HPP
