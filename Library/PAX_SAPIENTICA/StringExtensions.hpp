/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_STRING_EXTENSIONS_HPP
#define PAX_SAPIENTICA_STRING_EXTENSIONS_HPP

/*##########################################################################################

##########################################################################################*/

#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/ConvertToInt.hpp>

namespace paxs {

    /// @brief Extensions for std::string
    /// @brief std::string の拡張
    struct StringExtensions {

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
        /// @brief デリミタで文字列を分割する（ double 版）
        static std::vector<double> splitStod(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::vector<double> result;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                if (field.size() == 0) result.emplace_back(std::numeric_limits<double>::quiet_NaN()); // 文字列が空の時は NaN を入れる
                else result.emplace_back(std::stod(field)); // 文字列を数値に変換する
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
        /// @brief デリミタで文字列を分割する（ SlopeDegF64ToU0To250 版）
        static void splitSlopeDegU8(const std::string& input, const char delimiter, unsigned char* const result, const std::size_t size) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::size_t counter = 0u;
            while (std::getline(stream, field, delimiter) && counter < size) { // 1 行ごとに文字列を分割
                if (field.size() == 0) result[counter] = 251; // 文字列が空の時は NaN(251) を入れる
                else {
                    result[counter] = paxs::slopeDegF64ToLog2U8(std::stod(field)); // 文字列を数値に変換する

                    /* バイナリデータをより小さくするための実験 */
                    //if (result[counter] > 181) result[counter] = 181;
                    //else if (result[counter] > 162) result[counter] = 162;
                    //else if (result[counter] > 127) result[counter] = 127;
                    //else if (result[counter] > 0) result[counter] = 1;

                    //if ((result[counter] & 1) == 1) ++result[counter]; // 実験：奇数を削除
                    //const unsigned int b = 16;
                    //result[counter] = unsigned char(b * ((result[counter] + (b - 1)) / b)); // 倍数を削除
                    //if (result[counter] > 250) result[counter] = 250;
                }

                ++counter;
            }
            if (field.size() == 0 && counter < size) { // 最後に空文字がある場合の処理
                result[counter] = 251; // 文字列が空の時は NaN(251) を入れる
            }
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ ElevationDegF64ToU0To250 版）
        static void splitElevationS16(const std::string& input, const char delimiter, std::int_least16_t* const result, const std::size_t size) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::size_t counter = 0u;
            while (std::getline(stream, field, delimiter) && counter < size) { // 1 行ごとに文字列を分割
                if (field.size() == 0) result[counter] = 32761; // 文字列が空の時は NaN(32761) を入れる
                else {
                    result[counter] = paxs::elevationF64ToLog2S16(std::stod(field)); // 文字列を数値に変換する
                }

                ++counter;
            }
            if (field.size() == 0 && counter < size) { // 最後に空文字がある場合の処理
                result[counter] = 32761; // 文字列が空の時は NaN(32761) を入れる
            }
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ std::unordered_map 版）
        static std::unordered_map<std::string, std::size_t> splitHashMap(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field;
            std::unordered_map<std::string, std::size_t> result;
            std::size_t index = 0;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                result.emplace(field, index);
                ++index;
            }
            return result;
        }

        /// @brief Split string by delimiter
        /// @brief デリミタで文字列を分割する（ std::unordered_map 版）
        static std::unordered_map<std::uint_least32_t, std::size_t> splitHashMapMurMur3(const std::string& input, const char delimiter) noexcept {
            std::istringstream stream(input);
            std::string field{};
            std::unordered_map<std::uint_least32_t, std::size_t> result;
            std::size_t index = 0;
            while (std::getline(stream, field, delimiter)) { // 1 行ごとに文字列を分割
                result.emplace(MurMur3::calcHash(field.size(), field.c_str()), index);
                ++index;
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
    };
}

#endif // !PAX_SAPIENTICA_STRING_EXTENSIONS_HPP
