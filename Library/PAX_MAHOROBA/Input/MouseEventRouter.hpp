/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MOUSE_EVENT_ROUTER_HPP
#define PAX_MAHOROBA_MOUSE_EVENT_ROUTER_HPP

#include <algorithm>
#include <vector>

#include <PAX_MAHOROBA/Input/IMouseEventHandler.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

    /// @brief マウスイベントルーター
    /// @brief Mouse event router
    ///
    /// IMouseEventHandler オブジェクトを登録し、Z順序の逆順（前面→背面）で入力イベントをルーティングします。
    /// UIが地図よりも優先して入力を受け取り、処理済みの入力は背面に伝播しません。
    /// Registers IMouseEventHandler objects and routes input events in reverse Z-order (foreground → background).
    /// UI receives input before the map, and processed input does not propagate to the background.
    class MouseEventRouter {
    private:
        /// @brief 登録されたマウスイベントハンドラーのリスト
        /// @brief List of registered mouse event handlers
        std::vector<IMouseEventHandler*> handlers;

        /// @brief ソート済みフラグ（最適化用）
        /// @brief Sorted flag (for optimization)
        bool is_sorted_ = false;

        /// @brief ドラッグキャプチャ中のハンドラー（nullptrでない場合、このハンドラーのみに入力を送る）
        /// @brief Handler that has captured drag input (if not nullptr, only this handler receives input)
        IMouseEventHandler* captured_handler_ = nullptr;

    public:
        /// @brief デフォルトコンストラクタ
        /// @brief Default constructor
        MouseEventRouter() = default;

        /// @brief マウスイベントハンドラーを登録
        /// @brief Register an mouse event handler
        /// @param handler 登録するハンドラー / Handler to register
        void registerHandler(IMouseEventHandler* handler) {
            if (handler == nullptr) return;
            handlers.emplace_back(handler);
            is_sorted_ = false;  // 新規登録があったらソートフラグをリセット
        }

        /// @brief マウスイベントハンドラーの登録を解除
        /// @param handler 登録解除するハンドラー / Handler to unregister
        void unregisterHandler(IMouseEventHandler* handler) {
            auto it = std::find(handlers.begin(), handlers.end(), handler);
            if (it != handlers.end()) {
                handlers.erase(it);
                // ソートフラグはそのまま（削除してもソート順は維持される）
            }
        }

        /// @brief すべての登録を解除
        /// @brief Unregister all handlers
        void clear() {
            handlers.clear();
            is_sorted_ = false;
        }

        /// @brief 登録されているハンドラー数を取得
        /// @brief Get the number of registered handlers
        /// @return ハンドラー数 / Number of handlers
        std::size_t size() const {
            return handlers.size();
        }

        /// @brief 優先度（レイヤーの逆順）でソート
        /// @brief Sort in priority order (reverse layer order)
        ///
        /// レイヤー値が大きい順（前面→背面）にソートします。
        /// Sorts in descending layer value order (foreground → background).
        void sort() {
            std::stable_sort(handlers.begin(), handlers.end(),
                [](const IMouseEventHandler* a, const IMouseEventHandler* b) {
                    // 逆順（前面が先）
                    return a->getLayer() > b->getLayer();
                });
            is_sorted_ = true;
        }

        /// @brief マウスイベントをルーティング
        /// @brief Route mouse event
        /// @param event マウスイベント / Mouse event
        /// @return イベントが処理された場合true / true if event was handled
        ///
        /// 前面から順に各ハンドラーにイベントを渡し、処理されたら伝播を停止します。
        /// ドラッグキャプチャ中の場合は、キャプチャしたハンドラーのみに送信します。
        bool routeEvent(const MouseEvent& event) {
            // ドラッグキャプチャ中の場合、キャプチャしたハンドラーのみに送信
            if (captured_handler_ != nullptr) {
                if (captured_handler_->isEnabled()) {
                    captured_handler_->handleEvent(event);
                }

                // ボタンが離されたらキャプチャを解除
                if (event.left_button_state == MouseButtonState::Released ||
                    event.right_button_state == MouseButtonState::Released ||
                    event.middle_button_state == MouseButtonState::Released) {
                    captured_handler_ = nullptr;
                }

                return true; // キャプチャ中は他のハンドラーに伝播しない
            }

            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // 前面から順にマウスイベントを処理
            for (IMouseEventHandler* handler : handlers) {
                if (handler == nullptr || !handler->isEnabled()) continue;

                // ヒットテストを実行（座標がハンドラーの範囲内かチェック）
                if (!handler->isHit(event.x, event.y)) continue;

                // ハンドラーにマウスイベントを渡す
                EventHandlingResult result = handler->handleEvent(event);

                // ドラッグキャプチャが要求された場合
                if (result.request_capture) {
                    captured_handler_ = handler;
                }

                // イベントが処理された場合は伝播を停止
                if (result.handled) {
                    return true;
                }
            }

            // どのハンドラーも処理しなかった
            return false;
        }

        /// @brief 特定レイヤー範囲の入力イベントをルーティング
        /// @brief Route input event to specific layer range
        /// @param event マウスイベント / Mouse event
        /// @param min_layer 最小レイヤー / Minimum layer (inclusive)
        /// @param max_layer 最大レイヤー / Maximum layer (inclusive)
        /// @return イベントが処理された場合true / true if event was handled
        bool routeInputRange(const MouseEvent& event, RenderLayer min_layer, RenderLayer max_layer) {
            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // 指定範囲のレイヤーのみ処理
            for (IMouseEventHandler* handler : handlers) {
                if (handler == nullptr || !handler->isEnabled()) continue;

                RenderLayer layer = handler->getLayer();
                if (layer < min_layer || layer > max_layer) continue;

                // ヒットテストを実行
                if (!handler->isHit(event.x, event.y)) continue;

                // ハンドラーにマウスイベントを渡す
                EventHandlingResult result = handler->handleEvent(event);

                // ドラッグキャプチャが要求された場合
                if (result.request_capture) {
                    captured_handler_ = handler;
                }

                if (result.handled) {
                    return true;
                }
            }

            return false;
        }

        /// @brief ドラッグキャプチャを解除する
        void releaseDragCapture() {
            captured_handler_ = nullptr;
        }

        /// @brief ドラッグがキャプチャされているかチェック
        /// @return キャプチャされている場合true / true if captured
        bool isDragCaptured() const {
            return captured_handler_ != nullptr;
        }

        /// @brief 特定レイヤーのみに入力イベントをルーティング
        /// @brief Route input event to specific layer only
        /// @param event マウスイベント / Mouse event
        /// @param target_layer 対象レイヤー / Target layer
        /// @return イベントが処理された場合true / true if event was handled
        bool routeInputLayer(const MouseEvent& event, RenderLayer target_layer) {
            // ソート不要（特定レイヤーのみフィルタリング）
            for (IMouseEventHandler* handler : handlers) {
                if (handler == nullptr || !handler->isEnabled()) continue;

                if (handler->getLayer() != target_layer) continue;

                // ヒットテストを実行
                if (!handler->isHit(event.x, event.y)) continue;

                // ハンドラーにマウスイベントを渡す
                EventHandlingResult result = handler->handleEvent(event);

                // ドラッグキャプチャが要求された場合
                if (result.request_capture) {
                    captured_handler_ = handler;
                }

                if (result.handled) {
                    return true;
                }
            }

            return false;
        }

    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MOUSE_EVENT_ROUTER_HPP
