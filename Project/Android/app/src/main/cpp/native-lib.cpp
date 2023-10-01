/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#define PAXS_PATH "./"
#define PAXS_USING_DXLIB
#include <DxLib.h>

#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_MAHOROBA/Main.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

int android_main() {
    DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    if (DxLib::DxLib_Init() == -1) return -1;

    int w{640},h{360};
    DxLib::GetAndroidDisplayResolution(&w, &h);
    DxLib::SetGraphMode(w, h, 32);

    // 背景色を指定
    const paxg::Color color = paxg::Color(140, 180, 250); // 青
    paxg::Window::setBackgroundColor(color);

    int m = DxLib::LoadGraph("./Data/Map/XYZTile/Slope/Image/SlopePale20230920/2010/0/slope_pale_0_0_0.png");

    DxLib::SetDrawScreen(DX_SCREEN_BACK);
    // while (paxg::Window::update()) {
        DrawBox(0, 0, 300, 100, GetColor(230, 230, 240), TRUE);    // 四角形を描画する
        DrawGraph(0,0,m,TRUE);
    // }
    paxs::startMain(PAXS_PATH);

    return DxLib_End();
}
