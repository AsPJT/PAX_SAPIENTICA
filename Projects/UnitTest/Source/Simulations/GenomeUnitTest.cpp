#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Genome.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

TEST (GenomeUnitTest, getAndSet) {
    paxs::Genome genome;
    genome.setMtDNA(1);
    EXPECT_EQ(genome.getMtDNA(), 1);
    genome.setYDNA(1);
    EXPECT_EQ(genome.getYDNA(), 1);
    paxs::Chromosome chromosome = paxs::Chromosome::generateRandom();
    genome.setChromosome(chromosome);
    EXPECT_EQ(genome.cgetChromosome(), chromosome);
}

TEST (GenomeUnitTest, generateRandom) {
    paxs::Genome genome = paxs::Genome::generateRandom();
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        EXPECT_GE(genome.cgetChromosome().get(i), 0);
        EXPECT_LE(genome.cgetChromosome().get(i), std::numeric_limits<std::uint_least8_t>::max());
    }
    EXPECT_GE(genome.getMtDNA(), 0);
    EXPECT_LE(genome.getMtDNA(), std::numeric_limits<std::uint_least8_t>::max());
    EXPECT_GE(genome.getYDNA(), 0);
    EXPECT_LE(genome.getYDNA(), std::numeric_limits<std::uint_least8_t>::max());
}

TEST (GenomeUnitTest, generateFromParents) {
    paxs::Genome mother;
    paxs::Genome father;
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        mother.getChromosome().set(i, 1);
        father.getChromosome().set(i, 0);
    }
    paxs::Genome child = paxs::Genome::generateFromParents(mother, father);
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        if (i % 2 == 0) {
            EXPECT_EQ(child.cgetChromosome().get(i), 1);
        }
        else {
            EXPECT_EQ(child.cgetChromosome().get(i), 0);
        }
    }
    EXPECT_EQ(child.getMtDNA(), mother.getMtDNA());
    if (child.cgetChromosome().getGender() == paxs::SimulationConstants::getInstance()->female) {
        EXPECT_EQ(child.getYDNA(), mother.getYDNA());
    }
    else {
        EXPECT_EQ(child.getYDNA(), father.getYDNA());
    }
}
