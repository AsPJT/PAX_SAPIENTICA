/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_GENE_HPP
#define PAX_SAPIENTICA_SIMULATION_GENE_HPP

#include <PAX_SAPIENTICA/Simulation/Chromosome.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

    /// @brief ゲノム
    class Genome {
#ifndef USING_CHROMOSOME
        using Chromosome = std::uint_least8_t;
#endif // USING_CHROMOSOME
    public:
        Genome() = default;

        const Chromosome& cgetChromosome() const noexcept {
#ifdef USING_CHROMOSOME
            return chromosome;
#else
            return yDNA;
#endif // USING_CHROMOSOME
        }

        Chromosome& getChromosome() noexcept {
#ifdef USING_CHROMOSOME
            return chromosome;
#else
            return yDNA;
#endif // USING_CHROMOSOME
        }

        void setChromosome(const Chromosome& value) noexcept {
#ifdef USING_CHROMOSOME
            chromosome = value;
#else
            yDNA = value;
#endif // USING_CHROMOSOME
        }

        std::uint_least8_t getMtDNA() const noexcept {
            return mtDNA;
        }

        void setMtDNA(const std::uint_least8_t value) noexcept {
            mtDNA = value;
        }

        std::uint_least8_t getYDNA() const noexcept {
            return yDNA;
        }

        void setYDNA(const std::uint_least8_t value) noexcept {
            yDNA = value;
        }

        std::uint_least8_t getSNP() const noexcept {
            return SNP;
        }

        void setSNP(const std::uint_least8_t value) noexcept {
            SNP = value;
        }

        std::uint_least8_t getLanguage() const noexcept {
            return language;
        }

        void setLanguage(const std::uint_least8_t value) noexcept {
            language = value;
        }

        constexpr bool isFemale() const noexcept {
            return (yDNA == 0);
        }

        constexpr bool isMale() const noexcept {
            return (yDNA != 0);
        }

        static Genome generateRandom(std::mt19937& engine) noexcept {
            Genome genome;
#ifdef USING_CHROMOSOME
            genome.setChromosome(Chromosome::generateRandom(engine));
#endif // USING_CHROMOSOME
            const bool is_female = ((engine() % 2) == 0);
            std::uniform_int_distribution<> dist(1, (std::numeric_limits<std::uint_least8_t>::max)());
            genome.setMtDNA(static_cast<std::uint_least8_t>(dist(engine)));
            genome.setYDNA((is_female) ? 0 : static_cast<std::uint_least8_t>(dist(engine)));
            genome.setSNP(static_cast<std::uint_least8_t>(dist(engine)));
            genome.setLanguage(static_cast<std::uint_least8_t>(dist(engine)));
            return genome;
        }
        static Genome generateRandomSetMtDNA(std::mt19937& engine, const std::uint_least8_t mtdna_, const std::uint_least8_t snp_, const std::uint_least8_t language_) noexcept {
            Genome genome;
#ifdef USING_CHROMOSOME
            genome.setChromosome(Chromosome::generateRandom(engine));
#endif // USING_CHROMOSOME
            const bool is_female = ((engine() % 2) == 0);
            genome.setSNP(snp_);
            genome.setMtDNA(mtdna_);
            genome.setYDNA((is_female) ? 0 : 1/*static_cast<std::uint_least8_t>(dist(engine))*/);
            genome.setLanguage(language_);
            return genome;
        }

        static Genome generateFromParents(std::mt19937& engine, const Genome& mother, const Genome& father) noexcept {
            Genome genome;
#ifdef USING_CHROMOSOME
            genome.setChromosome(Chromosome::generateFromParents(engine, mother.cgetChromosome(), father.cgetChromosome()));
#endif // USING_CHROMOSOME
            const auto& engine_value = engine();
            const bool is_female = ((engine_value % 2) == 0);

            genome.setMtDNA(mother.getMtDNA());
            if (is_female) {
                genome.setYDNA(mother.getYDNA());
            }
            else {
                genome.setYDNA(father.getYDNA());
            }
            genome.setLanguage((((engine_value >> 1) % 2) == 0) ? mother.language : father.language);
            genome.setSNP(static_cast<std::uint_least8_t>((int(mother.getSNP()) + int(father.getSNP())) / 2));
            return genome;
        }

        bool operator==(const Genome& rhs) const noexcept {
            return
#ifdef USING_CHROMOSOME
                chromosome == rhs.chromosome &&
#endif // USING_CHROMOSOME
                mtDNA == rhs.mtDNA &&
                yDNA == rhs.yDNA &&
                language == rhs.language;
        }

    private:
#ifdef USING_CHROMOSOME
        Chromosome chromosome{};
#endif // USING_CHROMOSOME

        std::uint_least8_t SNP = 0;
        std::uint_least8_t mtDNA = 0;
        std::uint_least8_t yDNA = 0;
        std::uint_least8_t language = 0;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_GENE_HPP
