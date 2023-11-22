/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_SETTLEMENT_AGENT_HPP
#define PAX_SAPIENTICA_SIMULATION_SETTLEMENT_AGENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <array>
#include <limits>
#include <memory>
#include <random>
#include <stdexcept>

#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Object.hpp>

namespace paxs {

    /// @brief A class that represents an agent.
    /// @brief エージェントを表すクラス
    template <typename GridType>
    class SettlementAgent {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Environment = paxs::Environment<GridType>;

        constexpr explicit SettlementAgent(const std::uint_least64_t id, const std::uint_least32_t& name_id, const std::uint_least8_t gen,
            const std::uint_least32_t age, const std::uint_least32_t life_span, const std::shared_ptr<Environment> env) noexcept
            : id(id), name_id(name_id), gender(gen), age(age), life_span(life_span), environment(env) {}

        /// @brief Get the id.
        /// @brief idを取得
        constexpr std::uint_least64_t getId() const noexcept { return id; }

        /// @brief Is the agent dead?
        /// @brief エージェントが死んでいるかどうかを返す
        constexpr bool isDead() const noexcept { return age >= life_span; }

        /// @brief Get the agent's age.
        /// @brief エージェントの年齢を取得する
        constexpr float getAge() const noexcept { return age / static_cast<float>(steps_per_year); }

        /// @brief Get the agent's age.
        /// @brief エージェントの性別を取得する
        constexpr std::uint_least8_t getGender() const noexcept { return gender; }

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

        constexpr bool operator==(const SettlementAgent& a) const noexcept {
            return  id == a.id &&
                name_id == a.name_id &&
                gender == a.gender &&
                age == a.age &&
                life_span == a.life_span &&
                environment == a.environment;
        }

        /// @brief Is the agent married?
        /// @brief エージェントが結婚しているかどうかを返す
        bool isMarried() const noexcept { return is_married; }

        /// @brief Set the agent's marriage status.
        /// @brief 結婚する
        void marry(const std::uint_least64_t partner_id_) noexcept {
            is_married = true;
            partner_id = partner_id_;
        }

        /// @brief Is the agent able to marry?
        /// @brief エージェントが結婚可能かどうかを返す
        bool isAbleToMarriage() const noexcept {
            return age >= (gender ? male_marriageable_age_min : female_marriageable_age_min) &&
                age < marriageable_age_max && // TODO: 確認
                !is_married;
        }

        /// @brief Is able to give birth?
        /// @brief 出産可能かどうか
        bool isAbleToGiveBirth() const noexcept { return age >= birthable_age_min && age < birthable_age_max && is_married; }

    protected:
        std::uint_least64_t id; // ID
        std::uint_least32_t name_id; // 名前のID
        std::uint_least8_t gender; // 性別: 0 -> 女性, 1 -> 男性
        std::uint_least32_t age; // 年齢
        std::uint_least32_t life_span; // 寿命
        std::shared_ptr<Environment> environment; // 環境
        bool is_married = false; // 結婚しているかどうか
        std::uint_least32_t partner_id = 0; // 結婚相手のID
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_SETTLEMENT_AGENT_HPP
