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

#include <algorithm>
#include <cstdint>
#include <random>

#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

    template <typename GridType>
    class Settlement {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Object = paxs::Object<GridType>;
        using Agent = paxs::Agent<GridType>;

        constexpr explicit Settlement(const std::uint_least32_t id) noexcept : id(id) {}

        /// @brief Get the uuid.
        /// @brief idを取得
        constexpr std::uint_least32_t getId() const noexcept { return id; }

        /// @brief Add an agent to the settlement.
        /// @brief 集落にエージェントを追加
        void addAgent(const Agent& agent) noexcept { agents.push_back(agent); }

        /// @brief Set the position of the settlement.
        /// @brief 集落の座標を設定
        void setPosition(const Vector2& position) noexcept { positions.push_back(position); }

        /// @brief Get the position of the settlement.
        /// @brief 集落の座標を取得
        Vector2 getPosition() const noexcept { return positions.back(); }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        /// @param id The agent's id. エージェントのID
        Agent& getAgent(const std::uint_least32_t id) const {
            auto it = std::find_if(agents.begin(), agents.end(), [id](const Agent& agent) { return agent.getId() == id; });
            if (it == agents.end()) {
                paxs::Logger logger("Save/error_log.txt");
                const std::string message = "Agent not found.";
                logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
            return *it;
        }

        /// @brief Marriage.
        /// @brief 婚姻
        void marriage(std::vector<Settlement>& settlements) noexcept {
            // 結婚の条件を満たすエージェントを取得
            std::vector<std::size_t> marriageable_agents_index;
            for (std::size_t i = 0; i < agents.size(); ++i) {
                if (agents[i].getAge() >= marriageable_age_min && !agents[i].isMarried()) marriageable_agents_index.push_back(i);
            }

            // 結婚の条件を満たすエージェントがいない
            if (marriageable_agents_index.empty()) {
                return;
            }

            // 結婚相手を探す
            std::vector<std::size_t> close_settlements_index;
            for (std::size_t i = 0; i < settlements.size(); ++i) {
                if (settlements[i].getPosition().distance(positions[0]) < 10.0f) close_settlements_index.push_back(i);
            }

            // 自分の集落を含めて、近くに集落がない
            if (close_settlements_index.empty()) {
                Logger logger("Save/warning_log.txt");
                const std::string message = "No close settlements.";
                logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                return;
            }

            // TODO: 結婚対象者一人ずつについて、結婚相手を探す
            // 近くの集落をランダムに選択して、条件を満たすエージェントを探す
            for (std::size_t agent_index : marriageable_agents_index) {

            }


        }

        /// @brief Pre update.
        /// @brief 事前更新
        void preUpdate(std::mt19937& engine) noexcept {
            move(engine);
            birth();
            emigration();
        }

        /// @brief On update.
        /// @brief 更新
        void onUpdate() noexcept {
            ageUpdate();
            death();
        }

    private:
        /// @brief 集落id
        std::uint_least32_t id;
        /// @brief エージェントの配列
        std::vector<Agent> agents;
        /// @brief 集落の座標
        std::vector<Vector2> positions;

        /// @brief Move.
        /// @brief 移動
        void move(std::mt19937& engine) noexcept {
            // 0.1%の確率で移動
            std::uniform_int_distribution<> dist(0, 1000);
            if (dist(engine) != 0) return;

            // 座標を移動
            // TODO: 移動先の座標を決定

        }

        /// @brief Birth.
        /// @brief 出産
        void birth() noexcept {
            // TODO: 出産
        }

        /// @brief Emigration.
        /// @brief 渡来
        void emigration() noexcept {
            // TODO: 渡来
        }

        /// @brief Age update.
        /// @brief 年齢更新
        void ageUpdate() noexcept {
            for (Agent& agent : agents) {
                agent.incrementAge();
            }
        }

        /// @brief Death.
        /// @brief 死亡
        void death() noexcept {
            agents.erase(std::remove_if(agents.begin(), agents.end(), [](const Agent& agent) { return agent.isDead(); }), agents.end());
        }

    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_HPP
