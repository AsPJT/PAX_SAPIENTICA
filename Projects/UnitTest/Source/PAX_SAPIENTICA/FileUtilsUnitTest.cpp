/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>

#ifdef PROJECT_ROOT_PATH
const std::string root = PROJECT_ROOT_PATH;
const std::string prefix = root + "/Projects/UnitTest/Data/File";
#else
const std::string prefix = "../Data/File";
#endif // PROJECT_ROOT_PATH

TEST (FileUtilsUnitTest, readFile) {
    const std::string file_path = prefix + "/test_file.txt";
    const std::vector<std::string> expected = {
        "Test",
        "file"
    };
    const std::vector<std::string> actual = paxs::FileSystem::readFile(file_path);
    ASSERT_EQ(expected, actual);
}

TEST (FileUtilsUnitTest, readCSV) {
    const std::string file_path = prefix + "/test_file.csv";
    const std::vector<std::vector<std::string>> expected = {
        {"Test", "file"},
        {"Test2", "file2"}
    };
    const std::vector<std::vector<std::string>> actual = paxs::FileSystem::readCSV(file_path);
    ASSERT_EQ(expected, actual);
}

TEST (FileUtilsUnitTest, readTSV) {
    const std::string file_path = prefix + "/test_file.tsv";
    const std::vector<std::vector<std::string>> expected = {
        {"Test", "file"},
        {"Test2", "file2"}
    };
    const std::vector<std::vector<std::string>> actual = paxs::FileSystem::readTSV(file_path);
    ASSERT_EQ(expected, actual);
}

