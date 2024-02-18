/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_HPP
#define PAX_MAHOROBA_CALENDAR_HPP

/*##########################################################################################

##########################################################################################*/

#include <limits>
#include <string>
#include <variant>
#include <vector>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Calendars.hpp>
#include <PAX_SAPIENTICA/Calendar/ChineseEra.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/JapaneseEra.hpp>
#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法
#include <PAX_SAPIENTICA/Math.hpp> // 数学定数
#include <PAX_SAPIENTICA/MurMur3.hpp>
#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/TouchManager.hpp>

namespace paxs {

    class KoyomiSiv3D {
    public:
        /*##########################################################################################
            暦
        ##########################################################################################*/

        /// @brief 出力に必要な日付の情報
        struct OutputDate {
            std::uint_least32_t calendar_name_key; // 暦の名前
            cal::Calendars date{}; // 日付
        };

        cal::JDN_F64 jdn{}; // ユリウス通日（基準となる暦）
        cal::SimulationSteps steps{}; // シミュレーションのステップ数
        std::vector<OutputDate> date_list{}; // 表示する暦レイヤー

        std::vector<paxs::JapaneseEra> japanese_era_list; // 日本の元号の一覧
        std::vector<paxs::ChineseEra> chinese_era_list; // 中国大陸の元号の一覧

        /*##########################################################################################
            編年・時代区分（実験）
        ##########################################################################################*/

        std::uint_least32_t sueki_nakamura_key; // 須恵器の中村編年の配列のインデックス値を格納
        std::uint_least32_t sueki_tanabe_key; // 須恵器の田辺編年の配列のインデックス値を格納

        // 時代区分の文字列
        std::vector<std::string> options = {
            "旧石器時代", "縄文時代", "弥生時代", "古墳時代 CE251-",
            "飛鳥時代 CE592-", "奈良時代 CE710-", "平安時代 CE794-",
            "鎌倉時代", "室町時代", "安土桃山時代", "江戸時代", "明治時代" };
        // 時代区分ごとのユリウス日の数値
        std::array<int, 12> period_jdn = {
            0,0,1538799,1812736,
            1937666,1980488,2011388,
            2154234,2209376,2295823,2306626,2403629
        };
        // 月の英語名
        std::string month_name_long[13] = { "","January","February","March","April","May","June",
            "July","August","September","October","November","December" };
        std::string month_name[13] = { "","Jan.","Feb.","Mar.","Apr.","May","Jun.",
            "Jul.","Aug.","Sep.","Oct.","Nov.","Dec." };


        bool move_forward_in_time = true; // デバッグ false; // 時間を進めるか
        bool go_back_in_time = false; // 時間を戻すか
        bool is_agent_update = false; // エージェントの更新をするか

    public:
        /*##########################################################################################
            日付データの更新
        ##########################################################################################*/
        void calcDate() {
            // 暦データを更新
            paxs::cal::JapanDate jp_date{};
            paxs::cal::ChinaDate cn_date{};
            std::string gengo{};
            for (auto& dl : date_list) {
                switch (dl.date.index()) {
                case cal::gregorian_date_type:
                    // グレゴリオ暦を格納
                    dl.date = jdn.toGregorianCalendar();
                    break;
                case cal::julian_date_type:
                    // ユリウス暦を格納
                    dl.date = jdn.toJulianCalendar();
                    break;
                case cal::japan_date_type:
                    // 和暦を格納
                    jp_date = jdn.toJapaneseCalendar(japanese_era_list);
                    // 元号を格納
                    gengo = std::string("gengo_" + std::to_string(jp_date.cgetGengo()));
                    dl.calendar_name_key = ((MurMur3::calcHash(gengo.size(), gengo.c_str())));
                    dl.date = jp_date;
                    break;
                case cal::china_date_type:
                    // 和暦を格納
                    cn_date = jdn.toChineseCalendar(chinese_era_list);
                    // 元号を格納
                    gengo = std::string("chinese_calendar_" + std::to_string(cn_date.cgetGengo()));
                    dl.calendar_name_key = ((MurMur3::calcHash(gengo.size(), gengo.c_str())));
                    dl.date = cn_date;
                    break;
                case cal::jdn_f64_type:
                case cal::jdn_s32_type:
                case cal::jdn_s64_type:
                    dl.date = jdn; // ユリウス通日を格納
                    break;
                case cal::calbp_type:
                    // BP （年代測定）を格納
                    dl.date = jdn.toCalBP();
                    break;
                case cal::islamic_date_type:
                    // ヒジュラ暦を格納
                    dl.date = jdn.toIslamicCalendar();
                    break;
                case cal::simulation_steps_type:
                    // シミュレーションのステップ数を格納
                    dl.date = steps;
                    break;
                }
            }
        }

