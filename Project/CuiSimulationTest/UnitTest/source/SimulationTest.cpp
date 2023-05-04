/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <memory>

#include "../include/AgentSimulator.hpp"

int main() {
	std::unique_ptr<paxs::AgentSimulator<int>> simulator = std::make_unique<paxs::AgentSimulator<int>>(100);
    simulator->simulate();
}