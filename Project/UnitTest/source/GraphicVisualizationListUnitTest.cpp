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
    gvl.emplace("test", true);
    ASSERT_TRUE(gvl["test"]);
}
    
TEST (GraphicVisualizationListUnitTest, setNot) {
    paxs::GraphicVisualizationList gvl;
    gvl.emplace("test", true);
    gvl.setNot("test");
    ASSERT_FALSE(gvl["test"]);
}

TEST (GraphicVisualizationListUnitTest, set) {
    paxs::GraphicVisualizationList gvl;
    gvl.emplace("test", true);
    gvl.set("test", false);
    ASSERT_FALSE(gvl["test"]);
}

TEST (GraphicVisualizationListUnitTest, operator) {
    paxs::GraphicVisualizationList gvl;
    gvl.emplace("test", true);
    ASSERT_TRUE(gvl["test"]);
}
