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

	/// @brief 入力処理可能オブジェクトの基底インターフェース
	/// @brief Base interface for input-handling objects
	///
	/// このインターフェースを実装することで、オブジェクトはInputRouterによって
	/// Z順序の逆順（前面→背面）で入力イベントを受け取るようになります。
	/// By implementing this interface, objects receive input events from InputRouter
	/// in reverse Z-order (foreground → background).
	class IInputHandler {
	public:
		/// @brief 仮想デストラクタ
		/// @brief Virtual destructor
		virtual ~IInputHandler() = default;

		/// @brief 入力イベントを処理する
		/// @brief Handle input event
		/// @param event 入力イベント / Input event
		/// @return イベントを処理した場合true（伝播を停止）、処理しなかった場合false（伝播を継続）
		/// @return true if event was handled (stop propagation), false to continue propagation
		virtual bool handleInput(const InputEvent& event) = 0;

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
