/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_RENDERABLE_HPP
#define PAX_MAHOROBA_I_RENDERABLE_HPP

#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

    /// @brief 描画可能オブジェクトの基底インターフェース
    /// @brief Base interface for renderable objects
    ///
    /// このインターフェースを実装することで、オブジェクトはRenderLayerManagerによって
    /// Z順序に従って自動的に描画されるようになります。
    /// By implementing this interface, objects can be automatically rendered
    /// in Z-order by the RenderLayerManager.
    class IRenderable {
    public:
        /// @brief 仮想デストラクタ
        /// @brief Virtual destructor
        virtual ~IRenderable() = default;

        /// @brief オブジェクトを描画する
        /// @brief Render the object
        ///
        /// この関数は、RenderLayerManagerによってZ順序に従って呼び出されます。
        /// This function is called by the RenderLayerManager in Z-order.
        virtual void render() const = 0;

        /// @brief このオブジェクトが属するレンダリングレイヤーを取得
        /// @brief Get the rendering layer this object belongs to
        /// @return レンダリングレイヤー / Rendering layer
        virtual RenderLayer getLayer() const = 0;

        /// @brief オブジェクトが表示可能かどうかを取得
        /// @brief Check if the object is visible
        /// @return 表示可能ならtrue / true if visible
        virtual bool isVisible() const = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_I_RENDERABLE_HPP
