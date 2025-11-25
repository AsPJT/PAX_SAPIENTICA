/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_DEBUG_NOTIFICATION_MANAGER_HPP
#define PAX_MAHOROBA_UI_DEBUG_DEBUG_NOTIFICATION_MANAGER_HPP

#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>

namespace paxs {

/// @brief 通知アイテム（内部データ構造）
/// @brief Notification item (internal data structure)
struct NotificationItem {
    std::string display_text;       // 表示テキスト
    LogEvent::Level level;          // ログレベル
    double creation_time;           // 生成時刻（秒）
    double auto_dismiss_duration;   // 自動削除までの時間（秒）
    int id;                         // 一意なID
    paxg::Rect panel_rect;          // パネル領域
    paxg::Rect close_button_rect;   // 閉じるボタン領域
    bool is_close_button_hovered;   // ホバー状態
};

/// @brief デバッグ通知マネージャー（ログ通知の管理）
/// @brief Debug notification manager (log notification management)
///
/// Logger.hppからのLogEventを購読し、画面中央上部に通知を表示します。
class DebugNotificationManager {
private:
    std::deque<NotificationItem> notification_stack_;
    int next_notification_id_ = 0;

    // 設定定数
    static constexpr int MAX_NOTIFICATIONS = 5;
    static constexpr double AUTO_DISMISS_DURATION = 10.0;
    static constexpr int NOTIFICATION_WIDTH = 500;
    static constexpr int NOTIFICATION_HEIGHT = 60;
    static constexpr int NOTIFICATION_MARGIN = 10;
    static constexpr int SCREEN_MARGIN_Y = 20;
    static constexpr int ICON_SIZE = 30;
    static constexpr int CLOSE_BUTTON_SIZE = 40;

public:
    DebugNotificationManager() {
        EventBus::getInstance().subscribe<LogEvent>(
            [this](const LogEvent& event) {
                handleLogEvent(event);
            }
        );
    }

    ~DebugNotificationManager() = default;

    void render() const {
        if (notification_stack_.empty()) {
            return;
        }

        paxg::Font* font_icon = Fonts().getFont(FontProfiles::UI_MEDIUM);
        paxg::Font* font_text = Fonts().getFont(FontProfiles::UI_MEDIUM);
        if (!font_icon || !font_text) {
            return;
        }

        for (const auto& item : notification_stack_) {
            // パネル背景描画
            const paxg::Color bg_color = getLevelBackgroundColor(item.level);
            item.panel_rect.draw(bg_color);

            // アイコン描画
            font_icon->draw(getLevelIcon(item.level),
                           paxg::Vec2i(static_cast<int>(item.panel_rect.x() + 10), static_cast<int>(item.panel_rect.y() + 15)),
                           getLevelIconColor(item.level));

            // メッセージテキスト描画
            font_text->draw(item.display_text,
                           paxg::Vec2i(static_cast<int>(item.panel_rect.x() + ICON_SIZE + 20), static_cast<int>(item.panel_rect.y() + 10)),
                           paxg::Color(40, 40, 40));

            // 閉じるボタン描画
            const paxg::Color close_color = item.is_close_button_hovered
                ? paxg::Color(200, 50, 50)
                : paxg::Color(150, 150, 150);
            font_icon->draw("×",
                           paxg::Vec2i(item.close_button_rect.x() + 5, item.close_button_rect.y() + 5),
                           close_color);
        }
    }

    void handleLogEvent(const LogEvent& event) {
        while (notification_stack_.size() >= MAX_NOTIFICATIONS) {
            notification_stack_.pop_front();
        }

        NotificationItem item;
        item.display_text = formatMessage(event);
        item.level = event.level;
        item.creation_time = getCurrentTime();
        item.auto_dismiss_duration = AUTO_DISMISS_DURATION;
        item.id = next_notification_id_++;
        item.is_close_button_hovered = false;

        notification_stack_.emplace_back(item);
        updateLayout();
    }

