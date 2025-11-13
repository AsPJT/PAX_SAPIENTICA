/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_FILE_FILE_SYSTEM_HPP
#define PAX_SAPIENTICA_IO_FILE_FILE_SYSTEM_HPP

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief Handle files.
    /// @brief ファイルを扱う
    struct FileSystem {

        /// @brief Read the file.
        /// @brief ファイルを読み込む。
        /// @param relative_path アセットの相対パス / Relative to the root path configured in AppConfig.
        static std::vector<std::string> readFile(const std::string& relative_path) noexcept {
            InputFile file(relative_path);
            if (file.fail()) {
                PAXS_ERROR("Failed to read file: " + relative_path);
                return {};
            }
            // 1 行ごとに文字列を分離し vector へ格納
            std::vector<std::string> result;
            while (file.getLine()) {
                result.emplace_back(file.lineString());
            }
            return result;
        }

        /// @brief Read CSV file.
        /// @brief CSVファイルを読み込む。
        /// @param relative_path アセットの相対パス / Relative to the root path configured in AppConfig.
        static std::vector<std::vector<std::string>> readCSV(const std::string& relative_path) noexcept {
            std::vector<std::string> contents;
            try {
                contents = readFile(relative_path); // ファイルパスにあるファイルを読み込む
            }
            catch(const std::exception&) {
                PAXS_ERROR("Failed to read CSV file: " + relative_path);
                return {};
            }

            std::vector<std::vector<std::string>> result;
            for(auto& content : contents) {
                result.emplace_back(StringUtils::split(content, ','));
            }
            return result;
        }

        /// @brief Read TSV file.
        /// @brief TSVファイルを読み込む。
        /// @param relative_path アセットの相対パス / Relative to the root path configured in AppConfig.
        static std::vector<std::vector<std::string>> readTSV(const std::string& relative_path) noexcept {
            std::vector<std::string> contents;
            contents = readFile(relative_path);

            std::vector<std::vector<std::string>> result;
            for(auto& content : contents) {
                result.emplace_back(StringUtils::split(content, '\t'));
            }
            return result;
        }

        /// @brief Create directories recursively.
        /// @brief ディレクトリを再帰的に作成する。
        static bool createDirectories(const std::string& directory_path) noexcept {
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            return false; // std::filesystem が動作しないため何もしない
#else
            // 相対パスの場合は getRootPath() を使って絶対パスに変換
            const std::string root_path = AppConfig::getInstance()->getRootPath();
            const std::string full_path = root_path + directory_path;

            std::error_code ec;
            std::filesystem::create_directories(full_path, ec);
            if (ec) {
                PAXS_ERROR("Failed to create directories: " + directory_path + " (" + ec.message() + ")");
                return false;
            }
            return true;
#endif
        }

        /// @brief Check if a file or directory exists.
        /// @brief ファイルまたはディレクトリの存在を確認する。
        /// @param relative_path アセットの相対パス / Relative to the root path configured in AppConfig.
        static bool exists(const std::string& relative_path) noexcept {
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            // Android DxLib: アセットファイルは std::filesystem::exists で検出できないため
            // InputFile で試行して判定
            InputFile file(relative_path);
            return !file.fail();
#else
            const std::string absolute_path = paxs::AppConfig::getInstance()->getRootPath() + relative_path;
            return std::filesystem::exists(absolute_path);
#endif
        }

        /// @brief Create directories if they don't exist.
        /// @brief ディレクトリが存在しない場合に作成する。
        static bool createDirectoriesIfNotExists(const std::string& directory_path) noexcept {
            if (!exists(directory_path)) {
                return createDirectories(directory_path);
            }
            return true;
        }

        /// @brief Get the file paths in the directory.
        /// @brief ディレクトリ内のファイルパスを取得する。
        /// @param relative_directory_path アセットルートからの相対ディレクトリパス / Relative directory path from the asset root.
        /// @return 相対ファイルパスのベクター / Vector of relative file paths from the asset root.
        static std::vector<std::string> getFilePaths(const std::string& relative_directory_path) noexcept {
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            return {}; // std::filesystem が動作しないため何もしない
#else
            if (!exists(relative_directory_path)) {
                PAXS_ERROR("Failed to access: " + relative_directory_path);
                return {};
            }
            const std::string root_path = paxs::AppConfig::getInstance()->getRootPath();
            const std::string dir_path_str = root_path + relative_directory_path;
            std::filesystem::path dir_path(dir_path_str);
            std::filesystem::directory_iterator dir_iter(dir_path), end_iter;
            std::vector<std::string> result;

            while (dir_iter != end_iter) {
                try {
                    if(dir_iter->is_regular_file()) {
                        // 絶対パスから相対パスに変換
                        std::string absolute_path = dir_iter->path().string();
                        if (absolute_path.find(root_path) == 0) {
                            result.emplace_back(absolute_path.substr(root_path.size()));
                        } else {
                            result.emplace_back(absolute_path);
                        }
                    }
                    ++dir_iter;
                }
                catch (const std::exception&) {
                    PAXS_ERROR("Failed to access: " + dir_iter->path().string());
                    ++dir_iter;
                }
            }

            return result;
#endif
        }

        /// @brief Get parent directory path from a file path.
        /// @brief ファイルパスから親ディレクトリパスを取得する。
        /// @param file_path ファイルパス / File path.
        /// @return 親ディレクトリパス / Parent directory path.
        static std::string getParentPath(const std::string& file_path) noexcept {
            std::size_t last_slash = file_path.find_last_of("/\\");
            if (last_slash == std::string::npos) {
                return ""; // ディレクトリ区切りがない場合は空文字列
            }
            return file_path.substr(0, last_slash);
        }

        /// @brief Write PNG image file.
        /// @brief PNGファイルに画像を書き込む。
        /// @param relative_path アセットルートからの相対パス / Relative path from the asset root.
        /// @param width 画像の幅 / Image width.
        /// @param height 画像の高さ / Image height.
        /// @param channels チャンネル数（例：RGBA=4） / Number of channels (e.g., RGBA=4).
        /// @param data 画像データ / Image data.
        /// @return 成功時true / True on success.
        static bool writePngImage(
            const std::string& relative_path,
            int width,
            int height,
            int channels,
            const void* data
        ) noexcept {
            // 相対パスを絶対パスに変換
            const std::string root_path = AppConfig::getInstance()->getRootPath();
            const std::string full_path = root_path + relative_path;

            // PNG書き込み（stbi_write_pngは外部ライブラリなので絶対パスが必要）
            const int result = stbi_write_png(full_path.c_str(), width, height, channels, data, 0);

            if (result == 0) {
                PAXS_ERROR("Failed to write PNG file: " + relative_path);
                return false;
            }

            return true;
        }
    };
}

#endif // !PAX_SAPIENTICA_IO_FILE_FILE_SYSTEM_HPP
