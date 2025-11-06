/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_UNORDERED_MAP_HPP
#define PAX_SAPIENTICA_UNORDERED_MAP_HPP

#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {
    template <class T>
    concept StreamInsertable = requires(std::ostream& os, const T& value) {
        { os << value } -> std::same_as<std::ostream&>;
    };

    template <class T>
    std::string to_string_any(const T& v) {
        if constexpr (std::is_arithmetic_v<T>) {
            // int, double など数値なら std::to_string に任せる
            return std::to_string(v);
        } else if constexpr (std::is_same_v<T, std::string>) {
            // すでに std::string ならそのまま返す
            return v;
        } else if constexpr (std::is_same_v<T, const char*>) {
            return std::string(v);
        } else if constexpr (StreamInsertable<T>) {
            // ostream に流せる型ならそれを使う
            std::ostringstream oss;
            oss << v;
            return oss.str();
        } else {
            return "<unprintable type>";
        }
    }

    /// @brief ログ機能を持つ unordered_map のラッパークラス
    /// @tparam Key キーの型
    /// @tparam Value 値の型
    template<typename Key, typename Value>
    class UnorderedMap {
    private:
        std::unordered_map<Key, Value> map_;
        std::string name_; // マップの名前（ログ出力時に使用）
    public:
        // コンストラクタ
        UnorderedMap() : name_("UnnamedMap") {}
        explicit UnorderedMap(const std::string& name) : name_(name) {}

        // イテレータ型のエイリアス
        using iterator = typename std::unordered_map<Key, Value>::iterator;
        using const_iterator = typename std::unordered_map<Key, Value>::const_iterator;

        /// @brief 要素を追加
        template<typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args) {
            auto result = map_.emplace(std::forward<Args>(args)...);
            return result;
        }

        /// @brief 要素を挿入
        std::pair<iterator, bool> insert(const std::pair<Key, Value>& value) {
            auto result = map_.insert(value);
            return result;
        }

        /// @brief 要素を挿入（move版）
        std::pair<iterator, bool> insert(std::pair<Key, Value>&& value) {
            auto result = map_.insert(std::move(value));
            return result;
        }

        /// @brief 要素にアクセス（存在しない場合は例外を投げる）
        Value& at(const Key& key) {
            if (map_.find(key) == map_.end()) {
                PAXS_ERROR("[" + name_ + "] at: key not found (key=" + to_string_any(key) + ")");
            }
            return map_.at(key);
        }

        /// @brief 要素にアクセス（const版）
        const Value& at(const Key& key) const {
            if (map_.find(key) == map_.end()) {
                PAXS_ERROR("[" + name_ + "] at (const): key not found (key=" + to_string_any(key) + ")");
            }
            return map_.at(key);
        }

        /// @brief 要素にアクセス（存在しない場合はデフォルト構築）
        Value& operator[](const Key& key) {
            bool exists = (map_.find(key) != map_.end());
            return map_[key];
        }

        /// @brief キーが存在するかチェック
        bool contains(const Key& key) const {
            bool result = (map_.find(key) != map_.end());
            return result;
        }


        /// @brief 要素を検索
        iterator find(const Key& key) {
            auto it = map_.find(key);
            return it;
        }

        /// @brief 要素を検索（const版）
        const_iterator find(const Key& key) const {
            auto it = map_.find(key);
            return it;
        }

        /// @brief 要素を削除 (キーで削除)
        size_t erase(const Key& key) {
            size_t count = map_.erase(key);
            return count;
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
        const std::string& getName() const {
            return name_;
        }

        /// @brief 内部の unordered_map への参照を取得（必要な場合のみ使用）
        std::unordered_map<Key, Value>& getInternalMap() {
            return map_;
        }

        /// @brief 内部の unordered_map への const 参照を取得
        const std::unordered_map<Key, Value>& getInternalMap() const {
            return map_;
        }
    };
}

#endif // !PAX_SAPIENTICA_UNORDERED_MAP_HPP
