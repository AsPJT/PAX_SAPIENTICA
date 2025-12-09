/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_SAPIENTICA/Utility/LoggerIntegration.hpp>

#if defined(PAXS_USING_DXLIB) && defined(_WIN32)
#include <Windows.h>
#include <DxLib.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#elif defined(PAXS_USING_SIV3D)
void Main() {
#else
int main() {
#endif
#ifdef PAXS_USING_DXLIB
    // DxLib initialization
    paxg::Window::PreInit();
    DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    if (DxLib::DxLib_Init() == -1) return -1;
    DxLib::SetDrawScreen(DX_SCREEN_BACK);
#endif
    paxg::Window::Init(1200, 900, "Font Drawing Test - All Alignment Methods");

    // 背景色を水色に設定
    paxg::Window::setBackgroundColor(paxg::Color(145, 190, 240));

    // FontSystemを初期化
    paxs::FontSystem::getInstance().initialize();

    // フォントを取得（サイズ24、バッファ厚さ3）
    paxg::Font* font = paxs::Fonts().getFont(24, 3);
    if (!font) {
        // フォールバック: デフォルトフォントを作成
        static paxg::Font fallback_font(24, "", 3);
        font = &fallback_font;
    }

    // アウトラインを設定（黒いアウトライン）
    font->setOutline(0, 0.6, paxg::Color(0, 0, 0));

    // 他のサイズのフォントも取得
    paxg::Font* title_font = paxs::Fonts().getFont(32, 3);
    if (!title_font) {
        static paxg::Font fallback_title_font(32, "", 3);
        title_font = &fallback_title_font;
    }
    title_font->setOutline(0, 0.6, paxg::Color(0, 0, 0));

    paxg::Font* label_font = paxs::Fonts().getFont(18, 3);
    if (!label_font) {
        static paxg::Font fallback_label_font(18, "", 3);
        label_font = &fallback_label_font;
    }

    paxg::Font* desc_font = paxs::Fonts().getFont(14, 3);
    if (!desc_font) {
        static paxg::Font fallback_desc_font(14, "", 3);
        desc_font = &fallback_desc_font;
    }

    paxg::Font* info_font = paxs::Fonts().getFont(16, 3);
    if (!info_font) {
        static paxg::Font fallback_info_font(16, "", 3);
        info_font = &fallback_info_font;
    }

    const paxg::Color text_color(255, 255, 255, 255);  // メインテスト文字: 白
    const paxg::Color white_color(255, 255, 255, 255);
    const paxg::Color red_color(255, 0, 0, 255);
    const paxg::Color yellow_color(255, 255, 0, 255);
    const paxg::Color gray_color(100, 100, 100, 255);
    const paxg::Color light_gray_color(80, 80, 80, 255);  // より黒に近いグレー

    // グリッド描画用の設定（ループの外で定義）
    const int cell_width = 350;
    const int cell_height = 250;
    const int start_x = 100;
    const int start_y = 100;

    while (paxg::Window::update()) {
        paxg::Window::clear();

        // タイトル
        title_font->drawAt("Font Alignment Test", paxs::Vector2<int>(600, 30), white_color);

        // draw (Top-Left) - grid position (0,0)
        {
            int cell_x = start_x + 0 * cell_width;
            int cell_y = start_y + 0 * cell_height;
            int center_x = cell_x + cell_width / 2;
            int center_y = cell_y + cell_height / 2;

            paxg::Rect rect(static_cast<float>(cell_x), static_cast<float>(cell_y),
                           static_cast<float>(cell_width), static_cast<float>(cell_height));
            rect.drawFrame(0.0, 1.0, gray_color);

            paxg::Line(static_cast<float>(center_x - 10), static_cast<float>(center_y),
                      static_cast<float>(center_x + 10), static_cast<float>(center_y)).draw(2.0f, red_color);
            paxg::Line(static_cast<float>(center_x), static_cast<float>(center_y - 10),
                      static_cast<float>(center_x), static_cast<float>(center_y + 10)).draw(2.0f, red_color);
            paxg::Circle(static_cast<float>(center_x), static_cast<float>(center_y), 3.0f).draw(red_color);

            label_font->drawTopCenter("draw", paxs::Vector2<int>(center_x, cell_y + 10), yellow_color);
            desc_font->drawTopCenter("Top-Left\n(left, top)", paxs::Vector2<int>(center_x, cell_y + 35), light_gray_color);

            font->draw("Test", paxs::Vector2<int>(center_x, center_y), text_color);
        }

        // drawTopCenter (Top-Center) - grid position (1,0)
        {
            int cell_x = start_x + 1 * cell_width;
            int cell_y = start_y + 0 * cell_height;
            int center_x = cell_x + cell_width / 2;
            int center_y = cell_y + cell_height / 2;

            paxg::Rect rect(static_cast<float>(cell_x), static_cast<float>(cell_y),
                           static_cast<float>(cell_width), static_cast<float>(cell_height));
            rect.drawFrame(0.0, 1.0, gray_color);

            paxg::Line(static_cast<float>(center_x - 10), static_cast<float>(center_y),
                      static_cast<float>(center_x + 10), static_cast<float>(center_y)).draw(2.0f, red_color);
            paxg::Line(static_cast<float>(center_x), static_cast<float>(center_y - 10),
                      static_cast<float>(center_x), static_cast<float>(center_y + 10)).draw(2.0f, red_color);
            paxg::Circle(static_cast<float>(center_x), static_cast<float>(center_y), 3.0f).draw(red_color);

            label_font->drawTopCenter("drawTopCenter", paxs::Vector2<int>(center_x, cell_y + 10), yellow_color);
            desc_font->drawTopCenter("Top-Center\n(center, top)", paxs::Vector2<int>(center_x, cell_y + 35), light_gray_color);

            font->drawTopCenter("Test", paxs::Vector2<int>(center_x, center_y), text_color);
        }

        // drawTopRight (Top-Right) - grid position (2,0)
        {
            int cell_x = start_x + 2 * cell_width;
            int cell_y = start_y + 0 * cell_height;
            int center_x = cell_x + cell_width / 2;
            int center_y = cell_y + cell_height / 2;

            paxg::Rect rect(static_cast<float>(cell_x), static_cast<float>(cell_y),
                           static_cast<float>(cell_width), static_cast<float>(cell_height));
            rect.drawFrame(0.0, 1.0, gray_color);

            paxg::Line(static_cast<float>(center_x - 10), static_cast<float>(center_y),
                      static_cast<float>(center_x + 10), static_cast<float>(center_y)).draw(2.0f, red_color);
            paxg::Line(static_cast<float>(center_x), static_cast<float>(center_y - 10),
                      static_cast<float>(center_x), static_cast<float>(center_y + 10)).draw(2.0f, red_color);
            paxg::Circle(static_cast<float>(center_x), static_cast<float>(center_y), 3.0f).draw(red_color);

            label_font->drawTopCenter("drawTopRight", paxs::Vector2<int>(center_x, cell_y + 10), yellow_color);
            desc_font->drawTopCenter("Top-Right\n(right, top)", paxs::Vector2<int>(center_x, cell_y + 35), light_gray_color);

            font->drawTopRight("Test", paxs::Vector2<int>(center_x, center_y), text_color);
        }

        // drawBottomLeft (Bottom-Left) - grid position (0,1)
        {
            int cell_x = start_x + 0 * cell_width;
            int cell_y = start_y + 1 * cell_height;
            int center_x = cell_x + cell_width / 2;
            int center_y = cell_y + cell_height / 2;

            paxg::Rect rect(static_cast<float>(cell_x), static_cast<float>(cell_y),
                           static_cast<float>(cell_width), static_cast<float>(cell_height));
            rect.drawFrame(0.0, 1.0, gray_color);

            paxg::Line(static_cast<float>(center_x - 10), static_cast<float>(center_y),
                      static_cast<float>(center_x + 10), static_cast<float>(center_y)).draw(2.0f, red_color);
            paxg::Line(static_cast<float>(center_x), static_cast<float>(center_y - 10),
                      static_cast<float>(center_x), static_cast<float>(center_y + 10)).draw(2.0f, red_color);
            paxg::Circle(static_cast<float>(center_x), static_cast<float>(center_y), 3.0f).draw(red_color);

            label_font->drawTopCenter("drawBottomLeft", paxs::Vector2<int>(center_x, cell_y + 10), yellow_color);
            desc_font->drawTopCenter("Bottom-Left\n(left, bottom)", paxs::Vector2<int>(center_x, cell_y + 35), light_gray_color);

            font->drawBottomLeft("Test", paxs::Vector2<int>(center_x, center_y), text_color);
        }

        // drawBottomCenter (Bottom-Center) - grid position (1,1)
        {
            int cell_x = start_x + 1 * cell_width;
            int cell_y = start_y + 1 * cell_height;
            int center_x = cell_x + cell_width / 2;
            int center_y = cell_y + cell_height / 2;

            paxg::Rect rect(static_cast<float>(cell_x), static_cast<float>(cell_y),
                           static_cast<float>(cell_width), static_cast<float>(cell_height));
            rect.drawFrame(0.0, 1.0, gray_color);

            paxg::Line(static_cast<float>(center_x - 10), static_cast<float>(center_y),
                      static_cast<float>(center_x + 10), static_cast<float>(center_y)).draw(2.0f, red_color);
            paxg::Line(static_cast<float>(center_x), static_cast<float>(center_y - 10),
                      static_cast<float>(center_x), static_cast<float>(center_y + 10)).draw(2.0f, red_color);
            paxg::Circle(static_cast<float>(center_x), static_cast<float>(center_y), 3.0f).draw(red_color);

            label_font->drawTopCenter("drawBottomCenter", paxs::Vector2<int>(center_x, cell_y + 10), yellow_color);
            desc_font->drawTopCenter("Bottom-Center\n(center, bottom)", paxs::Vector2<int>(center_x, cell_y + 35), light_gray_color);

            font->drawBottomCenter("Test", paxs::Vector2<int>(center_x, center_y), text_color);
        }

        // drawBottomRight (Bottom-Right) - grid position (2,1)
        {
            int cell_x = start_x + 2 * cell_width;
            int cell_y = start_y + 1 * cell_height;
            int center_x = cell_x + cell_width / 2;
            int center_y = cell_y + cell_height / 2;

            paxg::Rect rect(static_cast<float>(cell_x), static_cast<float>(cell_y),
                           static_cast<float>(cell_width), static_cast<float>(cell_height));
            rect.drawFrame(0.0, 1.0, gray_color);

            paxg::Line(static_cast<float>(center_x - 10), static_cast<float>(center_y),
                      static_cast<float>(center_x + 10), static_cast<float>(center_y)).draw(2.0f, red_color);
            paxg::Line(static_cast<float>(center_x), static_cast<float>(center_y - 10),
                      static_cast<float>(center_x), static_cast<float>(center_y + 10)).draw(2.0f, red_color);
            paxg::Circle(static_cast<float>(center_x), static_cast<float>(center_y), 3.0f).draw(red_color);

            label_font->drawTopCenter("drawBottomRight", paxs::Vector2<int>(center_x, cell_y + 10), yellow_color);
            desc_font->drawTopCenter("Bottom-Right\n(right, bottom)", paxs::Vector2<int>(center_x, cell_y + 35), light_gray_color);

            font->drawBottomRight("Test", paxs::Vector2<int>(center_x, center_y), text_color);
        }

        // drawAt (Center) - grid position (1,2)
        {
            int cell_x = start_x + 1 * cell_width;
            int cell_y = start_y + 2 * cell_height;
            int center_x = cell_x + cell_width / 2;
            int center_y = cell_y + cell_height / 2;

            paxg::Rect rect(static_cast<float>(cell_x), static_cast<float>(cell_y),
                           static_cast<float>(cell_width), static_cast<float>(cell_height));
            rect.drawFrame(0.0, 1.0, gray_color);

            paxg::Line(static_cast<float>(center_x - 10), static_cast<float>(center_y),
                      static_cast<float>(center_x + 10), static_cast<float>(center_y)).draw(2.0f, red_color);
            paxg::Line(static_cast<float>(center_x), static_cast<float>(center_y - 10),
                      static_cast<float>(center_x), static_cast<float>(center_y + 10)).draw(2.0f, red_color);
            paxg::Circle(static_cast<float>(center_x), static_cast<float>(center_y), 3.0f).draw(red_color);

            label_font->drawTopCenter("drawAt", paxs::Vector2<int>(center_x, cell_y + 10), yellow_color);
            desc_font->drawTopCenter("Center\n(center, center)", paxs::Vector2<int>(center_x, cell_y + 35), light_gray_color);

            font->drawAt("Test", paxs::Vector2<int>(center_x, center_y), text_color);
        }

        // 使用方法の説明
        info_font->draw("Red cross marks (+) show the reference point for each drawing method.",
                       paxs::Vector2<int>(50, 850), light_gray_color);
        info_font->draw("All methods should align text consistently across Siv3D, DxLib, and SFML.",
                       paxs::Vector2<int>(50, 870), light_gray_color);

        paxg::Window::display();
    }
#ifdef PAXS_USING_DXLIB
    DxLib::DxLib_End();
    return 0;
#elif !defined(PAXS_USING_SIV3D)
    return 0;
#endif
}
