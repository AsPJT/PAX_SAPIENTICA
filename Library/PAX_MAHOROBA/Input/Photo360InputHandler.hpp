/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INPUT_PHOTO_360_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_INPUT_PHOTO_360_INPUT_HANDLER_HPP

#include <algorithm>

#include <PAX_GRAPHICA/Key.hpp>

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Input/InputEvents.hpp>
#include <PAX_MAHOROBA/Rendering/Photo360Layer.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief Photo360入力ハンドラー
    /// @brief Photo360 input handler
    ///
    /// Photo360Layerのキーボード入力を処理します。
    /// Photo360が表示中（ViewMenu::view_3d）の場合のみ入力を処理し、
    /// 背後のレイヤー（Settlement、Map）への入力伝播をブロックします。
    /// Handles keyboard input for Photo360Layer.
    /// Only processes input when Photo360 is visible (ViewMenu::view_3d),
    /// blocking input propagation to background layers (Settlement, Map).
    class Photo360InputHandler : public IInputHandler {
    private:
        Photo360Layer& photo360_layer_;

        // Photo360の設定値（Photo360Config::Sphereから）
        static constexpr double move_speed_ = 1.2;     // 移動速度（度/フレーム）/ Move speed (degrees/frame)
        static constexpr double zoom_speed_ = 0.015;   // ズーム速度 / Zoom speed
        static constexpr double zoom_min_ = -17.0;     // ズーム最小値 / Zoom minimum
        static constexpr double zoom_max_ = -14.2;     // ズーム最大値 / Zoom maximum

    public:
        Photo360InputHandler(Photo360Layer& layer) : photo360_layer_(layer) {}

        /// @brief キーボードイベント処理
        /// @brief Handle keyboard event
        /// @param event キーボードイベント / Keyboard event
        /// @return イベント処理結果 / Event handling result
        ///
        /// Photo360が表示中の場合、以下のキー入力を処理します：
        /// - Arrow Keys / WASD: 視点回転 / View rotation
        /// - Q: ズームイン / Zoom in
        /// - E: ズームアウト / Zoom out
        EventHandlingResult handleEvent(const KeyboardEvent& event) override {
            (void)event; // KeyboardEventは現在使用していない（直接キー状態を取得）

            // Photo360が表示中のみ処理
            if (!isEnabled()) {
                return EventHandlingResult::NotHandled();
            }

            // 現在の視点回転角度とズーム位置を取得
            // Get current view rotation angles and zoom position
            double rotX = 0.0, rotY = 0.0, rotZ = 0.0;
            photo360_layer_.getViewRotation(rotX, rotY, rotZ);

            bool handled = false;

            // 視点回転: 左右（Arrow Keys + A/D）
            // View rotation: Left/Right (Arrow Keys + A/D)
            if (paxg::PAXG_KEY_LEFT.pressed() || paxg::PAXG_KEY_A.pressed()) {
                rotX += move_speed_;
                handled = true;
            }
            if (paxg::PAXG_KEY_RIGHT.pressed() || paxg::PAXG_KEY_D.pressed()) {
                rotX -= move_speed_;
                handled = true;
            }

            // 視点回転: 上下（Arrow Keys + W/S）
            // View rotation: Up/Down (Arrow Keys + W/S)
            if (paxg::PAXG_KEY_DOWN.pressed() || paxg::PAXG_KEY_S.pressed()) {
                rotY -= move_speed_;
                handled = true;
            }
            if (paxg::PAXG_KEY_UP.pressed() || paxg::PAXG_KEY_W.pressed()) {
                rotY += move_speed_;
                handled = true;
            }

            // ズーム操作: Q/E
            // Zoom operation: Q/E
            if (paxg::PAXG_KEY_Q.pressed()) {
                rotZ -= zoom_speed_;
                rotZ = (std::max)(rotZ, zoom_min_);
                handled = true;
            }
            if (paxg::PAXG_KEY_E.pressed()) {
                rotZ += zoom_speed_;
                rotZ = (std::min)(rotZ, zoom_max_);
                handled = true;
            }

            // 回転角度を 0-360 度の範囲に正規化
            // Normalize rotation angles to 0-360 degree range
            if (rotX < 0.0) rotX += 360.0;
            if (rotY < 0.0) rotY += 360.0;
            if (rotX >= 360.0) rotX -= 360.0;
            if (rotY >= 360.0) rotY -= 360.0;

            // 視点を更新
            // Update view rotation
            if (handled) {
                photo360_layer_.setViewRotation(rotX, rotY, rotZ);
                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::NotHandled();
        }

        /// @brief マウスイベント処理（未使用）
        /// @brief Handle mouse event (unused)
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief マウスホイールイベント処理（未使用）
        /// @brief Handle mouse wheel event (unused)
        EventHandlingResult handleEvent(const MouseWheelEvent& event) override {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief ヒットテスト（キーボード入力のため常にtrue）
        /// @brief Hit test (always true for keyboard input)
        bool isHit(int x, int y) const override {
            (void)x;
            (void)y;
            return true;
        }

        RenderLayer getLayer() const override { return RenderLayer::Photo360; }

        bool isEnabled() const {
            return photo360_layer_.isVisible();
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_INPUT_PHOTO_360_INPUT_HANDLER_HPP
