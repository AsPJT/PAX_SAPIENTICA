/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_INTERFACE_I_PROGRESS_REPORTER_HPP
#define PAX_SAPIENTICA_INTERFACE_I_PROGRESS_REPORTER_HPP

#include <functional>
#include <string>

namespace paxs {

    /// @brief 進捗報告インターフェース
    /// @brief Progress reporting interface
    /// @details コンソール版とGUI版で異なる進捗表示実装を提供するための抽象インターフェース
    ///          Abstract interface to provide different progress display implementations for console and GUI versions
    class IProgressReporter {
    public:
        virtual ~IProgressReporter() = default;

        /// @brief 進捗を報告する
        /// @brief Report progress
        /// @param progress 進捗値（0.0～1.0） / Progress value (0.0 to 1.0)
        /// @param message 進捗メッセージ（オプショナル） / Progress message (optional)
        virtual void reportProgress(float progress, const std::string& message = "") = 0;

        /// @brief 進捗表示を開始する
        /// @brief Start progress display
        /// @param total_items 総アイテム数 / Total number of items
        /// @param message 開始メッセージ / Start message
        virtual void startProgress(int total_items, const std::string& message = "") = 0;

        /// @brief 進捗表示を終了する
        /// @brief End progress display
        virtual void endProgress() = 0;
    };

    /// @brief Null実装（進捗表示なし）
    /// @brief Null implementation (no progress display)
    /// @details コンソール版で進捗表示を省略する場合に使用
    ///          Used to omit progress display in console version
    class NullProgressReporter : public IProgressReporter {
    public:
        void reportProgress(float /*progress*/, const std::string& /*message*/ = "") override {}
        void startProgress(int /*total_items*/, const std::string& /*message*/ = "") override {}
        void endProgress() override {}
    };

    /// @brief コールバックベースの進捗報告実装
    /// @brief Callback-based progress reporting implementation
    /// @details ラムダ式やstd::functionを使った進捗報告に使用
    ///          Used for progress reporting with lambdas or std::function
    class CallbackProgressReporter : public IProgressReporter {
    private:
        std::function<void(float, const std::string&)> callback_;

    public:
        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param callback 進捗報告コールバック / Progress reporting callback
        explicit CallbackProgressReporter(std::function<void(float, const std::string&)> callback)
            : callback_(std::move(callback)) {}

        void reportProgress(float progress, const std::string& message = "") override {
            if (callback_) {
                callback_(progress, message);
            }
        }

        void startProgress(int /*total_items*/, const std::string& /*message*/ = "") override {}
        void endProgress() override {}
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_INTERFACE_I_PROGRESS_REPORTER_HPP
