/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_COMPRESSION_SLOPE_CODEC_HPP
#define PAX_SAPIENTICA_IO_COMPRESSION_SLOPE_CODEC_HPP

#include <cstdint>
#include <fstream>
#include <string>

#include <PAX_SAPIENTICA/Geography/Encoding/SlopeEncoding.hpp>

namespace paxs {

    /// @brief Read 8-bit binary slope data
    /// @brief 8ビットバイナリ傾斜データの読み込み
    struct Input8BitBinary {
        /// @brief Read binary file and return slope array
        /// @brief バイナリファイルを読み込み、傾斜配列を返す
        static bool readBinary(const std::string& file_path, unsigned char* data, const std::size_t size) noexcept {
            std::ifstream file(file_path, std::ios::binary);
            if (!file) return false;
            file.read(reinterpret_cast<char*>(data), size * sizeof(unsigned char));
            return file.good();
        }
    };

    /// @brief Output slope data as 8-bit binary (0-250 range)
    /// @brief 傾斜データを8ビットバイナリ(0-250範囲)として出力
    struct SlopeDegU0To250UnitOutput {
        /// @brief Convert and write slope data to binary file
        /// @brief 傾斜データを変換してバイナリファイルに書き込む
        template<typename T>
        static bool writeBinary(const std::string& file_path, const T* data, const std::size_t size) noexcept {
            std::ofstream file(file_path, std::ios::binary);
            if (!file) return false;

            for (std::size_t i = 0; i < size; ++i) {
                unsigned char value = paxs::SlopeEncoding::slopeDegF64ToLog2U8(static_cast<double>(data[i]));
                file.write(reinterpret_cast<const char*>(&value), sizeof(unsigned char));
            }
            return file.good();
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_COMPRESSION_SLOPE_CODEC_HPP
