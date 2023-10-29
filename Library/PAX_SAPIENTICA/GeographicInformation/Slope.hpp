/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_SLOPE_HPP
#define PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_SLOPE_HPP

/*##########################################################################################

##########################################################################################*/

// C ライブラリ
#include <cmath>
#include <cstddef>

// C++ ライブラリ
#include <array>
#include <fstream>
#include <limits>
#include <string>

#include <PAX_GRAPHICA/InputFile.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>

namespace paxs {

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

        paxg::InputFile ifs;

        paxs::SlopeDegU0To250UnitOutput sd;

        const char delimiter{};

        SlopeDegU0To250AllOutput(
            const std::string& ifs_str_,
            const std::string& ifs_str2_,
            const std::string& ofs_str_,
            const std::string& ofs_str2_,
            const char delimiter_
        ) :ifs(ifs_str_, ifs_str2_), delimiter(delimiter_) {
            if (ifs.fail()) return; // 読み込み失敗

            // 読み込み成功の場合は出力を生成する
            sd = paxs::SlopeDegU0To250UnitOutput(ofs_str2_ + ofs_str_);
        }

        void calc() {
            if (ifs.fail()) return; // 読み込み失敗
            unsigned char pre_value = 252; // 1 つ前の値
            std::array<unsigned char, 256> xyz_one_line{};
            while (ifs.getLine()) {
                ifs.splitSlopeDegF64ToU0To250(delimiter, xyz_one_line.data(), 256);
                for (std::size_t i = 0; i < 256; ++i) {
                    sd.calc(pre_value, xyz_one_line[i]);
                    pre_value = xyz_one_line[i];
                }
            }
            sd.calc(pre_value, 252);
        }

    };

    // 8bit 傾斜バイナリデータを読み込む
    struct Input8BitBinarySlope {

        paxg::InputFile ifs;

        Input8BitBinarySlope(
            const std::string& ifs_str_,
            const std::string& ifs_str2_
        ) :ifs(ifs_str_, ifs_str2_, paxs::MurMur3::calcHash("asset_file"), paxs::MurMur3::calcHash("binary")) {}

        void calc(unsigned char* tiles_) {
            if (ifs.fail()) return; // 読み込み失敗
            char xyz_tiles[256 * 256]{};
            const std::size_t byte_num = ifs.splitSlopeDegF64ToU0To250Binary(xyz_tiles, 256 * 256); // バイト数
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
        }

    };

}

#endif // !PAX_SAPIENTICA_GEOGRAPHIC_INFORMATION_SLOPE_HPP
