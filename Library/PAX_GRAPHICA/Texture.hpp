/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TEXTURE_HPP
#define PAX_GRAPHICA_TEXTURE_HPP

#include <memory>

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/implementations/Siv3DTextureImpl.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <PAX_GRAPHICA/implementations/DxLibTextureImpl.hpp>
#elif defined(PAXS_USING_SFML)
#include <PAX_GRAPHICA/implementations/SFMLTextureImpl.hpp>
#else
#include <PAX_GRAPHICA/implementations/NullTextureImpl.hpp>
#endif

#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/String.hpp>
#include <PAX_GRAPHICA/TextureImpl.hpp>

namespace paxg {

    /// @brief Texture class using pimpl idiom for multi-library support
    struct Texture {
    private:
        std::shared_ptr<TextureImpl> impl;

        template<typename... Args>
        void createImpl(Args&&... args) {
#if defined(PAXS_USING_SIV3D)
            impl = std::make_shared<Siv3DTextureImpl>(std::forward<Args>(args)...);
#elif defined(PAXS_USING_DXLIB)
            impl = std::make_shared<DxLibTextureImpl>(std::forward<Args>(args)...);
#elif defined(PAXS_USING_SFML)
            impl = std::make_shared<SFMLTextureImpl>(std::forward<Args>(args)...);
#else
            impl = std::make_shared<NullTextureImpl>(std::forward<Args>(args)...);
#endif
        }

    public:
        Texture() {
            createImpl();
        }

        Texture(const Image& image) {
            createImpl(image);
        }

        Texture(const String& path) {
            createImpl(path);
        }

        Texture(const std::string& path) {
            createImpl(path);
        }

        // Implicit conversion to bool for validity check
        explicit operator bool() const {
            return impl && impl->isValid();
        }

        bool operator!() const {
            return !impl || !impl->isValid();
        }

        int width() const {
            return impl ? impl->width() : 0;
        }

        int height() const {
            return impl ? impl->height() : 0;
        }

        void draw() const {
            if (impl) impl->draw();
        }

        void drawAt(const Vec2f& pos) const {
            if (impl) impl->drawAt(pos);
        }

        void drawAt(const Vec2i& pos) const {
            if (impl) impl->drawAt(pos);
        }

        void resizedDrawAt(const Vec2i& resize, const Vec2i& pos) const {
            if (impl) impl->resizedDrawAt(resize, pos);
        }

        void resizedDrawAt(int resize, const Vec2i& pos) const {
            if (impl) impl->resizedDrawAt(resize, pos);
        }

        void resizedDrawAt(const Vec2f& resize, const Vec2f& pos) const {
            if (impl) impl->resizedDrawAt(resize, pos);
        }

        void resizedDrawAt(int resize, const Vec2f& pos) const {
            if (impl) impl->resizedDrawAt(resize, pos);
        }

        void resizedDraw(const Vec2i& resize, const Vec2i& pos) const {
            if (impl) impl->resizedDraw(resize, pos);
        }

        void resizedDraw(int resize, const Vec2i& pos) const {
            if (impl) impl->resizedDraw(resize, pos);
        }

        void resizedDraw(const Vec2f& resize, const Vec2f& pos) const {
            if (impl) impl->resizedDraw(resize, pos);
        }

        void resizedDraw(int resize, const Vec2f& pos) const {
            if (impl) impl->resizedDraw(resize, pos);
        }

#if defined(PAXS_USING_SIV3D)
        // Native type conversion for Siv3D
        operator s3d::Texture() const {
            return impl ? static_cast<Siv3DTextureImpl*>(impl.get())->getNativeTexture() : s3d::Texture{};
        }
#elif defined(PAXS_USING_SFML)
        // Native type conversion for SFML
        operator sf::Texture() const {
            return impl ? static_cast<SFMLTextureImpl*>(impl.get())->getNativeTexture() : sf::Texture{};
        }
#endif
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_TEXTURE_HPP
