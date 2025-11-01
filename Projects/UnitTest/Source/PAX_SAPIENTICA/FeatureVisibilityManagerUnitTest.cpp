/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>

TEST (FeatureVisibilityManagerUnitTest, emplace) {
    paxs::FeatureVisibilityManager fvm;
    fvm.emplace(0, true);
    ASSERT_TRUE(fvm.isVisible(0));
}

TEST (FeatureVisibilityManagerUnitTest, toggle) {
    paxs::FeatureVisibilityManager fvm;
    fvm.emplace(0, true);
    fvm.toggle(0);
    ASSERT_FALSE(fvm.isVisible(0));
}

TEST (FeatureVisibilityManagerUnitTest, setVisibility) {
    paxs::FeatureVisibilityManager fvm;
    fvm.emplace(0, true);
    fvm.setVisibility(0, false);
    ASSERT_FALSE(fvm.isVisible(0));
}

TEST (FeatureVisibilityManagerUnitTest, operator) {
    paxs::FeatureVisibilityManager fvm;
    fvm.emplace(0, true);
    ASSERT_TRUE(fvm.isVisible(0));
}
