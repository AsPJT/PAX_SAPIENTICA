/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_INTERFACE_SPLINE_2D_IMPL_HPP
#define PAX_GRAPHICA_INTERFACE_SPLINE_2D_IMPL_HPP

#include <PAX_GRAPHICA/Color.hpp>

namespace paxg {

    /// @brief Abstract interface for 2D spline implementations
    /// @note Different graphics libraries may have different spline implementations
    class Spline2DImpl {
    public:
        virtual ~Spline2DImpl() = default;

        /// @brief Draw the spline with specified thickness and color
        /// @param thickness Line thickness
        /// @param color Line color
        virtual void draw(double thickness, const Color& color) const = 0;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_INTERFACE_SPLINE_2D_IMPL_HPP
