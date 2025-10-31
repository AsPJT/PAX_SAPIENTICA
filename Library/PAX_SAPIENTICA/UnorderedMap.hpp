/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_UNORDERED_MAP_HPP
#define PAX_SAPIENTICA_UNORDERED_MAP_HPP

/*##########################################################################################
	ログ機能を持つ unordered_map のラッパークラス

	デバッグ時にログを有効化することで、辞書へのアクセスを追跡できます。
	PAXS_ENABLE_MAP_LOGGING マクロを定義することでログが有効になります。
##########################################################################################*/

#include <string>
#include <unordered_map>

#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

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
#ifdef PAXS_ENABLE_MAP_LOGGING
			auto result = map_.emplace(std::forward<Args>(args)...);
			if (result.second) {
				PAXS_INFO("[" + name_ + "] emplace: key=" + std::to_string(result.first->first));
			}
			return result;
#else
			return map_.emplace(std::forward<Args>(args)...);
#endif
		}

		/// @brief 要素を挿入
		std::pair<iterator, bool> insert(const std::pair<Key, Value>& value) {
#ifdef PAXS_ENABLE_MAP_LOGGING
			auto result = map_.insert(value);
			if (result.second) {
				PAXS_INFO("[" + name_ + "] insert: key=" + std::to_string(value.first));
			}
			return result;
#else
			return map_.insert(value);
#endif
		}

		/// @brief 要素を挿入（move版）
		std::pair<iterator, bool> insert(std::pair<Key, Value>&& value) {
#ifdef PAXS_ENABLE_MAP_LOGGING
			auto result = map_.insert(std::move(value));
			if (result.second) {
				PAXS_INFO("[" + name_ + "] insert (move): key=" + std::to_string(result.first->first));
			}
			return result;
#else
			return map_.insert(std::move(value));
#endif
		}

		/// @brief 要素にアクセス（存在しない場合は例外を投げる）
		Value& at(const Key& key) {
#ifdef PAXS_ENABLE_MAP_LOGGING
			if (map_.find(key) == map_.end()) {
				PAXS_ERROR("[" + name_ + "] at: key not found (key=" + std::to_string(key) + ")");
			} else {
				PAXS_INFO("[" + name_ + "] at: accessing key=" + std::to_string(key));
			}
#endif
			return map_.at(key);
		}

		/// @brief 要素にアクセス（const版）
		const Value& at(const Key& key) const {
#ifdef PAXS_ENABLE_MAP_LOGGING
			if (map_.find(key) == map_.end()) {
				PAXS_ERROR("[" + name_ + "] at (const): key not found (key=" + std::to_string(key) + ")");
			} else {
				PAXS_INFO("[" + name_ + "] at (const): accessing key=" + std::to_string(key));
			}
#endif
			return map_.at(key);
		}

		/// @brief 要素にアクセス（存在しない場合はデフォルト構築）
		Value& operator[](const Key& key) {
#ifdef PAXS_ENABLE_MAP_LOGGING
			bool exists = (map_.find(key) != map_.end());
			if (!exists) {
				PAXS_INFO("[" + name_ + "] operator[]: creating new entry for key=" + std::to_string(key));
			} else {
				PAXS_INFO("[" + name_ + "] operator[]: accessing key=" + std::to_string(key));
			}
#endif
			return map_[key];
		}

		/// @brief キーが存在するかチェック
		bool contains(const Key& key) const {
#ifdef PAXS_ENABLE_MAP_LOGGING
			bool result = (map_.find(key) != map_.end());
			PAXS_INFO("[" + name_ + "] contains: key=" + std::to_string(key) + " -> " + (result ? "true" : "false"));
			return result;
#else
			return (map_.find(key) != map_.end());
#endif
		}

		/// @brief 要素を検索
		iterator find(const Key& key) {
#ifdef PAXS_ENABLE_MAP_LOGGING
			auto it = map_.find(key);
			PAXS_INFO("[" + name_ + "] find: key=" + std::to_string(key) + " -> " + (it != map_.end() ? "found" : "not found"));
			return it;
#else
			return map_.find(key);
#endif
		}

		/// @brief 要素を検索（const版）
		const_iterator find(const Key& key) const {
#ifdef PAXS_ENABLE_MAP_LOGGING
			auto it = map_.find(key);
			PAXS_INFO("[" + name_ + "] find (const): key=" + std::to_string(key) + " -> " + (it != map_.end() ? "found" : "not found"));
			return it;
#else
			return map_.find(key);
#endif
		}

		/// @brief 要素を削除 (キーで削除)
		size_t erase(const Key& key) {
#ifdef PAXS_ENABLE_MAP_LOGGING
			size_t count = map_.erase(key);
			PAXS_INFO("[" + name_ + "] erase: key=" + std::to_string(key) + " -> " + (count > 0 ? "erased" : "not found"));
			return count;
#else
			return map_.erase(key);
#endif
		}

		/// @brief 要素を削除 (イテレータで削除)
		iterator erase(const_iterator pos) {
#ifdef PAXS_ENABLE_MAP_LOGGING
			PAXS_INFO("[" + name_ + "] erase: erasing element at iterator position");
#endif
			return map_.erase(pos);
		}

		/// @brief 全要素を削除
		void clear() {
#ifdef PAXS_ENABLE_MAP_LOGGING
			PAXS_INFO("[" + name_ + "] clear: clearing all entries");
#endif
			map_.clear();
		}

		/// @brief 要素数を取得
		size_t size() const noexcept {
			return map_.size();
		}

		/// @brief 指定されたキーを持つ要素の数を取得
		size_t count(const Key& key) const {
#ifdef PAXS_ENABLE_MAP_LOGGING
			size_t result = map_.count(key);
			PAXS_INFO("[" + name_ + "] count: key=" + std::to_string(key) + " -> " + std::to_string(result));
			return result;
#else
			return map_.count(key);
#endif
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
