/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP
#define PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP

#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {
	/// @brief シミュレーションのパネルクラス
	/// @brief Simulation panel class
	class SimulationPanel : public IUIWidget {
	private:
		bool visible_ = true;
		bool enabled_ = true;
		paxg::Vec2i pos_{0, 0};
		int debug_start_y_ = 0;
		int pulldown_y_ = 600; // プルダウンのY座標

		// 外部参照（UIManagerから設定される）
		std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr_ = nullptr;
		paxs::InputStateManager* input_state_manager_ = nullptr;
		paxs::Koyomi* koyomi_ = nullptr;
		paxs::FeatureVisibilityManager* visible_list_ = nullptr;
		MapViewport* map_viewport_ = nullptr;
		LanguageFonts* language_fonts_ = nullptr;
		const SelectLanguage* select_language_ = nullptr;
		const paxs::Language* language_text_ = nullptr;

		// 影描画用テクスチャ（外部から注入）
		paxg::RenderTexture* shadow_texture_ = nullptr;
		paxg::RenderTexture* internal_texture_ = nullptr;
		/// @brief シミュレーションを初期化する
		/// @brief Initialize the simulation
		/// @param simulator シミュレータのユニークポインタ
		/// @param koyomi 暦情報
		void simulationInit(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::Koyomi& koyomi
		) const {
			const std::string model_name =
				(simulation_model_index >= simulation_model_name.size()) ?
				"Sample" : simulation_model_name[simulation_model_index];

			simulator->init();
			koyomi.steps.setDay(0); // ステップ数を 0 にする
			koyomi.jdn.setDay(static_cast<double>(SimulationConstants::getInstance(model_name)->start_julian_day)); // シミュレーション初期時の日付に設定
			koyomi.calcDate();
			koyomi.is_agent_update = false;
			koyomi.move_forward_in_time = false; // 一時停止
			koyomi.go_back_in_time = false;
		}

		/// @brief シミュレーションのUI描画と操作処理
		/// @brief Simulation UI drawing and operation processing
        /// @param simulator シミュレータのユニークポインタ
        /// @param koyomi 暦情報
        /// @param debug_start_y UIの開始Y座標
        void simulation(
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
            paxs::Koyomi& koyomi,
            int debug_start_y
        ) {
            if (!input_state_manager_) return;  // Null check
			const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();
			const int time_icon_size = 40; // 時間操作アイコンの大きさ

			const std::string model_name =
				(simulation_model_index >= simulation_model_name.size()) ?
				"Sample" : simulation_model_name[simulation_model_index];

			std::string map_list_path = "Data/Simulations/" + model_name + "/MapList.tsv";
			std::string japan_provinces_path = "Data/Simulations/" + model_name;

			// シミュレーションが初期化されていない場合
			if (simulator.get() == nullptr) {
				texture_dictionary.at(MurMur3::calcHash("texture_load_geographic_data2")).resizedDraw(
					time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
				if (input_state_manager_->get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 360, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {

					AppConfig::getInstance()->calcDataSettingsNotPath(MurMur3::calcHash("SimulationXYZTiles"),
						[&](const std::string& path_) {map_list_path = path_; });
					AppConfig::getInstance()->calcDataSettingsNotPath(MurMur3::calcHash("SimulationProvincesPath"),
						[&](const std::string& path_) {japan_provinces_path = path_; });
					// Sample を選択モデル名に置換
					paxs::StringExtensions::replace(map_list_path, "Sample", model_name);
					paxs::StringExtensions::replace(japan_provinces_path, "Sample", model_name);
					// シミュレーション変数を初期化
					SimulationConstants::getInstance(model_name)->init(model_name);
#ifdef PAXS_USING_SIV3D
					static bool is_console_open = false;
					if (!is_console_open) {
						s3d::detail::Console_impl{}.open(); // コンソールを開く s3d::Console::Open()
						is_console_open = true;
					}
#endif
					std::random_device seed_gen;
					simulator = std::make_unique<paxs::SettlementSimulator>(
						map_list_path, japan_provinces_path,
						seed_gen());
					simulationInit(simulator, koyomi);
				}
			}
			// シミュレーションが初期化されている場合
			else {

				const auto* constants = SimulationConstants::getInstance(model_name);
				const int total_steps = constants->total_steps;

				// 規定ステップ数に達したかチェック
				if (total_steps > 0 && koyomi.steps.cgetDay() >= static_cast<std::size_t>(total_steps)) {
					// 残り実行回数を減らす
					m_remaining_iterations--;

					if (m_remaining_iterations > 0) {
						// まだ実行回数が残っている場合、シミュレーションを初期化して自動で再開
						simulationInit(simulator, koyomi);
						koyomi.is_agent_update = true;
						koyomi.move_forward_in_time = true;
					}
					else {
						// 全ての実行が終了した場合、シミュレーションを停止
						koyomi.is_agent_update = false;
						koyomi.move_forward_in_time = false;
						m_remaining_iterations = 0; //念のため0にリセット
					}
				}

				// シミュレーションが再生されている場合
				if (koyomi.is_agent_update) {
					// シミュレーションを停止
					texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
					if (input_state_manager_->get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 300, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						// if (s3d::SimpleGUI::Button(U"Sim Stop", s3d::Vec2{ 330, 60 })) {
						koyomi.is_agent_update = false;

						koyomi.move_forward_in_time = false; // 一時停止
						koyomi.go_back_in_time = false;
					}
				}
				// シミュレーションが再生されていない場合
				else {
					// シミュレーション入力データを初期化
					texture_dictionary.at(MurMur3::calcHash("texture_reload")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60));
					if (input_state_manager_->get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						SimulationConstants::getInstance(model_name)->init(model_name);
					}
					// 人間データを初期化
					texture_dictionary.at(MurMur3::calcHash("texture_load_agent_data2")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y));
					if (input_state_manager_->get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 420, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						simulationInit(simulator, koyomi);

						koyomi.steps.setDay(0); // ステップ数を 0 にする
						koyomi.calcDate();
					}
					// 地形データを削除
					texture_dictionary.at(MurMur3::calcHash("texture_delete_geographic_data")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
					if (input_state_manager_->get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 360, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						simulator.reset();

						koyomi.steps.setDay(0); // ステップ数を 0 にする
						koyomi.calcDate();
					}

					// シミュレーションを再生
					texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
					if (input_state_manager_->get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 300, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						// if (s3d::SimpleGUI::Button(U"Sim Start", s3d::Vec2{ 190, 60 })) {
						koyomi.is_agent_update = true;

						// 実行回数をセット
						m_remaining_iterations = SimulationConstants::getInstance(model_name)->num_iterations;

						koyomi.move_forward_in_time = true; // 再生
						koyomi.go_back_in_time = false;
					}
					// シミュレーションを 1 Step 実行
					texture_dictionary.at(MurMur3::calcHash("texture_1step")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 240, debug_start_y));
					if (input_state_manager_->get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 240, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						simulator->step(); // シミュレーションを 1 ステップ実行する
						koyomi.steps.getDay()++; // ステップ数を増やす
						koyomi.calcDate();

						koyomi.move_forward_in_time = false; // 一時停止
						koyomi.go_back_in_time = false;
					}
				}
			}
		}

	public:
		// シミュレーションの Key
		std::vector<std::uint_least32_t> simulation_key;
		// シミュレーションモデル名
		std::vector<std::string> simulation_model_name;

		// シミュレーションのモデル番号
		std::size_t simulation_model_index = 0;
		// シミュレーションの繰り返し回数
		int m_remaining_iterations = 0;

		paxs::Pulldown simulation_pulldown;

		paxs::KeyValueTSV<paxg::Texture> key_value_tsv;

		/// @brief シミュレーションパネルの初期化
		/// @brief Initialize the simulation panel
		/// @param select_language 選択された言語
		/// @param simulation_text シミュレーション関連のテキスト
		/// @param language_fonts 言語フォント
		void init(
			const SelectLanguage& select_language,
			const paxs::Language& simulation_text,
			LanguageFonts& language_fonts
		) {
			// シミュレーションモデルのファイルを読み込む
			const std::string models_path = "Data/Simulations/Models.txt";
			paxs::InputFile models_tsv(AppConfig::getInstance()->getRootPath() + models_path);
			if (models_tsv.fail()) {
				PAXS_WARNING("Failed to read Models TXT file: " + models_path);
				simulation_model_name.emplace_back("Sample");
				simulation_key.emplace_back(MurMur3::calcHash("Sample"));
			}
			else {
				// 1 行目を読み込む
				if (!(models_tsv.getLine())) {
					simulation_model_name.emplace_back("Sample");
					simulation_key.emplace_back(MurMur3::calcHash("Sample"));
				}
				else {
					// BOM を削除
					models_tsv.deleteBOM();
					// 1 行目を分割する
					simulation_model_name.emplace_back(models_tsv.pline);
					simulation_key.emplace_back(MurMur3::calcHash(models_tsv.pline.c_str()));
					// 1 行ずつ読み込み（区切りはタブ）
					while (models_tsv.getLine()) {
						simulation_model_name.emplace_back(models_tsv.pline);
						simulation_key.emplace_back(MurMur3::calcHash(models_tsv.pline.c_str()));
					}
				}
			}

			// シミュレーションモデルのプルダウンメニューを初期化
			simulation_pulldown = paxs::Pulldown(&select_language, &simulation_text, simulation_key, language_fonts, static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_SIZE), static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS), paxg::Vec2i{ 3000, 0 }, paxs::PulldownDisplayType::SelectedValue, false);
			pulldown_y_ = 600;
			simulation_pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().w() - 200), pulldown_y_ });

			// 暦の時間操作のアイコン
			key_value_tsv.input(paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ value_ }; });
		}

		/// @brief 影用のテクスチャを設定
		/// @brief Set textures for shadow rendering
		void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
			shadow_texture_ = &shadow_tex;
			internal_texture_ = &internal_tex;
		}

		/// @brief シミュレーションの更新と描画
		/// @brief Update and draw simulation
        /// @param simulator シミュレータのユニークポインタ
        /// @param koyomi 暦情報
        /// @param debug_start_y UIの開始Y座標
        /// @param visible 可視性フラグ
        void update(
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
            paxs::Koyomi& koyomi,
            int debug_start_y,
            paxs::FeatureVisibilityManager& visible
        ) {
            if (!input_state_manager_) return;  // Null check
			// シミュレーションのボタン
			if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
				simulation(simulator, koyomi, debug_start_y);
			}
		}

		/// @brief シミュレーションプルダウンの描画
		/// @brief Draw simulation pulldown
		/// @param simulator シミュレータのユニークポインタ
		/// @param visible 可視性フラグ
		void drawPulldown(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::FeatureVisibilityManager& visible
		) {
			// シミュレーションのボタン
			if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
				if (simulator == nullptr) {
					// 画面サイズに合わせて位置を更新
					simulation_pulldown.setPos(paxg::Vec2i{
						static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().w() - 200),
						pulldown_y_
					});
					simulation_pulldown.draw(); // シミュレーション選択
				}
			}
		}

		/// @brief シミュレーションプルダウンの背景描画
		/// @brief Draw simulation pulldown background
		/// @param simulator シミュレータのユニークポインタ
		/// @param visible 可視性フラグ
		void drawPulldownBackground(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::FeatureVisibilityManager& visible
		) {
			// シミュレーションのボタン
			if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
				if (simulator == nullptr) {
					const float panel_height = static_cast<float>(simulation_pulldown.getRect().h());
					const float panel_width = static_cast<float>(paxg::Window::width());

#ifdef PAXS_USING_SIV3D
					// Siv3D: Use high-quality shadow renderer with Gaussian blur
					if (shadow_texture_ && internal_texture_) {
						paxs::ShadowRenderer::renderShadowWithPanels(
							*shadow_texture_,
							*internal_texture_,
							[panel_width, panel_height]() {
								// 影の形状を描画
								paxg::Rect{ 0, 0, panel_width, panel_height }.draw();
							},
							[panel_width, panel_height]() {
								// パネル本体を描画
								paxg::Rect{ 0, 0, panel_width, panel_height }.draw(paxg::Color{243, 243, 243});
							}
						);
					}
#else
					// SFML/DxLib: Use simple shadow with drawShadow method
					paxg::Rect{ 0, 0, panel_width, panel_height }
						.drawShadow({1, 1}, 4, 1).draw(paxg::Color{243, 243, 243});
#endif
				}
			}
		}

		// ========================================
		// IUIWidget インターフェース実装
		// ========================================

		/// @brief コンポーネント名を取得
		const char* getName() const override {
			return "SimulationPanel";
		}

		/// @brief コンポーネントが利用可能かチェック
		bool isAvailable() const override {
			return true; // PAXS_USING_SIMULATORが定義されている場合のみコンパイルされる
		}

		/// @brief 有効状態を設定
		void setEnabled(bool enabled) override {
			enabled_ = enabled;
		}

		/// @brief 有効状態を取得
		bool isEnabled() const override {
			return enabled_;
		}

		/// @brief 外部参照を設定
		/// @param simulator シミュレータのユニークポインタへの参照
		/// @param input_state_manager 入力状態マネージャー
		/// @param koyomi 暦情報
		/// @param visible_list 可視性リスト
		/// @param debug_start_y UIの開始Y座標
		void setReferences(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::InputStateManager& input_state_manager,
			paxs::Koyomi& koyomi,
			paxs::FeatureVisibilityManager& visible_list,
			MapViewport& map_viewport,
			LanguageFonts& language_fonts,
			const SelectLanguage& select_language,
			const paxs::Language& language_text,
			int debug_start_y
		) {
			simulator_ptr_ = &simulator;
			input_state_manager_ = &input_state_manager;
			koyomi_ = &koyomi;
			visible_list_ = &visible_list;
			map_viewport_ = &map_viewport;
			language_fonts_ = &language_fonts;
			select_language_ = &select_language;
			language_text_ = &language_text;
			debug_start_y_ = debug_start_y;
		}

		/// @brief 更新処理（IUIWidget）
		void update(paxs::InputStateManager& input_state_manager) override {
			if (!visible_ || !enabled_) return;
			if (!simulator_ptr_ || !koyomi_ || !visible_list_) return;

            // Temporarily set input_state_manager_ for this update cycle
            input_state_manager_ = &input_state_manager;

            // 既存のupdate()を呼び出し
            update(*simulator_ptr_, *koyomi_, debug_start_y_, *visible_list_);

            // プルダウンの更新
			if (visible_list_->at(MurMur3::calcHash("Simulation")) &&
			    visible_list_->at(MurMur3::calcHash("UI")) &&
			    visible_list_->at(MurMur3::calcHash("Calendar"))) {
				if (*simulator_ptr_ == nullptr) {
					simulation_pulldown.update(input_state_manager);
					simulation_model_index = simulation_pulldown.getIndex();
				}
			}
		}

		/// @brief 描画処理（IUIWidget）
		void draw() override {
			if (!visible_) return;
			if (!simulator_ptr_ || !visible_list_ || !koyomi_) return;

			// プルダウンの描画
			drawPulldown(*simulator_ptr_, *visible_list_);

			// シミュレーションコントロールボタンの描画
			if (visible_list_->at(MurMur3::calcHash("Simulation")) &&
			    visible_list_->at(MurMur3::calcHash("UI")) &&
			    visible_list_->at(MurMur3::calcHash("Calendar"))) {
				drawSimulationControls();
			}
		}

	private:
		/// @brief Z拡大率を描画
		/// @brief Draw Z magnification
		void drawZMagnification() {
			if (!map_viewport_ || !language_fonts_ || !select_language_ || !language_text_) return;

			// XYZ Tile Z magnification の計算
			const int magnification_z = static_cast<int>(-std::log2(map_viewport_->getHeight()) + 12.5);

			// フォントサイズを設定
			constexpr int font_size = 22;
			constexpr int font_buffer_thickness = 2;

			paxg::Font* font = language_fonts_->getAndAdd(select_language_->cgetKey(),
				static_cast<std::uint_least8_t>(font_size),
				static_cast<std::uint_least8_t>(font_buffer_thickness));
			if (font == nullptr) return;

			// ラベルテキストを取得
			const std::string* label_text_ptr = language_text_->getStringPtr(
				MurMur3::calcHash("debug_xyz_tiles_z"),
				select_language_->cgetKey()
			);
			if (label_text_ptr == nullptr) return;

			// Z拡大率のラベルを描画
			font->setOutline(0, 0.6, paxg::Color(255, 255, 255));
			font->drawTopRight(*label_text_ptr,
				paxg::Vec2i(paxg::Window::width() - 40, debug_start_y_), paxg::Color(0, 0, 0));

			// Z拡大率の値を描画
			font->drawTopRight(std::to_string(magnification_z),
				paxg::Vec2i(paxg::Window::width() - 40, debug_start_y_ + 30), paxg::Color(0, 0, 0));
		}

		/// @brief シミュレーションコントロールボタンを描画
		void drawSimulationControls() {
			if (!simulator_ptr_ || !koyomi_) return;

			const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();
			const int time_icon_size = 40;
			const int debug_start_y = debug_start_y_;

			// Z拡大率の表示
			drawZMagnification();

			// シミュレーションが初期化されていない場合
			if (simulator_ptr_->get() == nullptr) {
				// 地形データ読み込みボタン
				texture_dictionary.at(MurMur3::calcHash("texture_load_geographic_data2")).resizedDraw(
					time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
			}
			// シミュレーションが初期化されている場合
			else {
				// シミュレーションが再生中の場合
				if (koyomi_->is_agent_update) {
					// 停止ボタン
					texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
				}
				// シミュレーションが停止中の場合
				else {
					// シミュレーション入力データ初期化ボタン
					texture_dictionary.at(MurMur3::calcHash("texture_reload")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60));

					// 人間データ初期化ボタン (Simulation Init)
					texture_dictionary.at(MurMur3::calcHash("texture_load_agent_data2")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y));

					// 地形データ削除ボタン
					texture_dictionary.at(MurMur3::calcHash("texture_delete_geographic_data")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));

					// 再生ボタン
					texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));

					// 1ステップ実行ボタン
					texture_dictionary.at(MurMur3::calcHash("texture_1step")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 240, debug_start_y));
				}
			}
		}

	public:
		/// @brief 矩形を取得
		paxg::Rect getRect() const override {
			// シミュレーションパネルの矩形を返す
			// プルダウンがある場合はその矩形、ない場合は操作ボタンの領域
			if (simulator_ptr_ && *simulator_ptr_ == nullptr) {
				return simulation_pulldown.getRect();
			}
			// シミュレーション実行中の操作ボタン領域
			return paxg::Rect{
				static_cast<float>(paxg::Window::width() - 420),
				static_cast<float>(debug_start_y_),
				420.0f,
				100.0f
			};
		}

		/// @brief 位置を設定
		void setPos(const paxg::Vec2i& pos) override {
			pos_ = pos;
			// プルダウンの位置も更新
			simulation_pulldown.setPos(paxg::Vec2i{
				static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().w() - 200),
				pos.y()
			});
		}

		/// @brief 可視性を設定
		void setVisible(bool visible) override {
			visible_ = visible;
		}

		/// @brief 可視性を取得
		bool isVisible() const override {
			return visible_;
		}
	};
}

#endif // !PAX_MAHOROBA_UI_SIMULATION_PANEL_HPP
