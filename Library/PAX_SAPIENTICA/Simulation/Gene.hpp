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

    /// @brief 遺伝子
    class Gene {
    public:
        Gene() = default;

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

        static Gene generateRandom() noexcept {
            Gene gene;
            gene.setChromosome(Chromosome::generateRandom());

            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            std::uniform_int_distribution<std::uint_least8_t> dist(std::numeric_limits<std::uint_least8_t>::min(), std::numeric_limits<std::uint_least8_t>::max());
            gene.setMtDNA(dist(engine));
            gene.setYDNA(dist(engine));
            return gene;
        }

        static Gene generateFromParents(const Gene& mother, const Gene& father) noexcept {
            Gene gene;
            gene.setChromosome(Chromosome::generateFromParents(mother.cgetChromosome(), father.cgetChromosome()));

            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            gene.setMtDNA(mother.getMtDNA());
            if (gene.cgetChromosome().getGender() == SimulationConstants::getInstance()->female) {
                gene.setYDNA(mother.getYDNA());
            }
            else {
                gene.setYDNA(father.getYDNA());
            }
            return gene;
        }

        bool operator==(const Gene& rhs) const noexcept {
            return chromosome == rhs.chromosome && mtDNA == rhs.mtDNA && yDNA == rhs.yDNA;
        }

    private:
        Chromosome chromosome;
        std::uint_least8_t mtDNA;
        std::uint_least8_t yDNA;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_GENE_HPP
