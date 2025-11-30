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

#include <PAX_SAPIENTICA/Simulation/Config/JapanProvinces.hpp>


TEST (JapanProvincesUnitTest, constructor) {
    const std::string japan_region_tsv_path = "Data/Simulations/Sample";

    paxs::JapanProvinces japan_provinces(japan_region_tsv_path);

    // TODO: 詳細な内容の確認を追加する
}
