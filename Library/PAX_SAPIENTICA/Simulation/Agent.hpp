/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_AGENT_HPP
#define PAX_SAPIENTICA_SIMULATION_AGENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

namespace paxs {
    template <typename T>
    class Agent : public Object<T> {
    public:
        using Vector2 = paxs::Vector2<T>;
        using Environment = paxs::Environment<T>;

        Agent(const std::string& id, const std::string& name, const Vector2& pos, const bool gen, const int age, const int life_exp, Environment* env)
            : Object<T>(id, name, pos), gender(gen), age(age), life_expectancy(life_exp), environment(env) {}

        void move(const Vector2& v) {
            if (environment->isLand(this->position + v)) {
                this->position += v;
            }
        }

        bool isDead() const { return age >= life_expectancy; }
        void updateAge() { ++age; }
        std::uint_least8_t getGender() const { return gender; }

        bool operator==(const paxs::Agent<T>& a) {
            return  Object<T>::operator==(a) && 
                    gender == a.gender && 
                    age == a.age &&
                    life_expectancy == a.life_expectancy &&
                    environment == a.environment;
        }
    protected:
        std::uint_least8_t gender;
        int age;
        int life_expectancy;
        Environment* environment;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_AGENT_HPP