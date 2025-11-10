/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_NULL_TEXTURE_IMPL_HPP

#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/TextureImpl.hpp>

namespace paxg {

    /// @brief Null implementation for when no graphics library is available
    class NullTextureImpl : public TextureImpl {
    public:
        NullTextureImpl() = default;
        NullTextureImpl(const Image&) {}
        NullTextureImpl(const String&) {}
        NullTextureImpl(const std::string&) {}

        bool isValid() const override { return false; }
        int width() const override { return 0; }
        int height() const override { return 0; }

        void draw() const override {}
        void drawAt(const Vec2f&) const override {}
        void drawAt(const Vec2i&) const override {}

        void resizedDrawAt(const Vec2i&, const Vec2i&) const override {}
        void resizedDrawAt(int, const Vec2i&) const override {}
        void resizedDrawAt(const Vec2f&, const Vec2f&) const override {}
        void resizedDrawAt(int, const Vec2f&) const override {}

        void resizedDraw(const Vec2i&, const Vec2i&) const override {}
        void resizedDraw(int, const Vec2i&) const override {}
        void resizedDraw(const Vec2f&, const Vec2f&) const override {}
        void resizedDraw(int, const Vec2f&) const override {}
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_NULL_TEXTURE_IMPL_HPP
