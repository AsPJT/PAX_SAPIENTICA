/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_ASYNC_LOADING_HANDLE_HPP
#define PAX_SAPIENTICA_SYSTEM_ASYNC_LOADING_HANDLE_HPP

#include <exception>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include <PAX_SAPIENTICA/System/Async/LoadingState.hpp>
#include <PAX_SAPIENTICA/System/Async/ProgressToken.hpp>

namespace paxs {

    /// @brief メインスレッド側が保持するロード状態へのハンドル
    /// @brief Loading state handle held by main thread
    /// @tparam ResultT ロード結果の型 / Result type of loading operation
    template <class ResultT>
    class LoadingHandle {
    public:
        /// @brief デフォルトコンストラクタ
        /// @brief Default constructor
        LoadingHandle() = default;

        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param state 共有状態 / Shared state
        /// @param worker ワーカースレッド / Worker thread
        LoadingHandle(std::shared_ptr<LoadingState<ResultT>> state,
                      std::thread worker)
            : state_(std::move(state)), worker_(std::move(worker)) {}

        /// @brief デストラクタ（スレッドをjoin）
        /// @brief Destructor (joins thread)
        ~LoadingHandle() {
            if (worker_.joinable()) {
                worker_.join();
            }
        }

        // コピー禁止 / No copy
        LoadingHandle(const LoadingHandle&) = delete;
        LoadingHandle& operator=(const LoadingHandle&) = delete;

        /// @brief ムーブコンストラクタ
        /// @brief Move constructor
        LoadingHandle(LoadingHandle&& other) noexcept {
            *this = std::move(other);
        }

        /// @brief ムーブ代入演算子
        /// @brief Move assignment operator
        LoadingHandle& operator=(LoadingHandle&& other) noexcept {
            if (this != &other) {
                if (worker_.joinable()) worker_.join();
                state_  = std::move(other.state_);
                worker_ = std::move(other.worker_);
            }
            return *this;
        }

        /// @brief 進捗率を取得 (0.0 - 1.0)
        /// @brief Get progress ratio (0.0 - 1.0)
        /// @return 進捗率 / Progress ratio
        [[nodiscard]] float getProgress() const {
            if (!state_) return 0.0f;
            return state_->progress.load(std::memory_order_relaxed);
        }

        /// @brief 進捗メッセージを取得
        /// @brief Get progress message
        /// @return メッセージ / Message
        [[nodiscard]] std::string getMessage() const {
            if (!state_) return {};
            std::lock_guard<std::mutex> lock(state_->message_mutex);
            return state_->message;
        }

        /// @brief ロードが完了したかチェック
        /// @brief Check if loading is finished
        /// @return 完了していればtrue / True if finished
        [[nodiscard]] bool isFinished() const {
            return state_ && state_->finished.load(std::memory_order_acquire);
        }

        /// @brief キャンセルされたかチェック
        /// @brief Check if cancelled
        /// @return キャンセルされていればtrue / True if cancelled
        [[nodiscard]] bool isCancelled() const {
            return state_ && state_->cancelled.load(std::memory_order_relaxed);
        }

        /// @brief エラーが発生したかチェック
        /// @brief Check if error occurred
        /// @return エラーが発生していればtrue / True if error occurred
        [[nodiscard]] bool hasError() const {
            return state_ && state_->has_error.load(std::memory_order_relaxed);
        }

        /// @brief ロードをキャンセル
        /// @brief Cancel loading
        void cancel() {
            if (!state_) return;
            state_->cancelled.store(true, std::memory_order_relaxed);
        }

        /// @brief ワーカースレッドをjoin
        /// @brief Join worker thread
        void join() {
            if (worker_.joinable()) {
                worker_.join();
            }
        }

        /// @brief 結果オブジェクトへのポインタを取得（存在しなければnullptr）
        /// @brief Get pointer to result object (nullptr if not exists)
        /// @return 結果へのポインタ / Pointer to result
        [[nodiscard]] const ResultT* getResult() const {
            if (!state_) return nullptr;
            std::lock_guard<std::mutex> lock(state_->result_mutex);
            if (!state_->result.has_value()) return nullptr;
            return &(*state_->result);
        }

        /// @brief エラー情報を取得
        /// @brief Get error information
        /// @return エラー情報 / Error information
        [[nodiscard]] std::exception_ptr getError() const {
            if (!state_) return nullptr;
            return state_->error;
        }

        /// @brief ハンドルが有効かチェック
        /// @brief Check if handle is valid
        /// @return 有効ならtrue / True if valid
        [[nodiscard]] bool isValid() const {
            return state_ != nullptr;
        }

    private:
        std::shared_ptr<LoadingState<ResultT>> state_;  ///< 共有状態 / Shared state
        std::thread worker_;                            ///< ワーカースレッド / Worker thread
    };

    /// @brief ロード処理を別スレッドで実行するユーティリティ関数
    /// @brief Utility function to execute loading in separate thread
    /// @tparam ResultT ロード結果の型 / Result type
    /// @tparam TaskFunc タスク関数の型 / Task function type
    /// @param task タスク関数（ProgressToken<ResultT>を受け取りResultTを返す） / Task function
    /// @return LoadingHandle / Loading handle
    template <class ResultT, class TaskFunc>
    LoadingHandle<ResultT> startAsyncTask(TaskFunc&& task) {
        using State = LoadingState<ResultT>;

        auto state = std::make_shared<State>();

        std::thread worker([state, task = std::forward<TaskFunc>(task)]() mutable {
            ProgressToken<ResultT> token(state);
            try {
                // 実行開始 / Start execution
                token.setProgress(0.0f);
                token.setMessage("Loading...");

                ResultT result = task(token);

                if (!state->cancelled.load(std::memory_order_relaxed)) {
                    token.setResult(std::move(result));
                    token.setProgress(1.0f);
                }
            } catch (...) {
                state->has_error.store(true, std::memory_order_relaxed);
                state->error = std::current_exception();
            }

            state->finished.store(true, std::memory_order_release);
        });

        return LoadingHandle<ResultT>(std::move(state), std::move(worker));
    }

} // namespace paxs

#endif // !PAX_SAPIENTICA_SYSTEM_ASYNC_LOADING_HANDLE_HPP
