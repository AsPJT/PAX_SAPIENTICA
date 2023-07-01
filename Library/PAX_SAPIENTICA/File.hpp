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
#include <vector>

#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {
    
    /// @brief A class that handles files.
    class File {
    public:
        /// @brief Read the file.
        static std::vector<std::string> readFile(const std::string& file_path) {
            std::ifstream file(file_path);
            if (!file) {
                std::cout << "file_path:" << file_path << " not found." << std::endl;
                std::exit(1);
            }

            std::string line;
            std::vector<std::string> result;
            while (std::getline(file, line)) {
                result.emplace_back(line);
            }
            file.close();
            return result;
        }

        /// @brief Read CSV file.
        static std::vector<std::vector<std::string>> readCSV(const std::string& file_path) {
            std::vector<std::string> contents = readFile(file_path);
            std::vector<std::vector<std::string>> result;
            for(auto& content : contents) {
                result.emplace_back(StringExtensions::split(content, ','));
            }
            return result;
        }

        /// @brief Read TSV file.
        static std::vector<std::vector<std::string>> readTSV(const std::string& file_path) {
            std::vector<std::string> contents = readFile(file_path);
            std::vector<std::vector<std::string>> result;
            for(auto& content : contents) {
                result.emplace_back(StringExtensions::split(content, '\t'));
            }
            return result;
        }

        /// @brief Get the file name in the directory.
        static std::vector<std::string> getFileNames(const std::string& directory_path) {
            std::filesystem::path dir_path(directory_path);
            std::filesystem::directory_iterator dir_iter(dir_path);
            std::vector<std::string> result;
            for (const auto& file : dir_iter) {
                result.emplace_back(file.path().string());
            }
            return result;
        }
    };
}

#endif // !PAX_SAPIENTICA_TYPE_FILE_HPP