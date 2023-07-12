/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_AGENT_HPP
#define PAX_SAPIENTICA_SIMULATION_AGENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <array>
#include <limits>
#include <random>
#include <stdexcept>

#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

namespace paxs {

    /// @brief A class that represents an agent.
    /// @brief エージェントを表すクラス
    /// @tparam T A type that represents a coordinate value. 座標値を表す型
    template <typename T>
    class Agent : public Object<T> {
    public:
        using Vector2 = paxs::Vector2<T>;
        using Environment = paxs::Environment<T>;

        constexpr explicit Agent(const std::string& id, const std::string& name, const Vector2& pos, const std::uint_least8_t gen, const std::uint_least32_t age, const std::uint_least32_t life_span, Environment* env) noexcept
            : Object<T>(id, name, pos), gender(gen), age(age), life_span(life_span), environment(env) {}

        /// @brief Move the agent.
        /// @brief エージェントを移動させる
        void move() {
            const std::array<Vector2, 8> move_directions {
                    Vector2(-1, -1), Vector2(0, -1), Vector2(1, -1),
                    Vector2(-1, 0), Vector2(1, 0),
                    Vector2(-1, 1), Vector2(0, 1), Vector2(1, 1)
                };
            const float current_slope = environment->getSlope(this->position);

            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            // 傾斜が10度未満の場合は20%の確率で斜面を登る
            if (current_slope < 10){
                std::uniform_int_distribution<> dist(0, 4);
                if (dist(engine) != 0) {
                    return;
                }
            }

            // slopeが小さい場所ほど選ばれやすいようにする
            std::array<float, 8> probabilities;
            for (std::size_t i = 0; i < 8; ++i) {
                float slope = environment->getSlope(this->position + move_directions[i]);
                probabilities[i] = 1.0f / exp(slope / 4);
            }

            std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
            this->move(move_directions[dist(engine)]);            
        }

        /// @brief Move the agent.
        /// @brief エージェントを移動させる
        /// @param v A vector to move the agent. エージェントを移動させるベクトル
        void move(const Vector2& v) {
            try {
                if (environment->isLand(this->position + v)) {
                    this->position += v;
                }
            } catch (const std::exception&) {
                Logger logger("Save/error_log.txt");
                logger.log(Logger::Level::ERROR, __FILE__, __LINE__, "Failed to move agent to " + std::to_string(this->position.x) + ", " + std::to_string(this->position.y) + ".");
                throw;
            }
        }

        /// @brief Is the agent dead?
        /// @brief エージェントが死んでいるかどうかを返す
        constexpr bool isDead() const noexcept { return age >= life_span; }
        
        /// @brief Get the agent's age.
        /// @brief エージェントの年齢を取得する
        constexpr std::uint_least32_t getAge() const noexcept { return age; }

        /// @brief Update the agent's age.
        /// @brief エージェントの年齢を更新する
        constexpr void updateAge() noexcept { 
            if (age != (std::numeric_limits<std::uint_least32_t>::max)()) {
                ++age;
            }
        }

        /// @brief Get the agent's sex.
        /// @brief エージェントの性別を取得する
        constexpr std::uint_least8_t getGender() const noexcept { return gender; }

        constexpr bool operator==(const paxs::Agent<T>& a) const noexcept {
            return  Object<T>::operator==(a) && 
                    gender == a.gender && 
                    age == a.age &&
                    life_span == a.life_span &&
                    environment == a.environment;
        }
    protected:
        std::uint_least8_t gender; // 性別
        std::uint_least32_t age; // 年齢
        std::uint_least32_t life_span; // 寿命
        Environment* environment; // 環境
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_AGENT_HPP