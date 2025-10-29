/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_RENDER_TEXTURE_HPP
#define PAX_GRAPHICA_RENDER_TEXTURE_HPP

#include <memory>

#include <PAX_GRAPHICA/ColorF.hpp>
#include <PAX_GRAPHICA/RenderTextureImpl.hpp>
#include <PAX_GRAPHICA/implementations/NullRenderTextureImpl.hpp>

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/implementations/Siv3DRenderTextureImpl.hpp>
#endif

namespace paxg {

    /// @brief Cross-platform render texture wrapper for offscreen rendering
    /// @brief オフスクリーンレンダリング用のクロスプラットフォームレンダーテクスチャラッパー
    /// @details Uses Pimpl idiom to hide platform-specific implementations
    class RenderTexture {
    private:
        std::shared_ptr<RenderTextureImpl> impl;

    public:
        /// @brief Default constructor (creates window-sized render texture)
        RenderTexture() {
#if defined(PAXS_USING_SIV3D)
            impl = std::make_shared<Siv3DRenderTextureImpl>();
#else
            impl = std::make_shared<NullRenderTextureImpl>();
#endif
        }

        /// @brief Constructor with size and color
        /// @param size Size as Vector2<int>
        /// @param color Color as ColorF
        RenderTexture(const paxs::Vector2<int>& size, const ColorF& color) {
#if defined(PAXS_USING_SIV3D)
            impl = std::make_shared<Siv3DRenderTextureImpl>(size.x, size.y, color.r, color.g, color.b, color.a);
#else
            impl = std::make_shared<NullRenderTextureImpl>(size.x, size.y, color.r, color.g, color.b, color.a);
#endif
        }

        /// @brief Constructor with size only
        /// @param size Size as Vector2<int>
        explicit RenderTexture(const paxs::Vector2<int>& size) {
#if defined(PAXS_USING_SIV3D)
            impl = std::make_shared<Siv3DRenderTextureImpl>(size.x, size.y);
#else
            impl = std::make_shared<NullRenderTextureImpl>(size.x, size.y);
#endif
        }

        /// @brief Get the size of the render texture
        /// @return The size as a 2D vector
        paxs::Vector2<int> size() const {
            return impl->getSize();
        }

        /// @brief Clear the render texture with a color
        /// @param r Red component (0.0-1.0)
        /// @param g Green component (0.0-1.0)
        /// @param b Blue component (0.0-1.0)
        /// @param a Alpha component (0.0-1.0)
        void clear(double r, double g, double b, double a) {
            impl->clear(r, g, b, a);
        }

        /// @brief Begin rendering to this texture
        void beginRender() {
            impl->beginRender();
        }

        /// @brief End rendering to this texture
        void endRender() {
            impl->endRender();
        }

        /// @brief Draw this render texture to the screen
        /// @param x X position
        /// @param y Y position
        void draw(int x = 0, int y = 0) const {
            impl->draw(x, y);
        }

        /// @brief Draw this render texture with color modulation
        /// @param color Color multiplier
        void draw(const ColorF& color) const {
            impl->draw(0, 0, color.r, color.g, color.b, color.a);
        }

        /// @brief Draw this render texture at position with color modulation
        /// @param x X position
        /// @param y Y position
        /// @param color Color multiplier
        void draw(int x, int y, const ColorF& color) const {
            impl->draw(x, y, color.r, color.g, color.b, color.a);
        }

        /// @brief Get native handle for platform-specific operations
        /// @return Pointer to platform-specific render texture
        void* getNativeHandle() {
            return impl->getNativeHandle();
        }

#if defined(PAXS_USING_SIV3D)
        /// @brief Get the internal Siv3D render texture (Siv3D only)
        /// @return Reference to the internal s3d::RenderTexture
        s3d::RenderTexture& getInternalSiv3D() {
            auto siv3d_impl = std::dynamic_pointer_cast<Siv3DRenderTextureImpl>(impl);
            return siv3d_impl->getInternal();
        }

        const s3d::RenderTexture& getInternalSiv3D() const {
            auto siv3d_impl = std::dynamic_pointer_cast<const Siv3DRenderTextureImpl>(
                std::const_pointer_cast<RenderTextureImpl>(impl));
            return siv3d_impl->getInternal();
        }
#endif
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_RENDER_TEXTURE_HPP
