/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

// Test fixture for TsvTable tests
class TsvTableTest : public ::testing::Test {
protected:
    std::string test_file_path = "test_data.tsv";

    void SetUp() override {
        // Create a test TSV file
        std::ofstream ofs(test_file_path);
        ofs << "key\tvalue\tcount\n";
        ofs << "apple\tred\t5\n";
        ofs << "banana\tyellow\t3\n";
        ofs << "orange\torange\t7\n";
        ofs.close();
    }

    void TearDown() override {
        // Clean up test file
        std::remove(test_file_path.c_str());
    }
};

TEST_F(TsvTableTest, LoadFile) {
    paxs::TsvTable table;
    EXPECT_TRUE(table.load(test_file_path));
    EXPECT_TRUE(table.isLoaded());
    EXPECT_TRUE(table.isSuccessfullyLoaded());
}

TEST_F(TsvTableTest, LoadNonExistentFile) {
    paxs::TsvTable table;
    EXPECT_FALSE(table.load("nonexistent.tsv"));
    EXPECT_TRUE(table.isLoaded());
    EXPECT_FALSE(table.isSuccessfullyLoaded());
}

TEST_F(TsvTableTest, ConstructorLoad) {
    paxs::TsvTable table(test_file_path);
    EXPECT_TRUE(table.isLoaded());
    EXPECT_TRUE(table.isSuccessfullyLoaded());
}

TEST_F(TsvTableTest, RowAndColumnCount) {
    paxs::TsvTable table(test_file_path);
    EXPECT_EQ(3u, table.rowCount());
    EXPECT_EQ(3u, table.columnCount());
}

TEST_F(TsvTableTest, HasColumn) {
    paxs::TsvTable table(test_file_path);
    EXPECT_TRUE(table.hasColumn("key"));
    EXPECT_TRUE(table.hasColumn("value"));
    EXPECT_TRUE(table.hasColumn("count"));
    EXPECT_FALSE(table.hasColumn("nonexistent"));
}

TEST_F(TsvTableTest, HasColumnByHash) {
    paxs::TsvTable table(test_file_path);
    EXPECT_TRUE(table.hasColumn(paxs::MurMur3::calcHash("key")));
    EXPECT_TRUE(table.hasColumn(paxs::MurMur3::calcHash("value")));
    EXPECT_FALSE(table.hasColumn(paxs::MurMur3::calcHash("nonexistent")));
}

TEST_F(TsvTableTest, GetColumnIndex) {
    paxs::TsvTable table(test_file_path);
    EXPECT_EQ(0u, table.getColumnIndex("key"));
    EXPECT_EQ(1u, table.getColumnIndex("value"));
    EXPECT_EQ(2u, table.getColumnIndex("count"));
    EXPECT_EQ(SIZE_MAX, table.getColumnIndex("nonexistent"));
}

TEST_F(TsvTableTest, GetCellByIndex) {
    paxs::TsvTable table(test_file_path);
    EXPECT_EQ("apple", table.get(std::size_t(0), std::size_t(0)));
    EXPECT_EQ("red", table.get(std::size_t(0), std::size_t(1)));
    EXPECT_EQ("5", table.get(std::size_t(0), std::size_t(2)));
    EXPECT_EQ("banana", table.get(std::size_t(1), std::size_t(0)));
    EXPECT_EQ("yellow", table.get(std::size_t(1), std::size_t(1)));
    EXPECT_EQ("3", table.get(std::size_t(1), std::size_t(2)));
}

TEST_F(TsvTableTest, GetCellByColumnName) {
    paxs::TsvTable table(test_file_path);
    EXPECT_EQ("apple", table.get(0, "key"));
    EXPECT_EQ("red", table.get(0, "value"));
    EXPECT_EQ("5", table.get(0, "count"));
}

TEST_F(TsvTableTest, GetCellByColumnHash) {
    paxs::TsvTable table(test_file_path);
    const std::uint_least32_t key_hash = paxs::MurMur3::calcHash("key");
    const std::uint_least32_t value_hash = paxs::MurMur3::calcHash("value");
    const std::uint_least32_t count_hash = paxs::MurMur3::calcHash("count");
    EXPECT_EQ("apple", table.get(0, key_hash));
    EXPECT_EQ("red", table.get(0, value_hash));
    EXPECT_EQ("5", table.get(0, count_hash));
}

TEST_F(TsvTableTest, GetOutOfBounds) {
    paxs::TsvTable table(test_file_path);
    EXPECT_EQ("", table.get(std::size_t(10), std::size_t(0)));      // Row out of bounds
    EXPECT_EQ("", table.get(std::size_t(0), std::size_t(10)));      // Column out of bounds
    EXPECT_EQ("", table.get(std::size_t(10), std::size_t(10)));     // Both out of bounds
}

