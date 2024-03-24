#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Chromosome.hpp>

TEST (ChromosomeUnitTest, getAndSet) {
    paxs::Chromosome chromosome;
    chromosome.set(0, 1);
    EXPECT_EQ(chromosome.get(0), 1);
}

TEST (ChromosomeUnitTest, generateRandom) {
    paxs::Chromosome chromosome = paxs::Chromosome::generateRandom();
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        EXPECT_GE(chromosome.get(i), 0);
        EXPECT_LE(chromosome.get(i), std::numeric_limits<std::uint_least8_t>::max());
    }
}

TEST (ChromosomeUnitTest, generateFromParents) {
    paxs::Chromosome mother;
    paxs::Chromosome father;
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        mother.set(i, 1);
        father.set(i, 0);
    }
    paxs::Chromosome child = paxs::Chromosome::generateFromParents(mother, father);
    for (std::uint_least8_t i = 0; i < paxs::Chromosome::chromosome_length; ++i) {
        if (i % 2 == 0) {
            EXPECT_EQ(child.get(i), 1);
        }
        else {
            EXPECT_EQ(child.get(i), 0);
        }
    }
}
