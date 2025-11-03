/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_PANEL_HPP
#define PAX_MAHOROBA_CALENDAR_PANEL_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>

#include <PAX_MAHOROBA/UI/Calendar/CalendarWidget.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/Calendar/TimeControlWidget.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

	/// @brief カレンダーパネル - 時間操作とカレンダー表示を統合管理
	/// @brief Calendar Panel - Integrates time control and calendar display with shared background
	class CalendarPanel : public IWidget {
	public:
		CalendarPanel() = default;

		/// @brief 初期化
		/// @brief Initialize
		/// @param fonts LanguageFontsへの参照
		void init(paxs::LanguageFonts& fonts) {
			calendar_widget_.init(fonts);
		}

		/// @brief レイアウトとテクスチャの参照を設定
		/// @brief Set layout and texture references
		/// @param layout レイアウト情報
		/// @param texture_dict アイコンテクスチャ辞書
		void setLayout(const UILayout& layout, const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dict) {
			ui_layout_ = &layout;
			texture_dictionary_ = &texture_dict;
		}

		/// @brief カレンダー描画パラメータを設定
		/// @brief Set calendar rendering parameters
		void setCalendarParams(
			const paxs::Koyomi& koyomi,
			const SelectLanguage& select_language,
			const paxs::Language& language_text
		) {
			calendar_widget_.setRenderParams(koyomi, *ui_layout_, select_language, language_text);
		}

		/// @brief 時間操作ウィジェットの参照を設定
		/// @brief Set time control widget references
		void setTimeControlParams(paxs::Koyomi& koyomi) {
			if (!texture_dictionary_ || !ui_layout_) return;
			time_control_widget_.setReferences(*texture_dictionary_, koyomi);
			time_control_widget_.setPos(paxg::Vec2i{ui_layout_->time_control_base_x, ui_layout_->koyomi_font_y + ui_layout_->time_control_base_y});
		}

		/// @brief IWidget インターフェースの実装
		const char* getName() const override {
			return "CalendarPanel";
		}

		/// @brief レンダリングレイヤーを取得
		/// @brief Get rendering layer
		RenderLayer getLayer() const override {
			return RenderLayer::UIContent;
		}

		bool isAvailable() const override {
			return true;
		}

		bool handleInput(const InputEvent& event) override {
			if (!visible_ || !enabled_) return false;
			return time_control_widget_.handleInput(event);
		}

		void render() override {
			if (!visible_ || !ui_layout_) return;

			// コンポーネントを描画
			time_control_widget_.render();
			calendar_widget_.render();
		}

		/// @brief 時間操作ウィジェットとカレンダーウィジェットの可視性を設定
		/// @brief Set visibility of time control widget and calendar widget
		void setVisible(bool visible) override {
			visible_ = visible;
			time_control_widget_.setVisible(visible);
			calendar_widget_.setVisible(visible);
		}

		bool isVisible() const override {
			return visible_;
		}

		void setEnabled(bool enabled) override {
			enabled_ = enabled;
		}

		bool isEnabled() const override {
			return enabled_;
		}

		paxg::Rect getRect() const override {
			if (!ui_layout_) return paxg::Rect{0, 0, 0, 0};
			return paxg::Rect{
				static_cast<float>(ui_layout_->rect_start_x),
				static_cast<float>(ui_layout_->koyomi_font_y - 15),
				static_cast<float>(ui_layout_->rect_len_x),
				static_cast<float>(ui_layout_->next_rect_start_y)
			};
		}

		void setPos(const paxg::Vec2i& pos) override {
			pos_ = pos;
		}

		/// @brief 時間操作ウィジェットの高さを取得
		/// @brief Get height of time control widget
		int getTimeControlHeight() const {
			return time_control_widget_.getHeight();
		}

	private:
		// IWidget 状態
		bool visible_ = true;
		bool enabled_ = true;
		paxg::Vec2i pos_{0, 0};

		TimeControlWidget time_control_widget_;   // 時間操作ウィジェット
		CalendarWidget calendar_widget_;          // カレンダー表示ウィジェット

		const UILayout* ui_layout_ = nullptr;
		const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>* texture_dictionary_ = nullptr;
	};

} // namespace paxs

#endif // !PAX_MAHOROBA_CALENDAR_PANEL_HPP
