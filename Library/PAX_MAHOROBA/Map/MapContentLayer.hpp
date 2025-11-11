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
#include <PAX_SAPIENTICA/Simulation/SimulationManager.hpp>
#endif

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Map/Location/GeographicFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/MapFeatureRenderer.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/PlaceNameFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/RenderContext.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/PersonNameRepository.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/PlaceNameRepository.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    /// @brief 地図コンテンツレイヤークラス
    /// @brief Map Content Layer
    class MapContentLayer : public IRenderable {
    private:
        // 新しいFeatureベース構造
        std::vector<std::unique_ptr<MapFeature>> features_; ///< 地物のコレクション / Collection of features
        RenderContext render_context_; ///< 描画コンテキスト / Render context

        // データリポジトリ
        PersonNameRepository person_repository_;
        PlaceNameRepository place_repository_;
        paxs::KeyValueTSV<paxg::Texture> person_texture_map_; ///< 人物用テクスチャマップ / Person texture map
        paxs::KeyValueTSV<paxg::Texture> geographic_texture_map_; ///< 地理用テクスチャマップ / Geographic texture map
#ifdef PAXS_USING_SIMULATOR
        SettlementManager settlement_manager_{}; // 集落管理
        SettlementInputHandler settlement_input_handler_; // 集落入力処理
#endif

        const MapViewport* map_viewport_ptr = nullptr;

        EventBus* event_bus_ = nullptr;
        AppStateManager* app_state_manager_ = nullptr;


#ifdef PAXS_USING_SIMULATOR
        /// @brief Settlementデータのみ更新
        /// @brief Update settlement data only
        void updateSettlementData() {
            if (!app_state_manager_) return;

            const auto& koyomi = app_state_manager_->getKoyomi();
            const auto& simulation_manager = app_state_manager_->getSimulationManager();

            // SettlementManager に描画パラメータを設定
            if (simulation_manager.isActive()) {
                settlement_manager_.setDrawParams(
                    koyomi.jdn.cgetDay(),
                    simulation_manager.getSettlementGrids(),
                    simulation_manager.getMarriagePositions(),
                    map_viewport_ptr->getWidth(),
                    map_viewport_ptr->getHeight(),
                    map_viewport_ptr->getCenterX(),
                    map_viewport_ptr->getCenterY(),
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
            if (event_bus_ == nullptr) return;

            // ビューポート変更イベントの購読
            // すべてのコンテンツを更新（ビューポート変更時は全て再描画が必要）
            event_bus_->subscribe<ViewportChangedEvent>(
                [this](const ViewportChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        updateAllFeatures();
#ifdef PAXS_USING_SIMULATOR
                        updateSettlementData();
#endif
                    }
                }
            );

            // 日付変更イベントの購読
            // Settlement以外のコンテンツを更新（人物肖像画、地理的特徴は日付依存）
            event_bus_->subscribe<DateChangedEvent>(
                [this](const DateChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        updateAllFeatures();
                    }
                }
            );

#ifdef PAXS_USING_SIMULATOR
            // シミュレーション状態変更イベントの購読（初期化完了検知）
            event_bus_->subscribe<SimulationStateChangedEvent>(
                [this](const SimulationStateChangedEvent& event) {
                    // 停止状態になった時（初期化完了時）に集落データを更新
                    if (event.new_state == SimulationState::Stopped) {
                        if (app_state_manager_) {
                            updateSettlementData();
                        }
                    } else if (event.new_state == SimulationState::Uninitialized) {
                        // クリアされた場合はキャッシュをクリアして無効な参照を防ぐ
                        settlement_manager_.clearCache();
                    }
                }
            );

            // シミュレーションステップ実行イベントの購読
            // Settlementデータのみ更新（シミュレーション進行時）
            event_bus_->subscribe<SimulationStepExecutedEvent>(
                [this](const SimulationStepExecutedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        updateSettlementData();
                    }
                }
            );

            // 集落表示設定変更イベントの購読
            // Settlement表示設定（select_draw, is_line, is_arrow）変更時
            event_bus_->subscribe<SettlementDisplayChangedEvent>(
                [this](const SettlementDisplayChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        updateSettlementData();
                    }
                }
            );
