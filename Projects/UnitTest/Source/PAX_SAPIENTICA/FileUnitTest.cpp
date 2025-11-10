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

#include <PAX_SAPIENTICA/File.hpp>

#ifdef PROJECT_ROOT_PATH
const std::string root = PROJECT_ROOT_PATH;
const std::string prefix = root + "/Projects/UnitTest/Data/File";
#else
const std::string prefix = "../Data/File";
#endif // PROJECT_ROOT_PATH

TEST (FileUnitTest, readFile) {
    const std::string file_path = prefix + "/test_file.txt";
    const std::vector<std::string> expected = {
        "Test",
        "file"
    };
    const std::vector<std::string> actual = paxs::File::readFile(file_path);
    ASSERT_EQ(expected, actual);
}

TEST (FileUnitTest, readCSV) {
    const std::string file_path = prefix + "/test_file.csv";
    const std::vector<std::vector<std::string>> expected = {
        {"Test", "file"},
        {"Test2", "file2"}
    };
    const std::vector<std::vector<std::string>> actual = paxs::File::readCSV(file_path);
    ASSERT_EQ(expected, actual);
}

TEST (FileUnitTest, readTSV) {
    const std::string file_path = prefix + "/test_file.tsv";
    const std::vector<std::vector<std::string>> expected = {
        {"Test", "file"},
        {"Test2", "file2"}
    };
    const std::vector<std::vector<std::string>> actual = paxs::File::readTSV(file_path);
    ASSERT_EQ(expected, actual);
}

#ifndef _WIN32
TEST (FileUnitTest, getFileNames) {
    const std::vector<std::string> expected = {
        prefix + "/test_file.csv",
        prefix + "/test_file.tsv",
        prefix + "/test_file.txt"
    };
    std::vector<std::string> actual = paxs::File::getFileNames(prefix);

    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(expected, actual);

}
#endif // !_WIN32
