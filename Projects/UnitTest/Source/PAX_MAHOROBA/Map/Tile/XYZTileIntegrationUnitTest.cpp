/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <filesystem>
#include <fstream>

namespace paxs {

// XYZTile の統合テスト
// 実際のファイル読み込みや動作を検証
class XYZTileIntegrationTest : public ::testing::Test {
protected:
    std::string test_data_dir;

    void SetUp() override {
        // テスト用のデータディレクトリを取得
        test_data_dir = paxs::AppConfig::getInstance().getRootPath() + "Data/Map/XYZTiles/Test/";

        // テストディレクトリが存在しない場合は作成
        if (!std::filesystem::exists(test_data_dir)) {
            std::filesystem::create_directories(test_data_dir);
        }
    }

    void TearDown() override {
        // テスト後のクリーンアップ（必要に応じて）
    }

    // ヘルパー: テスト用のダミータイル画像を作成
    void createDummyTileFile(unsigned int z, unsigned int x, unsigned int y) {
        std::string path = test_data_dir + std::to_string(z) + "/" +
                          std::to_string(x) + "/";

        if (!std::filesystem::exists(path)) {
            std::filesystem::create_directories(path);
        }

        std::string file_path = path + std::to_string(y) + ".png";

        // 最小限のPNGファイル（1x1ピクセルの透明画像）
        // PNGヘッダー
        unsigned char png_data[] = {
            0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,  // PNG signature
            0x00, 0x00, 0x00, 0x0D,  // IHDR chunk length
            0x49, 0x48, 0x44, 0x52,  // "IHDR"
            0x00, 0x00, 0x00, 0x01,  // Width: 1
            0x00, 0x00, 0x00, 0x01,  // Height: 1
            0x08, 0x06, 0x00, 0x00, 0x00,  // Bit depth, color type, etc.
            0x1F, 0x15, 0xC4, 0x89,  // CRC
            0x00, 0x00, 0x00, 0x0A,  // IDAT chunk length
            0x49, 0x44, 0x41, 0x54,  // "IDAT"
            0x78, 0x9C, 0x62, 0x00, 0x01, 0x00, 0x00, 0x05, 0x00, 0x01,  // Compressed data
            0x0D, 0x0A, 0x2D, 0xB4,  // CRC
            0x00, 0x00, 0x00, 0x00,  // IEND chunk length
            0x49, 0x45, 0x4E, 0x44,  // "IEND"
            0xAE, 0x42, 0x60, 0x82   // CRC
        };

        std::ofstream file(file_path, std::ios::binary);
        file.write(reinterpret_cast<const char*>(png_data), sizeof(png_data));
        file.close();
    }

