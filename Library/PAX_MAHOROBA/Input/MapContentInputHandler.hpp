/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP

#include <string>

#include <PAX_MAHOROBA/Input/Events.hpp>
#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Map/Location/GeographicFeatureManager.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonPortraitManager.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    /// @brief 地図コンテンツ入力処理ハンドラー
    /// @brief Map content input handler
    class MapContentInputHandler : public IInputHandler {
    public:
        MapContentInputHandler(
            const PersonPortraitManager* person_portrait_manager,
            const GeographicFeatureManager* geographic_feature_manager,
            const FeatureVisibilityManager* visibility_manager
        )
            : person_portrait_manager_(person_portrait_manager)
            , geographic_feature_manager_(geographic_feature_manager)
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

            // キャッシュされた結果に基づいて処理
            if (hit_cache_.type == HitCache::HitType::PersonPortrait) {
                return EventHandlingResult::Handled();
            } else if (hit_cache_.type == HitCache::HitType::GeographicFeature) {
                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::NotHandled();
        }

        bool isHit(int x, int y) const override {
            if (visibility_manager_ != nullptr &&
                !visibility_manager_->isVisible(ViewMenu::map)) {
                    return false;
                }

            // 座標が変わったらキャッシュ無効化
            if (hit_cache_.cached_x != x || hit_cache_.cached_y != y) {
                hit_cache_.valid = false;
            }

            if (!hit_cache_.valid) {
                hit_cache_.cached_x = x;
                hit_cache_.cached_y = y;

                // 1. 人物肖像画をチェック
                if (person_portrait_manager_ != nullptr) {
                    std::string person_name;
                    if (person_portrait_manager_->isHit(x, y, person_name)) {
                        hit_cache_.type = HitCache::HitType::PersonPortrait;
                        hit_cache_.name = person_name;
                        hit_cache_.valid = true;
                        return true;
                    }
                }

                // 2. 地理的特徴をチェック
                if (geographic_feature_manager_ != nullptr) {
                    std::string place_name;
                    if (geographic_feature_manager_->isHit(x, y, place_name)) {
                        hit_cache_.type = HitCache::HitType::GeographicFeature;
                        hit_cache_.name = place_name;
                        hit_cache_.valid = true;
                        return true;
                    }
                }

                // ヒットなし
                hit_cache_.type = HitCache::HitType::None;
                hit_cache_.valid = true;
                return false;
            }

            return hit_cache_.type != HitCache::HitType::None;
        }

        RenderLayer getLayer() const override { return RenderLayer::MapContent; }
        bool isVisible() const { return true; }
        void setVisible(bool /*visible*/) {}
        bool isEnabled() const override { return true; }
        void setEnabled(bool /*enabled*/) {}

    private:
        const PersonPortraitManager* person_portrait_manager_ = nullptr;
        const GeographicFeatureManager* geographic_feature_manager_ = nullptr;
        const FeatureVisibilityManager* visibility_manager_ = nullptr;

        struct HitCache {
            bool valid = false;
            enum class HitType { None, PersonPortrait, GeographicFeature };
            HitType type = HitType::None;
            std::string name;
            int cached_x = -1;
            int cached_y = -1;
        };
        mutable HitCache hit_cache_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_CONTENT_INPUT_HANDLER_HPP
