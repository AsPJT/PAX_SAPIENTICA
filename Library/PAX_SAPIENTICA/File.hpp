/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_FILE_HPP
#define PAX_SAPIENTICA_TYPE_FILE_HPP

/*##########################################################################################

##########################################################################################*/

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {
    
    /// @brief Handle files.
    /// @brief ファイルを扱う
    struct File {

        /// @brief Read the file.
        /// @brief ファイルを読み込む。
        static std::vector<std::string> readFile(const std::string& file_path) {
            std::ifstream file(file_path); // ファイルパスにあるファイルを読み込む
            if (!file) { // もしファイルが無い場合はエラーを出す
                Logger logger("Save/error_log.txt"); // ログ管理クラス
                const std::string message = "Failed to read file: " + file_path; // エラーメッセージ
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message); // ログを出力
                throw std::runtime_error(message);
            }
            // 1 行ごとに文字列を分離し vector へ格納
            std::string line;
            std::vector<std::string> result;
            while (std::getline(file, line)) {
                result.emplace_back(line);
            }
            file.close();
            return result;
        }

        /// @brief Read CSV file.
        /// @brief CSVファイルを読み込む。
        static std::vector<std::vector<std::string>> readCSV(const std::string& file_path) {
            std::vector<std::string> contents;
            try {
                contents = readFile(file_path); // ファイルパスにあるファイルを読み込む
            }
            catch(const std::exception&) {
                Logger logger("Save/error_log.txt");
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, "Failed to read CSV file: " + file_path);
                throw;
            }
            
            std::vector<std::vector<std::string>> result;
            for(auto& content : contents) {
                result.emplace_back(StringExtensions::split(content, ','));
            }
            return result;
        }

        /// @brief Read TSV file.
        /// @brief TSVファイルを読み込む。
        static std::vector<std::vector<std::string>> readTSV(const std::string& file_path) {
            std::vector<std::string> contents;
            try {
                contents = readFile(file_path); // ファイルパスにあるファイルを読み込む
            }
            catch(const std::exception&) {
                Logger logger("Save/error_log.txt");
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, "Failed to read TSV file: " + file_path);
                throw;
            }

            std::vector<std::vector<std::string>> result;
            for(auto& content : contents) {
                result.emplace_back(StringExtensions::split(content, '\t'));
            }
            return result;
        }

        /// @brief Get the file name in the directory.
        /// @brief ディレクトリ内のファイル名を取得する。
        static std::vector<std::string> getFileNames(const std::string& directory_path) {
            std::filesystem::path dir_path(directory_path);
            if (!std::filesystem::exists(dir_path)) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to get file names: " + directory_path;
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }

            std::filesystem::directory_iterator dir_iter(dir_path), end_iter;
            std::vector<std::string> result;

            while (dir_iter != end_iter) {
                try {
                    if(dir_iter->is_regular_file()) {
                        result.push_back(dir_iter->path().string());
                    }
                    ++dir_iter;
                }
                catch (const std::exception&) {
                    Logger logger("Save/error_log.txt");
                    logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, "Failed to access: " + dir_iter->path().string());
                    throw;
                }
            }

            return result;
        }
    };
}

#endif // !PAX_SAPIENTICA_TYPE_FILE_HPP
