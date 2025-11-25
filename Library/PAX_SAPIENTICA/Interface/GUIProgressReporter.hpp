/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_INTERFACE_GUI_PROGRESS_REPORTER_HPP
#define PAX_SAPIENTICA_INTERFACE_GUI_PROGRESS_REPORTER_HPP

#include <string>

#include <PAX_SAPIENTICA/Interface/IProgressReporter.hpp>
#include <PAX_SAPIENTICA/System/Async/ProgressToken.hpp>

namespace paxs {

    /// @brief GUI用進捗報告実装
    /// @brief GUI progress reporting implementation
    /// @details ProgressTokenを使用してGUIに進捗を表示
    ///          Displays progress to GUI using ProgressToken
    template <typename ResultT>
    class GUIProgressReporter : public IProgressReporter {
    private:
        ProgressToken<ResultT>* token_ = nullptr;

    public:
        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param token ProgressTokenへの参照 / Reference to ProgressToken
        explicit GUIProgressReporter(ProgressToken<ResultT>& token) : token_(&token) {}

        void reportProgress(float progress, const std::string& message = "") override {
            if (token_ == nullptr) return;

            token_->setProgress(progress);
            if (!message.empty()) {
                token_->setMessage(message);
            }
        }

        void startProgress(int /*total_items*/, const std::string& message = "") override {
            if (token_ == nullptr) return;

            if (!message.empty()) {
                token_->setMessage(message);
            }
        }

        void endProgress() override {
            // GUIの場合は特に何もしない（進捗は1.0fで完了を表現）
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_INTERFACE_GUI_PROGRESS_REPORTER_HPP
