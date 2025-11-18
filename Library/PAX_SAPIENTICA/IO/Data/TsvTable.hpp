/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_DATA_TSV_TABLE_HPP
#define PAX_SAPIENTICA_IO_DATA_TSV_TABLE_HPP

/*##########################################################################################
    TSV（Tab-Separated Values）ファイルを読み込み、テーブル形式でアクセスするクラス
    Loads TSV (Tab-Separated Values) files and provides table-like access
##########################################################################################*/

#include <cstddef>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/InputFile.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief TSV table class for reading and accessing tab-separated data
    /// @brief TSVテーブルクラス（タブ区切りデータの読み込みとアクセス）
    class TsvTable {
    private:
        /// @brief Header column names mapped to their indices
        /// @brief ヘッダーカラム名とそのインデックスのマッピング
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> header_map_;

        /// @brief Header column names in order
        /// @brief ヘッダーカラム名（順序付き）
        std::vector<std::uint_least32_t> header_keys_;

        /// @brief All rows of data (excluding header row)
        /// @brief 全データ行（ヘッダー行を除く）
        std::vector<std::vector<std::string>> rows_;

        /// @brief Load status flags
        /// @brief 読み込み状態フラグ
        bool is_loaded_{ false };
        bool is_successfully_loaded_{ false };

    public:
        /// @brief Default constructor
        /// @brief デフォルトコンストラクタ
        TsvTable() = default;

        /// @brief Constructor that immediately loads a TSV file
        /// @brief TSVファイルを即座に読み込むコンストラクタ
        /// @param relative_path Path to TSV file / アセットルートからの相対パス
        explicit TsvTable(const std::string& relative_path) {
            load(relative_path);
        }

        /// @brief Load TSV file
        /// @brief TSVファイルを読み込む
        /// @param relative_path Path to TSV file / アセットルートからの相対パス
        /// @return true if successfully loaded / 読み込み成功時true
        bool load(const std::string& relative_path) {
            if (is_loaded_) {
                PAXS_WARNING("TsvTable already loaded. Ignoring duplicate load.");
                return is_successfully_loaded_;
            }
            is_loaded_ = true;

            paxs::InputFile input_file(relative_path);
            if (input_file.fail()) {
                PAXS_ERROR(relative_path + " could not be opened.");
                return false;
            }

            // Read header row
            if (!input_file.getLine()) {
                PAXS_ERROR("The first line (header) of " + relative_path + " could not be read.");
                return false;
            }

            // Delete BOM if present
            input_file.deleteBOM();

            // Parse header
            header_map_ = input_file.splitHashMapMurMur3('\t');
            header_keys_ = input_file.splitHashMapMurMur3Vector('\t');

            if (header_keys_.empty()) {
                PAXS_ERROR(relative_path + " has an empty header row.");
                return false;
            }

            // Read data rows
            while (input_file.getLine()) {
                std::vector<std::string> row = input_file.split('\t');
                if (!row.empty()) {
                    rows_.emplace_back(std::move(row));
                }
            }

            is_successfully_loaded_ = true;
            return true;
        }

        /// @brief Get the number of rows (excluding header)
        /// @brief 行数を取得（ヘッダー行を除く）
        /// @return Number of data rows / データ行数
        std::size_t rowCount() const {
            return rows_.size();
        }

        /// @brief Get the number of columns
        /// @brief 列数を取得
        /// @return Number of columns / 列数
        std::size_t columnCount() const {
            return header_keys_.size();
        }

        /// @brief Check if a column exists by name
        /// @brief カラム名でカラムの存在を確認
        /// @param column_name Column name / カラム名
        /// @return true if column exists / カラムが存在すればtrue
        bool hasColumn(const std::string& column_name) const {
            return header_map_.contains(MurMur3::calcHash(column_name));
        }

        /// @brief Check if a column exists by hash key
        /// @brief ハッシュキーでカラムの存在を確認
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return true if column exists / カラムが存在すればtrue
        bool hasColumn(const std::uint_least32_t column_key) const {
            return header_map_.contains(column_key);
        }

        /// @brief Get column index by name
        /// @brief カラム名でカラムインデックスを取得
        /// @param column_name Column name / カラム名
        /// @return Column index, or SIZE_MAX if not found / カラムインデックス、見つからない場合はSIZE_MAX
        std::size_t getColumnIndex(const std::string& column_name) const {
            return getColumnIndex(MurMur3::calcHash(column_name));
        }

        /// @brief Get column index by hash key
        /// @brief ハッシュキーでカラムインデックスを取得
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Column index, or SIZE_MAX if not found / カラムインデックス、見つからない場合はSIZE_MAX
        std::size_t getColumnIndex(const std::uint_least32_t column_key) const {
            const auto iterator = header_map_.find(column_key);
            return (iterator != header_map_.end()) ? iterator->second : SIZE_MAX;
        }

        /// @brief Get cell value by row and column index
        /// @brief 行と列のインデックスでセル値を取得
        /// @param row_index Row index / 行インデックス
        /// @param column_index Column index / 列インデックス
        /// @return Cell value, or empty string if out of bounds / セル値、範囲外の場合は空文字列
        const std::string& get(std::size_t row_index, std::size_t column_index) const {
            static const std::string empty_string = "";
            if (row_index >= rows_.size() || column_index >= rows_[row_index].size()) {
                return empty_string;
            }
            return rows_[row_index][column_index];
        }

        /// @brief Get cell value by row index and column name (const char*)
        /// @brief 行インデックスとカラム名でセル値を取得（const char*）
        /// @param row_index Row index / 行インデックス
        /// @param column_name Column name / カラム名
        /// @return Cell value, or empty string if not found / セル値、見つからない場合は空文字列
        const std::string& get(std::size_t row_index, const char* column_name) const {
            std::size_t column_index = getColumnIndex(std::string(column_name));
            return get(row_index, column_index);
        }

        /// @brief Get cell value by row index and column name (std::string)
        /// @brief 行インデックスとカラム名でセル値を取得（std::string）
        /// @param row_index Row index / 行インデックス
        /// @param column_name Column name / カラム名
        /// @return Cell value, or empty string if not found / セル値、見つからない場合は空文字列
        const std::string& get(std::size_t row_index, const std::string& column_name) const {
            std::size_t column_index = getColumnIndex(column_name);
            return get(row_index, column_index);
        }

        /// @brief Get cell value by row index and column hash key
        /// @brief 行インデックスとカラムハッシュキーでセル値を取得
        /// @param row_index Row index / 行インデックス
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Cell value, or empty string if not found / セル値、見つからない場合は空文字列
        const std::string& get(std::size_t row_index, const std::uint_least32_t column_key) const {
            std::size_t column_index = getColumnIndex(column_key);
            return get(row_index, column_index);
        }

        /// @brief Get entire row by index
        /// @brief 行インデックスで行全体を取得
        /// @param row_index Row index / 行インデックス
        /// @return Row data, or empty vector if out of bounds / 行データ、範囲外の場合は空のベクター
        const std::vector<std::string>& getRow(std::size_t row_index) const {
            static const std::vector<std::string> empty_row;
            if (row_index >= rows_.size()) {
                return empty_row;
            }
            return rows_[row_index];
        }

        /// @brief Get all header column keys
        /// @brief 全ヘッダーカラムキーを取得
        /// @return Vector of column keys / カラムキーのベクター
        const std::vector<std::uint_least32_t>& getHeaderKeys() const {
            return header_keys_;
        }

        /// @brief Get header map (column key to index mapping)
        /// @brief ヘッダーマップを取得（カラムキーからインデックスへのマッピング）
        /// @return Header map / ヘッダーマップ
        const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& getHeaderMap() const {
            return header_map_;
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
            return is_successfully_loaded_;
        }

        /// @brief Iterate over all rows with callback
        /// @brief 全行をコールバックで反復処理
        /// @param callback Function called for each row (row_index, row_data) / 各行に対して呼ばれる関数（行インデックス、行データ）
        template<typename Func>
        void forEachRow(Func&& callback) const {
            for (std::size_t i = 0; i < rows_.size(); ++i) {
                callback(i, rows_[i]);
            }
        }

        /// @brief Clear all data
        /// @brief 全データをクリア
        void clear() {
            header_map_.clear();
            header_keys_.clear();
            rows_.clear();
            is_loaded_ = false;
            is_successfully_loaded_ = false;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_DATA_TSV_TABLE_HPP
