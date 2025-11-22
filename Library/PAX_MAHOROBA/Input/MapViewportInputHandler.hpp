/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_VIEWPORT_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_MAP_VIEWPORT_INPUT_HANDLER_HPP

#include <algorithm>
#include <array>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/TouchInput.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>

namespace paxs {

    /// @brief MapViewport の入力処理を担当するクラス（UI層）
    /// @brief Handles input processing for MapViewport (UI layer)
    ///
    /// 座標に依存しないイベント（キーボード、マウスホイール）と座標ベースのマウス入力を処理します。
    class MapViewportInputHandler : public IInputHandler {
    private:
        std::array<paxg::Key, 1> enl_keys; // 拡大キー
        std::array<paxg::Key, 1> esc_keys; // 縮小キー
        std::array<paxg::Key, 2> move_left_keys;  // 左移動キー (A, Left)
        std::array<paxg::Key, 2> move_right_keys; // 右移動キー (D, Right)
        std::array<paxg::Key, 2> move_up_keys;    // 上移動キー (W, Up)
        std::array<paxg::Key, 2> move_down_keys;  // 下移動キー (S, Down)

#ifdef __ANDROID__
        int touch_num = 0;
        int old_touch_num = 0;
        std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> pos;
        std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> old_pos;
#endif

        MapViewport& viewport_;

        /// @brief ドラッグ中フラグ（地図上でドラッグが開始されたか）
        bool is_dragging_ = false;

        /// @brief ドラッグイベント発行済みフラグ（ドラッグ開始イベントを既に発行したか）
        bool drag_event_fired_ = false;

        /// @brief マウスホイールによるズーム処理（マウスカーソル位置を中心に拡大縮小）
        bool handleMouseWheelZoom(const MouseWheelEvent& event) {
            if (event.wheel_rotation == 0) {
                return false; // ズーム変更なし
            }

            // ズーム前の状態を保存
            const Vector2<double> old_center = viewport_.getCenter();
            const Vector2<double> old_size = viewport_.getSize();

            // 新しい高さを計算
            const double min_height = viewport_.getMinHeight();
            const double max_height = viewport_.getMaxHeight();
            double new_height = old_size.y * (1.0 + (event.wheel_rotation / MapViewportConstants::mouse_wheel_sensitivity));
            new_height = (std::clamp)(new_height, min_height, max_height);

            // ウィンドウサイズ
            const Vector2<double> window_size(
                static_cast<double>(paxg::Window::width()),
                static_cast<double>(paxg::Window::height())
            );

            // マウス位置を正規化座標に変換（-0.5 ~ 0.5）
            const Vector2<double> normalized_mouse(
                (static_cast<double>(event.mouse_pos.x) / window_size.x) - 0.5,
                0.5 - (static_cast<double>(event.mouse_pos.y) / window_size.y)
            );

            // マウス位置のワールド座標
            const Vector2<double> mouse_world(
                old_center.x + normalized_mouse.x * old_size.x,
                old_center.y + normalized_mouse.y * old_size.y
            );

            // 新しいサイズを設定
            viewport_.setSize(new_height);

            // 新しいサイズを取得
            const Vector2<double> new_size = viewport_.getSize();

            // ズーム後もマウスカーソル位置が同じワールド座標を指すように中心を調整
            Vector2<double> new_center(
                mouse_world.x - normalized_mouse.x * new_size.x,
                mouse_world.y - normalized_mouse.y * new_size.y
            );

            // 経度の範囲調整
            if (new_center.x < MapViewportConstants::longitude_min) {
                new_center.x += MapViewportConstants::longitude_range;
            }
            if (new_center.x >= MapViewportConstants::longitude_max) {
                new_center.x -= MapViewportConstants::longitude_range;
            }
            if (new_center.y < MapViewportConstants::longitude_min) {
                new_center.y += MapViewportConstants::longitude_range;
            }
            if (new_center.y >= MapViewportConstants::longitude_max) {
                new_center.y -= MapViewportConstants::longitude_range;
            }

            // 新しい中心座標を設定
            viewport_.setCenter(new_center);

            return true;
        }

        /// @brief マウスドラッグによる移動処理（デスクトップ）
        void handleMouseDrag(const MouseEvent& event) {
#ifndef __ANDROID__
            // 左ボタンが押されている場合のみドラッグ処理
            // Only process drag if left button is pressed
            if (event.left_button_state == MouseButtonState::Held || event.left_button_state == MouseButtonState::Pressed) {
                const double height = viewport_.getSize().y;

                Vector2<double> center = viewport_.getCenter();

                center.x += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(event.prev_pos.x - event.pos.x);
                center.y += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(event.pos.y - event.prev_pos.y);

                // 経度の範囲調整
                if (center.x < MapViewportConstants::longitude_min) {
                    center.x += MapViewportConstants::longitude_range;
                }
                if (center.x >= MapViewportConstants::longitude_max) {
                    center.x -= MapViewportConstants::longitude_range;
                }
                if (center.y < MapViewportConstants::longitude_min) {
                    center.y -= MapViewportConstants::longitude_max;
                }
                if (center.y > MapViewportConstants::longitude_max) {
                    center.y += MapViewportConstants::longitude_max;
                }

                // X座標とY座標を同時に設定
                viewport_.setCenter(center);
            }
#endif
        }

