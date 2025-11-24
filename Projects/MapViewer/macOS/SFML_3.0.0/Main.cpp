/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#define _CRT_SECURE_NO_WARNINGS
#define PAXS_USING_SFML

// STB implementation must be defined in exactly one cpp file per executable
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <SFML/Graphics.hpp>

#include <PAX_MAHOROBA/Core/Main.hpp>

#include <PAX_SAPIENTICA/Utility/ExecutablePath.hpp>

int main() {
    // 実行ファイルのディレクトリにカレントディレクトリを変更
    // Change the current directory to the executable file's directory
    paxs::ExecutablePath::changeToExecutableDirectory();

    paxs::startMain();
    return 0;
}
