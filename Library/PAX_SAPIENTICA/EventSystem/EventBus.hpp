/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_EVENT_SYSTEM_EVENT_BUS_HPP
#define PAX_SAPIENTICA_EVENT_SYSTEM_EVENT_BUS_HPP

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>
#include <vector>

#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

/// @brief イベント型の基底クラス
/// @brief Base class for all events
struct Event {
    virtual ~Event() = default;
};

/// @brief イベントハンドラー型
/// @brief Event handler function type
template<typename EventType>
using EventHandler = std::function<void(const EventType&)>;

/// @brief 中央イベントバス（シングルトン）
/// @brief Central event bus (singleton)
class EventBus {
public:
    /// @brief シングルトンインスタンスを取得
    /// @brief Get singleton instance
    static EventBus& getInstance() {
        static EventBus instance;
        return instance;
    }

    // コピー・ムーブを禁止
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus(EventBus&&) = delete;
    EventBus& operator=(EventBus&&) = delete;

    /// @brief イベントを購読
    /// @brief Subscribe to an event type
    /// @tparam EventType イベント型
    /// @param handler イベントハンドラー
    template<typename EventType>
    void subscribe(EventHandler<EventType> handler) {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

        const std::type_index type_id(typeid(EventType));

        auto wrapper = [handler](const Event& event) {
            handler(static_cast<const EventType&>(event));
        };

        subscribers_[type_id].emplace_back(wrapper);
    }

    /// @brief イベントを発行（即座に通知）
    /// @brief Publish event (notify immediately)
    /// @tparam EventType イベント型
    /// @param event イベントオブジェクト
    template<typename EventType>
    void publish(const EventType& event) {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

        const std::type_index type_id(typeid(EventType));

        auto it = subscribers_.find(type_id);
        if (it != subscribers_.end()) {
            for (const auto& handler : it->second) {
                handler(event);
            }
        }
    }

    /// @brief イベントをキューに追加（次フレームで処理）
    /// @brief Enqueue event (process in next frame)
    /// @tparam EventType イベント型
    /// @param event イベントオブジェクト
    template<typename EventType>
    void enqueue(const EventType& event) {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

        event_queue_.push(std::make_unique<EventType>(event));
    }

    /// @brief キューに溜まったイベントを処理
    /// @brief Process all queued events
    void processQueue() {
        while (!event_queue_.empty()) {
            std::unique_ptr<Event>& event = event_queue_.front();

            Event& event_ref = *event;
            const std::type_index type_id(typeid(event_ref));
            auto it = subscribers_.find(type_id);
            if (it != subscribers_.end()) {
                for (const auto& handler : it->second) {
                    handler(event_ref);
                }
            }

            event_queue_.pop();
        }
    }

    /// @brief 購読者数を取得（デバッグ用）
    /// @brief Get subscriber count (for debugging)
    /// @tparam EventType イベント型
    /// @return 購読者数
    template<typename EventType>
    std::size_t getSubscriberCount() const {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

        const std::type_index type_id(typeid(EventType));
        auto it = subscribers_.find(type_id);
        return (it != subscribers_.end()) ? it->second.size() : 0;
    }

    /// @brief キューのサイズを取得（デバッグ用）
    /// @brief Get queue size (for debugging)
    /// @return キューに溜まっているイベント数
    std::size_t getQueueSize() const {
        return event_queue_.size();
    }

    /// @brief 全購読を解除（テスト用）
    /// @brief Clear all subscriptions (for testing)
    /// @details テストで状態をリセットしたい場合のみ使用 / Use only for resetting state in tests
    /// @details Meyer's singletonは自動破棄されるため、このメソッドはテスト専用
    void clearForTesting() {
        subscribers_.clear();
        while (!event_queue_.empty()) {
            event_queue_.pop();
        }
    }

private:
    using EventHandlerWrapper = std::function<void(const Event&)>;

    paxs::UnorderedMap<std::type_index, std::vector<EventHandlerWrapper>> subscribers_;
    std::queue<std::unique_ptr<Event>> event_queue_;

    EventBus() = default;
    ~EventBus() = default;
};

} // namespace paxs

#endif // !PAX_SAPIENTICA_EVENT_SYSTEM_EVENT_BUS_HPP