TEST_F(TsvTableTest, GetRow) {
    paxs::TsvTable table(test_file_path);
    const auto& row0 = table.getRow(0);
    ASSERT_EQ(3u, row0.size());
    EXPECT_EQ("apple", row0[0]);
    EXPECT_EQ("red", row0[1]);
    EXPECT_EQ("5", row0[2]);

    const auto& row1 = table.getRow(1);
    ASSERT_EQ(3u, row1.size());
    EXPECT_EQ("banana", row1[0]);
    EXPECT_EQ("yellow", row1[1]);
    EXPECT_EQ("3", row1[2]);
}

TEST_F(TsvTableTest, GetRowOutOfBounds) {
    paxs::TsvTable table(test_file_path);
    const auto& row = table.getRow(100);
    EXPECT_TRUE(row.empty());
}

TEST_F(TsvTableTest, ForEachRow) {
    paxs::TsvTable table(test_file_path);
    int count = 0;
    table.forEachRow([&count](std::size_t row_index, const std::vector<std::string>& row) {
        EXPECT_EQ(count, row_index);
        EXPECT_EQ(3u, row.size());
        ++count;
    });
    EXPECT_EQ(3, count);
}

TEST_F(TsvTableTest, Clear) {
    paxs::TsvTable table(test_file_path);
    EXPECT_TRUE(table.isSuccessfullyLoaded());
    EXPECT_EQ(3u, table.rowCount());

    table.clear();
    EXPECT_FALSE(table.isLoaded());
    EXPECT_FALSE(table.isSuccessfullyLoaded());
    EXPECT_EQ(0u, table.rowCount());
    EXPECT_EQ(0u, table.columnCount());
}

TEST_F(TsvTableTest, DuplicateLoad) {
    paxs::TsvTable table(test_file_path);
    EXPECT_TRUE(table.isSuccessfullyLoaded());

    // Try to load again - should be ignored
    EXPECT_TRUE(table.load(test_file_path));
    EXPECT_EQ(3u, table.rowCount());
}

TEST_F(TsvTableTest, GetHeaderKeys) {
    paxs::TsvTable table(test_file_path);
    const auto& keys = table.getHeaderKeys();
    EXPECT_EQ(3u, keys.size());
    EXPECT_EQ(paxs::MurMur3::calcHash("key"), keys[0]);
    EXPECT_EQ(paxs::MurMur3::calcHash("value"), keys[1]);
    EXPECT_EQ(paxs::MurMur3::calcHash("count"), keys[2]);
}

TEST_F(TsvTableTest, GetHeaderMap) {
    paxs::TsvTable table(test_file_path);
    const auto& map = table.getHeaderMap();
    EXPECT_EQ(3u, map.size());
    EXPECT_TRUE(map.find(paxs::MurMur3::calcHash("key")) != map.end());
    EXPECT_TRUE(map.find(paxs::MurMur3::calcHash("value")) != map.end());
    EXPECT_TRUE(map.find(paxs::MurMur3::calcHash("count")) != map.end());
}

// Test with BOM
TEST(TsvTableBOMTest, HandleBOM) {
    std::string test_file = "test_bom.tsv";

    // Create file with UTF-8 BOM
    std::ofstream ofs(test_file, std::ios::binary);
    ofs << "\xEF\xBB\xBF";  // UTF-8 BOM
    ofs << "key\tvalue\n";
    ofs << "test\tdata\n";
    ofs.close();

    paxs::TsvTable table(test_file);
    EXPECT_TRUE(table.isSuccessfullyLoaded());
    EXPECT_EQ(1u, table.rowCount());
    EXPECT_EQ(2u, table.columnCount());
    EXPECT_TRUE(table.hasColumn("key"));
    EXPECT_TRUE(table.hasColumn("value"));
    EXPECT_EQ("test", table.get(0, "key"));

    std::remove(test_file.c_str());
}

// Test with empty lines
TEST(TsvTableEmptyLineTest, SkipEmptyLines) {
    std::string test_file = "test_empty.tsv";

    std::ofstream ofs(test_file);
    ofs << "key\tvalue\n";
    ofs << "test1\tdata1\n";
    ofs << "\n";  // Empty line
    ofs << "test2\tdata2\n";
    ofs.close();

    paxs::TsvTable table(test_file);
    EXPECT_TRUE(table.isSuccessfullyLoaded());
    EXPECT_EQ(2u, table.rowCount());
    EXPECT_EQ("test1", table.get(0, "key"));
    EXPECT_EQ("test2", table.get(1, "key"));

    std::remove(test_file.c_str());
}
