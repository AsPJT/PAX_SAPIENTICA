/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
#define PAX_MAHOROBA_GRAPHICS_MANAGER_HPP

#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Input/InputRouter.hpp>
#include <PAX_MAHOROBA/Input/MapViewportInputHandler.hpp>
#include <PAX_MAHOROBA/Map/MapContentLayer.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileManager.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayerManager.hpp>
#include <PAX_MAHOROBA/UI/UILayer.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

namespace paxs {

	/// @brief グラフィック統合管理クラス
	/// @brief Graphics integrated management class
	class GraphicsManager {
	private:
		FontManager font_manager_;
		TileManager tile_manager_;
		UILayer ui_manager_;
		MapContentLayer map_content_layer_;

		// レイヤーベースシステム（フェーズ1: 並行運用）
		// Layer-based system (Phase 1: parallel operation)
		RenderLayerManager render_layer_manager_;
		InputRouter input_router_;

		// MapViewportの入力処理（GraphicsManagerで管理）
		// MapViewport input handler (managed by GraphicsManager)
		MapViewportInputHandler* map_viewport_input_handler_ = nullptr;

	public:
		GraphicsManager() = default;

		/// @brief 初期化
		void init(
			const SelectLanguage& select_language,
			const paxs::Language& language_text,
			const paxs::Language& simulation_text
		) {
			font_manager_.init(select_language);
			ui_manager_.init(font_manager_, select_language, language_text, simulation_text);
			map_content_layer_.init(font_manager_, select_language);

			// レイヤーシステムに各コンポーネントを登録
			// Register components to layer system
			render_layer_manager_.registerRenderable(&tile_manager_);
			render_layer_manager_.registerRenderable(&map_content_layer_);
			render_layer_manager_.registerRenderable(&ui_manager_);

			// 入力ルーターに各コンポーネントを登録（UIが優先）
			// Register components to input router (UI has priority)
			input_router_.registerHandler(&ui_manager_);
			input_router_.registerHandler(&map_content_layer_);

#ifdef PAXS_USING_SIMULATOR
			// SettlementInputHandler を InputRouter に登録（MapContent層）
			// Register SettlementInputHandler to InputRouter (MapContent layer)
			input_router_.registerHandler(&map_content_layer_.getSettlementInputHandler());
#endif
		}

		/// @brief MapViewportInputHandlerを設定
		/// @brief Set MapViewportInputHandler
		/// @param handler MapViewportInputHandlerへのポインタ / Pointer to MapViewportInputHandler
		/// @param viewport MapViewportへのポインタ / Pointer to MapViewport
		///
		/// InputRouterにMapViewportInputHandlerを登録します。
		/// Registers MapViewportInputHandler to InputRouter.
		void setMapViewportInputHandler(MapViewportInputHandler* handler, MapViewport* viewport) {
			map_viewport_input_handler_ = handler;
			if (handler != nullptr) {
				handler->setViewport(viewport);
				// InputRouterに登録（最低優先度）
				input_router_.registerHandler(handler);
			}
		}

		/// @brief TileManagerへのアクセス（初期化用）
		TileManager& getTileManager() { return tile_manager_; }

		/// @brief UILayerへのアクセス
		UILayer& getUILayer() { return ui_manager_; }
		const UILayer& getUILayer() const { return ui_manager_; }

		/// @brief MapContentLayerへのアクセス
		/// @brief Access to MapContentLayer
		MapContentLayer& getMapContentLayer() { return map_content_layer_; }
		const MapContentLayer& getMapContentLayer() const { return map_content_layer_; }

		/// @brief RenderLayerManagerへのアクセス
		/// @brief Access to RenderLayerManager
		RenderLayerManager& getRenderLayerManager() { return render_layer_manager_; }
		const RenderLayerManager& getRenderLayerManager() const { return render_layer_manager_; }

		/// @brief InputRouterへのアクセス
		/// @brief Access to InputRouter
		InputRouter& getInputRouter() { return input_router_; }
		const InputRouter& getInputRouter() const { return input_router_; }


		/// @brief 更新・描画処理（レイヤーベースシステム使用）
		/// @brief Update and render (using layer-based system)
		void update(
			MapViewport& map_viewport,
			const SelectLanguage& select_language,
			const paxs::Language& language_text,
			paxs::Koyomi& koyomi,
#ifdef PAXS_USING_SIMULATOR
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
			paxs::InputStateManager& input_state_manager,
			paxs::FeatureVisibilityManager& visible
		) {
			// データ更新のみ実施（描画は分離）
			// Update data only (drawing is separated)
			tile_manager_.updateData(visible, map_viewport, koyomi.jdn.cgetDay());

			map_content_layer_.updateData(
				map_viewport,
				koyomi,
#ifdef PAXS_USING_SIMULATOR
				simulator,
#endif
				visible
			);

			ui_manager_.updateData(
				map_viewport,
				select_language,
				language_text,
#ifdef PAXS_USING_SIMULATOR
				simulator,
#endif
				input_state_manager,
				koyomi,
				visible
			);

#ifdef PAXS_USING_SIMULATOR
			// SettlementStatusPanel の表示モードを更新
			// Update SettlementStatusPanel display mode
			ui_manager_.getSettlementStatusPanel().setSelectDraw(
				map_content_layer_.getSettlementInputHandler().getSelectDraw()
			);

			// シミュレーターが初期化されている場合のみ表示
			// Show only when simulator is initialized
			ui_manager_.getSettlementStatusPanel().setVisible(simulator != nullptr);
#endif

			// レイヤーベース描画（Z順序自動管理）
			// Layer-based rendering (automatic Z-order management)
			render_layer_manager_.renderAll();
		}
	};

}

#endif // !PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
