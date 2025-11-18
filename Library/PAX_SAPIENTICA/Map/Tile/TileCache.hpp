/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_TILE_TILE_CACHE_HPP
#define PAX_SAPIENTICA_MAP_TILE_TILE_CACHE_HPP

#include <cstdint>
#include <list>
#include <memory>
#include <optional>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief タイルキャッシュを管理するクラス（LRU方式でサイズ制限あり）
    /// @brief Tile cache management class (with LRU eviction and size limit)
    /// @tparam TextureType テクスチャの型（PAX_GRAPHICAに依存しないため、テンプレート化）
    template<typename TextureType>
    class TileCache {
    public:
        /// @brief キャッシュサイズの上限（デフォルト: 5000タイル = 約1280MB）
        static constexpr std::size_t default_max_cache_size = 5000;

        /// @brief コンストラクタ
        /// @param max_size キャッシュサイズ上限
        explicit TileCache(std::size_t max_size = default_max_cache_size)
            : max_cache_size_(max_size) {}

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
        /// @param texture_ptr テクスチャのunique_ptr
        void insert(std::uint_least64_t key, std::unique_ptr<TextureType>&& texture_ptr) {
            if (!texture_ptr) return;

            // 既存エントリがあれば削除（LRUリストから）
            auto cache_it = cache_map_.find(key);
            if (cache_it != cache_map_.end()) {
                lru_list_.erase(cache_it->second.lru_iterator);
                cache_map_.erase(cache_it);
            }

            // キャッシュサイズ制限チェック（成功エントリのみカウント）
            if (getSuccessfulCacheSize() >= max_cache_size_) {
                evictLRU();
            }

            // LRUリストの先頭に追加
            lru_list_.push_front(key);

            // キャッシュマップに挿入
            CacheEntry entry;
            entry.texture = std::move(*texture_ptr);
            entry.lru_iterator = lru_list_.begin();
            cache_map_[key] = std::move(entry);  // operator[]で挿入または上書き
        }

        /// @brief 失敗を記録（テクスチャなし）
        /// @param key エンコードされたキー
        void insertFailure(std::uint_least64_t key) {
            // 既存エントリがあれば何もしない
            if (cache_map_.find(key) != cache_map_.end()) return;

            // 失敗エントリを挿入（LRUリストには追加しない）
            CacheEntry entry;
            entry.texture = std::nullopt;
            entry.lru_iterator = lru_list_.end();  // 無効なイテレータ
            cache_map_[key] = std::move(entry);  // operator[]で挿入
        }

        /// @brief 試行済みかどうかをチェック
        /// @param key エンコードされたキー
        /// @return 試行済みの場合true（成功・失敗問わず）
        bool hasAttempted(std::uint_least64_t key) const {
            return cache_map_.contains(key);
        }

        /// @brief テクスチャを取得（LRU更新あり）
        /// @param key エンコードされたキー
        /// @return テクスチャへのポインタ（存在しない場合はnullptr）
        const TextureType* getTexture(std::uint_least64_t key) {
            const auto iterator = cache_map_.find(key);
            if (iterator == cache_map_.end()) return nullptr;

            CacheEntry& entry = iterator->second;
            if (!entry.texture.has_value()) return nullptr;

            // LRUリストを更新（最近使用したので先頭に移動）
            if (entry.lru_iterator != lru_list_.end()) {
                lru_list_.erase(entry.lru_iterator);
                lru_list_.push_front(key);
                entry.lru_iterator = lru_list_.begin();
            }

            return &(entry.texture.value());
        }

        /// @brief テクスチャを取得（const版、LRU更新なし）
        /// @param key エンコードされたキー
        /// @return テクスチャへのポインタ（存在しない場合はnullptr）
        const TextureType* getTextureConst(std::uint_least64_t key) const {
            const auto iterator = cache_map_.find(key);
            if (iterator == cache_map_.end()) return nullptr;

            const CacheEntry& entry = iterator->second;
            if (!entry.texture.has_value()) return nullptr;

            return &(entry.texture.value());
        }

        /// @brief 指定座標のテクスチャを取得（const版、LRU更新なし）
        /// @param z ズームレベル
        /// @param y Y座標
        /// @param x X座標
        /// @return テクスチャへのポインタ（存在しない場合はnullptr）
        const TextureType* getTextureAt(
            unsigned int z,
            unsigned int y,
            unsigned int x
        ) const {
            return getTextureConst(encodeKey(z, y, x));
        }

        /// @brief キャッシュをクリア
        void clear() {
            cache_map_.clear();
            lru_list_.clear();
        }

        /// @brief 成功したテクスチャのキャッシュサイズを取得
        /// @return キャッシュに保存されている成功テクスチャ数
        std::size_t size() const {
            return getSuccessfulCacheSize();
        }

        /// @brief 試行済みエントリ数を取得（成功+失敗）
        /// @return 試行済み（成功+失敗）エントリ数
        std::size_t attemptedSize() const {
            return cache_map_.size();
        }

        /// @brief キャッシュサイズ上限を設定
        /// @param max_size 新しい上限値
        void setMaxCacheSize(std::size_t max_size) {
            max_cache_size_ = max_size;
            // 現在のサイズが上限を超えていればLRU削除
            while (getSuccessfulCacheSize() > max_cache_size_) {
                evictLRU();
            }
        }

        /// @brief キャッシュサイズ上限を取得
        /// @return 現在の上限値
        std::size_t getMaxCacheSize() const {
            return max_cache_size_;
        }

    private:
        /// @brief キャッシュエントリ構造体
        struct CacheEntry {
            std::optional<TextureType> texture;  // テクスチャ（失敗時はnullopt）
            std::list<std::uint_least64_t>::iterator lru_iterator;  // LRUリスト内の位置
        };

        /// @brief LRU方式で最も古いエントリを削除
        void evictLRU() {
            if (lru_list_.empty()) return;

            // 最も古いキー（リストの末尾）を取得
            std::uint_least64_t oldest_key = lru_list_.back();
            lru_list_.pop_back();

            // キャッシュマップから削除
            cache_map_.erase(oldest_key);
        }

        /// @brief 成功したテクスチャのキャッシュサイズを取得
        /// @return 成功したテクスチャ数
        std::size_t getSuccessfulCacheSize() const {
            return lru_list_.size();
        }

        /// キャッシュマップ（キー → CacheEntry）
        paxs::UnorderedMap<std::uint_least64_t, CacheEntry> cache_map_;

        /// LRUリスト（最近使用したキーが先頭）
        std::list<std::uint_least64_t> lru_list_;

        /// キャッシュサイズ上限
        std::size_t max_cache_size_;
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_MAP_TILE_TILE_CACHE_HPP
