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
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/StatusDisplayer.hpp>

namespace paxs {

    /// @brief シミュレーションを行うクラス
    /// @tparam T Vector2の座標の型
    template <typename T>
    class Simulator {
    public:
        using Environment = paxs::Environment<T>;
        using Vector2 = paxs::Vector2<T>;
        using Agent = paxs::Agent<T>;

        Simulator() = default;
        Simulator(const std::string& setting_file_path, const Vector2& start_position, const Vector2& end_position, const int z, const unsigned seed = 0) :
            environment(setting_file_path, start_position, end_position, z), gen(seed) {}
        
        /// @brief エージェントの初期化
        /// @details エージェントをクリアし、指定された数だけランダムに配置する
        // TODO: とりあえず陸地にランダムに設置しているため、改修必須
        void init() {
            std::cout << "Initializing..." << std::endl;
            clearAgents();
            randomizeAgents(100);
        }

        /// @brief シミュレーションを指定されたステップ数だけ実行する
        /// @param step_count ステップ数
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

        /// @brief シミュレーションを1ステップ実行する
        void step() {
            for(auto& agent : agents) {
                agent.updateAge();
                agent.move(Vector2(rand(gen), rand(gen)));
            }

            agents.erase(std::remove_if(agents.begin(), agents.end(),[](const Agent& agent) { return agent.isDead(); }),agents.end());

        }

        /// @brief エージェントのリストを取得する
        /// @return エージェントのリスト
        std::vector<Agent>& getAgents() {
            return agents;
        }
    private:
        std::vector<Agent> agents; // エージェントのリスト
        Environment environment; // 環境
        std::mt19937 gen; // 乱数生成器
        std::uniform_int_distribution<> gender_dist{0, 1}; // 性別の乱数分布
        std::uniform_int_distribution<> life_exp_dist{50, 100}; // 寿命の乱数分布
        std::uniform_int_distribution<> rand{-1, 1}; // 移動量の乱数分布

        // エージェントをクリアする
        void clearAgents() {
            agents.clear();
        }

        // エージェントをランダムに配置する
        void randomizeAgents(const int agent_count) {
            const Vector2& offset = environment.getEndPosition() - environment.getStartPosition();
            std::uniform_int_distribution<> x_dist(0, pixel_size * offset.x);
            std::uniform_int_distribution<> y_dist(0, pixel_size * offset.y);
            std::uniform_int_distribution<> age_dist(0, 20);
            std::cout << "Randomizing agents..." << std::endl;
            for(int i = 0;i < agent_count;++i) {
                StatusDisplayer::displayProgressBar(i, agent_count);
                Vector2 position = Vector2(x_dist(gen), y_dist(gen));
                while(!environment.isLand(position)) {
                    position = Vector2(x_dist(gen), y_dist(gen));
                }
                agents.push_back(Agent( "", "", position, static_cast<std::uint_least8_t>(gender_dist(gen)), age_dist(gen), life_exp_dist(gen), &environment));
            }
            StatusDisplayer::displayProgressBar(agent_count, agent_count);
            std::cout << std::endl;
        }

        // 現在の状態を表示する
        void printStatus() {
            std::cout << "Status: " << std::endl;
            std::cout << "  Agent Count: " << agents.size() << std::endl;
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_SIMULATOR_HPP