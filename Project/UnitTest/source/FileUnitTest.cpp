#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/File.hpp>

TEST (FileUnitTest, readFile) {
    const std::string file_path = "../data/test_file.txt";
    const std::vector<std::string> expected = {
        "Test",
        "file"
    };
    const std::vector<std::string> actual = paxs::File::readFile(file_path);
    ASSERT_EQ(expected, actual);
}