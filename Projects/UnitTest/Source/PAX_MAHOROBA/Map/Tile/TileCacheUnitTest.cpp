/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>
#include <PAX_SAPIENTICA/Map/Tile/TileCache.hpp>

namespace paxs {

// ダミーテクスチャ型（テスト用）
struct DummyTexture {
    int id;
    DummyTexture() : id(0) {}
    explicit DummyTexture(int id_) : id(id_) {}
    DummyTexture(DummyTexture&&) = default;
    DummyTexture& operator=(DummyTexture&&) = default;
};

// XYZTile のキャッシュ管理ロジックをテストする
class TileCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 必要に応じて初期化処理
    }

    void TearDown() override {
        // 必要に応じてクリーンアップ処理
    }

    // ヘルパー: キーエンコーディング
    static constexpr std::uint_least64_t encodeKey(
        std::uint_least64_t z,
        std::uint_least64_t y,
        std::uint_least64_t x
    ) {
        return (z << 48) + (y << 24) + (x);
    }

    // ヘルパー: キーデコーディング
    struct DecodedKey {
        std::uint_least64_t z;
        std::uint_least64_t y;
        std::uint_least64_t x;
    };

    static DecodedKey decodeKey(std::uint_least64_t key) {
        return DecodedKey{
            (key >> 48) & 0xFFFF,
            (key >> 24) & 0xFFFFFF,
            key & 0xFFFFFF
        };
    }
};

// テスト1: キーエンコーディングの正確性
TEST_F(TileCacheTest, KeyEncoding) {
    // 基本ケース
    EXPECT_EQ(encodeKey(0, 0, 0), 0ULL);
    EXPECT_EQ(encodeKey(1, 0, 0), 1ULL << 48);
    EXPECT_EQ(encodeKey(0, 1, 0), 1ULL << 24);
    EXPECT_EQ(encodeKey(0, 0, 1), 1ULL);

    // 複合ケース
    std::uint_least64_t key = encodeKey(5, 10, 15);
    EXPECT_EQ(key, (5ULL << 48) + (10ULL << 24) + 15ULL);

    // ユニーク性の確認
    EXPECT_NE(encodeKey(1, 2, 3), encodeKey(1, 2, 4));
    EXPECT_NE(encodeKey(1, 2, 3), encodeKey(1, 3, 3));
    EXPECT_NE(encodeKey(1, 2, 3), encodeKey(2, 2, 3));
}

// テスト2: キーデコーディングの正確性
TEST_F(TileCacheTest, KeyDecoding) {
    // エンコード→デコードで元の値が復元されるか
    std::vector<std::tuple<std::uint_least64_t, std::uint_least64_t, std::uint_least64_t>> test_cases = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {5, 10, 15},
        {255, 1000, 2000},
        {10, 5000, 3000}
    };

    for (const auto& [z, y, x] : test_cases) {
        std::uint_least64_t key = encodeKey(z, y, x);
        DecodedKey decoded = decodeKey(key);

        EXPECT_EQ(decoded.z, z) << "Z mismatch for input (" << z << ", " << y << ", " << x << ")";
        EXPECT_EQ(decoded.y, y) << "Y mismatch for input (" << z << ", " << y << ", " << x << ")";
        EXPECT_EQ(decoded.x, x) << "X mismatch for input (" << z << ", " << y << ", " << x << ")";
    }
}

// テスト3: キャッシュキーの範囲チェック
TEST_F(TileCacheTest, KeyRangeBounds) {
    // 各コンポーネントの最大値
    // Z: 16ビット (0-65535)
    // Y: 24ビット (0-16777215)
    // X: 24ビット (0-16777215)

    // Z の最大値
    std::uint_least64_t max_z_key = encodeKey(65535, 0, 0);
    EXPECT_EQ((max_z_key >> 48) & 0xFFFF, 65535ULL);

    // Y の最大値
    std::uint_least64_t max_y_key = encodeKey(0, 16777215, 0);
    EXPECT_EQ((max_y_key >> 24) & 0xFFFFFF, 16777215ULL);

    // X の最大値
    std::uint_least64_t max_x_key = encodeKey(0, 0, 16777215);
    EXPECT_EQ(max_x_key & 0xFFFFFF, 16777215ULL);

    // 全て最大値
    std::uint_least64_t max_key = encodeKey(65535, 16777215, 16777215);
    DecodedKey decoded = decodeKey(max_key);
    EXPECT_EQ(decoded.z, 65535ULL);
    EXPECT_EQ(decoded.y, 16777215ULL);
    EXPECT_EQ(decoded.x, 16777215ULL);
}