        /// @brief タッチ入力による移動・ズーム処理（Android）
        /// @brief Handle movement and zoom by touch input (Android)
        void handleTouchInput() {
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
                const double height = viewport_.getHeight();
                Vector2<double> center = viewport_.getCenter();

                center.x += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(old_pos[0].x - pos[0].x);
                center.y += height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(pos[0].y - old_pos[0].y);

                // 経度の範囲調整
                if (center.x < MapViewportConstants::longitude_min) {
                    center.x += MapViewportConstants::longitude_range;
                }
                if (center.x >= MapViewportConstants::longitude_max) {
                    center.x -= MapViewportConstants::longitude_range;
                }
                if (center.y < MapViewportConstants::longitude_min) {
                    center.y -= MapViewportConstants::longitude_max;
                }
                if (center.y > MapViewportConstants::longitude_max) {
                    center.y += MapViewportConstants::longitude_max;
                }

                // X座標とY座標を同時に設定（イベント通知は1回のみ）
                viewport_.setCenter(center);
            }
            // 2本指タッチ：ピンチズーム
            else if (old_touch_num == 2 && touch_num == 2) {
                const int len = (pos[0].x - pos[1].x) * (pos[0].x - pos[1].x) + (pos[0].y - pos[1].y) * (pos[0].y - pos[1].y);
                const int old_len = (old_pos[0].x - old_pos[1].x) * (old_pos[0].x - old_pos[1].x) + (old_pos[0].y - old_pos[1].y) * (old_pos[0].y - old_pos[1].y);
                const int sub = std::abs(len - old_len);

                double height = viewport_.getHeight();
                const double min_height = viewport_.getMinHeight();
                const double max_height = viewport_.getMaxHeight();
                const double expansion_size = viewport_.getExpansionSize();

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

                viewport_.setHeight(height);
                viewport_.setWidth(height / double(paxg::Window::height()) * double(paxg::Window::width()));
            }
#endif
        }

        /// @brief キーボードによるズーム処理（Q/Eキー）
        bool handleKeyboardZoom() {
            const bool zoom_in = pressed(enl_keys);   // Q キー：ズームイン
            const bool zoom_out = pressed(esc_keys);  // E キー：ズームアウト

            // 同時押し（両方true）または両方押されていない（両方false）場合は何もしない
            if (zoom_in == zoom_out) {
                return false;
            }

            double height = viewport_.getHeight();
            const double min_height = viewport_.getMinHeight();
            const double max_height = viewport_.getMaxHeight();
            const double expansion_size = viewport_.getExpansionSize();
            bool changed = false;

            if (zoom_in) {
                if (height > min_height) {
                    height -= (height / expansion_size);
                    height = (std::max)(height, min_height);
                    changed = true;
                }
            } else {
                if (height < max_height) {
                    height += (height / expansion_size);
                    height = (std::min)(height, max_height);
                    changed = true;
                }
            }

            // 変更があった場合のみsetSizeを一度だけ呼ぶ
            if (changed) {
                viewport_.setSize(height);
            }

            return changed;
        }

        /// @brief キーボードによる移動処理（WASD/矢印キー）
        bool handleKeyboardMovement() {
            const bool move_left = pressed(move_left_keys);   // A/Left キー：左移動
            const bool move_right = pressed(move_right_keys); // D/Right キー：右移動
            const bool move_down = pressed(move_down_keys);   // S/Down キー：下移動
            const bool move_up = pressed(move_up_keys);       // W/Up キー：上移動

            // 反対方向の同時押しをチェック（X軸・Y軸それぞれ）
            const bool horizontal_cancel = (move_left == move_right);
            const bool vertical_cancel = (move_down == move_up);

            // 両方向でキャンセルされている場合は何もしない
            if (horizontal_cancel && vertical_cancel) {
                return false;
            }

            Vector2<double> center = viewport_.getCenter();
            const double width = viewport_.getSize().x;
            const double movement_size = MapViewportConstants::default_movement_size;
            bool changed = false;

            // X軸の移動（左右が同時押しでない場合のみ）
            if (!horizontal_cancel) {
                if (move_left) {
                    center.x -= (width / movement_size);
                    if (center.x < MapViewportConstants::longitude_min) {
                        center.x += MapViewportConstants::longitude_range;
                    }
                    changed = true;
                } else {
                    center.x += (width / movement_size);
                    if (center.x >= MapViewportConstants::longitude_max) {
                        center.x -= MapViewportConstants::longitude_range;
                    }
                    changed = true;
                }
            }

            // Y軸の移動（上下が同時押しでない場合のみ）
            if (!vertical_cancel) {
                if (move_down) {
                    center.y -= (width / movement_size);
                    if (center.y < MapViewportConstants::longitude_min) {
                        center.y += MapViewportConstants::longitude_range;
                    }
                    changed = true;
                } else {
                    center.y += (width / movement_size);
                    if (center.y >= MapViewportConstants::longitude_max) {
                        center.y -= MapViewportConstants::longitude_range;
                    }
                    changed = true;
                }
            }

            // 座標が変更された場合はビューポートに設定
            if (changed) {
                viewport_.setCenter(center);
            }

            return changed;
        }

