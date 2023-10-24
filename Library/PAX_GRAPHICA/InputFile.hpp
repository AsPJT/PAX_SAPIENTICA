/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_INPUT_FILE_HPP
#define PAX_GRAPHICA_INPUT_FILE_HPP

/*##########################################################################################

##########################################################################################*/

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxg {

    enum class InputFileType : unsigned char {
        asset_file, // アセットファイル (読み込みのみ)
        internal_file, // 内部
        external_file // 外部
    };

    struct InputFile {

#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
        int file_handle{ 0 };
        std::string pline0{}; // ファイル読み込み用の文字列 (char*) として扱う
        std::string pline{}; // std::string として用いる文字列
        std::ifstream pifs{}; // assets 以外の読み込み時に用いる
        InputFileType type{};
#else
        std::ifstream pifs{};
        std::string pline{};
#endif // PAXS_USING_DXLIB

        // ファイルが読み込まれたか確認する（読み込まれていない時は true ）
        bool fail() const {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
#ifdef __ANDROID__
            return (type == InputFileType::asset_file) ?
                (file_handle == 0) : (pifs.fail());
#else // Android 以外
            return pifs.fail();
#endif
#else
            return pifs.fail();
#endif // PAXS_USING_DXLIB
        }

        // str_ ファイルのパスの文字列
        // default_path_ 基準となる根幹のパス
        // type_ どこにファイルを保存するか
        InputFile(const std::string& str_, const std::string& default_path_ = "", const InputFileType type_ = InputFileType::asset_file) {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
            type = type_; // フォルダの読み書きの種類を格納
#ifdef __ANDROID__
            static char file_path[256];
            switch (type) {
            case InputFileType::asset_file:
#ifdef __ANDROID__
                file_handle = DxLib::FileRead_open(str_.c_str());
#else // Android 以外の場合の処理も記載
                file_handle = DxLib::FileRead_open(std::string(default_path_ + str_).c_str());
#endif // __ANDROID__
                DxLib::FileRead_set_format(file_handle, DX_CHARCODEFORMAT_UTF8); // UTF-8 を読み込む
                if (file_handle != 0) {
                    pline0.resize(4096);
                }
                break;
            case InputFileType::internal_file:
                DxLib::GetInternalDataPath(file_path, sizeof(file_path));
                pifs = std::ifstream(std::string(file_path) + "/" + str_); // ファイルを読み込む
                break;
            case InputFileType::external_file:
                DxLib::GetExternalDataPath(file_path, sizeof(file_path));
                pifs = std::ifstream(std::string(file_path) + "/" + str_); // ファイルを読み込む
                break;
            }
#else // Android 以外
            pifs = std::ifstream((default_path_.size() == 0) ? str_ : default_path_ + str_); // ファイルを読み込む
#endif

#else // DxLib 以外
            pifs = std::ifstream((default_path_.size() == 0) ? str_ : default_path_ + str_); // ファイルを読み込む
#endif
        }
        // 1 行読み込む
        bool getLine() {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
#ifdef __ANDROID__
            if (type == InputFileType::asset_file) {
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
            return paxs::StringExtensions::split(pline, delimiter);
        }
        // 区切り文字で分割する
        std::unordered_map<std::string, std::size_t> splitHashMap(const char delimiter) const {
            return paxs::StringExtensions::splitHashMap(pline, delimiter);
        }
        // 区切り文字で分割する（ 32bit ハッシュ値で管理）
        std::unordered_map<std::uint_least32_t, std::size_t> splitHashMapMurMur3(const char delimiter) const {
            return paxs::StringExtensions::splitHashMapMurMur3(pline, delimiter);
        }

        std::string& lineString() {
            return pline;
        }

    };

}

#endif // !PAX_GRAPHICA_INPUT_FILE_HPP
