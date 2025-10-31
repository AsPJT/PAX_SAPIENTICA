/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_UI_WIDGET_HPP
#define PAX_MAHOROBA_I_UI_WIDGET_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {

    // UIウィジェットの共通インターフェース
    class IUIWidget {
    public:
        virtual ~IUIWidget() = default;

        // 基本操作
        virtual void update(InputStateManager& input_state_manager) = 0;
        virtual void draw() = 0;

        // 位置・サイズ管理
        virtual paxg::Rect getRect() const = 0;
        virtual void setPos(const paxg::Vec2i& pos) = 0;

        // 可視性管理
        virtual void setVisible(bool visible) = 0;
        virtual bool isVisible() const = 0;

        // 有効/無効管理
        virtual void setEnabled(bool enabled) = 0;
        virtual bool isEnabled() const = 0;
    };

}

#endif // !PAX_MAHOROBA_I_UI_WIDGET_HPP
