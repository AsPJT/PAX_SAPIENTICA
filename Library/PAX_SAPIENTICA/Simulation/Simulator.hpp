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

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>

namespace paxs {
    template <typename T>
    class Simulator {
    public:
        using Environment = paxs::Environment<T>;
        using Vector2 = paxs::Vector2<T>;

        Simulator(const std::string& directory_path, const Vector2& start_position, const int z) :
            environment(directory_path, start_position, z) {}
        void init() {
            std::cout << "Initializing..." << std::endl;

            environment.randomizeAgents(100, Vector2(0, 0), Vector2(22784, 25600));
        }
        void run(const int step_count) {
            std::cout << "Running..." << std::endl;
            for(int i = 0; i < step_count; ++i) {
                step();
            }
            std::cout << "Finished." << std::endl;
        }
        void step() {
            std::cout << "Stepping..." << std::endl;

        }
    private:
        Environment environment;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_SIMULATOR_HPP