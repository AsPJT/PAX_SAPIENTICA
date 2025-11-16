/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP
#define PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP

#include <memory>
#include <vector>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Input/SettlementInputHandler.hpp>
#include <PAX_MAHOROBA/Map/Location/SettlementManager.hpp>
#include <PAX_SAPIENTICA/Simulation/Manager/SimulationManager.hpp>
#endif

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Map/Location/GenomeFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/GeographicFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeatureRenderer.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/PlaceNameFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/RenderContext.hpp>
#include <PAX_MAHOROBA/Map/MapAssetRegistry.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Map/Repository/GenomeRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/GeographicFeatureRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/PersonNameRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/PlaceNameRepository.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief 地図コンテンツレイヤークラス
    /// @brief Map Content Layer
    class MapContentLayer : public IRenderable {
    private:
        // 新しいFeatureベース構造
        std::vector<std::unique_ptr<MapFeature>> features_; ///< 地物のコレクション / Collection of features
        RenderContext render_context_; ///< 描画コンテキスト / Render context

        // アセット管理
        MapAssetRegistry asset_registry_; ///< 地図アイコンアセットレジストリ / Map icon asset registry
#ifdef PAXS_USING_SIMULATOR
        SettlementManager settlement_manager_{}; // 集落管理
        SettlementInputHandler settlement_input_handler_; // 集落入力処理
#endif

        const MapViewport& map_viewport_;
        const AppStateManager& app_state_manager_;

#ifdef PAXS_USING_SIMULATOR
        /// @brief Settlementデータのみ更新
        /// @brief Update settlement data only
        void updateSettlementData() {
            const auto& koyomi = app_state_manager_.getKoyomi();
            const auto& simulation_manager = app_state_manager_.getSimulationManager();

            // SettlementManager に描画パラメータを設定
            if (simulation_manager.isActive()) {
                settlement_manager_.setDrawParams(
                    koyomi.jdn.getDay(),
                    simulation_manager.getSettlementGrids(),
                    simulation_manager.getMarriagePositions(),
                    map_viewport_.getWidth(),
                    map_viewport_.getHeight(),
                    map_viewport_.getCenterX(),
                    map_viewport_.getCenterY(),
                    settlement_input_handler_.getSelectDraw(),
                    settlement_input_handler_.getIsLine(),
                    settlement_input_handler_.getIsArrow()
                );
            }
        }
#endif

        /// @brief イベントを購読
        /// @brief Subscribe to events
        void subscribeToEvents() {
            paxs::EventBus& event_bus = paxs::EventBus::getInstance();

            // ビューポート変更イベントの購読
            // すべてのコンテンツを更新（ビューポート変更時は全て再描画が必要）
            event_bus.subscribe<ViewportChangedEvent>(
                [this](const ViewportChangedEvent& event) {
                    (void)event;
                    updateAllFeatures();
#ifdef PAXS_USING_SIMULATOR
                    updateSettlementData();
#endif
                }
            );

            // 日付変更イベントの購読
            // Settlement以外のコンテンツを更新（人物肖像画、地理的特徴は日付依存）
            event_bus.subscribe<DateChangedEvent>(
                [this](const DateChangedEvent& event) {
                    (void)event;
                    updateAllFeatures();
                }
            );

#ifdef PAXS_USING_SIMULATOR
            // シミュレーション状態変更イベントの購読（初期化完了検知）
            event_bus.subscribe<SimulationStateChangedEvent>(
                [this](const SimulationStateChangedEvent& event) {
                    // 停止状態になった時（初期化完了時）に集落データを更新
                    if (event.new_state == SimulationState::Stopped) {
                        updateSettlementData();
                    } else if (event.new_state == SimulationState::Uninitialized) {
                        // クリアされた場合はキャッシュをクリアして無効な参照を防ぐ
                        settlement_manager_.clearCache();
                    }
                }
            );

            // シミュレーションステップ実行イベントの購読
            // Settlementデータのみ更新（シミュレーション進行時）
            event_bus.subscribe<SimulationStepExecutedEvent>(
                [this](const SimulationStepExecutedEvent& event) {
                    (void)event;
                    updateSettlementData();
                }
            );

            // 集落表示設定変更イベントの購読
            // Settlement表示設定（select_draw, is_line, is_arrow）変更時
            event_bus.subscribe<SettlementDisplayChangedEvent>(
                [this](const SettlementDisplayChangedEvent& event) {
                    (void)event;
                    updateSettlementData();
                }
            );
#endif
        }

        /// @brief RenderContextを更新
        void updateRenderContext() {
            const auto& koyomi = app_state_manager_.getKoyomi();
            render_context_.jdn = koyomi.jdn.getDay();
            render_context_.map_view_width = map_viewport_.getWidth();
            render_context_.map_view_height = map_viewport_.getHeight();
            render_context_.map_view_center_x = map_viewport_.getCenterX();
            render_context_.map_view_center_y = map_viewport_.getCenterY();
            render_context_.visibility_manager = &app_state_manager_.getVisibilityManager();
        }

        /// @brief 全Featureのupdate()を呼び出し
        void updateAllFeatures() {
            updateRenderContext();
            for (auto& feature : features_) {
                if (feature && feature->isInTimeRange(render_context_.jdn)) {
                    feature->update(render_context_);
                }
            }
        }

        /// @brief 人物データを読み込み
        void loadPersonFeatures() {
            // テクスチャを読み込み（Portraitsの設定を使用）
            const std::string portraits_path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("Portraits"));

            if (portraits_path.size() > 0) {
                asset_registry_.loadPersonIcons(portraits_path);
            }

            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_person_count = 1000;
            features_.reserve(current_capacity + estimated_person_count);

            // PersonFeatureに変換
            auto all_persons = PersonNameRepository::loadPersonNameList();
            for (const auto& [person_data, person_list] : all_persons) {
                features_.emplace_back(
                    std::make_unique<PersonFeature>(person_data, person_list)
                );
            }
        }

        /// @brief 地理的地物データを読み込み
        void loadGeographicFeatures() {
            // 容量を事前確保（地理的地物は多いため5000を確保）
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_geographic_count = 5000;
            features_.reserve(current_capacity + estimated_geographic_count);

            // GeographicFeatureに変換
            auto all_geographic_features = GeographicFeatureRepository::loadGeographicFeatureList();
            for (auto& location_data : all_geographic_features) {
                features_.emplace_back(
                    std::make_unique<GeographicFeature>(std::move(location_data))
                );
            }
        }

        /// @brief 地名データを読み込み
        void loadPlaceNameFeatures() {
            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_place_name_count = 1000;
            features_.reserve(current_capacity + estimated_place_name_count);

            // PlaceNameFeatureに変換
            auto all_place_names = PlaceNameRepository::loadPlaceNameList();
            for (auto& location_data : all_place_names) {
                features_.emplace_back(
                    std::make_unique<PlaceNameFeature>(std::move(location_data))
                );
            }
        }

        /// @brief ゲノムデータを読み込み
        void loadGenomeFeatures() {
            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_genome_count = 500;
            features_.reserve(current_capacity + estimated_genome_count);

            // GenomeFeatureに変換
            auto all_genomes = GenomeRepository::loadGenomeList();
            for (auto& genome_data : all_genomes) {
                features_.emplace_back(
                    std::make_unique<GenomeFeature>(std::move(genome_data))
                );
            }
        }

        /// @brief 全データを読み込み
        void loadAllFeatures() {
            features_.clear();

            // MapIconsを1回だけロード（地理・ゲノム共用）
            const std::string map_icons_path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("MapIcons"));
            if (map_icons_path.size() > 0) {
                asset_registry_.loadMapIcons(map_icons_path);
            }

            std::cout << "Loading person features...\n";
            loadPersonFeatures();
            std::cout << "Person features loaded: " << features_.size() << "\n";

            std::cout << "Loading geographic features...\n";
            loadGeographicFeatures();
            std::cout << "Geographic features loaded: " << features_.size() << " total\n";

            std::cout << "Loading place name features...\n";
            loadPlaceNameFeatures();
            std::cout << "Place name features loaded: " << features_.size() << " total\n";

            std::cout << "Loading genome features...\n";
            loadGenomeFeatures();
            std::cout << "Genome features loaded: " << features_.size() << " total\n";

            // 全アイコンをマージ
            asset_registry_.mergeCategories();
        }

    public:
        MapContentLayer(const AppStateManager& app_state_manager)
            : app_state_manager_(app_state_manager)
            , map_viewport_(app_state_manager.getMapViewport()) {
            // データロード（初回のみ）
            if (features_.empty()) {
                loadAllFeatures();
            }
            subscribeToEvents();
            // 初回更新を即座に実行
            updateAllFeatures();
#ifdef PAXS_USING_SIMULATOR
            updateSettlementData();
#endif
        }

        // コピー・ムーブ禁止（観察ポインタとイベント購読を持つため）
        ~MapContentLayer() override = default;
        MapContentLayer(const MapContentLayer&) = delete;
        auto operator=(const MapContentLayer&) -> MapContentLayer& = delete;
        MapContentLayer(MapContentLayer&&) = delete;
        auto operator=(MapContentLayer&&) -> MapContentLayer& = delete;

        void render() const override {
            if (!isVisible()) {
                return;
            }

            MapFeatureRenderer::drawFeatures(features_, render_context_, asset_registry_.getMergedMap());

#ifdef PAXS_USING_SIMULATOR
            // SettlementManager を描画（シミュレーション表示時）
            if (app_state_manager_.getVisibilityManager().isVisible(ViewMenu::simulation)) {
                settlement_manager_.render();
            }
#endif
        }

#ifdef PAXS_USING_SIMULATOR
        /// @brief SettlementInputHandler への参照を取得（AppComponentManager での登録用）
        SettlementInputHandler& getSettlementInputHandler() {
            return settlement_input_handler_;
        }
#endif

        const std::vector<std::unique_ptr<MapFeature>>& getFeatures() const {
            return features_;
        }

        /// @brief RenderContextへの参照を取得（入力処理用）
        const RenderContext& getRenderContext() const {
            return render_context_;
        }

        /// @brief IDでFeatureを検索
        MapFeature* findFeatureById(const std::string& id) {
            for (auto& feature : features_) {
                if (feature && feature->getId() == id) {
                    return feature.get();
                }
            }
            return nullptr;
        }

        /// @brief マウス座標でFeatureを検索（入力処理用）
        MapFeature* findFeatureAt(const paxg::Vec2i& mouse_pos) {
            for (auto& feature : features_) {
                if (feature && feature->isVisible() && feature->isHit(mouse_pos)) {
                    return feature.get();
                }
            }
            return nullptr;
        }

        bool isVisible() const override {
            return app_state_manager_.getVisibilityManager().isVisible(ViewMenu::map);
        }
        RenderLayer getLayer() const override { return RenderLayer::MapContent; }
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP
