/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_TEXTURE_IMPL_HPP

#if defined(PAXS_USING_SIV3D)

#include <Siv3D.hpp>

#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/Interface/TextureImpl.hpp>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
namespace paxg {

    class Siv3DTextureImpl : public TextureImpl {
    private:
        s3d::Texture texture;

    public:
        Siv3DTextureImpl() = default;

        Siv3DTextureImpl(const paxg::Image& image) : texture(image) {}

        Siv3DTextureImpl(const paxg::String& path) : texture(s3d::Unicode::FromUTF8(paxs::AppConfig::getInstance().getRootPath()) + path.string) {}

        Siv3DTextureImpl(const std::string& path)
            : texture(static_cast<paxg::String>(paxs::AppConfig::getInstance().getRootPath() + path).string) {}

        bool isValid() const override {
            return !!texture;
        }

        int width() const override {
            return texture.width();
        }

        int height() const override {
            return texture.height();
        }

        void draw() const override {
            texture.draw();
        }

        void drawAt(const paxg::Vec2f& pos) const override {
            texture.drawAt(pos.x(), pos.y());
        }

        void drawAt(const paxg::Vec2i& pos) const override {
            texture.drawAt(pos.x(), pos.y());
        }

        void resizedDrawAt(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const override {
            texture.resized(resize.x(), resize.y()).drawAt(pos.x(), pos.y());
        }

        void resizedDrawAt(int resize, const paxg::Vec2i& pos) const override {
            texture.resized(resize).drawAt(pos.x(), pos.y());
        }

        void resizedDrawAt(const paxg::Vec2f& resize, const paxg::Vec2f& pos) const override {
            texture.resized(resize.x(), resize.y()).drawAt(pos.x(), pos.y());
        }

        void resizedDrawAt(int resize, const paxg::Vec2f& pos) const override {
            texture.resized(resize).drawAt(pos.x(), pos.y());
        }

        void resizedDraw(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const override {
            texture.resized(resize.x(), resize.y()).draw(pos.x(), pos.y());
        }

        void resizedDraw(int resize, const paxg::Vec2i& pos) const override {
            texture.resized(resize).draw(pos.x(), pos.y());
        }

        void resizedDraw(const paxg::Vec2f& resize, const paxg::Vec2f& pos) const override {
            texture.resized(resize.x(), resize.y()).draw(pos.x(), pos.y());
        }

        void resizedDraw(int resize, const paxg::Vec2f& pos) const override {
            texture.resized(resize).draw(pos.x(), pos.y());
        }

        const s3d::Texture& getNativeTexture() const { return texture; }
    };

} // namespace paxg

#endif // defined(PAXS_USING_SIV3D)

#endif // !PAX_GRAPHICA_SIV3D_TEXTURE_IMPL_HPP
