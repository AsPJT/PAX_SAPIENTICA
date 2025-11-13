/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHY_TERRAIN_SLOPE_HPP
#define PAX_SAPIENTICA_GEOGRAPHY_TERRAIN_SLOPE_HPP

#include <cmath>
#include <cstddef>

#include <array>
#include <fstream>
#include <limits>
#include <string>

#include <PAX_SAPIENTICA/System/InputFile.hpp>

namespace paxs {

    // 標高
    struct ElevationS16UnitOutput {

        std::ofstream ofs{};

        ElevationS16UnitOutput() = default;
        ElevationS16UnitOutput(const std::string& str_)
            : ofs(str_, std::ios::binary) {}

        std::size_t taple_count = 0; // 被りの個数
        std::size_t tmp_count = 0; // 被りの個数
        std::size_t tmp_count2 = 0; // 被りの個数

        char write_str[6]{};

        void calc(const std::int_least16_t first_s16_, const std::int_least16_t second_) {
            if (first_s16_ == second_) {
                ++taple_count; // 個数を増加
            }
            else {
                if (first_s16_ >= 32762) {
                    taple_count = 0;
                    return; // 値無しは除外
                }

                if (taple_count >= 256) {
                    // 値無しの場合
                    if (first_s16_ == 32761) {
                        write_str[0] = static_cast<char>(static_cast<unsigned char>(32764u >> 8)); // 2 バイト
                        write_str[1] = static_cast<char>(static_cast<unsigned char>(32764u & 255u)); // 2 バイト
                        write_str[2] = static_cast<char>(static_cast<unsigned char>(taple_count >> 8)); // 上位バイト
                        write_str[3] = static_cast<char>(static_cast<unsigned char>(taple_count & 255u)); // 下位バイト
                        ofs.write((char*)(&write_str), 4);
                        tmp_count += taple_count + 1;
                        tmp_count2 += 4;
                    }
                    else {
                        write_str[0] = static_cast<char>(static_cast<unsigned char>(32766u >> 8)); // 2 バイト
                        write_str[1] = static_cast<char>(static_cast<unsigned char>(32766u & 255u)); // 2 バイト
                        write_str[2] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) >> 8)); // 上位の値
                        write_str[3] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) & 255u)); // 下位の値
                        write_str[4] = static_cast<char>(static_cast<unsigned char>(taple_count >> 8)); // 上位バイト
                        write_str[5] = static_cast<char>(static_cast<unsigned char>(taple_count & 255u)); // 下位バイト
                        ofs.write((char*)(&write_str), 6);
                        tmp_count += taple_count + 1;
                        tmp_count2 += 6;
                    }
                }
                else if (taple_count >= 3) { // 同じ数が 4 つ以上連続で並んでいる
                    // 値無しの場合
                    if (first_s16_ == 32761) {
                        write_str[0] = static_cast<char>(static_cast<unsigned char>(32765u >> 8)); // 2 バイト
                        write_str[1] = static_cast<char>(static_cast<unsigned char>(32765u & 255u)); // 2 バイト
                        write_str[2] = static_cast<char>(static_cast<unsigned char>(taple_count)); // バイト数
                        ofs.write((char*)(&write_str), 3);
                        tmp_count += taple_count + 1;
                        tmp_count2 += 3;
                    }
                    else {
                        write_str[0] = static_cast<char>(static_cast<unsigned char>(32767u >> 8)); // 2 バイト
                        write_str[1] = static_cast<char>(static_cast<unsigned char>(32767u & 255u)); // 2 バイト
                        write_str[2] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) >> 8)); // 上位の値
                        write_str[3] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) & 255u)); // 下位の値
                        write_str[4] = static_cast<char>(static_cast<unsigned char>(taple_count)); // バイト数
                        ofs.write((char*)(&write_str), 5);
                        tmp_count += taple_count + 1;
                        tmp_count2 += 5;
                    }
                }
                else if (taple_count == 2) { // 同じ数が 3 つ連続で並んでいる
                    write_str[0] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) >> 8)); // 上位の値
                    write_str[1] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) & 255u)); // 下位の値
                    write_str[2] = write_str[0]; // 上位の値
                    write_str[3] = write_str[1]; // 下位の値
                    write_str[4] = write_str[0]; // 上位の値
                    write_str[5] = write_str[1]; // 下位の値
                    ofs.write((char*)(&write_str), 6);
                    tmp_count += 3;
                    tmp_count2 += 6;
                }
                else if (taple_count == 1) { // 同じ数が 2 つ連続で並んでいる
                    write_str[0] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) >> 8)); // 上位の値
                    write_str[1] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) & 255u)); // 下位の値
                    write_str[2] = write_str[0]; // 上位の値
                    write_str[3] = write_str[1]; // 下位の値
                    ofs.write((char*)(&write_str), 4);
                    tmp_count += 2;
                    tmp_count2 += 4;
                }
                else {
                    write_str[0] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) >> 8)); // 上位の値
                    write_str[1] = static_cast<char>(static_cast<unsigned char>(static_cast<std::uint_least16_t>(first_s16_) & 255u)); // 下位の値
                    ofs.write((char*)(&write_str), 2);
                    tmp_count += 1;
                    tmp_count2 += 2;
                }
                taple_count = 0;
            }
        }
    };

    // 全ての傾斜度を出力する
    struct ElevationS16AllOutput {

        paxs::InputFile ifs;

        paxs::ElevationS16UnitOutput sd;

        const char delimiter{};

        ElevationS16AllOutput(
            const std::string& ifs_str_,
            const std::string& ifs_str2_,
            const std::string& ofs_str_,
            const std::string& ofs_str2_,
            const char delimiter_
        ) :ifs(ifs_str_, ifs_str2_), delimiter(delimiter_) {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file: " + ifs_str_ + ifs_str2_);
                return; // 読み込み失敗
            }

            // 読み込み成功の場合は出力を生成する
            sd = paxs::ElevationS16UnitOutput(ofs_str2_ + ofs_str_);
        }

        void calc() {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file during calculation.");
                return; // 読み込み失敗
            }
            std::int_least16_t pre_value = 32762; // 1 つ前の値
            std::array<std::int_least16_t, 256> xyz_one_line{};
            while (ifs.getLine()) {
                ifs.splitElevationS16(delimiter, xyz_one_line.data(), 256);
                for (std::size_t i = 0; i < 256; ++i) {
                    sd.calc(pre_value, xyz_one_line[i]);
                    pre_value = xyz_one_line[i];
                }
            }
            sd.calc(pre_value, 32762);
        }

    };

    // 傾斜度
    struct SlopeDegU0To250UnitOutput {

        std::ofstream ofs{};

        SlopeDegU0To250UnitOutput() = default;
        SlopeDegU0To250UnitOutput(const std::string& str_)
            : ofs(str_, std::ios::binary) {}

        std::size_t taple_count = 0; // 被りの個数
        std::size_t tmp_count = 0; // 被りの個数
        std::size_t tmp_count2 = 0; // 被りの個数

        char write_str[4]{};

        void calc(const unsigned char first_, const unsigned char second_) {
            if (first_ == second_) {
                ++taple_count; // 個数を増加
            }
            else {
                if (first_ >= 252) {
                    taple_count = 0;
                    return; // 値無しは除外
                }
                if (taple_count >= 256) {
                    write_str[0] = static_cast<char>(static_cast<unsigned char>(0xfe)); // 254
                    write_str[1] = static_cast<char>(first_); // 値
                    write_str[2] = static_cast<char>(static_cast<unsigned char>(taple_count >> 8)); // 上位バイト
                    write_str[3] = static_cast<char>(static_cast<unsigned char>(taple_count & 255)); // 下位バイト
                    ofs.write((char*)(&write_str), 4);
                    //ofs << (int)first_ << "B(" << taple_count+1 << ")";
                    //ofs << 255 << "-" << (taple_count >> 8) << "-" << (taple_count & 255) << "-";
                    tmp_count += taple_count + 1;
                    tmp_count2 += 4;
                }
                else if (taple_count >= 3) { // 同じ数が 4 つ以上連続で並んでいる
                    write_str[0] = static_cast<char>(static_cast<unsigned char>(0xff)); // 255
                    write_str[1] = static_cast<char>(first_); // 値
                    write_str[2] = static_cast<char>(static_cast<unsigned char>(taple_count)); // バイト数
                    ofs.write((char*)(&write_str), 3);
                    //ofs << (int)first_ << "A(" << taple_count+1 << ")";
                    tmp_count += taple_count + 1;
                    tmp_count2 += 3;
                }
                else if (taple_count == 2) { // 同じ数が 3 つ連続で並んでいる
                    write_str[0] = static_cast<char>(first_); // 値
                    write_str[1] = static_cast<char>(first_); // 値
                    write_str[2] = static_cast<char>(first_); // 値
                    ofs.write((char*)(&write_str), 3);
                    //ofs << (int)first_ << "," << (int)first_ << ", ";
                    tmp_count += 3;
                    tmp_count2 += 3;
                }
                else if (taple_count == 1) { // 同じ数が 2 つ連続で並んでいる
                    write_str[0] = static_cast<char>(first_); // 値
                    write_str[1] = static_cast<char>(first_); // 値
                    ofs.write((char*)(&write_str), 2);
                    //ofs << (int)first_ << "," << (int)first_ << ", ";
                    tmp_count += 2;
                    tmp_count2 += 2;
                }
                else {
                    write_str[0] = static_cast<char>(first_); // 値
                    ofs.write((char*)(&write_str), 1);
                    //ofs << (int)first_ << ",";
                    tmp_count += 1;
                    tmp_count2 += 1;
                }
                taple_count = 0;
            }
        }
    };

    // 全ての傾斜度を出力する
    struct SlopeDegU0To250AllOutput {

        paxs::InputFile ifs;

        paxs::SlopeDegU0To250UnitOutput sd;

        const char delimiter{};

        SlopeDegU0To250AllOutput(
            const std::string& ifs_str_,
            const std::string& ifs_str2_,
            const std::string& ofs_str_,
            const std::string& ofs_str2_,
            const char delimiter_
        ) :ifs(ifs_str_, ifs_str2_), delimiter(delimiter_) {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file: " + ifs_str_ + ifs_str2_);
                return; // 読み込み失敗
            }

            // 読み込み成功の場合は出力を生成する
            sd = paxs::SlopeDegU0To250UnitOutput(ofs_str2_ + ofs_str_);
        }

        void calc() {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file during calculation.");
                return; // 読み込み失敗
            }
            unsigned char pre_value = 252; // 1 つ前の値
            std::array<unsigned char, 256> xyz_one_line{};
            while (ifs.getLine()) {
                ifs.splitSlopeDegU8(delimiter, xyz_one_line.data(), 256);
                for (std::size_t i = 0; i < 256; ++i) {
                    sd.calc(pre_value, xyz_one_line[i]);
                    pre_value = xyz_one_line[i];
                }
            }
            sd.calc(pre_value, 252);
        }

    };

    // 16bit バイナリデータを読み込む
    struct Input16BitBinary {

        paxs::InputFile ifs;

        Input16BitBinary(
            const std::string& ifs_str_
        ) :ifs(ifs_str_, "", paxs::MurMur3::calcHash("asset_file"), paxs::MurMur3::calcHash("binary")) {}

        bool calc(std::int_least16_t* tiles_) {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file for 16-bit binary reading: " + ifs.getFilePath());
                return false; // 読み込み失敗
            }
            char xyz_tiles[256 * 256 * 2/*2byte*/]{};
            const std::size_t byte_num = ifs.splitBinary(xyz_tiles, 256 * 256 * 2); // バイト数
            if (byte_num <= 1) return false; // 1 文字になることはない

            for (std::size_t i = 0, j = 0;
                i < byte_num // 入力された文字数
                && j < 256 * 256 // XYZ タイルの大きさ
                ;) {

                std::int_least16_t first_value = static_cast<std::int_least16_t>(
                    (static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i])) << 8) // 値
                    + static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i + 1]))); // 値

                // 値だったら代入
                if (first_value <= 32763) {
                    tiles_[j] = first_value;
                    i += 2;
                    ++j;
                }
                // command の場合
                else if (first_value == 32765 || first_value == 32764) {
                    const std::size_t len =
                        (first_value == 32765) ?
                        static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) :

                        ((static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) << 8) // 上位バイト
                            + static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 3]))); // 下位バイト

                    for (std::size_t k = 0; k < len + 1 && j < 256 * 256; ++j, ++k) {
                        tiles_[j] = 32761; // 固定値
                    }
                    i += ((first_value == 32765) ? 3 : 4);
                }
                // command の場合
                else {
                    std::int_least16_t second_value = static_cast<std::int_least16_t>(
                        (static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) << 8) // 値
                        + static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i + 3]))); // 値
                    const std::size_t len =
                        (first_value == 32767) ?
                        static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 4])) :

                        ((static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 4])) << 8) // 上位バイト
                            + static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 5]))); // 下位バイト

                    for (std::size_t k = 0; k < len + 1 && j < 256 * 256; ++j, ++k) {
                        tiles_[j] = second_value; // 値
                    }
                    i += ((first_value == 32767) ? 5 : 6);
                }
            }
            return true;
        }

        template<typename Func_>
        bool calc(Func_&& func_) {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file for 16-bit binary reading: " + ifs.getFilePath());
                return false; // 読み込み失敗
            }
            char xyz_tiles[256 * 256 * 2/*2byte*/]{};
            const std::size_t byte_num = ifs.splitBinary(xyz_tiles, 256 * 256 * 2); // バイト数
            if (byte_num <= 1) {
                PAXS_WARNING("Input file for 16-bit binary reading is too short: " + ifs.getFilePath());
                return false; // 1 文字になることはない
            }

            for (std::size_t i = 0, j = 0;
                i < byte_num // 入力された文字数
                && j < 256 * 256 // XYZ タイルの大きさ
                ;) {

                std::int_least16_t first_value = static_cast<std::int_least16_t>(
                    (static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i])) << 8) // 値
                    + static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i + 1]))); // 値

                // 値だったら代入
                if (first_value <= 32763) {
                    func_(first_value, static_cast<std::uint_least8_t>(j & 0xff), static_cast<std::uint_least8_t>(j >> 8));
                    i += 2;
                    ++j;
                }
                // command の場合
                else if (first_value == 32765 || first_value == 32764) {
                    const std::size_t len =
                        (first_value == 32765) ?
                        static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) :

                        ((static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) << 8) // 上位バイト
                            + static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 3]))); // 下位バイト

                    for (std::size_t k = 0; k < len + 1 && j < 256 * 256; ++j, ++k) {
                        func_(32761, static_cast<std::uint_least8_t>(j & 0xff), static_cast<std::uint_least8_t>(j >> 8)); // 固定値
                    }
                    i += ((first_value == 32765) ? 3 : 4);
                }
                // command の場合
                else {
                    std::int_least16_t second_value = static_cast<std::int_least16_t>(
                        (static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) << 8) // 値
                        + static_cast<std::uint_least16_t>(static_cast<unsigned char>(xyz_tiles[i + 3]))); // 値
                    const std::size_t len =
                        (first_value == 32767) ?
                        static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 4])) :

                        ((static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 4])) << 8) // 上位バイト
                            + static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 5]))); // 下位バイト

                    for (std::size_t k = 0; k < len + 1 && j < 256 * 256; ++j, ++k) {
                        func_(second_value, static_cast<std::uint_least8_t>(j & 0xff), static_cast<std::uint_least8_t>(j >> 8)); // 値
                    }
                    i += ((first_value == 32767) ? 5 : 6);
                }
            }
            return true;
        }

    };

    // 8bit バイナリデータを読み込む
    struct Input8BitBinary {

        paxs::InputFile ifs;

        Input8BitBinary(
            const std::string& ifs_str_
        ) :ifs(ifs_str_, "", paxs::MurMur3::calcHash("asset_file"), paxs::MurMur3::calcHash("binary")) {}

        bool calc(unsigned char* tiles_) {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file for 8-bit binary reading: " + ifs.getFilePath());
                return false; // 読み込み失敗
            }
            char xyz_tiles[256 * 256]{};
            const std::size_t byte_num = ifs.splitBinary(xyz_tiles, 256 * 256); // バイト数
            for (std::size_t i = 0, j = 0;
                i < byte_num // 入力された文字数
                && j < 256 * 256 // XYZ タイルの大きさ
                ;) {

                unsigned char first_value = static_cast<unsigned char>(xyz_tiles[i]); // 値

                // 値だったら代入
                if (first_value <= 253) {
                    tiles_[j] = first_value;
                    ++i;
                    ++j;
                }
                // command の場合
                else {
                    unsigned char second_value = static_cast<unsigned char>(xyz_tiles[i + 1]); // 値
                    const std::size_t len =
                        (first_value == 255) ?
                        static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) :

                        ((static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) << 8) // 上位バイト
                            + static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 3]))); // 下位バイト

                    for (std::size_t k = 0; k < len + 1 && j < 256 * 256; ++j, ++k) {
                        tiles_[j] = second_value; // 値
                    }
                    i += ((first_value == 255) ? 3 : 4);
                }
            }
            return true;
        }

        template<typename Func_>
        bool calc(Func_&& func_) {
            if (ifs.fail()) {
                PAXS_WARNING("Failed to open input file for 8-bit binary reading: " + ifs.getFilePath());
                return false; // 読み込み失敗
            }
            char xyz_tiles[256 * 256]{};
            const std::size_t byte_num = ifs.splitBinary(xyz_tiles, 256 * 256); // バイト数
            for (std::size_t i = 0, j = 0;
                i < byte_num // 入力された文字数
                && j < 256 * 256 // XYZ タイルの大きさ
                ;) {

                unsigned char first_value = static_cast<unsigned char>(xyz_tiles[i]); // 値

                // 値だったら代入
                if (first_value <= 253) {
                    func_(first_value, static_cast<std::uint_least8_t>(j & 0xff), static_cast<std::uint_least8_t>(j >> 8));
                    ++i;
                    ++j;
                }
                // command の場合
                else {
                    unsigned char second_value = static_cast<unsigned char>(xyz_tiles[i + 1]); // 値
                    const std::size_t len =
                        (first_value == 255) ?
                        static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) :

                        ((static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 2])) << 8) // 上位バイト
                            + static_cast<std::size_t>(static_cast<unsigned char>(xyz_tiles[i + 3]))); // 下位バイト

                    for (std::size_t k = 0; k < len + 1 && j < 256 * 256; ++j, ++k) {
                        func_(second_value, static_cast<std::uint_least8_t>(j & 0xff), static_cast<std::uint_least8_t>(j >> 8)); // 値
                    }
                    i += ((first_value == 255) ? 3 : 4);
                }
            }
            return true;
        }

    };

}

#endif // !PAX_SAPIENTICA_GEOGRAPHY_TERRAIN_SLOPE_HPP
