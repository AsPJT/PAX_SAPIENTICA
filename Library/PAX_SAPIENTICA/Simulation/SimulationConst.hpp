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

#include <array>
#include <cstdint>
#include <random>
#include <string>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/InputFile/SimulationRange.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

// M_PI が定義されていない場合
#ifndef M_PI
#define M_PI 3.141592653589
#endif

namespace paxs {

    constexpr int pixel_size = 256; // 1画像あたりの縦横のピクセル数

    constexpr std::size_t max_number_of_districts = 90; // 最大地区数

    constexpr std::uint_least8_t female_value = 0; // 女性
    constexpr std::uint_least8_t male_value = 1; // 男性

    constexpr double pi_per_4 = M_PI / 4.0; // π／４
    constexpr double pi_per_8 = M_PI / 8.0; // π／８
    // constexpr double pi_per_4m8 = pi_per_4 - pi_per_8; // π／４ーπ／８

    // std::sqrt(2 * M_PI)
    constexpr double sqrt_2_x_pi = static_cast<double>(2.506628275);

    using GridType = int;
    using HumanIndexType = std::uint_least32_t;
    using AgeType = std::uint_least16_t;

    // 集落グリッドの辞書型
    using SettlementGridsType = std::uint_least16_t;
    // Data の辞書型
    using DataGridsType = std::uint_least32_t;

    constexpr std::array <Vector2<GridType>, 8> astar_adjacent_cell{ {Vector2<GridType>(-1, -1),Vector2<GridType>(1, -1),Vector2<GridType>(-1, 1),Vector2<GridType>(1, 1),Vector2<GridType>(0, -1),Vector2<GridType>(-1, 0),Vector2<GridType>(0, 1),Vector2<GridType>(1, 0)} };

    // 始点と終点を管理（婚姻の前後の位置情報を保持する用）
    struct GridType4 {
        GridType sx{}, sy{}, ex{}, ey{};
        bool is_matrilocality = false;
    };
    // 始点と終点を管理（婚姻の前後の位置情報を保持する用）
    struct Marriage3 {
        std::uint_least32_t first{}, second{}, third{};
        std::uint_least8_t farming{};
    };

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
        std::uint_least32_t land_key = MurMur3::calcHash("district"); // 陸地データの Key
        std::uint_least32_t district_key = MurMur3::calcHash("district"); // 地区データの Key
        SimulationRange sr;

        int steps_per_year = 12; // 1年あたりのステップ数

        int output_step_frequency = 12; // 何ステップおきに出力するか？

        // 女性の結婚可能年齢：13歳以上60歳未満, 男性の結婚可能年齢：17歳以上60歳未満
        std::uint_least32_t female_marriageable_age_min = 13;
        double female_marriageable_age_min_f64 = 13.0;
        std::uint_least32_t male_marriageable_age_min = 17;
        std::uint_least32_t female_marriageable_age_max = 60;
        std::uint_least32_t male_marriageable_age_max = 70;
        // 出産可能：15歳以上50歳未満
        std::uint_least32_t childbearing_age_min = 15;
        double childbearing_age_min_f64 = 15.0;
        std::uint_least32_t childbearing_age_max = 50;
        // 出産の間隔：10ヶ月
        std::uint_least8_t birth_interval = 10;
        // 妊娠可能
        double pregnant_age_min_f64 = childbearing_age_min_f64 - static_cast<double>(birth_interval) / static_cast<double>(steps_per_year);
        // 婚姻可能年齢定数
        double marriageable_age_constant = 8.5;
        double marriageable_age_threshold = 0.98;
        double marriageable_age_all_weight = 101.8;
        // 出産可能年齢定数
        double childbearing_age_constant = 8.5;
        double childbearing_age_threshold = 16.0;
        double childbearing_age_all_weight = 101.8;

        //　結婚時に近くの集落からエージェントを探す際の探索範囲
        std::uint_least32_t marriage_search_range = 60;
        std::uint_least32_t marriage_search_range_pow2 = marriage_search_range * marriage_search_range;

        // 集落をグループ分けする際の1グリッド辺の長さ
        std::uint_least32_t cell_group_length = 64;

        // 渡来開始ステップ数
        std::uint_least64_t immigration_start_steps = 2401; // 前 900 年
        std::uint_least64_t immigration_end_steps = 21600;

        // 青銅開始ステップ数
        std::uint_least64_t bronze_start_steps = 9601; // 前 300 年

        // 渡来地区 ID
        std::uint_least8_t immigration_district_id = 73;

        // 青銅量
        std::uint_least64_t bronze = 100; // 渡来人が持ってくる青銅量

