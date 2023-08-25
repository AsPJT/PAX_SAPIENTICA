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

#include <array>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

namespace paxs {

    /// @brief A class that represents a settlement.
    /// @brief 集落を表すクラス
    template <typename GridType>
    class Settlement : public Object<GridType> {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Object = paxs::Object<GridType>;
        using Agent = paxs::Agent<GridType>;

        constexpr explicit Settlement(const std::string& id, const std::string& name, const Vector2& pos) noexcept : Object(id, name, pos), agent_count(0) {}

        /// @brief Add an agent to the settlement.
        /// @brief 集落にエージェントを追加
        void addAgent(const Agent& agent) {
            if (agent_count >= agents.size()) {
                throw std::out_of_range("The number of agents has exceeded the limit.");
            }
            agents[agent_count] = agent;
            ++agent_count;
        }

        /// @brief Get the number of agents.
        /// @brief エージェントの数を取得
        constexpr std::uint_least32_t getAgentCount() const noexcept { return agent_count; }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        /// @param id The agent's id. エージェントのID
        constexpr Agent getAgent(const std::string& id) const {
            for (std::uint_least32_t i = 0; i < agent_count; ++i) {
                if (agents[i].getId() == id) {
                    return agents[i];
                }
            }
            throw std::out_of_range("The agent does not exist.");
        }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        constexpr Agent getAgent(const std::string& name, const std::uint_least32_t index) const {
            std::uint_least32_t count = 0;
            for (std::uint_least32_t i = 0; i < agent_count; ++i) {
                if (agents[i].getName() == name) {
                    if (count == index) {
                        return agents[i];
                    }
                    ++count;
                }
            }
            throw std::out_of_range("The agent does not exist.");
        }
    private:
        /// @brief エージェントの配列
        std::array<Agent, 100> agents;
        /// @brief エージェントの数
        std::uint_least32_t agent_count;
    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_HPP