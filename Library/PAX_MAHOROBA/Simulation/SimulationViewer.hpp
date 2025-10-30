/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_SIMULATION_SIMULATION_VIEWER_HPP
#define PAX_MAHOROBA_SIMULATION_SIMULATION_VIEWER_HPP

#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Calendar.hpp>
#include <PAX_MAHOROBA/IUIWidget.hpp>
#include <PAX_MAHOROBA/IViewerComponent.hpp>
#include <PAX_MAHOROBA/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Pulldown.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp>
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/TouchStateManager.hpp>

namespace paxs {
	/// @brief シミュレーションのビューアクラス
	/// @brief Simulation viewer class
	class SimulationViewer : public IViewerComponent, public IUIWidget {
	private:
		bool visible_ = true;
		bool enabled_ = true;
		paxg::Vec2i pos_{0, 0};
		int debug_start_y_ = 0;

		// 外部参照（UIManagerから設定される）
		std::unique_ptr<paxs::SettlementSimulator>* simulator_ptr_ = nullptr;
		paxs::TouchStateManager* touch_manager_ = nullptr;
		paxs::KoyomiSiv3D* koyomi_siv_ = nullptr;
		paxs::GraphicVisualizationList* visible_list_ = nullptr;
		/// @brief シミュレーションを初期化する
		/// @brief Initialize the simulation
		/// @param simulator シミュレータのユニークポインタ
		/// @param koyomi_siv 暦情報
		void simulationInit(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::KoyomiSiv3D& koyomi_siv
		) const {
			const std::string model_name =
				(simulation_model_index >= simulation_model_name.size()) ?
				"Sample" : simulation_model_name[simulation_model_index];

			simulator->init();
			koyomi_siv.steps.setDay(0); // ステップ数を 0 にする
			koyomi_siv.jdn.setDay(static_cast<double>(SimulationConstants::getInstance(model_name)->start_julian_day)); // シミュレーション初期時の日付に設定
			koyomi_siv.calcDate();
			koyomi_siv.is_agent_update = false;
			koyomi_siv.move_forward_in_time = false; // 一時停止
			koyomi_siv.go_back_in_time = false;
		}