    void handleMouseMove(const Vector2<int>& mouse_pos) {
        for (auto& item : notification_stack_) {
            item.is_close_button_hovered = item.close_button_rect.contains(mouse_pos);
        }
    }

    void handleMouseClick(const Vector2<int>& mouse_pos) {
        for (const auto& item : notification_stack_) {
            if (item.close_button_rect.contains(mouse_pos)) {
                removeNotification(item.id);
                break;
            }
        }
    }

    void update(double current_time) {
        auto it = notification_stack_.begin();
        while (it != notification_stack_.end()) {
            if (current_time - it->creation_time >= it->auto_dismiss_duration) {
                it = notification_stack_.erase(it);
            } else {
                ++it;
            }
        }

        if (!notification_stack_.empty()) {
            updateLayout();
        }
    }

    double getCurrentTime() const {
        using namespace std::chrono;
        auto now = steady_clock::now();
        auto duration = now.time_since_epoch();
        return duration_cast<milliseconds>(duration).count() / 1000.0;
    }

private:
    void updateLayout() {
        const int window_width = paxg::Window::width();
        const int start_x = (window_width - NOTIFICATION_WIDTH) / 2;  // 中央揃え
        int current_y = SCREEN_MARGIN_Y;

        for (auto& item : notification_stack_) {
            item.panel_rect = paxg::Rect(
                start_x,
                current_y,
                NOTIFICATION_WIDTH,
                NOTIFICATION_HEIGHT
            );

            item.close_button_rect = paxg::Rect(
                start_x + NOTIFICATION_WIDTH - CLOSE_BUTTON_SIZE - 5,
                current_y + 5,
                CLOSE_BUTTON_SIZE,
                CLOSE_BUTTON_SIZE
            );

            current_y += NOTIFICATION_HEIGHT + NOTIFICATION_MARGIN;
        }
    }

    void removeNotification(int id) {
        auto it = std::find_if(notification_stack_.begin(), notification_stack_.end(),
                              [id](const NotificationItem& item) { return item.id == id; });
        if (it != notification_stack_.end()) {
            notification_stack_.erase(it);
            updateLayout();
        }
    }

    std::string formatMessage(const LogEvent& event) const {
        std::string level_str;
        switch(event.level) {
            case LogEvent::Level::Info:    level_str = "[INFO]"; break;
            case LogEvent::Level::Warning: level_str = "[WARNING]"; break;
            case LogEvent::Level::Error:   level_str = "[ERROR]"; break;
        }

        std::string short_filename = event.filename;
        size_t last_slash = short_filename.find_last_of("/\\");
        if (last_slash != std::string::npos) {
            short_filename = short_filename.substr(last_slash + 1);
        }

        return level_str + " " + event.message +
               "\n(" + short_filename + ":" + std::to_string(event.line) + ")";
    }

    paxg::Color getLevelBackgroundColor(LogEvent::Level level) const {
        switch(level) {
            case LogEvent::Level::Info:    return paxg::Color(200, 230, 255, 240);
            case LogEvent::Level::Warning: return paxg::Color(255, 240, 200, 240);
            case LogEvent::Level::Error:   return paxg::Color(255, 200, 200, 240);
            default: return paxg::Color(255, 255, 255, 240);
        }
    }

    paxg::Color getLevelIconColor(LogEvent::Level level) const {
        switch(level) {
            case LogEvent::Level::Info:    return paxg::Color(50, 100, 200);
            case LogEvent::Level::Warning: return paxg::Color(200, 150, 0);
            case LogEvent::Level::Error:   return paxg::Color(200, 50, 50);
            default: return paxg::Color(100, 100, 100);
        }
    }

    const char* getLevelIcon(LogEvent::Level level) const {
        switch(level) {
            case LogEvent::Level::Info:    return "ⓘ";
            case LogEvent::Level::Warning: return "⚠";
            case LogEvent::Level::Error:   return "✖";
            default: return "●";
        }
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_DEBUG_NOTIFICATION_MANAGER_HPP
