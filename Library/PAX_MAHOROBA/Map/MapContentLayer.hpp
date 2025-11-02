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
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Map/Location/PlaceNameManager.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_MAHOROBA/Rendering/TextureManager.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonNameManager.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>

#include <PAX_SAPIENTICA/Map/MapDomainLogic.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 地図コンテンツレイヤークラス
    /// @brief Map Content Layer
    ///
    /// 地図上のコンテンツ（地名、人名、集落等）の管理と描画を統合的に担当します。
    /// Manages and renders map content (place names, person names, settlements, etc.) in an integrated manner.
    class MapContentLayer : public IRenderable, public IInputHandler {
    private:
        std::unique_ptr<TextureManager> texture_manager_; // 地図上に描画する画像の一覧

        PlaceNameManager place_name_manager_{}; // 地名
        PersonNameManager person_name_manager_{}; // 人名
#ifdef PAXS_USING_SIMULATOR
        SettlementManager settlement_manager_{}; // 集落管理
        SettlementInputHandler settlement_input_handler_; // 集落入力処理
#endif
        paxs::map::MapDomainLogic map_domain_logic_; // ドメインロジック

        // 依存性注入された参照
        FontManager* font_manager_ = nullptr;
        const SelectLanguage* select_language_ = nullptr;

        // 可視性・有効性管理
        bool visible_ = true;
        bool enabled_ = true;

        // 描画に必要なデータをキャッシュ（updateData()で更新、render()で使用）
        MapViewport cached_map_viewport_;
        paxs::Koyomi cached_koyomi_;
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator>* cached_simulator_ = nullptr;
#endif
        paxs::FeatureVisibilityManager* cached_visible_ = nullptr;

    public:
        MapContentLayer()
            :texture_manager_(std::make_unique<TextureManager>())
        {
            // メモリ割り当てチェック
            if (!texture_manager_) {
                PAXS_ERROR("Failed to allocate TextureManager");
            }
        }

        void init(FontManager& font_manager, const SelectLanguage& select_language) {
            // 地名
            place_name_manager_.init();
            place_name_manager_.add();
            person_name_manager_.init();
            person_name_manager_.add();

            // フォント管理への参照を保存
            font_manager_ = &font_manager;
            select_language_ = &select_language;
        }

        /// @brief データ更新（描画は行わない）
        /// @brief Update data (no drawing)
        void updateData(
            MapViewport& map_viewport,
            const paxs::Koyomi& koyomi,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            paxs::FeatureVisibilityManager& visible
            ) {
            // データ更新
            texture_manager_->update(map_viewport.getCenterX(), map_viewport.getCenterY(), map_viewport.getWidth(), map_viewport.getHeight());

            // 描画用にデータをキャッシュ
            cached_map_viewport_ = map_viewport;
            cached_koyomi_ = koyomi;
#ifdef PAXS_USING_SIMULATOR
            cached_simulator_ = &simulator;

            // SettlementManager に描画パラメータを設定
            if (simulator) {
                settlement_manager_.setDrawParams(
                    koyomi.jdn.cgetDay(),
                    simulator->getSettlementGrids(),
                    simulator->getMarriagePosList(),
                    map_viewport.getWidth(),
                    map_viewport.getHeight(),
                    map_viewport.getCenterX(),
                    map_viewport.getCenterY(),
                    settlement_input_handler_.getSelectDraw(),
                    settlement_input_handler_.getIsLine(),
                    settlement_input_handler_.getIsArrow()
                );
            }
#endif
            cached_visible_ = &visible;

            // PersonNameManager と PlaceNameManager に描画パラメータを設定
            // Set drawing parameters to PersonNameManager and PlaceNameManager
            if (font_manager_ && select_language_) {
                paxg::Font* main_font = font_manager_->getMainFont(*select_language_);

                person_name_manager_.setDrawParams(
                    koyomi.jdn.cgetDay(),
                    map_viewport.getWidth(),
                    map_viewport.getHeight(),
                    map_viewport.getCenterX(),
                    map_viewport.getCenterY(),
                    (main_font == nullptr) ? font_manager_->getPinFont() : (*main_font),
                    font_manager_->getEnFont(),
                    font_manager_->getPinFont()
                );

                place_name_manager_.setDrawParams(
                    visible,
                    koyomi.jdn.cgetDay(),
                    map_viewport.getWidth(),
                    map_viewport.getHeight(),
                    map_viewport.getCenterX(),
                    map_viewport.getCenterY(),
                    (main_font == nullptr) ? font_manager_->getPinFont() : (*main_font),
                    font_manager_->getEnFont(),
                    font_manager_->getPinFont()
                );
            }
        }

        // IRenderable の実装
        // IRenderable implementation

        /// @brief レンダリング処理
        /// @brief Render
        void render() override {
            if (!visible_ || cached_visible_ == nullptr) return;

            paxs::FeatureVisibilityManager& visible = *cached_visible_;

            if (visible.isVisible(MurMur3::calcHash("Map"))) { // 地図が「可視」の場合は描画する
                person_name_manager_.render();
                place_name_manager_.render();
            }

#ifdef PAXS_USING_SIMULATOR
            // SettlementManager を描画（シミュレーション表示時）
            // Render SettlementManager (when simulation is visible)
            if (visible.isVisible(MurMur3::calcHash("Simulation"))) {
                settlement_manager_.render();
            }
#endif
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }

        /// @brief 可視性を取得
        /// @brief Get visibility
        bool isVisible() const override {
            return visible_;
        }

        /// @brief 可視性を設定
        /// @brief Set visibility
        void setVisible(bool visible) override {
            visible_ = visible;
        }

        // IInputHandler の実装
        // IInputHandler implementation

        /// @brief 入力処理
        /// @brief Handle input
        bool handleInput(const InputEvent& event) override {
            if (!visible_ || !enabled_ || cached_visible_ == nullptr) return false;

#ifdef PAXS_USING_SIMULATOR
            // 集落の入力処理
            // Settlement input processing
            if (cached_visible_->isVisible(MurMur3::calcHash("Map")) || cached_visible_->isVisible(MurMur3::calcHash("Simulation"))) {
                if (cached_simulator_ && *cached_simulator_) {
                    settlement_input_handler_.handleInput(event);
                }
            }
#endif
            // 入力を消費しない（背後のハンドラーにも伝播させる）
            return false;
        }

        /// @brief ヒットテスト
        /// @brief Hit test
        bool hitTest(int x, int y) const override {
            // 地図全体が対象なので常にtrue
            // Always true as the entire map is the target
            return visible_ && enabled_;
        }

        /// @brief 有効性を取得
        /// @brief Get enabled state
        bool isEnabled() const override {
            return enabled_;
        }

#ifdef PAXS_USING_SIMULATOR
        /// @brief SettlementInputHandler への参照を取得（GraphicsManager での登録用）
        /// @brief Get reference to SettlementInputHandler (for registration in GraphicsManager)
        SettlementInputHandler& getSettlementInputHandler() {
            return settlement_input_handler_;
        }
#endif
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTENT_LAYER_HPP
