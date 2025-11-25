/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_UTILITY_TIME_UTILS_HPP
#define PAX_SAPIENTICA_UTILITY_TIME_UTILS_HPP

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

#include <PAX_SAPIENTICA/Core/Platform.hpp>

namespace paxs {

    /// @brief 時刻関連のユーティリティ関数
    /// @brief Time-related utility functions
    class TimeUtils {
    public:
        /// @brief 現在の日時を指定されたフォーマットで文字列として取得する
        /// @brief Get the current date and time as a string in the specified format
        /// @param format strftime互換フォーマット文字列 / strftime-compatible format string
        /// @param buffer_size バッファサイズ（内部用） / Buffer size (for internal use)
        /// @return フォーマットされた日時文字列 / Formatted date-time string
        static std::string getCurrentDateTime(const char* format = "%Y-%m-%d %X", std::size_t buffer_size = 128) noexcept {
            using namespace std::chrono;

            try {
                // システムクロックから現在時刻を取得
                auto now = system_clock::now();

                // time_tに変換
                std::time_t now_time_t = system_clock::to_time_t(now);

                // ローカル時刻に変換
                std::tm tm_buf{};
                if (!toLocalTimeSafe(now_time_t, tm_buf)) {
                    return ""; // エラー時は空文字列を返す
                }

                // strftimeでフォーマット
                std::string buffer(buffer_size, '\0');
                const std::size_t written = std::strftime(&buffer[0], buffer_size, format, &tm_buf);

                if (written == 0) {
                    return ""; // フォーマット失敗時は空文字列を返す
                }

                buffer.resize(written);
                return buffer;
            }
            catch (...) {
                // 例外が発生した場合は空文字列を返す
                return "";
            }
        }

        /// @brief 指定された time_point を文字列に変換する
        /// @brief Convert the specified time_point to a string
        /// @param time_point 変換する時刻 / Time point to convert
        /// @param format strftime互換フォーマット文字列 / strftime-compatible format string
        /// @param buffer_size バッファサイズ / Buffer size
        /// @return フォーマットされた日時文字列 / Formatted date-time string
        template<typename Clock, typename Duration>
        static std::string formatTimePoint(
            const std::chrono::time_point<Clock, Duration>& time_point,
            const char* format = "%Y-%m-%d-%H-%M-%S",
            std::size_t buffer_size = 64
        ) noexcept {
            using namespace std::chrono;

            try {
                // system_clockに変換
                auto sys_time = time_point_cast<system_clock::duration>(
                    time_point - Clock::now() + system_clock::now()
                );

                std::time_t time_t_val = system_clock::to_time_t(sys_time);
                return formatTime(time_t_val, format, buffer_size);
            }
            catch (...) {
                return "";
            }
        }

