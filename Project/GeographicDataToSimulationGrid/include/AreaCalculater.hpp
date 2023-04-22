/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_AREA_CALCULATER_HPP
#define PAX_SAPIENTICA_AREA_CALCULATER_HPP

/*##########################################################################################

##########################################################################################*/

#include <Helper.hpp>

namespace paxs{
    class AreaCalculater{
    public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingPath("AreaCalculate");
            if(!std::filesystem::exists(setting_path)){
                std::ofstream writing_file;
                writing_file.open(setting_path, std::ios::out);
                std::string writing_text = "tsv_path = \ninput_path = \noutput_path = ";
                writing_file << writing_text << std::endl;
                writing_file.close();
                return false;
            }
			paxs::Settings settings = paxs::Helper::readSettings(setting_path);

            input_path = settings["input_path"];
            output_path = settings["output_path"];

            tsv = paxs::Helper::readTsv(settings["tsv_path"]);
            
            return true;
        }

        void calculateArea(){

        }
    private:
        std::string input_path;
        std::string output_path;
        std::vector<std::vector<std::string>> tsv;
    };
}

#endif // !PAX_SAPIENTICA_AREA_CALCULATER_HPP