/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <fstream>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>

class PathTSV {
public:
	static void generatePathTSV() {
		std::ofstream ofs("Config.tsv");
		ofs << "key\tvalue\nasset_file\t../" << std::endl;
		ofs.close();
	}

	static void removePathTSV() {
		std::remove("Config.tsv");
	}
};

class FileUtilsTest : public ::testing::Test {
protected:
	void SetUp() override {
		is_path_tsv_exist = std::filesystem::exists("Config.tsv");
		if (!is_path_tsv_exist) {
			PathTSV::generatePathTSV();
		}
	}

	void TearDown() override {
		if (!is_path_tsv_exist) {
			PathTSV::removePathTSV();
		}
	}

	bool is_path_tsv_exist = false;
};

TEST_F (FileUtilsTest, readFile) {
	const std::string file_path = "Projects/UnitTest/Data/File/test_file.txt";
	const std::vector<std::string> expected = {
		"Test",
		"file"
	};
	const std::vector<std::string> actual = paxs::FileSystem::readFile(file_path);
	ASSERT_EQ(expected, actual);
}

TEST_F (FileUtilsTest, readCSV) {
	const std::string file_path = "Projects/UnitTest/Data/File/test_file.csv";
	const std::vector<std::vector<std::string>> expected = {
		{"Test", "file"},
		{"Test2", "file2"}
	};
	const std::vector<std::vector<std::string>> actual = paxs::FileSystem::readCSV(file_path);
	ASSERT_EQ(expected, actual);
}

TEST_F (FileUtilsTest, readTSV) {
	const std::string file_path = "Projects/UnitTest/Data/File/test_file.tsv";
	const std::vector<std::vector<std::string>> expected = {
		{"Test", "file"},
		{"Test2", "file2"}
	};
	const std::vector<std::vector<std::string>> actual = paxs::FileSystem::readTSV(file_path);
	ASSERT_EQ(expected, actual);
}
