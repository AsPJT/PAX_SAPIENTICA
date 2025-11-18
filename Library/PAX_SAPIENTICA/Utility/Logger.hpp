/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_LOGGER_HPP
#define PAX_SAPIENTICA_LOGGER_HPP

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <PAX_SAPIENTICA/Core/Utility/TimeUtils.hpp>

namespace paxs {

    /// @brief A struct that logs the message.
    /// @brief メッセージをログする構造体。
    struct Logger {

        /// @brief The level of the log.
        /// @brief ログのレベル。
        enum class Level {
            PAX_INFO, // 情報を示すメッセージ（警告やエラー以外）
            PAX_WARNING, // 警告メッセージ
            PAX_ERROR // エラーメッセージ
        };

        /// @brief Get the current date and time as a string.
        /// @brief 現在の日時を文字列として取得する。
        static std::string currentDateTime() noexcept {
            return TimeUtils::getCurrentDateTime("%Y-%m-%d %X");
        }

        /// @brief Logs the message.
        /// @brief メッセージをログする。
        /// @param level The level of the log. ログのレベル。
        /// @param filename The name of the file to log. ログを記録するファイルの名前。ex) __FILE__
        /// @param line The line number of the file to log. ログを記録するファイルの行番号。ex) __LINE__
        /// @param message The message to log. ログするメッセージ。
        static void log(const Level level, const std::string& filename, const int line, const std::string& message) noexcept {
            const std::string directory = "Save";
            if (!std::filesystem::exists(directory)) {
                std::filesystem::create_directories(directory);
            }

            std::ofstream file(directory + "/log.txt", std::ios::app);

            const std::string current_time = currentDateTime();
            file << "[" << current_time << "] ";

            switch(level) {
                case Level::PAX_INFO:
                    file << "[INFO]: ";
                    break;
                case Level::PAX_WARNING:
                    file << "[WARNING]: ";
                    break;
                case Level::PAX_ERROR:
                    file << "[ERROR]: ";
                    break;
            }

            file << message << " (" << filename << ":" << line << ")" << std::endl;

            // EventBus経由でログイベント発行（定義は後で提供）
            publishLogEvent(level, filename, line, message, current_time);
        }

    private:
        static void publishLogEvent(const Level level,
                                    const std::string& filename,
                                    const int line,
                                    const std::string& message,
                                    const std::string& timestamp) noexcept;
    };
}

#ifdef PAXS_DEVELOPMENT

#define PAXS_ERROR(message) paxs::Logger::log(paxs::Logger::Level::PAX_ERROR, __FILE__, __LINE__, message)
#define PAXS_WARNING(message) paxs::Logger::log(paxs::Logger::Level::PAX_WARNING, __FILE__, __LINE__, message)
#define PAXS_INFO(message) paxs::Logger::log(paxs::Logger::Level::PAX_INFO, __FILE__, __LINE__, message)

#else

#define PAXS_ERROR(message)
#define PAXS_WARNING(message)
#define PAXS_INFO(message)

#endif

#endif // !PAX_SAPIENTICA_LOGGER_HPP
