/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_AGENT_SIMULATOR_HPP
#define PAX_SAPIENTICA_AGENT_SIMULATOR_HPP

/*##########################################################################################

##########################################################################################*/

#include <random>
#include <vector>

#include <PAX_SAPIENTICA/SFML/AgentGraphics.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs{
    template <typename T>
    class AgentSimulator{
        using Agent = paxs::Agent<T>;
        using Vector2 = paxs::Vector2<T>;
    public:
        AgentSimulator(const int agent_num) {
            init(agent_num);
        }
        void simulate(){
            paxs::AgentGraphics<int> graphics("Agent Simulator");
            while (graphics.window.isOpen()) {
                sf::Event event;
                while (graphics.window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        graphics.window.close();
                }
                update();
                graphics.agentsDraw(agents);
            }
        }
        void update(){
            for (auto& agent : agents) {
                agent.updateAge();
                agent.move(x_change(gen), y_change(gen));
                
                if(agent.getX() < 0){
                    agent.setX(agent.getX() + width);
                }
                if(agent.getX() >= width){
                    agent.setX(agent.getX() - width);
                }
                if(agent.getY() < 0){
                    agent.setY(agent.getY() + height);
                }
                if(agent.getY() >= height){
                    agent.setY(agent.getY() - height);
                }
            }
        }
    private:
        std::vector<Agent> agents;
        std::mt19937 gen;
        std::uniform_int_distribution<> x_change{-1, 1};
        std::uniform_int_distribution<> y_change{-1, 1};
        std::uniform_real_distribution<> dist{0.0, 1.0};
        std::uniform_int_distribution<> gender_dist{0, 1};
        std::uniform_int_distribution<> life_exp_dist{50, 100};

        void init(const int agent_num){
            std::uniform_int_distribution<> x_dist(0, width);
            std::uniform_int_distribution<> y_dist(0, height);
            std::uniform_int_distribution<> age_dist(0, 20);

            for (int i = 0; i < agent_num; ++i) {
                agents.push_back(Agent(Vector2(x_dist(gen), y_dist(gen)), (bool)gender_dist(gen), age_dist(gen), life_exp_dist(gen)));
            }
        }
        void reproduceAgents(std::vector<Agent>& agents, double reproduction_distance, double reproduction_probability) {
            for (size_t i = 0; i < agents.size(); ++i) {
                for (size_t j = i + 1; j < agents.size(); ++j) {
                    double distance = std::pow(agents[i].getX() - agents[j].getX(), 2) + std::pow(agents[i].getY() - agents[j].getY(), 2);
                    if (distance < reproduction_distance && agents[i].getGender() != agents[j].getGender() && dist(gen) < reproduction_probability) {
                        bool new_gender = dist(gen) > 0.5;
                        agents.push_back(paxs::Agent((agents[i].getX() + agents[j].getX()) / 2, (agents[i].getY() + agents[j].getY()) / 2, new_gender, 0, life_exp_dist(gen)));
                    }
                }
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_AGENT_SIMULATOR_HPP