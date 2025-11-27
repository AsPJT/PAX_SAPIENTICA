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

TEST_F (FileUtilsTest, getParentPath) {
	// テストケース1: 通常のパス
	EXPECT_EQ("Data/Map", paxs::FileSystem::getParentPath("Data/Map/file.txt"));

	// テストケース2: 複数階層のパス
	EXPECT_EQ("Projects/UnitTest/Data/File", paxs::FileSystem::getParentPath("Projects/UnitTest/Data/File/test.tsv"));

	// テストケース3: バックスラッシュ区切り（Windows形式）
	EXPECT_EQ("Data\\Map", paxs::FileSystem::getParentPath("Data\\Map\\file.txt"));

	// テストケース4: 混在したパス区切り
	EXPECT_EQ("Data/Map\\Sub", paxs::FileSystem::getParentPath("Data/Map\\Sub/file.txt"));

	// テストケース5: ディレクトリ区切りがない場合
	EXPECT_EQ("", paxs::FileSystem::getParentPath("file.txt"));

	// テストケース6: 末尾がスラッシュ（ディレクトリパスの場合）
	EXPECT_EQ("Data/Map", paxs::FileSystem::getParentPath("Data/Map/"));
}

TEST_F (FileUtilsTest, getFilename) {
	// テストケース1: 通常のパス
	EXPECT_EQ("file.txt", paxs::FileSystem::getFilename("Data/Map/file.txt"));

	// テストケース2: 複数階層のパス
	EXPECT_EQ("test.tsv", paxs::FileSystem::getFilename("Projects/UnitTest/Data/File/test.tsv"));

	// テストケース3: バックスラッシュ区切り（Windows形式）
	EXPECT_EQ("file.txt", paxs::FileSystem::getFilename("Data\\Map\\file.txt"));

	// テストケース4: 混在したパス区切り
	EXPECT_EQ("file.txt", paxs::FileSystem::getFilename("Data/Map\\Sub/file.txt"));

	// テストケース5: ディレクトリ区切りがない場合
	EXPECT_EQ("file.txt", paxs::FileSystem::getFilename("file.txt"));

	// テストケース6: 拡張子なし
	EXPECT_EQ("filename", paxs::FileSystem::getFilename("Data/Map/filename"));

	// テストケース7: ドット始まりのファイル（隠しファイル）
	EXPECT_EQ(".gitignore", paxs::FileSystem::getFilename("Projects/.gitignore"));

	// テストケース8: 複数のドットを含むファイル名
	EXPECT_EQ("file.backup.txt", paxs::FileSystem::getFilename("Data/file.backup.txt"));
}
