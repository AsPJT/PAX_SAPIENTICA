#define USING_CHROMOSOME
#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Entity/Genome.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>

TEST (GenomeUnitTest, getAndSet) {
    std::random_device rd;
    std::mt19937 gen(rd());
    paxs::Genome genome;
    genome.setMtDNA(1);
    EXPECT_EQ(genome.getMtDNA(), 1);
    genome.setYDNA(1);
    EXPECT_EQ(genome.getYDNA(), 1);
    paxs::Chromosome chromosome = paxs::Chromosome::generateRandom(gen);
    genome.setChromosome(chromosome);
    EXPECT_EQ(genome.cgetChromosome(), chromosome);
}

TEST (GenomeUnitTest, generateRandom) {
    std::random_device rd;
    std::mt19937 gen(rd());
    paxs::Genome genome = paxs::Genome::generateRandom(gen);
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
    std::random_device rd;
    std::mt19937 gen(rd());
    paxs::Genome mother;
    paxs::Genome father;
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        mother.getChromosome().set(i, 1);
        father.getChromosome().set(i, 0);
    }
    paxs::Genome child = paxs::Genome::generateFromParents(gen, mother, father);
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        if (i % 2 == 0) {
            EXPECT_EQ(child.cgetChromosome().get(i), 1);
        }
        else {
            EXPECT_EQ(child.cgetChromosome().get(i), 0);
        }
    }
    EXPECT_EQ(child.getMtDNA(), mother.getMtDNA());
    if (child.isFemale()/*female_value*/) {
        EXPECT_EQ(child.getYDNA(), mother.getYDNA());
    }
    else {
        EXPECT_EQ(child.getYDNA(), father.getYDNA());
    }
}
