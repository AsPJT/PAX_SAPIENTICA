/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_SCOPE_DATA_HPP
#define PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_SCOPE_DATA_HPP

#include <algorithm>
#include <deque>
#include <string>
#include <utility>
#include <vector>

namespace paxs {

/// @brief パフォーマンススコープの統計データ
/// @brief Performance scope statistics data
class PerformanceScopeData {
public:
    std::string name;                        ///< スコープ名 / Scope name
    std::string parent_name;                 ///< 親スコープ名 / Parent scope name
    int depth = 0;                           ///< 階層の深さ / Hierarchy depth

    // 現在のフレームでの計測値
    double current_time_ms = 0.0;            ///< 現在の処理時間（ミリ秒） / Current time in milliseconds
    std::size_t call_count = 0;              ///< 呼び出し回数 / Call count

    // 統計情報（直近のフレーム分）
    double min_time_ms = 0.0;                ///< 最小時間 / Minimum time
    double max_time_ms = 0.0;                ///< 最大時間 / Maximum time
    double avg_time_ms = 0.0;                ///< 平均時間 / Average time

    // 履歴データ（グラフ描画用）
    std::deque<double> history;              ///< 時間履歴 / Time history
    static constexpr std::size_t MAX_HISTORY = 120;  ///< 履歴の最大数 / Maximum history count

    // 子スコープ
    std::vector<std::string> children;       ///< 子スコープ名のリスト / List of child scope names

    PerformanceScopeData() = default;

    explicit PerformanceScopeData(std::string  scope_name, std::string  parent = "", int d = 0)
        : name(std::move(scope_name))
        , parent_name(std::move(parent))
        , depth(d)
    {}

    /// @brief 計測データを追加
    /// @brief Add measurement data
    /// @param time_ms 計測時間（ミリ秒） / Measured time in milliseconds
    void addMeasurement(double time_ms) {
        current_time_ms = time_ms;
        call_count++;

        // 履歴に追加
        history.push_back(time_ms);
        if (history.size() > MAX_HISTORY) {
            history.pop_front();
        }

        // 統計を更新
        updateStatistics();
    }

    /// @brief 統計情報を更新
    /// @brief Update statistics
    void updateStatistics() {
        if (history.empty()) {
            min_time_ms = 0.0;
            max_time_ms = 0.0;
            avg_time_ms = 0.0;
            return;
        }

        double sum = 0.0;
        min_time_ms = history[0];
        max_time_ms = history[0];

        for (double time : history) {
            sum += time;
            min_time_ms = (std::min)(min_time_ms, time);
            max_time_ms = (std::max)(max_time_ms, time);
        }

        avg_time_ms = sum / static_cast<double>(history.size());
    }

    /// @brief 子スコープを追加
    /// @brief Add child scope
    /// @param child_name 子スコープ名 / Child scope name
    void addChild(const std::string& child_name) {
        // 重複チェック
        for (const auto& child : children) {
            if (child == child_name) {
                return;
            }
        }
        children.push_back(child_name);
    }

    /// @brief フレームをリセット（新しいフレームの開始時）
    /// @brief Reset frame (at the start of a new frame)
    void resetFrame() {
        current_time_ms = 0.0;
        call_count = 0;
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_SCOPE_DATA_HPP
