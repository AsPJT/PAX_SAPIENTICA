/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SCOPED_RENDER_TARGET_HPP
#define PAX_GRAPHICA_SCOPED_RENDER_TARGET_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#include <memory>
#endif

#include <PAX_GRAPHICA/ColorF.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>

namespace paxg {

    /// @brief RAII-style scoped render target for rendering to textures
    /// @brief テクスチャへのレンダリング用RAIIスタイルスコープドレンダーターゲット
    /// @details Automatically restores previous render target when scope ends
    class ScopedRenderTarget {
    private:
#if defined(PAXS_USING_SIV3D)
        std::unique_ptr<s3d::ScopedRenderTarget2D> target;
#endif

    public:
        /// @brief Constructor that sets render target and clears with color
        /// @param render_texture The render texture to draw to
        /// @param clear_color Color to clear the texture with
        ScopedRenderTarget(RenderTexture& render_texture, const ColorF& clear_color) {
#if defined(PAXS_USING_SIV3D)
            auto* native = static_cast<s3d::RenderTexture*>(render_texture.getNativeHandle());
            if (native) {
                target = std::make_unique<s3d::ScopedRenderTarget2D>(
                    native->clear(s3d::ColorF(clear_color.r, clear_color.g, clear_color.b, clear_color.a))
                );
            }
#else
            (void)render_texture;
            (void)clear_color;
#endif
        }

        ~ScopedRenderTarget() = default;

        // Non-copyable, non-movable (RAII semantics)
        ScopedRenderTarget(const ScopedRenderTarget&) = delete;
        ScopedRenderTarget& operator=(const ScopedRenderTarget&) = delete;
        ScopedRenderTarget(ScopedRenderTarget&&) = delete;
        ScopedRenderTarget& operator=(ScopedRenderTarget&&) = delete;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SCOPED_RENDER_TARGET_HPP
