/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_CHROMOSOME_HPP
#define PAX_SAPIENTICA_SIMULATION_CHROMOSOME_HPP

/*##########################################################################################

##########################################################################################*/

#include <array>
#include <cstdint>
#include <random>

#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    /// @brief 染色体
    class Chromosome {
    public:
        static constexpr std::uint_least32_t chromosome_length = 46;
        Chromosome() = default;

        std::uint_least8_t get(const std::uint_least32_t index) const noexcept {
            if (index >= chromosome_length || index < 0) {
                PAXS_ERROR("Index out of range: " + std::to_string(index));
                return 0;
            }
            return chromosome[index];
        }

        void set(const std::uint_least32_t index, const std::uint_least8_t value) noexcept {
            if (index >= chromosome_length || index < 0) {
                PAXS_ERROR("Index out of range: " + std::to_string(index));
                return;
            }
            chromosome[index] = value;
        }

        friend std::ostream& operator<<(std::ostream& os, const Chromosome& value) noexcept {
            for (std::uint_least8_t i = 0; i < value.chromosome.size(); ++i) {
                os << static_cast<int>(value.chromosome[i]) << " ";
            }
            return os;
        }

        std::uint_least8_t getGender() const noexcept {
            return gender;
        }

        void setGender(const std::uint_least8_t value) noexcept {
            gender = value;
        }

        bool operator==(const Chromosome& rhs) const noexcept {
            for (std::uint_least8_t i = 0; i < chromosome_length; ++i) {
                if (chromosome[i] != rhs.chromosome[i]) {
                    return false;
                }
            }
            return true;
        }

        static Chromosome generateFromParents(const Chromosome& mother, const Chromosome& father) noexcept {
            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            std::uniform_int_distribution<std::uint_least64_t> dist(std::numeric_limits<std::uint_least64_t>::min(), std::numeric_limits<std::uint_least64_t>::max());
            std::uint_least64_t random_value = dist(engine);

            Chromosome child;
            for (std::uint_least8_t i = 0; i < chromosome_length; ++i) {
                // 偶数が母親、奇数が父親から遺伝
                if (i % 2 == 0) {
                    child.set(i, mother.get(i + (random_value % 2)));
                }
                else {
                    child.set(i, father.get(i - 1 + (random_value % 2)));
                }
                if (i == chromosome_length - 1) {
                    child.setGender(random_value % 2);
                }
                random_value >>= 1;
            }
            return child;
        }

        static Chromosome generateRandom() noexcept {
            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            std::uniform_int_distribution<std::uint_least8_t> dist(std::numeric_limits<std::uint_least8_t>::min(), std::numeric_limits<std::uint_least8_t>::max());

            Chromosome random_chromosome;
            for (std::uint_least8_t i = 0; i < chromosome_length; ++i) {
                random_chromosome.set(i, dist(engine));
            }
            random_chromosome.setGender(dist(engine) % 2);
            return random_chromosome;
        }

    private:
        std::array<std::uint_least8_t, chromosome_length> chromosome;
        std::uint_least8_t gender;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_CHROMOSOME_HPP
