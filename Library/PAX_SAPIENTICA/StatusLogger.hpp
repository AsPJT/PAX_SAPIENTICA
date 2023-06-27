/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_STATUS_LOGGER_HPP
#define PAX_SAPIENTICA_STATUS_LOGGER_HPP

/*##########################################################################################

##########################################################################################*/

#include <iostream>
#include <iomanip>

namespace paxs {
    /// @brief 状態を表示するクラス
    class StatusLogger {
    public:
        /// @brief プログレスバーを表示する
        /// @param current 進行度
        /// @param total 総数
        static void displayProgressBar(unsigned int current, unsigned int total) {
            const int bar_width = 50;

            float progress = static_cast<float>(current) / total;
            int position = static_cast<int>(bar_width * progress);

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

#endif // !PAX_SAPIENTICA_STATUS_LOGGER_HPP