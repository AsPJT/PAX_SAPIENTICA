/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_DATA_BINARY_TABLE_WRITER_HPP
#define PAX_SAPIENTICA_IO_DATA_BINARY_TABLE_WRITER_HPP

/*##########################################################################################
    TSVファイルをバイナリ形式に変換するクラス
    Converts TSV files to binary format
##########################################################################################*/

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/IO/Data/BinaryTable.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

namespace paxs {

    /// @brief Binary table writer class for converting TSV to binary format
    /// @brief TSVをバイナリ形式に変換するライタークラス
    class BinaryTableWriter {
    private:
        /// @brief Write a null-terminated string to binary stream
        /// @brief バイナリストリームにヌル終端文字列を書き込む
        /// @param ofs Output file stream / 出力ファイルストリーム
        /// @param str String to write / 書き込む文字列
        static void writeString(std::ofstream& ofs, const std::string& str) {
            ofs.write(str.c_str(), str.size() + 1);  // +1 for null terminator
        }

        /// @brief Write float value to binary stream
        /// @brief バイナリストリームにfloat値を書き込む
        /// @param ofs Output file stream / 出力ファイルストリーム
        /// @param value Float value to write / 書き込むfloat値
        static void writeFloat(std::ofstream& ofs, float value) {
            ofs.write(reinterpret_cast<const char*>(&value), sizeof(float));
        }

        /// @brief Write int32 value to binary stream
        /// @brief バイナリストリームにint32値を書き込む
        /// @param ofs Output file stream / 出力ファイルストリーム
        /// @param value Int32 value to write / 書き込むint32値
        static void writeInt32(std::ofstream& ofs, std::int32_t value) {
            ofs.write(reinterpret_cast<const char*>(&value), sizeof(std::int32_t));
        }

        /// @brief Write uint32 value to binary stream
        /// @brief バイナリストリームにuint32値を書き込む
        /// @param ofs Output file stream / 出力ファイルストリーム
        /// @param value Uint32 value to write / 書き込むuint32値
        static void writeUint32(std::ofstream& ofs, std::uint32_t value) {
            ofs.write(reinterpret_cast<const char*>(&value), sizeof(std::uint32_t));
        }

        /// @brief Determine column type from column name
        /// @brief カラム名からカラム型を判定
        /// @param column_name Column name / カラム名
        /// @return Column type / カラム型
        static BinaryColumnType getColumnType(const std::string& column_name) {
            const std::uint_least32_t hash = MurMur3::calcHash(column_name);
            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            const std::uint_least32_t value_hash = MurMur3::calcHash("value");
            const std::uint_least32_t longitude_hash = MurMur3::calcHash("longitude");
            const std::uint_least32_t latitude_hash = MurMur3::calcHash("latitude");
            const std::uint_least32_t first_year_hash = MurMur3::calcHash("first_year");
            const std::uint_least32_t last_year_hash = MurMur3::calcHash("last_year");

            if (hash == key_hash) return BinaryColumnType::KeyHash;
            if (hash == value_hash) return BinaryColumnType::ValueString;
            if (hash == longitude_hash) return BinaryColumnType::Longitude;
            if (hash == latitude_hash) return BinaryColumnType::Latitude;
            if (hash == first_year_hash) return BinaryColumnType::FirstYear;
            if (hash == last_year_hash) return BinaryColumnType::LastYear;
            
            return BinaryColumnType::Unknown;
        }

