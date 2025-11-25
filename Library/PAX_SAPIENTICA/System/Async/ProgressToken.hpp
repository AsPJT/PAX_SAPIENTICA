/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_ASYNC_PROGRESS_TOKEN_HPP
#define PAX_SAPIENTICA_SYSTEM_ASYNC_PROGRESS_TOKEN_HPP

#include <memory>
#include <string>
#include <utility>

#include <PAX_SAPIENTICA/System/Async/LoadingState.hpp>

namespace paxs {

    /// @brief ロードスレッド側が使用する進捗更新用トークン
    /// @brief Progress update token used by loading thread
    /// @tparam ResultT ロード結果の型 / Result type of loading operation
    template <class ResultT>
    class ProgressToken {
    public:
        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param state 共有状態 / Shared state
        explicit ProgressToken(std::shared_ptr<LoadingState<ResultT>> state)
            : state_(std::move(state)) {}

        /// @brief 進捗率を設定 (0.0 - 1.0)
        /// @brief Set progress ratio (0.0 - 1.0)
        /// @param p 進捗率 / Progress ratio
        void setProgress(float p) {
            if (!state_) return;
            if (p < 0.0f) p = 0.0f;
            if (p > 1.0f) p = 1.0f;
            state_->progress.store(p, std::memory_order_relaxed);
        }

        /// @brief 進捗メッセージを設定
        /// @brief Set progress message
        /// @param msg メッセージ / Message
        void setMessage(const std::string& msg) {
            if (!state_) return;
            std::lock_guard<std::mutex> lock(state_->message_mutex);
            state_->message = msg;
        }

        /// @brief キャンセルされたかチェック
        /// @brief Check if cancelled
        /// @return キャンセルされていればtrue / True if cancelled
        [[nodiscard]] bool isCancelled() const {
            return state_ && state_->cancelled.load(std::memory_order_relaxed);
        }

        /// @brief ロード結果を設定
        /// @brief Set loading result
        /// @param result 結果 / Result
        void setResult(ResultT result) {
            if (!state_) return;
            std::lock_guard<std::mutex> lock(state_->result_mutex);
            state_->result = std::move(result);
        }

    private:
        std::shared_ptr<LoadingState<ResultT>> state_; ///< 共有状態 / Shared state
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_SYSTEM_ASYNC_PROGRESS_TOKEN_HPP
