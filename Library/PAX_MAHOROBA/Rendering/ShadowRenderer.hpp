/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_SHADOW_RENDERER_HPP
#define PAX_MAHOROBA_SHADOW_RENDERER_HPP

#include <functional>

#include <PAX_GRAPHICA/ColorF.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/ScopedRenderState.hpp>
#include <PAX_GRAPHICA/ScopedRenderTarget.hpp>
#include <PAX_GRAPHICA/ScopedTransform2D.hpp>
#include <PAX_GRAPHICA/Shader.hpp>

namespace paxs {

    // 影効果のレンダリングユーティリティ
    // ガウスぼかしを使った柔らかい影を生成
    class ShadowRenderer {
    public:
        // 影を描画
        // shadow_texture: 影を描くレンダーテクスチャ
        // internal_texture: ぼかし処理用の内部テクスチャ
        // draw_shapes: 影の形状を描画するコールバック関数
        // shadow_color: 影の色（デフォルト: 黒、透明度0.5）
        // blur_iterations: ぼかしの回数（デフォルト: 2回）
        static void renderShadow(
            paxg::RenderTexture& shadow_texture,
            paxg::RenderTexture& internal_texture,
            std::function<void()> draw_shapes,
            const paxg::ColorF& shadow_color = paxg::ColorF{ 0.0, 0.5 },
            int blur_iterations = 2
        ) {
            // 影を作る図形を shadow_texture に描く
            {
                const paxg::ScopedRenderTarget target{ shadow_texture, paxg::ColorF{ 1.0, 0.0 } };
                const paxg::ScopedBlendState blend{ paxg::BlendState::MaxAlpha };
                const paxg::ScopedTransform2D transform{ 3.0, 3.0 };

                // 影の形状を描画
                draw_shapes();
            }

            // shadow_texture をガウスぼかし
            for (int i = 0; i < blur_iterations; ++i) {
                paxg::Shader::GaussianBlur(shadow_texture, internal_texture, shadow_texture);
            }

            // 影を描画
            shadow_texture.draw(shadow_color);
        }

        // shadow_texture: 影を描くレンダーテクスチャ
        // internal_texture: ぼかし処理用の内部テクスチャ
        // draw_shadow_shapes: 影の形状を描画するコールバック関数
        // draw_panels: パネル本体を描画するコールバック関数
        // shadow_color: 影の色（デフォルト: 黒、透明度0.5）
        // blur_iterations: ぼかしの回数（デフォルト: 2回）
        static void renderShadowWithPanels(
            paxg::RenderTexture& shadow_texture,
            paxg::RenderTexture& internal_texture,
            std::function<void()> draw_shadow_shapes,
            std::function<void()> draw_panels,
            const paxg::ColorF& shadow_color = paxg::ColorF{ 0.0, 0.5 },
            int blur_iterations = 2
        ) {
            // 影を描画
            renderShadow(shadow_texture, internal_texture, draw_shadow_shapes, shadow_color, blur_iterations);

            // パネル本体を描画（影の上に重ねる）
            draw_panels();
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_SHADOW_RENDERER_HPP
