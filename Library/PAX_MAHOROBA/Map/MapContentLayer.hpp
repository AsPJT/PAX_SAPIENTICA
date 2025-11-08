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

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Input/SettlementInputHandler.hpp>
#include <PAX_MAHOROBA/Map/Location/SettlementManager.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationManager.hpp>
#endif

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Input/IEventHandler.hpp>
#include <PAX_MAHOROBA/Input/IMouseEventHandler.hpp>
#include <PAX_MAHOROBA/Map/Location/GeographicFeatureManager.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_MAHOROBA/Rendering/TextureManager.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonPortraitManager.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 地図コンテンツレイヤークラス
    /// @brief Map Content Layer
    class MapContentLayer : public IRenderable, public IEventHandler, public IMouseEventHandler {
    private:
        std::unique_ptr<TextureManager> texture_manager_; // 地図上に描画する画像の一覧

        GeographicFeatureManager geographic_feature_manager_{}; // 地理的特徴(地名とアイコン)
        PersonPortraitManager person_portrait_manager_{}; // 人物の肖像画と名前
#ifdef PAXS_USING_SIMULATOR
        SettlementManager settlement_manager_{}; // 集落管理
        SettlementInputHandler settlement_input_handler_; // 集落入力処理
#endif

        // 可視性・有効性管理
        bool visible_ = true;
        bool enabled_ = true;

        const MapViewport* map_viewport_ptr = nullptr;

        EventBus* event_bus_ = nullptr;
        AppStateManager* app_state_manager_ = nullptr;

    public:
        MapContentLayer(const MapViewport* map_viewport)
            : map_viewport_ptr(map_viewport), texture_manager_(std::make_unique<TextureManager>())
        {
            // メモリ割り当てチェック
            if (!texture_manager_) {
                PAXS_ERROR("Failed to allocate TextureManager");
            }
        }

        /// @brief AppStateManagerを設定してイベント駆動を有効化
        void setAppStateManager(AppStateManager* app_state_manager) {
            app_state_manager_ = app_state_manager;
            if (app_state_manager_ != nullptr) {
                event_bus_ = &EventBus::getInstance();
                subscribeToEvents();
                // 初回更新を即座に実行
                updateContentData();
            }
        }

        void render() const override {
            if (!visible_ || !app_state_manager_) return;

            // AppStateManagerから最新データを直接取得して描画のみ実行
            const auto& visible = app_state_manager_->getVisibilityManager();

            if (visible.isVisible(MurMur3::calcHash("Map"))) { // 地図が「可視」の場合は描画する
                person_portrait_manager_.render();
                geographic_feature_manager_.render();
            }

#ifdef PAXS_USING_SIMULATOR
            // SettlementManager を描画（シミュレーション表示時）
            // Render SettlementManager (when simulation is visible)
            if (visible.isVisible(MurMur3::calcHash("Simulation"))) {
                settlement_manager_.render();
            }
#endif
        }

        /// @brief キーボードイベント処理
        /// @param event キーボードイベント / Keyboard event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const KeyboardEvent& event) override {
            if (!app_state_manager_) return EventHandlingResult::NotHandled();

            const auto& visible = app_state_manager_->getVisibilityManager();

#ifdef PAXS_USING_SIMULATOR
            // 集落の入力処理
            if (visible.isVisible(MurMur3::calcHash("Map")) || visible.isVisible(MurMur3::calcHash("Simulation"))) {
                const auto& simulation_manager = app_state_manager_->getSimulationManager();
                if (simulation_manager.isActive()) {
                    settlement_input_handler_.handleEvent(event);
                }
            }
#endif
            // 入力を消費しない（背後のハンドラーにも伝播させる）
            return EventHandlingResult::NotHandled();
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // TODO: マウスイベント処理
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        bool isHit([[maybe_unused]] int x, [[maybe_unused]] int y) const override {
            // 地図全体が対象なので常にtrue
            return visible_ && enabled_;
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }
        bool isVisible() const override {
            return visible_;
        }
        void setVisible(bool visible) override {
            visible_ = visible;
        }
        bool isEnabled() const override {
            return enabled_;
        }

#ifdef PAXS_USING_SIMULATOR
        /// @brief SettlementInputHandler への参照を取得（GraphicsManager での登録用）
        SettlementInputHandler& getSettlementInputHandler() {
            return settlement_input_handler_;
        }
#endif

    private:
        /// @brief コンテンツデータを更新
        /// @brief Update content data
        void updateContentData() {
            if (!app_state_manager_) return;

            const auto& koyomi = app_state_manager_->getKoyomi();
            const auto& visible = app_state_manager_->getVisibilityManager();

            // テクスチャ更新
            texture_manager_->update(map_viewport_ptr->getCenterX(), map_viewport_ptr->getCenterY(),
                                    map_viewport_ptr->getWidth(), map_viewport_ptr->getHeight());

#ifdef PAXS_USING_SIMULATOR
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
#endif

            // 人物肖像画の描画パラメータ設定
            person_portrait_manager_.setDrawParams(
                koyomi.jdn.cgetDay(),
                map_viewport_ptr->getWidth(),
                map_viewport_ptr->getHeight(),
                map_viewport_ptr->getCenterX(),
                map_viewport_ptr->getCenterY()
            );

            // 地理的特徴の描画パラメータ設定
            geographic_feature_manager_.setDrawParams(
                const_cast<FeatureVisibilityManager&>(visible),
                koyomi.jdn.cgetDay(),
                map_viewport_ptr->getWidth(),
                map_viewport_ptr->getHeight(),
                map_viewport_ptr->getCenterX(),
                map_viewport_ptr->getCenterY()
            );
        }

        /// @brief イベントを購読
        /// @brief Subscribe to events
        void subscribeToEvents() {
            if (event_bus_ == nullptr) return;

            // ビューポート変更イベントの購読
            event_bus_->subscribe<ViewportChangedEvent>(
                [this](const ViewportChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にコンテンツデータを更新
                        updateContentData();
                    }
                }
            );

            // 日付変更イベントの購読
            event_bus_->subscribe<DateChangedEvent>(
                [this](const DateChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にコンテンツデータを更新
                        updateContentData();
                    }
                }
            );

            // 機能可視性変更イベントの購読
            event_bus_->subscribe<FeatureVisibilityChangedEvent>(
                [this](const FeatureVisibilityChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にコンテンツデータを更新
                        updateContentData();
                    }
                }
            );

#ifdef PAXS_USING_SIMULATOR
            // シミュレーション状態変更イベントの購読
            event_bus_->subscribe<SimulationStateChangedEvent>(
                [this](const SimulationStateChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にコンテンツデータを更新
                        updateContentData();
                    }
                }
            );

            // シミュレーションステップ実行イベントの購読
            event_bus_->subscribe<SimulationStepExecutedEvent>(
                [this](const SimulationStepExecutedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にコンテンツデータを更新
                        updateContentData();
                    }
                }
            );
#endif
        }
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP
