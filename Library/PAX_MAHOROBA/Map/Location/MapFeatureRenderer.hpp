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
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

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
                drawGeographic(static_cast<const GeographicFeature&>(*feature), texture_map);
                break;
            case FeatureType::PlaceName:
                drawPlaceName(static_cast<const PlaceNameFeature&>(*feature));
                break;
            case FeatureType::Genome:
                drawGenome(static_cast<const GenomeFeature&>(*feature), texture_map);
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
        const auto& group_data = feature.getGroupData();
        const auto& screen_positions = feature.getScreenPositions();
        const int display_size = feature.getDisplaySize();

        const bool is_small_size = data.zoom_range.excludes(context.map_view_size.x);

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture_map, data.feature_type_hash, draw_pos)) {
                continue;
            }

            // テクスチャを取得
            const std::uint_least32_t place_tex = (data.texture_key == 0) ? group_data.texture_key : data.texture_key;
            if (!texture_map.contains(place_tex)) continue;

            if (is_small_size) {
                // 肖像画のみ描画
                texture_map.at(place_tex).resizedDrawAt(display_size, draw_pos);
            }
            else {
                // 肖像画とテキストを描画
                texture_map.at(place_tex).resizedDrawAt(120, draw_pos);

                // テキスト位置
                const paxg::Vec2<double> draw_font_pos = paxg::Vec2<double>{ draw_pos.x(), draw_pos.y() - 60 };

                const std::string name = feature.getName();
                if (!name.empty()) {
                    paxg::Font* font = Fonts().getFont(FontProfiles::MAIN);
                    font->setOutline(0, 0.6, paxg::Color(243, 243, 243));
                    font->drawTopCenter(name, draw_font_pos, paxg::Color(0, 0, 0));
                }
            }
        }
    }

    /// @brief 地理的地物を描画
    /// @brief Draw geographic feature
    /// @param feature 地理的地物 / Geographic feature
    /// @param texture_map テクスチャマップ / Texture map
    static void drawGeographic(
        const GeographicFeature& feature,
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        const auto& data = feature.getData();
        const auto& screen_positions = feature.getScreenPositions();
        const int display_size = feature.getDisplaySize();
        const int draw_count = feature.getDrawCount();

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture_map, data.feature_type_hash, draw_pos)) {
                continue;
            }

            // テクスチャを取得（親のテクスチャをフォールバック）
            // Note: GeographicFeature は LocationPoint を使用しているため、親のテクスチャ情報は含まれていない
            // 将来的には LocationPointGroup への参照も保持する必要があるかもしれない
            const std::uint_least32_t place_tex = data.texture_key;
            if (!texture_map.contains(place_tex)) continue;

            const auto& texture = texture_map.at(place_tex);

            // draw_countが1の場合は通常描画
            if (draw_count == 1) {
                texture.resizedDrawAt(display_size, draw_pos);
            } else {
                // draw_countが2以上の場合は横に複数並べて描画（中央揃え）
                constexpr int spacing = 4;  // テクスチャ間の間隔
                const int total_width = (draw_count - 1) * spacing;
                const int start_x = static_cast<int>(draw_pos.x()) - total_width / 2;

                for (int i = 0; i < draw_count; ++i) {
                    const paxg::Vec2<double> draw_item_pos{
                        static_cast<double>(start_x + i * spacing),
                        draw_pos.y()
                    };
                    texture.resizedDrawAt(display_size, draw_item_pos);
                }
            }
        }
    }

    /// @brief ゲノム地物を描画
    /// @brief Draw genome feature
    /// @param feature ゲノム地物 / Genome feature
    /// @param context 描画コンテキスト / Rendering context
    /// @param texture_map テクスチャマップ / Texture map
    /// @brief 警告用のテクスチャを描画（テクスチャが見つからない場合）
    /// @brief Draw warning texture when texture is not found
    static void drawWarningTexture(const paxg::Vec2<double>& pos, int size) {
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
        const UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_map
    ) {
        const auto& data = feature.getData();
        const auto& screen_positions = feature.getScreenPositions();
        const int display_size = feature.getDisplaySize();

        // 名前表示の閾値（表示サイズが小さい場合は名前を表示しない）
        bool should_show_name = (display_size >= 15);

        // フォント設定
        paxg::Font* font = nullptr;
        if (should_show_name) {
            font = Fonts().getFont(FontProfiles::MAIN);
            if (font == nullptr) {
                // フォントが取得できない場合は名前表示をスキップ
                PAXS_WARNING("Font not found for genome feature name rendering");
                should_show_name = false;
            } else {
                font->setOutline(0, 0.6, paxg::Color(243, 243, 243));
            }
        }

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            // エージェントアイコン描画
            if (LocationRendererHelper::drawAgentIcon(texture_map, data.feature_type_hash, draw_pos)) {
                continue;
            }

            // テクスチャを取得
            const std::uint_least32_t place_tex = data.texture_key;

            // テクスチャが見つからない場合は警告表示
            if (place_tex == 0 || !texture_map.contains(place_tex)) {
                drawWarningTexture(draw_pos, display_size > 0 ? display_size : 20);
                continue;
            }

            // テクスチャを描画
            texture_map.at(place_tex).resizedDrawAt(display_size, draw_pos);

            // 名前を描画（表示サイズが十分な場合のみ）
            if (should_show_name) {
                const std::string name = feature.getName();
                if (!name.empty()) {
                    // テクスチャの上部に名前を描画
                    const paxg::Vec2<double> text_pos = paxg::Vec2<double>{
                        draw_pos.x(),
                        draw_pos.y() - (display_size / 2) - 5  // アイコンの上部から少し離す
                    };
                    font->drawBottomCenter(name, text_pos, paxg::Color(0, 0, 0));
                }
            }
        }
    }

    /// @brief 地名地物を描画
    /// @brief Draw place name feature
    /// @param feature 地名地物 / Place name feature
    static void drawPlaceName(
        const PlaceNameFeature& feature
    ) {
        const auto& screen_positions = feature.getScreenPositions();
        const std::string name = feature.getName();

        // 名前が空の場合は早期リターン
        if (name.empty()) {
            return;
        }

        // フォント設定（ループ外で1回のみ）
        paxg::Font* font = Fonts().getFont(FontProfiles::MAIN);
        if (font == nullptr) {
            PAXS_WARNING("Font not found for place name feature rendering");
            return;
        }

        font->setOutline(0, 0.6, paxg::Color(243, 243, 243));

        // 各スクリーン座標で描画（経度ラップ対応）
        for (const auto& draw_pos : screen_positions) {
            font->drawAt(name, draw_pos, paxg::Color(0, 0, 0));
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

};

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_FEATURE_RENDERER_HPP
