/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
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

#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

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
        constexpr std::uint_least32_t cgetKey() const { return select_key; }
    };
    class Language {
    private:

    private:
        std::vector<std::uint_least32_t> ordered_languages{}; // 登録順の言語
        std::unordered_map<std::uint_least32_t, std::size_t> unordered_languages{}; // 非順序の言語
        std::unordered_map<std::uint_least64_t, std::string> texts_and_languages{}; // テキストを辞書で管理
        std::unordered_map<std::uint_least32_t, std::uint_least64_t> text_key{}; // テキストを辞書で管理 (１つの言語のみ)
    public:
        // key が存在しているか
        bool isIndex(const std::uint_least64_t key) const {
            return  (texts_and_languages.find(key) != texts_and_languages.end());
        }
        // key が存在しているか
        bool isIndex32(const std::uint_least32_t key) const {
            return  (text_key.find(key) != text_key.end());
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
                if (!isIndex32(key_)) { // 入力の Key が 1 つも登録されていない場合
                    return nullptr; // 単語名がない場合は描画しない
                }
                if (!isIndex(text_key.at(key_))) { // 入力の Key が 1 つも登録されていない場合
                    return nullptr; // 単語名がない場合は描画しない
                }
                return &(texts_and_languages.at(text_key.at(key_)));
            }
            return &(texts_and_languages.at(key_language));
        }

    private:

        // 項目の ID を返す
        std::size_t getMenuIndex(const std::unordered_map<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) const {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }

    public:
        // 新しいテキストの追加
        void add(const std::string& str_) {
            paxs::InputFile pifs(str_);
            if (pifs.fail()) return;
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                return; // 何もない場合
            }
            // BOM を削除
            pifs.deleteBOM();
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

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> sub_menu_v = pifs.split('\t');
                if (key >= sub_menu_v.size()) continue; // Key がない場合は処理しない
                for (std::size_t i = 0; i < sub_menu_v.size() && i < menu_v.size(); ++i) {
                    if (i == key) continue; // key と key の場合は無視
                    if (sub_menu_v[i].size() == 0) continue; // 文字がない場合は処理しない

                    const std::uint_least64_t hash64 = (static_cast<std::uint_least64_t>(MurMur3::calcHash(sub_menu_v[key].size(), sub_menu_v[key].c_str())) << 32)
                        + static_cast<std::uint_least64_t>(menu_v[i]); // key と language
                    if (!isIndex(hash64)) {
                        texts_and_languages.emplace(hash64, sub_menu_v[i]);
                    }
                    const std::uint_least32_t hash32 = MurMur3::calcHash(sub_menu_v[key].size(), sub_menu_v[key].c_str());
                    if (!isIndex32(hash32)) {
                        text_key.emplace(hash32, hash64);
                    }
                }
            }
        }
        Language() = default;
    };

}

#endif // !PAX_SAPIENTICA_SIV3D_LANGUAGE_HPP
