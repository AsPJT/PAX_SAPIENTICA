/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_SCOPE_HPP
#define PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_SCOPE_HPP

#ifdef PAXS_DEVELOPMENT
#include <string>

#include <PAX_MAHOROBA/UI/Debug/PerformanceProfiler.hpp>

#include <PAX_SAPIENTICA/Utility/TimeUtils.hpp>

namespace paxs {

/// @brief パフォーマンススコープ（RAIIパターン）
/// @brief Performance scope (RAII pattern)
/// @details コンストラクタで計測開始、デストラクタで計測終了
class PerformanceScope {
private:
    std::string scope_name_;                   ///< スコープ名 / Scope name
    paxs::TimeUtils::PerformanceTimePoint start_time_; ///< 開始時刻 / Start time
    bool active_;                              ///< アクティブフラグ / Active flag

public:
    /// @brief コンストラクタ（計測開始）
    /// @brief Constructor (start measurement)
    /// @param scope_name スコープ名 / Scope name
    explicit PerformanceScope(const std::string& scope_name)
        : scope_name_(scope_name)
        , active_(PerformanceProfiler::getInstance().isEnabled())
    {
        if (active_) {
            start_time_ = PerformanceProfiler::getInstance().beginScope(scope_name_);
        }
    }

    /// @brief デストラクタ（計測終了）
    /// @brief Destructor (end measurement)
    ~PerformanceScope() {
        if (active_) {
            PerformanceProfiler::getInstance().endScope(scope_name_, start_time_);
        }
    }

    // コピー・ムーブ禁止
    PerformanceScope(const PerformanceScope&) = delete;
    PerformanceScope& operator=(const PerformanceScope&) = delete;
    PerformanceScope(PerformanceScope&&) = delete;
    PerformanceScope& operator=(PerformanceScope&&) = delete;
};

} // namespace paxs
#endif

// デバッグビルド時のみ有効化するマクロ
#ifdef PAXS_DEVELOPMENT
    #define PERF_SCOPE(name) paxs::PerformanceScope perf_scope_##__LINE__(name)
    #define PERF_BEGIN_FRAME() paxs::PerformanceProfiler::getInstance().beginFrame()
    #define PERF_END_FRAME() paxs::PerformanceProfiler::getInstance().endFrame()
#else
    #define PERF_SCOPE(name) ((void)0)
    #define PERF_BEGIN_FRAME() ((void)0)
    #define PERF_END_FRAME() ((void)0)
#endif

#endif // !PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_SCOPE_HPP
