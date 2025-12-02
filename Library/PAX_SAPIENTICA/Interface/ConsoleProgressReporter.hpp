/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_INTERFACE_CONSOLE_PROGRESS_REPORTER_HPP
#define PAX_SAPIENTICA_INTERFACE_CONSOLE_PROGRESS_REPORTER_HPP

#include <string>

#include <PAX_SAPIENTICA/Interface/IProgressReporter.hpp>
#include <PAX_SAPIENTICA/Utility/StatusDisplayer.hpp>

namespace paxs {

    /// @brief コンソール用進捗報告実装
    /// @brief Console progress reporting implementation
    /// @details StatusDisplayerを使用してコンソールに進捗を表示
    ///          Displays progress to console using StatusDisplayer
    class ConsoleProgressReporter : public IProgressReporter {
    private:
        int total_items_ = 0;
        int current_items_ = 0;

    public:
        ConsoleProgressReporter() = default;

        void reportProgress(float progress, const std::string& message = "") override {
            if (total_items_ > 0) {
                current_items_ = static_cast<int>(progress * static_cast<float>(total_items_));
                StatusDisplayer::displayProgressBar(current_items_, total_items_);
            }
            if (!message.empty()) {
                std::cout << message << std::endl;
            }
        }

        void startProgress(int total_items, const std::string& message = "") override {
            total_items_ = total_items;
            current_items_ = 0;
            if (!message.empty()) {
                std::cout << message << std::endl;
            }
        }

        void endProgress() override {
            if (total_items_ > 0) {
                StatusDisplayer::displayProgressBar(total_items_, total_items_);
                std::cout << std::endl;
            }
            total_items_ = 0;
            current_items_ = 0;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_INTERFACE_CONSOLE_PROGRESS_REPORTER_HPP
