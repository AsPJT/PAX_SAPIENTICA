/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_FILE_READ_READ_HPP
#define PAX_SAPIENTICA_TYPE_FILE_READ_READ_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/FileRead/Split.hpp>

#include <filesystem>
#include <fstream>
#include <vector>

namespace paxs {
	std::vector<std::string> readFile(const std::string& file_path) {
        // ファイルの読み込み
        std::ifstream file(file_path);
        if (!file) {
            std::cout << "File not found." << std::endl;
            std::exit(1);
        }
        std::string line;
        std::vector<std::string> result;
        while (std::getline(file, line)) {
            result.emplace_back(line);
        }
        return result;
	}
    std::vector<std::vector<std::string>> readCSV(const std::string& file_path) {
        std::vector<std::string> contents = readFile(file_path);
        std::vector<std::vector<std::string>> result;
        for(auto& content : contents) {
            result.emplace_back(split(content, ','));
        }
        return result;
    }
    std::vector<std::vector<std::string>> readTSV(const std::string& file_path) {
        std::vector<std::string> contents = readFile(file_path);
        std::vector<std::vector<std::string>> result;
        for(auto& content : contents) {
            result.emplace_back(split(content, '\t'));
        }
        return result;
    }
    std::vector<std::string> getFileNames(const std::string& directory_path) {
        std::filesystem::path dir_path(directory_path);
        std::filesystem::directory_iterator dir_iter(dir_path);
        std::vector<std::string> result;
        for (const auto& file : dir_iter) {
            result.emplace_back(file.path().string());
        }
        return result;
    }
}

#endif // !PAX_SAPIENTICA_TYPE_FILE_READ_READ_HPP