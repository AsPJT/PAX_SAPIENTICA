/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_TILE_CACHE_HPP
#define PAX_SAPIENTICA_MAP_TILE_CACHE_HPP

#include <cstdint>

#include <PAX_SAPIENTICA/UnorderedMap.hpp>

namespace paxs {

    /// @brief タイルキャッシュを管理するクラス
    /// @brief Tile cache management class
    /// @tparam TextureType テクスチャの型（PAX_GRAPHICAに依存しないため、テンプレート化）
    template<typename TextureType>
    class TileCache {
    public:
        /// @brief デフォルトコンストラクタ
        TileCache() = default;

        /// @brief キーエンコーディング（z, y, x から 64ビットキーを生成）
        /// @param z ズームレベル（16ビット）
        /// @param y Y座標（24ビット）
        /// @param x X座標（24ビット）
        /// @return エンコードされたキー
        static constexpr std::uint_least64_t encodeKey(
            std::uint_least64_t z,
            std::uint_least64_t y,
            std::uint_least64_t x
        ) {
            return (z << 48) + (y << 24) + (x);
        }

        /// @brief Zのみのキーエンコーディング
        static constexpr std::uint_least64_t encodeKeyZ(std::uint_least64_t z) {
            return (z << 48);
        }

        /// @brief Yのみのキーエンコーディング
        static constexpr std::uint_least64_t encodeKeyY(std::uint_least64_t y) {
            return (y << 24);
        }

        /// @brief Z+Yのキーエンコーディング
        static constexpr std::uint_least64_t encodeKeyZY(
            std::uint_least64_t z,
            std::uint_least64_t y
        ) {
            return (z << 48) + (y << 24);
        }

        /// @brief テクスチャを挿入（成功時）
        /// @param key エンコードされたキー
        /// @param texture テクスチャ（ムーブセマンティクス）
        void insert(std::uint_least64_t key, TextureType&& texture) {
            texture_list_.insert({key, std::move(texture)});
            is_texture_list_.insert({key, 0});  // 0 = 成功
        }

        /// @brief 失敗を記録（テクスチャなし）
        /// @param key エンコードされたキー
        void insertFailure(std::uint_least64_t key) {
            is_texture_list_.insert({key, 1});  // 1 = 失敗
        }

        /// @brief 試行済みかどうかをチェック
        /// @param key エンコードされたキー
        /// @return 試行済みの場合true（成功・失敗問わず）
        bool hasAttempted(std::uint_least64_t key) const {
            return is_texture_list_.find(key) != is_texture_list_.end();
        }

        /// @brief テクスチャを取得
        /// @param key エンコードされたキー
        /// @return テクスチャへのポインタ（存在しない場合はnullptr）
        const TextureType* getTexture(std::uint_least64_t key) const {
            auto it = texture_list_.find(key);
            if (it != texture_list_.end()) {
                return &(it->second);
            }
            return nullptr;
        }

        /// @brief 指定座標のテクスチャを取得
        /// @param z ズームレベル
        /// @param y Y座標
        /// @param x X座標
        /// @return テクスチャへのポインタ（存在しない場合はnullptr）
        const TextureType* getTextureAt(
            unsigned int z,
            unsigned int y,
            unsigned int x
        ) const {
            return getTexture(encodeKey(z, y, x));
        }

        /// @brief キャッシュをクリア
        void clear() {
            texture_list_.clear();
            is_texture_list_.clear();
        }

        /// @brief キャッシュサイズを取得
        /// @return キャッシュに保存されているテクスチャ数
        std::size_t size() const {
            return texture_list_.size();
        }

        /// @brief 試行済みエントリ数を取得
        /// @return 試行済み（成功+失敗）エントリ数
        std::size_t attemptedSize() const {
            return is_texture_list_.size();
        }

    private:
        /// テクスチャのキャッシュ（成功したもののみ）
        paxs::UnorderedMap<std::uint_least64_t, TextureType> texture_list_;

        /// 試行済みフラグ（0=成功、1=失敗）
        paxs::UnorderedMap<std::uint_least64_t, unsigned char> is_texture_list_;
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_MAP_TILE_CACHE_HPP
