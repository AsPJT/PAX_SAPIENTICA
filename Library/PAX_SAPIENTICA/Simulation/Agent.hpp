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

namespace paxs {
    template <typename T>
    class Agent {
        using Vector2 = paxs::Vector2<T>;
    public:
        Agent(Vector2 pos, bool gen, int age, int life_exp)
            : position(pos), gender(gen), age(age), life_expectancy(life_exp) {}

        T getX() const { return position.x; }
        T getY() const { return position.y; }
        Vector2 getPosition() const { return position; }
        void setX(const T& x) { position.x = x; }
        void setY(const T& y) { position.y = y; }
        void setPosition(const Vector2& pos) { position = pos; }
        paxs::Vector2<double> getLocation(const int z, const int pixel_size) const {
            return convertToMercatorCoordinate({861, 350}, position, z, pixel_size);
        }
        void move(const T& x, const T& y) { position.x += x; position.y += y; }
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
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_AGENT_HPP