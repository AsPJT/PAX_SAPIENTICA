/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define CHANGE_DIR _chdir
#else
    #include <unistd.h>
    #define CHANGE_DIR chdir
#endif

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/ProvincesJapan.hpp>

TEST (ProvincesJapanUnitTest, constructor) {
    const std::string root = PROJECT_ROOT_PATH;
    const std::string japan_region_tsv_path = root + "/Project/UnitTest/data/Simulation/JapanRegion.tsv";
    const std::string ryoseikoku_tsv_path = root + "/Project/UnitTest/data/Simulation/Ryoseikoku.tsv";

    paxs::ProvincesJapan provinces_japan(japan_region_tsv_path, ryoseikoku_tsv_path);

    EXPECT_EQ(provinces_japan.getJapanRegionPopulation(0), 0);
    EXPECT_EQ(provinces_japan.getJapanRegionPopulation(2), 64600);

    EXPECT_EQ(provinces_japan.getRyoseikokuPopulation(0), 0);
    EXPECT_EQ(provinces_japan.getRyoseikokuPopulation(2), 40600);
}
