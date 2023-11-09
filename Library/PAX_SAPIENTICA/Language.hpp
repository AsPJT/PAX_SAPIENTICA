/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP
#define PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstddef>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <PAX_SAPIENTICA/MurMur3.hpp>

#include <PAX_GRAPHICA/InputFile.hpp>

namespace paxs {

    // 選択言語
    class SelectLanguage {
    private:
        std::size_t select_language = 0; // 選択している言語
        std::uint_least32_t select_key = 0; // 選択している言語
    public:
        constexpr void set(const std::size_t select_language_) { select_language = select_language_; }
        constexpr void setKey(const std::uint_least32_t select_key_) { select_key = select_key_; }
        constexpr std::size_t cget() const { return select_language; }
        constexpr std::size_t cgetKey() const { return select_key; }
    };
    class Language {
    private:

    private:
        std::string null_str{};
        std::vector<std::uint_least32_t> ordered_languages{}; // 登録順の言語
        std::unordered_map<std::uint_least32_t, std::size_t> unordered_languages{}; // 非順序の言語
        std::unordered_map<std::uint_least64_t, std::string> texts_and_languages{}; // テキストを辞書で管理
    public:
        // key が存在しているか
        std::size_t isIndex(const std::uint_least64_t key) const {
            return  (texts_and_languages.find(key) != texts_and_languages.end());
        }

        // 64bit Key を返す
        std::uint_least64_t getKey(const std::uint_least32_t key_, const std::uint_least32_t language_) const {
            return (static_cast<std::uint_least64_t>(key_) << 32)
                + static_cast<std::uint_least64_t>(language_);
        }
        const std::string* getStringPtr(const std::uint_least32_t key_, const std::uint_least32_t language_) const {
            const std::uint_least64_t key_language = getKey(key_, language_);

            // 指定した Key が存在しない場合
            if (key_language == 0 || !isIndex(key_language)) {
                return &null_str; // 単語名がない場合は描画しない
            }
            return &(texts_and_languages.at(key_language));
        }

    private:
        using Languages = std::vector<std::string>; // 各国の言語の読み方を保持

        std::vector<Languages> text{}; // テキストを二次元配列で管理
        std::unordered_map<std::uint_least32_t, std::size_t> text_map{}; // テキストを辞書で管理
        Languages empty{}; // テキストが見つからなかった時に返す変数

        // 文字を指定した区切り文字で分割し、テキストの配列や辞書に格納
        void split(const std::string& input, const char delimiter) {
            std::istringstream stream(input);
            std::string field;
            Languages result;
            while (std::getline(stream, field, delimiter)) {
                result.emplace_back(field);
            }
            text_map.emplace(MurMur3::calcHash(result.front().size(), result.front().c_str()), text.size());
            text.emplace_back(result);
        }

        // 項目の ID を返す
        std::size_t getMenuIndex(const std::unordered_map<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) const {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }

    public:
        // テキストの二次元配列を返す
        constexpr std::vector<Languages>& get() {
            return text;
        }
        constexpr const std::vector<Languages>& cget() const {
            return text;
        }
        // 新しいテキストの追加
        void add(const std::string& str_) {
            paxg::InputFile pifs(str_);
            if (pifs.fail()) return;
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                return; // 何もない場合
            }
            // 1 行目を分割する
            std::unordered_map<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');
            std::vector<std::uint_least32_t> menu_v = pifs.splitHashMapMurMur3Vector('\t');

            const std::size_t key = getMenuIndex(menu, MurMur3::calcHash("key"));
            if (key == SIZE_MAX) return; // Key がないのはデータにならない

            // Key が登録されていない場合は登録する
            if (unordered_languages.find(MurMur3::calcHash("key")) == unordered_languages.end()) {
                unordered_languages.emplace(MurMur3::calcHash("key"), SIZE_MAX);
            }
            for (const std::uint_least32_t v : menu_v) {
                // 言語が登録されていない場合は登録する
                if (unordered_languages.find(v) == unordered_languages.end()) {
                    unordered_languages.emplace(v, ordered_languages.size());
                    ordered_languages.emplace_back(v);
                }
            }
            // unordered_languages

            split(pifs.lineString(), '\t'); // 旧仕様の分割方法

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                split(pifs.lineString(), '\t');

                std::vector<std::string> sub_menu_v = pifs.split('\t');
                if (key >= sub_menu_v.size()) continue; // Key がない場合は処理しない
                for (std::size_t i = 0; i < sub_menu_v.size() && i < menu_v.size(); ++i) {
                    if (i == key) continue; // key と key の場合は無視
                    texts_and_languages.emplace(
                        (static_cast<std::uint_least64_t>(MurMur3::calcHash(sub_menu_v[key].size(), sub_menu_v[key].c_str())) << 32)
                        + static_cast<std::uint_least64_t>(menu_v[i]), // key と language
                        sub_menu_v[i] // 単語
                    );
                }
            }
            if (text.size() == 0) {
                empty.resize(0);
            }
            else {
                empty.resize(text.front().size());
            }
        }
        Language(const std::string& str_) {
            add(str_);
        }
        // 始点を探す
        std::size_t findStart(const std::uint_least32_t& str_) const {
            if (text_map.find(str_) != text_map.end()) {
                return text_map.at(str_);
            }
            return 0;
        }
        // 見つけたい単語を探す
        Languages& getFindStart(const std::uint_least32_t& str_) {
            const std::size_t index = findStart(str_);
            if (index < text.size()) {
                return text[index];
            }
            return empty;
        }
        // 見つけたい単語を探す
        const Languages& cgetFindStart(const std::uint_least32_t& str_) const {
            const std::size_t index = findStart(str_);
            if (index < text.size()) {
                return text[index];
            }
            return empty;
        }
        // 見つけたい単語を探し、テキストの二次元配列（単語一覧）で返す
        std::vector<Languages> getFindStartToVVS(const std::uint_least32_t& str_, const std::size_t start_index) const {
            std::vector<Languages> tmp{};
            const Languages& lt = cgetFindStart(str_);
            for (std::size_t i = start_index; i < lt.size(); ++i) {
                tmp.emplace_back(Languages{ lt[i] });
            }
            return tmp;
        }
    };

}

#endif // !PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP
