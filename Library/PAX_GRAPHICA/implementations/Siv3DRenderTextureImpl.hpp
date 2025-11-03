/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_RENDER_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_RENDER_TEXTURE_IMPL_HPP

#include <PAX_GRAPHICA/RenderTextureImpl.hpp>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>

namespace paxg {

    /// @brief Siv3D implementation of render texture
    /// @brief Siv3D用のレンダーテクスチャ実装
    class Siv3DRenderTextureImpl : public RenderTextureImpl {
    private:
        s3d::RenderTexture render_texture;

    public:
        /// @brief Constructor with size and color
        /// @param width Width in pixels
        /// @param height Height in pixels
        /// @param r Red component (0.0-1.0)
        /// @param g Green component (0.0-1.0)
        /// @param b Blue component (0.0-1.0)
        /// @param a Alpha component (0.0-1.0)
        Siv3DRenderTextureImpl(int width, int height, double r, double g, double b, double a)
            : render_texture(s3d::Size(width, height), s3d::ColorF(r, g, b, a)) {}

        /// @brief Constructor with window size
        Siv3DRenderTextureImpl()
            : render_texture(s3d::Scene::Size(), s3d::ColorF(1.0, 0.0)) {}

        /// @brief Constructor with size only (uses Siv3D's default color)
        /// @param width Width in pixels
        /// @param height Height in pixels
        Siv3DRenderTextureImpl(int width, int height)
            : render_texture(s3d::Size(width, height)) {}

        paxs::Vector2<int> getSize() const override {
            auto size = render_texture.size();
            return paxs::Vector2<int>(size.x, size.y);
        }

        void clear(double r, double g, double b, double a) override {
            render_texture.clear(s3d::ColorF(r, g, b, a));
        }

        void beginRender() override {
            // Siv3Dではスコープベースなので、実際にはScopedRenderTarget2Dを使う必要がある
            // ここでは直接的な実装として空にする
        }

        void endRender() override {
            // Siv3Dではスコープベースなので空
        }

        void draw(int x, int y) const override {
            render_texture.draw(x, y);
        }

        void draw(int x, int y, double r, double g, double b, double a) const override {
            render_texture.draw(x, y, s3d::ColorF(r, g, b, a));
        }

        void* getNativeHandle() override {
            return &render_texture;
        }

        /// @brief Get the internal Siv3D render texture
        s3d::RenderTexture& getInternal() {
            return render_texture;
        }

        const s3d::RenderTexture& getInternal() const {
            return render_texture;
        }
    };

} // namespace paxg

#endif // PAXS_USING_SIV3D

#endif // !PAX_GRAPHICA_SIV3D_RENDER_TEXTURE_IMPL_HPP
