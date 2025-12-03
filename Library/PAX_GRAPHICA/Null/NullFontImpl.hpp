/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_FONT_IMPL_HPP
#define PAX_GRAPHICA_NULL_FONT_IMPL_HPP

#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/FontImpl.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    /// @brief Null font implementation (no-op)
    /// @brief Nullフォント実装（何もしない）
    class NullFontImpl : public FontImpl {
    public:
        NullFontImpl(int /*size*/, const std::string& /*path*/, int /*buffer_thickness*/) {
            // No operation
        }

        void setOutline(double /*inner*/, double /*outer*/, const Color& /*color*/) override {
            // No operation
        }

        void drawBottomLeft(const std::string& /*str*/, const paxs::Vector2<int>& /*pos*/, const Color& /*color*/) const override {
            // No operation
        }

        void drawTopRight(const std::string& /*str*/, const paxs::Vector2<int>& /*pos*/, const Color& /*color*/) const override {
            // No operation
        }

        void drawBottomRight(const std::string& /*str*/, const paxs::Vector2<int>& /*pos*/, const Color& /*color*/) const override {
            // No operation
        }

        void draw(const std::string& /*str*/, const paxs::Vector2<int>& /*pos*/, const Color& /*color*/) const override {
            // No operation
        }

        void drawBottomCenter(const std::string& /*str*/, const paxs::Vector2<int>& /*pos*/, const Color& /*color*/) const override {
            // No operation
        }

        void drawTopCenter(const std::string& /*str*/, const paxs::Vector2<int>& /*pos*/, const Color& /*color*/) const override {
            // No operation
        }

        void drawAt(const std::string& /*str*/, const paxs::Vector2<int>& /*pos*/, const Color& /*color*/) const override {
            // No operation
        }

        int height() const override {
            return 0;
        }

        int width(const std::string& /*str*/) override {
            return 0;
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_NULL_FONT_IMPL_HPP
