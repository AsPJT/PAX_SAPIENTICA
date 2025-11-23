/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_LOCATION_RENDERER_HELPER_HPP
#define PAX_MAHOROBA_LOCATION_RENDERER_HELPER_HPP

#include <cstdint>
#include <cstring>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 地名・人名・集落レンダリングの共通ヘルパー
    /// @brief Common helper utilities for location, person name, and settlement rendering
    struct LocationRendererHelper {
        /// @brief agent1/agent2アイコンを描画
        /// @brief Draw agent1/agent2 icons
        /// @param texture テクスチャマップ
        /// @param feature_type_hash 地物の種別ハッシュ値（agent1 or agent2のハッシュ値）
        /// @param draw_pos 描画位置
        /// @return true if agent icon was drawn, false otherwise
        static bool drawAgentIcon(
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture,
            std::uint_least32_t feature_type_hash,
            const paxg::Vec2<double>& draw_pos
        ) {
            // エージェント1を描画
            if (feature_type_hash == MurMur3::calcHash("agent1")) {
                const auto iterator = texture.find(MurMur3::calcHash("BlueCircle"));
                if (iterator != texture.end()) {
                    iterator->second.resizedDrawAt(15, draw_pos);
                }
                return true;
            }

            // エージェント2を描画
            if (feature_type_hash == MurMur3::calcHash("agent2")) {
                const auto iterator = texture.find(MurMur3::calcHash("RedCircle"));
                if (iterator != texture.end()) {
                    iterator->second.resizedDrawAt(15, draw_pos);
                }
                return true;
            }
            return false;
        }
    };

}

#endif // !PAX_MAHOROBA_LOCATION_RENDERER_HELPER_HPP
