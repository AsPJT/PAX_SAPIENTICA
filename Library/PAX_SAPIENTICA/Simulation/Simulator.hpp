/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_SIMULATOR_HPP
#define PAX_SAPIENTICA_SIMULATION_SIMULATOR_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>

namespace paxs {
    template <typename T>
    class Simulator {
    public:
        using Environment = paxs::Environment<T>;
        using Vector2 = paxs::Vector2<T>;
        using Agent = paxs::Agent<T>;

        Simulator() = default;
        Simulator(const std::string& land_file_path, const Vector2& start_position, const Vector2& end_position, const int z) :
            environment(land_file_path, start_position, end_position, z) {}
        void init() {
            std::cout << "Initializing..." << std::endl;

            randomizeAgents(100);
        }
        void run(const int step_count) {
#ifdef PAX_SAPIENTICA_DEBUG
            printStatus();
#endif
            std::cout << "Running..." << std::endl;
            for(int i = 0; i < step_count; ++i) {
                step();
            }
            std::cout << "Finished." << std::endl;
#ifdef PAX_SAPIENTICA_DEBUG
            printStatus();
#endif
        }
        void step() {
            for(auto& agent : agents) {
                agent.updateAge();
                agent.move(Vector2(rand(gen), rand(gen)));
            }

            agents.erase(std::remove_if(agents.begin(), agents.end(),[](const Agent& agent) { return agent.isDead(); }),agents.end());

        }
        std::vector<Agent>& getAgents() {
            return agents;
        }
    private:
        std::vector<Agent> agents;
        Environment environment;
        std::mt19937 gen;
        std::uniform_int_distribution<> gender_dist{0, 1};
        std::uniform_int_distribution<> life_exp_dist{50, 100};
        std::uniform_int_distribution<> rand{-1, 1};

        void randomizeAgents(const int agent_count) {
            const Vector2& offset = environment.getEndPosition() - environment.getStartPosition();
            std::uniform_int_distribution<> x_dist(0, environment.pixel_size * offset.x);
            std::uniform_int_distribution<> y_dist(0, environment.pixel_size * offset.y);
            std::uniform_int_distribution<> age_dist(0, 20);
            std::cout << "Randomizing agents..." << std::endl;
            for(int i = 0;i < agent_count;++i) {
                displayProgressBar(i, agent_count);
                Vector2 position = Vector2(x_dist(gen), y_dist(gen));
                while(!environment.isLand(position)) {
                    position = Vector2(x_dist(gen), y_dist(gen));
                }
                agents.push_back(Agent(position, (bool)gender_dist(gen), age_dist(gen), life_exp_dist(gen), &environment));
            }
            displayProgressBar(agent_count, agent_count);
            std::cout << std::endl;
        }

#ifdef PAX_SAPIENTICA_DEBUG
        void printStatus() {
            std::cout << "Status: " << std::endl;
            std::cout << "  Agent Count: " << agents.size() << std::endl;
        }
#endif
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_SIMULATOR_HPP