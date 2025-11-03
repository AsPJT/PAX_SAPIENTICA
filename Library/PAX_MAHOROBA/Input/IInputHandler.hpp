/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_I_INPUT_HANDLER_HPP

#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {

    /// @brief 入力イベントの種類
    /// @brief Input event type
    enum class InputEventType {
        Mouse,         ///< マウス/タッチ入力 / Mouse/Touch input
        Keyboard,      ///< キーボード入力 / Keyboard input
        MouseWheel,    ///< マウスホイール入力 / Mouse wheel input
        WindowResize,  ///< ウィンドウリサイズ / Window resize
        WindowFocus    ///< ウィンドウフォーカス / Window focus
    };

    /// @brief マウスボタンの状態変化
    enum class MouseButtonState {
        None,       ///< 状態変化なし / No change
        Pressed,    ///< 押された瞬間 / Just pressed
        Released,   ///< 離された瞬間 / Just released
        Held        ///< 押され続けている / Held down
    };

    /// @brief 入力イベントの構造体
    /// @brief Input event structure
    struct InputEvent {
        /// @brief 入力イベントの種類
        /// @brief Input event type
        InputEventType type = InputEventType::Mouse;

        /// @brief 入力状態マネージャー（既存のシステムとの互換性のため）
        /// @brief Input state manager (for compatibility with existing system)
        InputStateManager* input_state_manager = nullptr;

        /// @brief マウス/タッチのX座標
        /// @brief Mouse/Touch X coordinate
        int x = 0;

        /// @brief マウス/タッチのY座標
        /// @brief Mouse/Touch Y coordinate
        int y = 0;

        /// @brief ウィンドウの幅（WindowResizeイベント用）
        /// @brief Window width (for WindowResize event)
        int window_width = 0;

        /// @brief ウィンドウの高さ（WindowResizeイベント用）
        /// @brief Window height (for WindowResize event)
        int window_height = 0;

        /// @brief フォーカス状態（WindowFocusイベント用、true=フォーカス取得、false=フォーカス喪失）
        /// @brief Focus state (for WindowFocus event, true=gained focus, false=lost focus)
        bool has_focus = true;

        /// @brief マウスボタン情報（ビットフラグ: 左=0x01, 右=0x02, 中=0x04）
        /// @brief Mouse button information (bit flags: left=0x01, right=0x02, middle=0x04)
        unsigned char mouse_buttons = 0;

        /// @brief 修飾キー情報（ビットフラグ: Shift=0x01, Ctrl=0x02, Alt=0x04, Command=0x08）
        /// @brief Modifier key information (bit flags: Shift=0x01, Ctrl=0x02, Alt=0x04, Command=0x08)
        unsigned char modifier_keys = 0;

        /// @brief マウスホイールの回転量（MouseWheelイベント用）
        /// @brief Mouse wheel rotation volume (for MouseWheel event)
        int wheel_rotation = 0;

        /// @brief 前フレームのX座標（ドラッグ処理用）
        /// @brief Previous frame X coordinate (for drag processing)
        int prev_x = 0;

        /// @brief 前フレームのY座標（ドラッグ処理用）
        /// @brief Previous frame Y coordinate (for drag processing)
        int prev_y = 0;

        /// @brief 左マウスボタンの状態
        MouseButtonState left_button_state = MouseButtonState::None;

        /// @brief 右マウスボタンの状態
        MouseButtonState right_button_state = MouseButtonState::None;

        /// @brief 中マウスボタンの状態
        MouseButtonState middle_button_state = MouseButtonState::None;

        // マウスボタンのビットフラグ定数
        // Mouse button bit flag constants
        static constexpr unsigned char MOUSE_BUTTON_LEFT = 0x01;
        static constexpr unsigned char MOUSE_BUTTON_RIGHT = 0x02;
        static constexpr unsigned char MOUSE_BUTTON_MIDDLE = 0x04;

        // 修飾キーのビットフラグ定数
        // Modifier key bit flag constants
        static constexpr unsigned char MODIFIER_SHIFT = 0x01;
        static constexpr unsigned char MODIFIER_CTRL = 0x02;
        static constexpr unsigned char MODIFIER_ALT = 0x04;
        static constexpr unsigned char MODIFIER_COMMAND = 0x08;  // macOS用 / For macOS

        /// @brief 左マウスボタンが押されているかチェック
        /// @brief Check if left mouse button is pressed
        bool isLeftButtonPressed() const {
            return (mouse_buttons & MOUSE_BUTTON_LEFT) != 0;
        }

        /// @brief 右マウスボタンが押されているかチェック
        /// @brief Check if right mouse button is pressed
        bool isRightButtonPressed() const {
            return (mouse_buttons & MOUSE_BUTTON_RIGHT) != 0;
        }

        /// @brief 中マウスボタンが押されているかチェック
        /// @brief Check if middle mouse button is pressed
        bool isMiddleButtonPressed() const {
            return (mouse_buttons & MOUSE_BUTTON_MIDDLE) != 0;
        }

        /// @brief Shiftキーが押されているかチェック
        /// @brief Check if Shift key is pressed
        bool isShiftPressed() const {
            return (modifier_keys & MODIFIER_SHIFT) != 0;
        }

        /// @brief Ctrlキーが押されているかチェック
        /// @brief Check if Ctrl key is pressed
        bool isCtrlPressed() const {
            return (modifier_keys & MODIFIER_CTRL) != 0;
        }

        /// @brief Altキーが押されているかチェック
        /// @brief Check if Alt key is pressed
        bool isAltPressed() const {
            return (modifier_keys & MODIFIER_ALT) != 0;
        }

        /// @brief Commandキー（macOS）が押されているかチェック
        /// @brief Check if Command key (macOS) is pressed
        bool isCommandPressed() const {
            return (modifier_keys & MODIFIER_COMMAND) != 0;
        }

        /// @brief デフォルトコンストラクタ
        /// @brief Default constructor
        InputEvent() = default;

        /// @brief コンストラクタ（マウス/タッチ入力用）
        /// @brief Constructor (for Mouse/Touch input)
        /// @param manager 入力状態マネージャー / Input state manager
        /// @param x_ X座標 / X coordinate
        /// @param y_ Y座標 / Y coordinate
        InputEvent(InputStateManager* manager, int x_, int y_)
            : type(InputEventType::Mouse), input_state_manager(manager), x(x_), y(y_) {}

        /// @brief コンストラクタ（キーボード/マウスホイール入力用）
        /// @brief Constructor (for Keyboard/MouseWheel input)
        /// @param event_type 入力イベントの種類 / Input event type
        /// @param manager 入力状態マネージャー / Input state manager
        InputEvent(InputEventType event_type, InputStateManager* manager)
            : type(event_type), input_state_manager(manager), x(0), y(0) {}

        /// @brief コンストラクタ（ウィンドウリサイズイベント用）
        /// @brief Constructor (for WindowResize event)
        /// @param width ウィンドウの幅 / Window width
        /// @param height ウィンドウの高さ / Window height
        InputEvent(int width, int height)
            : type(InputEventType::WindowResize), input_state_manager(nullptr),
              x(0), y(0), window_width(width), window_height(height) {}

        /// @brief コンストラクタ（ウィンドウフォーカスイベント用）
        /// @brief Constructor (for WindowFocus event)
        /// @param focus フォーカス状態 / Focus state
        static InputEvent createFocusEvent(bool focus) {
            InputEvent event;
            event.type = InputEventType::WindowFocus;
            event.has_focus = focus;
            return event;
        }
    };

    /// @brief 入力処理結果
    /// @brief Input handling result
    struct InputHandlingResult {
        /// @brief イベントが処理されたか（伝播を停止するか）
        bool handled = false;

        /// @brief ドラッグキャプチャを要求するか
        bool request_drag_capture = false;

        /// @brief デフォルトコンストラクタ（処理しない）
        InputHandlingResult() = default;

        /// @param handled_ イベントが処理されたか / Whether event was handled
        /// @param request_capture_ ドラッグキャプチャを要求するか / Whether to request drag capture
        InputHandlingResult(bool handled_, bool request_capture_ = false)
            : handled(handled_), request_drag_capture(request_capture_) {}

        /// @brief 処理済み（伝播停止）を返す
        /// @brief Return handled (stop propagation)
        static InputHandlingResult Handled() {
            return InputHandlingResult(true, false);
        }

        /// @brief 処理済み＋ドラッグキャプチャ要求を返す
        static InputHandlingResult HandledWithCapture() {
            return InputHandlingResult(true, true);
        }

        /// @brief 未処理（伝播継続）を返す
        static InputHandlingResult NotHandled() {
            return InputHandlingResult(false, false);
        }
    };

    /// @brief 入力処理可能オブジェクトの基底インターフェース
    /// @brief Base interface for input-handling objects
    ///
    /// このインターフェースを実装することで、オブジェクトはInputRouterによって
    /// Z順序の逆順（前面→背面）で入力イベントを受け取るようになります。
    class IInputHandler {
    public:
        /// @brief 仮想デストラクタ
        /// @brief Virtual destructor
        virtual ~IInputHandler() = default;

        /// @brief 入力イベントを処理する
        /// @brief Handle input event
        /// @param event 入力イベント / Input event
        /// @return 入力処理結果（処理の有無とドラッグキャプチャ要求）
        /// @return Input handling result (handled status and drag capture request)
        virtual InputHandlingResult handleInput(const InputEvent& event) = 0;

        /// @brief 指定座標がこのオブジェクトの範囲内かどうかをチェック（ヒットテスト）
        /// @brief Check if the specified coordinates are within this object's bounds (hit test)
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 範囲内ならtrue / true if within bounds
        virtual bool hitTest(int x, int y) const = 0;

        /// @brief このオブジェクトが属する入力処理レイヤーを取得
        /// @brief Get the input handling layer this object belongs to
        /// @return レンダリングレイヤー（入力優先度として使用）/ Rendering layer (used as input priority)
        virtual RenderLayer getLayer() const = 0;

        /// @brief オブジェクトが入力処理可能かどうかを取得
        /// @brief Check if the object can handle input
        /// @return 入力処理可能ならtrue / true if input handling is enabled
        virtual bool isEnabled() const = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_I_INPUT_HANDLER_HPP
