/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_KOYOMI_HPP
#define PAX_SAPIENTICA_CALENDAR_KOYOMI_HPP

#include <array>
#include <limits>
#include <string>
#include <variant>
#include <vector>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#endif

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Calendars.hpp>
#include <PAX_SAPIENTICA/Calendar/ChineseEra.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/JapaneseEra.hpp>
#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/Key/CalendarKeys.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    class Koyomi {
    public:
        /// @brief 出力に必要な日付の情報
        struct OutputDate {
            std::uint_least32_t calendar_name_key; // 暦の名前
            cal::Calendars date{}; // 日付
        };

        cal::JDN_F64 jdn{}; // ユリウス通日（基準となる暦）
        cal::SimulationSteps steps{}; // シミュレーションのステップ数

        // 表示する暦レイヤー（8種類の暦）
        std::array<OutputDate, 8> date_list = {
            OutputDate{CalendarKeys::CALENDAR_JAPAN_HASH, cal::JapanDate{}},
            OutputDate{CalendarKeys::CALENDAR_GREGORIAN_HASH, cal::GregorianDate{}},
            OutputDate{CalendarKeys::CALENDAR_JULIAN_HASH, cal::JulianDate{}},
            OutputDate{CalendarKeys::CALENDAR_HIJRI_HASH, cal::IslamicDate{}},
            OutputDate{CalendarKeys::CALENDAR_CHINESE_HASH, cal::ChinaDate{}},
            OutputDate{CalendarKeys::CALENDAR_JULIAN_DAY_HASH, cal::JDN_S64{}},
            OutputDate{CalendarKeys::CALENDAR_CALBP_HASH, cal::CalBP{}},
            OutputDate{CalendarKeys::MENU_BAR_VIEW_SIMULATION_HASH, cal::SimulationSteps{}}
        };

        std::vector<paxs::JapaneseEra> japanese_era_list; // 日本の元号の一覧
        std::vector<paxs::ChineseEra> chinese_era_list; // 中国大陸の元号の一覧

        static constexpr std::uint_least32_t sueki_nakamura_key = CalendarKeys::SUEKI_NAKAMURA_HASH; // 須恵器の中村編年の配列のインデックス値
        static constexpr std::uint_least32_t sueki_tanabe_key = CalendarKeys::SUEKI_TANABE_HASH; // 須恵器の田辺編年の配列のインデックス値

        // 時代区分の文字列
        static constexpr std::array period_names = {
            "旧石器時代", "縄文時代", "弥生時代", "古墳時代 CE251-",
            "飛鳥時代 CE592-", "奈良時代 CE710-", "平安時代 CE794-",
            "鎌倉時代", "室町時代", "安土桃山時代", "江戸時代", "明治時代" };
        // 時代区分ごとのユリウス日の数値
        static constexpr std::array period_jdn = {
            0, 0, 1538799, 1812736,
            1937666, 1980488, 2011388,
            2154234, 2209376, 2295823, 2306626, 2403629
        };
        // 月の英語名（完全版）
        static constexpr std::array month_name_long = {
            "", "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };
        // 月の英語名（短縮版）
        static constexpr std::array month_name = {
            "", "Jan.", "Feb.", "Mar.", "Apr.", "May", "Jun.",
            "Jul.", "Aug.", "Sep.", "Oct.", "Nov.", "Dec."
        };

        bool move_forward_in_time = true; // 時間を進めるか
        bool go_back_in_time = false; // 時間を戻すか
        bool is_agent_update = false; // エージェントの更新をするか

    private:
        // 暦の更新に関する定数
        static constexpr double days_per_year = 365.2425; // グレゴリオ暦の1年の平均日数
        static constexpr double time_scale_factor = 18.0; // 時間経過の速度調整係数
        static constexpr int calendar_update_threshold = 0; // 暦更新のしきい値

        /// @brief グレゴリオ暦に変換
        void updateGregorianDate(OutputDate& output_date) {
            output_date.date = jdn.toGregorianCalendar();
        }

        /// @brief ユリウス暦に変換
        void updateJulianDate(OutputDate& output_date) {
            output_date.date = jdn.toJulianCalendar();
        }

        /// @brief 和暦に変換
        void updateJapaneseDate(OutputDate& output_date) {
            const paxs::cal::JapanDate jp_date = jdn.toJapaneseCalendar(japanese_era_list);
            const std::string gengo = "gengo_" + std::to_string(jp_date.cgetGengo());
            output_date.calendar_name_key = MurMur3::calcHash(gengo.size(), gengo.c_str());
            output_date.date = jp_date;
        }

        /// @brief 中国暦に変換
        void updateChineseDate(OutputDate& output_date) {
            const paxs::cal::ChinaDate cn_date = jdn.toChineseCalendar(chinese_era_list);
            const std::string gengo = "chinese_calendar_" + std::to_string(cn_date.cgetGengo());
            output_date.calendar_name_key = MurMur3::calcHash(gengo.size(), gengo.c_str());
            output_date.date = cn_date;
        }

        /// @brief ユリウス通日を格納
        void updateJulianDayNumber(OutputDate& output_date) {
            output_date.date = jdn;
        }

        /// @brief BP（年代測定）に変換
        void updateCalBP(OutputDate& output_date) {
            output_date.date = jdn.toCalBP();
        }

        /// @brief イスラム暦（ヒジュラ暦）に変換
        void updateIslamicDate(OutputDate& output_date) {
            output_date.date = jdn.toIslamicCalendar();
        }

        /// @brief シミュレーションステップ数を格納
        void updateSimulationSteps(OutputDate& output_date) {
            output_date.date = steps;
        }

    public:
        void calcDate() {
            // 暦データを更新
            for (auto& dl : date_list) {
                switch (dl.date.index()) {
                case cal::gregorian_date_type:
                    updateGregorianDate(dl);
                    break;
                case cal::julian_date_type:
                    updateJulianDate(dl);
                    break;
                case cal::japan_date_type:
                    updateJapaneseDate(dl);
                    break;
                case cal::china_date_type:
                    updateChineseDate(dl);
                    break;
                case cal::jdn_f64_type:
                case cal::jdn_s32_type:
                case cal::jdn_s64_type:
                    updateJulianDayNumber(dl);
                    break;
                case cal::calbp_type:
                    updateCalBP(dl);
                    break;
                case cal::islamic_date_type:
                    updateIslamicDate(dl);
                    break;
                case cal::simulation_steps_type:
                    updateSimulationSteps(dl);
                    break;
                }
            }
        }

        Koyomi() {
            // 暦を読み込み
            paxs::JapaneseEra::inputList(japanese_era_list, AppConfig::getInstance()->getRootPath() + "Data/Calendars/JapaneseEraName.tsv");
            paxs::ChineseEra::inputList(chinese_era_list, AppConfig::getInstance()->getRootPath() + "Data/Calendars/ChineseEraName.tsv");
            // 日付計算
            calcDate();
        }

        void update() {
            static int calendar_update_counter = 0; // 暦を繰り上げるタイミングを決めるためのカウンタ
            ++calendar_update_counter;
            if (calendar_update_counter >= calendar_update_threshold) { // カウンタが指定した値を超えたら日付を変える処理を実行
                calendar_update_counter = 0;
                // 時間を進めている場合（逆行していない場合）
                if (move_forward_in_time) {
#ifdef PAXS_USING_SIMULATOR
                    // エージェント機能（シミュレーション実行時）
                    if (is_agent_update) {
                        // シミュレーション用の時間進行（AppStateManagerが制御）
                        jdn += (days_per_year / static_cast<double>(SimulationConstants::getInstance()->steps_per_year));
                        calcDate(); // 日付計算
                        steps.getDay()++; // ステップ数を増やす
                    }
                    else
#endif
                    // 通常の時間進行（シミュレーション再生中は実行しない）
                    if (jdn.cgetDay() != (std::numeric_limits<int>::max)()) {
                        jdn += (days_per_year / time_scale_factor / 2.0); // ユリウス日を繰り上げ（次の日にする）
                        calcDate(); // 日付計算
                    }
                }
                // 時間を逆行している場合（シミュレーション再生中は逆再生しない）
                else if (go_back_in_time
#ifdef PAXS_USING_SIMULATOR
                    && !is_agent_update
#endif
                ) {
                    if (jdn.cgetDay() != (std::numeric_limits<int>::max)()) {
                        jdn -= (days_per_year / time_scale_factor); // ユリウス日を繰り下げ（前の日にする）
                        calcDate(); // 日付計算
                    }
                }
            }
        }

    };

}

#endif // !PAX_SAPIENTICA_CALENDAR_KOYOMI_HPP