#endif
        }

        /// @brief RenderContextを更新
        void updateRenderContext() {
            if (!map_viewport_ptr || !app_state_manager_) return;

            const auto& koyomi = app_state_manager_->getKoyomi();
            render_context_.jdn = koyomi.jdn.cgetDay();
            render_context_.map_view_width = map_viewport_ptr->getWidth();
            render_context_.map_view_height = map_viewport_ptr->getHeight();
            render_context_.map_view_center_x = map_viewport_ptr->getCenterX();
            render_context_.map_view_center_y = map_viewport_ptr->getCenterY();
            render_context_.visibility_manager = &app_state_manager_->getVisibilityManager();
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
            std::string portraits_path = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Portraits"),
                [&](const std::string& path_) { portraits_path = path_; });

            if (portraits_path.size() > 0) {
                person_texture_map_.input(portraits_path);
            }

            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_person_count = 1000;
            features_.reserve(current_capacity + estimated_person_count);

            // PersonNameRepository.loadPersonNameList() takes only a callback
            // The callback is invoked for each file path found in the index file
            auto inputPlaceFunc = [this](const std::string& path, double min_view, double max_view,
                                          int min_year, int max_year,
                                          std::uint_least32_t lpe, std::uint_least32_t place_texture) {
                auto loaded = person_repository_.loadPersonFromFile(path, min_view, max_view, min_year, max_year, lpe, place_texture);
                if (loaded.person_location_list.empty()) return;

                // PersonFeatureに変換
                for (const auto& person_data : loaded.person_location_list) {
                    features_.emplace_back(
                        std::make_unique<PersonFeature>(person_data, loaded)
                    );
                }
            };

            person_repository_.loadPersonNameList(inputPlaceFunc);
        }

        /// @brief 地理的地物データを読み込み
        void loadGeographicFeatures() {
            // 地理的特徴用のテクスチャを読み込み（MiniIconsから）
            std::string mini_icons_path = "";
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("MiniIcons"),
                [&](const std::string& path_) { mini_icons_path = path_; });
            if (mini_icons_path.size() > 0) {
                geographic_texture_map_.input(mini_icons_path);
            }

            // 容量を事前確保（地理的地物は多いため5000を確保）
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_geographic_count = 5000;
            features_.reserve(current_capacity + estimated_geographic_count);

            // PlaceNameRepository.loadPlaceNameList() takes only a callback
            // The callback is invoked for each file path found in the index file
            auto inputPlaceFunc = [this](const std::string& path, double min_view, double max_view,
                                          int min_year, int max_year,
                                          std::uint_least32_t lpe, std::uint_least32_t place_texture,
                                          double zoom) {
                auto loaded = place_repository_.loadPlaceFromFile(path, min_view, max_view, min_year, max_year, lpe, place_texture, zoom);
                if (loaded.location_point_list.empty()) return;

                // 地名とアイコンの判定ロジック
                // place_textureがある場合はアイコン（GeographicFeature）
                // place_textureがない場合は地名（PlaceNameFeature）
                for (const auto& location_data : loaded.location_point_list) {
                    const std::uint_least32_t effective_texture =
                        (location_data.place_texture == 0) ? loaded.place_texture : location_data.place_texture;

                    if (effective_texture != 0) {
                        // テクスチャがある場合はアイコン
                        features_.emplace_back(
                            std::make_unique<GeographicFeature>(location_data)
                        );
                    } else {
                        // テクスチャがない場合は地名
                        features_.emplace_back(
                            std::make_unique<PlaceNameFeature>(location_data)
                        );
                    }
                }
            };

            place_repository_.loadPlaceNameList(inputPlaceFunc);
        }

        /// @brief 全データを読み込み
        void loadAllFeatures() {
            features_.clear();
            std::cout << "Loading person features..." << std::endl;
            loadPersonFeatures();
            std::cout << "Person features loaded: " << features_.size() << std::endl;

            std::cout << "Loading geographic features..." << std::endl;
            loadGeographicFeatures();
            std::cout << "Geographic features loaded: " << features_.size() << " total" << std::endl;
        }

    public:
        MapContentLayer(const MapViewport* map_viewport)
            : map_viewport_ptr(map_viewport) {}

        // コピー・ムーブ禁止（観察ポインタとイベント購読を持つため）
        MapContentLayer(const MapContentLayer&) = delete;
        MapContentLayer& operator=(const MapContentLayer&) = delete;
        MapContentLayer(MapContentLayer&&) = delete;
        MapContentLayer& operator=(MapContentLayer&&) = delete;

        /// @brief AppStateManagerを設定してイベント駆動を有効化
        void setAppStateManager(AppStateManager* app_state_manager) {
            app_state_manager_ = app_state_manager;
            if (app_state_manager_ != nullptr) {
                event_bus_ = &EventBus::getInstance();
#ifdef PAXS_USING_SIMULATOR
                settlement_input_handler_.setEventBus(event_bus_);
#endif
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
        }

        void render() const override {
            if (!isVisible() || !app_state_manager_) return;

            UnorderedMap<std::uint_least32_t, paxg::Texture> merged_textures = person_texture_map_.get();
            for (const auto& [key, texture] : geographic_texture_map_.get()) {
                merged_textures.emplace(key, texture);
            }
            MapFeatureRenderer::drawFeatures(features_, render_context_, merged_textures);

#ifdef PAXS_USING_SIMULATOR
            // SettlementManager を描画（シミュレーション表示時）
            if (app_state_manager_->getVisibilityManager().isVisible(ViewMenu::simulation)) {
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
            return app_state_manager_->getVisibilityManager().isVisible(ViewMenu::map);
        }
        void setVisible(bool /*visible*/) override {}
        RenderLayer getLayer() const override { return RenderLayer::MapContent; }
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP
