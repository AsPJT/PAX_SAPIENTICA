/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
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

#include <PAX_SAPIENTICA/Simulation/JapanProvinces.hpp>

// もしConfig.tsvが存在しない場合、一時的に作成する
class PathTSV {
public:
    static void generatePathTSV() {
        std::ofstream ofs("Config.tsv");
        ofs << "key\tvalue\nasset_file\t../" << std::endl;
        ofs.close();
    }

    static void removePathTSV() {
        const std::string path_tsv_path = "Config.tsv";

        std::remove(path_tsv_path.c_str());
    }
};

TEST (JapanProvincesUnitTest, constructor) {
    // Config.tsvが存在しない場合、一時的に作成する
    bool is_path_tsv_exist = std::filesystem::exists("Config.tsv");
    if (!is_path_tsv_exist) {
        PathTSV::generatePathTSV();
    }

    std::cout << paxs::AppConfig::getInstance()->getRootPath() << std::endl;

    const std::string japan_region_tsv_path = "Projects/UnitTest/Data/Simulations";

    paxs::JapanProvinces japan_provinces(japan_region_tsv_path);

    // EXPECT_EQ(japan_provinces.getJapanRegionPopulation(0), 0);
    // EXPECT_EQ(japan_provinces.getJapanRegionPopulation(2), 64600);

    EXPECT_EQ(japan_provinces.getDistrictPopulationAd200(0), 0);
    EXPECT_EQ(japan_provinces.getDistrictPopulationAd200(2), 2400);

    if (!is_path_tsv_exist) {
        PathTSV::removePathTSV();
    }
}