    public:
        /// @brief Convert TSV file to binary format
        /// @brief TSVファイルをバイナリ形式に変換
        /// @param tsv_relative_path Input TSV file path / 入力TSVファイルパス
        /// @param binary_relative_path Output binary file path / 出力バイナリファイルパス
        /// @return true if successfully converted / 変換成功時true
        static bool convertTsvToBinary(const std::string& tsv_relative_path, const std::string& binary_relative_path) {
            // Load TSV file
            TsvTable tsv(tsv_relative_path);
            if (!tsv.isSuccessfullyLoaded()) {
                PAXS_ERROR("Failed to load TSV file for conversion: " + tsv_relative_path);
                return false;
            }

            // Get column information
            const auto& header_keys = tsv.getHeaderKeys();
            const std::size_t column_count = header_keys.size();
            const std::size_t row_count = tsv.rowCount();

            if (column_count == 0) {
                PAXS_ERROR("TSV file has no columns: " + tsv_relative_path);
                return false;
            }

            if (column_count > 255) {
                PAXS_ERROR("TSV file has too many columns (max 255): " + tsv_relative_path);
                return false;
            }

            // Determine column types
            std::vector<BinaryColumnType> column_types(column_count);
            std::vector<std::string> column_names(column_count);
            std::size_t longitude_index = SIZE_MAX;
            std::size_t latitude_index = SIZE_MAX;

            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            const std::uint_least32_t value_hash = MurMur3::calcHash("value");
            const std::uint_least32_t longitude_hash = MurMur3::calcHash("longitude");
            const std::uint_least32_t latitude_hash = MurMur3::calcHash("latitude");
            const std::uint_least32_t first_year_hash = MurMur3::calcHash("first_year");
            const std::uint_least32_t last_year_hash = MurMur3::calcHash("last_year");

            for (std::size_t i = 0; i < column_count; ++i) {
                const std::uint_least32_t column_key = header_keys[i];
                
                if (column_key == key_hash) {
                    column_types[i] = BinaryColumnType::KeyHash;
                    column_names[i] = "key";
                } else if (column_key == value_hash) {
                    column_types[i] = BinaryColumnType::ValueString;
                    column_names[i] = "value";
                } else if (column_key == longitude_hash) {
                    column_types[i] = BinaryColumnType::Longitude;
                    column_names[i] = "longitude";
                    longitude_index = i;
                } else if (column_key == latitude_hash) {
                    column_types[i] = BinaryColumnType::Latitude;
                    column_names[i] = "latitude";
                    latitude_index = i;
                } else if (column_key == first_year_hash) {
                    column_types[i] = BinaryColumnType::FirstYear;
                    column_names[i] = "first_year";
                } else if (column_key == last_year_hash) {
                    column_types[i] = BinaryColumnType::LastYear;
                    column_names[i] = "last_year";
                } else {
                    column_types[i] = BinaryColumnType::Unknown;
                    PAXS_WARNING("Unknown column type in TSV, skipping: " + std::to_string(column_key));
                }
            }

            // Open output binary file
            const std::string full_path = AppConfig::getInstance().getRootPath() + binary_relative_path;
            std::ofstream ofs(full_path, std::ios::binary);
            if (!ofs.is_open()) {
                PAXS_ERROR("Failed to open binary file for writing: " + binary_relative_path);
                return false;
            }

            // 1バイト目：カラムの個数
            const std::uint8_t column_count_u8 = static_cast<std::uint8_t>(column_count);
            ofs.write(reinterpret_cast<const char*>(&column_count_u8), 1);

            // 2バイト目：データの個数のバイト数
            std::uint8_t data_count_bytes = 1;
            if (row_count > 0xFFFFFF) data_count_bytes = 4;
            else if (row_count > 0xFFFF) data_count_bytes = 3;
            else if (row_count > 0xFF) data_count_bytes = 2;
            ofs.write(reinterpret_cast<const char*>(&data_count_bytes), 1);

            // 3～6バイト目：データの個数
            const std::uint32_t row_count_u32 = static_cast<std::uint32_t>(row_count);
            for (int i = data_count_bytes - 1; i >= 0; --i) {
                const std::uint8_t byte_value = static_cast<std::uint8_t>((row_count_u32 >> (i * 8)) & 0xFF);
                ofs.write(reinterpret_cast<const char*>(&byte_value), 1);
            }

            // カラム定義を書き込む
            for (std::size_t i = 0; i < column_count; ++i) {
                const std::uint8_t column_type_id = static_cast<std::uint8_t>(column_types[i]);
                ofs.write(reinterpret_cast<const char*>(&column_type_id), 1);
            }

            // データを書き込む
            for (std::size_t row = 0; row < row_count; ++row) {
                bool longitude_latitude_written = false;

                for (std::size_t col = 0; col < column_count; ++col) {
                    const std::string& cell_value = tsv.get(row, header_keys[col]);

                    // longitude と latitude は同時に書き込む
                    if (column_types[col] == BinaryColumnType::Longitude) {
                        if (!longitude_latitude_written && latitude_index != SIZE_MAX) {
                            const std::string& lon_str = cell_value;
                            const std::string& lat_str = tsv.get(row, header_keys[latitude_index]);

                            auto lon_opt = StringUtils::toDouble(lon_str);
                            auto lat_opt = StringUtils::toDouble(lat_str);

                            if (lon_opt && lat_opt) {
                                writeFloat(ofs, static_cast<float>(*lon_opt));
                                writeFloat(ofs, static_cast<float>(*lat_opt));
                                longitude_latitude_written = true;
                            } else {
                                PAXS_WARNING("Invalid longitude/latitude at row " + std::to_string(row));
                                writeFloat(ofs, 0.0f);
                                writeFloat(ofs, 0.0f);
                                longitude_latitude_written = true;
                            }
                        }
                    }
                    else if (column_types[col] == BinaryColumnType::Latitude) {
                        // latitude は longitude と同時に書き込み済み
                        continue;
                    }
                    else if (column_types[col] == BinaryColumnType::KeyHash) {
                        // keyをハッシュ化してuint32として書き込む
                        const std::uint32_t key_hash = MurMur3::calcHash(cell_value.size(), cell_value.c_str());
                        writeUint32(ofs, key_hash);
                    }
                    else if (column_types[col] == BinaryColumnType::ValueString) {
                        writeString(ofs, cell_value);
                    }
                    else if (column_types[col] == BinaryColumnType::FirstYear ||
                        column_types[col] == BinaryColumnType::LastYear) {
                        // 年をint32として書き込む
                        auto year_opt = StringUtils::toInt(cell_value);
                        if (year_opt) {
                            writeInt32(ofs, static_cast<std::int32_t>(*year_opt));
                        } else {
                            PAXS_WARNING("Invalid year value at row " + std::to_string(row) + ": \"" + cell_value + "\"");
                            writeInt32(ofs, 0);
                        }
                    }
                }
            }

            ofs.close();
            PAXS_INFO("Successfully converted TSV to binary: " + binary_relative_path);
            return true;
        }

