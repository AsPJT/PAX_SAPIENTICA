/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/


#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>

#include <iostream>

int  main() {

    paxs::SlopeDegU0To250AllOutput sda(
        "Data/Map/XYZTile/Slope/Data/SlopeData20230514/2010/8/slope_8_0_63.tsv", "./../../../",
        "slope_test4.bin", "./", '\t'
    );
    sda.calc();

    //paxg::InputFile ifs("Data/Map/XYZTile/Slope/Data/SlopeData20230514/2010/8/slope_8_0_53.tsv", "./../../../");
    //paxg::InputFile ifs("Data/Map/XYZTile/Slope/Data/SlopeData20230514/2010/8/slope_8_179_109.tsv", "./../../../");
    //paxg::InputFile ifs("Data/Map/XYZTile/Slope/Data/SlopeData20230514/2010/8/slope_8_0_84.tsv", "./../../../");


    std::cout << "\n" << sda.sd.tmp_count;
    std::cout << "\n" << sda.sd.tmp_count2;

}
