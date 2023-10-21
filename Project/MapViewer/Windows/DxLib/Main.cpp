/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#define _CRT_SECURE_NO_WARNINGS
#define PAXS_PATH "./../../../../"
#define PAXS_USING_DXLIB
#include <DxLib.h> // DxLib
#include <omp.h>
#include <PAX_MAHOROBA/Main.hpp>

#if defined(__ANDROID__)
// Android 専用処理
int android_main() {
#elif defined(__APPLE__)
// iOS 専用処理
int ios_main() {
#elif defined(__LINUX__)
// Linux 専用処理
int linux_main() {
#else
// その他の処理 (Windows)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif
    DxLib::SetOutApplicationLogValidFlag(FALSE);
    DxLib::ChangeWindowMode(TRUE);
    DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    if (DxLib::DxLib_Init() == -1) return -1;
    DxLib::SetDrawScreen(DX_SCREEN_BACK);
    DxLib::SetWaitVSyncFlag(TRUE);
    // DxLib::SetWindowSizeChangeEnableFlag(TRUE, FALSE);
    // DxLib::SetUseASyncLoadFlag(TRUE);
    paxs::startMain(PAXS_PATH);
    return DxLib::DxLib_End();
}
