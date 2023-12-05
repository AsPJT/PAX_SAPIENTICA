/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/RandomSelector.hpp>

TEST(RandomSelectorUnitTest, select1) {
    paxs::RandomSelector selector;

    std::vector<int> vec = { 0, 1, 2, 3, 4, 5 };
    std::vector<int> result = selector.select(vec, 3);

    ASSERT_EQ(3, result.size());
}

TEST(RandomSelectorUnitTest, select2) {
    paxs::RandomSelector selector;

    std::vector<int> vec = { 0, 1, 2, 3, 4, 5 };
    std::vector<int> result = selector.select(vec, 3);
    ASSERT_EQ(3, result.size());
    for (int i : result) {
        ASSERT_TRUE(std::find(vec.begin(), vec.end(), i) != vec.end());
    }

    std::vector<std::pair<std::size_t, std::size_t>> result1 = selector.select(5, 3);
    ASSERT_EQ(3, result1.size());

    std::vector<std::pair<std::size_t, std::size_t>> result2 = selector.select(3, 5);
    ASSERT_EQ(3, result2.size());
    for (const auto& i : result2) {
        ASSERT_TRUE(i.first < 3);
        ASSERT_TRUE(i.second < 5);
    }
}

TEST(RandomSelectorUnitTest, outOfRange) {
    paxs::RandomSelector selector;

    std::vector<int> vec = { 0, 1, 2, 3, 4, 5 };
    std::vector<int> result = {};
    ASSERT_EQ(selector.select(vec, 7), result);
}
