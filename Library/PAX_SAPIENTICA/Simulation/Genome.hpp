/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_GENE_HPP
#define PAX_SAPIENTICA_SIMULATION_GENE_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/Chromosome.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

    /// @brief ゲノム
    class Genome {
    public:
        Genome() = default;

        const Chromosome& cgetChromosome() const noexcept {
            return chromosome;
        }

        Chromosome& getChromosome() noexcept {
            return chromosome;
        }

        void setChromosome(const Chromosome& value) noexcept {
            chromosome = value;
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

        constexpr std::uint_least8_t getGender() const noexcept {
            return chromosome.getGender();
        }

        static Genome generateRandom(std::mt19937& engine) noexcept {
            Genome genome;
            genome.setChromosome(Chromosome::generateRandom(engine));

            std::uniform_int_distribution<> dist((std::numeric_limits<std::uint_least8_t>::min)(), (std::numeric_limits<std::uint_least8_t>::max)());
            genome.setMtDNA(static_cast<std::uint_least8_t>(dist(engine)));
            genome.setYDNA(static_cast<std::uint_least8_t>(dist(engine)));
            genome.setSNP(static_cast<std::uint_least8_t>(dist(engine)));
            return genome;
        }
        static Genome generateRandomSetMtDNA(std::mt19937& engine, const std::uint_least8_t mtdna_, const std::uint_least8_t snp_) noexcept {
            Genome genome;
            genome.setChromosome(Chromosome::generateRandom(engine));

            std::uniform_int_distribution<> dist((std::numeric_limits<std::uint_least8_t>::min)(), (std::numeric_limits<std::uint_least8_t>::max)());
            genome.setSNP(snp_);
            genome.setMtDNA(mtdna_);
            genome.setYDNA(static_cast<std::uint_least8_t>(dist(engine)));
            return genome;
        }

        static Genome generateFromParents(std::mt19937& engine, const Genome& mother, const Genome& father) noexcept {
            Genome genome;
            genome.setChromosome(Chromosome::generateFromParents(engine, mother.cgetChromosome(), father.cgetChromosome()));

            genome.setMtDNA(mother.getMtDNA());
            if (genome.cgetChromosome().getGender() == female_value) {
                genome.setYDNA(mother.getYDNA());
            }
            else {
                genome.setYDNA(father.getYDNA());
            }
            genome.setSNP(static_cast<std::uint_least8_t>((int(mother.getSNP()) + int(father.getSNP())) / 2));
            return genome;
        }

        bool operator==(const Genome& rhs) const noexcept {
            return chromosome == rhs.chromosome && mtDNA == rhs.mtDNA && yDNA == rhs.yDNA;
        }

    private:
        Chromosome chromosome{};
        std::uint_least8_t SNP = 0;
        std::uint_least8_t mtDNA = 0;
        std::uint_least8_t yDNA = 0;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_GENE_HPP
