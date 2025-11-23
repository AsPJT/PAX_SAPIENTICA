/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UPDATE_COORDINATOR_HPP
#define PAX_MAHOROBA_UPDATE_COORDINATOR_HPP

#include <functional>
#include <memory>
#include <vector>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/IUpdatable.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Map/Core/MapAssetRegistry.hpp>
#include <PAX_MAHOROBA/Map/Core/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/UI/Debug/PerformanceScope.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief 更新調整クラス
    /// @brief Update coordinator class
    ///
    /// 責務:
    /// - イベント購読の管理
    /// - UnifiedContextの更新
    /// - 全地物の更新処理（空間的・時間的・ローカライゼーション）
    /// - Settlement更新コールバックの管理
    class UpdateCoordinator {
    private:
        const AppStateManager& app_state_manager_;
        const MapViewport& map_viewport_;
        UnifiedContext unified_context_; ///< 統合コンテキスト / Unified context

        // コールバック関数（オプション）
        std::function<void()> settlement_update_callback_; ///< Settlement更新コールバック

        /// @brief UnifiedContextを更新
        /// @brief Update UnifiedContext
        void updateUnifiedContext(const MapAssetRegistry& asset_registry) {
            const auto& koyomi = app_state_manager_.getKoyomi();
            unified_context_.jdn = koyomi.jdn.getDay();
            unified_context_.map_view_size = map_viewport_.getSize();
            unified_context_.map_view_center = map_viewport_.getCenter();
            unified_context_.visibility_manager = &app_state_manager_.getVisibilityManager();
            unified_context_.texture_map = &asset_registry.getMergedMap();
            unified_context_.font = Fonts().getFont(FontProfiles::MAIN);
            unified_context_.language_key = Fonts().getSelectedLanguageKey();
        }

    public:
        UpdateCoordinator(const AppStateManager& app_state_manager)
            : app_state_manager_(app_state_manager)
            , map_viewport_(app_state_manager.getMapViewport()) {
        }

        ~UpdateCoordinator() = default;

        // コピー・ムーブ禁止（観察ポインタとイベント購読を持つため）
        UpdateCoordinator(const UpdateCoordinator&) = delete;
        auto operator=(const UpdateCoordinator&)->UpdateCoordinator & = delete;
        UpdateCoordinator(UpdateCoordinator&&) = delete;
        auto operator=(UpdateCoordinator&&)->UpdateCoordinator & = delete;

        /// @brief Settlement更新コールバックを設定
        void setSettlementUpdateCallback(std::function<void()> callback) {
            settlement_update_callback_ = std::move(callback);
        }

        /// @brief イベントを購読
        /// @brief Subscribe to events
        void subscribeToEvents(
            std::vector<std::unique_ptr<MapFeature>>& features,
            const MapAssetRegistry& asset_registry) {

            paxs::EventBus& event_bus = paxs::EventBus::getInstance();

            // ビューポート変更イベントの購読
            // 空間的な更新のみ実施（座標変換、範囲チェック、ズームフィルタ）
            event_bus.subscribe<ViewportChangedEvent>(
                [this, &features, &asset_registry](const ViewportChangedEvent& event) {
                    (void)event;
                    updateAllSpatial(features, asset_registry);
                    if (settlement_update_callback_) {
                        settlement_update_callback_();
                    }
                }
            );

            // 日付変更イベントの購読
            // 時間的な更新のみ実施（PersonFeatureの補間座標計算など）
            event_bus.subscribe<DateChangedEvent>(
                [this, &features, &asset_registry](const DateChangedEvent& event) {
                    (void)event;
                    updateAllTemporal(features, asset_registry);
                }
            );

            // 言語変更イベントの購読
            // ローカライゼーション更新のみ実施（フォント幅計算、名前の再取得）
            event_bus.subscribe<LanguageChangedEvent>(
                [this, &features, &asset_registry](const LanguageChangedEvent& event) {
                    (void)event;
                    updateAllLocalization(features, asset_registry);
                }
            );
        }

        /// @brief 全地物の空間的更新
        /// @brief Update all features spatially
        void updateAllSpatial(
            std::vector<std::unique_ptr<MapFeature>>& features,
            const MapAssetRegistry& asset_registry) {

            PERF_SCOPE("UpdateCoordinator::updateAllSpatial");
            updateUnifiedContext(asset_registry);
            const SpatialContext ctx = unified_context_.toSpatial();

            for (auto& feature : features) {
                if (!feature || !feature->isInTimeRange(unified_context_.jdn)) {
                    continue;
                }

                // ISpatiallyUpdatableを実装している地物のみ更新
                if (auto* spatial = dynamic_cast<ISpatiallyUpdatable*>(feature.get())) {
                    spatial->updateSpatial(ctx);
                }
            }
        }

        /// @brief 全地物の時間的更新
        /// @brief Update all features temporally
        void updateAllTemporal(
            std::vector<std::unique_ptr<MapFeature>>& features,
            const MapAssetRegistry& asset_registry) {

            PERF_SCOPE("UpdateCoordinator::updateAllTemporal");
            updateUnifiedContext(asset_registry);
            const TemporalContext ctx = unified_context_.toTemporal();

            for (auto& feature : features) {
                if (!feature) {
                    continue;
                }

                // ITemporallyUpdatableを実装している地物のみ更新
                if (auto* temporal = dynamic_cast<ITemporallyUpdatable*>(feature.get())) {
                    // 時間範囲チェックは更新前に行う
                    if (feature->isInTimeRange(unified_context_.jdn)) {
                        temporal->updateTemporal(ctx);
                    }
                }
            }
        }

        /// @brief 全地物のローカライゼーション更新
        /// @brief Update all features for localization
        void updateAllLocalization(
            std::vector<std::unique_ptr<MapFeature>>& features,
            const MapAssetRegistry& asset_registry) {

            PERF_SCOPE("UpdateCoordinator::updateAllLocalization");
            updateUnifiedContext(asset_registry);
            const LocalizationContext ctx = unified_context_.toLocalization();

            for (auto& feature : features) {
                if (!feature || !feature->isInTimeRange(unified_context_.jdn)) {
                    continue;
                }

                // ILocalizableを実装している地物のみ更新
                if (auto* localizable = dynamic_cast<ILocalizable*>(feature.get())) {
                    localizable->updateLocalization(ctx);
                }
            }
        }

        /// @brief UnifiedContextへの参照を取得（入力処理用）
        /// @brief Get UnifiedContext reference (for input handling)
        const UnifiedContext& getUnifiedContext() const {
            return unified_context_;
        }
    };
}

#endif // !PAX_MAHOROBA_UPDATE_COORDINATOR_HPP
