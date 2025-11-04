/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_VIEWPORT_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_MAP_VIEWPORT_INPUT_HANDLER_HPP

#include <algorithm>
#include <array>
#include <cmath>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/TouchInput.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Input/IEventHandler.hpp>
#include <PAX_MAHOROBA/Input/IMouseEventHandler.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

    /// @brief MapViewport の入力処理を担当するクラス（UI層）
    /// @brief Handles input processing for MapViewport (UI layer)
    ///
    /// IEventHandlerとIInputHandlerの両方を継承し、座標に依存しないイベント（キーボード、
    /// マウスホイール、リサイズ）と座標ベースのマウス入力の両方を処理します。
    /// 画面全体のパン・ズーム操作を担当するため、hitTest()は常にtrueを返します。
    /// Inherits both IEventHandler and IInputHandler to handle coordinate-independent events
    /// (keyboard, mouse wheel, resize) and coordinate-based mouse input.
    /// Handles pan/zoom for the entire screen, so hitTest() always returns true.
    class MapViewportInputHandler : public IEventHandler, public IMouseEventHandler {
    private:
        std::array<Key, 1> enl_keys; // 拡大キー
        std::array<Key, 1> esc_keys; // 縮小キー

        bool enabled_ = true; // 入力処理の有効/無効

#ifdef __ANDROID__
        int touch_num = 0;
        int old_touch_num = 0;
        std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> pos;
        std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> old_pos;
#endif

        /// @brief MapViewportへの参照（入力処理用）
        /// @brief Reference to MapViewport (for input processing)
        MapViewport* viewport_ = nullptr;

        /// @brief ドラッグ中フラグ（地図上でドラッグが開始されたか）
        bool is_dragging_ = false;

    public:
        MapViewportInputHandler()
            : enl_keys{Key(PAXG_KEY_Q)}, esc_keys{Key(PAXG_KEY_E)}
#ifdef __ANDROID__
            , touch_num(0), old_touch_num(0)
            , pos{paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}}
            , old_pos{paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}}
