/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_BASIC_GROUP_HPP
#define PAX_SAPIENTICA_BASIC_GROUP_HPP

/*##########################################################################################

##########################################################################################*/

#include <vector>

#include <PAX_SAPIENTICA/Simulation/BaseGroup.hpp>

namespace paxs {

    /// @brief A class that represents a basic group.
    /// @brief 基礎集団を表すクラス
    template <typename GridType>
    class BasicGroup : public BaseGroup<GridType>
    {
    public:
        using Agent = paxs::Agent<GridType>;
        using Vector2 = paxs::Vector2<GridType>;

        constexpr explicit BasicGroup(const std::string& id, const std::vector<Vector2>& positions) noexcept
        : BaseGroup<GridType>(id), positions(positions) {}

        void addAgent(const Agent& agent) override {
            if (this->agents.size() + 1 < basic_group_max) {
                this->agents.emplace_back(agent);
            } else {
                throw std::runtime_error("The number of agents in the basic group has reached the limit.");
            }
        }
    private:
        /// @brief 基礎集団の位置
        std::vector<Vector2> positions;

    };

}

#endif // !PAX_SAPIENTICA_BASIC_GROUP_HPP
