/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#define PAXS_PATH ""
#define PAXS_USING_DXLIB
#include <DxLib.h>

#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_MAHOROBA/Main.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

#include <filesystem>
#include <iostream>
#include <fstream>

int android_main() {
    DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    if (DxLib::DxLib_Init() == -1) return -1;

    int w{640},h{360};
    DxLib::GetAndroidDisplayResolution(&w, &h);
    DxLib::SetGraphMode(w, h, 32);

    char file_path[256]{};
    std::string str;
    DxLib::GetExternalDataPath( file_path, sizeof( file_path ) ) ;
    str = std::string(file_path) + std::string("/PAX_SAPIENTICA");

    std::filesystem::create_directories(str);
    std::ofstream ofs(str + "/Test.txt");
    if (!ofs) printfDx("False");
    else{
        printfDx("%s",str.c_str());
        ofs << "Test" << std::endl;
    }

    // 背景色を指定
    const paxg::Color color = paxg::Color(140, 180, 250); // 青
    paxg::Window::setBackgroundColor(color);
    DxLib::SetDrawScreen(DX_SCREEN_BACK);
    paxs::startMain(PAXS_PATH);

    return DxLib_End();
}