// テスト4: 実際のタイル座標でのキーエンコーディング
TEST_F(TileCacheTest, RealWorldTileCoordinates) {
    // ズームレベル5で東京付近のタイル
    // Z=5, Y=11, X=28 (おおよその東京)
    std::uint_least64_t tokyo_key = encodeKey(5, 11, 28);
    DecodedKey decoded = decodeKey(tokyo_key);

    EXPECT_EQ(decoded.z, 5ULL);
    EXPECT_EQ(decoded.y, 11ULL);
    EXPECT_EQ(decoded.x, 28ULL);

    // ズームレベル10でニューヨーク付近
    // Z=10, Y=384, X=301
    std::uint_least64_t ny_key = encodeKey(10, 384, 301);
    decoded = decodeKey(ny_key);

    EXPECT_EQ(decoded.z, 10ULL);
    EXPECT_EQ(decoded.y, 384ULL);
    EXPECT_EQ(decoded.x, 301ULL);
}

// テスト5: キャッシュのヒット/ミス判定ロジック
TEST_F(TileCacheTest, CacheHitMissLogic) {
    // 元の実装のロジック:
    // if (is_texture_list.find(index_zyx) != is_texture_list.end()) {
    //     continue;  // 既に試行済み（キャッシュヒット）
    // }

    paxs::UnorderedMap<std::uint_least64_t, unsigned char> is_texture_list;

    std::uint_least64_t key1 = encodeKey(5, 10, 15);
    std::uint_least64_t key2 = encodeKey(5, 10, 16);

    // 初期状態: どちらも未登録
    EXPECT_TRUE(is_texture_list.find(key1) == is_texture_list.end());
    EXPECT_TRUE(is_texture_list.find(key2) == is_texture_list.end());

    // key1 を成功として登録
    is_texture_list.insert({key1, 0});

    // key1 は登録済み、key2 は未登録
    EXPECT_FALSE(is_texture_list.find(key1) == is_texture_list.end());
    EXPECT_TRUE(is_texture_list.find(key2) == is_texture_list.end());

    // key2 を失敗として登録
    is_texture_list.insert({key2, 1});

    // 両方とも登録済み
    EXPECT_FALSE(is_texture_list.find(key1) == is_texture_list.end());
    EXPECT_FALSE(is_texture_list.find(key2) == is_texture_list.end());

    // 値の確認
    EXPECT_EQ(is_texture_list.at(key1), 0);  // 成功
    EXPECT_EQ(is_texture_list.at(key2), 1);  // 失敗
}

// テスト6: キャッシュの状態フラグ（0=成功, 1=失敗）
TEST_F(TileCacheTest, CacheStatusFlags) {
    paxs::UnorderedMap<std::uint_least64_t, unsigned char> is_texture_list;

    std::uint_least64_t success_key = encodeKey(1, 2, 3);
    std::uint_least64_t failure_key = encodeKey(4, 5, 6);

    // 成功を記録
    is_texture_list.insert({success_key, 0});
    EXPECT_EQ(is_texture_list.at(success_key), 0);

    // 失敗を記録
    is_texture_list.insert({failure_key, 1});
    EXPECT_EQ(is_texture_list.at(failure_key), 1);

    // 重複登録の防止確認
    // UnorderedMapは既存のキーへのinsertを無視する
    is_texture_list.insert({success_key, 1});  // 上書きしようとする
    // insertは既存キーには何もしないので、値は0のまま
    EXPECT_EQ(is_texture_list.at(success_key), 0);
}

// テスト7: 大量のキーでの衝突チェック
TEST_F(TileCacheTest, MassiveKeyCollisionCheck) {
    paxs::UnorderedMap<std::uint_least64_t, unsigned char> cache;

    // ズームレベル5で100x100のタイルを登録
    const unsigned int z = 5;
    for (unsigned int y = 0; y < 100; ++y) {
        for (unsigned int x = 0; x < 100; ++x) {
            std::uint_least64_t key = encodeKey(z, y, x);
            cache.insert({key, 0});
        }
    }

    // 全て登録されたか確認
    EXPECT_EQ(cache.size(), 10000ULL);

    // ランダムにアクセスして確認
    EXPECT_TRUE(cache.find(encodeKey(z, 50, 50)) != cache.end());
    EXPECT_TRUE(cache.find(encodeKey(z, 99, 99)) != cache.end());
    EXPECT_TRUE(cache.find(encodeKey(z, 0, 0)) != cache.end());

    // 未登録のキーはヒットしない
    EXPECT_TRUE(cache.find(encodeKey(z, 100, 100)) == cache.end());
    EXPECT_TRUE(cache.find(encodeKey(6, 0, 0)) == cache.end());
}

