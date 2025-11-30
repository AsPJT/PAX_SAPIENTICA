/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_TYPE_UNORDERED_MAP_HPP
#define PAX_SAPIENTICA_CORE_TYPE_UNORDERED_MAP_HPP

#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {
    template <class T>
    concept StreamInsertable = requires(std::ostream& ostream, const T& value) {
        { ostream << value } -> std::same_as<std::ostream&>;
    };

    template <class T>
    std::string to_string_any(const T& value) {
        if constexpr (std::is_arithmetic_v<T>) {
            // int, double など数値なら std::to_string に任せる
            return std::to_string(value);
        } else if constexpr (std::is_same_v<T, std::string>) {
            // すでに std::string ならそのまま返す
            return value;
        } else if constexpr (std::is_same_v<T, const char*>) {
            return std::string(value);
        } else if constexpr (StreamInsertable<T>) {
            // ostream に流せる型ならそれを使う
            std::ostringstream oss;
            oss << value;
            return oss.str();
        } else {
            return "<unprintable type>";
        }
    }

    /// @brief ログ機能を持つ unordered_map のラッパークラス
    /// @tparam Key キーの型
    /// @tparam Value 値の型
    /// @tparam Hash ハッシュ関数の型（デフォルト: std::hash<Key>）
    /// @tparam KeyEqual キー比較関数の型（デフォルト: std::equal_to<Key>）
    template<typename Key, typename Value, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class UnorderedMap {
    private:
        std::unordered_map<Key, Value, Hash, KeyEqual> map_;
        std::string name_; // マップの名前（ログ出力時に使用）

    public:
        // コンストラクタ
        UnorderedMap() : name_("UnnamedMap") {}
        explicit UnorderedMap(std::string  name) : name_(std::move(name)) {}

        // イテレータ型のエイリアス
        using iterator = typename std::unordered_map<Key, Value, Hash, KeyEqual>::iterator;
        using const_iterator = typename std::unordered_map<Key, Value, Hash, KeyEqual>::const_iterator;

        /// @brief 要素を追加
        template<typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args) {
            return map_.emplace(std::forward<Args>(args)...);
        }

        /// @brief 要素を挿入
        std::pair<iterator, bool> insert(const std::pair<Key, Value>& value) {
            return map_.insert(value);
        }

        /// @brief 要素を挿入（move版）
        std::pair<iterator, bool> insert(std::pair<Key, Value>&& value) {
            return map_.insert(std::move(value));
        }

        /// @brief 要素を挿入（存在しない場合のみ）
        constexpr std::pair<iterator, bool> try_emplace(const Key& key, Value&& value) {
            return map_.try_emplace(key, std::move(value));
        }

        /// @brief 要素にアクセス（存在しない場合は例外を投げる）
        Value& at(const Key& key) {
            const auto iterator = map_.find(key);
            if (iterator == map_.end()) {
                PAXS_ERROR("[" + name_ + "] at: key not found (key=" + to_string_any(key) + ")");
                throw std::out_of_range("[" + name_ + "] at: key not found (key=" + to_string_any(key) + ")");
            }
            return iterator->second;
        }

        /// @brief 要素にアクセス（const版）
        const Value& at(const Key& key) const {
            const auto iterator = map_.find(key);
            if (iterator == map_.end()) {
                PAXS_ERROR("[" + name_ + "] at (const): key not found (key=" + to_string_any(key) + ")");
                throw std::out_of_range("[" + name_ + "] at (const): key not found (key=" + to_string_any(key) + ")");
            }
            return iterator->second;
        }

        /// @brief キーが存在すればその値を返し、存在しなければ default_value を返す
        /// @example auto val = map.value_or("key", 0);
        Value value_or(const Key& key, const Value& default_value) const {
            const auto iterator = map_.find(key);
            if (iterator != map_.end()) {
                return iterator->second;
            }
            return default_value;
        }

        /// @brief キーが存在すれば値へのポインタを返し、存在しなければ nullptr を返す
        /// @note ポインタ経由で値を変更可能
        /// @example if (auto* val = map.try_get("key")) { *val = 10; }
        Value* try_get(const Key& key) {
            auto iterator = map_.find(key);
            return (iterator != map_.end()) ? &(iterator->second) : nullptr;
        }

        /// @brief キーが存在すれば値へのポインタを返し、存在しなければ nullptr を返す (const版)
        const Value* try_get(const Key& key) const {
            auto iterator = map_.find(key);
            return (iterator != map_.end()) ? &(iterator->second) : nullptr;
        }

        /// @brief キーが存在するかチェック
        bool contains(const Key& key) const {
            return map_.find(key) != map_.end();
        }


        /// @brief 要素を検索
        iterator find(const Key& key) {
            return map_.find(key);
        }

        /// @brief 要素を検索（const版）
        const_iterator find(const Key& key) const {
            return map_.find(key);
        }

        /// @brief 要素を削除 (キーで削除)
        size_t erase(const Key& key) {
            return map_.erase(key);
        }

        /// @brief 要素を削除 (イテレータで削除)
        iterator erase(const_iterator pos) {
            return map_.erase(pos);
        }

        /// @brief 全要素を削除
        void clear() {
            map_.clear();
        }

        /// @brief 要素数を取得
        size_t size() const noexcept {
            return map_.size();
        }

        /// @brief 指定されたキーを持つ要素の数を取得
        size_t count(const Key& key) const {
            size_t result = map_.count(key);
            return result;
        }

        /// @brief 空かどうか判定
        bool empty() const noexcept {
            return map_.empty();
        }

        /// @brief バケット数を事前予約（リハッシュを回避）
        /// @brief Reserve bucket count (avoid rehashing)
        /// @param count 予約する要素数 / Number of elements to reserve
        void reserve(size_t count) {
            map_.reserve(count);
        }

        /// @brief イテレータの取得
        iterator begin() noexcept { return map_.begin(); }
        const_iterator begin() const noexcept { return map_.begin(); }
        iterator end() noexcept { return map_.end(); }
        const_iterator end() const noexcept { return map_.end(); }

        /// @brief マップ名を設定
        void setName(const std::string& name) {
            name_ = name;
        }

        /// @brief マップ名を取得
        [[nodiscard]] const std::string& getName() const {
            return name_;
        }

        /// @brief 内部の unordered_map への const 参照を取得
        const std::unordered_map<Key, Value>& getInternalMap() const {
            return map_;
        }
    };
}

#endif // !PAX_SAPIENTICA_CORE_TYPE_UNORDERED_MAP_HPP
