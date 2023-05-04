/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>

template <typename T>
class AgentTest{
    using Agent = paxs::Agent<T>;
public:
    AgentTest() : gen(std::random_device{}()) {}
    void createAgents(std::vector<Agent>& agents, int num_agents) {
        std::uniform_real_distribution<> x_dist(-90.0, 90.0);
        std::uniform_real_distribution<> y_dist(-180.0, 180.0);
        std::uniform_int_distribution<> gender_dist(0, 1);
        std::uniform_int_distribution<> age_dist(0, 100);
        std::uniform_int_distribution<> life_exp_dist(50, 100);

        for (int i = 0; i < num_agents; ++i) {
            paxs::Vector2<int> position(x_dist(gen), y_dist(gen));
            agents.push_back(Agent(position, gender_dist(gen), age_dist(gen), life_exp_dist(gen)));
        }
    }
    void moveAgents(std::vector<Agent>& agents) {
        for (Agent& agent : agents) {
            agent.move(x_change(gen), y_change(gen));
        }
    }
    void printAgents(const std::vector<Agent>& agents) {
        for (const Agent& agent : agents) {
            std::cout << agent.getX() << ", " << agent.getY() << std::endl;
        }
    }
    void updateAgents(std::vector<Agent>& agents) {
        for (auto it = agents.begin(); it != agents.end();) {
            it->updateAge();
            if (it->isDead()) {
                it = agents.erase(it);
            } else {
                ++it;
            }
        }
    }
    void reproduceAgents(std::vector<Agent>& agents, double reproduction_distance, double reproduction_probability) {
        for (size_t i = 0; i < agents.size(); ++i) {
            for (size_t j = i + 1; j < agents.size(); ++j) {
                double distance = std::pow(agents[i].getX() - agents[j].getX(), 2) + std::pow(agents[i].getY() - agents[j].getY(), 2);
                if (distance < reproduction_distance && agents[i].getGender() != agents[j].getGender() && dist(gen) < reproduction_probability) {
                    bool new_gender = dist(gen) > 0.5;
                    paxs::Vector2<int> position((agents[i].getX() + agents[j].getX()) / 2, (agents[i].getY() + agents[j].getY()) / 2);
                    agents.push_back(Agent(position, new_gender, 0, life_exp_dist(gen)));
                }
            }
        }
    }
private:
    std::mt19937 gen;
    std::uniform_real_distribution<> x_change{-1.0, 1.0};
    std::uniform_real_distribution<> y_change{-1.0, 1.0};
    std::uniform_real_distribution<> dist{0.0, 1.0};
    std::uniform_int_distribution<> age_dist{0, 100};
    std::uniform_int_distribution<> life_exp_dist{50, 100};
};

int main()
{
    const double reproduction_distance = 2.0;
    const double reproduction_probability = 0.1;
    std::unique_ptr<AgentTest<int>> agent_test = std::make_unique<AgentTest<int>>();
    std::vector<paxs::Agent<int>> agents;
    agent_test->createAgents(agents, 10);
    agent_test->printAgents(agents);
    std::cout << std::endl;
    for(int i = 0; i < 10; ++i){
        agent_test->updateAgents(agents);
        agent_test->moveAgents(agents);
        agent_test->reproduceAgents(agents, reproduction_distance, reproduction_probability);
    }
    agent_test->printAgents(agents);
}