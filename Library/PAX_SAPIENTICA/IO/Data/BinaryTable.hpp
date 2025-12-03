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
        LastYear = 6,         // last_year:int32
        FirstJulianDay = 7,   // first_julian_day:int32
        LastJulianDay = 8,    // last_julian_day:int32
        MinSize = 9,          // min_size:float
        MaxSize = 10          // max_size:float
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

        /// @brief Number of rows
        /// @brief 行数
        std::size_t row_count_{ 0 };

        /// @brief Key hash values (stored separately when KeyHash column exists)
        /// @brief キーハッシュ値（KeyHashカラムが存在する場合に別途保存）
        std::vector<std::uint32_t> key_hashes_;

        /// @brief Value string data (stored separately when ValueString column exists)
        /// @brief 値文字列データ（ValueStringカラムが存在する場合に別途保存）
        std::vector<std::string> value_strings_;

        /// @brief Longitude values stored as double
        /// @brief 経度情報（doubleとして管理）
        std::vector<double> longitudes_;

        /// @brief Latitude values stored as double
        /// @brief 緯度情報（doubleとして管理）
        std::vector<double> latitudes_;

        /// @brief First year values stored as int32
        /// @brief 開始年情報（int32として管理）
        std::vector<std::int32_t> first_years_;

        /// @brief Last year values stored as int32
        /// @brief 終了年情報（int32として管理）
        std::vector<std::int32_t> last_years_;

        /// @brief First Julian Day values stored as int32
        /// @brief 開始ユリウス日情報（int32として管理）
        std::vector<std::int32_t> first_julian_days_;

        /// @brief Last Julian Day values stored as int32
        /// @brief 終了ユリウス日情報（int32として管理）
        std::vector<std::int32_t> last_julian_days_;

        /// @brief Min size values stored as float
        /// @brief 最小サイズ情報（floatとして管理）
        std::vector<float> min_sizes_;

        /// @brief Max size values stored as float
        /// @brief 最大サイズ情報（floatとして管理）
        std::vector<float> max_sizes_;

        /// @brief Cache for returning string reference in get()
        /// @brief get()で文字列参照を返すための一時キャッシュ
        mutable std::string string_cache_;

        /// @brief Load status flags
        /// @brief 読み込み状態フラグ
        bool is_loaded_{ false };
        bool is_successfully_loaded_{ false };

        /// @brief Helper to read data from memory buffer
        struct BufferReader {
            const char* ptr;
            const char* end;

            explicit BufferReader(const std::vector<char>& buffer)
                : ptr(buffer.data()), end(buffer.data() + buffer.size()) {
            }

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

            row_count_ = data_count;

            // カラム定義を読み込む（最適化: インデックスを事前計算）
            if (!reader.safeCheck(column_count)) return false;
            column_types_.resize(column_count);

            // カラムインデックスを事前計算
            std::size_t longitude_index = SIZE_MAX;
            std::size_t latitude_index = SIZE_MAX;
            std::size_t key_hash_index = SIZE_MAX;
            std::size_t value_string_index = SIZE_MAX;
            std::size_t first_year_index = SIZE_MAX;
            std::size_t last_year_index = SIZE_MAX;
            std::size_t first_jd_index = SIZE_MAX;
            std::size_t last_jd_index = SIZE_MAX;
            std::size_t min_size_index = SIZE_MAX;
            std::size_t max_size_index = SIZE_MAX;

            for (std::uint8_t i = 0; i < column_count; ++i) {
                std::uint8_t column_type_id = reader.read<std::uint8_t>();
                column_types_[i] = static_cast<BinaryColumnType>(column_type_id);

                // カラムマッピングを設定（同時にインデックスを記録）
                switch (column_types_[i]) {
                case BinaryColumnType::KeyHash:
                    column_map_.emplace(MurMur3::calcHash("key"), i);
                    key_hash_index = i;
                    break;
                case BinaryColumnType::ValueString:
                    column_map_.emplace(MurMur3::calcHash("value"), i);
                    value_string_index = i;
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
                    first_year_index = i;
                    break;
                case BinaryColumnType::LastYear:
                    column_map_.emplace(MurMur3::calcHash("last_year"), i);
                    last_year_index = i;
                    break;
                case BinaryColumnType::FirstJulianDay:
                    column_map_.emplace(MurMur3::calcHash("first_julian_day"), i);
                    first_jd_index = i;
                    break;
                case BinaryColumnType::LastJulianDay:
                    column_map_.emplace(MurMur3::calcHash("last_julian_day"), i);
                    last_jd_index = i;
                    break;
                case BinaryColumnType::MinSize:
                    column_map_.emplace(MurMur3::calcHash("min_size"), i);
                    min_size_index = i;
                    break;
                case BinaryColumnType::MaxSize:
                    column_map_.emplace(MurMur3::calcHash("max_size"), i);
                    max_size_index = i;
                    break;
                default:
                    PAXS_WARNING("Unknown column type: " + std::to_string(column_type_id));
                    break;
                }
            }

            // 各カラムのベクトル確保（最適化: resize → reserve）
            if (key_hash_index != SIZE_MAX) key_hashes_.reserve(data_count);
            if (value_string_index != SIZE_MAX) value_strings_.reserve(data_count);
            if (longitude_index != SIZE_MAX) longitudes_.reserve(data_count);
            if (latitude_index != SIZE_MAX) latitudes_.reserve(data_count);
            if (first_year_index != SIZE_MAX) first_years_.reserve(data_count);
            if (last_year_index != SIZE_MAX) last_years_.reserve(data_count);
            if (first_jd_index != SIZE_MAX) first_julian_days_.reserve(data_count);
            if (last_jd_index != SIZE_MAX) last_julian_days_.reserve(data_count);
            if (min_size_index != SIZE_MAX) min_sizes_.reserve(data_count);
            if (max_size_index != SIZE_MAX) max_sizes_.reserve(data_count);

            // データを読み込む（最適化: インデックス比較で条件分岐を削減）
            for (std::uint32_t row = 0; row < data_count; ++row) {
                for (std::size_t col = 0; col < column_count; ++col) {
                    if (col == longitude_index && latitude_index != SIZE_MAX) {
                        // longitude/latitude は同時に読み込む
                        const float lon = reader.read<float>();
                        const float lat = reader.read<float>();
                        longitudes_.push_back(static_cast<double>(lon));
                        latitudes_.push_back(static_cast<double>(lat));
                    }
                    else if (col == latitude_index) {
                        // latitude は longitude と同時に読み込み済み（スキップ）
                        continue;
                    }
                    else if (col == key_hash_index) {
                        key_hashes_.push_back(reader.read<std::uint32_t>());
                    }
                    else if (col == value_string_index) {
                        value_strings_.push_back(reader.readString());
                    }
                    else if (col == first_year_index) {
                        first_years_.push_back(reader.read<std::int32_t>());
                    }
                    else if (col == last_year_index) {
                        last_years_.push_back(reader.read<std::int32_t>());
                    }
                    else if (col == first_jd_index) {
                        first_julian_days_.push_back(reader.read<std::int32_t>());
                    }
                    else if (col == last_jd_index) {
                        last_julian_days_.push_back(reader.read<std::int32_t>());
                    }
                    else if (col == min_size_index) {
                        min_sizes_.push_back(reader.read<float>());
                    }
                    else if (col == max_size_index) {
                        max_sizes_.push_back(reader.read<float>());
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
            return row_count_;
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
            if (column_index >= column_types_.size()) {
                return empty_string;
            }

            // 数値型カラムの場合は get 関数を使用しない
            if (column_types_[column_index] == BinaryColumnType::Longitude ||
                column_types_[column_index] == BinaryColumnType::Latitude ||
                column_types_[column_index] == BinaryColumnType::FirstYear ||
                column_types_[column_index] == BinaryColumnType::LastYear ||
                column_types_[column_index] == BinaryColumnType::FirstJulianDay ||
                column_types_[column_index] == BinaryColumnType::LastJulianDay ||
                column_types_[column_index] == BinaryColumnType::MinSize ||
                column_types_[column_index] == BinaryColumnType::MaxSize) {
                PAXS_WARNING("The get() function cannot be used for numeric columns. Use getInt32(), getFloat(), or getDouble() instead.");
                return empty_string;
            }

            // ValueStringカラムの場合
            if (column_types_[column_index] == BinaryColumnType::ValueString) {
                if (row_index >= value_strings_.size()) {
                    return empty_string;
                }
                return value_strings_[row_index];
            }

            // KeyHashカラムの場合は空文字列を返す（ハッシュ値はgetKeyHashで取得）
            return empty_string;
        }

        /// @brief Get int32 value by row index and column hash key
        /// @brief 行インデックスとカラムハッシュキーでint32値を取得
        /// @param row_index Row index / 行インデックス
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Int32 value, or 0 if not found / int32値、見つからない場合は0
        std::int32_t getInt32(std::size_t row_index, const std::uint_least32_t column_key) const {
            std::size_t column_index = getColumnIndex(column_key);
            if (row_index >= row_count_ || column_index >= column_types_.size()) {
                return 0;
            }

            if (column_types_[column_index] == BinaryColumnType::FirstYear) {
                if (row_index >= first_years_.size()) return 0;
                return first_years_[row_index];
            }
            else if (column_types_[column_index] == BinaryColumnType::LastYear) {
                if (row_index >= last_years_.size()) return 0;
                return last_years_[row_index];
            }
            else if (column_types_[column_index] == BinaryColumnType::FirstJulianDay) {
                if (row_index >= first_julian_days_.size()) return 0;
                return first_julian_days_[row_index];
            }
            else if (column_types_[column_index] == BinaryColumnType::LastJulianDay) {
                if (row_index >= last_julian_days_.size()) return 0;
                return last_julian_days_[row_index];
            }

            return 0;
        }

        /// @brief Get float value by row index and column hash key
        /// @brief 行インデックスとカラムハッシュキーでfloat値を取得
        /// @param row_index Row index / 行インデックス
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Float value, or 0.0f if not found / float値、見つからない場合は0.0f
        float getFloat(std::size_t row_index, const std::uint_least32_t column_key) const {
            const std::size_t column_index = getColumnIndex(column_key);
            if (row_index >= row_count_ || column_index >= column_types_.size()) {
                return 0.0f;
            }

            if (column_types_[column_index] == BinaryColumnType::MinSize) {
                if (row_index >= min_sizes_.size()) return 0.0f;
                return min_sizes_[row_index];
            }
            else if (column_types_[column_index] == BinaryColumnType::MaxSize) {
                if (row_index >= max_sizes_.size()) return 0.0f;
                return max_sizes_[row_index];
            }

            return 0.0f;
        }

        /// @brief Get double value by row index and column hash key (optimized for lat/lon)
        /// @brief 行インデックスとカラムハッシュキーでdouble値を取得（緯度経度最適化用）
        /// @param row_index Row index / 行インデックス
        /// @param column_key Column key (MurMur3 hash) / カラムキー（MurMur3ハッシュ）
        /// @return Double value, or 0.0 if not found / double値、見つからない場合は0.0
        double getDouble(std::size_t row_index, const std::uint_least32_t column_key) const {
            const std::size_t column_index = getColumnIndex(column_key);
            if (row_index >= row_count_ || column_index >= column_types_.size()) {
                return 0.0;
            }

            if (column_types_[column_index] == BinaryColumnType::Longitude) {
                if (row_index >= longitudes_.size()) return 0.0;
                return longitudes_[row_index];
            }
            else if (column_types_[column_index] == BinaryColumnType::Latitude) {
                if (row_index >= latitudes_.size()) return 0.0;
                return latitudes_[row_index];
            }
            // int32型もdoubleとして返す（互換性のため）
            else if (column_types_[column_index] == BinaryColumnType::FirstYear) {
                if (row_index >= first_years_.size()) return 0.0;
                return static_cast<double>(first_years_[row_index]);
            }
            else if (column_types_[column_index] == BinaryColumnType::LastYear) {
                if (row_index >= last_years_.size()) return 0.0;
                return static_cast<double>(last_years_[row_index]);
            }
            else if (column_types_[column_index] == BinaryColumnType::FirstJulianDay) {
                if (row_index >= first_julian_days_.size()) return 0.0;
                return static_cast<double>(first_julian_days_[row_index]);
            }
            else if (column_types_[column_index] == BinaryColumnType::LastJulianDay) {
                if (row_index >= last_julian_days_.size()) return 0.0;
                return static_cast<double>(last_julian_days_[row_index]);
            }
            // float型もdoubleとして返す
            else if (column_types_[column_index] == BinaryColumnType::MinSize) {
                if (row_index >= min_sizes_.size()) return 0.0;
                return static_cast<double>(min_sizes_[row_index]);
            }
            else if (column_types_[column_index] == BinaryColumnType::MaxSize) {
                if (row_index >= max_sizes_.size()) return 0.0;
                return static_cast<double>(max_sizes_[row_index]);
            }

            return 0.0;
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

        /// @brief Clear all data
        /// @brief 全データをクリア
        void clear() {
            column_types_.clear();
            column_map_.clear();
            row_count_ = 0;
            key_hashes_.clear();
            value_strings_.clear();
            longitudes_.clear();
            latitudes_.clear();
            first_years_.clear();
            last_years_.clear();
            first_julian_days_.clear();
            last_julian_days_.clear();
            min_sizes_.clear();
            max_sizes_.clear();
            is_loaded_ = false;
            is_successfully_loaded_ = false;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_IO_DATA_BINARY_TABLE_HPP
