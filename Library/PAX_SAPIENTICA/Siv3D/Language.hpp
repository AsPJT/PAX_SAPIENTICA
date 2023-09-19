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

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>

namespace paxs {

    // 選択言語
    class SelectLanguage {
    private:
        std::size_t select_language = 0; // 選択している言語
    public:
        constexpr void set(const std::size_t select_language_) { select_language = select_language_; }
        constexpr std::size_t& get() { return select_language; }
        constexpr std::size_t cget() const { return select_language; }
    };
    class Language {
    private:
        using Languages = std::vector<std::string>; // 各国の言語の読み方を保持

        std::vector<Languages> text{}; // テキストを二次元配列で管理
        std::unordered_map<std::string, std::size_t> text_map{}; // テキストを辞書で管理
        Languages empty{}; // テキストが見つからなかった時に返す変数

        // 文字を指定した区切り文字で分割し、テキストの配列や辞書に格納
        void split(const std::string& input, const char delimiter) {
            std::istringstream stream(input);
            std::string field;
            Languages result;
            while (std::getline(stream, field, delimiter)) {
                result.emplace_back(field);
            }
            text_map.emplace(result.front(), text.size());
            text.emplace_back(result);
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
            std::ifstream ifs(str_); // ファイルを読み込む
            std::string line;
            while (std::getline(ifs, line)) {
                split(line, '\t'); // 分割し、格納
            }
            empty.resize(text.front().size());
        }
        Language(const std::string& str_) {
            add(str_);
        }
        // 始点を探す
        std::size_t findStart(const std::string& str_) const {
            if (text_map.find(str_) != text_map.end()) {
                return text_map.at(str_);
            }
            return 0;
        }
        // 見つけたい単語を探す
        Languages& getFindStart(const std::string& str_) {
            const std::size_t index = findStart(str_);
            if (index < text.size()) {
                return text[index];
            }
            return empty;
        }
        // 見つけたい単語を探す
        const Languages& cgetFindStart(const std::string& str_) const {
            const std::size_t index = findStart(str_);
            if (index < text.size()) {
                return text[index];
            }
            return empty;
        }
        // 見つけたい単語を探し、テキストの二次元配列（単語一覧）で返す
        std::vector<Languages> getFindStartToVVS(const std::string& str_, const std::size_t start_index) const {
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
