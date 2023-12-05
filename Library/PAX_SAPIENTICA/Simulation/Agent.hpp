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
#include <memory>
#include <random>
#include <stdexcept>

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

namespace paxs {

    /// @brief A class that represents an agent.
    /// @brief エージェントを表すクラス
    template <typename GridType>
    class Agent : public Object<GridType> {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Environment = paxs::Environment<GridType>;

        constexpr explicit Agent(const std::uint_least32_t id, const std::string& name, const Vector2& pos, const std::uint_least8_t gen,
            const std::uint_least32_t age, const std::uint_least32_t life_span, const std::shared_ptr<Environment> env) noexcept
            : Object<GridType>(id, name, pos), gender(gen), age(age), life_span(life_span), environment(env) {}

        /// @brief Move the agent.
        /// @brief エージェントを移動させる
        void move() {
            constexpr std::array<Vector2, 8> move_directions{
                    Vector2(-1, -1), Vector2(0, -1), Vector2(1, -1),
                    Vector2(-1, 0), Vector2(1, 0),
                    Vector2(-1, 1), Vector2(0, 1), Vector2(1, 1)
            };
            const float current_slope = environment->getSlope(this->position);

            const int th_max_slope = 30;
            const int th_min_slope = 5;

            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());

            // 0~min_slopeの間の傾斜の場合は10%の確率で移動
            if (current_slope <= th_min_slope) {
                std::uniform_int_distribution<> dist(0, 9);
                if (dist(engine) != 0) {
                    return;
                }
            }

            // min_slope~max_slopeの間の傾斜の場合は傾斜が大きいほど移動しやすくする
            if (current_slope > th_min_slope && current_slope <= th_max_slope) {
                std::uniform_int_distribution<> dist(0, static_cast<int>(9 + (current_slope - th_min_slope) * (90 / (th_max_slope - th_min_slope))));
                if (dist(engine) != 0) {
                    return;
                }
            }

            std::uniform_int_distribution<> amount(1, 5);
            std::array<float, 8> probabilities = {};
            const float elevation = environment->getElevation(this->position);
            for (bool is_movable = false; !is_movable;) {
                // 移動量は1~5の間でランダムに決定
                const int move_amount = amount(engine);

                for (std::size_t i = 0; i < 8; ++i) {
                    Vector2 pos = this->position + move_directions[i] * move_amount;
                    if (!environment->isLand(pos)) {
                        continue;
                    }

                    // slopeが小さい場所ほど選ばれやすいようにする
                    float slope = environment->getSlope(pos);
                    probabilities[i] += 1.0f / exp(slope / 4);

                    // 今の標高よりも低い方が選ばれやすいようにする
                    const float diff = elevation - environment->getElevation(pos);
                    if (diff > 0) {
                        probabilities[i] += diff / 10;
                    }

                    is_movable = true;
                }
            }

            std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
            this->move(move_directions[dist(engine)]);
        }

        /// @brief Move the agent.
        /// @brief エージェントを移動させる
        /// @param v A vector to move the agent. エージェントを移動させるベクトル
        void move(const Vector2& v) {
            if (environment->isLand(this->position + v)) {
                this->position += v;
            }
        }

        /// @brief Is the agent dead?
        /// @brief エージェントが死んでいるかどうかを返す
        constexpr bool isDead() const noexcept { return age >= life_span; }

        /// @brief Get the agent's age.
        /// @brief エージェントの年齢を取得する
        constexpr float getAge() const noexcept { return age / static_cast<float>(steps_per_year); }

        /// @brief Increment the agent's age.
        /// @brief エージェントの年齢をインクリメントする
        constexpr void incrementAge() noexcept {
            if (age != (std::numeric_limits<std::uint_least32_t>::max)()) {
                ++age;
            }
        }

        /// @brief Increment the agent's age.
        /// @brief エージェントの年齢をインクリメントする
        constexpr void incrementAge(const std::uint_least32_t n) noexcept { age += n; }

        /// @brief Get the agent's sex.
        /// @brief エージェントの性別を取得する
        constexpr std::uint_least8_t getGender() const noexcept { return gender; }

        constexpr bool operator==(const Agent& a) const noexcept {
            return  Object<GridType>::operator==(a) &&
                gender == a.gender &&
                age == a.age &&
                life_span == a.life_span &&
                environment == a.environment;
        }

        /// @brief Is the agent married?
        /// @brief エージェントが結婚しているかどうかを返す
        bool isMarried() const noexcept { return _isMarried; }
    protected:
        std::uint_least8_t gender; // 性別
        std::uint_least32_t age; // 年齢
        std::uint_least32_t life_span; // 寿命
        std::shared_ptr<Environment> environment; // 環境
        bool _isMarried = false; // 結婚しているかどうか
        std::uint_least32_t partnerId = 0; // 結婚相手のID
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_AGENT_HPP
