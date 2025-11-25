/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP
#define PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Manager/FeatureCollectionManager.hpp>
#include <PAX_MAHOROBA/Map/Content/Manager/UpdateCoordinator.hpp>
#include <PAX_MAHOROBA/Map/Content/Renderer/MapFeatureRenderer.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Map/Simulation/ISimulationProvider.hpp>
#include <PAX_MAHOROBA/Map/Simulation/NullSimulationProvider.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Map/Simulation/SettlementSimulationProvider.hpp>
#endif

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief 地図コンテンツレイヤークラス（Facade）
    /// @brief Map Content Layer (Facade)
    ///
    /// - FeatureCollectionManager、UpdateCoordinator、ISimulationProviderを統合
    class MapContentLayer : public IRenderable {
    private:
        const AppStateManager& app_state_manager_;

        // 3つのマネージャー
        FeatureCollectionManager feature_collection_manager_; ///< 地物コレクション管理
        UpdateCoordinator update_coordinator_; ///< 更新調整
        std::unique_ptr<ISimulationProvider> simulation_provider_; ///< シミュレーション機能（Strategy パターン）

    public:
        MapContentLayer(const AppStateManager& app_state_manager)
            : app_state_manager_(app_state_manager)
            , update_coordinator_(app_state_manager)
#ifdef PAXS_USING_SIMULATOR
            , simulation_provider_(std::make_unique<SettlementSimulationProvider>(app_state_manager))
#else
            , simulation_provider_(std::make_unique<NullSimulationProvider>())
#endif
        {
            // データロード（初回のみ）
            feature_collection_manager_.loadAllFeatures();

            // Settlement更新コールバックを設定
            update_coordinator_.setSettlementUpdateCallback(
                [this]() {
                    simulation_provider_->updateSettlementData();
                }
            );

            // イベント購読
            update_coordinator_.subscribeToEvents(
                feature_collection_manager_.getFeatures(),
                feature_collection_manager_.getAssetRegistry()
            );

            // シミュレーション関連イベント購読（Strategy パターンで常に呼び出し可能）
            simulation_provider_->subscribeToSimulationEvents();

            // 初回更新を即座に実行（全ての更新が必要）
            // 注意: PersonFeatureはupdateTemporal()で補間座標を計算するため、
            //       updateSpatial()の前にupdateTemporal()を呼ぶ必要がある
            update_coordinator_.updateAllTemporal(
                feature_collection_manager_.getFeatures(),
                feature_collection_manager_.getAssetRegistry()
            );
            update_coordinator_.updateAllSpatial(
                feature_collection_manager_.getFeatures(),
                feature_collection_manager_.getAssetRegistry()
            );
            update_coordinator_.updateAllLocalization(
                feature_collection_manager_.getFeatures(),
                feature_collection_manager_.getAssetRegistry()
            );

            // Settlementデータ更新（Strategy パターンで常に呼び出し可能）
            simulation_provider_->updateSettlementData();
        }

        // コピー・ムーブ禁止（観察ポインタとイベント購読を持つため）
        ~MapContentLayer() override = default;
        MapContentLayer(const MapContentLayer&) = delete;
        auto operator=(const MapContentLayer&)->MapContentLayer & = delete;
        MapContentLayer(MapContentLayer&&) = delete;
        auto operator=(MapContentLayer&&)->MapContentLayer & = delete;

        void render() const override {
            if (app_state_manager_.getVisibilityManager().isVisible(ViewMenu::simulation)) {
                simulation_provider_->render();
            }

            if (app_state_manager_.getVisibilityManager().isVisible(ViewMenu::map)) {
                MapFeatureRenderer::drawFeatures(
                    feature_collection_manager_.getFeatures(),
                    update_coordinator_.getUnifiedContext(),
                    feature_collection_manager_.getAssetRegistry().getMergedMap()
                );
            }
        }

        /// @brief SettlementInputHandler への参照を取得（AppComponentManager での登録用）
        /// @brief Get SettlementInputHandler reference (for AppComponentManager registration)
        /// @return SettlementInputHandler* (nullptrの場合あり)
        SettlementInputHandler* getSettlementInputHandler() {
            return simulation_provider_->getSettlementInputHandler();
        }

        const std::vector<std::unique_ptr<MapFeature>>& getFeatures() const {
            return feature_collection_manager_.getFeatures();
        }

        /// @brief UnifiedContextへの参照を取得（入力処理用）
        /// @brief Get UnifiedContext reference (for input handling)
        const UnifiedContext& getUnifiedContext() const {
            return update_coordinator_.getUnifiedContext();
        }

        /// @brief IDでFeatureを検索
        MapFeature* findFeatureById(std::uint_least32_t id) {
            return feature_collection_manager_.findFeatureById(id);
        }

        /// @brief マウス座標でFeatureを検索（入力処理用）
        MapFeature* findFeatureAt(const paxs::Vector2<int>& mouse_pos) {
            return feature_collection_manager_.findFeatureAt(mouse_pos);
        }

        bool isVisible() const override { return true; }
        RenderLayer getLayer() const override { return RenderLayer::MapContent; }
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP
