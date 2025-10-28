/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_INPUT_FILE_KEY_MATRIX_HPP
#define PAX_GRAPHICA_INPUT_FILE_KEY_MATRIX_HPP

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxg {

    // 入力ファイルの Key による行列を扱う
    // 例）
    // key  a   b   c
    // x    1   2   3
    // y    4   5   6
    // z    7   8   9
    // のようなデータの
    struct InputFileKeyMatrix {
        // 1 行目 （上記の例だと key,a,b,c が入る）
        std::unordered_map<std::string, std::size_t> first_line;
        // 2 行目以降
        std::unordered_map<std::string, std::string> line;

    };

}

#endif // !PAX_GRAPHICA_INPUT_FILE_KEY_MATRIX_HPP
