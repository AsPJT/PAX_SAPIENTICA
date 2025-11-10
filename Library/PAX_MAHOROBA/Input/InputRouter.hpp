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

namespace paxs {

    /// @brief 統合入力ルーター
    /// @brief Unified input router
    ///
    /// IInputHandler オブジェクトを登録し、レイヤー順序（前面→背面）で全ての入力イベントをルーティングします。
    /// 前面のハンドラーが入力を処理した場合、背面のハンドラーには伝播しません。
    class InputRouter {
    private:
        std::vector<IInputHandler*> handlers_;

        bool is_sorted_ = false;

        /// @brief ドラッグキャプチャ中のハンドラー（nullptrでない場合、このハンドラーのみに入力を送る）
        /// @brief Handler that has captured drag input (if not nullptr, only this handler receives input)
        IInputHandler* captured_handler_ = nullptr;

    public:
        InputRouter() = default;

        /// @brief 入力ハンドラーを登録
        /// @brief Register an input handler
        /// @param handler 登録するハンドラー / Handler to register
        void registerHandler(IInputHandler* handler) {
            if (handler == nullptr) return;
            handlers_.emplace_back(handler);
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
            captured_handler_ = nullptr;
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
        void sort() {
            std::stable_sort(handlers_.begin(), handlers_.end(),
                [](const IInputHandler* a, const IInputHandler* b) {
                    // 逆順（前面が先）
                    return a->getLayer() > b->getLayer();
                });
            is_sorted_ = true;
        }

        /// @brief キーボードイベントをルーティング
        /// @brief Route keyboard event
        /// @param event キーボードイベント / Keyboard event
        /// @return イベントが処理された場合true / true if event was handled
        ///
        /// 前面から順に各ハンドラーにイベントを渡し、処理されたら伝播を停止します。
        /// キーボードイベントは座標に依存しないため、ヒットテストは行われません。
        bool routeEvent(const KeyboardEvent& event) {
            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // 前面から順に処理
            for (IInputHandler* handler : handlers_) {
                if (handler == nullptr) continue;

                EventHandlingResult result = handler->handleEvent(event);

                // イベントが処理されたら伝播を停止
                if (result.handled) {
                    return true;
                }
            }

            return false;
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
                captured_handler_->handleEvent(event);

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
            for (IInputHandler* handler : handlers_) {
                if (handler == nullptr) continue;

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

        /// @brief マウスホイールイベントをルーティング
        /// @brief Route mouse wheel event
        /// @param event マウスホイールイベント / Mouse wheel event
        /// @return イベントが処理された場合true / true if event was handled
        ///
        /// 前面から順に各ハンドラーにイベントを渡し、処理されたら伝播を停止します。
        /// マウスホイールイベントは座標に依存しないため、ヒットテストは行われません。
        bool routeEvent(const MouseWheelEvent& event) {
            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // 前面から順に処理
            for (IInputHandler* handler : handlers_) {
                if (handler == nullptr) continue;

                EventHandlingResult result = handler->handleEvent(event);

                // イベントが処理されたら伝播を停止
                if (result.handled) {
                    return true;
                }
            }

            return false;
        }

        /// @brief 特定レイヤー範囲のキーボードイベントをルーティング
        /// @brief Route keyboard event to specific layer range
        /// @param event キーボードイベント / Keyboard event
        /// @param min_layer 最小レイヤー / Minimum layer (inclusive)
        /// @param max_layer 最大レイヤー / Maximum layer (inclusive)
        /// @return イベントが処理された場合true / true if event was handled
        bool routeEventRange(const KeyboardEvent& event, RenderLayer min_layer, RenderLayer max_layer) {
            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // 指定範囲のレイヤーのみ処理
            for (IInputHandler* handler : handlers_) {
                if (handler == nullptr) continue;

                RenderLayer layer = handler->getLayer();
                if (layer < min_layer || layer > max_layer) continue;

                EventHandlingResult result = handler->handleEvent(event);

                if (result.handled) {
                    return true;
                }
            }

            return false;
        }

        /// @brief 特定レイヤー範囲のマウスイベントをルーティング
        /// @brief Route mouse event to specific layer range
        /// @param event マウスイベント / Mouse event
        /// @param min_layer 最小レイヤー / Minimum layer (inclusive)
        /// @param max_layer 最大レイヤー / Maximum layer (inclusive)
        /// @return イベントが処理された場合true / true if event was handled
        bool routeEventRange(const MouseEvent& event, RenderLayer min_layer, RenderLayer max_layer) {
            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // 指定範囲のレイヤーのみ処理
            for (IInputHandler* handler : handlers_) {
                if (handler == nullptr) continue;

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

        /// @brief 特定レイヤーのみのキーボードイベントをルーティング
        /// @brief Route keyboard event to specific layer only
        /// @param event キーボードイベント / Keyboard event
        /// @param target_layer 対象レイヤー / Target layer
        /// @return イベントが処理された場合true / true if event was handled
        bool routeEventLayer(const KeyboardEvent& event, RenderLayer target_layer) {
            // ソート不要（特定レイヤーのみフィルタリング）
            for (IInputHandler* handler : handlers_) {
                if (handler == nullptr) continue;

                if (handler->getLayer() != target_layer) continue;

                EventHandlingResult result = handler->handleEvent(event);

                if (result.handled) {
                    return true;
                }
            }

            return false;
        }

        /// @brief 特定レイヤーのみのマウスイベントをルーティング
        /// @brief Route mouse event to specific layer only
        /// @param event マウスイベント / Mouse event
        /// @param target_layer 対象レイヤー / Target layer
        /// @return イベントが処理された場合true / true if event was handled
        bool routeEventLayer(const MouseEvent& event, RenderLayer target_layer) {
            // ソート不要（特定レイヤーのみフィルタリング）
            for (IInputHandler* handler : handlers_) {
                if (handler == nullptr) continue;

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

        /// @brief ドラッグキャプチャを解除する
        /// @brief Release drag capture
        void releaseDragCapture() {
            captured_handler_ = nullptr;
        }

        /// @brief ドラッグがキャプチャされているかチェック
        /// @brief Check if drag is captured
        /// @return キャプチャされている場合true / true if captured
        bool isDragCaptured() const {
            return captured_handler_ != nullptr;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_INPUT_ROUTER_HPP
