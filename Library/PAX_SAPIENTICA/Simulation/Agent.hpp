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

#include <PAX_SAPIENTICA/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Simulation/ConvertToMercatorCoordinate.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>

namespace paxs {
    template <typename T>
    class Agent {
    public:
        using Environment = paxs::Environment<T>;
        using Vector2 = paxs::Vector2<T>;

        Agent(const Vector2& pos, const bool gen, const int age, const int life_exp, const Environment* env)
            : position(pos), gender(gen), age(age), life_expectancy(life_exp), environment(env) {}

        T getX() const { return position.x; }
        T getY() const { return position.y; }
        Vector2 getPosition() const { return position; }
        void setX(const T& x) { position.x = x; }
        void setY(const T& y) { position.y = y; }
        void setPosition(const Vector2& pos) { position = pos; }
        paxs::Vector2<double> getLocation(const int z, const int pixel_size) const {
            return convertToMercatorCoordinate({861, 350}, position, z, pixel_size);
        }
        void move(const Vector2& v) {
            if (environment->isLand(position + v)) {
                position += v;
            }
        }

        bool isDead() const { return age >= life_expectancy; }
        int getAge() const { return age; }
        void updateAge() { ++age; }
        bool getGender() const { return gender; }
        int getLifeExpectancy() const { return life_expectancy; }

        bool operator==(const paxs::Agent<T>& a) {
            return a.getX() == getX() && a.getY() == getY() && a.getGender() == getGender() && a.getAge() == getAge() && a.getLifeExpectancy() == getLifeExpectancy();
        }

    private:
        Vector2 position;
        bool gender; // true: male, false: female
        int age;
        int life_expectancy;
        const Environment* environment;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_AGENT_HPP