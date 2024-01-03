/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_MOUSE_HPP
#define PAX_GRAPHICA_MOUSE_HPP

/*##########################################################################################

##########################################################################################*/

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
        int getWheelRotVol() {
            return wheel_rot_vol;
        }

        // 毎フレーム最初に呼び出す
        void calledFirstEveryFrame() {
            // 以前の位置を 1 フレーム前の位置へ移動
            pos_x_before_1frame = pos_x;
            pos_y_before_1frame = pos_y;

#if defined(PAXS_USING_SIV3D)
            wheel_rot_vol = s3d::Mouse::Wheel();
#elif defined(PAXS_USING_DXLIB)
            if (DxLib::GetMousePoint(&pos_x, &pos_y) == -1) {
                pos_x = pos_y = 0;
            }
            wheel_rot_vol = DxLib::GetMouseWheelRotVol();

#elif defined(PAXS_USING_SFML)
            wheel_rot_vol = paxg::SFML_Event::getInstance()->wheel_delta;
#endif
        }

    private:
        static Mouse* instance;

        int pos_x_before_1frame = 0; // 1 フレーム前の位置
        int pos_y_before_1frame = 0; // 1 フレーム前の位置

        int pos_x = -1, pos_y = -1;

        bool left = false;
        bool right = false;
        bool middle = false;

        int wheel_rot_vol = 0;

        Mouse() = default;

        ~Mouse() {
            delete instance;
        }

    };

    Mouse* Mouse::instance = nullptr;

}

#endif // !PAX_GRAPHICA_MOUSE_HPP
