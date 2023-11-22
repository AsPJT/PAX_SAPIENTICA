/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_BASE_GROUP_HPP
#define PAX_SAPIENTICA_BASE_GROUP_HPP

/*##########################################################################################

##########################################################################################*/

#include <vector>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

namespace paxs {

    template <typename GridType>
    class BaseGroup {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Object = paxs::Object<GridType>;
        using Agent = paxs::Agent<GridType>;

        constexpr explicit BaseGroup(const std::uint_least32_t id) noexcept : id(id) {}

        /// @brief Get the uuid.
        /// @brief idを取得
        constexpr std::uint_least32_t getId() const noexcept { return id; }

        /// @brief Add an agent to the settlement.
        /// @brief 集落にエージェントを追加
        virtual void addAgent(const Agent& agent) { agents.emplace_back(agent); }

        /// @brief Get the agent.
        /// @brief エージェントを取得
        /// @param id The agent's id. エージェントのID
        constexpr Agent getAgent(const std::string& id) const {
            for (const auto& agent : agents) {
                if (agent.getID() == id) {
                    return agent;
                }
            }
            throw std::runtime_error("Agent not found.");
        }

    protected:
        /// @brief エージェントの配列
        std::vector<Agent> agents;

    private:
        /// @brief 集落id
        std::uint_least32_t id;
    };

}

#endif // !PAX_SAPIENTICA_BASE_GROUP_HPP
