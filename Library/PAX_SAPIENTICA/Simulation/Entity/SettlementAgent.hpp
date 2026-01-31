/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_ENTITY_SETTLEMENT_AGENT_HPP
#define PAX_SAPIENTICA_SIMULATION_ENTITY_SETTLEMENT_AGENT_HPP

#include <limits>

#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/Genome.hpp>

namespace paxs {

    /// @brief A class that represents an agent.
    /// @brief エージェントを表すクラス
    class SettlementAgent {
    public:

        static constexpr std::uint_least8_t RiceTypeNone = 0; // 稲作なし
        static constexpr std::uint_least8_t RiceTypeIne = 1; // イネ栽培
        static constexpr std::uint_least8_t RiceTypePaddy = 2; // 水田稲作

        constexpr SettlementAgent() = default;

        explicit SettlementAgent(const HumanIndexType id,
            const AgeType age, const AgeType life_span, const Genome& genome,
            std::uint_least8_t farming_, // 農耕文化
            std::uint_least8_t hunter_gatherer_, // 狩猟採集文化
            std::uint_least8_t language_, // 言語
            std::uint_least8_t pottery_make_ = 0,
            std::uint_least8_t pottery_lineage_ = 0,
            std::uint_least8_t rice_type_ = 0
        ) noexcept
            : farming(farming_), hunter_gatherer(hunter_gatherer_), language(language_),
            pottery_make(pottery_make_), pottery_lineage(pottery_lineage_), rice_type(rice_type_),
            age(age), life_span(life_span),
            id(id), genome(genome) {
        }

        /// @brief Get the id.
        /// @brief idを取得
        constexpr HumanIndexType getId() const noexcept { return id; }
        constexpr AgeType getLifeSpan() const noexcept { return life_span; }

        /// @brief Is the agent dead?
        /// @brief エージェントが死んでいるかどうかを返す
        constexpr bool isDead() const noexcept { return age > life_span; }

        /// @brief Get the agent's age.
        /// @brief エージェントの年齢を取得する
        float getAge() const noexcept { return age / static_cast<float>(SimulationConstants::getInstance().steps_per_year); }
        std::size_t getAgeSizeT() const noexcept { return static_cast<std::size_t>(age) / static_cast<std::size_t>(SimulationConstants::getInstance().steps_per_year); }

        constexpr AgeType getAgeInt() const noexcept { return age; }

        /// @brief Get the agent's gender.
        /// @brief エージェントの性別を取得する
        constexpr bool isFemale() const noexcept { return genome.isFemale(); }
        constexpr bool isMale() const noexcept { return genome.isMale(); }

        /// @brief Increment the agent's age.
        /// @brief エージェントの年齢をインクリメントする
        constexpr void incrementAge() noexcept {
            if (age != (std::numeric_limits<AgeType>::max)()) {
                ++age;
            }
        }

        /// @brief Increment the agent's age.
        /// @brief エージェントの年齢をインクリメントする
        constexpr void incrementAge(const AgeType n) noexcept { age += n; }

        constexpr const Genome& cgetGenome() const noexcept { return genome; }

        constexpr void setPartnerGenome(const Genome& genome_) noexcept { partner_genome = genome_; }

        constexpr const Genome& cgetPartnerGenome() const noexcept { return partner_genome; }

        constexpr std::uint_least8_t cgetFarming() const noexcept { return farming; }

        constexpr std::uint_least8_t cgetHunterGatherer() const noexcept { return hunter_gatherer; }

        constexpr std::uint_least8_t cgetPartnerFarming() const noexcept { return partner_farming; }

        constexpr std::uint_least8_t cgetPartnerHunterGatherer() const noexcept { return partner_hunter_gatherer; }

        constexpr std::uint_least8_t cgetLanguage() const noexcept { return language; }

        constexpr std::uint_least8_t cgetPartnerLanguage() const noexcept { return partner_language; }

        constexpr std::uint_least8_t cgetPotteryMake() const noexcept { return pottery_make; }

        constexpr std::uint_least8_t cgetPotteryLineage() const noexcept { return pottery_lineage; }

        constexpr std::uint_least8_t cgetRiceType() const noexcept { return rice_type; }

        constexpr std::uint_least8_t cgetPartnerPotteryLineage() const noexcept { return partner_pottery_lineage; }

        constexpr std::uint_least8_t cgetPartnerRiceType() const noexcept { return partner_rice_type; }

        constexpr bool operator==(const SettlementAgent& a) const noexcept {
            return  id == a.id &&
                age == a.age &&
                life_span == a.life_span &&
                genome == a.genome &&
                partner_id == a.partner_id &&
                partner_genome == a.partner_genome &&
                partner_farming == a.partner_farming &&
                partner_hunter_gatherer == a.partner_hunter_gatherer &&
                partner_language == a.partner_language &&
                pottery_make == a.pottery_make &&
                pottery_lineage == a.pottery_lineage &&
                rice_type == a.rice_type &&
                partner_pottery_lineage == a.partner_pottery_lineage &&
                partner_rice_type == a.partner_rice_type;
        }

