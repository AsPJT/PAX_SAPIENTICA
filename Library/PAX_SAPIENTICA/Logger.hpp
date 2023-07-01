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

        explicit Logger(const std::string& filename = "Save/error_log.txt") noexcept {
            std::string directory = filename.substr(0, filename.find_last_of("/\\"));
            if (!std::filesystem::exists(directory)) {
                std::filesystem::create_directories(directory);
            }

            file.open(filename, std::ios::app);
        }

        ~Logger() {
            if (file) {
                file.close();
            }
        }

        /// @brief Logs the message.
        void log(Level level, const std::string& message) noexcept {
            // TODO: Add time stamp.
            
            switch(level) {
                case Level::INFO: 
                    file << "[INFO]: " << message << std::endl;
                    break;
                case Level::WARNING: 
                    file << "[WARNING]: " << message << std::endl;
                    break;
                case Level::ERROR: 
                    file << "[ERROR]: " << message << std::endl;
                    break;
            }
        }

        /// @brief logging from an exceptions.
        void handleException(const std::exception& e) noexcept {
            log(Level::ERROR, "Exception: " + std::string(e.what()));
        }
    private:
        std::ofstream file;
    };
}

#endif // !PAX_SAPIENTICA_LOGGER_HPP