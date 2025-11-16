/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_CONFIG_SIMULATION_CONST_HPP
#define PAX_SAPIENTICA_SIMULATION_CONFIG_SIMULATION_CONST_HPP

#include <array>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/IO/Data/SimulationRange.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>

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

    /// @brief
    /// @brief 生命表
    struct lifeSpan {
        std::vector<double> weight_farming_female{}; // 農耕民の女性の重み
        std::vector<double> weight_farming_male{}; // 農耕民の男性の重み
        std::vector<double> weight_hunter_gatherer_female{}; // 狩猟採集民の女性の重み
        std::vector<double> weight_hunter_gatherer_male{}; // 狩猟採集民の男性の重み
        std::discrete_distribution<> dist_farming_female{};
        std::discrete_distribution<> dist_farming_male{};
        std::discrete_distribution<> dist_hunter_gatherer_female{};
        std::discrete_distribution<> dist_hunter_gatherer_male{};
    };

    /// @brief
    /// @brief 妊娠確率
    struct MarriageProbability {
        std::vector<double> agricultural; // 農耕民の確率
        std::vector<double> hunter_gatherer; // 狩猟採集民の確率
    };
    /// @brief
    /// @brief 出産確率
    struct ChildbearingProbability {
        std::vector<double> agricultural; // 農耕民の確率
        std::vector<double> hunter_gatherer; // 狩猟採集民の確率
    };


    struct SimulationConstants {
        // Meyer's Singleton パターン
        static SimulationConstants& getInstance(const std::string& model_name_ = "Sample") {
            static SimulationConstants instance(model_name_);
            return instance;
        }

        // コピー・ムーブを禁止
        SimulationConstants(const SimulationConstants&) = delete;
        SimulationConstants& operator=(const SimulationConstants&) = delete;
        SimulationConstants(SimulationConstants&&) = delete;
        SimulationConstants& operator=(SimulationConstants&&) = delete;

        ~SimulationConstants() = default;

        int start_julian_day = 1728746; // シミュレーション開始日（ユリウス日）
        std::uint_least32_t area = MurMur3::calcHash("japan"); // シミュレーションの対象範囲
        std::uint_least32_t land_key = MurMur3::calcHash("district"); // 陸地データの Key
        std::uint_least32_t district_key = MurMur3::calcHash("district"); // 地区データの Key
        SimulationRange sr;

        int steps_per_year = 12; // 1年あたりのステップ数

        int output_step_frequency = 12; // 何ステップおきに出力するか？

        int total_steps = 0; // 総ステップ数（０は指定なし）
        int num_iterations = 1; // シミュレーションの繰り返し回数

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

        //　結婚時に近くの集落からエージェントを探す際の探索範囲
        std::uint_least32_t marriage_search_range = 60;
        std::uint_least32_t marriage_search_range_pow2 = marriage_search_range * marriage_search_range;

        // 集落をグループ分けする際の1グリッド辺の長さ
        std::uint_least32_t cell_group_length = 64;

        // 渡来開始ステップ数
        std::uint_least64_t immigration_start_steps = 2401; // 前 900 年
        std::uint_least64_t immigration_end_steps = 21600;

        // 渡来のステップ間隔 (step)
        std::uint_least64_t immigration_step_interval = 1;

        // 青銅開始ステップ数
        std::uint_least64_t bronze_start_steps = 9601; // 前 300 年

        // 渡来地区 ID
        std::uint_least8_t immigration_district_id = 73;

        // 青銅量
        std::uint_least64_t bronze = 500; // 渡来人が持ってくる青銅量

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

        lifeSpan life_span;
        MarriageProbability marriage_probability;
        ChildbearingProbability childbearing_probability;

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

        std::string output_directory_name = "Sample";

        std::string marriage_file = "Marriage.tsv";
        std::string childbearing_file = "Childbearing.tsv";
        std::string life_span_file = "LifeSpan.tsv";

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

        // 項目の ID を返す
        std::size_t getMenuIndex(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) const {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }

        AgeType getLifeSpan(const bool is_farming, const bool is_female, std::mt19937& gen) noexcept {
            return static_cast<AgeType>((is_farming) ?
                ((is_female) ? life_span.dist_farming_female(gen) :
                    life_span.dist_farming_male(gen)) :
                ((is_female) ? life_span.dist_hunter_gatherer_female(gen) :
                    life_span.dist_hunter_gatherer_male(gen)));
        }

        void inputLifeSpan(const std::string& model_name_) noexcept {
            std::string path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("SimulationProvincesPath"));
            // Sample を選択モデル名に置換
            paxs::StringUtils::replace(path, "Sample", model_name_);
            path += std::string("/" + life_span_file);

            paxs::InputFile life_span_tsv(path);
            if (life_span_tsv.fail()) {
                PAXS_WARNING("Failed to read LifeSpan TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(life_span_tsv.getLine())) {
                PAXS_WARNING("LifeSpan TSV file is empty: " + path);
                return; // 何もない場合
            }
            // BOM を削除
            life_span_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = life_span_tsv.splitHashMapMurMur3('\t');
#ifdef PAXS_DEVELOPMENT
            std::size_t i = 1;
#endif
            // 生命表を初期化
            life_span = lifeSpan{};
            // 1 行ずつ読み込み（区切りはタブ）
            while (life_span_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = life_span_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("hunter_gatherer_male_ndx")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("hunter_gatherer_female_ndx")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("agricultural_male_ndx")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("agricultural_female_ndx"))
                    ) {
#ifdef PAXS_DEVELOPMENT
                    PAXS_WARNING("Failed to read Japan LifeSpan TSV file: " + path + " at line " + std::to_string(i));
#endif

                    life_span.weight_farming_female.emplace_back(0.0);
                    life_span.weight_farming_male.emplace_back(0.0);
                    life_span.weight_hunter_gatherer_female.emplace_back(0.0);
                    life_span.weight_hunter_gatherer_male.emplace_back(0.0);
#ifdef PAXS_DEVELOPMENT
                    ++i;
#endif
                    continue;
                }
                life_span.weight_farming_female.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("agricultural_female_ndx")]], 0.0, true));
                life_span.weight_farming_male.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("agricultural_male_ndx")]], 0.0, true));
                life_span.weight_hunter_gatherer_female.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("hunter_gatherer_female_ndx")]], 0.0, true));
                life_span.weight_hunter_gatherer_male.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("hunter_gatherer_male_ndx")]], 0.0, true));
                // 確率分布を生成
                life_span.dist_farming_female = std::discrete_distribution<>(life_span.weight_farming_female.begin(), life_span.weight_farming_female.end());
                life_span.dist_farming_male = std::discrete_distribution<>(life_span.weight_farming_male.begin(), life_span.weight_farming_male.end());
                life_span.dist_hunter_gatherer_female = std::discrete_distribution<>(life_span.weight_hunter_gatherer_female.begin(), life_span.weight_hunter_gatherer_female.end());
                life_span.dist_hunter_gatherer_male = std::discrete_distribution<>(life_span.weight_hunter_gatherer_male.begin(), life_span.weight_hunter_gatherer_male.end());
