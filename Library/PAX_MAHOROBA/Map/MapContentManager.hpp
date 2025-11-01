/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTENT_MANAGER_HPP
#define PAX_MAHOROBA_MAP_CONTENT_MANAGER_HPP

#include <memory>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Map/Location/SettlementRenderer.hpp>
#include <PAX_MAHOROBA/Map/Location/Input/SettlementInputHandler.hpp>
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

    /// @brief 地図コンテンツ管理クラス
    /// @brief Map Content Manager
    ///
    /// 地図上のコンテンツ（地名、人名、集落等）の管理と描画を統合的に担当します。
    /// Manages and renders map content (place names, person names, settlements, etc.) in an integrated manner.
    class MapContentManager : public IRenderable, public IInputHandler {
    private:
        std::unique_ptr<TextureManager> texture_manager_; // 地図上に描画する画像の一覧

        PlaceNameManager place_name_manager_{}; // 地名
        PersonNameManager person_name_manager_{}; // 人名
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<SettlementRenderer> settlement_renderer; // 集落描画
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
        MapContentManager()
            :texture_manager_(std::make_unique<TextureManager>())
#ifdef PAXS_USING_SIMULATOR
            ,settlement_renderer(std::make_unique<SettlementRenderer>())
#endif
        {
            // メモリ割り当てチェック
            if (!texture_manager_) {
                PAXS_ERROR("Failed to allocate TextureManager");
            }
#ifdef PAXS_USING_SIMULATOR
            if (!settlement_renderer) {
                PAXS_ERROR("Failed to allocate SettlementRenderer");
            }
#endif
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
#endif
            cached_visible_ = &visible;
        }

        // IRenderable の実装
        // IRenderable implementation

        /// @brief レンダリング処理
        /// @brief Render
        void render() override {
            if (!visible_ || cached_visible_ == nullptr) return;

            paxs::FeatureVisibilityManager& visible = *cached_visible_;
            MapViewport& map_viewport = cached_map_viewport_;
            paxs::Koyomi& koyomi = cached_koyomi_;

            if (visible.isVisible(MurMur3::calcHash("Map"))) { // 地図が「可視」の場合は描画する
                // フォントを取得
                paxg::Font* main_font = font_manager_->getMainFont(*select_language_);

                // 描画処理（旧Presenterの処理を統合）
                const double width = map_viewport.getWidth();
                const double height = map_viewport.getHeight();
                const double center_x = map_viewport.getCenterX();
                const double center_y = map_viewport.getCenterY();
                const double julian_day = koyomi.jdn.cgetDay();

                // 地名を描画
                place_name_manager_.draw(
                    visible,
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    (main_font == nullptr) ? font_manager_->getPinFont() : (*main_font),
                    font_manager_->getEnFont(),
                    font_manager_->getPinFont()
                );

                // 人名を描画
                person_name_manager_.draw(
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    (main_font == nullptr) ? font_manager_->getPinFont() : (*main_font),
                    font_manager_->getEnFont(),
                    font_manager_->getPinFont()
                );

#ifdef PAXS_USING_SIMULATOR
                if (cached_simulator_ && *cached_simulator_) {
                    auto& simulator = **cached_simulator_;

                    // エージェントを描画
                    settlement_renderer->draw(
                        julian_day,
                        simulator.getSettlementGrids(),
                        simulator.getMarriagePosList(),
                        width,
                        height,
                        center_x,
                        center_y,
                        settlement_input_handler_.getSelectDraw(),
                        settlement_input_handler_.getIsLine(),
                        settlement_input_handler_.getIsArrow()
                    );

                    // エージェントのテキストを描画
                    settlement_renderer->drawText(settlement_input_handler_.getSelectDraw());
                }
#endif
            }
#ifdef PAXS_USING_SIMULATOR
            else if (visible.isVisible(MurMur3::calcHash("Simulation"))) {
                // シミュレーションのみ表示の場合
                if (settlement_renderer.get() != nullptr && cached_simulator_ && *cached_simulator_ && (*cached_simulator_).get() != nullptr) {
                    auto& simulator = **cached_simulator_;

                    settlement_renderer->draw(
                        koyomi.jdn.cgetDay(),
                        simulator.getSettlementGrids(),
                        simulator.getMarriagePosList(),
                        map_viewport.getWidth(),
                        map_viewport.getHeight(),
                        map_viewport.getCenterX(),
                        map_viewport.getCenterY(),
                        settlement_input_handler_.getSelectDraw(),
                        settlement_input_handler_.getIsLine(),
                        settlement_input_handler_.getIsArrow()
                    );
                    settlement_renderer->drawText(settlement_input_handler_.getSelectDraw());
                }
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
            // 集落の入力処理（Phase 14でhandleInput()に移行予定）
            // Settlement input processing (will migrate to handleInput() in Phase 14)
            if (cached_visible_->isVisible(MurMur3::calcHash("Map")) || cached_visible_->isVisible(MurMur3::calcHash("Simulation"))) {
                if (cached_simulator_ && *cached_simulator_) {
                    settlement_input_handler_.update();
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
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTENT_MANAGER_HPP
