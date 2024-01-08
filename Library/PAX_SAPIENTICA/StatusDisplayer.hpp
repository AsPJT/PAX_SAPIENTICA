/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_STATUS_DISPLAYER_HPP
#define PAX_SAPIENTICA_STATUS_DISPLAYER_HPP

/*##########################################################################################

##########################################################################################*/

#include <iostream>
#include <iomanip>

namespace paxs {
    /// @brief Display the status.
    /// @brief ステータスを表示する
    struct StatusDisplayer {

        /// @brief Display the progress bar.
        /// @brief プログレスバーを表示する
        static void displayProgressBar(unsigned int current, unsigned int total) noexcept {
            const int bar_width = 50;

            if (total == 0) {
                std::cout << "[";
                for (int i = 0; i < bar_width; ++i) {
                    std::cout << " ";
                }
                std::cout << "] " << std::fixed << std::setprecision(1) << 0.0 << "%\r";
                std::cout.flush();
                return;
            }

            const float progress = static_cast<float>(current) / total;
            const int position = static_cast<int>(bar_width * progress);

            std::cout << "[";
            for (int i = 0; i < bar_width; ++i) {
                if (i < position) {
                    std::cout << "=";
                } else if (i == position) {
                    std::cout << ">";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "] " << std::fixed << std::setprecision(1) << (progress * 100.0) << "%\r";
            std::cout.flush();
        }
    };
}

#endif // !PAX_SAPIENTICA_STATUS_DISPLAYER_HPP
