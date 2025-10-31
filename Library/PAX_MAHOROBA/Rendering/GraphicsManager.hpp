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
#include <PAX_MAHOROBA/Map/MapController.hpp>
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
		MapController map_controller_;

		// レイヤーベースシステム（フェーズ1: 並行運用）
		// Layer-based system (Phase 1: parallel operation)
		RenderLayerManager render_layer_manager_;
		InputRouter input_router_;

		// ウィンドウサイズ変更の検知用
		int old_width_ = 0;
		int old_height_ = 0;

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
			map_controller_.init(font_manager_, select_language);

			old_width_ = paxg::Window::width();
			old_height_ = paxg::Window::height();

			// レイヤーシステムに各コンポーネントを登録
			// Register components to layer system
			render_layer_manager_.registerRenderable(&tile_manager_);
			render_layer_manager_.registerRenderable(&map_controller_);
			render_layer_manager_.registerRenderable(&ui_manager_);

			// 入力ルーターに各コンポーネントを登録（UIが優先）
			// Register components to input router (UI has priority)
			input_router_.registerHandler(&ui_manager_);
			input_router_.registerHandler(&map_controller_);
		}

		/// @brief TileManagerへのアクセス（初期化用）
		TileManager& getTileManager() { return tile_manager_; }

		/// @brief UILayerへのアクセス
		UILayer& getUILayer() { return ui_manager_; }
		const UILayer& getUILayer() const { return ui_manager_; }

		/// @brief MapControllerへのアクセス
		MapController& getMapController() { return map_controller_; }
		const MapController& getMapController() const { return map_controller_; }

		/// @brief RenderLayerManagerへのアクセス
		/// @brief Access to RenderLayerManager
		RenderLayerManager& getRenderLayerManager() { return render_layer_manager_; }
		const RenderLayerManager& getRenderLayerManager() const { return render_layer_manager_; }

		/// @brief InputRouterへのアクセス
		/// @brief Access to InputRouter
		InputRouter& getInputRouter() { return input_router_; }
		const InputRouter& getInputRouter() const { return input_router_; }

		/// @brief ウィンドウサイズ変更を検知してMapViewportを更新
		void handleWindowResize(MapViewport& map_viewport) {
			const int current_width = paxg::Window::width();
			const int current_height = paxg::Window::height();

			if (old_height_ != current_height || old_width_ != current_width) {
				// MapViewportのサイズ調整
				if (old_height_ != current_height) {
					map_viewport.setHeight(current_height * map_viewport.getHeight() / old_height_);
					map_viewport.setWidth(map_viewport.getHeight() / double(current_height) * double(current_width));
				}
				if (old_width_ != current_width) {
					map_viewport.setWidth(map_viewport.getHeight() / double(current_height) * double(current_width));
				}

				// UILayerのテクスチャを再初期化
				ui_manager_.handleWindowResize();
			}
			else {
				ui_manager_.resetSizeChangeCount();
			}

			old_width_ = current_width;
			old_height_ = current_height;
		}

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
			// ウィンドウサイズ変更の処理
			handleWindowResize(map_viewport);

			// データ更新のみ実施（描画は分離）
			// Update data only (drawing is separated)
			tile_manager_.updateData(visible, map_viewport, koyomi.jdn.cgetDay());

			map_controller_.updateData(
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

			// レイヤーベース描画（Z順序自動管理）
			// Layer-based rendering (automatic Z-order management)
			render_layer_manager_.renderAll();
		}
	};

}

#endif // !PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