    public:
        MapViewportInputHandler(MapViewport& viewport)
            : viewport_(viewport),
              enl_keys{paxg::Key(paxg::PAXG_KEY_Q)}, esc_keys{paxg::Key(paxg::PAXG_KEY_E)},
              move_left_keys{paxg::Key(paxg::PAXG_KEY_A), paxg::Key(paxg::PAXG_KEY_LEFT)},
              move_right_keys{paxg::Key(paxg::PAXG_KEY_D), paxg::Key(paxg::PAXG_KEY_RIGHT)},
              move_up_keys{paxg::Key(paxg::PAXG_KEY_W), paxg::Key(paxg::PAXG_KEY_UP)},
              move_down_keys{paxg::Key(paxg::PAXG_KEY_S), paxg::Key(paxg::PAXG_KEY_DOWN)}
#ifdef __ANDROID__
            , touch_num(0), old_touch_num(0)
            , pos{paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}}
            , old_pos{paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}, paxs::Vector2<int>{0,0}}
#endif
        {}

        /// @brief キーボードイベント処理
        /// @brief Handle keyboard event
        /// @param event キーボードイベント / Keyboard event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const KeyboardEvent& event) override {
            (void)event;
            // キーボード入力（Q/Eキーによるズーム）
            bool zoom_changed = handleKeyboardZoom();

            // キーボード入力（WASD/矢印キーによる移動）
            bool movement_changed = handleKeyboardMovement();

            // ズームまたは移動で変更があった場合、境界制約を適用
            if (zoom_changed || movement_changed) {
                viewport_.applyConstraints();
                viewport_.notifyViewportChanged();
            }

            return EventHandlingResult::NotHandled(); // 他のハンドラーにも処理を継続
        }

        /// @brief マウスホイールイベント処理
        /// @brief Handle mouse wheel event
        /// @param event マウスホイールイベント / Mouse wheel event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const MouseWheelEvent& event) override {
            // マウスホイール入力（ズーム）
            bool changed = handleMouseWheelZoom(event);
            if (changed) {
                viewport_.applyConstraints();
                viewport_.notifyViewportChanged();
            }
            return EventHandlingResult::NotHandled(); // 他のハンドラーにも処理を継続
        }

        /// @brief マウスイベント処理
        /// @brief Handle mouse event
        /// @param event マウスイベント / Mouse event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const MouseEvent& event) override {
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
                // 実際にマウスが動いた時のみイベントを発行（初回のみ）
                if (!drag_event_fired_ && (event.pos.x != event.prev_pos.x || event.pos.y != event.prev_pos.y)) {
                    drag_event_fired_ = true;
                    EventBus::getInstance().publish(MapViewportDragStartedEvent());
                }

                handleMouseDrag(event);
                viewport_.applyConstraints();
                viewport_.notifyViewportChanged();
                // ドラッグキャプチャを要求（UIの上でもドラッグを継続）
                return EventHandlingResult::HandledWithCapture();
            }
            // ドラッグ終了判定：左ボタンが離された瞬間（Up時）
            else if (event.left_button_state == MouseButtonState::Released) {
                if (is_dragging_) {
                    // ドラッグフラグON + Up時：フラグを外して処理完了（UIには渡さない）
                    is_dragging_ = false;
                    drag_event_fired_ = false;  // イベント発行フラグもリセット
                    return EventHandlingResult::Handled();
                }
                // ドラッグフラグOFFの場合は NotHandled でUIに処理させる
                return EventHandlingResult::NotHandled();
            }

            handleTouchInput();
            viewport_.applyConstraints();
            viewport_.notifyViewportChanged();
            return EventHandlingResult::NotHandled(); // 他のハンドラーにも処理を継続
        }

        bool isHit(const paxs::Vector2<int>&) const override {
            // 画面全体が対象なので常にtrue
            return true;
        }
        RenderLayer getLayer() const override { return RenderLayer::Background; }
    };

}

#endif // !PAX_MAHOROBA_MAP_VIEWPORT_INPUT_HANDLER_HPP
