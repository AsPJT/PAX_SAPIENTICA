/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INPUT_ROUTER_HPP
#define PAX_MAHOROBA_INPUT_ROUTER_HPP

#include <algorithm>
#include <vector>

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

	/// @brief 入力ルーター
	/// @brief Input router
	///
	/// IInputHandler オブジェクトを登録し、Z順序の逆順（前面→背面）で入力イベントをルーティングします。
	/// UIが地図よりも優先して入力を受け取り、処理済みの入力は背面に伝播しません。
	/// Registers IInputHandler objects and routes input events in reverse Z-order (foreground → background).
	/// UI receives input before the map, and processed input does not propagate to the background.
	class InputRouter {
	private:
		/// @brief 登録された入力ハンドラーのリスト
		/// @brief List of registered input handlers
		std::vector<IInputHandler*> handlers_;

		/// @brief ソート済みフラグ（最適化用）
		/// @brief Sorted flag (for optimization)
		bool is_sorted_ = false;

	public:
		/// @brief デフォルトコンストラクタ
		/// @brief Default constructor
		InputRouter() = default;

		/// @brief 入力ハンドラーを登録
		/// @brief Register an input handler
		/// @param handler 登録するハンドラー / Handler to register
		void registerHandler(IInputHandler* handler) {
			if (handler == nullptr) return;
			handlers_.push_back(handler);
			is_sorted_ = false;  // 新規登録があったらソートフラグをリセット
		}

		/// @brief 入力ハンドラーの登録を解除
		/// @brief Unregister an input handler
		/// @param handler 登録解除するハンドラー / Handler to unregister
		void unregisterHandler(IInputHandler* handler) {
			auto it = std::find(handlers_.begin(), handlers_.end(), handler);
			if (it != handlers_.end()) {
				handlers_.erase(it);
				// ソートフラグはそのまま（削除してもソート順は維持される）
			}
		}

		/// @brief すべての登録を解除
		/// @brief Unregister all handlers
		void clear() {
			handlers_.clear();
			is_sorted_ = false;
		}

		/// @brief 登録されているハンドラー数を取得
		/// @brief Get the number of registered handlers
		/// @return ハンドラー数 / Number of handlers
		std::size_t size() const {
			return handlers_.size();
		}

		/// @brief 優先度（レイヤーの逆順）でソート
		/// @brief Sort in priority order (reverse layer order)
		///
		/// レイヤー値が大きい順（前面→背面）にソートします。
		/// Sorts in descending layer value order (foreground → background).
		void sort() {
			std::stable_sort(handlers_.begin(), handlers_.end(),
				[](const IInputHandler* a, const IInputHandler* b) {
					// 逆順（前面が先）
					return a->getLayer() > b->getLayer();
				});
			is_sorted_ = true;
		}

		/// @brief 入力イベントをルーティング
		/// @brief Route input event
		/// @param event 入力イベント / Input event
		/// @return イベントが処理された場合true / true if event was handled
		///
		/// 前面から順に各ハンドラーにイベントを渡し、処理されたら伝播を停止します。
		/// Passes the event to each handler from front to back, stopping propagation if handled.
		bool routeInput(const InputEvent& event) {
			// ソートされていない場合は自動的にソート
			if (!is_sorted_) {
				sort();
			}

			// 前面から順に入力イベントを処理
			for (IInputHandler* handler : handlers_) {
				if (handler == nullptr || !handler->isEnabled()) continue;

				// ヒットテストを実行（座標がハンドラーの範囲内かチェック）
				if (!handler->hitTest(event.x, event.y)) continue;

				// ハンドラーに入力イベントを渡す
				if (handler->handleInput(event)) {
					// イベントが処理されたら伝播を停止
					return true;
				}
			}

			// どのハンドラーも処理しなかった
			return false;
		}

		/// @brief 入力イベントをルーティング（座標指定版）
		/// @brief Route input event (coordinate version)
		/// @param input_state_manager 入力状態マネージャー / Input state manager
		/// @param x X座標 / X coordinate
		/// @param y Y座標 / Y coordinate
		/// @return イベントが処理された場合true / true if event was handled
		bool routeInput(InputStateManager* input_state_manager, int x, int y) {
			InputEvent event(input_state_manager, x, y);
			return routeInput(event);
		}

		/// @brief キーボード入力イベントをルーティング
		/// @brief Route keyboard input event
		/// @param input_state_manager 入力状態マネージャー / Input state manager
		/// @return イベントが処理された場合true / true if event was handled
		///
		/// キーボード入力は座標に依存しないため、ヒットテストをスキップして
		/// 全ハンドラーに順番に処理を試みさせます。
		/// Keyboard input is coordinate-independent, so hit test is skipped and
		/// all handlers are given a chance to process it in order.
		bool routeKeyboardInput(InputStateManager* input_state_manager) {
			InputEvent event(InputEventType::Keyboard, input_state_manager);

			// ソートされていない場合は自動的にソート
			// Sort automatically if not sorted
			if (!is_sorted_) {
				sort();
			}

			// 前面から順に入力イベントを処理（ヒットテストなし）
			// Process input event from front to back (without hit test)
			for (IInputHandler* handler : handlers_) {
				if (handler == nullptr || !handler->isEnabled()) continue;

				// キーボード入力はヒットテストをスキップ
				// Skip hit test for keyboard input
				if (handler->handleInput(event)) {
					// イベントが処理されたら伝播を停止
					// Stop propagation if event was handled
					return true;
				}
			}

			// どのハンドラーも処理しなかった
			// No handler processed the event
			return false;
		}

		/// @brief マウスホイール入力イベントをルーティング
		/// @brief Route mouse wheel input event
		/// @param input_state_manager 入力状態マネージャー / Input state manager
		/// @return イベントが処理された場合true / true if event was handled
		///
		/// マウスホイール入力は座標に依存しないため、ヒットテストをスキップして
		/// 全ハンドラーに順番に処理を試みさせます。
		/// Mouse wheel input is coordinate-independent, so hit test is skipped and
		/// all handlers are given a chance to process it in order.
		bool routeMouseWheelInput(InputStateManager* input_state_manager) {
			InputEvent event(InputEventType::MouseWheel, input_state_manager);

			// ソートされていない場合は自動的にソート
			// Sort automatically if not sorted
			if (!is_sorted_) {
				sort();
			}

			// 前面から順に入力イベントを処理（ヒットテストなし）
			// Process input event from front to back (without hit test)
			for (IInputHandler* handler : handlers_) {
				if (handler == nullptr || !handler->isEnabled()) continue;

				// マウスホイール入力はヒットテストをスキップ
				// Skip hit test for mouse wheel input
				if (handler->handleInput(event)) {
					// イベントが処理されたら伝播を停止
					// Stop propagation if event was handled
					return true;
				}
			}

			// どのハンドラーも処理しなかった
			// No handler processed the event
			return false;
		}

		/// @brief 特定レイヤー範囲の入力イベントをルーティング
		/// @brief Route input event to specific layer range
		/// @param event 入力イベント / Input event
		/// @param min_layer 最小レイヤー / Minimum layer (inclusive)
		/// @param max_layer 最大レイヤー / Maximum layer (inclusive)
		/// @return イベントが処理された場合true / true if event was handled
		bool routeInputRange(const InputEvent& event, RenderLayer min_layer, RenderLayer max_layer) {
			// ソートされていない場合は自動的にソート
			if (!is_sorted_) {
				sort();
			}

			// 指定範囲のレイヤーのみ処理
			for (IInputHandler* handler : handlers_) {
				if (handler == nullptr || !handler->isEnabled()) continue;

				RenderLayer layer = handler->getLayer();
				if (layer < min_layer || layer > max_layer) continue;

				// ヒットテストを実行
				if (!handler->hitTest(event.x, event.y)) continue;

				// ハンドラーに入力イベントを渡す
				if (handler->handleInput(event)) {
					return true;
				}
			}

			return false;
		}

		/// @brief 特定レイヤーのみに入力イベントをルーティング
		/// @brief Route input event to specific layer only
		/// @param event 入力イベント / Input event
		/// @param target_layer 対象レイヤー / Target layer
		/// @return イベントが処理された場合true / true if event was handled
		bool routeInputLayer(const InputEvent& event, RenderLayer target_layer) {
			// ソート不要（特定レイヤーのみフィルタリング）
			for (IInputHandler* handler : handlers_) {
				if (handler == nullptr || !handler->isEnabled()) continue;

				if (handler->getLayer() != target_layer) continue;

				// ヒットテストを実行
				if (!handler->hitTest(event.x, event.y)) continue;

				// ハンドラーに入力イベントを渡す
				if (handler->handleInput(event)) {
					return true;
				}
			}

			return false;
		}

		/// @brief ウィンドウリサイズイベントをルーティング
		/// @brief Route window resize event
		/// @param width 新しいウィンドウ幅 / New window width
		/// @param height 新しいウィンドウ高さ / New window height
		/// @return イベントが処理された場合true / true if event was handled
		///
		/// ウィンドウリサイズは座標に依存しないため、ヒットテストをスキップして
		/// 全ハンドラーに順番に通知します。
		/// Window resize is coordinate-independent, so hit test is skipped and
		/// all handlers are notified in order.
		bool routeWindowResizeEvent(int width, int height) {
			InputEvent event(width, height);

			// ソートされていない場合は自動的にソート
			// Sort automatically if not sorted
			if (!is_sorted_) {
				sort();
			}

			// 前面から順にイベントを通知（ヒットテストなし）
			// Notify from front to back (without hit test)
			for (IInputHandler* handler : handlers_) {
				if (handler == nullptr || !handler->isEnabled()) continue;

				// ウィンドウイベントはヒットテストをスキップ
				// Skip hit test for window events
				if (handler->handleInput(event)) {
					// イベントが処理されたら伝播を停止
					// Stop propagation if event was handled
					return true;
				}
			}

			// どのハンドラーも処理しなかった
			// No handler processed the event
			return false;
		}

		/// @brief ウィンドウフォーカスイベントをルーティング
		/// @brief Route window focus event
		/// @param has_focus フォーカス状態（true=取得、false=喪失）/ Focus state (true=gained, false=lost)
		/// @return イベントが処理された場合true / true if event was handled
		///
		/// フォーカスイベントは座標に依存しないため、ヒットテストをスキップして
		/// 全ハンドラーに順番に通知します。
		/// Focus events are coordinate-independent, so hit test is skipped and
		/// all handlers are notified in order.
		bool routeWindowFocusEvent(bool has_focus) {
			InputEvent event = InputEvent::createFocusEvent(has_focus);

			// ソートされていない場合は自動的にソート
			// Sort automatically if not sorted
			if (!is_sorted_) {
				sort();
			}

			// 前面から順にイベントを通知（ヒットテストなし）
			// Notify from front to back (without hit test)
			for (IInputHandler* handler : handlers_) {
				if (handler == nullptr || !handler->isEnabled()) continue;

				// ウィンドウイベントはヒットテストをスキップ
				// Skip hit test for window events
				if (handler->handleInput(event)) {
					// イベントが処理されたら伝播を停止
					// Stop propagation if event was handled
					return true;
				}
			}

			// どのハンドラーも処理しなかった
			// No handler processed the event
			return false;
		}
	};

} // namespace paxs

#endif // !PAX_MAHOROBA_INPUT_ROUTER_HPP
