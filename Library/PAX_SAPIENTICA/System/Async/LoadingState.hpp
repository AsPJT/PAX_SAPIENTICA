/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_ASYNC_LOADING_STATE_HPP
#define PAX_SAPIENTICA_SYSTEM_ASYNC_LOADING_STATE_HPP

#include <atomic>
#include <exception>
#include <mutex>
#include <optional>
#include <string>

namespace paxs {

    /// @brief ロード処理の共有状態
    /// @brief Shared state for loading operations
    /// @tparam ResultT ロード結果の型 / Result type of loading operation
    template <class ResultT>
    struct LoadingState {
        std::atomic<float> progress{0.0f};     ///< 進捗率 (0.0 - 1.0) / Progress ratio
        std::atomic<bool>  finished{false};    ///< 完了フラグ / Completion flag
        std::atomic<bool>  cancelled{false};   ///< キャンセルフラグ / Cancellation flag
        std::atomic<bool>  has_error{false};   ///< エラー発生フラグ / Error occurrence flag

        std::mutex         message_mutex;      ///< メッセージ用ミューテックス / Mutex for message
        std::string        message;            ///< 進捗メッセージ / Progress message

        std::mutex         result_mutex;       ///< 結果用ミューテックス / Mutex for result
        std::optional<ResultT> result;         ///< ロード結果 / Loading result

        std::exception_ptr error;              ///< エラー情報 / Error information
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_SYSTEM_ASYNC_LOADING_STATE_HPP
