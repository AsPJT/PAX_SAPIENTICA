/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INTERACTIVE_UI_COMPONENT_HPP
#define PAX_MAHOROBA_INTERACTIVE_UI_COMPONENT_HPP

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/UIComponent.hpp>

namespace paxs {

    /// @brief インタラクティブUI基底コンポーネント（表示+入力処理）
    /// @brief Base interactive UI component (display + input handling)
    class InteractiveUIComponent : public UIComponent, public IInputHandler {
    public:
        ~InteractiveUIComponent() override = default;

        // コピー・ムーブ操作はデフォルト
        InteractiveUIComponent(const InteractiveUIComponent&) = default;
        InteractiveUIComponent& operator=(const InteractiveUIComponent&) = default;
        InteractiveUIComponent(InteractiveUIComponent&&) = default;
        InteractiveUIComponent& operator=(InteractiveUIComponent&&) = default;

        /// @brief ヒットテスト
        /// @brief Hit test
        /// @param pos マウス座標 / Mouse position
        /// @return 範囲内ならtrue / true if within bounds
        ///
        /// デフォルト実装では、getRect()の範囲内かつisVisible()がtrueの場合にヒットと判定します。
        /// Default implementation checks if the position is within getRect() and isVisible() is true.
        bool isHit(const Vector2<int>& pos) const override {
            if (!isVisible()) {
                return false;
            }
            return getRect().contains(pos);
        }

        /// @brief このオブジェクトが属する入力処理レイヤーを取得
        /// @brief Get the input handling layer this object belongs to
        /// @return レンダリングレイヤー（入力優先度として使用）/ Rendering layer (used as input priority)
        using UIComponent::getLayer;

    protected:
        InteractiveUIComponent() = default;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_INTERACTIVE_UI_COMPONENT_HPP