#ifdef PAXS_DEVELOPMENT
                ++i;
#endif
            }
        }

        // 婚姻確率を返す
        double getMarriageProbability(const std::size_t age_, const bool is_agricultural) const noexcept {
            return (is_agricultural) ? marriage_probability.agricultural[age_] :
                marriage_probability.hunter_gatherer[age_];
        }
        // 出産確率を返す
        double getChildbearingProbability(const std::size_t age_, const bool is_agricultural) const noexcept {
            return (is_agricultural) ? childbearing_probability.agricultural[age_] :
                childbearing_probability.hunter_gatherer[age_];
        }

        void inputMarriage(const std::string& model_name_) noexcept {
            std::string path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("SimulationProvincesPath"));
            // Sample を選択モデル名に置換
            paxs::StringUtils::replace(path, "Sample", model_name_);
            path += std::string("/" + marriage_file);
            paxs::InputFile probability_tsv(path);
            if (probability_tsv.fail()) {
                PAXS_WARNING("Failed to read Marriage TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(probability_tsv.getLine())) {
                PAXS_WARNING("Marriage TSV file is empty: " + path);
                return; // 何もない場合
            }
            // BOM を削除
            probability_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = probability_tsv.splitHashMapMurMur3('\t');
            marriage_probability.agricultural.clear();
            marriage_probability.hunter_gatherer.clear();
#ifdef PAXS_DEVELOPMENT
            std::size_t i = 1;
#endif
            // 婚姻確率を初期化
            marriage_probability = MarriageProbability{};
            // 1 行ずつ読み込み（区切りはタブ）
            while (probability_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = probability_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("hunter_gatherer")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("agricultural"))
                    ) {
#ifdef PAXS_DEVELOPMENT
                    PAXS_WARNING("Failed to read Japan Marriage TSV file: " + path + " at line " + std::to_string(i));
#endif
                    marriage_probability.agricultural.emplace_back(0.0);
                    marriage_probability.hunter_gatherer.emplace_back(0.0);
#ifdef PAXS_DEVELOPMENT
                    ++i;
#endif
                    continue;
                }
                marriage_probability.agricultural.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("agricultural")]], 0.0, true));
                marriage_probability.hunter_gatherer.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("hunter_gatherer")]], 0.0, true));
