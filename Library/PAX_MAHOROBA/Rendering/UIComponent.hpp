/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_COMPONENT_HPP
#define PAX_MAHOROBA_UI_COMPONENT_HPP

#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

    /// @brief UI基底コンポーネント（表示専用）
    /// @brief Base UI component (display only)
    class UIComponent : public IRenderable {
    public:
        virtual ~UIComponent() = default;

        // 位置・サイズ管理
        // Position and size management

        /// @brief コンポーネントの矩形領域を取得
        /// @brief Get the rectangular area of the component
        /// @return 矩形領域 / Rectangular area
        virtual Rect<int> getRect() const = 0;

        /// @brief コンポーネントの位置を設定
        /// @brief Set the position of the component
        /// @param pos 新しい位置 / New position
        virtual void setPos(const Vector2<int>& pos) = 0;

        /// @brief コンポーネント名を取得
        /// @brief Get the component name
        /// @return コンポーネント名 / Component name
        virtual const char* getName() const = 0;

        /// @brief レンダリング処理
        /// @brief Rendering process
        void render() const override = 0;

        /// @brief このオブジェクトが属するレンダリングレイヤーを取得
        /// @brief Get the rendering layer this object belongs to
        /// @return レンダリングレイヤー / Rendering layer
        RenderLayer getLayer() const override = 0;

        /// @brief オブジェクトが表示可能かどうかを取得
        /// @brief Check if the object is visible
        /// @return 表示可能ならtrue / true if visible
        bool isVisible() const override = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_COMPONENT_HPP