#endif
        {}

        /// @brief マウスホイールによるズーム処理
        /// @brief Handle zoom by mouse wheel
        /// @param viewport MapViewport参照 / MapViewport reference
        /// @param event マウスホイールイベント / Mouse wheel event
        void handleMouseWheelZoom(MapViewport& viewport, const MouseWheelEvent& event) {
            double height = viewport.getHeight();
            const double min_height = viewport.getMinHeight();
            const double max_height = viewport.getMaxHeight();

            height *= (1.0 + (event.wheel_rotation / MapViewportConstants::mouse_wheel_sensitivity));
            height = (std::clamp)(height, min_height, max_height);

            viewport.setHeight(height);
            viewport.setWidth(height / double(paxg::Window::height()) * double(paxg::Window::width()));
        }

        /// @brief マウスドラッグによる移動処理（デスクトップ）
        /// @brief Handle movement by mouse drag (desktop)
        /// @param viewport MapViewport参照 / MapViewport reference
        /// @param event マウスイベント / Mouse event
        void handleMouseDrag(MapViewport& viewport, const MouseEvent& event) {
#ifndef __ANDROID__
            // 左ボタンが押されている場合のみドラッグ処理
            // Only process drag if left button is pressed
            if (event.left_button_state == MouseButtonState::Held || event.left_button_state == MouseButtonState::Pressed) {
                const double height = viewport.getHeight();
                double center_x = viewport.getCenterX();
                double center_y = viewport.getCenterY();

                center_x += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(event.prev_x - event.x);
                center_y += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(event.y - event.prev_y);

                // 経度の範囲調整
                if (center_x < MapViewportConstants::longitude_min) {
                    center_x += MapViewportConstants::longitude_range;
                }
                if (center_x >= MapViewportConstants::longitude_max) {
                    center_x -= MapViewportConstants::longitude_range;
                }
                if (center_y < MapViewportConstants::longitude_min) {
                    center_y -= MapViewportConstants::longitude_max;
                }
                if (center_y > MapViewportConstants::longitude_max) {
                    center_y += MapViewportConstants::longitude_max;
                }

                viewport.setCenterX(center_x);
                viewport.setCenterY(center_y);
            }
#endif
        }

        /// @brief タッチ入力による移動・ズーム処理（Android）
        /// @brief Handle movement and zoom by touch input (Android)
        void handleTouchInput([[maybe_unused]] MapViewport& viewport) {
#ifdef __ANDROID__
            old_touch_num = touch_num;
            old_pos = pos;

            touch_num = paxg::TouchInput::getTouchCount();

            // タッチ位置の取得と視覚的フィードバック
            for (int i = 0; i < touch_num; i++) {
                if (i >= MapViewportConstants::max_touch_points) break;

                if (paxg::TouchInput::getTouchPosition(i, pos[i])) {
                    paxg::Circle(pos[i].x, pos[i].y, 40).draw(paxg::Color(230, 230, 240));
                }
            }

            // 1本指タッチ：移動
            if (old_touch_num == 1 && touch_num == 1) {
                const double height = viewport.getHeight();
                double center_x = viewport.getCenterX();
                double center_y = viewport.getCenterY();

                center_x += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(old_pos[0].x - pos[0].x);
                center_y += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(pos[0].y - old_pos[0].y);

                // 経度の範囲調整
                if (center_x < MapViewportConstants::longitude_min) {
                    center_x += MapViewportConstants::longitude_range;
                }
                if (center_x >= MapViewportConstants::longitude_max) {
                    center_x -= MapViewportConstants::longitude_range;
                }
                if (center_y < MapViewportConstants::longitude_min) {
                    center_y -= MapViewportConstants::longitude_max;
                }
                if (center_y > MapViewportConstants::longitude_max) {
                    center_y += MapViewportConstants::longitude_max;
                }

                viewport.setCenterX(center_x);
                viewport.setCenterY(center_y);
            }
            // 2本指タッチ：ピンチズーム
            else if (old_touch_num == 2 && touch_num == 2) {
                const int len = (pos[0].x - pos[1].x) * (pos[0].x - pos[1].x) + (pos[0].y - pos[1].y) * (pos[0].y - pos[1].y);
                const int old_len = (old_pos[0].x - old_pos[1].x) * (old_pos[0].x - old_pos[1].x) + (old_pos[0].y - old_pos[1].y) * (old_pos[0].y - old_pos[1].y);
                const int sub = std::abs(len - old_len);

                double height = viewport.getHeight();
                const double min_height = viewport.getMinHeight();
                const double max_height = viewport.getMaxHeight();
                const double expansion_size = viewport.getExpansionSize();

                if (len > old_len) {
                    // ズームイン
                    if (height > min_height) {
                        height -= ((height * (1.0 + (sub / MapViewportConstants::touch_zoom_divisor))) / expansion_size);
                        if (height < min_height) {
                            height = min_height;
                        }
                    }
                }
                else if (len < old_len) {
                    // ズームアウト
                    if (height < max_height) {
                        height += ((height * (1.0 + (sub / MapViewportConstants::touch_zoom_divisor))) / expansion_size);
                        if (height > max_height) {
                            height = max_height;
                        }
                    }
                }

                viewport.setHeight(height);
                viewport.setWidth(height / double(paxg::Window::height()) * double(paxg::Window::width()));
            }
#endif
        }

        /// @brief キーボードによるズーム処理（Q/Eキー）
        /// @brief Handle zoom by keyboard (Q/E keys)
        /// @note Public access for selective input processing
        void handleKeyboardZoom(MapViewport& viewport) {
            double height = viewport.getHeight();
            const double min_height = viewport.getMinHeight();
            const double max_height = viewport.getMaxHeight();
            const double expansion_size = viewport.getExpansionSize();

            // Q キー：ズームイン
            if (pressed(enl_keys)) {
                if (height > min_height) {
                    height -= (height / expansion_size);
                    if (height < min_height) {
                        height = min_height;
                    }
                }
                viewport.setHeight(height);
                viewport.setWidth(height / double(paxg::Window::height()) * double(paxg::Window::width()));
            }

            // E キー：ズームアウト
            if (pressed(esc_keys)) {
                if (height < max_height) {
                    height += (height / expansion_size);
                    if (height > max_height) {
                        height = max_height;
                    }
                }
                viewport.setHeight(height);
                viewport.setWidth(height / double(paxg::Window::height()) * double(paxg::Window::width()));
            }
        }

        /// @brief MapViewportへの参照を設定
        /// @brief Set reference to MapViewport
        /// @param viewport MapViewportへの参照 / Reference to MapViewport
        void setViewport(MapViewport* viewport) {
            viewport_ = viewport;
        }

        /// @brief ドラッグ中かどうかを取得
        /// @brief Get whether dragging is in progress
        /// @return ドラッグ中ならtrue / true if dragging
        bool isDragging() const {
            return is_dragging_;
        }

        // IInputHandler の実装
        // IInputHandler implementation

        /// @brief キーボードイベント処理
        /// @brief Handle keyboard event
        /// @param event キーボードイベント / Keyboard event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const KeyboardEvent& event) override {
            (void)event;
            if (!enabled_ || viewport_ == nullptr) {
                return EventHandlingResult::NotHandled();
            }

            // キーボード入力（Q/Eキーによるズーム）
            // Keyboard input (zoom with Q/E keys)
            handleKeyboardZoom(*viewport_);
            viewport_->applyConstraints();
            return EventHandlingResult::NotHandled(); // 他のハンドラーにも処理を継続
        }

        /// @brief マウスホイールイベント処理
        /// @brief Handle mouse wheel event
        /// @param event マウスホイールイベント / Mouse wheel event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const MouseWheelEvent& event) override {
            if (!enabled_ || viewport_ == nullptr) {
                return EventHandlingResult::NotHandled();
            }

            // マウスホイール入力（ズーム）
            // Mouse wheel input (zoom)
            handleMouseWheelZoom(*viewport_, event);
            viewport_->applyConstraints();
            return EventHandlingResult::NotHandled(); // 他のハンドラーにも処理を継続
        }

        /// @brief マウスイベント処理
        /// @brief Handle mouse event
        /// @param event マウスイベント / Mouse event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!enabled_ || viewport_ == nullptr) {
                return EventHandlingResult::NotHandled();
            }

            // マウス/タッチ入力（パンと移動）
            // Mouse/Touch input (pan and move)

            // ドラッグ開始判定：左ボタンが押された瞬間（Down時）
            if (event.left_button_state == MouseButtonState::Pressed) {
                is_dragging_ = true;
                // ドラッグフラグを立てて処理完了（UIには渡さない）
                return EventHandlingResult::Handled();
            }
            // ドラッグ中（Held状態）：ドラッグフラグONの時にドラッグ処理
            else if (event.left_button_state == MouseButtonState::Held && is_dragging_) {
                handleMouseDrag(*viewport_, event);
                // ドラッグキャプチャを要求（UIの上でもドラッグを継続）
                return EventHandlingResult::HandledWithCapture();
            }
            // ドラッグ終了判定：左ボタンが離された瞬間（Up時）
            else if (event.left_button_state == MouseButtonState::Released) {
                if (is_dragging_) {
                    // ドラッグフラグON + Up時：フラグを外して処理完了（UIには渡さない）
                    is_dragging_ = false;
                    return EventHandlingResult::Handled();
                }
                // ドラッグフラグOFFの場合は NotHandled でUIに処理させる
                return EventHandlingResult::NotHandled();
            }

            handleTouchInput(*viewport_);
            viewport_->applyConstraints();
            return EventHandlingResult::NotHandled(); // 他のハンドラーにも処理を継続
        }

        /// @brief リサイズイベント処理
        /// @brief Handle resize event
        /// @param event リサイズイベント / Resize event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const ResizeEvent& event) override {
            if (!enabled_ || viewport_ == nullptr) {
                return EventHandlingResult::NotHandled();
            }

            // ウィンドウリサイズイベント
            // Window resize event
            // MapViewportのサイズをウィンドウに合わせて調整
            // Adjust MapViewport size to match window
            int new_width = event.width;
            int new_height = event.height;
            int old_height = paxg::Window::height();

            if (old_height > 0 && new_height > 0) {
                viewport_->setWidth(viewport_->getHeight() / double(new_height) * double(new_width));
            }
            return EventHandlingResult::NotHandled(); // 他のハンドラーにも処理を継続
        }

        /// @brief ヒットテスト（画面全体を対象）
        /// @brief Hit test (targets entire screen)
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 常にtrue（画面全体が対象） / Always true (entire screen is target)
        bool hitTest(int /*x*/, int /*y*/) const override {
            // 画面全体が対象なので常にtrue
            return enabled_;
        }

        /// @brief レイヤーを取得
        /// @brief Get layer
        /// @return Backgroundレイヤー（最低優先度） / Background layer (lowest priority)
        RenderLayer getLayer() const override {
            // 最も低い優先度（UI、MapContentManagerの後）
            return RenderLayer::Background;
        }

        /// @brief 有効性を取得
        /// @brief Get enabled state
        bool isEnabled() const override {
            return enabled_;
        }

        /// @brief 有効性を設定
        /// @brief Set enabled state
        void setEnabled(bool enabled) {
            enabled_ = enabled;
        }
    };

}

#endif // !PAX_MAHOROBA_MAP_VIEWPORT_INPUT_HANDLER_HPP
