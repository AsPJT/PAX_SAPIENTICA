/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_LOGGER_HPP
#define PAX_SAPIENTICA_LOGGER_HPP

/*##########################################################################################

##########################################################################################*/

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace paxs {

    /// @brief A class that logs the message.
    /// @brief メッセージをログするクラス。
    class Logger {
    public:

        /// @brief The level of the log.
        /// @brief ログのレベル。
        enum class Level {
            INFO,
            WARNING,
            ERROR
        };

        /// @brief Constructor.
        /// @brief コンストラクタ。
        /// @param filename The name of the file to log. ログを記録するファイルの名前。
        explicit Logger(const std::string& filename = "Save/error_log.txt") noexcept {
            const std::string directory = filename.substr(0, filename.find_last_of("/\\"));
            if (!std::filesystem::exists(directory)) {
                std::filesystem::create_directories(directory);
            }

            file.open(filename, std::ios::app);
        }

        /// @brief Destructor.
        /// @brief デストラクタ。
        ~Logger() noexcept {
            file.close();
        }

        /// @brief Get the current date and time as a string.
        /// @brief 現在の日時を文字列として取得する。
        std::string currentDateTime() const noexcept {
            const auto now = std::chrono::system_clock::now();
            const auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
            return ss.str();
        }

        /// @brief Logs the message.
        /// @brief メッセージをログする。
        /// @param level The level of the log. ログのレベル。
        /// @param filename The name of the file to log. ログを記録するファイルの名前。ex) __FILE__
        /// @param line The line number of the file to log. ログを記録するファイルの行番号。ex) __LINE__
        /// @param message The message to log. ログするメッセージ。
        void log(Level level, const std::string& filename, int line, const std::string& message) noexcept {
            const std::string current_time = currentDateTime();
            file << "[" << current_time << "] ";
            
            switch(level) {
                case Level::INFO: 
                    file << "[INFO]: ";
                    break;
                case Level::WARNING: 
                    file << "[WARNING]: ";
                    break;
                case Level::ERROR: 
                    file << "[ERROR]: ";
                    break;
            }

            file << message << " (" << filename << ":" << line << ")" << std::endl;
        }

        /// @brief logging from an exceptions.
        /// @brief 例外からのログ。
        /// @param e The exception to log. ログする例外。
        /// @param filename The name of the file to log. ログを記録するファイルの名前。ex) __FILE__
        /// @param line The line number of the file to log. ログを記録するファイルの行番号。ex) __LINE__
        void handleException(const std::exception& e, const std::string& filename, int line) noexcept {
            log(Level::ERROR, filename, line, "Exception: " + std::string(e.what()));
        }
    private:
        std::ofstream file; // The file to log.
    };
}

#endif // !PAX_SAPIENTICA_LOGGER_HPP