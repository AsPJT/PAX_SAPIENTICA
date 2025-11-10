/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_BUTTON_HPP
#define PAX_MAHOROBA_I_BUTTON_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

namespace paxs {

    class IWidget : public IRenderable, public IInputHandler {
    public:
        virtual ~IWidget() = default;

        // 位置・サイズ管理
        // Position and size management
        virtual paxg::Rect getRect() const = 0;
        virtual void setPos(const paxg::Vec2i& pos) = 0;

        // 可視性管理
        // Visibility management
        void setVisible(bool visible) override = 0;
        bool isVisible() const override = 0;

        // 有効/無効管理
        // Enable/disable management
        virtual void setEnabled(bool enabled) = 0;
        bool isEnabled() const override = 0;

        // コンポーネント情報
        // Component information
        virtual const char* getName() const = 0;

        // IRenderable の実装
        // IRenderable implementation

        /// @brief レンダリング処理
        /// @brief Rendering process
        void render() const override = 0;

        /// @brief このオブジェクトが属するレンダリングレイヤーを取得
        /// @brief Get the rendering layer this object belongs to
        /// @return レンダリングレイヤー / Rendering layer
        RenderLayer getLayer() const override = 0;

        // IInputHandler の実装
        // IInputHandler implementation

        /// @brief ヒットテスト（getRect()を使用）
        /// @brief Hit test (uses getRect())
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 範囲内ならtrue / true if within bounds
        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            const paxg::Rect rect = getRect();
            return (x >= rect.x() && x < rect.x() + rect.w() &&
                y >= rect.y() && y < rect.y() + rect.h());
        }
    };

}

#endif // !PAX_MAHOROBA_I_BUTTON_HPP
