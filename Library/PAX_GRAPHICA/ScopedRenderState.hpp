/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SCOPED_RENDER_STATE_HPP
#define PAX_GRAPHICA_SCOPED_RENDER_STATE_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#include <memory>
#endif

namespace paxg {

    /// @brief Sampler state types for texture filtering
    /// @brief テクスチャフィルタリング用のサンプラーステート
    enum class SamplerState {
        ClampNearest,   ///< Clamp addressing with nearest neighbor filtering
        ClampLinear,    ///< Clamp addressing with linear filtering
        RepeatNearest,  ///< Repeat addressing with nearest neighbor filtering
        RepeatLinear    ///< Repeat addressing with linear filtering
    };

    /// @brief Blend state types for alpha blending
    /// @brief アルファブレンディング用のブレンドステート
    enum class BlendState {
        Opaque,        ///< 不透明ブレンディング
        Additive,      ///< 加算ブレンディング
        Multiplicative,///< 乗算ブレンディング
        MaxAlpha       ///< 最大アルファブレンディング
    };

    /// @brief RAII-style scoped render state manager for sampler settings
    /// @brief サンプラー設定用のRAIIスタイルスコープドレンダーステートマネージャー
    /// @details Automatically restores previous state when scope ends
    class ScopedSamplerState {
    private:
#if defined(PAXS_USING_SIV3D)
        std::unique_ptr<s3d::ScopedRenderStates2D> state;
#endif

    public:
        /// @brief Constructor with sampler state
        /// @param sampler The sampler state to apply
        explicit ScopedSamplerState(SamplerState sampler) {
#if defined(PAXS_USING_SIV3D)
            switch (sampler) {
                case SamplerState::ClampNearest:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::SamplerState::ClampNearest);
                    break;
                case SamplerState::ClampLinear:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::SamplerState::ClampLinear);
                    break;
                case SamplerState::RepeatNearest:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::SamplerState::RepeatNearest);
                    break;
                case SamplerState::RepeatLinear:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::SamplerState::RepeatLinear);
                    break;
            }
#else
            (void)sampler;
#endif
        }

        ~ScopedSamplerState() = default;

        // Non-copyable, non-movable (RAII semantics)
        ScopedSamplerState(const ScopedSamplerState&) = delete;
        ScopedSamplerState& operator=(const ScopedSamplerState&) = delete;
        ScopedSamplerState(ScopedSamplerState&&) = delete;
        ScopedSamplerState& operator=(ScopedSamplerState&&) = delete;
    };

    /// @brief RAII-style scoped render state manager for blend settings
    /// @brief ブレンド設定用のRAIIスタイルスコープドレンダーステートマネージャー
    /// @details Automatically restores previous state when scope ends
    class ScopedBlendState {
    private:
#if defined(PAXS_USING_SIV3D)
        std::unique_ptr<s3d::ScopedRenderStates2D> state;
#endif

    public:
        /// @brief Constructor with blend state
        /// @param blend The blend state to apply
        explicit ScopedBlendState(BlendState blend) {
#if defined(PAXS_USING_SIV3D)
            switch (blend) {
                case BlendState::Opaque:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::BlendState::Opaque);
                    break;
                case BlendState::Additive:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::BlendState::Additive);
                    break;
                case BlendState::Multiplicative:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::BlendState::Multiplicative);
                    break;
                case BlendState::MaxAlpha:
                    state = std::make_unique<s3d::ScopedRenderStates2D>(s3d::BlendState::MaxAlpha);
                    break;
            }
#else
            (void)blend;
#endif
        }

        ~ScopedBlendState() = default;

        // Non-copyable, non-movable (RAII semantics)
        ScopedBlendState(const ScopedBlendState&) = delete;
        ScopedBlendState& operator=(const ScopedBlendState&) = delete;
        ScopedBlendState(ScopedBlendState&&) = delete;
        ScopedBlendState& operator=(ScopedBlendState&&) = delete;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SCOPED_RENDER_STATE_HPP