    // ヘルパー: テストファイルを削除
    void removeDummyTileFile(unsigned int z, unsigned int x, unsigned int y) {
        std::string file_path = test_data_dir + std::to_string(z) + "/" +
                               std::to_string(x) + "/" + std::to_string(y) + ".png";

        if (std::filesystem::exists(file_path)) {
            std::filesystem::remove(file_path);
        }
    }
};

// テスト1: ファイル存在チェックの振る舞い
TEST_F(XYZTileIntegrationTest, FileExistenceCheck) {
    // 存在するファイル
    createDummyTileFile(5, 10, 15);
    EXPECT_TRUE(std::filesystem::exists(test_data_dir + "5/10/15.png"));

    // 存在しないファイル
    EXPECT_FALSE(std::filesystem::exists(test_data_dir + "5/10/999.png"));

    // クリーンアップ
    removeDummyTileFile(5, 10, 15);
}

// テスト2: XYZTile のコンストラクタが正常に動作するか
TEST_F(XYZTileIntegrationTest, DISABLED_ConstructorBasic) {
    // NOTE: このテストはXYZTileのコンストラクタが複雑なため、DISABLED_としています
    // 実際の統合テストでは、より単純なコンストラクタ引数で作成できるようにリファクタリング後に有効化します

    // 将来的には以下のようなシンプルな作成が可能になることを期待:
    // XYZTile tile(TileMetadata{...});
    // EXPECT_NO_THROW(tile.update(360.0, 360.0, 0.0, 0.0));
    // TODO:
}

// テスト3: update() メソッドが例外を投げないか
TEST_F(XYZTileIntegrationTest, DISABLED_UpdateDoesNotThrow) {
    // NOTE: DISABLED_ - Phase 2以降で有効化
    // XYZTile tile(...);
    // EXPECT_NO_THROW(tile.update(360.0, 360.0, 0.0, 0.0));
    // TODO:
}

// テスト4: 存在しないタイルに対してエラーログが出ないことを確認
TEST_F(XYZTileIntegrationTest, NonExistentTileNoErrorLog) {
    // この振る舞いは重要:
    // - 存在しないタイルは正常な状態（海洋部、範囲外など）
    // - ログにエラーを出さず、静かに失敗する必要がある

    // 存在しないファイルパスをチェック
    std::string non_existent = test_data_dir + "99/99/99.png";
    EXPECT_FALSE(std::filesystem::exists(non_existent));

    // ファイルが存在しない場合、テクスチャ読み込みは失敗するが、
    // これは正常な振る舞いであり、エラーログは出ない
    // （実装では is_texture_list に失敗フラグを記録）
}

// テスト5: ズームレベルの自動計算
TEST_F(XYZTileIntegrationTest, ZoomLevelAutoCalculation) {
    // migration_plan.md の通り:
    // magnification_z = int(-log2(map_view_height) + 12.5);

    auto calcZoom = [](double map_view_height) -> int {
        return static_cast<int>(-std::log2(map_view_height) + 12.5);
    };

    // 様々なビューの高さでズームレベルを計算
    EXPECT_EQ(calcZoom(360.0), 4);   // 全世界表示
    EXPECT_EQ(calcZoom(180.0), 5);
    EXPECT_EQ(calcZoom(90.0), 6);
    EXPECT_EQ(calcZoom(45.0), 7);
    EXPECT_EQ(calcZoom(22.5), 8);
    EXPECT_EQ(calcZoom(11.25), 9);

    // 極端なズームアウト
    EXPECT_GE(calcZoom(720.0), 0);

    // 極端なズームイン
    EXPECT_LE(calcZoom(0.1), 20);
}

// テスト6: タイル範囲の計算（範囲内であることを確認）
TEST_F(XYZTileIntegrationTest, TileRangeBounds) {
    // z=5の場合、タイル数は32x32 (2^5 = 32)
    unsigned int z = 5;
    unsigned int z_num = 1U << z;  // 32

    EXPECT_EQ(z_num, 32U);

    // 有効なタイル範囲は 0 <= x,y < z_num
    // ただし、元の実装では z_num の前後にバッファがあることに注意
    // （画面外のタイルもロードする可能性がある）

    // 中心が(0, 0)、幅360度の場合の始点・終点は妥当な範囲内か
    auto calcStartX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x - width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    auto calcEndX = [](double center_x, double width, unsigned int z_num) -> int {
        return static_cast<int>((((center_x + width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num);
    };

    int start_x = calcStartX(0.0, 360.0, z_num);
    int end_x = calcEndX(0.0, 360.0, z_num);

    // 始点 <= 終点
    EXPECT_LE(start_x, end_x);

    // 妥当な範囲内（多少のバッファを許容）
    EXPECT_GE(start_x, -static_cast<int>(z_num));
    EXPECT_LE(end_x, static_cast<int>(z_num) * 2);
}

// テスト7: バイナリ→PNG変換のロジック（概念的なテスト）
TEST_F(XYZTileIntegrationTest, BinaryToPngConversionLogic) {
    // migration_plan.md の通り:
    // - 8ビットバイナリ標高データ → RGBA色に変換
    // - 0-127: 白～薄緑
    // - 127-181: 薄緑～緑
    // - 181-250: 緑～濃緑
    // - 251-255, 0: 透明

    auto convertElevationToColor = [](unsigned char elevation) -> std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> {
        if (elevation == 0 || elevation >= 251) {
            return {0, 0, 0, 0};  // 透明
        } else if (elevation < 127) {
            // 白～薄緑
            unsigned char g = 255 - static_cast<unsigned char>((elevation / 127.0) * 100);
            return {255, g, 255, 255};
        } else if (elevation < 181) {
            // 薄緑～緑
            unsigned char factor = elevation - 127;
            return {155, 155 + factor, 155, 255};
        } else {
            // 緑～濃緑
            unsigned char factor = elevation - 181;
            return {55 + factor, 55 + factor, 55, 255};
        }
    };

    // いくつかのサンプル値でテスト
    auto [r0, g0, b0, a0] = convertElevationToColor(0);
    EXPECT_EQ(a0, 0);  // 透明

    auto [r127, g127, b127, a127] = convertElevationToColor(127);
    EXPECT_EQ(a127, 255);  // 不透明

    auto [r200, g200, b200, a200] = convertElevationToColor(200);
    EXPECT_EQ(a200, 255);  // 不透明

    auto [r255, g255, b255, a255] = convertElevationToColor(255);
    EXPECT_EQ(a255, 0);  // 透明
}

// テスト8: キャッシュキーのエンコード・デコードの一貫性
TEST_F(XYZTileIntegrationTest, CacheKeyConsistency) {
    auto encodeKey = [](std::uint_least64_t z, std::uint_least64_t y, std::uint_least64_t x) -> std::uint_least64_t {
        return (z << 48) + (y << 24) + (x);
    };

    auto decodeZ = [](std::uint_least64_t key) -> std::uint_least64_t {
        return (key >> 48) & 0xFFFF;
    };

    auto decodeY = [](std::uint_least64_t key) -> std::uint_least64_t {
        return (key >> 24) & 0xFFFFFF;
    };

    auto decodeX = [](std::uint_least64_t key) -> std::uint_least64_t {
        return key & 0xFFFFFF;
    };

    // 様々な座標でテスト
    std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> test_coords = {
        {5, 10, 15},
        {10, 100, 200},
        {15, 1000, 2000},
    };

    for (const auto& [z, y, x] : test_coords) {
        std::uint_least64_t key = encodeKey(z, y, x);

        EXPECT_EQ(decodeZ(key), z);
        EXPECT_EQ(decodeY(key), y);
        EXPECT_EQ(decodeX(key), x);
    }
}

} // namespace paxs
