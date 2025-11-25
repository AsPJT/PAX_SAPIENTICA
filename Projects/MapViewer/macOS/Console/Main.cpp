/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <iostream>
#include <string>

#include <PAX_SAPIENTICA/Simulation/Manager/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Interface/ConsoleProgressReporter.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/LoggerIntegration.hpp>

int main() {
    std::cout << "=== PAX SAPIENTICA Console Simulator ===" << std::endl;
    std::cout << std::endl;

    // モデル名を入力
    std::cout << "Enter model name (e.g., Sample, EpiJomon, Yaponesia): ";
    std::string model_name;
    std::getline(std::cin, model_name);

    if (model_name.empty()) {
        model_name = "Sample";
        std::cout << "Using default model: " << model_name << std::endl;
    }

    // シミュレーション定数を初期化
    paxs::SimulationConstants::getInstance(model_name).init(model_name);

    // パスを生成
    std::string map_list_path = paxs::AppConfig::getInstance().getSettingPath(paxs::MurMur3::calcHash("SimulationXYZTiles"));
    std::string japan_provinces_path = paxs::AppConfig::getInstance().getSettingPath(paxs::MurMur3::calcHash("SimulationProvincesPath"));

    paxs::StringUtils::replace(map_list_path, "Sample", model_name);
    paxs::StringUtils::replace(japan_provinces_path, "Sample", model_name);

    std::cout << "Map list path: " << map_list_path << std::endl;
    std::cout << "Provinces path: " << japan_provinces_path << std::endl;
    std::cout << std::endl;

    // シミュレータを作成
    std::cout << "Creating simulator..." << std::endl;
    paxs::SettlementSimulator simulator;

    // コンソール用の進捗レポーターを作成
    paxs::ConsoleProgressReporter progress_reporter;
    simulator.setProgressReporter(&progress_reporter);

    // 環境データを読み込み
    std::cout << "Loading environment data..." << std::endl;
    simulator.setEnvironment(map_list_path, japan_provinces_path, 0);

    std::cout << "Environment loaded successfully." << std::endl;
    std::cout << std::endl;

    // 集落を初期化
    std::cout << "Initializing settlements..." << std::endl;
    simulator.init();

    std::cout << "Initialization complete." << std::endl;
    std::cout << "Population: " << simulator.cgetPopulationNum() << std::endl;
    std::cout << "Settlements: " << simulator.cgetSettlement() << std::endl;
    std::cout << std::endl;

    // ステップ数を入力
    std::cout << "Enter number of simulation steps (default: 100): ";
    std::string steps_str;
    std::getline(std::cin, steps_str);

    int steps = 100;
    if (!steps_str.empty()) {
        try {
            steps = std::stoi(steps_str);
        } catch (...) {
            std::cout << "Invalid input, using default: 100" << std::endl;
        }
    }

    // シミュレーションを実行
    std::cout << std::endl;
    std::cout << "Running simulation for " << steps << " steps..." << std::endl;
    simulator.run(steps);

    std::cout << std::endl;
    std::cout << "Simulation complete." << std::endl;
    std::cout << "Final population: " << simulator.cgetPopulationNum() << std::endl;
    std::cout << "Final settlements: " << simulator.cgetSettlement() << std::endl;

    return 0;
}
