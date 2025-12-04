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
#include <PAX_GRAPHICA/SFML_Event.hpp>
#endif

#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    // 実行時定数
    class Mouse {
    private:
        static Mouse instance;

        int pos_x_before_1frame = 0; // 1 フレーム前の位置
        int pos_y_before_1frame = 0; // 1 フレーム前の位置

        int pos_x = -1, pos_y = -1;

        bool left_before_1frame = false; // 1 フレーム前の左クリック判定

        bool left = false;
        bool right = false;
        bool middle = false;

        int wheel_rot_vol = 0;

        Mouse() = default;
        ~Mouse() = default;

    public:
        // Delete copy and move operations
        Mouse(const Mouse&) = delete;
        Mouse& operator=(const Mouse&) = delete;
        Mouse(Mouse&&) = delete;
        Mouse& operator=(Mouse&&) = delete;

        // インスタンスを取得
        static Mouse* getInstance() {
            return &instance;
        }
        // マウスの回転量を取得
        int getWheelRotVol() const {
            return wheel_rot_vol;
        }

        paxs::Vector2<int> getPosBefore1Frame() const {
            return paxs::Vector2<int>(pos_x_before_1frame, pos_y_before_1frame);
        }
        int getPosX() const {
            return pos_x;
        }
        int getPosY() const {
            return pos_y;
        }
        paxs::Vector2<int> getPos() const {
            return paxs::Vector2<int>(pos_x, pos_y);
        }

        bool getLeftBefore1Frame() const {
            return left_before_1frame;
        }
        bool getLeft() const {
            return left;
        }
        bool getRight() const {
            return right;
        }
        bool getMiddle() const {
            return middle;
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
            right = s3d::MouseR.pressed();
            middle = s3d::MouseM.pressed();
            pos_x = s3d::Cursor::Pos().x;
            pos_y = s3d::Cursor::Pos().y;
#elif defined(PAXS_USING_DXLIB)
            wheel_rot_vol = DxLib::GetMouseWheelRotVol();
            int mouse_input = DxLib::GetMouseInput();
            left = ((mouse_input & MOUSE_INPUT_LEFT) != 0);
            right = ((mouse_input & MOUSE_INPUT_RIGHT) != 0);
            middle = ((mouse_input & MOUSE_INPUT_MIDDLE) != 0);
            if (DxLib::GetMousePoint(&pos_x, &pos_y) == -1) {
                pos_x = pos_y = 0;
            }
#elif defined(PAXS_USING_SFML)
            wheel_rot_vol = static_cast<int>(paxg::SFML_Event::getInstance()->wheel_delta);
            left = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            right = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            middle = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
            auto mouse_pos = sf::Mouse::getPosition(Window::window());
            pos_x = mouse_pos.x;
            pos_y = mouse_pos.y;
#endif
        }

        // SFML 3.0.0: より安全なマウス位置取得（std::optional）
        std::optional<paxs::Vector2<int>> tryGetPosition() const {
#if defined(PAXS_USING_SIV3D)
            return paxs::Vector2<int>(s3d::Cursor::Pos().x, s3d::Cursor::Pos().y);
#elif defined(PAXS_USING_DXLIB)
            int x = 0, y = 0;
            if (DxLib::GetMousePoint(&x, &y) == -1) {
                return std::nullopt;
            }
            return paxs::Vector2<int>(x, y);
#elif defined(PAXS_USING_SFML)
            // SFML 3.0 では常に成功するが、一貫性のためoptionalを返す
            return paxs::Vector2<int>(pos_x, pos_y);
#else
            return std::nullopt;
#endif
        }
    };

    // Static member definition
    Mouse Mouse::instance;

}

#endif // !PAX_GRAPHICA_MOUSE_HPP
