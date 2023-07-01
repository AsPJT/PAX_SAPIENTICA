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

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

namespace paxs {

    /// @brief エージェントを表すクラス
    /// @tparam T 座標の型
    template <typename T>
    class Agent : public Object<T> {
    public:
        using Vector2 = paxs::Vector2<T>;
        using Environment = paxs::Environment<T>;

        Agent(const std::string& id, const std::string& name, const Vector2& pos, const std::uint_least8_t gen, const std::uint_least32_t age, const std::uint_least32_t life_span, Environment* env)
            : Object<T>(id, name, pos), gender(gen), age(age), life_span(life_span), environment(env) {}

        /// @brief エージェントを移動させる
        /// @param v 移動量
        void move(const Vector2& v) {
            if (environment->isLand(this->position + v)) {
                this->position += v;
            }
        }

        /// @brief エージェントが死んでいるかどうかを返す
        bool isDead() const { return age >= life_span; }
        /// @brief エージェントの年齢を更新する
        void updateAge() { ++age; }
        /// @brief エージェントの性別を取得する
        std::uint_least8_t getGender() const { return gender; }

        bool operator==(const paxs::Agent<T>& a) {
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