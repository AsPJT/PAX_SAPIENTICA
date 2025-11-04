/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_WIDGET_HPP
#define PAX_MAHOROBA_I_WIDGET_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Input/IMouseEventHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

namespace paxs {

    /// @brief ウィジェット（UI、マップオブジェクト等）の共通インターフェース
    /// @brief Common interface for widgets (UI, map objects, etc.)
    class IWidget : public IRenderable, public IMouseEventHandler {
    public:
        virtual ~IWidget() = default;

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

        /// @brief レンダリング処理
        /// @brief Rendering process
        void render() const override = 0;

        /// @brief このオブジェクトが属するレンダリングレイヤーを取得
        /// @brief Get the rendering layer this object belongs to
        /// @return レンダリングレイヤー / Rendering layer
        ///
        RenderLayer getLayer() const override = 0;

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

        // handleMouseInput() は各サブクラスで実装必須
        // handleMouseInput() must be implemented by each subclass
        /// @brief マウス入力処理
        /// @brief Handle mouse input
        /// @param event マウスイベント / Mouse event
        /// @return イベント処理結果 / Event handling result
        virtual EventHandlingResult handleMouseInput(const MouseEvent& event) = 0;
    };

}

#endif // !PAX_MAHOROBA_I_WIDGET_HPP
