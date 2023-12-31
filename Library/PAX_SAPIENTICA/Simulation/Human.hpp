/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_HUMAN_HPP
#define PAX_SAPIENTICA_SIMULATION_HUMAN_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>

namespace paxs {

    /// @brief A class that represents a human.
    /// @brief 人間を表すクラス
    template <typename GridType>
    class Human : public Agent<GridType> {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Environment = paxs::Environment<GridType>;

        constexpr explicit Human(const std::string& id, const std::string& name, const Vector2& pos, const std::uint_least8_t gen, const std::uint_least8_t age, const std::uint_least8_t life_exp, Environment* env) noexcept
            : Agent<GridType>(id, name, pos, gen, age, life_exp, env) {}

        constexpr bool operator==(const paxs::Human<GridType>& a) const noexcept {
            return  Agent<GridType>::operator==(a);
        }
    protected:

    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_HUMAN_HPP
