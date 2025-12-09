/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_CIRCLE_HPP
#define PAX_GRAPHICA_CIRCLE_HPP

#include <memory>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/CircleImpl.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/Siv3D/Siv3DCircleImpl.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <PAX_GRAPHICA/DxLib/DxLibCircleImpl.hpp>
#elif defined(PAXS_USING_SFML)
#include <PAX_GRAPHICA/SFML/SFMLCircleImpl.hpp>
#else
#include <PAX_GRAPHICA/Null/NullCircleImpl.hpp>
#endif

namespace paxg {

    /// @brief Circle class with Pimpl idiom for graphics library abstraction
    /// @note Uses shared_ptr for value semantics and automatic resource management
    struct Circle {
    private:
        std::shared_ptr<CircleImpl> impl;

        /// @brief Create implementation based on the active graphics library
        /// @param x X-coordinate of the circle center
        /// @param y Y-coordinate of the circle center
        /// @param r Radius of the circle
        /// @return Shared pointer to the implementation
        static std::shared_ptr<CircleImpl> createImpl(float x, float y, float r) {
#if defined(PAXS_USING_SIV3D)
            return std::make_shared<Siv3DCircleImpl>(x, y, r);
#elif defined(PAXS_USING_DXLIB)
            return std::make_shared<DxLibCircleImpl>(x, y, r);
#elif defined(PAXS_USING_SFML)
            return std::make_shared<SFMLCircleImpl>(x, y, r);
#else
            return std::make_shared<NullCircleImpl>(x, y, r);
#endif
        }

    public:
        /// @brief Constructor with separate x, y coordinates and radius
        /// @param x X-coordinate of the circle center
        /// @param y Y-coordinate of the circle center
        /// @param r Radius of the circle
        Circle(float x, float y, float r)
            : impl(createImpl(x, y, r)) {}

        /// @brief Constructor with Vector2<int> position and radius
        /// @param pos Position of the circle center
        /// @param r Radius of the circle
        Circle(const paxs::Vector2<int>& pos, const float r)
            : impl(createImpl(static_cast<float>(pos.x), static_cast<float>(pos.y), r)) {}

        /// @brief Constructor with Vector2<float> position and radius
        /// @param pos Position of the circle center
        /// @param r Radius of the circle
        Circle(const paxs::Vector2<float>& pos, const float r)
            : impl(createImpl(pos.x, pos.y, r)) {}

        /// @brief Constructor with Vector2<double> position and radius
        /// @param pos Position of the circle center
        /// @param r Radius of the circle
        Circle(const paxs::Vector2<double>& pos, const float r)
            : impl(createImpl(static_cast<float>(pos.x), static_cast<float>(pos.y), r)) {}

        /// @brief Draw the circle without color
        void draw() const {
            if (impl) {
                impl->draw();
            }
        }

        /// @brief Draw the circle with specified color
        /// @param color The color to draw the circle
        void draw(const Color& color) const {
            if (impl) {
                impl->draw(color);
            }
        }

        /// @brief Get the position of the circle center
        /// @return Position as Vector2<float>
        paxs::Vector2<float> getPosition() const {
            return impl ? impl->getPosition() : paxs::Vector2<float>(0.0f, 0.0f);
        }

        /// @brief Get the radius of the circle
        float getRadius() const {
            return impl ? impl->getRadius() : 0.0f;
        }

#if defined(PAXS_USING_SIV3D)
        /// @brief Conversion operator to s3d::Circle (Siv3D only)
        operator s3d::Circle() const {
            auto* siv3dImpl = dynamic_cast<Siv3DCircleImpl*>(impl.get());
            if (siv3dImpl) {
                return siv3dImpl->getNativeCircle();
            }
            return s3d::Circle(0, 0, 0);
        }
#endif
    };
}

#endif // !PAX_GRAPHICA_CIRCLE_HPP
