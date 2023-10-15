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
#include <random>

#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

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

        /// @brief Pre update.
        /// @brief 事前更新
        void preUpdate(std::mt19937& engine, std::vector<Settlement>& closest_settlements) noexcept {
            move(engine);
            marriage();
            birth();
            emigration(closest_settlements);
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

        /// @brief Marriage.
        /// @brief 婚姻
        void marriage() noexcept {
            // TODO: 結婚
        }

        /// @brief Birth.
        /// @brief 出産
        void birth() noexcept {
            // TODO: 出産
        }

        /// @brief Emigration.
        /// @brief 渡来
        void emigration(std::vector<Settlement>& closest_settlements) noexcept {
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