        /// @brief Is the agent married?
        /// @brief エージェントが結婚しているかどうかを返す
        bool isMarried() const noexcept { return partner_id != 0; }

        /// @brief Set the agent's marriage status.
        /// @brief 結婚する
        void marry(const HumanIndexType partner_id_, const Genome& partner_genome_,
            std::uint_least8_t partner_farming_, // 結婚相手の農耕文化
            std::uint_least8_t partner_hunter_gatherer_, // 結婚相手の狩猟採集文化
            std::uint_least8_t partner_language_, // 結婚相手の言語
            std::uint_least8_t partner_pottery_lineage_,
            std::uint_least8_t partner_rice_type_
        ) noexcept {
            partner_id = partner_id_;
            partner_genome = partner_genome_;
            partner_farming = partner_farming_;
            partner_hunter_gatherer = partner_hunter_gatherer_;
            partner_language = partner_language_;
            partner_pottery_lineage = partner_pottery_lineage_;
            partner_rice_type = partner_rice_type_;
        }

        /// @brief Is the agent able to marry?
        /// @brief エージェントが結婚可能かどうかを返す
        bool isAbleToMarriage() const noexcept {
            const float age_f = static_cast<float>(age) / SimulationConstants::getInstance().steps_per_year;
            return age_f > (genome.isMale() ?
                SimulationConstants::getInstance().male_marriageable_age_min :
                SimulationConstants::getInstance().female_marriageable_age_min) &&
                age_f < (genome.isMale() ?
                    SimulationConstants::getInstance().male_marriageable_age_max :
                    SimulationConstants::getInstance().female_marriageable_age_max) &&
                partner_id == 0;
        }

        /// @brief Is able to give birth?
        /// @brief 出産可能かどうか
        bool isAbleToGiveBirth() const noexcept {
            const float age_f = static_cast<float>(age) / SimulationConstants::getInstance().steps_per_year;
            return age_f > SimulationConstants::getInstance().childbearing_age_min
                && age_f < SimulationConstants::getInstance().childbearing_age_max && partner_id != 0;
        }

        /// @brief Get the partner's ID.
        /// @brief 結婚相手のIDを取得
        HumanIndexType getPartnerId() const noexcept { return partner_id; }

        /// @brief Divorce.
        /// @brief 離婚
        void divorce() noexcept {
            partner_id = 0;
        }

        std::uint_least8_t getBirthIntervalCount() const noexcept { return birth_interval_count; }
        void setBirthIntervalCount(const std::uint_least8_t count) noexcept { birth_interval_count = count; }
        std::uint_least8_t decrementBirthIntervalCount() noexcept { return --birth_interval_count; }

        void setAge(const AgeType age_) { age = age_; }
        void setLifeSpan(const AgeType life_span_) { life_span = life_span_; }
        void setPartnerId(const HumanIndexType partner_id_) { partner_id = partner_id_; }
        void setPotteryMake(const std::uint_least8_t value) { pottery_make = value; }
        void setPotteryLineage(const std::uint_least8_t value) { pottery_lineage = value; }
        void setRiceType(const std::uint_least8_t value) { rice_type = value; }

    protected:
        std::uint_least8_t birth_interval_count = 0; // 出産の間隔のカウント

        std::uint_least8_t farming = 0; // 農耕文化
        std::uint_least8_t hunter_gatherer = 0; // 狩猟採集文化
        std::uint_least8_t partner_farming = 0; // 結婚相手の農耕文化
        std::uint_least8_t partner_hunter_gatherer = 0; // 結婚相手の狩猟採集文化

        std::uint_least8_t language = 0; // 言語
        std::uint_least8_t partner_language = 0; // 結婚相手の言語

        std::uint_least8_t pottery_make = 0; // 土器を作るかどうか
        std::uint_least8_t pottery_lineage = 0; // 土器系統（ビットマスク）
        std::uint_least8_t rice_type = 0; // 稲作の種別

        std::uint_least8_t partner_pottery_lineage = 0; // 結婚相手の土器系統
        std::uint_least8_t partner_rice_type = 0; // 結婚相手の稲作種別

        AgeType age = 0; // 年齢
        AgeType life_span = 0; // 寿命

        HumanIndexType id = 0;
        HumanIndexType partner_id = 0; // 結婚相手のID（0は未婚）

        Genome genome{}; // ゲノム
        Genome partner_genome{}; // 結婚相手のゲノム
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_ENTITY_SETTLEMENT_AGENT_HPP
