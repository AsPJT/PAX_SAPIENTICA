/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_LOADING_PROGRESS_BAR_HPP
#define PAX_MAHOROBA_UI_LOADING_PROGRESS_BAR_HPP

#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>
#include <PAX_SAPIENTICA/System/Async/LoadingHandle.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>

namespace paxs {

    /// @brief ロード進捗表示バー
    /// @brief Loading progress bar
    /// @tparam ResultT ロード結果の型 / Result type of loading operation
    template <class ResultT>
    class LoadingProgressBar : public IRenderable {
    private:
        const LoadingHandle<ResultT>* handle_ = nullptr;  ///< ロードハンドルへの参照 / Reference to loading handle
        int x_ = 0;                                       ///< X座標 / X coordinate
        int y_ = 0;                                       ///< Y座標 / Y coordinate
        int width_ = 400;                                 ///< 幅 / Width
        int height_ = 30;                                 ///< 高さ / Height
        paxg::Font* font_ = nullptr;                      ///< フォント / Font

        // 色設定 / Color settings
        paxg::Color background_color_{40, 40, 40, 255};   ///< 背景色 / Background color
        paxg::Color progress_color_{100, 200, 100, 255};  ///< 進捗バー色 / Progress bar color
        paxg::Color border_color_{200, 200, 200, 255};    ///< 枠線色 / Border color
        paxg::Color text_color_{0, 0, 0, 255};            ///< テキスト色 / Text color

        // イベント購読（RAII対応） / Event subscription (RAII-safe)
        ScopedSubscription window_resize_subscription_;   ///< ウィンドウリサイズイベント購読 / Window resize event subscription

        /// @brief ウィンドウサイズに基づいて位置を更新
        /// @brief Update position based on window size
        void updatePosition() {
            x_ = static_cast<int>(paxg::Window::width() / 2) - (width_ / 2);
            y_ = static_cast<int>(paxg::Window::height() / 2) - (height_ / 2);
        }

    public:
        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param handle ロードハンドル / Loading handle
        /// @param pos_x X座標 / X coordinate
        /// @param pos_y Y座標 / Y coordinate
        /// @param bar_width 幅 / Width
        /// @param bar_height 高さ / Height
        /// @param bar_font フォント / Font
        LoadingProgressBar(
            const LoadingHandle<ResultT>* handle,
            int pos_x, int pos_y,
            int bar_width = 400,
            int bar_height = 30,
            paxg::Font* bar_font = nullptr
        ) : handle_(handle), x_(pos_x), y_(pos_y), width_(bar_width), height_(bar_height), font_(bar_font),
            window_resize_subscription_(
                EventBus::getInstance().subscribeScoped<WindowResizedEvent>([this](const WindowResizedEvent&) {
                    updatePosition();
                })
            )
        {
            // 初期位置を設定 / Set initial position
            updatePosition();
        }

        /// @brief ロードハンドルを設定
        /// @brief Set loading handle
        /// @param handle ロードハンドル / Loading handle
        void setHandle(const LoadingHandle<ResultT>* handle) {
            handle_ = handle;
        }

        /// @brief 描画
        /// @brief Render
        void render() const override {
            if (handle_ == nullptr || !handle_->isValid()) {
                return;
            }

            const float progress = handle_->getProgress();
            const std::string message = handle_->getMessage();

            // 背景の描画 / Draw background
            paxg::Rect{static_cast<float>(x_), static_cast<float>(y_), static_cast<float>(width_), static_cast<float>(height_)}.draw(background_color_);

            // 進捗バーの描画 / Draw progress bar
            const int progress_width = static_cast<int>(width_ * progress);
            if (progress_width > 0) {
                paxg::Rect{static_cast<float>(x_), static_cast<float>(y_), static_cast<float>(progress_width), static_cast<float>(height_)}.draw(progress_color_);
            }

            // 枠線の描画 / Draw border
            paxg::Rect{static_cast<float>(x_), static_cast<float>(y_), static_cast<float>(width_), static_cast<float>(height_)}.drawFrame(0.0, 1.0, border_color_);

            // 進捗テキストの描画 / Draw progress text
            if (font_ != nullptr) {
                const int percent = static_cast<int>(progress * 100.0F);
                const std::string percent_text = std::to_string(percent) + "%";

                // パーセンテージを中央に描画 / Draw percentage in center
                const int text_x = x_ + (width_ / 2) - 20;
                const int text_y = y_ + (height_ / 2) - 10;
                font_->drawAt(percent_text, paxg::Vec2i{text_x, text_y}, text_color_);

                // メッセージをバーの下に描画 / Draw message below bar
                if (!message.empty()) {
                    const int msg_y = y_ + height_ + 10;
                    font_->drawAt(message, paxg::Vec2i{x_, msg_y}, text_color_);
                }
            }

            // エラー表示 / Display error
            if (handle_->hasError()) {
                if (font_ != nullptr) {
                    const std::string error_text = "Error occurred during loading";
                    const int error_y = y_ + height_ + 30;
                    font_->drawAt(error_text, paxg::Vec2i{x_, error_y}, paxg::Color{255, 50, 50, 255});
                }
            }
        }

        /// @brief 可視性を取得
        /// @brief Get visibility
        /// @return 常にtrue / Always true
        [[nodiscard]] bool isVisible() const override {
            return handle_ != nullptr && handle_->isValid() && !handle_->isFinished();
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        /// @return UIOverlay レイヤー / UIOverlay layer
        [[nodiscard]] RenderLayer getLayer() const override {
            return RenderLayer::UIOverlay;
        }

        // 色設定用メソッド / Color setting methods

        /// @brief 背景色を設定
        /// @brief Set background color
        void setBackgroundColor(const paxg::Color& color) { background_color_ = color; }

        /// @brief 進捗バー色を設定
        /// @brief Set progress bar color
        void setProgressColor(const paxg::Color& color) { progress_color_ = color; }

        /// @brief 枠線色を設定
        /// @brief Set border color
        void setBorderColor(const paxg::Color& color) { border_color_ = color; }

        /// @brief テキスト色を設定
        /// @brief Set text color
        void setTextColor(const paxg::Color& color) { text_color_ = color; }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_LOADING_PROGRESS_BAR_HPP
