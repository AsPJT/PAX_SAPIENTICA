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

#include <memory>
#include <stdexcept>

#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/StatusDisplayer.hpp>

namespace paxs {

    /// @brief Class that performs simulation.
    /// @brief シミュレーションを行うクラス
    template <typename GridType>
    class Simulator {
    public:
        using Environment = paxs::Environment<GridType>;
        using Vector2 = paxs::Vector2<GridType>;
        using Agent = paxs::Agent<GridType>;

        constexpr explicit Simulator() = default;
        explicit Simulator(const std::string& setting_file_path, const Vector2& start_position, const Vector2& end_position, const int z, const unsigned seed = 0) :
            environment(std::make_unique<Environment>(setting_file_path, start_position, end_position, z)), gen(seed) {
                if (z <= 0) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Z must be greater than 0.";
                    logger.log(Logger::Level::ERRORa, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }
                if (start_position.x < 0 || start_position.y < 0 || end_position.x < 0 || end_position.y < 0) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Start position and end position must be greater than or equal to 0.";
                    logger.log(Logger::Level::ERRORa, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }
            }
        
        /// @brief Initialize the simulator.
        /// @brief エージェントの初期化
        /// @details エージェントをクリアし、指定された数だけランダムに配置する
        // TODO: とりあえず傾斜の緩やかな陸地にランダムに設置しているため、改修必須
        void init() {
            std::cout << "Initializing..." << std::endl;
            clearAgents();
            randomizeAgents(10000);
        }

        /// @brief Run the simulation for the specified number of steps.
        /// @brief シミュレーションを指定されたステップ数だけ実行する
        void run(const int step_count) noexcept {
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

        /// @brief Execute the simulation for the one step.
        /// @brief シミュレーションを1ステップ実行する
        void step() noexcept {
            for(auto& agent : agents) {
                agent.incrementAge();
                try {
                    agent.move();
                } catch (const std::runtime_error&) {
                    Logger logger("Save/error_log.txt");
                    logger.log(Logger::Level::ERRORa, __FILE__, __LINE__, "Failed to move agent. Agent id: " + std::to_string(agent.getId()));
                }
            }

            agents.erase(std::remove_if(agents.begin(), agents.end(), [](const Agent& agent) { return agent.isDead(); }), agents.end());

        }

        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr std::vector<Agent>& getAgents() noexcept { return agents; }
        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr const std::vector<Agent>& cgetAgents() const noexcept { return agents; }
    private:
        std::vector<Agent> agents; // エージェントのリスト
        std::shared_ptr<Environment> environment; // 環境
        std::mt19937 gen; // 乱数生成器
        std::uniform_int_distribution<> gender_dist{0, 1}; // 性別の乱数分布
        std::uniform_int_distribution<> life_exp_dist{50, 100}; // 寿命の乱数分布

        /// @brief Clear the agents.
        /// @brief エージェントをクリアする
        void clearAgents() noexcept { agents.clear(); }

        /// @brief Randomly place the agents.
        /// @brief エージェントをランダムに配置する
        void randomizeAgents(const int agent_count) {
            const Vector2& offset = environment->getEndPosition() - environment->getStartPosition();
            std::uniform_int_distribution<> x_dist(0, pixel_size * offset.x);
            std::uniform_int_distribution<> y_dist(0, pixel_size * offset.y);
            std::uniform_int_distribution<> age_dist(0, 20);
            std::cout << "Randomizing agents..." << std::endl;
            for(int i = 0;i < agent_count;++i) {
                StatusDisplayer::displayProgressBar(i, agent_count);
                Vector2 position = Vector2(x_dist(gen), y_dist(gen));
                try {
                    while(!environment->isLive(position)) {
                        position = Vector2(x_dist(gen), y_dist(gen));
                    }
                } catch (const std::runtime_error&) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Failed to randomize agent.";
                    logger.log(Logger::Level::ERRORa, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }
                
                // TODO: uuidの生成
                agents.push_back(Agent( 0, "", position, static_cast<std::uint_least8_t>(gender_dist(gen)), age_dist(gen), life_exp_dist(gen), environment));
            }
            StatusDisplayer::displayProgressBar(agent_count, agent_count);
            std::cout << std::endl;
        }

        /// @brief Print the current status.
        /// @brief 現在の状態を表示する
        constexpr void printStatus() const noexcept {
            std::cout << "Status: " << std::endl;
            std::cout << "  Agent Count: " << agents.size() << std::endl;
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_SIMULATOR_HPP
