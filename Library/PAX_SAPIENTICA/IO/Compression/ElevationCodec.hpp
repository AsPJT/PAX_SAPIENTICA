/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_COMPRESSION_ELEVATION_CODEC_HPP
#define PAX_SAPIENTICA_IO_COMPRESSION_ELEVATION_CODEC_HPP

#include <cstdint>
#include <fstream>
#include <string>

#include <PAX_SAPIENTICA/Geography/Encoding/ElevationEncoding.hpp>

namespace paxs {

    /// @brief Read 16-bit binary elevation data
    /// @brief 16ビットバイナリ標高データの読み込み
    struct Input16BitBinary {
        /// @brief Read binary file and return elevation array
        /// @brief バイナリファイルを読み込み、標高配列を返す
        static bool readBinary(const std::string& file_path, std::int_least16_t* data, const std::size_t size) noexcept {
            std::ifstream file(file_path, std::ios::binary);
            if (!file) return false;
            file.read(reinterpret_cast<char*>(data), size * sizeof(std::int_least16_t));
            return file.good();
        }
    };

    /// @brief Output elevation data as 16-bit binary
    /// @brief 標高データを16ビットバイナリとして出力
    struct ElevationS16UnitOutput {
        /// @brief Convert and write elevation data to binary file
        /// @brief 標高データを変換してバイナリファイルに書き込む
        template<typename T>
        static bool writeBinary(const std::string& file_path, const T* data, const std::size_t size) noexcept {
            std::ofstream file(file_path, std::ios::binary);
            if (!file) return false;

            for (std::size_t i = 0; i < size; ++i) {
                std::int_least16_t value = paxs::ElevationEncoding::elevationF64ToLog2S16(static_cast<double>(data[i]));
                file.write(reinterpret_cast<const char*>(&value), sizeof(std::int_least16_t));
            }
            return file.good();
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_COMPRESSION_ELEVATION_CODEC_HPP