        /// @brief 指定された time_t を文字列に変換する
        /// @brief Convert the specified time_t to a string
        /// @param time 変換する時刻 / Time to convert
        /// @param format strftime互換フォーマット文字列 / strftime-compatible format string
        /// @param buffer_size バッファサイズ / Buffer size
        /// @return フォーマットされた日時文字列 / Formatted date-time string
        static std::string formatTime(
            const std::time_t& time,
            const char* format = "%Y-%m-%d-%H-%M-%S",
            std::size_t buffer_size = 64
        ) noexcept {
            std::tm tm_buf{};

            if (!toLocalTimeSafe(time, tm_buf)) {
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

        /// @brief 現在のUNIXタイムスタンプを取得（秒単位）
        /// @brief Get current UNIX timestamp in seconds
        /// @return 秒単位のタイムスタンプ / Timestamp in seconds
        static std::int64_t getCurrentTimestamp() noexcept {
            using namespace std::chrono;
            return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        }

        /// @brief 現在のタイムスタンプを取得（ミリ秒単位）
        /// @brief Get current timestamp in milliseconds
        /// @return ミリ秒単位のタイムスタンプ / Timestamp in milliseconds
        static std::int64_t getCurrentTimestampMillis() noexcept {
            using namespace std::chrono;
            return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        }

        // ========================================================================
        // パフォーマンス計測用ユーティリティ
        // Performance Measurement Utilities
        // ========================================================================

        /// @brief パフォーマンス計測用の時刻型（単調増加保証）
        /// @brief Time point type for performance measurement (monotonic)
        using PerformanceTimePoint = std::chrono::steady_clock::time_point;

        /// @brief パフォーマンス計測用の現在時刻を取得
        /// @brief Get current time for performance measurement
        /// @return 現在の時刻（単調増加保証） / Current time point (monotonic)
        static PerformanceTimePoint now() noexcept {
            return std::chrono::steady_clock::now();
        }

        /// @brief 2つの時刻の差を計算（秒）
        /// @brief Calculate time difference in seconds
        /// @tparam TimePoint 時刻の型 / Type of time point
        /// @param start 開始時刻 / Start time
        /// @param end 終了時刻 / End time
        /// @return 秒単位の差 / Difference in seconds
        template<typename TimePoint>
        static double getDifferenceSeconds(const TimePoint& start, const TimePoint& end) noexcept {
            using namespace std::chrono;
            return duration_cast<duration<double>>(end - start).count();
        }

        /// @brief 2つの時刻の差を計算（ミリ秒）
        /// @brief Calculate time difference in milliseconds
        /// @tparam TimePoint 時刻の型 / Type of time point
        /// @param start 開始時刻 / Start time
        /// @param end 終了時刻 / End time
        /// @return ミリ秒単位の差 / Difference in milliseconds
        template<typename TimePoint>
        static double getDifferenceMilliseconds(const TimePoint& start, const TimePoint& end) noexcept {
            using namespace std::chrono;
            return duration_cast<duration<double, std::milli>>(end - start).count();
        }

        /// @brief 開始時刻からの経過時間を計算（秒）
        /// @brief Calculate elapsed time from start (seconds)
        /// @tparam TimePoint 時刻の型 / Type of time point
        /// @param start 開始時刻 / Start time
        /// @return 経過時間（秒） / Elapsed time in seconds
        template<typename TimePoint>
        static double getElapsedSeconds(const TimePoint& start) noexcept {
            return getDifferenceSeconds(start, now());
        }

        /// @brief 開始時刻からの経過時間を計算（ミリ秒）
        /// @brief Calculate elapsed time from start (milliseconds)
        /// @tparam TimePoint 時刻の型 / Type of time point
        /// @param start 開始時刻 / Start time
        /// @return 経過時間（ミリ秒） / Elapsed time in milliseconds
        template<typename TimePoint>
        static double getElapsedMilliseconds(const TimePoint& start) noexcept {
            return getDifferenceMilliseconds(start, now());
        }

        /// @brief ISO 8601形式の日時文字列を取得
        /// @brief Get ISO 8601 formatted date-time string
        /// @return ISO 8601形式の文字列（例: "2024-11-25T12:34:56"）
        static std::string getISO8601DateTime() noexcept {
            return getCurrentDateTime("%Y-%m-%dT%H:%M:%S");
        }

        /// @brief ファイル名に適した日時文字列を取得
        /// @brief Get filename-safe date-time string
        /// @return ファイル名用の文字列（例: "2024-11-25-12-34-56"）
        static std::string getFilenameSafeDateTime() noexcept {
            return getCurrentDateTime("%Y-%m-%d-%H-%M-%S");
        }

    private:

        /// @brief time_t を std::tm に変換する
        /// @brief Convert time_t to std::tm
        /// @param time 変換する時刻 / Time to convert
        /// @param result 結果を格納する std::tm 構造体 / std::tm structure to store the result
        /// @return true if successful, false otherwise
        static bool toLocalTimeSafe(const std::time_t& time, std::tm& result) noexcept {
            try {
    #if defined(PAXS_COMPILER_MSVC)
                // localtime_s (スレッドセーフ)
                // 第1引数: 出力バッファ, 第2引数: 入力 time_t
                return ::localtime_s(&result, &time) == 0;
    #else
                // localtime_r (スレッドセーフ)
                // 第1引数: 入力 time_t, 第2引数: 出力バッファ
                return ::localtime_r(&time, &result) != nullptr;
    #endif
            }
            catch (...) {
                return false;
            }
        }

        /// @brief durationを人間が読みやすい文字列に変換
        /// @brief Convert duration to human-readable string
        /// @tparam Rep duration の値型 / Value type of duration
        /// @tparam Period duration の周期 / Period of duration
        /// @param duration 変換する時間 / Duration to convert
        /// @return 読みやすい文字列（例: "1h 23m 45s"） / Human-readable string
        template<typename Rep, typename Period>
        static std::string formatDuration(const std::chrono::duration<Rep, Period>& duration) noexcept {
            using namespace std::chrono;

            try {
                auto h = duration_cast<hours>(duration);
                auto m = duration_cast<minutes>(duration - h);
                auto s = duration_cast<seconds>(duration - h - m);
                auto ms = duration_cast<milliseconds>(duration - h - m - s);

                std::ostringstream oss;
                bool has_value = false;

                if (h.count() > 0) {
                    oss << h.count() << "h";
                    has_value = true;
                }
                if (m.count() > 0 || has_value) {
                    if (has_value) oss << " ";
                    oss << m.count() << "m";
                    has_value = true;
                }
                if (s.count() > 0 || has_value) {
                    if (has_value) oss << " ";
                    oss << s.count() << "s";
                    has_value = true;
                }
                if (ms.count() > 0 || !has_value) {
                    if (has_value) oss << " ";
                    oss << ms.count() << "ms";
                }

                return oss.str();
            }
            catch (...) {
                return "0ms";
            }
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_UTILITY_TIME_UTILS_HPP
