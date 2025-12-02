/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_DATA_BINARY_TABLE_HPP
#define PAX_SAPIENTICA_IO_DATA_BINARY_TABLE_HPP

/*##########################################################################################
    バイナリテーブルファイルを読み込み、テーブル形式でアクセスするクラス
    Loads binary table files and provides table-like access
##########################################################################################*/

#include <algorithm>
#include <array>
#include <charconv> // std::to_chars (C++17)
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief Column type enumeration for binary table
    /// @brief バイナリテーブルのカラム型列挙
    enum class BinaryColumnType : std::uint8_t {
        Unknown = 0,
        KeyHash = 1,          // key:uint32 (hashed)
        ValueString = 2,      // value:string
        Longitude = 3,        // longitude:float
        Latitude = 4,         // latitude:float
        FirstYear = 5,        // first_year:int32
        LastYear = 6          // last_year:int32
    };

    /// @brief Binary table class for reading binary format data
    /// @brief バイナリ形式データの読み込み用テーブルクラス
    class BinaryTable {
    private:
        /// @brief Column type definitions
        /// @brief カラム型定義
        std::vector<BinaryColumnType> column_types_;

        /// @brief Column name to index mapping
        /// @brief カラム名からインデックスへのマッピング
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> column_map_;

        /// @brief Data rows (each cell is stored as string for compatibility)
        /// @brief データ行（各セルは互換性のため文字列として格納）
        std::vector<std::vector<std::string>> rows_;

        /// @brief Key hash values (stored separately when KeyHash column exists)
        /// @brief キーハッシュ値（KeyHashカラムが存在する場合に別途保存）
        std::vector<std::uint32_t> key_hashes_;

        /// @brief Load status flags
        /// @brief 読み込み状態フラグ
        bool is_loaded_{ false };
        bool is_successfully_loaded_{ false };

        /// @brief Helper to read data from memory buffer
        struct BufferReader {
            const char* ptr;
            const char* end;

            explicit BufferReader(const std::vector<char>& buffer)
                : ptr(buffer.data()), end(buffer.data() + buffer.size()) {}

            bool safeCheck(std::size_t size) const {
                return ptr + size <= end;
            }

            template <typename T>
            T read() {
                if (!safeCheck(sizeof(T))) return T{};
                T value;
                std::memcpy(&value, ptr, sizeof(T));
                ptr += sizeof(T);
                return value;
            }

            std::string readString() {
                const char* start = ptr;
                while (ptr < end && *ptr != '\0') {
                    ptr++;
                }
                std::string s(start, ptr - start);
                if (ptr < end) ptr++; // Skip null terminator
                return s;
            }
        };

        /// @brief Fast float to string conversion using std::to_chars
        static std::string fastFloatToString(float value) {
            std::array<char, 32> buffer;
            auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
            if (ec == std::errc()) {
                return std::string(buffer.data(), ptr - buffer.data());
            }
            return std::to_string(value); // Fallback
        }

        /// @brief Fast int to string conversion using std::to_chars
        static std::string fastIntToString(std::int32_t value) {
            std::array<char, 16> buffer;
            auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
            if (ec == std::errc()) {
                return std::string(buffer.data(), ptr - buffer.data());
            }
            return std::to_string(value);
        }

    public:
        /// @brief Default constructor
        /// @brief デフォルトコンストラクタ
        BinaryTable() = default;

        /// @brief Constructor that immediately loads a binary file
        /// @brief バイナリファイルを即座に読み込むコンストラクタ
        /// @param relative_path Path to binary file / アセットルートからの相対パス
        explicit BinaryTable(const std::string& relative_path) {
            load(relative_path);
        }

        /// @brief Load binary file
        /// @brief バイナリファイルを読み込む
        /// @param relative_path Path to binary file / アセットルートからの相対パス
        /// @return true if successfully loaded / 読み込み成功時true
        bool load(const std::string& relative_path) {
            if (is_loaded_) {
                PAXS_WARNING("BinaryTable already loaded. Ignoring duplicate load.");
                return is_successfully_loaded_;
            }
            is_loaded_ = true;

            // Construct full path
            const std::string full_path = AppConfig::getInstance().getRootPath() + relative_path;

            // Open file at the end to get size
            std::ifstream ifs(full_path, std::ios::binary | std::ios::ate);
            if (!ifs.is_open()) {
                PAXS_ERROR("Failed to open binary file: " + relative_path);
                return false;
            }

            // Get file size and read entire content into buffer
            std::streamsize size = ifs.tellg();
            ifs.seekg(0, std::ios::beg);

            if (size <= 0) {
                PAXS_WARNING("Binary file is empty: " + relative_path);
                return false;
            }

            std::vector<char> buffer(static_cast<std::size_t>(size));
            if (!ifs.read(buffer.data(), size)) {
                PAXS_ERROR("Failed to read binary file content: " + relative_path);
                return false;
            }

            // Close file early
            ifs.close();

            BufferReader reader(buffer);

            // 1バイト目：カラムの個数
            if (!reader.safeCheck(1)) return false;
            std::uint8_t column_count = reader.read<std::uint8_t>();
            if (column_count == 0) {
                PAXS_WARNING("Binary file has 0 columns: " + relative_path);
                return false;
            }

            // 2バイト目：データの個数のバイト数
            if (!reader.safeCheck(1)) return false;
            std::uint8_t data_count_bytes = reader.read<std::uint8_t>();
            if (data_count_bytes == 0 || data_count_bytes > 4) {
                PAXS_ERROR("Invalid data count byte size: " + std::to_string(data_count_bytes));
                return false;
            }

            // 3～6バイト目：データの個数
            std::uint32_t data_count = 0;
            if (!reader.safeCheck(data_count_bytes)) return false;
            for (std::uint8_t i = 0; i < data_count_bytes; ++i) {
                std::uint8_t byte_value = reader.read<std::uint8_t>();
                data_count = (data_count << 8) | byte_value;
            }

            // カラム定義を読み込む
            if (!reader.safeCheck(column_count)) return false;
            column_types_.resize(column_count);
            std::size_t longitude_index = SIZE_MAX;
            std::size_t latitude_index = SIZE_MAX;
            std::size_t key_hash_index = SIZE_MAX;

            for (std::uint8_t i = 0; i < column_count; ++i) {
                std::uint8_t column_type_id = reader.read<std::uint8_t>();
                column_types_[i] = static_cast<BinaryColumnType>(column_type_id);

                // カラムマッピングを設定
                switch (column_types_[i]) {
                case BinaryColumnType::KeyHash:
                    column_map_.emplace(MurMur3::calcHash("key"), i);
                    key_hash_index = i;
                    break;
                case BinaryColumnType::ValueString:
                    column_map_.emplace(MurMur3::calcHash("value"), i);
                    break;
                case BinaryColumnType::Longitude:
                    column_map_.emplace(MurMur3::calcHash("longitude"), i);
                    longitude_index = i;
                    break;
                case BinaryColumnType::Latitude:
                    column_map_.emplace(MurMur3::calcHash("latitude"), i);
                    latitude_index = i;
                    break;
                case BinaryColumnType::FirstYear:
                    column_map_.emplace(MurMur3::calcHash("first_year"), i);
                    break;
                case BinaryColumnType::LastYear:
                    column_map_.emplace(MurMur3::calcHash("last_year"), i);
                    break;
                default:
                    PAXS_WARNING("Unknown column type: " + std::to_string(column_type_id));
                    break;
                }
            }

            // データを読み込む
            rows_.resize(data_count); // 事前確保
            if (key_hash_index != SIZE_MAX) {
                key_hashes_.reserve(data_count);
            }

            for (std::uint32_t row = 0; row < data_count; ++row) {
                // 行ベクトルのサイズを確保
                rows_[row].resize(column_count);
                std::vector<std::string>& row_data = rows_[row];

                bool longitude_latitude_read = false;

                for (std::size_t col = 0; col < column_count; ++col) {
                    // longitude/latitude は同時に読み込む
                    if (column_types_[col] == BinaryColumnType::Longitude) {
                        if (!longitude_latitude_read && latitude_index != SIZE_MAX) {
                            float lon = reader.read<float>();
                            float lat = reader.read<float>();
                            row_data[col] = fastFloatToString(lon);
                            row_data[latitude_index] = fastFloatToString(lat);
                            longitude_latitude_read = true;
                        }
                    }
                    else if (column_types_[col] == BinaryColumnType::Latitude) {
                        // latitude は longitude と同時に読み込み済み
                        if (!longitude_latitude_read) {
                            PAXS_ERROR("Latitude without longitude in binary file: " + relative_path);
                            return false;
                        }
                    }
                    else if (column_types_[col] == BinaryColumnType::KeyHash) {
                        std::uint32_t key_hash = reader.read<std::uint32_t>();
                        key_hashes_.push_back(key_hash);
                        // keyは空文字列のまま
                    }
                    else if (column_types_[col] == BinaryColumnType::ValueString) {
                        row_data[col] = reader.readString();
                    }
                    else if (column_types_[col] == BinaryColumnType::FirstYear ||
                        column_types_[col] == BinaryColumnType::LastYear) {
                        std::int32_t year = reader.read<std::int32_t>();
                        row_data[col] = fastIntToString(year);
                    }
                }
            }

            is_successfully_loaded_ = true;
            return true;
        }

        /// @brief Get the number of rows
        /// @brief 行数を取得
        /// @return Number of data rows / データ行数
        std::size_t rowCount() const {
            return rows_.size();
        }

        /// @brief Get the number of columns
        /// @brief 列数を取得
        /// @return Number of columns / 列数
        std::size_t columnCount() const {
            return column_types_.size();
        }

        /// @brief Check if a column exists by name
        /// @brief カラム名でカラムの存在を確認
        /// @param column_name Column name / カラム名
        /// @return true if column exists / カラムが存在すればtrue
        bool hasColumn(const std::string& column_name) const {
            return column_map_.contains(MurMur3::calcHash(column_name));
        }

        /// @brief Check if a column exists by hash key
        /// @brief ハッシュキーでカラムの存在を確認
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return true if column exists / カラムが存在すればtrue
        bool hasColumn(const std::uint_least32_t column_key) const {
            return column_map_.contains(column_key);
        }

        /// @brief Get column index by hash key
        /// @brief ハッシュキーでカラムインデックスを取得
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Column index, or SIZE_MAX if not found / カラムインデックス、見つからない場合はSIZE_MAX
        std::size_t getColumnIndex(const std::uint_least32_t column_key) const {
            const auto iterator = column_map_.find(column_key);
            return (iterator != column_map_.end()) ? iterator->second : SIZE_MAX;
        }

        /// @brief Get cell value by row index and column hash key
        /// @brief 行インデックスとカラムハッシュキーでセル値を取得
        /// @param row_index Row index / 行インデックス
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Cell value, or empty string if not found / セル値、見つからない場合は空文字列
        const std::string& get(std::size_t row_index, const std::uint_least32_t column_key) const {
            static const std::string empty_string = "";
            std::size_t column_index = getColumnIndex(column_key);
            if (row_index >= rows_.size() || column_index >= rows_[row_index].size()) {
                return empty_string;
            }
            return rows_[row_index][column_index];
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

        /// @brief Get key hash value by row index
        /// @brief 行インデックスでキーハッシュ値を取得
        /// @param row_index Row index / 行インデックス
        /// @return Key hash value, or 0 if not available / キーハッシュ値、利用不可の場合は0
        std::uint32_t getKeyHash(std::size_t row_index) const {
            if (row_index >= key_hashes_.size()) {
                return 0;
            }
            return key_hashes_[row_index];
        }

        /// @brief Check if key hash data is available
        /// @brief キーハッシュデータが利用可能か確認
        /// @return true if key hash data exists / キーハッシュデータが存在する場合true
        bool hasKeyHashes() const {
            return !key_hashes_.empty();
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
            column_types_.clear();
            column_map_.clear();
            rows_.clear();
            key_hashes_.clear();
            is_loaded_ = false;
            is_successfully_loaded_ = false;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_DATA_BINARY_TABLE_HPP
