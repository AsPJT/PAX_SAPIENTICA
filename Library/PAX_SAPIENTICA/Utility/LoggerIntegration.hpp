/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_LOGGER_INTEGRATION_HPP
#define PAX_SAPIENTICA_LOGGER_INTEGRATION_HPP

#include <PAX_SAPIENTICA/Utility/Logger.hpp>

#ifdef PAXS_DEVELOPMENT
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#endif

namespace paxs {

// Logger::publishLogEvent の実装
inline void Logger::publishLogEvent(const Level level,
                                    const std::string& filename,
                                    const int line,
                                    const std::string& message,
                                    const std::string& timestamp) noexcept {
#ifdef PAXS_DEVELOPMENT
    try {
        LogEvent::Level event_level = LogEvent::Level::Info;  // デフォルト値で初期化
        switch(level) {
            case Level::PAX_INFO:
                event_level = LogEvent::Level::Info;
                break;
            case Level::PAX_WARNING:
                event_level = LogEvent::Level::Warning;
                break;
            case Level::PAX_ERROR:
                event_level = LogEvent::Level::Error;
                break;
            default:
                // 未知のレベルの場合はInfoとして扱う
                event_level = LogEvent::Level::Info;
                break;
        }

        EventBus::getInstance().publish(LogEvent(
            event_level,
            message,
            filename,
            line,
            timestamp
        ));
    } catch (...) {
        // EventBus未初期化時のエラーを無視
    }
#else
    // EventBusが利用できない場合は何もしない
    (void)level;
    (void)filename;
    (void)line;
    (void)message;
    (void)timestamp;
#endif
}

} // namespace paxs

#endif // !PAX_SAPIENTICA_LOGGER_INTEGRATION_HPP