		/// @brief シミュレーションのUI描画と操作処理
		/// @brief Simulation UI drawing and operation processing
		/// @param simulator シミュレータのユニークポインタ
		/// @param tm_ タッチマネージャー
		/// @param koyomi_siv 暦情報
		/// @param debug_start_y UIの開始Y座標
		void simulation(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::TouchStateManager& tm_,
			paxs::KoyomiSiv3D& koyomi_siv,
			int debug_start_y
		) {
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
				if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 360, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {

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
					simulationInit(simulator, koyomi_siv);
				}
			}
			// シミュレーションが初期化されている場合
			else {

				const auto* constants = SimulationConstants::getInstance(model_name);
				const int total_steps = constants->total_steps;

				// 規定ステップ数に達したかチェック
				if (total_steps > 0 && koyomi_siv.steps.cgetDay() >= total_steps) {
					// 残り実行回数を減らす
					m_remaining_iterations--;

					if (m_remaining_iterations > 0) {
						// まだ実行回数が残っている場合、シミュレーションを初期化して自動で再開
						simulationInit(simulator, koyomi_siv);
						koyomi_siv.is_agent_update = true;
						koyomi_siv.move_forward_in_time = true;
					}
					else {
						// 全ての実行が終了した場合、シミュレーションを停止
						koyomi_siv.is_agent_update = false;
						koyomi_siv.move_forward_in_time = false;
						m_remaining_iterations = 0; //念のため0にリセット
					}
				}

				// シミュレーションが再生されている場合
				if (koyomi_siv.is_agent_update) {
					// シミュレーションを停止
					texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
					if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 300, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						// if (s3d::SimpleGUI::Button(U"Sim Stop", s3d::Vec2{ 330, 60 })) {
						koyomi_siv.is_agent_update = false;

						koyomi_siv.move_forward_in_time = false; // 一時停止
						koyomi_siv.go_back_in_time = false;
					}
				}
				// シミュレーションが再生されていない場合
				else {
					// シミュレーション入力データを初期化
					texture_dictionary.at(MurMur3::calcHash("texture_reload")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60));
					if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						SimulationConstants::getInstance(model_name)->init(model_name);
					}
					// 人間データを初期化
					texture_dictionary.at(MurMur3::calcHash("texture_load_agent_data2")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y));
					if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 420, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						simulationInit(simulator, koyomi_siv);

						koyomi_siv.steps.setDay(0); // ステップ数を 0 にする
						koyomi_siv.calcDate();
					}
					// 地形データを削除
					texture_dictionary.at(MurMur3::calcHash("texture_delete_geographic_data")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
					if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 360, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						simulator.reset();

						koyomi_siv.steps.setDay(0); // ステップ数を 0 にする
						koyomi_siv.calcDate();
					}

					// シミュレーションを再生
					texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
					if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 300, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						// if (s3d::SimpleGUI::Button(U"Sim Start", s3d::Vec2{ 190, 60 })) {
						koyomi_siv.is_agent_update = true;

						// 実行回数をセット
						m_remaining_iterations = SimulationConstants::getInstance(model_name)->num_iterations;

						koyomi_siv.move_forward_in_time = true; // 再生
						koyomi_siv.go_back_in_time = false;
					}
					// シミュレーションを 1 Step 実行
					texture_dictionary.at(MurMur3::calcHash("texture_1step")).resizedDraw(
						time_icon_size, paxg::Vec2i(paxg::Window::width() - 240, debug_start_y));
					if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 240, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
						simulator->step(); // シミュレーションを 1 ステップ実行する
						koyomi_siv.steps.getDay()++; // ステップ数を増やす
						koyomi_siv.calcDate();

						koyomi_siv.move_forward_in_time = false; // 一時停止
						koyomi_siv.go_back_in_time = false;
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

		/// @brief シミュレーションビューアの初期化
		/// @brief Initialize the simulation viewer
		/// @param select_language 選択された言語
		/// @param simulation_text シミュレーション関連のテキスト
		/// @param language_fonts 言語フォント
		/// @param pulldown_font_size プルダウンのフォントサイズ
		/// @param pulldown_font_buffer_thickness_size プルダウンのフォント太さ
		/// @param path ルートパス
		void init(
			const SelectLanguage& select_language,
			const paxs::Language& simulation_text,
			LanguageFonts& language_fonts,
			int pulldown_font_size,
			int pulldown_font_buffer_thickness_size,
			const std::string& path
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
			simulation_pulldown = paxs::Pulldown(&select_language, &simulation_text, simulation_key, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), paxg::Vec2i{ 3000, 0 }, 0, false);
			simulation_pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().w() - 200), 600 });

			// 暦の時間操作のアイコン
			key_value_tsv.input(path + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ path + value_ }; });
		}

		/// @brief シミュレーションの更新と描画
		/// @brief Update and draw simulation
		/// @param simulator シミュレータのユニークポインタ
		/// @param tm_ タッチマネージャー
		/// @param koyomi_siv 暦情報
		/// @param debug_start_y UIの開始Y座標
		/// @param visible 可視性フラグ
		void update(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::TouchStateManager& tm_,
			paxs::KoyomiSiv3D& koyomi_siv,
			int debug_start_y,
			paxs::GraphicVisualizationList& visible
		) {
			// シミュレーションのボタン
			if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
				simulation(simulator, tm_, koyomi_siv, debug_start_y);
			}
		}

		/// @brief シミュレーションプルダウンの描画
		/// @brief Draw simulation pulldown
		/// @param simulator シミュレータのユニークポインタ
		/// @param visible 可視性フラグ
		void drawPulldown(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::GraphicVisualizationList& visible
		) {
			// シミュレーションのボタン
			if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
				if (simulator == nullptr) {
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
			paxs::GraphicVisualizationList& visible
		) {
			// シミュレーションのボタン
			if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
				if (simulator == nullptr) {
					paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(simulation_pulldown.getRect().h()) }.draw(paxg::Color{ 243, 243, 243 });
				}
			}
		}

		// ========================================
		// IViewerComponent インターフェース実装
		// ========================================

		/// @brief コンポーネント名を取得
		const char* getName() const override {
			return "SimulationViewer";
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

		// ========================================
		// IUIWidget インターフェース実装
		// ========================================

		/// @brief 外部参照を設定
		/// @param simulator シミュレータのユニークポインタへの参照
		/// @param tm タッチマネージャー
		/// @param koyomi_siv 暦情報
		/// @param visible_list 可視性リスト
		/// @param debug_start_y UIの開始Y座標
		void setReferences(
			std::unique_ptr<paxs::SettlementSimulator>& simulator,
			paxs::TouchStateManager& tm,
			paxs::KoyomiSiv3D& koyomi_siv,
			paxs::GraphicVisualizationList& visible_list,
			int debug_start_y
		) {
			simulator_ptr_ = &simulator;
			touch_manager_ = &tm;
			koyomi_siv_ = &koyomi_siv;
			visible_list_ = &visible_list;
			debug_start_y_ = debug_start_y;
		}

		/// @brief 更新処理（IUIWidget）
		void update(paxs::TouchStateManager& tm) override {
			if (!visible_ || !enabled_) return;
			if (!simulator_ptr_ || !koyomi_siv_ || !visible_list_) return;

			// 既存のupdate()を呼び出し
			update(*simulator_ptr_, tm, *koyomi_siv_, debug_start_y_, *visible_list_);

			// プルダウンの更新
			if (visible_list_->at(MurMur3::calcHash("Simulation")) &&
			    visible_list_->at(MurMur3::calcHash("UI")) &&
			    visible_list_->at(MurMur3::calcHash("Calendar"))) {
				if (*simulator_ptr_ == nullptr) {
					simulation_pulldown.update(tm);
					simulation_model_index = simulation_pulldown.getIndex();
				}
			}
		}

		/// @brief 描画処理（IUIWidget）
		void draw() override {
			if (!visible_) return;
			if (!simulator_ptr_ || !visible_list_) return;

			// プルダウンの描画のみ（背景はUIManagerで描画される）
			drawPulldown(*simulator_ptr_, *visible_list_);
		}

		/// @brief 矩形を取得
		paxg::Rect getRect() const override {
			// シミュレーションビューアの矩形を返す
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

#endif // !PAX_MAHOROBA_SIMULATION_SIMULATION_VIEWER_HPP