        void init() {
            // 各暦の日付情報を初期化
            date_list = std::vector<OutputDate>{
                OutputDate{(MurMur3::calcHash("calendar_japan")),cal::JapanDate() },
                    OutputDate{(MurMur3::calcHash("calendar_gregorian")),cal::GregorianDate() },
                    OutputDate{(MurMur3::calcHash("calendar_julian")), cal::JulianDate() },
                    OutputDate{(MurMur3::calcHash("calendar_hijri")), cal::IslamicDate() },
                    OutputDate{(MurMur3::calcHash("calendar_chinese")), cal::ChinaDate() },
                    OutputDate{(MurMur3::calcHash("calendar_julian_day")), cal::JDN_S64() },
                    OutputDate{(MurMur3::calcHash("calendar_calbp")), cal::CalBP()},
                    OutputDate{(MurMur3::calcHash("menu_bar_view_simulation")), cal::SimulationSteps()}
            };
            // 須恵器編年の文字列を言語テキストファイルから探して格納
            sueki_nakamura_key = (MurMur3::calcHash("sueki_nakamura"));
            sueki_tanabe_key = (MurMur3::calcHash("sueki_tanabe"));

            // 暦を読み込み
            paxs::JapaneseEra::inputList(japanese_era_list, AppConfig::getInstance()->getRootPath() + "Data/Calendars/JapaneseEraName.tsv");
            paxs::ChineseEra::inputList(chinese_era_list, AppConfig::getInstance()->getRootPath() + "Data/Calendars/ChineseEraName.tsv");
            // 日付計算
            calcDate();
        }

        bool update(
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator<int>>& simulator // コンパイル時の分岐により使わない場合あり
#endif
        ) {
            bool return_bool = false;
            /*##########################################################################################
                暦関連
            ##########################################################################################*/

            static int calendar_update_counter = 0; // 暦を繰り上げるタイミングを決めるためのカウンタ
            ++calendar_update_counter;
            // if (move_forward_in_time) jdn.getDay() += 1.0; // デバッグ
            //else if(go_back_in_time) jdn -= 1000;
        //if (count >= 0) {
            if (calendar_update_counter >= /*30*/0) { // カウンタが指定した値を超えたら日付を変える処理を実行
                calendar_update_counter = 0;
                // 時間を進めている場合（逆行していない場合）
                if (move_forward_in_time) {
#ifdef PAXS_USING_SIMULATOR
                    // エージェント機能
                    if (is_agent_update && simulator.get() != nullptr) {
                        jdn += (365.2425 / static_cast<double>(SimulationConstants::getInstance()->steps_per_year));
                        calcDate(); // 日付計算
                        simulator->step(); // シミュレーションを 1 ステップ実行する
                        steps.getDay()++; // ステップ数を増やす
                        return_bool = true;
                    }
                    else
#endif
                    if (jdn.cgetDay() != (std::numeric_limits<int>::max)()) {
                        jdn += (365.2425 / 6.0);//(0.8 / 30.0); //(365.2425 / 12.0);//365.2425;//1.0;// ユリウス日を繰り上げ（次の日にする）
                        calcDate(); // 日付計算
                    }
                }
                // 時間を逆行している場合
                else if (go_back_in_time) {
                    if (jdn.cgetDay() != (std::numeric_limits<int>::max)()) {
                        jdn -= (365.2425 / 6.0);//(0.8 / 30.0); //(365.2425 / 12.0);//365.24252;//1.0;// ユリウス日を繰り上げ（次の日にする）
                        calcDate(); // 日付計算
                    }
                }
            }
            return return_bool;
        }

    };

}

#endif // !PAX_MAHOROBA_CALENDAR_HPP
