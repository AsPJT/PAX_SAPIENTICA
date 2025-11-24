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

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <libgen.h>
#include <unistd.h>
#endif

int main() {
#ifdef __APPLE__
    // macOS: 実行ファイルのディレクトリにカレントディレクトリを変更
    char exe_path[1024];
    uint32_t size = sizeof(exe_path);
    if (_NSGetExecutablePath(exe_path, &size) == 0) {
        // 実行ファイルのディレクトリを取得
        char* dir = dirname(exe_path);
        chdir(dir);
    }
#endif

    paxs::startMain();
    return 0;
}
