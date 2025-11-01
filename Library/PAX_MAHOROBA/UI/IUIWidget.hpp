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

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {

    /// @brief UIウィジェットの共通インターフェース
    /// @brief Common interface for UI widgets
    class IUIWidget : public IRenderable, public IInputHandler {
    public:
        virtual ~IUIWidget() = default;

        // 位置・サイズ管理
        virtual paxg::Rect getRect() const = 0;
        virtual void setPos(const paxg::Vec2i& pos) = 0;

        // 可視性管理
        void setVisible(bool visible) override = 0;
        bool isVisible() const override = 0;

        // 有効/無効管理
        virtual void setEnabled(bool enabled) = 0;
        bool isEnabled() const override = 0;

        // コンポーネント情報
        virtual const char* getName() const = 0;
        virtual bool isAvailable() const = 0;

        // IRenderable の実装
        // IRenderable implementation
        // render() は各サブクラスで実装必須
        // render() must be implemented by each subclass

        // getLayer() は各サブクラスで実装必須
        // getLayer() must be implemented by each subclass

        // IInputHandler の実装
        // IInputHandler implementation

        /// @brief ヒットテスト（getRect()を使用）
        /// @brief Hit test (uses getRect())
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 範囲内ならtrue / true if within bounds
        bool hitTest(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            const paxg::Rect rect = getRect();
            return (x >= rect.x() && x < rect.x() + rect.w() &&
                    y >= rect.y() && y < rect.y() + rect.h());
        }

        // handleInput() は各サブクラスで実装必須
        // handleInput() must be implemented by each subclass
    };

}

#endif // !PAX_MAHOROBA_I_UI_WIDGET_HPP
