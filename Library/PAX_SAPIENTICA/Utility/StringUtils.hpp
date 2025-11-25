/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_UTILITY_STRING_UTILS_HPP
#define PAX_SAPIENTICA_UTILITY_STRING_UTILS_HPP

#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief String utility functions
    /// @brief 文字列ユーティリティ関数
    struct StringUtils {

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する
        static std::vector<std::string> split(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::vector<std::string> result;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                result.emplace_back(field);
            }
            return result;
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する
        template<typename Func_>
        static void split(const std::string& input, const char delimiter, Func_&& func_) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::size_t count = 0;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                if (field.size() == 0) {
                    ++count;
                    continue;
                }
                func_(field, count);
                ++count;
            }
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ double 版）
        static std::vector<double> splitStod(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::vector<double> result;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                if (field.size() == 0) result.emplace_back(std::numeric_limits<double>::quiet_NaN()); // 文字列が空の時は NaN を入れる
                else {
                    try {
                        result.emplace_back(std::stod(field)); // 文字列を数値に変換する
                    }
                    catch (const std::invalid_argument&/*ia*/) {
                        PAXS_WARNING("field is not convertible to double");
                        result.emplace_back(std::numeric_limits<double>::quiet_NaN());
                    }
                    catch (const std::out_of_range&/*oor*/) {
                        PAXS_WARNING("field is out of range for a double");
                        result.emplace_back(std::numeric_limits<double>::quiet_NaN());
                    }
                }
            }
            return result;
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ double 版）
        static void splitStod(const std::string& input, const char delimiter, double* const result, const std::size_t size) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::size_t counter = 0u;
            while (std::getline(stream, field, delimiter) && counter < size) { // 1 行ごとに文字列を分割
                if (field.size() == 0) result[counter] = (std::numeric_limits<double>::quiet_NaN()); // 文字列が空の時は NaN を入れる
                else result[counter] = (std::stod(field)); // 文字列を数値に変換する
                ++counter;
            }
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ UnorderedMap 版）
        static paxs::UnorderedMap<std::string, std::size_t> splitHashMap(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field;
            paxs::UnorderedMap<std::string, std::size_t> result;
            std::size_t index = 0;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                result.emplace(field, index);
                ++index;
            }
            return result;
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ UnorderedMap 版）
        static paxs::UnorderedMap<std::uint_least32_t, std::size_t> splitHashMapMurMur3(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field{};
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> result;
            std::size_t index = 0;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                result.emplace(MurMur3::calcHash(field.size(), field.c_str()), index);
                ++index;
            }
            return result;
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ std::vector 版）
        static std::vector<std::uint_least32_t> splitHashMapMurMur3Vector(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field{};
            std::vector<std::uint_least32_t> result;
            //std::size_t index = 0; // 未使用
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                result.emplace_back(MurMur3::calcHash(field.size(), field.c_str()));
                //++index;
            }
            return result;
        }

        /// @brief Replace string
        /// @brief 文字列を置換する
        static void replace(std::string& str, const std::string& from, const std::string& to) noexcept {
            if (from.empty()) return;
            const std::size_t from_len = from.length();
            const std::size_t to_len = to.length();
            std::size_t pos = 0;
            while ((pos = str.find(from, pos)) != std::string::npos) {
                str.replace(pos, from_len, to);
                pos += to_len;
            }
        }

        /// @brief Replace string list
        /// @brief 文字列リストを置換する
        static void replaceList(std::string& str, const std::vector<std::string>& from, const std::vector<std::string>& to) noexcept {
            for (std::size_t i = 0; i < from.size() && i < to.size(); ++i) {
                replace(str, from[i], to[i]);
            }
        }

        /// @brief Remove relative path patterns from path string (in-place version)
        /// @brief パス文字列から相対パスパターン（./../../等）を削除する（インプレース版）
        /// @param path パス文字列（参照）
        static void removeRelativePathPrefixInPlace(std::string& path) noexcept {
            std::size_t pos = 0;

            // パターン: "./" または "../" または "../../" 等の繰り返し
            while (pos < path.length()) {
                // "./" を探す
                if (path.substr(pos, 2) == "./") {
                    // "./" を削除
                    path.erase(pos, 2);
                    continue;
                }
                // "../" を探す
                else if (path.substr(pos, 3) == "../") {
                    // "../" を削除
                    path.erase(pos, 3);
                    continue;
                }
                // それ以外は次の文字へ
                else {
                    break;
                }
            }
        }

        /// @brief Convert string to double
        /// @brief 文字列を double に変換する
        static std::variant<double, std::string> tryToConvertStringToDouble(const std::string& str) noexcept {
            try {
                return std::stod(str);
            }
            catch (const std::invalid_argument&/*ia*/) {
                // str is not convertible to double
                return str;
            }
            catch (const std::out_of_range&/*oor*/) {
                // str is out of range for a double
                return str;
            }
        }

        /// @brief Convert string to int
        /// @brief 文字列を int に変換する
        static std::variant<int, std::string> tryToConvertStringToInt(const std::string& str) noexcept {
            try {
                return std::stoi(str);
            }
            catch (const std::invalid_argument&/*ia*/) {
                // str is not convertible to int
                return str;
            }
            catch (const std::out_of_range&/*oor*/) {
                // str is out of range for a int
                return str;
            }
        }

        /// @brief Safely convert string to double (returns std::optional)
        /// @brief 文字列を安全に double に変換する（std::optional版）
        /// @param str 変換する文字列
        /// @return 変換成功時は値、空文字列または失敗時は std::nullopt
        /// @note 空文字列は std::nullopt を返す（0.0 とは区別される）
        static std::optional<double> toDouble(const std::string& str) noexcept {
            if (str.empty()) {
                return std::nullopt;
            }
            try {
                return std::stod(str);
            }
            catch (const std::invalid_argument&) {
                return std::nullopt;
            }
            catch (const std::out_of_range&) {
                return std::nullopt;
            }
        }

        /// @brief Safely convert string to double with default value
        /// @brief 文字列を安全に double に変換する（デフォルト値付き）
        /// @param str 変換する文字列
        /// @param default_value 変換失敗時のデフォルト値
        /// @param warn_on_error エラー時に警告を出力するか（デフォルト: true）
        /// @return 変換結果またはデフォルト値
        /// @note 空文字列の場合もデフォルト値を返す
        static double safeStod(const std::string& str, double default_value, bool warn_on_error = true) noexcept {
            if (str.empty()) {
                return default_value;
            }
            try {
                return std::stod(str);
            }
            catch (const std::invalid_argument&) {
                if (warn_on_error) {
                    PAXS_WARNING("Invalid numeric value: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
            catch (const std::out_of_range&) {
                if (warn_on_error) {
                    PAXS_WARNING("Numeric value out of range: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
        }

        /// @brief Safely convert string to int (returns std::optional)
        /// @brief 文字列を安全に int に変換する（std::optional版）
        /// @param str 変換する文字列
        /// @return 変換成功時は値、空文字列または失敗時は std::nullopt
        /// @note 空文字列は std::nullopt を返す（0 とは区別される）
        static std::optional<int> toInt(const std::string& str) noexcept {
            if (str.empty()) {
                return std::nullopt;
            }
            try {
                return std::stoi(str);
            }
            catch (const std::invalid_argument&) {
                return std::nullopt;
            }
            catch (const std::out_of_range&) {
                return std::nullopt;
            }
        }

        /// @brief Safely convert string to int with default value
        /// @brief 文字列を安全に int に変換する（デフォルト値付き）
        /// @param str 変換する文字列
        /// @param default_value 変換失敗時のデフォルト値
        /// @param warn_on_error エラー時に警告を出力するか（デフォルト: true）
        /// @return 変換結果またはデフォルト値
        /// @note 空文字列の場合もデフォルト値を返す
        static int safeStoi(const std::string& str, int default_value, bool warn_on_error = true) noexcept {
            if (str.empty()) {
                return default_value;
            }
            try {
                return std::stoi(str);
            }
            catch (const std::invalid_argument&) {
                if (warn_on_error) {
                    PAXS_WARNING("Invalid integer value: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
            catch (const std::out_of_range&) {
                if (warn_on_error) {
                    PAXS_WARNING("Integer value out of range: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
        }

        /// @brief Safely convert string to long (returns std::optional)
        /// @brief 文字列を安全に long に変換する（std::optional版）
        /// @param str 変換する文字列
        /// @return 変換成功時は値、空文字列または失敗時は std::nullopt
        static std::optional<long> toLong(const std::string& str) noexcept {
            if (str.empty()) {
                return std::nullopt;
            }
            try {
                return std::stol(str);
            }
            catch (const std::invalid_argument&) {
                return std::nullopt;
            }
            catch (const std::out_of_range&) {
                return std::nullopt;
            }
        }

        /// @brief Safely convert string to long with default value
        /// @brief 文字列を安全に long に変換する（デフォルト値付き）
        /// @param str 変換する文字列
        /// @param default_value 変換失敗時のデフォルト値
        /// @param warn_on_error エラー時に警告を出力するか（デフォルト: true）
        /// @return 変換結果またはデフォルト値
        /// @note 空文字列の場合もデフォルト値を返す
        static long safeStol(const std::string& str, long default_value, bool warn_on_error = true) noexcept {
            if (str.empty()) {
                return default_value;
            }
            try {
                return std::stol(str);
            }
            catch (const std::invalid_argument&) {
                if (warn_on_error) {
                    PAXS_WARNING("Invalid long value: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
            catch (const std::out_of_range&) {
                if (warn_on_error) {
                    PAXS_WARNING("Long value out of range: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
        }

        /// @brief Safely convert string to float (returns std::optional)
        /// @brief 文字列を安全に float に変換する（std::optional版）
        /// @param str 変換する文字列
        /// @return 変換成功時は値、空文字列または失敗時は std::nullopt
        static std::optional<float> toFloat(const std::string& str) noexcept {
            if (str.empty()) {
                return std::nullopt;
            }
            try {
                return std::stof(str);
            }
            catch (const std::invalid_argument&) {
                return std::nullopt;
            }
            catch (const std::out_of_range&) {
                return std::nullopt;
            }
        }

        /// @brief Safely convert string to float with default value
        /// @brief 文字列を安全に float に変換する（デフォルト値付き）
        /// @param str 変換する文字列
        /// @param default_value 変換失敗時のデフォルト値
        /// @param warn_on_error エラー時に警告を出力するか（デフォルト: true）
        /// @return 変換結果またはデフォルト値
        /// @note 空文字列の場合もデフォルト値を返す
        static float safeStof(const std::string& str, float default_value, bool warn_on_error = true) noexcept {
            if (str.empty()) {
                return default_value;
            }
            try {
                return std::stof(str);
            }
            catch (const std::invalid_argument&) {
                if (warn_on_error) {
                    PAXS_WARNING("Invalid float value: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
            catch (const std::out_of_range&) {
                if (warn_on_error) {
                    PAXS_WARNING("Float value out of range: \"" + str + "\", using default: " + std::to_string(default_value));
                }
                return default_value;
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_UTILITY_STRING_UTILS_HPP
