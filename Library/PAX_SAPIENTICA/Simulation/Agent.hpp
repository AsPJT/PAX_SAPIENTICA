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

namespace paxs {
    class Agent {
    public:
        Agent(double x, double y, bool gen, int age, int life_exp)
            : x(x), y(y), gender(gen), age(age), life_expectancy(life_exp) {}

        double getX() const { return x; }
        double getY() const { return y; }
        void setX(const double& x) { this->x = x; }
        void setY(const double& y) { this->y = y; }
        void move(const double& x, const double& y) { this->x += x; this->y += y; }
        bool isDead() const { return age >= life_expectancy; }
        int getAge() const { return age; }
        void updateAge() { ++age; }
        bool getGender() const { return gender; }
        int getLifeExpectancy() const { return life_expectancy; }

        bool operator==(const paxs::Agent& a) {
            return a.getX() == getX() && a.getY() == getY() && a.getGender() == getGender() && a.getAge() == getAge() && a.getLifeExpectancy() == getLifeExpectancy();
        }

    private:
        double x;
        double y;
        bool gender; // true: male, false: female
        int age;
        int life_expectancy;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_AGENT_HPP