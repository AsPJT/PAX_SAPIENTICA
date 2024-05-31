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

        constexpr std::uint_least8_t getGender() const noexcept {
            return chromosome.getGender();
        }

        static Genome generateRandom() noexcept {
            Genome genome;
            genome.setChromosome(Chromosome::generateRandom());

            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            std::uniform_int_distribution<> dist((std::numeric_limits<std::uint_least8_t>::min)(), (std::numeric_limits<std::uint_least8_t>::max)());
            genome.setMtDNA(static_cast<std::uint_least8_t>(dist(engine)));
            genome.setYDNA(static_cast<std::uint_least8_t>(dist(engine)));
            return genome;
        }

        static Genome generateFromParents(const Genome& mother, const Genome& father) noexcept {
            Genome genome;
            genome.setChromosome(Chromosome::generateFromParents(mother.cgetChromosome(), father.cgetChromosome()));

            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            genome.setMtDNA(mother.getMtDNA());
            if (genome.cgetChromosome().getGender() == SimulationConstants::getInstance()->female) {
                genome.setYDNA(mother.getYDNA());
            }
            else {
                genome.setYDNA(father.getYDNA());
            }
            return genome;
        }

        bool operator==(const Genome& rhs) const noexcept {
            return chromosome == rhs.chromosome && mtDNA == rhs.mtDNA && yDNA == rhs.yDNA;
        }

    private:
        Chromosome chromosome;
        std::uint_least8_t mtDNA;
        std::uint_least8_t yDNA;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_GENE_HPP