        // 初期化時の寿命までの最低年数
        AgeType init_lifespan_grace_period = 180;

        // 農耕集落の最大人数
        double max_agricultural_settlement_weight = 1.0 / 80.0;
        // 狩猟採集集落の最大人数
        double max_hunter_gatherer_settlement_weight = 1.0 / 25.0;

        // 最小移動距離
        std::uint_least32_t min_move_distance = 10;
        // 最大移動距離
        std::uint_least32_t max_move_distance = 800;
        // 移動確率
        double move_probability = 1;
        // 移動再試行回数
        std::uint_least32_t move_redo = 10;
        // A* を行うルート数
        std::uint_least32_t move_astar_loop = 20;
        // A* を行うルート間隔
        std::uint_least32_t move_astar_distance = 32;

        std::uint_least32_t move_method = MurMur3::calcHash("random");

        // 片親が農耕文化を持ち、もう一方の片親が農耕文化を持たない時の農耕文化継承の優先度
        double child_agriculture_priority = 0.7;

        // 狩猟採集文化の死産率
        double hunter_gatherer_stillbirth_rate = 0.1;
        // 農耕文化の死産率
        double agricultural_stillbirth_rate = 0.1;

        // 母方居住婚の確率
        double maternal_residence_probability = 0.5;

        double coast_cost = 0.5; // 海岸の通行コスト
        double ocean_cost = 0.5; // 海上の通行コスト
        double land_cost = 0.5; // 傾斜度0度の陸上の通行コスト

        std::uniform_real_distribution<float> random_dist_f32 = std::uniform_real_distribution<float>( 0.0f, 1.0f ); // 乱数分布
        std::uniform_real_distribution<double> random_dist = std::uniform_real_distribution<double>( 0.0, 1.0 ); // 乱数分布
        std::uniform_int_distribution<int> step_per_year_dist = std::uniform_int_distribution<int>( 0, 11 ); // 乱数分布

        std::uniform_int_distribution<int> move_dist = std::uniform_int_distribution<int>(min_move_distance, max_move_distance);
        std::uniform_real_distribution<double> theta_dist = std::uniform_real_distribution<double>(0.0, static_cast<double>(2.0 * M_PI));

        std::array<std::uniform_real_distribution<double>, 8> theta_dist_array = {
            std::uniform_real_distribution<double>(-pi_per_8, pi_per_8),
            std::uniform_real_distribution<double>(pi_per_8, pi_per_8 + pi_per_4),
            std::uniform_real_distribution<double>(pi_per_8 + pi_per_4, pi_per_8 + 2* pi_per_4),
            std::uniform_real_distribution<double>(pi_per_8 + 2 * pi_per_4, pi_per_8 + 3 * pi_per_4),
            std::uniform_real_distribution<double>(pi_per_8 + 3 * pi_per_4, pi_per_8 + 4 * pi_per_4),
            std::uniform_real_distribution<double>(pi_per_8 + 4 * pi_per_4, pi_per_8 + 5 * pi_per_4),
            std::uniform_real_distribution<double>(pi_per_8 + 5 * pi_per_4, pi_per_8 + 6 * pi_per_4),
            std::uniform_real_distribution<double>(pi_per_8 + 7 * pi_per_4, pi_per_8 + 8 * pi_per_4)
        };

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

        void init() {
            std::string str = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationConstants"),
                [&](const std::string& path_) {str = path_; });
            if (str.size() == 0) return;

            KeyValueTSV<std::string> kvt;
            kvt.input(str);

            stoiFunc(kvt, MurMur3::calcHash("start_julian_day"), [&](const std::string& str_) {start_julian_day = std::stoi(str_); });
            stoiFunc(kvt, MurMur3::calcHash("area"), [&](const std::string& str_) {area = MurMur3::calcHash(str_.size(), str_.c_str()); });
            stoiFunc(kvt, MurMur3::calcHash("land_key"), [&](const std::string& str_) {land_key = MurMur3::calcHash(str_.size(), str_.c_str()); });
            stoiFunc(kvt, MurMur3::calcHash("district_key"), [&](const std::string& str_) {district_key = MurMur3::calcHash(str_.size(), str_.c_str()); });

