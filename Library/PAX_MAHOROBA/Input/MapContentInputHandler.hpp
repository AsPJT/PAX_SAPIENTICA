/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <PAX_MAHOROBA/Input/InputEvents.hpp>
#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Map/Location/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Location/MapContentHitTester.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    /// @brief 地図コンテンツ入力処理ハンドラー（新Featureシステム対応）
    /// @brief Map content input handler (new Feature system)
    class MapContentInputHandler : public IInputHandler {
    public:
        MapContentInputHandler() = default;

        /// @param features 地物のリスト / List of features
        /// @param render_context 描画コンテキスト / Render context
        /// @param visibility_manager 可視性マネージャー / Visibility manager
        MapContentInputHandler(
            const std::vector<std::unique_ptr<MapFeature>>* features,
            const RenderContext* render_context,
            const FeatureVisibilityManager* visibility_manager
        )
            : features_(features)
            , render_context_(render_context)
            , visibility_manager_(visibility_manager)
        {
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // 地図が表示されていない場合は処理しない
            if (visibility_manager_ != nullptr &&
                !visibility_manager_->isVisible(ViewMenu::map)) {
                return EventHandlingResult::NotHandled();
            }

            // マウスクリック時のみ処理（左ボタンが押された瞬間）
            if (event.left_button_state != MouseButtonState::Pressed) {
                return EventHandlingResult::NotHandled();
            }

            // キャッシュが有効かチェック
            if (!hit_cache_.valid ||
                hit_cache_.cached_x != event.x ||
                hit_cache_.cached_y != event.y) {
                // isHit() が先に呼ばれているはずだが、念のため再チェック
                if (!isHit(event.x, event.y)) {
                    return EventHandlingResult::NotHandled();
                }
            }

            // キャッシュされたFeatureに対してonClickを呼び出し
            if (hit_cache_.hit_feature != nullptr) {
                ClickContext context;
                context.mouse_pos = paxg::Vec2i(event.x, event.y);

                hit_cache_.hit_feature->onClick(context);

                std::cout << "Clicked on feature: " << hit_cache_.hit_feature->getName() << std::endl;
                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::NotHandled();
        }

        bool isHit(int x, int y) const override {
            if (visibility_manager_ != nullptr && !visibility_manager_->isVisible(ViewMenu::map)) {
                    return false;
            }

            // 座標が変わったらキャッシュ無効化
            if (hit_cache_.cached_x != x || hit_cache_.cached_y != y) {
                hit_cache_.valid = false;
            }

            if (!hit_cache_.valid) {
                hit_cache_.cached_x = x;
                hit_cache_.cached_y = y;

                // 新システム: MapContentHitTesterを使用してヒットテスト
                if (features_ != nullptr && render_context_ != nullptr) {
                    MapFeature* hit_feature = MapContentHitTester::findFeatureAt(*features_, *render_context_, x, y);

                    if (hit_feature != nullptr) {
                        hit_cache_.hit_feature = hit_feature;
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
        const std::vector<std::unique_ptr<MapFeature>>* features_ = nullptr;
        const RenderContext* render_context_ = nullptr;
        const FeatureVisibilityManager* visibility_manager_ = nullptr;

        /// @brief ヒットテスト結果のキャッシュ
        /// @brief Cache for hit test results
        struct HitCache {
            bool valid = false;
            MapFeature* hit_feature = nullptr;  ///< ヒットしたFeature（所有権なし）
            int cached_x = -1;
            int cached_y = -1;
        };
        mutable HitCache hit_cache_;
    };

    // ========================================
    // MapContentHitTester::findFeatureAt の実装
    // ========================================

    /// @brief Featureリストからマウス座標でヒットしたFeatureを検索
    inline MapFeature* MapContentHitTester::findFeatureAt(
        const std::vector<std::unique_ptr<MapFeature>>& features,
        const RenderContext& context,
        int mouse_x,
        int mouse_y
    ) {
        // 逆順で検索（後に描画されたものが優先）
        for (auto it = features.rbegin(); it != features.rend(); ++it) {
            const auto& feature = *it;
            if (!feature || !feature->isVisible()) continue;

            // 時間フィルタリング：現在の時刻に表示されないFeatureはクリック不可
            if (!feature->isInTimeRange(context.jdn)) continue;

            // 描画されていないFeatureはクリック不可（空間・ズームレベルフィルタリング済み）
            if (feature->getScreenPositions().empty()) continue;

            // Featureのヒット判定メソッドを呼び出し
            if (feature->isHit(paxg::Vec2i(mouse_x, mouse_y))) {
                return feature.get();
            }
        }
        return nullptr;
    }

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP
