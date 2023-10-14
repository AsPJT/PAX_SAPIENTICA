/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp>

TEST (GraphicVisualizationListUnitTest, emplace) {
    paxs::GraphicVisualizationList gvl;
    gvl.emplace(0, true);
    ASSERT_TRUE(gvl[0]);
}
    
TEST (GraphicVisualizationListUnitTest, setNot) {
    paxs::GraphicVisualizationList gvl;
    gvl.emplace(0, true);
    gvl.setNot(0);
    ASSERT_FALSE(gvl[0]);
}

TEST (GraphicVisualizationListUnitTest, set) {
    paxs::GraphicVisualizationList gvl;
    gvl.emplace(0, true);
    gvl.set(0, false);
    ASSERT_FALSE(gvl[0]);
}

TEST (GraphicVisualizationListUnitTest, operator) {
    paxs::GraphicVisualizationList gvl;
    gvl.emplace(0, true);
    ASSERT_TRUE(gvl[0]);
}
