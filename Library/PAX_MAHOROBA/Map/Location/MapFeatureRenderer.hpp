/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_FEATURE_RENDERER_HPP
#define PAX_MAHOROBA_MAP_FEATURE_RENDERER_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Map/Location/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Location/GenomeFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/GeographicFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/LocationRendererHelper.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/Model3DFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/PlaceNameFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/RenderContext.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>

namespace paxs {

/// @brief 地物の描画を統括するレンダラークラス
/// @brief Renderer class that manages drawing of all map features
class MapFeatureRenderer {
public:
    /// @brief 地物のリストを描画
    /// @brief Draw list of features
    /// @param features 描画する地物のリスト / List of features to draw
    /// @param context 描画コンテキスト / Rendering context
    /// @param texture_map テクスチャマップ / Texture map
    static void drawFeatures(
        const std::vector<std::unique_ptr<MapFeature>>& features,
        const RenderContext& context,
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        for (const auto& feature : features) {
            if (!feature || !feature->isVisible()) continue;
            if (!feature->isInTimeRange(context.jdn)) continue;
            // 空間フィルタリング済み（update()でスクリーン座標が空の場合はスキップ）
            if (feature->getScreenPositions().empty()) continue;

            switch (feature->getType()) {
            case FeatureType::Person:
                drawPerson(static_cast<const PersonFeature&>(*feature), context, texture_map);
                break;
            case FeatureType::Geographic:
                drawGeographic(static_cast<const GeographicFeature&>(*feature), context, texture_map);
                break;
            case FeatureType::PlaceName:
                drawPlaceName(static_cast<const PlaceNameFeature&>(*feature), context, texture_map);
                break;
            case FeatureType::Genome:
                drawGenome(static_cast<const GenomeFeature&>(*feature), context, texture_map);
                break;
            case FeatureType::Model3D:
                drawModel3D(static_cast<const Model3DFeature&>(*feature), context, texture_map);
                break;
            default:
                break;
            }
        }
    }

private:
    MapFeatureRenderer() = default;

    // 描画定数
    static constexpr int TEXTURE_SPACING_HORIZONTAL = 4;
    static constexpr int TEXTURE_SPACING_HORIZONTAL_ZOOMED = 6;
    static constexpr int TEXTURE_SPACING_VERTICAL = 4;
    static constexpr std::uint_least16_t ZOOM_SPLIT_COUNT = 10;

    /// @brief 人物地物を描画
    /// @brief Draw person feature
    /// @param feature 人物地物 / Person feature
    /// @param context 描画コンテキスト / Rendering context
    /// @param texture_map テクスチャマップ / Texture map
    static void drawPerson(
        const PersonFeature& feature,
        const RenderContext& context,
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        const auto& data = feature.getData();
        const auto& list_data = feature.getListData();
        const auto& screen_positions = feature.getScreenPositions();
        const int display_size = feature.getDisplaySize();

        const bool is_small_size = (data.min_zoom_level > context.map_view_size.x || data.max_zoom_level < context.map_view_size.x);

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture_map, data.feature_type_hash, draw_pos)) {
                continue;
            }

            // テクスチャを取得
            const std::uint_least32_t place_tex = (data.texture_key == 0) ? list_data.texture_key : data.texture_key;
            if (texture_map.find(place_tex) == texture_map.end()) continue;