            // シミュレーションの範囲を設定
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationRange"),
                [&](const std::string& path_) {sr.input(path_); });

            stoiFunc(kvt, MurMur3::calcHash("steps_per_year"), [&](const std::string& str_) {steps_per_year = std::stoi(str_); });
            step_per_year_dist = std::uniform_int_distribution<int>(0, steps_per_year - 1);

            stoiFunc(kvt, MurMur3::calcHash("output_step_frequency"), [&](const std::string& str_) {output_step_frequency = std::stoi(str_); });

            stoiFunc(kvt, MurMur3::calcHash("female_marriageable_age_min"), [&](const std::string& str_) {female_marriageable_age_min = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            female_marriageable_age_min_f64 = static_cast<double>(female_marriageable_age_min);

            stoiFunc(kvt, MurMur3::calcHash("male_marriageable_age_min"), [&](const std::string& str_) {male_marriageable_age_min = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("female_marriageable_age_max"), [&](const std::string& str_) {female_marriageable_age_max = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("male_marriageable_age_max"), [&](const std::string& str_) {male_marriageable_age_max = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("childbearing_age_min"), [&](const std::string& str_) {childbearing_age_min = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            childbearing_age_min_f64 = static_cast<double>(childbearing_age_min);
            stoiFunc(kvt, MurMur3::calcHash("childbearing_age_max"), [&](const std::string& str_) {childbearing_age_max = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("birth_interval"), [&](const std::string& str_) {birth_interval = static_cast<std::uint_least8_t>(std::stoul(str_)); });

            stoiFunc(kvt, MurMur3::calcHash("marriage_search_range"), [&](const std::string& str_) {marriage_search_range = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            marriage_search_range_pow2 = marriage_search_range * marriage_search_range;

            stoiFunc(kvt, MurMur3::calcHash("marriageable_age_constant"), [&](const std::string& str_) {marriageable_age_constant = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("marriageable_age_threshold"), [&](const std::string& str_) {marriageable_age_threshold = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("marriageable_age_all_weight"), [&](const std::string& str_) {marriageable_age_all_weight = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("childbearing_age_constant"), [&](const std::string& str_) {childbearing_age_constant = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("childbearing_age_threshold"), [&](const std::string& str_) {childbearing_age_threshold = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("childbearing_age_all_weight"), [&](const std::string& str_) {childbearing_age_all_weight = std::stod(str_); });

            stoiFunc(kvt, MurMur3::calcHash("cell_group_length"), [&](const std::string& str_) {cell_group_length = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("immigration_start_steps"), [&](const std::string& str_) {immigration_start_steps = static_cast<std::uint_least64_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("immigration_end_steps"), [&](const std::string& str_) {immigration_end_steps = static_cast<std::uint_least64_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("immigration_district_id"), [&](const std::string& str_) {immigration_district_id = static_cast<std::uint_least8_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("init_lifespan_grace_period"), [&](const std::string& str_) {init_lifespan_grace_period = static_cast<AgeType>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("max_agricultural_settlement_population"), [&](const std::string& str_) {max_agricultural_settlement_weight = 1.0 / static_cast<std::uint_least64_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("max_hunter_gatherer_settlement_population"), [&](const std::string& str_) {max_hunter_gatherer_settlement_weight = 1.0 / static_cast<std::uint_least64_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("min_move_distance"), [&](const std::string& str_) {min_move_distance = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("max_move_distance"), [&](const std::string& str_) {max_move_distance = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("move_redo"), [&](const std::string& str_) {move_redo = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("move_astar_loop"), [&](const std::string& str_) {move_astar_loop = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("move_astar_distance"), [&](const std::string& str_) {move_astar_distance = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("move_method"), [&](const std::string& str_) {move_method = MurMur3::calcHash(str_.size(), str_.c_str()); });
            move_dist = std::uniform_int_distribution<int>(min_move_distance, max_move_distance);

            stoiFunc(kvt, MurMur3::calcHash("move_probability"), [&](const std::string& str_) {move_probability = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("child_agriculture_priority"), [&](const std::string& str_) {child_agriculture_priority = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("hunter_gatherer_stillbirth_rate"), [&](const std::string& str_) {hunter_gatherer_stillbirth_rate = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("agricultural_stillbirth_rate"), [&](const std::string& str_) {agricultural_stillbirth_rate = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("maternal_residence_probability"), [&](const std::string& str_) {maternal_residence_probability = std::stod(str_); });

            stoiFunc(kvt, MurMur3::calcHash("coast_cost"), [&](const std::string& str_) {coast_cost = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("ocean_cost"), [&](const std::string& str_) {ocean_cost = std::stod(str_); });
            stoiFunc(kvt, MurMur3::calcHash("land_cost"), [&](const std::string& str_) {land_cost = std::stod(str_); });

            pregnant_age_min_f64 = childbearing_age_min_f64 - static_cast<double>(birth_interval) / static_cast<double>(steps_per_year);
        }

        SimulationConstants() {
            init();
        }

    };
    SimulationConstants* SimulationConstants::instance = nullptr;
}

#endif // !PAX_SAPIENTICA_SIMULATION_SIMULATION_CONST_HPP
