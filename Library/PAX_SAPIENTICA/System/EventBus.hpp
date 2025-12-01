/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_EVENT_BUS_HPP
#define PAX_SAPIENTICA_SYSTEM_EVENT_BUS_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <typeindex>
#include <vector>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>

namespace paxs {

/// @brief イベント型の基底クラス
/// @brief Base class for all events
struct Event {
    Event() = default;
    Event(const Event&) = default;
    auto operator=(const Event&) -> Event& = default;
    Event(Event&&) noexcept = default;
    auto operator=(Event&&) noexcept -> Event& = default;
    virtual ~Event() = default;
};

/// @brief イベントハンドラー型
/// @brief Event handler function type
template<typename EventType>
using EventHandler = std::function<void(const EventType&)>;

/// @brief 購読ハンドル（購読解除用）
/// @brief Subscription handle (for unsubscribing)
struct SubscriptionHandle {
    std::type_index type_id;
    std::size_t handler_id;

    SubscriptionHandle(std::type_index type, std::size_t handler_index)
        : type_id(type), handler_id(handler_index) {}

    [[nodiscard]] auto isValid() const -> bool { return handler_id != 0; }
};

/// @brief RAII スコープ付き購読
/// @brief RAII scoped subscription
/// @details デストラクタで自動的に購読解除
/// @details Automatically unsubscribes in destructor
class ScopedSubscription {
public:
    ScopedSubscription() = default;
    explicit ScopedSubscription(SubscriptionHandle handle)
        : handle_(handle) {}

    ~ScopedSubscription() {
        unsubscribe();
    }

    // ムーブのみ許可
    ScopedSubscription(const ScopedSubscription&) = delete;
    auto operator=(const ScopedSubscription&) -> ScopedSubscription& = delete;

    ScopedSubscription(ScopedSubscription&& other) noexcept
        : handle_(other.handle_) {
        other.handle_ = SubscriptionHandle(std::type_index(typeid(void)), 0);
    }

    auto operator=(ScopedSubscription&& other) noexcept -> ScopedSubscription& {
        if (this != &other) {
            unsubscribe();
            handle_ = other.handle_;
            other.handle_ = SubscriptionHandle(std::type_index(typeid(void)), 0);
        }
        return *this;
    }

    /// @brief 購読を解除
    /// @brief Unsubscribe
    void unsubscribe();

    /// @brief 有効かチェック
    /// @brief Check if valid
    [[nodiscard]] auto isValid() const -> bool { return handle_.isValid(); }

private:
    SubscriptionHandle handle_{std::type_index(typeid(void)), 0};
};

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

