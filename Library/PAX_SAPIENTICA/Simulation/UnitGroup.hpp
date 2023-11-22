/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_UNIT_HPP
#define PAX_SAPIENTICA_UNIT_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/BaseGroup.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

    /// @brief A class that represents a unit group.
    /// @brief 単位集団を表すクラス
    template <typename GridType>
    class UnitGroup : public BaseGroup<GridType>
    {
    public:
        using Agent = paxs::Agent<GridType>;
        using Vector2 = paxs::Vector2<GridType>;
        constexpr explicit UnitGroup(const std::uint_least32_t& id, const Vector2& position) noexcept : BaseGroup<GridType>(id), position(position) {}
        void addAgent(const Agent& agent) override {
            if (this->agents.size() + 1 < unit_group_max) {
                this->agents.emplace_back(agent);
            } else {
                throw std::runtime_error("The number of agents in the unit group has reached the limit.");
            }
        }

    private:
        /// @brief 単位集団の位置
        Vector2 position;
    };

}

#endif // !PAX_SAPIENTICA_UNIT_HPP
