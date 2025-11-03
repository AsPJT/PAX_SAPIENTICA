/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_RENDER_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_RENDER_TEXTURE_IMPL_HPP

#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxg {

    /// @brief Abstract base class for render texture implementations
    /// @brief レンダーテクスチャ実装の抽象基底クラス
    class RenderTextureImpl {
    public:
        virtual ~RenderTextureImpl() = default;

        /// @brief Get the size of the render texture
        /// @return The size as a 2D vector
        virtual paxs::Vector2<int> getSize() const = 0;

        /// @brief Clear the render texture with a color
        /// @param r Red component (0.0-1.0)
        /// @param g Green component (0.0-1.0)
        /// @param b Blue component (0.0-1.0)
        /// @param a Alpha component (0.0-1.0)
        virtual void clear(double r, double g, double b, double a) = 0;

        /// @brief Begin rendering to this texture
        virtual void beginRender() = 0;

        /// @brief End rendering to this texture
        virtual void endRender() = 0;

        /// @brief Draw this render texture to the screen
        /// @param x X position
        /// @param y Y position
        virtual void draw(int x, int y) const = 0;

        /// @brief Draw this render texture with color modulation
        /// @param x X position
        /// @param y Y position
        /// @param r Red multiplier (0.0-1.0)
        /// @param g Green multiplier (0.0-1.0)
        /// @param b Blue multiplier (0.0-1.0)
        /// @param a Alpha multiplier (0.0-1.0)
        virtual void draw(int x, int y, double r, double g, double b, double a) const = 0;

        /// @brief Get native handle for render target operations (platform-specific)
        /// @return Pointer to platform-specific data
        virtual void* getNativeHandle() = 0;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_RENDER_TEXTURE_IMPL_HPP