        /// @brief Convert binary file back to TSV format
        /// @brief バイナリファイルをTSV形式に変換
        /// @param binary_relative_path Input binary file path / 入力バイナリファイルパス
        /// @param tsv_relative_path Output TSV file path / 出力TSVファイルパス
        /// @return true if successfully converted / 変換成功時true
        static bool convertBinaryToTsv(const std::string& binary_relative_path, const std::string& tsv_relative_path) {
            // Load binary file
            BinaryTable binary(binary_relative_path);
            if (!binary.isSuccessfullyLoaded()) {
                PAXS_ERROR("Failed to load binary file for conversion: " + binary_relative_path);
                return false;
            }

            // Open output TSV file
            const std::string full_path = AppConfig::getInstance().getRootPath() + tsv_relative_path;
            std::ofstream ofs(full_path);
            if (!ofs.is_open()) {
                PAXS_ERROR("Failed to open TSV file for writing: " + tsv_relative_path);
                return false;
            }

            // Write header
            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            const std::uint_least32_t value_hash = MurMur3::calcHash("value");
            const std::uint_least32_t longitude_hash = MurMur3::calcHash("longitude");
            const std::uint_least32_t latitude_hash = MurMur3::calcHash("latitude");
            const std::uint_least32_t first_year_hash = MurMur3::calcHash("first_year");
            const std::uint_least32_t last_year_hash = MurMur3::calcHash("last_year");

            std::vector<std::string> header_names;
            if (binary.hasColumn(key_hash)) header_names.push_back("key");
            if (binary.hasColumn(value_hash)) header_names.push_back("value");
            if (binary.hasColumn(longitude_hash)) header_names.push_back("longitude");
            if (binary.hasColumn(latitude_hash)) header_names.push_back("latitude");
            if (binary.hasColumn(first_year_hash)) header_names.push_back("first_year");
            if (binary.hasColumn(last_year_hash)) header_names.push_back("last_year");

            for (std::size_t i = 0; i < header_names.size(); ++i) {
                if (i > 0) ofs << '\t';
                ofs << header_names[i];
            }
            ofs << '\n';

            // Write data rows
            binary.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                (void)row_index;
                for (std::size_t i = 0; i < row.size(); ++i) {
                    if (i > 0) ofs << '\t';
                    ofs << row[i];
                }
                ofs << '\n';
            });

            ofs.close();
            PAXS_INFO("Successfully converted binary to TSV: " + tsv_relative_path);
            return true;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_DATA_BINARY_TABLE_WRITER_HPP
