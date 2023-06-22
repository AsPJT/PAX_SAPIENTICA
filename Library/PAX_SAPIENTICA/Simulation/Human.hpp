/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_HUMAN_HPP
#define PAX_SAPIENTICA_SIMULATION_HUMAN_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/Human.hpp>

namespace paxs {
    template <typename T>
    class Human : public Agent<T> {
    public:
        using Vector2 = paxs::Vector2<T>;
        using Environment = paxs::Environment<T>;

        Human(const std::string& id, const std::string& name, const Vector2& pos, const std::uint_least8_t gen, const std::uint_least8_t age, const std::uint_least8_t life_exp, Environment* env)
            : Agent<T>(id, name, pos, gen, age, life_exp, env) {}

        bool operator==(const paxs::Human<T>& a) {
            return  Agent<T>::operator==(a);
        }
    protected:

    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_HUMAN_HPP