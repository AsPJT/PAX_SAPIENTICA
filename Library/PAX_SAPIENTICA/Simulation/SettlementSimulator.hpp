/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
#define PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP

/*##########################################################################################

##########################################################################################*/

#include <memory>
#include <random>
#include <vector>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/JapanProvinces.hpp>
#include <PAX_SAPIENTICA/Simulation/Settlement.hpp>

namespace paxs {

    template <typename GridType>
    class SettlementSimulator {
    public:
        using Agent = paxs::Agent<GridType>;
        using Environment = paxs::Environment<GridType>;
        using Settlement = paxs::Settlement<GridType>;
        using Vector2 = paxs::Vector2<GridType>;

        constexpr explicit SettlementSimulator() = default;

        explicit SettlementSimulator(const std::string& map_list_path, const std::string& japan_provinces_path, const Vector2& start_position, const Vector2& end_position, const int z, const unsigned seed = 0) :
            environment(std::make_unique<Environment>(map_list_path, start_position, end_position, z)), gen(seed) {
                if (z <= 0) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Z must be greater than 0.";
                    logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }
                if (start_position.x < 0 || start_position.y < 0 || end_position.x < 0 || end_position.y < 0) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Start position and end position must be greater than or equal to 0.";
                    logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }

                try {
                    japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path + "/JapanRegion.tsv", japan_provinces_path + "/Ryoseikoku.tsv");
                } catch (const std::runtime_error&) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Failed to read Japan provinces TSV file: " + japan_provinces_path;
                    logger.log(Logger::Level::ERROR, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }

            }

        /// @brief Initialize the simulator.
        /// @brief 集落の初期化
        /// @details 集落をクリアし、地域ごとに指定されたエージェント数になるようにランダムに配置する
        void init() {
            settlements.clear();
            randomizeSettlements();
        }

        /// @brief Run the simulation for the specified number of steps.
        /// @brief シミュレーションを指定されたステップ数だけ実行する
        void run(const int step_count) noexcept {
            for(int i = 0; i < step_count; ++i) {
                step();
            }
        }

        /// @brief Execute the simulation for the one step.
        /// @brief シミュレーションを1ステップ実行する
        void step() noexcept {
            for (auto& settlement : settlements) {
                settlement.preUpdate();
            }

            for (auto& settlement : settlements) {
                settlement.onUpdate();
            }
        }

    private:
        std::vector<Settlement> settlements;
        std::shared_ptr<Environment> environment;
        std::mt19937 gen; // 乱数生成器
        std::uniform_int_distribution<> gender_dist{0, 1}; // 性別の乱数分布
        std::uniform_int_distribution<> life_exp_dist{50, 100}; // 寿命の乱数分布
        std::unique_ptr<paxs::JapanProvinces> japan_provinces;

        /// @brief Randomly place settlements.
        /// @brief 集落をランダムに配置する
        void randomizeSettlements() noexcept {
            // TODO: 未実装
        }

    };
}

#endif // !PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
