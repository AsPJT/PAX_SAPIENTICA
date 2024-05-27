/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_SIMULATION_CONST_HPP
#define PAX_SAPIENTICA_SIMULATION_SIMULATION_CONST_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstdint>
#include <string>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/InputFile/SimulationRange.hpp>

namespace paxs {

    constexpr int pixel_size = 256; // 1画像あたりの縦横のピクセル数

    constexpr std::size_t max_number_of_districts = 90; // 最大地区数

    using GridType = int;

    struct SimulationConstants {
        // インスタンスを取得
        static SimulationConstants* getInstance() {
            if (instance == nullptr) {
                instance = new SimulationConstants();
            }
            return instance;
        }
        static SimulationConstants* instance;

        ~SimulationConstants() {
            delete instance;
        }

        int start_julian_day = 1728746; // シミュレーション開始日（ユリウス日）
        std::uint_least32_t area = MurMur3::calcHash("japan"); // シミュレーションの対象範囲
        SimulationRange sr;

        int unit_group_min = 11; // 単位集団の最小人数
        int unit_group_max = 40; // 単位集団の最大人数
        int basic_group_min = 41; // 基礎集団の最小人数
        int basic_group_max = 240; // 基礎集団の最大人数
        int composite_settlement_min = 82; // 複合集落の最小人数

        int steps_per_year = 12; // 1年あたりのステップ数

        std::uint_least8_t female = 0; // 女性
        std::uint_least8_t male = 1; // 男性

        // 女性の結婚可能年齢：13歳以上60歳未満, 男性の結婚可能年齢：17歳以上60歳未満
        std::uint_least32_t female_marriageable_age_min = 13;
        std::uint_least32_t male_marriageable_age_min = 17;
        std::uint_least32_t female_marriageable_age_max = 60;
        std::uint_least32_t male_marriageable_age_max = 70;
        // 出産可能：15歳以上50歳未満
        std::uint_least32_t birthable_age_min = 15;
        std::uint_least32_t birthable_age_max = 50;
        // 出産の間隔：10ヶ月
        std::uint_least8_t birth_interval = 10;

        //　結婚時に近くの集落からエージェントを探す際の探索範囲
        std::uint_least32_t marriage_search_range = 60;

        // 集落をグループ分けする際の1グリッド辺の長さ
        std::uint_least32_t grid_length = 64;

        // 集落の最大人数
        std::uint_least8_t max_settlement_population = 80;

        // 最小移動距離
        std::uint_least32_t min_move_distance = 10;
        // 最大移動距離
        std::uint_least32_t max_move_distance = 800;
        // 移動確率下限
        int min_move_probability = 1;
        // 移動確率上限
        int max_move_probability = 10;
        // 移動確率の正規化係数
        int move_probability_normalization_coefficient = 1000;

        // 片親が農耕文化を持ち、もう一方の片親が農耕文化を持たない時の農耕文化継承の優先度
        float child_agriculture_priority = 0.7f;

        // 死産率
        float stillbirth_rate = 0.1f;

    private:
        template<typename Func_>
        void stoiFunc(KeyValueTSV<std::string>& key_value_tsv_, const std::uint_least32_t key_, Func_&& func_) {
            const std::string& str = key_value_tsv_[key_];
            if (str.size() == 0) return;
            func_(str);
        }

    public:

        paxs::Vector2<int>& getStartArea() {
            return sr.getStart(area);
        }
        paxs::Vector2<int>& getEndArea() {
            return sr.getEnd(area);
        }
        int getZ() {
            return sr.getZ(area);
        }

        SimulationConstants() {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationConstants"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            KeyValueTSV<std::string> kvt;
            kvt.input(str);

            stoiFunc(kvt, MurMur3::calcHash("start_julian_day"), [&](const std::string& str_) {start_julian_day = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("area"), [&](const std::string& str_) {area = MurMur3::calcHash(str_.size(), str_.c_str()); });

            // シミュレーションの範囲を設定
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationRange"),
                [&](const std::string& path_) {sr.input(path_); });

            stoiFunc(kvt, MurMur3::calcHash("unit_group_min"), [&](const std::string& str_) {unit_group_min = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("unit_group_max"), [&](const std::string& str_) {unit_group_max = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("basic_group_min"), [&](const std::string& str_) {basic_group_min = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("basic_group_max"), [&](const std::string& str_) {basic_group_max = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("composite_settlement_min"), [&](const std::string& str_) {composite_settlement_min = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("steps_per_year"), [&](const std::string& str_) {steps_per_year = std::stoi(str_); });

            stoiFunc(kvt, MurMur3::calcHash("female"), [&](const std::string& str_) {female = static_cast<std::uint_least8_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("male"), [&](const std::string& str_) {male = static_cast<std::uint_least8_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("female_marriageable_age_min"), [&](const std::string& str_) {female_marriageable_age_min = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("male_marriageable_age_min"), [&](const std::string& str_) {male_marriageable_age_min = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("female_marriageable_age_max"), [&](const std::string& str_) {female_marriageable_age_max = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("male_marriageable_age_max"), [&](const std::string& str_) {male_marriageable_age_max = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("birthable_age_min"), [&](const std::string& str_) {birthable_age_min = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("birthable_age_max"), [&](const std::string& str_) {birthable_age_max = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("birth_interval"), [&](const std::string& str_) {birth_interval = static_cast<std::uint_least8_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("marriage_search_range"), [&](const std::string& str_) {marriage_search_range = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("grid_length"), [&](const std::string& str_) {grid_length = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("max_settlement_population"), [&](const std::string& str_) {max_settlement_population = static_cast<std::uint_least8_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("min_move_distance"), [&](const std::string& str_) {min_move_distance = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("max_move_distance"), [&](const std::string& str_) {max_move_distance = static_cast<std::uint_least32_t>(std::stoul(str_)); });

            stoiFunc(kvt, MurMur3::calcHash("min_move_probability"), [&](const std::string& str_) {min_move_probability = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("max_move_probability"), [&](const std::string& str_) {max_move_probability = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("move_probability_normalization_coefficient"), [&](const std::string& str_) {move_probability_normalization_coefficient = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("child_agriculture_priority"), [&](const std::string& str_) {child_agriculture_priority = std::stof(str_); });
            stoiFunc(kvt, MurMur3::calcHash("stillbirth_rate"), [&](const std::string& str_) {stillbirth_rate = std::stof(str_); });

        }

    };
    SimulationConstants* SimulationConstants::instance = nullptr;
}

#endif // !PAX_SAPIENTICA_SIMULATION_SIMULATION_CONST_HPP
