/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_PANEL_BACKGROUND_HPP
#define PAX_MAHOROBA_UI_PANEL_BACKGROUND_HPP

#include <functional>
#include <map>
#include <vector>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>

#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>
#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>

namespace paxs {

    /// @brief UIパネルの背景と影を描画するクラス
    /// @brief Class for rendering UI panel background and shadow
    /// Siv3Dでは影を一度にまとめて描画することで、影の重複を防ぎます。
    /// For Siv3D, renders all shadows at once to prevent shadow duplication.
    class PanelBackgroundRenderer {
    public:
        /// @brief 影用のテクスチャを初期化（静的メソッド）
        /// @brief Initialize textures for shadow rendering (static method)
        /// @param size テクスチャサイズ / Texture size
        ///
        /// UI初期化時に一度だけ呼び出してください。
        /// Should be called only once during UI initialization.
        static void initShadowTextures(const paxs::Vector2<int>& size) {
#ifdef PAXS_USING_SIV3D
            if (shadow_texture_.size().x <= 0 || shadow_texture_.size().y <= 0) {
                shadow_texture_ = paxg::RenderTexture{ size, paxg::ColorF{ 1.0, 0.0 } };
                internal_texture_ = paxg::RenderTexture{ size };
            }
#else
            (void)size; // 未使用パラメータの警告を抑制
#endif
        }

        /// @brief バッチ描画の開始（Siv3D用）
        /// @brief Begin batch rendering (for Siv3D)
        ///
        /// 複数のパネルの影をまとめて描画するため、フレームの最初に呼び出します。
        /// Call this at the beginning of the frame to batch render multiple panel shadows.
        static void beginBatch() {
#ifdef PAXS_USING_SIV3D
            shadow_shapes_map_.clear();
            panel_shapes_map_.clear();
#endif
        }

        /// @brief バッチ描画の終了と実行（Siv3D用）
        /// @brief End and execute batch rendering (for Siv3D)
        ///
        /// 登録されたすべてのパネルの影をレイヤー順に描画します。
        /// Renders all registered panel shadows in layer order.
        static void endBatch() {
#ifdef PAXS_USING_SIV3D
            // テクスチャが初期化されているか確認
            if (shadow_texture_.size().x <= 0 || shadow_texture_.size().y <= 0) return;

            // レイヤー順（小→大）に描画
            for (auto& [layer, shapes] : shadow_shapes_map_) {
                if (shapes.empty()) continue;

                // このレイヤーのすべての影を一度に描画
                paxs::ShadowRenderer::renderShadowWithPanels(
                    shadow_texture_,
                    internal_texture_,
                    [&shapes]() {
                        // すべての影の形状を描画
                        for (const auto& draw_func : shapes) {
                            draw_func();
                        }
                    },
                    [layer]() {
                        // すべてのパネル本体を描画
                        for (const auto& draw_func : panel_shapes_map_[layer]) {
                            draw_func();
                        }
                    }
                );
            }
#endif
        }

        /// @brief 背景と影を描画（角丸）
        /// @brief Draw background and shadow (rounded corners)
        /// @param x パネルのX座標 / Panel X coordinate
        /// @param y パネルのY座標 / Panel Y coordinate
        /// @param width パネルの幅 / Panel width
        /// @param height パネルの高さ / Panel height
        /// @param corner_radius 角の丸み半径 / Corner radius (default: 10)
        /// @param bg_color 背景色 / Background color (default: white)
        /// @param layer レンダリングレイヤー / Rendering layer (default: UIBackground)
        static void draw(
            int x, int y, int width, int height,
            int corner_radius = 10,
            const paxg::Color& bg_color = paxg::Color{255, 255, 255},
            RenderLayer layer = RenderLayer::UIBackground
        ) {
#ifdef PAXS_USING_SIV3D
            // Siv3D: バッチ描画に登録（レイヤー別）
            shadow_shapes_map_[layer].push_back([x, y, width, height, corner_radius]() {
                paxg::RoundRect{ x, y, width, height, corner_radius }.draw();
            });
            panel_shapes_map_[layer].push_back([x, y, width, height, corner_radius, bg_color]() {
                paxg::RoundRect{ x, y, width, height, corner_radius }.draw(bg_color);
            });
#else
            // SFML/DxLib: 即座に描画
            (void)layer; // 未使用パラメータの警告を抑制
            paxg::RoundRect{ x, y, width, height, corner_radius }
                .drawShadow({1, 1}, 4, 1).draw(bg_color);
#endif
        }

        /// @brief 背景と影を描画（矩形）
        /// @brief Draw background and shadow (rectangle)
        /// @param x パネルのX座標 / Panel X coordinate
        /// @param y パネルのY座標 / Panel Y coordinate
        /// @param width パネルの幅 / Panel width
        /// @param height パネルの高さ / Panel height
        /// @param bg_color 背景色 / Background color (default: white)
        /// @param layer レンダリングレイヤー / Rendering layer (default: UIBackground)
        static void drawRect(
            int x, int y, int width, int height,
            const paxg::Color& bg_color = paxg::Color{255, 255, 255},
            RenderLayer layer = RenderLayer::UIBackground
        ) {
#ifdef PAXS_USING_SIV3D
            // Siv3D: バッチ描画に登録（レイヤー別）
            shadow_shapes_map_[layer].push_back([x, y, width, height]() {
                paxg::Rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }.draw();
            });
            panel_shapes_map_[layer].push_back([x, y, width, height, bg_color]() {
                paxg::Rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }.draw(bg_color);
            });
#else
            // SFML/DxLib: 即座に描画
            (void)layer; // 未使用パラメータの警告を抑制
            paxg::Rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }
                .drawShadow({1, 1}, 4, 1).draw(bg_color);
#endif
        }

    private:
#ifdef PAXS_USING_SIV3D
        // 影描画用テクスチャ（全インスタンスで共有）
        inline static paxg::RenderTexture shadow_texture_{};
        inline static paxg::RenderTexture internal_texture_{};

        // バッチ描画用の静的コンテナ（レイヤー別）
        inline static std::map<RenderLayer, std::vector<std::function<void()>>> shadow_shapes_map_;
        inline static std::map<RenderLayer, std::vector<std::function<void()>>> panel_shapes_map_;
#endif
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_PANEL_BACKGROUND_HPP
