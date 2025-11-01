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
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>

#include <PAX_MAHOROBA/UI/Calendar/CalendarRenderer.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/Calendar/TimeControlWidget.hpp>
#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

	/// @brief カレンダーパネル - 時間操作とカレンダー表示を統合管理
	/// @brief Calendar Panel - Integrates time control and calendar display with shared background
	class CalendarPanel : public IUIWidget {
	public:
		CalendarPanel() = default;

		/// @brief 初期化
		/// @brief Initialize
		/// @param fonts LanguageFontsへの参照
		void init(paxs::LanguageFonts& fonts) {
			calendar_renderer_.init(fonts);
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
			const paxs::Language& language_text,
			bool is_simulator_active
		) {
			calendar_renderer_.setRenderParams(koyomi, *ui_layout_, select_language, language_text, is_simulator_active);
		}

		/// @brief 時間操作ウィジェットの参照を設定
		/// @brief Set time control widget references
		void setTimeControlParams(paxs::Koyomi& koyomi) {
			if (!texture_dictionary_ || !ui_layout_) return;
			time_control_widget_.setReferences(*texture_dictionary_, koyomi);
			time_control_widget_.setPos(paxg::Vec2i{ui_layout_->time_control_base_x, ui_layout_->koyomi_font_y + ui_layout_->time_control_base_y});
		}

		/// @brief 影用のテクスチャを設定
		/// @brief Set textures for shadow rendering
		void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
			shadow_texture_ = &shadow_tex;
			internal_texture_ = &internal_tex;
		}

		/// @brief IUIWidget インターフェースの実装
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

			// 背景パネルを描画
			drawBackground();

			// コンポーネントを描画
			time_control_widget_.render();
			calendar_renderer_.render();
		}

		/// @brief 時間操作ウィジェットとカレンダーレンダラーの可視性を設定
		/// @brief Set visibility of time control widget and calendar renderer
		void setVisible(bool visible) override {
			visible_ = visible;
			time_control_widget_.setVisible(visible);
			calendar_renderer_.setVisible(visible);
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
		// IUIWidget 状態
		bool visible_ = true;
		bool enabled_ = true;
		paxg::Vec2i pos_{0, 0};

		TimeControlWidget time_control_widget_;   // 時間操作ウィジェット
		CalendarRenderer calendar_renderer_;      // カレンダー表示

		const UILayout* ui_layout_ = nullptr;
		const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>* texture_dictionary_ = nullptr;

		// 影描画用テクスチャ（外部から注入）
		paxg::RenderTexture* shadow_texture_ = nullptr;
		paxg::RenderTexture* internal_texture_ = nullptr;

		/// @brief 背景パネルを描画
		/// @brief Draw background panel
		void drawBackground() {
#ifdef PAXS_USING_SIV3D
			// Siv3D: Use high-quality shadow renderer with Gaussian blur
			if (shadow_texture_ && internal_texture_) {
				paxs::ShadowRenderer::renderShadowWithPanels(
					*shadow_texture_,
					*internal_texture_,
					[this]() {
						// 影の形状を描画
						paxg::RoundRect{ ui_layout_->rect_start_x, ui_layout_->koyomi_font_y - 15,
										 ui_layout_->rect_len_x, ui_layout_->next_rect_start_y, 10 }.draw();
					},
					[this]() {
						// パネル本体を描画
						paxg::RoundRect{ ui_layout_->rect_start_x, ui_layout_->koyomi_font_y - 15,
										 ui_layout_->rect_len_x, ui_layout_->next_rect_start_y, 10 }.draw(paxg::Color{255, 255, 255});
					}
				);
			}
#else
			// SFML/DxLib: Use simple shadow with drawShadow method
			paxg::RoundRect{ ui_layout_->rect_start_x, ui_layout_->koyomi_font_y - 15,
							 ui_layout_->rect_len_x, ui_layout_->next_rect_start_y, 10 }
				.drawShadow({1, 1}, 4, 1).draw(paxg::Color{255, 255, 255});
#endif
		}
	};

} // namespace paxs

#endif // !PAX_MAHOROBA_CALENDAR_PANEL_HPP
