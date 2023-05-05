/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP
#define PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <iostream>
#include <map>
#include <random>
#include <regex>

#include <PAX_SAPIENTICA/CuiOutput/Graphic.hpp>
#include <PAX_SAPIENTICA/FileRead/Read.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/GeographicInformation.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {
    template <typename T>
    class Environment {
    public:
        using Vector2 = paxs::Vector2<T>;
        using Agent = paxs::Agent<T>;

        std::map<Vector2, GeographicInformation> geographic_informations;

        Environment(const std::string& directory_path, const Vector2& start_position, const Vector2& end_position, const int z) : start_position(start_position), end_position(end_position), z(z) {
            loadIsLand(directory_path);
        }

        void randomizeAgents(const int agent_count) {
            const Vector2& offset = end_position - start_position;
            std::uniform_int_distribution<> x_dist(0, pixel_size * offset.x);
            std::uniform_int_distribution<> y_dist(0, pixel_size * offset.y);
            std::uniform_int_distribution<> age_dist(0, 20);
            std::uniform_int_distribution<> gender_dist{0, 1};
            std::uniform_int_distribution<> life_exp_dist{50, 100};
            std::cout << "Randomizing agents..." << std::endl;
            for(int i = 0;i < agent_count;++i) {
                displayProgressBar(i, agent_count);
                Vector2 position = Vector2(x_dist(gen), y_dist(gen));
                while(!geographic_informations[position].isLand()) {
                    position = Vector2(x_dist(gen), y_dist(gen));
                }
                agents.push_back(Agent(position, (bool)gender_dist(gen), age_dist(gen), life_exp_dist(gen)));
            }
            displayProgressBar(agent_count, agent_count);
            std::cout << std::endl;
        }
        void step() {
            for(auto& agent : agents) {
                agent.updateAge();
            }
            agents.erase(
                std::remove_if(
                    agents.begin(), agents.end(),[](const Agent& agent) { return agent.isDead(); }
                    ),
                 agents.end());
        }
        std::vector<Agent>& getAgents() {
            return agents;
        }
    private:
        const int pixel_size = 256;
        std::vector<Agent> agents;
        Vector2 start_position;
        Vector2 end_position;
        int z;
        std::mt19937 gen;

        void loadIsLand(const std::string& directory_path) {
            std::cout << "Loading is land..." << std::endl;
            const std::vector<std::string> file_names = getFileNames(directory_path);
            std::cout << file_names.size() << " files are found." << std::endl; 

            unsigned int file_count = 0;

            for(const auto& file_name : file_names) {
                displayProgressBar(file_count, file_names.size());

                Vector2 default_position = (getXAndYFromFileName(file_name) - start_position) * pixel_size;
                std::vector<std::string> file = readFile(file_name);
                for(std::size_t y = 0;y < file.size();++y) {
                    for(std::size_t x = 0;x < file[y].size();++x) {
                        Vector2 position = default_position + Vector2((int)x, (int)y);
                        setIsLand(file_name, position, file[y][x]);
                    }
                }
                ++file_count;
            }
            displayProgressBar(file_count, file_names.size());
            std::cout << std::endl << "Loading is land is completed." << std::endl;
        }
        void setIsLand(const std::string& file_name, const Vector2& position, const char value) {
            if(value == '1') {
                geographic_informations[position] = GeographicInformation(true);
            }
            else if(value == '0') {
                geographic_informations[position] = GeographicInformation();
            }
            else {
                std::cerr << "Error: " << file_name << " is not a binary file." << std::endl;
                std::exit(1);
            }
        }
        Vector2 getXAndYFromFileName(const std::string& file_name) {
            std::regex pattern(R"((\w+)_(\d+)_(\d+)_(\d+)\.txt)");
            std::smatch matches;

            if(std::regex_search(file_name, matches, pattern)) {
                return Vector2(std::stoi(matches[3]), std::stoi(matches[4]));
            }
            else {
                std::cerr << "Error: The format of " << file_name << " is incorrect." << std::endl;
                std::exit(1);
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_ENVIRONMENT_HPP