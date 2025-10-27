/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_MOUSE_HPP
#define PAX_GRAPHICA_MOUSE_HPP

#include <optional>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#if defined(PAXS_USING_SFML)
#include <PAX_GRAPHICA/SFML_Event.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    // 実行時定数
    class Mouse {
    public:
        // インスタンスを取得
        static Mouse* getInstance() {
            if (instance == nullptr) {
                instance = new Mouse();
            }
            return instance;
        }
        // マウスの回転量を取得
        int getWheelRotVol() const {
            return wheel_rot_vol;
        }

        int getPosXBefore1Frame() const {
            return pos_x_before_1frame;
        }
        int getPosYBefore1Frame() const {
            return pos_y_before_1frame;
        }
        int getPosX() const {
            return pos_x;
        }
        int getPosY() const {
            return pos_y;
        }

        bool getLeftBefore1Frame() const {
            return left_before_1frame;
        }
        bool getLeft() const {
            return left;
        }
        // 離した瞬間
        bool upLeft() const {
            return (left_before_1frame && !left);
        }
        // 押した瞬間
        bool downLeft() const {
            return (!left_before_1frame && left);
        }
        // ２フレーム以上連続で押されている
        bool pressedLeft2() const {
            return (left_before_1frame && left);
        }

        // 毎フレーム最初に呼び出す
        void calledFirstEveryFrame() {
            // 以前の位置を 1 フレーム前の位置へ移動
            pos_x_before_1frame = pos_x;
            pos_y_before_1frame = pos_y;
            left_before_1frame = left;

#if defined(PAXS_USING_SIV3D)
            wheel_rot_vol = static_cast<int>(s3d::Mouse::Wheel());
            left = s3d::MouseL.pressed();
            pos_x = s3d::Cursor::Pos().x;
            pos_y = s3d::Cursor::Pos().y;
#elif defined(PAXS_USING_DXLIB)
            wheel_rot_vol = DxLib::GetMouseWheelRotVol();
            left = ((DxLib::GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
            if (DxLib::GetMousePoint(&pos_x, &pos_y) == -1) {
                pos_x = pos_y = 0;
            }
#elif defined(PAXS_USING_SFML)
            wheel_rot_vol = static_cast<int>(paxg::SFML_Event::getInstance()->wheel_delta);
            left = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            auto mouse_pos = sf::Mouse::getPosition(Window::window());
            pos_x = mouse_pos.x;
            pos_y = mouse_pos.y;
#endif
        }

        // SFML 3.0.0: より安全なマウス位置取得（std::optional）
        std::optional<Vec2i> tryGetPosition() const {
#if defined(PAXS_USING_SIV3D)
            return Vec2i(s3d::Cursor::Pos().x, s3d::Cursor::Pos().y);
#elif defined(PAXS_USING_DXLIB)
            int x = 0, y = 0;
            if (DxLib::GetMousePoint(&x, &y) == -1) {
                return std::nullopt;
            }
            return Vec2i(x, y);
#elif defined(PAXS_USING_SFML)
            // SFML 3.0 では常に成功するが、一貫性のためoptionalを返す
            return Vec2i(pos_x, pos_y);
#else
            return std::nullopt;
#endif
        }

    private:
        static Mouse* instance;

        int pos_x_before_1frame = 0; // 1 フレーム前の位置
        int pos_y_before_1frame = 0; // 1 フレーム前の位置

        int pos_x = -1, pos_y = -1;

        bool left_before_1frame = false; // 1 フレーム前の左クリック判定

        bool left = false;
        //bool right = false; // 未使用
        //bool middle = false; // 未使用

        int wheel_rot_vol = 0;

        Mouse() = default;

        ~Mouse() {
            delete instance;
        }

    };

    Mouse* Mouse::instance = nullptr;

}

#endif // !PAX_GRAPHICA_MOUSE_HPP
