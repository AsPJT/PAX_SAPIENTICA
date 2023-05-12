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
                std::string writing_text = "area_tsv_path = \ncolor_tsv_path = \ninput_path = \noutput_path = \nstart_y = \ndeviation_z = ";
                writing_file << writing_text << std::endl;
                writing_file.close();
                return false;
            }
			paxs::Settings settings = paxs::Helper::readSettings(setting_path);

            input_path = settings["input_path"];
            output_path = settings["output_path"];
            start_y = std::stoi(settings["start_y"]);
            deviation_z = std::stoi(settings["deviation_z"]);

            std::vector<std::vector<std::string>> area_tsv = paxs::Helper::readTsv(settings["area_tsv_path"]);
            color_tsv = paxs::Helper::readTsv(settings["color_tsv_path"]);
            setAreaMap(area_tsv);
            return setColor();
        }

        void calculateArea(){
            cv::Mat img = cv::imread(input_path);
            if(img.empty()){
                std::cout << "image not found" << std::endl;
                return;
            }

            std::vector<double> area_count(color_tsv.size());
            double y_pow = Helper::powOfTwo(deviation_z);
            double r_pow = Helper::powOfTwo(deviation_z - 8);
            double area_pow = r_pow * r_pow;
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
                    // 面積を取得
                    int y = y_pow * start_y + r_pow * r;
                    double area = area_map[y];
                    if(area == 0) std::cout << y << " not found" << std::endl;
                    area_count[num - 1] += area;
                }
            }
            std::string content;
            for(int i = 0;i < color_tsv.size();i++){
                content += std::to_string(i + 1) + '\t' + color_tsv[i][1] + '\t' + std::to_string(area_count[i] * area_pow) + '\n';
            }
            
            std::ofstream ofs;
            ofs.open(output_path, std::ios_base::trunc);
            ofs << content << std::endl;
            ofs.close();
        }
    private:
        std::string input_path;
        std::string output_path;
        int start_y;
        int deviation_z;
        std::vector<std::vector<std::string>> color_tsv;
        std::map<int, int> color_map;
        std::map<int, double> area_map;

        bool setColor(){
            bool is_rgb;
            if(color_tsv[0].size() == 5) {
                setRgbColor();
                return true;
            }
            else if(color_tsv[0].size() != 3) return false; // Hexかどうか
            for(const auto& row : color_tsv){
                std::string color = row[2];
                int size = color.size();
                if(size < 6)
                    for(int i = 0;i < 6 - size;i++)
                        color.insert(color.begin(), '0');
                paxs::Rgb rgb = paxs::Helper::hexToRgb(color);
                std::string s_rgb = std::to_string(rgb.red) + ',' + std::to_string(rgb.green) + ',' + std::to_string(rgb.blue);
                color_map[std::stoi(s_rgb)] = std::stoi(row[0]);
            }
            return true;
        }
        void setRgbColor(){
            int color_num[3];
            for(int i=2;i<=4;i++){
                if(color_tsv[0][i] == "赤" || color_tsv[0][i] == "red") color_num[0] = i;
                else if(color_tsv[0][i] == "緑" || color_tsv[0][i] == "green") color_num[1] = i;
                else if(color_tsv[0][i] == "青" || color_tsv[0][i] == "blue") color_num[2] = i;
                else return;
            }
            color_tsv.erase(color_tsv.begin());
            for(const auto& row : color_tsv){
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
        void setAreaMap(const std::vector<std::vector<std::string>>& area_tsv){
            if(area_tsv[0].size() == 5){
                for(int i = 1;i < area_tsv.size();i++) area_map[std::stoi(area_tsv[i][1])] = std::stod(area_tsv[i][4]);
                return;
            }
            for(int i = 1;i < area_tsv.size();i++){
                double x = std::stod(area_tsv[i][2]);
                double y = std::stod(area_tsv[i][3]);
                area_map[std::stoi(area_tsv[i][1])] = x * y;
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_AREA_CALCULATER_HPP