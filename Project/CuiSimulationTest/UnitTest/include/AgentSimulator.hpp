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

namespace paxs{
    class AgentSimulator{
    public:
        AgentSimulator(const int agent_num) : agents(agent_num, paxs::Agent(0, 0, true, 0, 0)){
            init();
        }
        void simulate(){
            paxs::AgentGraphics graphics("Agent Simulator");
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
        std::vector<paxs::Agent> agents;
        std::mt19937 gen;
        std::uniform_real_distribution<> x_change{-1.0, 1.0};
        std::uniform_real_distribution<> y_change{-1.0, 1.0};
        std::uniform_real_distribution<> dist{0.0, 1.0};
        std::uniform_int_distribution<> gender_dist{0, 1};
        std::uniform_int_distribution<> life_exp_dist{50, 100};

        void init(){
            std::uniform_real_distribution<> x_dist(0, width);
            std::uniform_real_distribution<> y_dist(0, height);
            std::uniform_int_distribution<> age_dist(0, 20);

            for (int i = 0; i < agents.size(); ++i) {
                agents[i] = paxs::Agent(x_dist(gen), y_dist(gen), (bool)gender_dist(gen), age_dist(gen), life_exp_dist(gen));
            }
        }
        void reproduceAgents(std::vector<paxs::Agent>& agents, double reproduction_distance, double reproduction_probability) {
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