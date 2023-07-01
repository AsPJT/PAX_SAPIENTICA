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

#include <fstream>
#include <iostream>

namespace paxs {

    /// @brief A class that logs the message.
    class Logger {
    public:

        /// @brief The level of the log.
        enum class Level {
            INFO,
            WARNING,
            ERROR
        };

        Logger(const std::string& fileName)
        : logFile(fileName, std::ios::out | std::ios::app) {}

        ~Logger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        void log(Level level, const std::string& message) {
            if (!logFile.is_open()) {
                std::cerr << "Log file not open!" << std::endl;
                return;
            }

            switch(level) {
                case Level::INFO:
                    logFile << "[INFO]: ";
                    break;
                case Level::WARNING:
                    logFile << "[WARNING]: ";
                    break;
                case Level::ERROR:
                    logFile << "[ERROR]: ";
                    break;
            }

            logFile << message << std::endl;
        }

    private:
        std::ofstream logFile;
    };
}

#endif // !PAX_SAPIENTICA_LOGGER_HPP