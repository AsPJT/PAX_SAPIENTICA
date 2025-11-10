/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SCOPED_TRANSFORM_2D_HPP
#define PAX_GRAPHICA_SCOPED_TRANSFORM_2D_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#include <memory>
#endif

namespace paxg {

    /// @brief RAII-style scoped 2D transformation
    /// @brief RAIIスタイルスコープド2D変換
    /// @details Automatically restores previous transformation when scope ends
    class ScopedTransform2D {
    private:
#if defined(PAXS_USING_SIV3D)
        std::unique_ptr<s3d::Transformer2D> transformer;
#endif

    public:
        /// @brief Constructor with translation
        /// @param x X translation offset
        /// @param y Y translation offset
        ScopedTransform2D(double x, double y) {
#if defined(PAXS_USING_SIV3D)
            transformer = std::make_unique<s3d::Transformer2D>(s3d::Mat3x2::Translate(x, y));
#else
            (void)x;
            (void)y;
            // Other libraries don't support scoped transformations
#endif
        }

        ~ScopedTransform2D() = default;

        // Non-copyable, non-movable (RAII semantics)
        ScopedTransform2D(const ScopedTransform2D&) = delete;
        ScopedTransform2D& operator=(const ScopedTransform2D&) = delete;
        ScopedTransform2D(ScopedTransform2D&&) = delete;
        ScopedTransform2D& operator=(ScopedTransform2D&&) = delete;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SCOPED_TRANSFORM_2D_HPP
