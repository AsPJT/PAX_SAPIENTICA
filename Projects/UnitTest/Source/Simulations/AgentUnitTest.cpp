/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <memory>

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

using Vector2 = paxs::Vector2<paxs::GridType>;

class EnvironmentMock : public paxs::Environment {
public:

    explicit EnvironmentMock() : paxs::Environment() {}

    bool isLand(const Vector2&) const noexcept override {
        return true;
    }
};

TEST (AgentUnitTest, move) {
    std::shared_ptr<EnvironmentMock> env = std::make_shared<EnvironmentMock>();
    paxs::Agent agent(0, "test", paxs::Vector2(0, 0), 0, 0, 0, env);
    agent.move(paxs::Vector2(1, 1));
    EXPECT_EQ(agent.getPosition(), paxs::Vector2(1, 1));
}

TEST (AgentUnitTest, isDead) {
    std::shared_ptr<EnvironmentMock> env = std::make_shared<EnvironmentMock>();
    paxs::Agent agent(0, "test", paxs::Vector2(0, 0), 0, 10, 5, env);
    EXPECT_TRUE(agent.isDead());
}

TEST (AgentUnitTest, getAge) {
    std::shared_ptr<EnvironmentMock> env = std::make_shared<EnvironmentMock>();
    paxs::Agent agent(0, "test", paxs::Vector2(0, 0), 0, 0, 0, env);
    EXPECT_EQ(agent.getAge(), 0);
}

TEST (AgentUnitTest, updateAge) {
    std::shared_ptr<EnvironmentMock> env = std::make_shared<EnvironmentMock>();
    paxs::Agent agent(0, "test", paxs::Vector2(0, 0), 0, 0, 0, env);
    agent.incrementAge();
    EXPECT_EQ(agent.getAge(), 1 / static_cast<float>(paxs::SimulationConstants::getInstance()->steps_per_year));
}

TEST (AgentUnitTest, getGender) {
    std::shared_ptr<EnvironmentMock> env = std::make_shared<EnvironmentMock>();
    paxs::Agent agent(0, "test", paxs::Vector2(0, 0), 0, 0, 0, env);
    EXPECT_EQ(agent.getGender(), 0);
}

TEST (AgentUnitTest, operatorEqual) {
    std::shared_ptr<EnvironmentMock> env = std::make_shared<EnvironmentMock>();
    paxs::Agent agent1(0, "test", paxs::Vector2(0, 0), 0, 0, 0, env);
    paxs::Agent agent2(0, "test", paxs::Vector2(0, 0), 0, 0, 0, env);
    EXPECT_TRUE(agent1 == agent2);

    paxs::Agent agent3(1, "test", paxs::Vector2(0, 0), 0, 0, 0, env);
    EXPECT_FALSE(agent1 == agent3);
}
