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
#include <PAX_SAPIENTICA/Simulation/GeographicInformation.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {
    template <typename T>
    class Environment {
    public:
        using Vector2 = paxs::Vector2<T>;

        int pixel_size = 256;
        std::map<Vector2, GeographicInformation> geographic_informations;

        Environment() = default;
        Environment(const std::string& land_file_path, const Vector2& start_position, const Vector2& end_position, const int z) : start_position(start_position), end_position(end_position), z(z) {
            loadIsLand(land_file_path);
        }

        Vector2 getStartPosition() const { return start_position; }
        Vector2 getEndPosition() const { return end_position; }
        bool isLand(const Vector2& position) const {
            auto it = geographic_informations.find(position);
            if (it != geographic_informations.end()) {
                return it->second.isLand();
            }
            return false;
        }
    private:
        Vector2 start_position;
        Vector2 end_position;
        int z;

        void loadIsLand(const std::string& land_file_path) {
            std::cout << "Loading is land..." << std::endl;
            const std::vector<std::string> file_names = getFileNames(land_file_path);
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