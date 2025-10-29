/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SHADER_HPP
#define PAX_GRAPHICA_SHADER_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#endif

#include <PAX_GRAPHICA/RenderTexture.hpp>

namespace paxg {

    /// @brief Shader effects wrapper for cross-platform shader operations
    /// @brief クロスプラットフォームシェーダー操作用のシェーダーエフェクトラッパー
    class Shader {
    public:
        /// @brief Apply Gaussian blur effect
        /// @brief ガウシアンブラーエフェクトを適用
        /// @param source Source render texture
        /// @param temporary Temporary render texture for intermediate results
        /// @param destination Destination render texture
        /// @details Applies a two-pass Gaussian blur for smooth results
        static void GaussianBlur(RenderTexture& source, RenderTexture& temporary, RenderTexture& destination) {
#if defined(PAXS_USING_SIV3D)
            s3d::Shader::GaussianBlur(
                source.getInternalSiv3D(),
                temporary.getInternalSiv3D(),
                destination.getInternalSiv3D()
            );
#else
            (void)source;
            (void)temporary;
            (void)destination;
#endif
        }

        /// @brief Apply Gaussian blur effect (overload for direct parameter passing)
        /// @param source Source render texture (will be read)
        /// @param temp Temporary render texture
        /// @param dest Destination render texture (will be written)
        static void applyGaussianBlur(RenderTexture& source, RenderTexture& temp, RenderTexture& dest) {
            GaussianBlur(source, temp, dest);
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SHADER_HPP