            if (is_small_size) {
                // 肖像画のみ描画
                texture_map.at(place_tex).resizedDrawAt(display_size, draw_pos);
            }
            else {
                // 肖像画とテキストを描画
                texture_map.at(place_tex).resizedDrawAt(120, draw_pos);

                // テキスト位置
                const paxg::Vec2i draw_font_pos = paxg::Vec2i{ draw_pos.x(), draw_pos.y() - 60 };
                LocationRendererHelper::drawBilingualText(data.person_name, draw_font_pos, "topCenter");
            }
        }
    }

    /// @brief 地理的地物を描画
    /// @brief Draw geographic feature
    /// @param feature 地理的地物 / Geographic feature
    /// @param context 描画コンテキスト / Rendering context
    /// @param texture_map テクスチャマップ / Texture map
    static void drawGeographic(
        const GeographicFeature& feature,
        const RenderContext& context,
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        const auto& data = feature.getData();
        const auto& screen_positions = feature.getScreenPositions();
        const int display_size = feature.getDisplaySize();

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture_map, data.feature_type_hash, draw_pos)) {
                continue;
            }

            // テクスチャを取得（親のテクスチャをフォールバック）
            // Note: GeographicFeature は LocationPoint を使用しているため、親のテクスチャ情報は含まれていない
            // 将来的には LocationPointList への参照も保持する必要があるかもしれない
            const std::uint_least32_t place_tex = data.texture_key;
            if (texture_map.find(place_tex) == texture_map.end()) continue;

            const bool is_zoomed = (data.zoom > 1.0);

            // 複数タイルの描画
            drawTextureMultiple(
                texture_map.at(place_tex),
                display_size,
                draw_pos,
                data.x_size,
                data.y_size,
                is_zoomed
            );
        }
    }

    /// @brief ゲノム地物を描画
    /// @brief Draw genome feature
    /// @param feature ゲノム地物 / Genome feature
    /// @param context 描画コンテキスト / Rendering context
    /// @param texture_map テクスチャマップ / Texture map
    /// @brief 警告用のテクスチャを描画（テクスチャが見つからない場合）
    /// @brief Draw warning texture when texture is not found
    static void drawWarningTexture(const paxg::Vec2i& pos, int size) {
        // 赤い四角形で警告表示
        paxg::Rect(
            static_cast<float>(pos.x() - size / 2),
            static_cast<float>(pos.y() - size / 2),
            static_cast<float>(size),
            static_cast<float>(size)
        ).draw(paxg::Color(255, 0, 0, 180)); // 半透明の赤

        // 中央に白い×印を描画
        const float center_x = static_cast<float>(pos.x());
        const float center_y = static_cast<float>(pos.y());
        const float half_size = static_cast<float>(size) * 0.3f;

        // ×印の線（太さ3ピクセル）
        paxg::Line(
            center_x - half_size, center_y - half_size,
            center_x + half_size, center_y + half_size
        ).draw(3.0f, paxg::Color(255, 255, 255));

        paxg::Line(
            center_x - half_size, center_y + half_size,
            center_x + half_size, center_y - half_size
        ).draw(3.0f, paxg::Color(255, 255, 255));
    }

    static void drawGenome(
        const GenomeFeature& feature,
        const RenderContext& context,
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        const auto& data = feature.getData();
        const auto& screen_positions = feature.getScreenPositions();
        const int display_size = feature.getDisplaySize();

        // 名前表示の閾値（表示サイズが小さい場合は名前を表示しない）
        const bool should_show_name = (display_size >= 15);

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture_map, data.feature_type_hash, draw_pos)) {
                continue;
            }

            // テクスチャを取得
            const std::uint_least32_t place_tex = data.texture_key;

            // テクスチャが見つからない場合は警告表示
            if (place_tex == 0 || texture_map.find(place_tex) == texture_map.end()) {
                drawWarningTexture(draw_pos, display_size > 0 ? display_size : 20);
                continue;
            }

            const bool is_zoomed = (data.zoom > 1.0);

            // 複数タイルの描画
            drawTextureMultiple(
                texture_map.at(place_tex),
                display_size,
                draw_pos,
                data.x_size,
                data.y_size,
                is_zoomed
            );

            // 名前を描画（表示サイズが十分な場合のみ）
            if (should_show_name && !data.place_name.empty()) {
                // テクスチャの上部に名前を描画
                const paxg::Vec2i text_pos = paxg::Vec2i{
                    draw_pos.x(),
                    draw_pos.y() - display_size / 2 - 5  // アイコンの上部から少し離す
                };
                LocationRendererHelper::drawBilingualText(data.place_name, text_pos, "bottomCenter");
            }
        }
    }

    /// @brief 地名地物を描画
    /// @brief Draw place name feature
    /// @param feature 地名地物 / Place name feature
    /// @param context 描画コンテキスト / Rendering context
    /// @param texture_map テクスチャマップ / Texture map
    static void drawPlaceName(
        const PlaceNameFeature& feature,
        const RenderContext& context,
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        const auto& data = feature.getData();
        const auto& screen_positions = feature.getScreenPositions();

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            LocationRendererHelper::drawBilingualText(data.place_name, draw_pos, "at");
        }
    }

    /// @brief 3Dモデル地物を描画
    /// @brief Draw 3D model feature
    /// @param feature 3Dモデル地物 / 3D model feature
    /// @param context 描画コンテキスト / Rendering context
    /// @param texture_map テクスチャマップ / Texture map
    /// @note 将来の実装: 3Dモデルの描画処理を実装
    /// @note Future implementation: Implement 3D model rendering
    static void drawModel3D(
        const Model3DFeature& feature,
        const RenderContext& context,
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        // 現在は未実装（将来的にModel3DRendererを使用して描画）
        // Currently unimplemented (will use Model3DRenderer for rendering in the future)
        (void)feature;
        (void)context;
        (void)texture_map;

        // TODO: 将来的な実装例
        // const auto& screen_positions = feature.getScreenPositions();
        // for (const auto& draw_pos : screen_positions) {
        //     // 3Dモデルを描画位置に配置
        //     feature.getRenderer().render();
        // }
    }

    /// @brief テクスチャを複数描画（GeographicFeature用）
    /// @brief Draw texture multiple times (for GeographicFeature)
    static void drawTextureMultiple(
        const paxg::Texture& texture,
        const int display_size,
        const paxg::Vec2i& draw_pos,
        const std::uint_least16_t x_size,
        const std::uint_least16_t y_size,
        const bool is_zoomed
    ) {
        if (x_size <= 1) {
            if (y_size <= 1) {
                texture.resizedDrawAt(display_size, draw_pos);
            }
            else {
                for (std::uint_least16_t iy = 0; iy < y_size; ++iy) {
                    texture.resizedDrawAt(display_size, paxg::Vec2i{ draw_pos.x(), draw_pos.y() + static_cast<int>(iy) * TEXTURE_SPACING_VERTICAL });
                }
            }
        }
        else {
            if (is_zoomed) {
                // ズーム時は分割表示
                constexpr std::uint_least16_t split_count = ZOOM_SPLIT_COUNT;
                if (y_size <= 1) {
                    for (std::uint_least16_t ix = 0, ixx = 0, iyy = 0; ix < x_size; ++ix, ++ixx) {
                        if (ix != 0 && ix % split_count == 0) {
                            ixx = 0;
                            ++iyy;
                        }
                        texture.resizedDrawAt(display_size, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ixx) * TEXTURE_SPACING_HORIZONTAL_ZOOMED, draw_pos.y() + static_cast<int>(iyy) * TEXTURE_SPACING_VERTICAL });
                    }
                }
                else {
                    for (std::uint_least16_t iy = 0; iy < y_size; ++iy) {
                        for (std::uint_least16_t ix = 0; ix < x_size; ++ix) {
                            texture.resizedDrawAt(display_size, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ix) * TEXTURE_SPACING_HORIZONTAL, draw_pos.y() + static_cast<int>(iy) * TEXTURE_SPACING_VERTICAL });
                        }
                    }
                }
            }
            else {
                // 通常時
                if (y_size <= 1) {
                    for (std::uint_least16_t ix = 0; ix < x_size; ++ix) {
                        texture.resizedDrawAt(display_size, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ix) * TEXTURE_SPACING_HORIZONTAL, draw_pos.y() });
                    }
                }
                else {
                    for (std::uint_least16_t iy = 0; iy < y_size; ++iy) {
                        for (std::uint_least16_t ix = 0; ix < x_size; ++ix) {
                            texture.resizedDrawAt(display_size, paxg::Vec2i{ draw_pos.x() + static_cast<int>(ix) * TEXTURE_SPACING_HORIZONTAL, draw_pos.y() + static_cast<int>(iy) * TEXTURE_SPACING_VERTICAL });
                        }
                    }
                }
            }
        }
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_FEATURE_RENDERER_HPP
