/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_DATA_UNIFIED_TABLE_HPP
#define PAX_SAPIENTICA_IO_DATA_UNIFIED_TABLE_HPP

/*##########################################################################################
    TSVまたはバイナリ形式のファイルを自動判別して読み込むクラス
    Automatically detects and loads TSV or binary format files
##########################################################################################*/

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/IO/Data/BinaryTable.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief Unified table class that supports both TSV and binary formats
    /// @brief TSVとバイナリ形式の両方をサポートする統合テーブルクラス
    class UnifiedTable {
    private:
        /// @brief TSV table instance
        /// @brief TSVテーブルインスタンス
        TsvTable tsv_table_;

        /// @brief Binary table instance
        /// @brief バイナリテーブルインスタンス
        BinaryTable binary_table_;

        /// @brief Flag indicating which format is loaded
        /// @brief 読み込まれた形式を示すフラグ
        bool is_binary_{ false };

        /// @brief Load status flag
        /// @brief 読み込み状態フラグ
        bool is_loaded_{ false };

    public:
        /// @brief Default constructor
        /// @brief デフォルトコンストラクタ
        UnifiedTable() = default;

        /// @brief Constructor that immediately loads a file
        /// @brief ファイルを即座に読み込むコンストラクタ
        /// @param relative_path Path to file (with or without extension) / ファイルパス（拡張子あり/なし）
        explicit UnifiedTable(const std::string& relative_path) {
            load(relative_path);
        }

        /// @brief Load file (automatically detects TSV or binary format)
        /// @brief ファイルを読み込む（TSVまたはバイナリ形式を自動判別）
        /// @param relative_path Path to file / ファイルパス
        /// @return true if successfully loaded / 読み込み成功時true
        bool load(const std::string& relative_path) {
            if (is_loaded_) {
                PAXS_WARNING("UnifiedTable already loaded. Ignoring duplicate load.");
                return isSuccessfullyLoaded();
            }
            is_loaded_ = true;

            // Try binary format first (.bin)
            std::string binary_path = relative_path;
            if (binary_path.size() >= 4 && binary_path.substr(binary_path.size() - 4) == ".tsv") {
                binary_path = binary_path.substr(0, binary_path.size() - 4) + ".bin";
            } else if (binary_path.size() < 4 || binary_path.substr(binary_path.size() - 4) != ".bin") {
                binary_path += ".bin";
            }

            if (FileSystem::exists(binary_path)) {
                if (binary_table_.load(binary_path)) {
                    is_binary_ = true;
                    PAXS_INFO("Loaded binary format: " + binary_path);
                    return true;
                }
            }

            // Try TSV format (.tsv)
            std::string tsv_path = relative_path;
            if (tsv_path.size() >= 4 && tsv_path.substr(tsv_path.size() - 4) == ".bin") {
                tsv_path = tsv_path.substr(0, tsv_path.size() - 4) + ".tsv";
            } else if (tsv_path.size() < 4 || tsv_path.substr(tsv_path.size() - 4) != ".tsv") {
                tsv_path += ".tsv";
            }

            if (FileSystem::exists(tsv_path)) {
                if (tsv_table_.load(tsv_path)) {
                    is_binary_ = false;
                    PAXS_INFO("Loaded TSV format: " + tsv_path);
                    return true;
                }
            }

            PAXS_ERROR("Failed to load file in either binary or TSV format: " + relative_path);
            return false;
        }

        /// @brief Get the number of rows
        /// @brief 行数を取得
        /// @return Number of data rows / データ行数
        std::size_t rowCount() const {
            return is_binary_ ? binary_table_.rowCount() : tsv_table_.rowCount();
        }

        /// @brief Get the number of columns
        /// @brief 列数を取得
        /// @return Number of columns / 列数
        std::size_t columnCount() const {
            return is_binary_ ? binary_table_.columnCount() : tsv_table_.columnCount();
        }

        /// @brief Check if a column exists by name
        /// @brief カラム名でカラムの存在を確認
        /// @param column_name Column name / カラム名
        /// @return true if column exists / カラムが存在すればtrue
        bool hasColumn(const std::string& column_name) const {
            return is_binary_ ? binary_table_.hasColumn(column_name) : tsv_table_.hasColumn(column_name);
        }

        /// @brief Check if a column exists by hash key
        /// @brief ハッシュキーでカラムの存在を確認
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return true if column exists / カラムが存在すればtrue
        bool hasColumn(const std::uint_least32_t column_key) const {
            return is_binary_ ? binary_table_.hasColumn(column_key) : tsv_table_.hasColumn(column_key);
        }

        /// @brief Get cell value by row index and column hash key
        /// @brief 行インデックスとカラムハッシュキーでセル値を取得
        /// @param row_index Row index / 行インデックス
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Cell value, or empty string if not found / セル値、見つからない場合は空文字列
        const std::string& get(std::size_t row_index, const std::uint_least32_t column_key) const {
            return is_binary_ ? binary_table_.get(row_index, column_key) : tsv_table_.get(row_index, column_key);
        }

        /// @brief Get entire row by index
        /// @brief 行インデックスで行全体を取得
        /// @param row_index Row index / 行インデックス
        /// @return Row data, or empty vector if out of bounds / 行データ、範囲外の場合は空のベクター
        const std::vector<std::string>& getRow(std::size_t row_index) const {
            return is_binary_ ? binary_table_.getRow(row_index) : tsv_table_.getRow(row_index);
        }

        /// @brief Get all header column keys (TSV only)
        /// @brief 全ヘッダーカラムキーを取得（TSVのみ）
        /// @return Vector of column keys / カラムキーのベクター
        const std::vector<std::uint_least32_t> getHeaderKeys() const {
            if (!is_binary_) {
                return tsv_table_.getHeaderKeys();
            }
            // Binary format doesn't store full header keys, return empty vector
            return std::vector<std::uint_least32_t>();
        }

        /// @brief Get key hash value by row index (binary only)
        /// @brief 行インデックスでキーハッシュ値を取得（バイナリのみ）
        /// @param row_index Row index / 行インデックス
        /// @return Key hash value, or 0 if not available / キーハッシュ値、利用不可の場合は0
        std::uint32_t getKeyHash(std::size_t row_index) const {
            if (is_binary_) {
                return binary_table_.getKeyHash(row_index);
            }
            // TSV format: hash the key string
            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            const std::string& key_str = tsv_table_.get(row_index, key_hash);
            if (!key_str.empty()) {
                return MurMur3::calcHash(key_str.size(), key_str.c_str());
            }
            return 0;
        }

        /// @brief Check if key hash data is available
        /// @brief キーハッシュデータが利用可能か確認
        /// @return true if key hash data exists / キーハッシュデータが存在する場合true
        bool hasKeyHashes() const {
            if (is_binary_) {
                return binary_table_.hasKeyHashes();
            }
            // TSV format can generate hash from key column
            return tsv_table_.hasColumn("key");
        }

        /// @brief Check if file was loaded
        /// @brief ファイルが読み込まれたかを確認
        /// @return true if load() was called / load()が呼ばれた場合true
        bool isLoaded() const {
            return is_loaded_;
        }

        /// @brief Check if file was successfully loaded
        /// @brief ファイルが正常に読み込まれたかを確認
        /// @return true if successfully loaded / 正常に読み込まれた場合true
        bool isSuccessfullyLoaded() const {
            return is_binary_ ? binary_table_.isSuccessfullyLoaded() : tsv_table_.isSuccessfullyLoaded();
        }

        /// @brief Check if binary format is loaded
        /// @brief バイナリ形式が読み込まれたかを確認
        /// @return true if binary format / バイナリ形式の場合true
        bool isBinary() const {
            return is_binary_;
        }

        /// @brief Iterate over all rows with callback
        /// @brief 全行をコールバックで反復処理
        /// @param callback Function called for each row (row_index, row_data) / 各行に対して呼ばれる関数（行インデックス、行データ）
        template<typename Func>
        void forEachRow(Func&& callback) const {
            if (is_binary_) {
                binary_table_.forEachRow(std::forward<Func>(callback));
            } else {
                tsv_table_.forEachRow(std::forward<Func>(callback));
            }
        }

        /// @brief Clear all data
        /// @brief 全データをクリア
        void clear() {
            tsv_table_.clear();
            binary_table_.clear();
            is_loaded_ = false;
            is_binary_ = false;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_DATA_UNIFIED_TABLE_HPP
