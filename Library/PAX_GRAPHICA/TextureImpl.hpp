/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_TEXTURE_IMPL_HPP

#include <string>

#include <PAX_GRAPHICA/String.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>

namespace paxg {

    /// @brief Abstract base class for texture implementations
    class TextureImpl {
    public:
        virtual ~TextureImpl() = default;

        virtual bool isValid() const = 0;
        virtual int width() const = 0;
        virtual int height() const = 0;

        virtual void draw() const = 0;
        virtual void drawAt(const paxg::Vec2f& pos) const = 0;
        virtual void drawAt(const paxg::Vec2i& pos) const = 0;

        virtual void resizedDrawAt(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const = 0;
        virtual void resizedDrawAt(int resize, const paxg::Vec2i& pos) const = 0;
        virtual void resizedDrawAt(const paxg::Vec2f& resize, const paxg::Vec2f& pos) const = 0;
        virtual void resizedDrawAt(int resize, const paxg::Vec2f& pos) const = 0;

        virtual void resizedDraw(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const = 0;
        virtual void resizedDraw(int resize, const paxg::Vec2i& pos) const = 0;
        virtual void resizedDraw(const paxg::Vec2f& resize, const paxg::Vec2f& pos) const = 0;
        virtual void resizedDraw(int resize, const paxg::Vec2f& pos) const = 0;

    protected:
        // Helper to convert SVG paths to PNG
        static std::string convertSvgToPng(std::string path) {
            paxs::StringUtils::replace(path, ".svg", ".png");
            return path;
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_TEXTURE_IMPL_HPP
