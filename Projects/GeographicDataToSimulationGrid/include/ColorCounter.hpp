/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COLOR_COUNTER_HPP
#define PAX_SAPIENTICA_COLOR_COUNTER_HPP

/*##########################################################################################

##########################################################################################*/

#include <filesystem>
#include <map>

#include <Helper.hpp>

namespace paxs{
    class ColorCounter{
    public:
        bool init(){
            std::string setting_path = paxs::Helper::getSettingPath("ColorCount");
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
            return setColor();
        }
        void countColor(){
            cv::Mat img = cv::imread(input_path);
            if(img.empty()){
                std::cout << "image not found" << std::endl;
                return;
            }

            std::vector<int> count(tsv.size());
            for(int r=0;r<img.rows;r++){
                for(int c=0;c<img.cols;c++){
                    cv::Vec3b vec_color = img.at<cv::Vec3b>(r, c);
                    std::string color;
                    for(int i=2;i>=0;i--){
                        std::string s_rgb = std::to_string(vec_color[i]);
                        int size = s_rgb.size();
                        for(int j = 0;j < 3 - size;j++)
                            color += '0';
                        color += s_rgb;
                    }
                    int num = color_map[std::stoi(color)];
                    if(num == 0) continue;
                    count[num - 1]++;
                }
            }
            std::string content;
            for(int i = 0;i < tsv.size();i++){
                content += std::to_string(i + 1) + '\t' + tsv[i][1] + '\t' + std::to_string(count[i]) + '\n';
            }
            
            std::ofstream ofs;
            ofs.open(output_path, std::ios_base::trunc);
            ofs << content << std::endl;
            ofs.close();
        }
    private:
        std::string input_path;
        std::string output_path;
        std::vector<std::vector<std::string>> tsv;
        std::map<int, int> color_map;

        bool setColor(){
            if(tsv[0].size() == 5) {
                setRgbColor();
                return true;
            }
            else if(tsv[0].size() != 3) return false; // Hexかどうか
            for(const auto& row : tsv){
                std::string color = row[2];
                int size = color.size();
                if(size < 6)
                    for(int i = 0;i < 6 - size;i++)
                        color.insert(color.begin(), '0');
                paxs::Rgb rgb = paxs::Helper::hexToRgb("color");
                std::string s_rgb = std::to_string(rgb.red) + ',' + std::to_string(rgb.green) + ',' + std::to_string(rgb.blue);
                color_map[std::stoi(s_rgb)] = std::stoi(row[0]);
            }
            return true;
        }
        void setRgbColor(){
            int color_num[3];
            for(int i=2;i<=4;i++){
                if(tsv[0][i] == "赤" || tsv[0][i] == "red") color_num[0] = i;
                else if(tsv[0][i] == "緑" || tsv[0][i] == "green") color_num[1] = i;
                else if(tsv[0][i] == "青" || tsv[0][i] == "blue") color_num[2] = i;
                else return;
            }
            tsv.erase(tsv.begin());
            for(const auto& row : tsv){
                std::string color;
                for(int i=0;i<3;i++){
                    std::string tmp_color = row[color_num[i]];
                    int size = tmp_color.size();
                    for(int j = 0;j < 3 - size;j++)
                         color += '0';
                    color += tmp_color;
                }
                int n = std::stoi(row[0]);
                color_map[std::stoi(color)] = n;
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_COLOR_COUNTER_HPP