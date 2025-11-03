/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TIME_UTILITY_HPP
#define PAX_SAPIENTICA_TIME_UTILITY_HPP

#include <ctime>
#include <string>

namespace paxs {

	/// @brief 時刻関連のユーティリティ関数
	/// @brief Time-related utility functions
	class TimeUtility {
	public:
		/// @brief time_t を std::tm に安全に変換する（プラットフォーム依存）
		/// @brief Safely convert time_t to std::tm (platform-dependent)
		/// @param time 変換する時刻 / Time to convert
		/// @param result 結果を格納する std::tm 構造体 / std::tm structure to store the result
		/// @return true if successful, false otherwise
		static bool localTimeSafe(const std::time_t& time, std::tm& result) noexcept {
#ifdef _MSC_VER
			// Windows: localtime_s を使用
			return localtime_s(&result, &time) == 0;
#else
			// POSIX: localtime_r を使用
			return localtime_r(&time, &result) != nullptr;
#endif
		}

		/// @brief 現在の日時を指定されたフォーマットで文字列として取得する
		/// @brief Get the current date and time as a string in the specified format
		/// @param format strftime フォーマット文字列 / strftime format string (default: "%Y-%m-%d %X")
		/// @param buffer_size バッファサイズ / Buffer size (default: 128)
		/// @return フォーマットされた日時文字列 / Formatted date-time string
		static std::string getCurrentDateTime(const char* format = "%Y-%m-%d %X", std::size_t buffer_size = 128) noexcept {
			const std::time_t now = std::time(nullptr);
			std::tm tm_buf;

			if (!localTimeSafe(now, tm_buf)) {
				return ""; // エラー時は空文字列を返す
			}

			std::string buffer(buffer_size, '\0');
			const std::size_t written = std::strftime(&buffer[0], buffer_size, format, &tm_buf);

			if (written == 0) {
				return ""; // フォーマット失敗時は空文字列を返す
			}

			buffer.resize(written);
			return buffer;
		}

		/// @brief 指定された time_t を文字列に変換する
		/// @brief Convert the specified time_t to a string
		/// @param time 変換する時刻 / Time to convert
		/// @param format strftime フォーマット文字列 / strftime format string (default: "%Y-%m-%d-%H-%M-%S")
		/// @param buffer_size バッファサイズ / Buffer size (default: 64)
		/// @return フォーマットされた日時文字列 / Formatted date-time string
		static std::string formatTime(const std::time_t& time, const char* format = "%Y-%m-%d-%H-%M-%S", std::size_t buffer_size = 64) noexcept {
			std::tm tm_buf;

			if (!localTimeSafe(time, tm_buf)) {
				return ""; // エラー時は空文字列を返す
			}

			std::string buffer(buffer_size, '\0');
			const std::size_t written = std::strftime(&buffer[0], buffer_size, format, &tm_buf);

			if (written == 0) {
				return ""; // フォーマット失敗時は空文字列を返す
			}

			buffer.resize(written);
			return buffer;
		}
	};

} // namespace paxs

#endif // !PAX_SAPIENTICA_TIME_UTILITY_HPP