#ifdef PAXS_DEVELOPMENT
                ++i;
#endif
            }
        }
        void inputChildbearing(const std::string& model_name_) noexcept {
            std::string path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("SimulationProvincesPath"));
            // Sample を選択モデル名に置換
            paxs::StringUtils::replace(path, "Sample", model_name_);
            if (path.size() == 0) return;
            path += std::string("/" + childbearing_file);
            paxs::InputFile probability_tsv(path);
            if (probability_tsv.fail()) {
                PAXS_WARNING("Failed to read Childbearing TSV file: " + path);
                return;
            }
            // 1 行目を読み込む
            if (!(probability_tsv.getLine())) {
                PAXS_WARNING("Childbearing TSV file is empty: " + path);
                return; // 何もない場合
            }
            // BOM を削除
            probability_tsv.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = probability_tsv.splitHashMapMurMur3('\t');
            childbearing_probability.agricultural.clear();
            childbearing_probability.hunter_gatherer.clear();
#ifdef PAXS_DEVELOPMENT
            std::size_t i = 1;
#endif
            // 出産確率を初期化
            childbearing_probability = ChildbearingProbability{};
            // 1 行ずつ読み込み（区切りはタブ）
            while (probability_tsv.getLine()) {
                std::vector<std::string> sub_menu_v = probability_tsv.split('\t');
                if (
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("hunter_gatherer")) ||
                    sub_menu_v.size() <= getMenuIndex(menu, MurMur3::calcHash("agricultural"))
                    ) {
#ifdef PAXS_DEVELOPMENT
                    PAXS_WARNING("Failed to read Japan Childbearing TSV file: " + path + " at line " + std::to_string(i));
#endif
                    childbearing_probability.agricultural.emplace_back(0.0);
                    childbearing_probability.hunter_gatherer.emplace_back(0.0);
#ifdef PAXS_DEVELOPMENT
                    ++i;
#endif
                    continue;
                }
                childbearing_probability.agricultural.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("agricultural")]], 0.0, true));
                childbearing_probability.hunter_gatherer.emplace_back(StringUtils::safeStod(sub_menu_v[menu[MurMur3::calcHash("hunter_gatherer")]], 0.0, true));
#ifdef PAXS_DEVELOPMENT
                ++i;