// テスト8: キャッシュのクリア動作
TEST_F(TileCacheTest, CacheClearBehavior) {
    paxs::UnorderedMap<std::uint_least64_t, unsigned char> cache;

    // いくつかのキーを登録
    cache.insert({encodeKey(1, 2, 3), 0});
    cache.insert({encodeKey(4, 5, 6), 1});
    cache.insert({encodeKey(7, 8, 9), 0});

    EXPECT_EQ(cache.size(), 3ULL);

    // クリア
    cache.clear();

    EXPECT_EQ(cache.size(), 0ULL);
    EXPECT_TRUE(cache.find(encodeKey(1, 2, 3)) == cache.end());
}

// テスト9: TileCacheクラスの基本機能
TEST_F(TileCacheTest, TileCacheClassBasicFunctionality) {
    paxs::TileCache<DummyTexture> cache;

    // 初期状態
    EXPECT_EQ(cache.size(), 0ULL);
    EXPECT_EQ(cache.attemptedSize(), 0ULL);

    // テクスチャを挿入（成功）
    std::uint_least64_t key1 = paxs::TileCache<DummyTexture>::encodeKey(5, 10, 15);
    auto tex1 = std::make_unique<DummyTexture>(100);
    cache.insert(key1, std::move(tex1));

    EXPECT_EQ(cache.size(), 1ULL);
    EXPECT_EQ(cache.attemptedSize(), 1ULL);
    EXPECT_TRUE(cache.hasAttempted(key1));

    // テクスチャを取得
    const DummyTexture* retrieved = cache.getTexture(key1);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->id, 100);

    // 失敗を記録
    std::uint_least64_t key2 = paxs::TileCache<DummyTexture>::encodeKey(5, 10, 16);
    cache.insertFailure(key2);

    EXPECT_EQ(cache.size(), 1ULL);  // 失敗したのでテクスチャは保存されない
    EXPECT_EQ(cache.attemptedSize(), 2ULL);  // 試行済みフラグは記録される
    EXPECT_TRUE(cache.hasAttempted(key2));
    EXPECT_EQ(cache.getTexture(key2), nullptr);  // テクスチャは存在しない
}

// テスト10: TileCacheのキーエンコーディング
TEST_F(TileCacheTest, TileCacheKeyEncoding) {
    // encodeKey()の検証
    EXPECT_EQ(paxs::TileCache<DummyTexture>::encodeKey(0, 0, 0), 0ULL);
    EXPECT_EQ(paxs::TileCache<DummyTexture>::encodeKey(1, 0, 0), 1ULL << 48);
    EXPECT_EQ(paxs::TileCache<DummyTexture>::encodeKey(0, 1, 0), 1ULL << 24);
    EXPECT_EQ(paxs::TileCache<DummyTexture>::encodeKey(0, 0, 1), 1ULL);

    // encodeKeyZ, encodeKeyY, encodeKeyZY の検証
    EXPECT_EQ(paxs::TileCache<DummyTexture>::encodeKeyZ(5), 5ULL << 48);
    EXPECT_EQ(paxs::TileCache<DummyTexture>::encodeKeyY(10), 10ULL << 24);
    EXPECT_EQ(paxs::TileCache<DummyTexture>::encodeKeyZY(5, 10),
              (5ULL << 48) + (10ULL << 24));
}

// テスト11: TileCacheのgetTextureAt
TEST_F(TileCacheTest, TileCacheGetTextureAt) {
    paxs::TileCache<DummyTexture> cache;

    // テクスチャを挿入
    unsigned int z = 5, y = 10, x = 15;
    auto tex = std::make_unique<DummyTexture>(999);
    std::uint_least64_t key = paxs::TileCache<DummyTexture>::encodeKey(z, y, x);
    cache.insert(key, std::move(tex));

    // getTextureAt()で取得
    const DummyTexture* retrieved = cache.getTextureAt(z, y, x);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->id, 999);

    // 存在しない座標
    EXPECT_EQ(cache.getTextureAt(z, y, x + 1), nullptr);
}

// テスト12: TileCacheのクリア
TEST_F(TileCacheTest, TileCacheClear) {
    paxs::TileCache<DummyTexture> cache;

    // 複数のテクスチャを挿入
    for (unsigned int i = 0; i < 10; ++i) {
        std::uint_least64_t key = paxs::TileCache<DummyTexture>::encodeKey(5, i, i);
        auto tex = std::make_unique<DummyTexture>(static_cast<int>(i));
        cache.insert(key, std::move(tex));
    }

    EXPECT_EQ(cache.size(), 10ULL);
    EXPECT_EQ(cache.attemptedSize(), 10ULL);

    // クリア
    cache.clear();

    EXPECT_EQ(cache.size(), 0ULL);
    EXPECT_EQ(cache.attemptedSize(), 0ULL);
    EXPECT_FALSE(cache.hasAttempted(paxs::TileCache<DummyTexture>::encodeKey(5, 0, 0)));
}

} // namespace paxs
