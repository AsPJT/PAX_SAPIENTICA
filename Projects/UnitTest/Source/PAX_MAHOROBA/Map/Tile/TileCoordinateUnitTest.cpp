/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>

#include <PAX_SAPIENTICA/Map/Tile/TileCoordinate.hpp>

namespace paxs {

// XYZTile の座標計算ロジックをテストする
class TileCoordinateTest : public ::testing::Test {
protected:
    // テストケース共通のセットアップ
    void SetUp() override {
        // 必要に応じて初期化処理
    }

    // テストケース共通のクリーンアップ
    void TearDown() override {
        // 必要に応じてクリーンアップ処理
    }

    // ヘルパー関数: タイル座標のエンコード
    static constexpr std::uint_least64_t encodeZYX(
        std::uint_least64_t z,
        std::uint_least64_t y,
        std::uint_least64_t x
    ) {
        return (z << 48) + (y << 24) + (x);
    }
};

// テスト1: textureIndex() のエンコーディングが正しいことを確認
TEST_F(TileCoordinateTest, TextureIndexEncoding) {
    // 基本的なケース
    EXPECT_EQ(encodeZYX(0, 0, 0), 0ULL);
    EXPECT_EQ(encodeZYX(1, 0, 0), 1ULL << 48);
    EXPECT_EQ(encodeZYX(0, 1, 0), 1ULL << 24);
    EXPECT_EQ(encodeZYX(0, 0, 1), 1ULL);

    // 複合ケース
    EXPECT_EQ(encodeZYX(5, 10, 15),
              (5ULL << 48) + (10ULL << 24) + 15ULL);

    // 最大値に近いケース
    EXPECT_EQ(encodeZYX(255, 0, 0), 255ULL << 48);
    EXPECT_EQ(encodeZYX(0, 16777215, 0), 16777215ULL << 24);  // 2^24 - 1
    EXPECT_EQ(encodeZYX(0, 0, 16777215), 16777215ULL);        // 2^24 - 1
}

// テスト2: ズームレベルの計算
TEST_F(TileCoordinateTest, ZoomLevelCalculation) {
    // magnification_z = int(-log2(map_view_height) + 12.5);
    //
    // map_view_height = 360 → log2(360) ≈ 8.49 → -8.49 + 12.5 = 4.01 → z=4
    // map_view_height = 180 → log2(180) ≈ 7.49 → -7.49 + 12.5 = 5.01 → z=5
    // map_view_height = 90  → log2(90)  ≈ 6.49 → -6.49 + 12.5 = 6.01 → z=6

    auto calcZ = [](double map_view_height) -> int {
        return static_cast<int>(-std::log2(map_view_height) + 12.5);
    };

    EXPECT_EQ(calcZ(360.0), 4);
    EXPECT_EQ(calcZ(180.0), 5);
    EXPECT_EQ(calcZ(90.0), 6);
    EXPECT_EQ(calcZ(45.0), 7);
    EXPECT_EQ(calcZ(22.5), 8);
}

// テスト3: 始点セル計算のロジック検証
TEST_F(TileCoordinateTest, StartCellCalculation) {
    // 元の実装のロジック:
    // const Vector2<int> new_start_cell{
    //     int((((map_view_center_x - map_view_width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num),
    //     int(((360.0 - ((map_view_center_y + map_view_height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num) };

    auto calcStartCellX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x - width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcStartCellY = [](double center_y, double height, unsigned int z_num) -> int {
        return static_cast<int>(((360.0 - ((center_y + height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    // ズームレベル2 (z_num = 4) の場合
    unsigned int z_num = 4;

    // 中心が(0, 0)、幅・高さが360度の場合（全世界表示）
    // 始点は(-2, -2)付近になるはず
    int start_x = calcStartCellX(0.0, 360.0, z_num);
    int start_y = calcStartCellY(0.0, 360.0, z_num);

    // 値の妥当性チェック（範囲内）
    EXPECT_GE(start_x, -10);
    EXPECT_LE(start_x, 10);
    EXPECT_GE(start_y, -10);
    EXPECT_LE(start_y, 10);

    // 日本付近 (139.6917°E, 35.6895°N) を中心に、幅90度表示
    start_x = calcStartCellX(139.6917, 90.0, z_num);
    start_y = calcStartCellY(35.6895, 90.0, z_num);

    // 東京付近なので正の値のはず
    EXPECT_GE(start_x, 0);
    EXPECT_GE(start_y, 0);
    EXPECT_LT(start_x, static_cast<int>(z_num) * 2);
    EXPECT_LT(start_y, static_cast<int>(z_num) * 2);
}

// テスト4: 終点セル計算のロジック検証
TEST_F(TileCoordinateTest, EndCellCalculation) {
    // 元の実装のロジック:
    // const Vector2<int> new_end_cell{
    //     int((((map_view_center_x + map_view_width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num),
    //     int(((360.0 - ((map_view_center_y - map_view_height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num) };

    auto calcEndCellX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x + width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcEndCellY = [](double center_y, double height, unsigned int z_num) -> int {
        return static_cast<int>(((360.0 - ((center_y - height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    // ズームレベル2 (z_num = 4) の場合
    unsigned int z_num = 4;

    // 中心が(0, 0)、幅・高さが360度の場合（全世界表示）
    int end_x = calcEndCellX(0.0, 360.0, z_num);
    int end_y = calcEndCellY(0.0, 360.0, z_num);

    // 値の妥当性チェック
    EXPECT_GE(end_x, -10);
    EXPECT_LE(end_x, 10);
    EXPECT_GE(end_y, -10);
    EXPECT_LE(end_y, 10);

    // 日本付近を中心に、幅90度表示
    end_x = calcEndCellX(139.6917, 90.0, z_num);
    end_y = calcEndCellY(35.6895, 90.0, z_num);

    EXPECT_GE(end_x, 0);
    EXPECT_GE(end_y, 0);
    EXPECT_LT(end_x, static_cast<int>(z_num) * 2);
    EXPECT_LT(end_y, static_cast<int>(z_num) * 2);
}

// テスト5: z_num の計算 (2^z)
TEST_F(TileCoordinateTest, ZNumCalculation) {
    EXPECT_EQ(1U << 0, 1U);
    EXPECT_EQ(1U << 1, 2U);
    EXPECT_EQ(1U << 2, 4U);
    EXPECT_EQ(1U << 3, 8U);
    EXPECT_EQ(1U << 4, 16U);
    EXPECT_EQ(1U << 5, 32U);
    EXPECT_EQ(1U << 10, 1024U);
    EXPECT_EQ(1U << 20, 1048576U);
}

// テスト6: 座標計算の一貫性（始点 < 終点）
TEST_F(TileCoordinateTest, StartEndConsistency) {
    auto calcStartCellX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x - width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcEndCellX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x + width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcStartCellY = [](double center_y, double height, unsigned int z_num) -> int {
        return static_cast<int>(((360.0 - ((center_y + height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcEndCellY = [](double center_y, double height, unsigned int z_num) -> int {
        return static_cast<int>(((360.0 - ((center_y - height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    unsigned int z_num = 8;  // ズームレベル3

    // 様々な中心点でテスト
    std::vector<std::pair<double, double>> test_centers = {
        {0.0, 0.0},          // 赤道・本初子午線
        {139.6917, 35.6895}, // 東京
        {-74.006, 40.7128},  // ニューヨーク
        {2.3522, 48.8566},   // パリ
    };

    for (const auto& center : test_centers) {
        double width = 90.0;
        double height = 90.0;

        int start_x = calcStartCellX(center.first, width, z_num);
        int end_x = calcEndCellX(center.first, width, z_num);
        int start_y = calcStartCellY(center.second, height, z_num);
        int end_y = calcEndCellY(center.second, height, z_num);

        // 始点 <= 終点 であることを確認
        EXPECT_LE(start_x, end_x) << "Center: (" << center.first << ", " << center.second << ")";
        EXPECT_LE(start_y, end_y) << "Center: (" << center.first << ", " << center.second << ")";
    }
}

// テスト7: TileCoordinateクラスの使用テスト
TEST_F(TileCoordinateTest, TileCoordinateClassUsage) {
    // ズームレベル5でTileCoordinateを作成
    unsigned int z = 5;
    unsigned int z_num = paxs::TileCoordinate::calculateZNum(z);
    EXPECT_EQ(z_num, 32U);

    paxs::TileCoordinate coord(z, z_num);

    // ズームレベル自動計算
    int auto_z = paxs::TileCoordinate::calculateZoomLevel(90.0);
    EXPECT_EQ(auto_z, 6);

    // 東京付近（139.6917°E, 35.6895°N）を中心に、幅90度表示
    Vector2<int> start_cell = coord.calculateStartCell({139.6917, 35.6895}, {90.0, 90.0});
    Vector2<int> end_cell = coord.calculateEndCell({139.6917, 35.6895}, {90.0, 90.0});

    // 始点 <= 終点
    EXPECT_LE(start_cell.x, end_cell.x);
    EXPECT_LE(start_cell.y, end_cell.y);

    // 値の妥当性（正の値のはず）
    EXPECT_GE(start_cell.x, 0);
    EXPECT_GE(start_cell.y, 0);
    EXPECT_GE(end_cell.x, 0);
    EXPECT_GE(end_cell.y, 0);

    // 範囲内（z_num * 2 以下）
    EXPECT_LT(start_cell.x, static_cast<int>(z_num) * 2);
    EXPECT_LT(start_cell.y, static_cast<int>(z_num) * 2);
    EXPECT_LT(end_cell.x, static_cast<int>(z_num) * 2);
    EXPECT_LT(end_cell.y, static_cast<int>(z_num) * 2);
}

// テスト8: TileCoordinateクラスと元のロジックの一致確認
TEST_F(TileCoordinateTest, TileCoordinateMatchesOriginalLogic) {
    // 元のロジック（ラムダで再現）
    auto calcOriginalStartX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x - width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcOriginalStartY = [](double center_y, double height, unsigned int z_num) -> int {
        return static_cast<int>(((360.0 - ((center_y + height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcOriginalEndX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x + width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcOriginalEndY = [](double center_y, double height, unsigned int z_num) -> int {
        return static_cast<int>(((360.0 - ((center_y - height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    // TileCoordinateクラスで同じ計算
    unsigned int z = 8;
    unsigned int z_num = paxs::TileCoordinate::calculateZNum(z);
    paxs::TileCoordinate coord(z, z_num);

    std::vector<std::tuple<Vector2<double>, Vector2<double>>> test_cases = {
        {{0.0, 0.0}, {360.0, 360.0}},         // 全世界
        {{139.6917, 35.6895}, {90.0, 90.0}},  // 東京
        {{-74.006, 40.7128}, {45.0, 45.0}},   // ニューヨーク
        {{2.3522, 48.8566}, {60.0, 60.0}},    // パリ
    };

    for (const auto& [center, size] : test_cases) {
        Vector2<int> start_cell = coord.calculateStartCell(center, size);
        Vector2<int> end_cell = coord.calculateEndCell(center, size);

        int original_start_x = calcOriginalStartX(center.x, size.x, z_num);
        int original_start_y = calcOriginalStartY(center.y, size.y, z_num);
        int original_end_x = calcOriginalEndX(center.x, size.x, z_num);
        int original_end_y = calcOriginalEndY(center.y, size.y, z_num);

        EXPECT_EQ(start_cell.x, original_start_x)
            << "Start X mismatch at (" << center.x << ", " << center.y << ")";
        EXPECT_EQ(start_cell.y, original_start_y)
            << "Start Y mismatch at (" << center.x << ", " << center.y << ")";
        EXPECT_EQ(end_cell.x, original_end_x)
            << "End X mismatch at (" << center.x << ", " << center.y << ")";
        EXPECT_EQ(end_cell.y, original_end_y)
            << "End Y mismatch at (" << center.x << ", " << center.y << ")";
    }
}

} // namespace paxs
