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

#include <PAX_MAHOROBA/Map/MapController.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileManager.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>
#include <PAX_MAHOROBA/UI/UIManager.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
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
		UIManager ui_manager_;
		MapController map_controller_;

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
			// UIManagerを初期化（FontManagerを渡す）
			ui_manager_.init(font_manager_, select_language, language_text, simulation_text);

			// MapControllerを初期化（FontManagerを渡す）
			map_controller_.init(
				font_manager_,
				select_language,
				static_cast<std::uint_least8_t>(ui_manager_.getKoyomiFontSize()),
				static_cast<std::uint_least8_t>(ui_manager_.getKoyomiFontBufferThicknessSize())
			);

			old_width_ = paxg::Window::width();
			old_height_ = paxg::Window::height();
		}

		/// @brief TileManagerへのアクセス（初期化用）
		TileManager& getTileManager() { return tile_manager_; }

		/// @brief UIManagerへのアクセス
		UIManager& getUIManager() { return ui_manager_; }
		const UIManager& getUIManager() const { return ui_manager_; }

		/// @brief MapControllerへのアクセス
		MapController& getMapController() { return map_controller_; }
		const MapController& getMapController() const { return map_controller_; }

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

				// UIManagerのテクスチャを再初期化
				ui_manager_.handleWindowResize();
			}
			else {
				ui_manager_.resetSizeChangeCount();
			}

			old_width_ = current_width;
			old_height_ = current_height;
		}

		/// @brief 更新・描画処理
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

			// 地図タイルを更新・描画
			tile_manager_.update(visible, map_viewport, koyomi.jdn.cgetDay());

			// 地図コントローラーを更新・描画
			map_controller_.update(
				map_viewport,
				koyomi,
#ifdef PAXS_USING_SIMULATOR
				simulator,
#endif
				visible
			);

			// UIを更新・描画
			ui_manager_.update(
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
		}
	};

}

#endif // !PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
