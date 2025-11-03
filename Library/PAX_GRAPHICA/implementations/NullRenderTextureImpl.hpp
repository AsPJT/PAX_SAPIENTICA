/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_RENDER_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_NULL_RENDER_TEXTURE_IMPL_HPP

#include <PAX_GRAPHICA/RenderTextureImpl.hpp>

namespace paxg {

    /// @brief Null implementation for platforms without render texture support
    /// @brief レンダーテクスチャ非対応プラットフォーム用のNull実装
    class NullRenderTextureImpl : public RenderTextureImpl {
    private:
        paxs::Vector2<int> size;

    public:
        NullRenderTextureImpl(int width, int height, double, double, double, double)
            : size(width, height) {}

        NullRenderTextureImpl(int width, int height)
            : size(width, height) {}

        NullRenderTextureImpl() : size(800, 600) {}

        paxs::Vector2<int> getSize() const override {
            return size;
        }

        void clear(double, double, double, double) override {}
        void beginRender() override {}
        void endRender() override {}
        void draw(int, int) const override {}
        void draw(int, int, double, double, double, double) const override {}
        void* getNativeHandle() override { return nullptr; }
        void resize(int new_width, int new_height) override {
            size = paxs::Vector2<int>(new_width, new_height);
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_NULL_RENDER_TEXTURE_IMPL_HPP
