/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_INPUT_FILE_HPP
#define PAX_SAPIENTICA_INPUT_FILE_HPP

#include <fstream>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/Geography/StringConversions.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    // UTF-8 BOM
    constexpr char utf8_bom[4] = {
        static_cast<char>(static_cast<unsigned char>(0xef)) ,
        static_cast<char>(static_cast<unsigned char>(0xbb)) ,
        static_cast<char>(static_cast<unsigned char>(0xbf)) ,
        0 };

    struct InputFile {

#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
        int file_handle{ 0 };
        std::string pline0{}; // ファイル読み込み用の文字列 (char*) として扱う
        std::string pline{}; // std::string として用いる文字列
        std::ifstream pifs{}; // assets 以外の読み込み時に用いる
        std::uint_least32_t type{};
        int file_size = 0; // ファイルの大きさ
#else
        std::ifstream pifs{};
        std::string pline{};
#endif // PAXS_USING_DXLIB
        std::string file_path_{}; // 読み込んだファイルパス（相対パス）

        // ファイルが読み込まれたか確認する（読み込まれていない時は true ）
        bool fail() const {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
#ifdef __ANDROID__
            return (type == paxs::MurMur3::calcHash("asset_file")) ?
                (file_handle == 0) : (pifs.fail());
#else // Android 以外
            return pifs.fail();
#endif
#else
            return pifs.fail();
#endif // PAXS_USING_DXLIB
        }

        /// @brief 読み込んだファイルパスを取得
        /// @return 相対ファイルパス（読み込み前は空文字列）
        const std::string& getFilePath() const {
            return file_path_;
        }

        // str_ ファイルのパスの文字列（アセットルートからの相対パス）
        // default_path_ 基準となる根幹のパス（空の場合はAppConfig::getRootPath()を使用）
        // type_ どこにファイルを保存するか
        InputFile(const std::string& str_, const std::string& default_path_ = "",

            // 【フォルダの種類】
            //    asset_file    // アセットファイル (読み込みのみ)
            //    internal_file // 内部
            //    external_file // 外部
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
            const std::uint_least32_t type_ = paxs::MurMur3::calcHash("asset_file"),
#else
            [[maybe_unused]] const std::uint_least32_t type_ = paxs::MurMur3::calcHash("asset_file"),
#endif
            // 【フォーマットの種類】
            //    string // 文字列
            //    binary // バイナリ
            const std::uint_least32_t format_ = paxs::MurMur3::calcHash("string")
        ) {
            // ファイルパスを保存
            file_path_ = str_;

            // default_path_ が空の場合は AppConfig からルートパスを取得
            const std::string root_path = (default_path_.size() == 0) ?
                paxs::AppConfig::getInstance()->getRootPath() : default_path_;

#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
            type = type_; // フォルダの読み書きの種類を格納
#ifdef __ANDROID__
            static char file_path[256];
            switch (type) {
            case paxs::MurMur3::calcHash("asset_file"):
#ifdef __ANDROID__
                file_size = DxLib::FileRead_size(str_.c_str());
                file_handle = DxLib::FileRead_open(str_.c_str());
#else // Android 以外の場合の処理も記載
                file_handle = DxLib::FileRead_open(std::string(root_path + str_).c_str());
#endif // __ANDROID__
                DxLib::FileRead_set_format(file_handle, DX_CHARCODEFORMAT_UTF8); // UTF-8 を読み込む
                if (file_handle != 0) {
                    pline0.resize(4096);
                }
                break;
            case paxs::MurMur3::calcHash("internal_file"):
                DxLib::GetInternalDataPath(file_path, sizeof(file_path));
                pifs = (format_ == paxs::MurMur3::calcHash("binary")) ?
                    std::ifstream(std::string(file_path) + "/" + str_, std::ios::binary) : // ファイルを読み込む
                    std::ifstream(std::string(file_path) + "/" + str_); // ファイルを読み込む
                break;
            case paxs::MurMur3::calcHash("external_file"):
                DxLib::GetExternalDataPath(file_path, sizeof(file_path));
                pifs = (format_ == paxs::MurMur3::calcHash("binary")) ?
                    std::ifstream(std::string(file_path) + "/" + str_, std::ios::binary) : // ファイルを読み込む
                    std::ifstream(std::string(file_path) + "/" + str_); // ファイルを読み込む
                break;
            }
#else // Android 以外
            if (format_ == paxs::MurMur3::calcHash("binary")) {
                pifs = std::ifstream(root_path + str_, std::ios::binary); // ファイルを読み込む
            }
            else {
                pifs = std::ifstream(root_path + str_); // ファイルを読み込む
            }
#endif

#else // DxLib 以外
            if (format_ == paxs::MurMur3::calcHash("binary")) {
                pifs = std::ifstream(root_path + str_, std::ios::binary); // ファイルを読み込む
            }
            else {
                pifs = std::ifstream(root_path + str_); // ファイルを読み込む
            }
#endif
        }
        // 1 行読み込む
        bool getLine() {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
#ifdef __ANDROID__
            if (type == paxs::MurMur3::calcHash("asset_file")) {
                const int dline = DxLib::FileRead_gets(&(pline0[0]), 4096, file_handle);
                if (dline == -1 || dline == 0) {
                    pline.clear();
                    return false;
                }
                pline = std::string(pline0.c_str()); // std::string 型に変える
                return true;
            }
            return static_cast<bool>(std::getline(pifs, pline));
#else // Android 以外
            return static_cast<bool>(std::getline(pifs, pline));
#endif
#else
            return static_cast<bool>(std::getline(pifs, pline));
#endif // PAXS_USING_DXLIB
        }
        // 区切り文字で分割する
        std::vector<std::string> split(const char delimiter) const {
            return paxs::StringUtils::split(pline, delimiter);
        }
        // 区切り文字で分割する (double)
        std::vector<double> splitStod(const char delimiter) const {
            return paxs::StringUtils::splitStod(pline, delimiter);
        }
        // 区切り文字で分割する (double)
        void splitStod(const char delimiter, double* const result, const std::size_t size) const {
            paxs::StringUtils::splitStod(pline, delimiter, result, size);
        }
        // 区切り文字で分割する (SlopeDegU8)
        void splitSlopeDegU8(const char delimiter, unsigned char* const result, const std::size_t size) const {
            paxs::GeoStringConversions::splitSlopeDegU8(pline, delimiter, result, size);
        }
        // 区切り文字で分割する (ElevationS16)
        void splitElevationS16(const char delimiter, std::int_least16_t* const result, const std::size_t size) const {
            paxs::GeoStringConversions::splitElevationS16(pline, delimiter, result, size);
        }
        // 区切り文字で分割する
        paxs::UnorderedMap<std::string, std::size_t> splitHashMap(const char delimiter) const {
            return paxs::StringUtils::splitHashMap(pline, delimiter);
        }
        // 区切り文字で分割する（ 32bit ハッシュ値で管理）
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> splitHashMapMurMur3(const char delimiter) const {
            return paxs::StringUtils::splitHashMapMurMur3(pline, delimiter);
        }
        // 区切り文字で分割する（ 32bit ハッシュ値で管理）
        std::vector<std::uint_least32_t> splitHashMapMurMur3Vector(const char delimiter) const {
            return paxs::StringUtils::splitHashMapMurMur3Vector(pline, delimiter);
        }

        std::string& lineString() {
            return pline;
        }

        // BOM を削除する
        void deleteBOM() {
            if (pline.size() < 3) return;
            if (
                pline[0] == utf8_bom[0] &&
                pline[1] == utf8_bom[1] &&
                pline[2] == utf8_bom[2]
                ) {
                pline = std::string(&(pline[3]));
            }
        }

        // バイナリ分割
        std::size_t splitBinary(char* const result, const std::size_t size) {
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            if (type == paxs::MurMur3::calcHash("asset_file")) {
                const std::size_t read_size = (std::min)(static_cast<std::size_t>(file_size), size); // 読み込むサイズ
                DxLib::FileRead_read(result, static_cast<int>(read_size), file_handle);
                return read_size;
            }
            else {
                pifs.seekg(0, std::ios::end);
                const std::size_t read_max_size = static_cast<std::size_t>(pifs.tellg()); // 最大サイズ
                pifs.seekg(0);

                const std::size_t read_size = (std::min)(read_max_size, size); // 読み込むサイズ

                //読み込んだデータを char に出力
                pifs.read(result, read_size);
                return read_size;
            }
#else
            pifs.seekg(0, std::ios::end);
            const std::size_t read_max_size = static_cast<std::size_t>(pifs.tellg()); // 最大サイズ
            pifs.seekg(0);

            const std::size_t read_size = (std::min)(read_max_size, size); // 読み込むサイズ

            //読み込んだデータを char に出力
            pifs.read(result, read_size);
            return read_size;
#endif
        }

    };

}

#endif // !PAX_SAPIENTICA_INPUT_FILE_HPP
