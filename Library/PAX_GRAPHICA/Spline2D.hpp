/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SPLINE_2D_HPP
#define PAX_GRAPHICA_SPLINE_2D_HPP

#include <memory>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/Spline2DImpl.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

#if defined(PAXS_USING_SIV3D)
#include <PAX_GRAPHICA/Siv3D/Siv3DSpline2DImpl.hpp>
#else
#include <PAX_GRAPHICA/Components/GenericSpline2DImpl.hpp>
#endif

namespace paxg {

    /// @brief 2D spline class with Pimpl idiom for graphics library abstraction
    /// @note Uses shared_ptr for value semantics and automatic resource management
    /// @note Siv3D uses native s3d::Spline2D, others use self-implemented Catmull-Rom interpolation
    class Spline2D {
    private:
        std::shared_ptr<Spline2DImpl> impl;

        /// @brief Create implementation based on the active graphics library
        /// @tparam T Vector component type (int, float, or double)
        /// @param points Control points for the spline
        /// @param is_closed Whether the spline is a closed loop
        /// @return Shared pointer to the implementation
        template<typename T>
        static std::shared_ptr<Spline2DImpl> createImpl(const std::vector<paxs::Vector2<T>>& points, bool is_closed) {
#if defined(PAXS_USING_SIV3D)
            return std::make_shared<Siv3DSpline2DImpl>(points, is_closed);
#else
            return std::make_shared<GenericSpline2DImpl>(points, is_closed);
#endif
        }

    public:
        /// @brief Default constructor
        Spline2D() = default;

        /// @brief Constructor with Vector2<double> points
        /// @param points Control points for the spline
        /// @param is_closed Whether the spline is a closed loop (default: false, auto-detected for Generic)
        explicit Spline2D(const std::vector<paxs::Vector2<double>>& points, bool is_closed = false)
            : impl(createImpl(points, is_closed)) {}

        /// @brief Constructor with Vector2<float> points
        /// @param points Control points for the spline
        /// @param is_closed Whether the spline is a closed loop (default: false, auto-detected for Generic)
        explicit Spline2D(const std::vector<paxs::Vector2<float>>& points, bool is_closed = false)
            : impl(createImpl(points, is_closed)) {}

        /// @brief Constructor with Vector2<int> points
        /// @param points Control points for the spline
        /// @param is_closed Whether the spline is a closed loop (default: false, auto-detected for Generic)
        explicit Spline2D(const std::vector<paxs::Vector2<int>>& points, bool is_closed = false)
            : impl(createImpl(points, is_closed)) {}

        /// @brief Draw the spline with specified thickness and color
        /// @param thickness Line thickness
        /// @param color Line color
        void draw(double thickness, const Color& color) const {
            if (impl) {
                impl->draw(thickness, color);
            }
        }

        /// @brief Draw the spline with default thickness (1.0) and specified color
        /// @param color Line color
        void draw(const Color& color) const {
            draw(1.0, color);
        }

#if defined(PAXS_USING_SIV3D)
        /// @brief Conversion operator to s3d::Spline2D (Siv3D only)
        operator s3d::Spline2D() const {
            auto* siv3dImpl = dynamic_cast<Siv3DSpline2DImpl*>(impl.get());
            if (siv3dImpl) {
                return siv3dImpl->getNativeSpline();
            }
            return s3d::Spline2D();
        }
#endif
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SPLINE_2D_HPP
