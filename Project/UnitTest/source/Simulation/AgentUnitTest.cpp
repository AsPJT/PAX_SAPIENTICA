/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>

template <typename T>
class EnvironmentMock : public paxs::Environment<T> {
public:
    using Vector2 = paxs::Vector2<T>;

    constexpr explicit EnvironmentMock() : paxs::Environment<T>() {}

    bool isLand(const Vector2&) const {
        return true;
    }
};

TEST (AgentUnitTest, move) {
    EnvironmentMock<int> env;
    paxs::Agent<int> agent("0", "test", paxs::Vector2<int>(0, 0), 0, 0, 0, &env);
    agent.move(paxs::Vector2<int>(1, 1));
    EXPECT_EQ(agent.getPosition(), paxs::Vector2<int>(1, 1));
}

TEST (AgentUnitTest, isDead) {
    EnvironmentMock<int> env;
    paxs::Agent<int> agent("0", "test", paxs::Vector2<int>(0, 0), 0, 10, 5, &env);
    EXPECT_TRUE(agent.isDead());
}

TEST (AgentUnitTest, getAge) {
    EnvironmentMock<int> env;
    paxs::Agent<int> agent("0", "test", paxs::Vector2<int>(0, 0), 0, 0, 0, &env);
    EXPECT_EQ(agent.getAge(), 0);
}

TEST (AgentUnitTest, updateAge) {
    EnvironmentMock<int> env;
    paxs::Agent<int> agent("0", "test", paxs::Vector2<int>(0, 0), 0, 0, 0, &env);
    agent.updateAge();
    EXPECT_EQ(agent.getAge(), 1);
}

TEST (AgentUnitTest, getGender) {
    EnvironmentMock<int> env;
    paxs::Agent<int> agent("0", "test", paxs::Vector2<int>(0, 0), 0, 0, 0, &env);
    EXPECT_EQ(agent.getGender(), 0);
}

TEST (AgentUnitTest, operatorEqual) {
    EnvironmentMock<int> env;
    paxs::Agent<int> agent1("0", "test", paxs::Vector2<int>(0, 0), 0, 0, 0, &env);
    paxs::Agent<int> agent2("0", "test", paxs::Vector2<int>(0, 0), 0, 0, 0, &env);
    EXPECT_TRUE(agent1 == agent2);

    paxs::Agent<int> agent3("1", "test", paxs::Vector2<int>(0, 0), 0, 0, 0, &env);
    EXPECT_FALSE(agent1 == agent3);
}