#endif
            }
        }

        void init(const std::string& model_name_ = "Sample") {
            std::string str = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("SimulationConstants"));
            // Sample を選択モデル名に置換
            paxs::StringUtils::replace(str, "Sample", model_name_);
            if (str.size() == 0) {
                PAXS_WARNING("Simulation Constants TSV file path is empty.");
                return;
            }

            KeyValueTSV<std::string> kvt;
            if (!kvt.input(str)) {
                PAXS_WARNING("Failed to read Simulation Constants TSV file: " + str);
                return;
            }

            stoiFunc(kvt, MurMur3::calcHash("start_julian_day"), [&](const std::string& str_) {start_julian_day = StringUtils::safeStoi(str_, 0, true); });
            stoiFunc(kvt, MurMur3::calcHash("area"), [&](const std::string& str_) {area = MurMur3::calcHash(str_.size(), str_.c_str()); });
            stoiFunc(kvt, MurMur3::calcHash("land_key"), [&](const std::string& str_) {land_key = MurMur3::calcHash(str_.size(), str_.c_str()); });
            stoiFunc(kvt, MurMur3::calcHash("district_key"), [&](const std::string& str_) {district_key = MurMur3::calcHash(str_.size(), str_.c_str()); });

            // シミュレーションの範囲を設定
            AppConfig::getInstance().ifSettingExists(MurMur3::calcHash("SimulationRange"),
                [&](const std::string& path_) {sr.input(path_); });

            stoiFunc(kvt, MurMur3::calcHash("steps_per_year"), [&](const std::string& str_) {steps_per_year = StringUtils::safeStoi(str_, 1, true); });
            step_per_year_dist = std::uniform_int_distribution<int>(0, steps_per_year - 1);

            stoiFunc(kvt, MurMur3::calcHash("output_step_frequency"), [&](const std::string& str_) {output_step_frequency = StringUtils::safeStoi(str_, 1, true); });
            stoiFunc(kvt, MurMur3::calcHash("total_steps"), [&](const std::string& str_) {total_steps = StringUtils::safeStoi(str_, 0, true); });
            stoiFunc(kvt, MurMur3::calcHash("num_iterations"), [&](const std::string& str_) {num_iterations = StringUtils::safeStoi(str_, 1, true); });

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

            stoiFunc(kvt, MurMur3::calcHash("cell_group_length"), [&](const std::string& str_) {cell_group_length = static_cast<std::uint_least32_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("immigration_start_steps"), [&](const std::string& str_) {immigration_start_steps = static_cast<std::uint_least64_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("immigration_end_steps"), [&](const std::string& str_) {immigration_end_steps = static_cast<std::uint_least64_t>(std::stoul(str_)); });
            stoiFunc(kvt, MurMur3::calcHash("immigration_step_interval"), [&](const std::string& str_) {immigration_step_interval = static_cast<std::uint_least64_t>(std::stoul(str_)); });
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

            stoiFunc(kvt, MurMur3::calcHash("move_probability"), [&](const std::string& str_) {move_probability = StringUtils::safeStod(str_, 0.0, true); });
            stoiFunc(kvt, MurMur3::calcHash("child_agriculture_priority"), [&](const std::string& str_) {child_agriculture_priority = StringUtils::safeStod(str_, 0.0, true); });
            stoiFunc(kvt, MurMur3::calcHash("hunter_gatherer_stillbirth_rate"), [&](const std::string& str_) {hunter_gatherer_stillbirth_rate = StringUtils::safeStod(str_, 0.0, true); });
            stoiFunc(kvt, MurMur3::calcHash("agricultural_stillbirth_rate"), [&](const std::string& str_) {agricultural_stillbirth_rate = StringUtils::safeStod(str_, 0.0, true); });
            stoiFunc(kvt, MurMur3::calcHash("maternal_residence_probability"), [&](const std::string& str_) {maternal_residence_probability = StringUtils::safeStod(str_, 0.0, true); });

            stoiFunc(kvt, MurMur3::calcHash("coast_cost"), [&](const std::string& str_) {coast_cost = StringUtils::safeStod(str_, 0.0, true); });
            stoiFunc(kvt, MurMur3::calcHash("ocean_cost"), [&](const std::string& str_) {ocean_cost = StringUtils::safeStod(str_, 0.0, true); });
            stoiFunc(kvt, MurMur3::calcHash("land_cost"), [&](const std::string& str_) {land_cost = StringUtils::safeStod(str_, 0.0, true); });

            pregnant_age_min_f64 = childbearing_age_min_f64 - static_cast<double>(birth_interval) / static_cast<double>(steps_per_year);

            stoiFunc(kvt, MurMur3::calcHash("output_directory_name"), [&](const std::string& str_) {output_directory_name = str_; });

            stoiFunc(kvt, MurMur3::calcHash("marriage_file"), [&](const std::string& str_) {marriage_file = str_; });
            stoiFunc(kvt, MurMur3::calcHash("childbearing_file"), [&](const std::string& str_) {childbearing_file = str_; });
            stoiFunc(kvt, MurMur3::calcHash("life_span_file"), [&](const std::string& str_) {life_span_file = str_; });

            inputMarriage(model_name_); // 婚姻確率
            inputChildbearing(model_name_); // 出産確率
            inputLifeSpan(model_name_); // 生命表
        }

        SimulationConstants(const std::string& model_name_ = "Sample") {
            init(model_name_);
        }

    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_CONFIG_SIMULATION_CONST_HPP
