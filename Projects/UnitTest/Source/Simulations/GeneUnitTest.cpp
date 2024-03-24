#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Gene.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

TEST (GeneUnitTest, getAndSet) {
    paxs::Gene gene;
    gene.setMtDNA(1);
    EXPECT_EQ(gene.getMtDNA(), 1);
    gene.setYDNA(1);
    EXPECT_EQ(gene.getYDNA(), 1);
    paxs::Chromosome chromosome = paxs::Chromosome::generateRandom();
    gene.setChromosome(chromosome);
    EXPECT_EQ(gene.cgetChromosome(), chromosome);
}

TEST (GeneUnitTest, generateRandom) {
    paxs::Gene gene = paxs::Gene::generateRandom();
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        EXPECT_GE(gene.cgetChromosome().get(i), 0);
        EXPECT_LE(gene.cgetChromosome().get(i), std::numeric_limits<std::uint_least8_t>::max());
    }
    EXPECT_GE(gene.getMtDNA(), 0);
    EXPECT_LE(gene.getMtDNA(), std::numeric_limits<std::uint_least8_t>::max());
    EXPECT_GE(gene.getYDNA(), 0);
    EXPECT_LE(gene.getYDNA(), std::numeric_limits<std::uint_least8_t>::max());
}

TEST (GeneUnitTest, generateFromParents) {
    paxs::Gene mother;
    paxs::Gene father;
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        mother.getChromosome().set(i, 1);
        father.getChromosome().set(i, 0);
    }
    paxs::Gene child = paxs::Gene::generateFromParents(mother, father);
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
