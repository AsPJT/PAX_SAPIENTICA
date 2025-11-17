/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_BUTTON_HPP
#define PAX_MAHOROBA_I_BUTTON_HPP

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

    class IWidget : public IRenderable, public IInputHandler {
    public:
        virtual ~IWidget() = default;

        // 位置・サイズ管理
        // Position and size management
        virtual Rect<int> getRect() const = 0;
        virtual void setPos(const Vector2<int>& pos) = 0;

        // 可視性管理
        // Visibility management
        bool isVisible() const override = 0;

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

        /// @brief ヒットテスト
        /// @param pos マウス座標 / Mouse position
        /// @return 範囲内ならtrue / true if within bounds
        bool isHit(const Vector2<int>& pos) const override {
            if (!isVisible()) return false;
            return getRect().contains(pos);
        }
    };

}

#endif // !PAX_MAHOROBA_I_BUTTON_HPP
