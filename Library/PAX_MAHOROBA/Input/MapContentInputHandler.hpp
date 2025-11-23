/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP

#include <memory>
#include <vector>

#include <PAX_MAHOROBA/Events/FeatureEvents.hpp>
#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Input/InputEvents.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief 地図コンテンツ入力処理ハンドラー（新Featureシステム対応）
    /// @brief Map content input handler (new Feature system)
    class MapContentInputHandler : public IInputHandler {
    public:
        /// @param features 地物のリスト / List of features
        /// @param unified_context 統合コンテキスト / Unified context
        /// @param visibility_manager 可視性マネージャー / Visibility manager
        MapContentInputHandler(
            const std::vector<std::unique_ptr<MapFeature>>& features,
            const UnifiedContext& unified_context,
            const FeatureVisibilityManager& visibility_manager
        )
            : features_(features)
            , unified_context_(unified_context)
            , visibility_manager_(visibility_manager)
        {
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // 地図が表示されていない場合は処理しない
            if (!visibility_manager_.isVisible(ViewMenu::map)) {
                return EventHandlingResult::NotHandled();
            }

            // マウスクリック時のみ処理（左ボタンが押された瞬間）
            if (event.left_button_state != MouseButtonState::Pressed) {
                return EventHandlingResult::NotHandled();
            }

            // キャッシュされたFeatureに対してonClickを呼び出し
            if (hit_cache_.hit_feature != nullptr) {
                ClickContext context;
                context.mouse_pos = event.pos;

                hit_cache_.hit_feature->onClick(context);

                // EventBus経由でFeatureSelectedEventを発行
                EventBus::getInstance().publish(FeatureSelectedEvent(hit_cache_.hit_feature));
            }

            // 地図のドラッグ処理を妨げないようにNotHandledを返す
            return EventHandlingResult::NotHandled();
        }

        bool isHit(const paxs::Vector2<int>& pos) const override {
            if (!visibility_manager_.isVisible(ViewMenu::map)) {
                    return false;
            }

            // 座標が変わったらキャッシュ無効化
            if (hit_cache_.cached_pos != pos) {
                hit_cache_.valid = false;
            }

            if (!hit_cache_.valid) {
                hit_cache_.cached_pos = pos;

                // 逆順で検索（後に描画されたものが優先）
                for (auto it = features_.rbegin(); it != features_.rend(); ++it) {
                    const auto& feature = *it;
                    if (!feature) continue;

                    if (!feature->isVisible()) {
                        continue;
                    }

                    // 時間フィルタリング：現在の時刻に表示されないFeatureはクリック不可
                    if (!feature->isInTimeRange(unified_context_.jdn)) {
                        continue;
                    }

                    // 描画されていないFeatureはクリック不可（空間・ズームレベルフィルタリング済み）
                    if (feature->getScreenPositions().empty()) {
                        continue;
                    }

                    // Featureのヒット判定メソッドを呼び出し
                    if (feature->isHit(pos)) {
                        hit_cache_.hit_feature = feature.get();
                        hit_cache_.valid = true;
                        return true;
                    }
                }

                // ヒットなし
                hit_cache_.hit_feature = nullptr;
                hit_cache_.valid = true;
                return false;
            }

            return hit_cache_.hit_feature != nullptr;
        }

        bool isVisible() const { return true; }
        RenderLayer getLayer() const override { return RenderLayer::MapContent; }

    private:
        const std::vector<std::unique_ptr<MapFeature>>& features_;
        const UnifiedContext& unified_context_;
        const FeatureVisibilityManager& visibility_manager_;

        /// @brief ヒットテスト結果のキャッシュ
        /// @brief Cache for hit test results
        struct HitCache {
            bool valid = false;
            MapFeature* hit_feature = nullptr;  ///< ヒットしたFeature（所有権なし）
            paxs::Vector2<int> cached_pos{-1, -1};
        };
        mutable HitCache hit_cache_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP
