/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_EVENT_ROUTER_HPP
#define PAX_MAHOROBA_EVENT_ROUTER_HPP

#include <algorithm>
#include <vector>

#include <PAX_MAHOROBA/Input/IEventHandler.hpp>

namespace paxs {

    /// @brief イベントルーター（座標に依存しないイベントをブロードキャスト）
    /// @brief Event router (broadcasts coordinate-independent events)
    ///
    /// キーボード、マウスホイール、ウィンドウリサイズ、フォーカスなど、
    /// 座標に依存しないイベントを全ハンドラーにブロードキャストします。
    /// Broadcasts coordinate-independent events to all handlers.
    class EventRouter {
    private:
        /// @brief 登録されたイベントハンドラーのリスト
        /// @brief List of registered event handlers
        std::vector<IEventHandler*> handlers;

    public:
        /// @brief デフォルトコンストラクタ
        /// @brief Default constructor
        EventRouter() = default;

        /// @brief イベントハンドラーを登録
        /// @brief Register event handler
        /// @param handler 登録するイベントハンドラー / Event handler to register
        void registerHandler(IEventHandler* handler) {
            if (handler != nullptr) {
                handlers.emplace_back(handler);
            }
        }

        /// @brief イベントハンドラーの登録を解除
        /// @brief Unregister event handler
        /// @param handler 登録解除するイベントハンドラー / Event handler to unregister
        void unregisterHandler(IEventHandler* handler) {
            handlers.erase(
                std::remove(handlers.begin(), handlers.end(), handler),
                handlers.end()
            );
        }

        /// @brief 全ハンドラーをクリア
        /// @brief Clear all handlers
        void clear() {
            handlers.clear();
        }

        /// @brief 登録されているハンドラー数を取得
        /// @brief Get the number of registered handlers
        /// @return ハンドラー数 / Number of handlers
        std::size_t size() const {
            return handlers.size();
        }

        /// @brief キーボードイベントをブロードキャスト
        /// @brief Broadcast keyboard event to all handlers
        /// @param event キーボードイベント / Keyboard event
        /// @return いずれかのハンドラーが処理した場合true / true if any handler processed
        ///
        /// キーボードイベントは座標に依存しないため、全ハンドラーに順番に通知します。
        /// Keyboard events are coordinate-independent, so all handlers are notified in order.
        bool broadcastEvent(const KeyboardEvent& event) {
            bool handled = false;
            for (IEventHandler* handler : handlers) {
                if (handler != nullptr && handler->isEnabled()) {
                    EventHandlingResult result = handler->handleEvent(event);
                    if (result.handled) {
                        handled = true;
                        // キーボードイベントは複数のハンドラーが処理可能
                        // Multiple handlers can process keyboard events
                    }
                }
            }
            return handled;
        }

        /// @brief マウスホイールイベントをブロードキャスト
        /// @brief Broadcast mouse wheel event to all handlers
        /// @param event マウスホイールイベント / Mouse wheel event
        /// @return いずれかのハンドラーが処理した場合true / true if any handler processed
        ///
        /// マウスホイールイベントは座標に依存しないため、全ハンドラーに順番に通知します。
        /// Mouse wheel events are coordinate-independent, so all handlers are notified in order.
        bool broadcastEvent(const MouseWheelEvent& event) {
            bool handled = false;
            for (IEventHandler* handler : handlers) {
                if (handler != nullptr && handler->isEnabled()) {
                    EventHandlingResult result = handler->handleEvent(event);
                    if (result.handled) {
                        handled = true;
                        // マウスホイールは複数のハンドラーが処理可能
                        // Multiple handlers can process mouse wheel events
                    }
                }
            }
            return handled;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_EVENT_ROUTER_HPP
