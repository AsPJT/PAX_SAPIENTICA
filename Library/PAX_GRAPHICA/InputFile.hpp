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
        internal_file, // 内部
        external_file // 外部
    };

    struct InputFile {

#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
        int file_handle{ 0 };
        std::string pline{};
        std::string pline_tmp{};
#else
        std::ifstream pifs{};
        std::string pline{};
#endif // PAXS_USING_DXLIB

        // ファイルが読み込まれたか確認する（読み込まれていない時は true ）
        bool fail() const {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
            return (file_handle == 0);
#else
            return pifs.fail();
#endif // PAXS_USING_DXLIB
        }

        // str_ ファイルのパスの文字列
        // default_path_ 基準となる根幹のパス
        // type_ どこにファイルを保存するか
        InputFile(const std::string& str_, const std::string& default_path_ = "", const InputFileType type_ = InputFileType::internal_file) {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
#ifdef __ANDROID__
            file_handle = DxLib::FileRead_open(str_.c_str());
#else
            file_handle = DxLib::FileRead_open(std::string(default_path_ + str_).c_str());
#endif // __ANDROID__
            DxLib::FileRead_set_format(file_handle, DX_CHARCODEFORMAT_UTF8); // UTF-8 を読み込む
            if (file_handle != 0) {
                pline.resize(4096);
                pline_tmp.resize(4096);
            }
#else
            pifs = std::ifstream((default_path_.size() == 0) ? str_ : default_path_ + str_); // ファイルを読み込む
#endif
        }
        // 1 行読み込む
        bool getLine() {
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
            const int dline = DxLib::FileRead_gets(&(pline[0]), 4096, file_handle);
            if (dline == -1) return false;
            if (dline == 0) return false;
            return true;
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