        std::unique_lock lock(mutex_);  // 書き込みロック
        auto* const ptr = subscribers_.try_get(type_id);
        if (ptr != nullptr) {
            ptr->emplace_back(HandlerEntry{next_handler_id_++, wrapper});
        } else {
            std::vector<HandlerEntry> handlers;
            handlers.emplace_back(HandlerEntry{next_handler_id_++, wrapper});
            subscribers_.emplace(type_id, std::move(handlers));
        }
    }

    /// @brief イベントを購読（RAII対応）
    /// @brief Subscribe to an event type (RAII-safe)
    /// @tparam EventType イベント型
    /// @param handler イベントハンドラー
    /// @return ScopedSubscription オブジェクト / ScopedSubscription object
    template<typename EventType>
    [[nodiscard]] auto subscribeScoped(EventHandler<EventType> handler) -> ScopedSubscription {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

        const std::type_index type_id(typeid(EventType));

        std::unique_lock lock(mutex_);  // 書き込みロック
        const std::size_t handler_id = next_handler_id_++;

        auto wrapper = [handler](const Event& event) {
            handler(static_cast<const EventType&>(event));
        };

        auto* const ptr = subscribers_.try_get(type_id);
        if (ptr != nullptr) {
            ptr->emplace_back(HandlerEntry{handler_id, wrapper});
        } else {
            std::vector<HandlerEntry> handlers;
            handlers.emplace_back(HandlerEntry{handler_id, wrapper});
            subscribers_.emplace(type_id, std::move(handlers));
        }

        return ScopedSubscription(SubscriptionHandle(type_id, handler_id));
    }

    /// @brief 購読を解除
    /// @brief Unsubscribe
    /// @param handle 購読ハンドル / Subscription handle
    void unsubscribe(const SubscriptionHandle& handle) {
        if (!handle.isValid()) {
            return;
        }

        std::unique_lock lock(mutex_);  // 書き込みロック
        auto* const ptr = subscribers_.try_get(handle.type_id);
        if (ptr != nullptr) {
            ptr->erase(
                std::remove_if(ptr->begin(), ptr->end(),
                    [handler_id = handle.handler_id](const HandlerEntry& entry) {
                        return entry.id == handler_id;
                    }),
                ptr->end()
            );
        }
    }

    /// @brief イベントを発行（即座に通知）
    /// @brief Publish event (notify immediately)
    /// @tparam EventType イベント型
    /// @param event イベントオブジェクト
    template<typename EventType>
    void publish(const EventType& event) {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

        const std::type_index type_id(typeid(EventType));

        std::shared_lock lock(mutex_);  // 読み取りロック
        const auto* const ptr = subscribers_.try_get(type_id);
        if (ptr != nullptr) {
            for (const auto& entry : *ptr) {
                entry.handler(event);
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

        std::unique_lock lock(mutex_);  // 書き込みロック（キューへの追加）
        event_queue_.push(std::make_unique<EventType>(event));
    }

    /// @brief キューに溜まったイベントを処理
    /// @brief Process all queued events
    void processQueue() {
        std::unique_lock lock(mutex_);  // 書き込みロック（キューとsubscribers_両方にアクセス）
        while (!event_queue_.empty()) {
            const std::unique_ptr<Event>& event = event_queue_.front();

            const Event& event_ref = *event;
            const std::type_index type_id(typeid(event_ref));
            const auto* const ptr = subscribers_.try_get(type_id);
            if (ptr != nullptr) {
                for (const auto& entry : *ptr) {
                    entry.handler(event_ref);
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
        std::shared_lock lock(mutex_);  // 読み取りロック
        const auto iterator = subscribers_.find(type_id);
        return iterator != subscribers_.end() ? iterator->second.size() : 0;
    }

    /// @brief キューのサイズを取得（デバッグ用）
    /// @brief Get queue size (for debugging)
    /// @return キューに溜まっているイベント数
    std::size_t getQueueSize() const {
        std::shared_lock lock(mutex_);  // 読み取りロック
        return event_queue_.size();
    }

    /// @brief 全購読を解除（テスト用）
    /// @brief Clear all subscriptions (for testing)
    /// @details テストで状態をリセットしたい場合のみ使用 / Use only for resetting state in tests
    /// @details Meyer's singletonは自動破棄されるため、このメソッドはテスト専用
    void clearForTesting() {
        std::unique_lock lock(mutex_);  // 書き込みロック
        subscribers_.clear();
        while (!event_queue_.empty()) {
            event_queue_.pop();
        }
    }

private:
    using EventHandlerWrapper = std::function<void(const Event&)>;

    /// @brief ハンドラーエントリー（ID付き）
    /// @brief Handler entry (with ID)
    struct HandlerEntry {
        std::size_t id;
        EventHandlerWrapper handler;
    };

    mutable std::shared_mutex mutex_;  ///< スレッド保護用mutex / Mutex for thread safety
    paxs::UnorderedMap<std::type_index, std::vector<HandlerEntry>> subscribers_;
    std::queue<std::unique_ptr<Event>> event_queue_;
    std::size_t next_handler_id_ = 1;  ///< 次のハンドラーID（0は無効値） / Next handler ID (0 is invalid)

    EventBus() = default;
    ~EventBus() = default;
};

// ============================================================================
// ScopedSubscription implementation (after EventBus is fully defined)
// ============================================================================

inline void ScopedSubscription::unsubscribe() {
    if (handle_.isValid()) {
        EventBus::getInstance().unsubscribe(handle_);
        handle_ = SubscriptionHandle(std::type_index(typeid(void)), 0);
    }
}

} // namespace paxs

#endif // !PAX_SAPIENTICA_SYSTEM_EVENT_BUS_HPP
