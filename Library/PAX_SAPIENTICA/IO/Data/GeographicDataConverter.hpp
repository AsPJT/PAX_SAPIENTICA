/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_DATA_GEOGRAPHIC_DATA_CONVERTER_HPP
#define PAX_SAPIENTICA_IO_DATA_GEOGRAPHIC_DATA_CONVERTER_HPP

/*##########################################################################################
    地理データ（TSV <-> Binary）変換ユーティリティ
    Geographic data (TSV <-> Binary) conversion utility
##########################################################################################*/

#include <string>
#include <vector>

#include <PAX_SAPIENTICA/IO/Data/BinaryTableWriter.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief Geographic data format converter
    /// @brief 地理データフォーマット変換器
    class GeographicDataConverter {
    public:
        /// @brief Convert a single TSV file to binary format
        /// @brief 単一のTSVファイルをバイナリ形式に変換
        /// @param tsv_path TSV file path / TSVファイルパス
        /// @param binary_path Binary file path (optional, auto-generated if empty) / バイナリファイルパス（省略時は自動生成）
        /// @return true if successfully converted / 変換成功時true
        static bool tsvToBinary(const std::string& tsv_path, const std::string& binary_path = "") {
            std::string output_path = binary_path;
            if (output_path.empty()) {
                // Auto-generate output path
                if (tsv_path.size() >= 4 && tsv_path.substr(tsv_path.size() - 4) == ".tsv") {
                    output_path = tsv_path.substr(0, tsv_path.size() - 4) + ".bin";
                } else {
                    output_path = tsv_path + ".bin";
                }
            }

            PAXS_INFO("Converting TSV to binary: " + tsv_path + " -> " + output_path);
            return BinaryTableWriter::convertTsvToBinary(tsv_path, output_path);
        }

        /// @brief Convert a single binary file to TSV format
        /// @brief 単一のバイナリファイルをTSV形式に変換
        /// @param binary_path Binary file path / バイナリファイルパス
        /// @param tsv_path TSV file path (optional, auto-generated if empty) / TSVファイルパス（省略時は自動生成）
        /// @return true if successfully converted / 変換成功時true
        static bool binaryToTsv(const std::string& binary_path, const std::string& tsv_path = "") {
            std::string output_path = tsv_path;
            if (output_path.empty()) {
                // Auto-generate output path
                if (binary_path.size() >= 4 && binary_path.substr(binary_path.size() - 4) == ".bin") {
                    output_path = binary_path.substr(0, binary_path.size() - 4) + ".tsv";
                } else {
                    output_path = binary_path + ".tsv";
                }
            }

            PAXS_INFO("Converting binary to TSV: " + binary_path + " -> " + output_path);
            return BinaryTableWriter::convertBinaryToTsv(binary_path, output_path);
        }

        /// @brief Convert all TSV files in a directory to binary format
        /// @brief ディレクトリ内のすべてのTSVファイルをバイナリ形式に変換
        /// @param directory_path Directory path / ディレクトリパス
        /// @param recursive Recursively convert subdirectories / サブディレクトリも再帰的に変換するか
        /// @return Number of successfully converted files / 変換成功したファイル数
        static int convertDirectoryTsvToBinary(const std::string& directory_path, bool recursive = false) {
            std::vector<std::string> files;
            
            if (recursive) {
                files = FileSystem::getFilePathsRecursive(directory_path);
            } else {
                files = FileSystem::getFilePaths(directory_path);
            }

            int success_count = 0;
            int tsv_file_count = 0;

            for (const std::string& file_path : files) {
                // Check if file is a TSV file
                if (file_path.size() >= 4 && file_path.substr(file_path.size() - 4) == ".tsv") {
                    ++tsv_file_count;
                    if (tsvToBinary(file_path)) {
                        ++success_count;
                    }
                }
            }

            PAXS_INFO("Converted " + std::to_string(success_count) + " / " + std::to_string(tsv_file_count) + " TSV files to binary");
            return success_count;
        }

        /// @brief Convert all binary files in a directory to TSV format
        /// @brief ディレクトリ内のすべてのバイナリファイルをTSV形式に変換
        /// @param directory_path Directory path / ディレクトリパス
        /// @param recursive Recursively convert subdirectories / サブディレクトリも再帰的に変換するか
        /// @return Number of successfully converted files / 変換成功したファイル数
        static int convertDirectoryBinaryToTsv(const std::string& directory_path, bool recursive = false) {
            std::vector<std::string> files;
            
            if (recursive) {
                files = FileSystem::getFilePathsRecursive(directory_path);
            } else {
                files = FileSystem::getFilePaths(directory_path);
            }

            int success_count = 0;
            int binary_file_count = 0;

            for (const std::string& file_path : files) {
                // Check if file is a binary file
                if (file_path.size() >= 4 && file_path.substr(file_path.size() - 4) == ".bin") {
                    ++binary_file_count;
                    if (binaryToTsv(file_path)) {
                        ++success_count;
                    }
                }
            }

            PAXS_INFO("Converted " + std::to_string(success_count) + " / " + std::to_string(binary_file_count) + " binary files to TSV");
            return success_count;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_DATA_GEOGRAPHIC_DATA_CONVERTER_HPP
