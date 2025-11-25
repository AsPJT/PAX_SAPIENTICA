/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_PROFILER_HPP
#define PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_PROFILER_HPP

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

#include <PAX_MAHOROBA/UI/Debug/PerformanceScopeData.hpp>

namespace paxs {

/// @brief パフォーマンスプロファイラ（シングルトン）
/// @brief Performance profiler (singleton)
/// @details スコープベースの階層的なパフォーマンス計測を管理
class PerformanceProfiler {
private:
    /// @brief スコープデータのマップ（スコープ名 → データ）
    std::unordered_map<std::string, PerformanceScopeData> scopes_;

    /// @brief 現在のスコープスタック（階層管理用）
    std::vector<std::string> scope_stack_;

    /// @brief ルートスコープのリスト
    std::vector<std::string> root_scopes_;

    /// @brief プロファイラの有効/無効
    bool enabled_ = true;

    /// @brief フレーム開始時刻
    std::chrono::steady_clock::time_point frame_start_time_;

    /// @brief 総フレーム時間（ミリ秒）
    double total_frame_time_ms_ = 0.0;

    // シングルトンパターン
    PerformanceProfiler() {
        frame_start_time_ = std::chrono::steady_clock::now();
    }

    ~PerformanceProfiler() = default;

    // コピー・ムーブ禁止
    PerformanceProfiler(const PerformanceProfiler&) = delete;
    PerformanceProfiler& operator=(const PerformanceProfiler&) = delete;
    PerformanceProfiler(PerformanceProfiler&&) = delete;
    PerformanceProfiler& operator=(PerformanceProfiler&&) = delete;

public:
    /// @brief シングルトンインスタンスを取得
    /// @brief Get singleton instance
    static PerformanceProfiler& getInstance() {
        static PerformanceProfiler instance;
        return instance;
    }

    /// @brief スコープの開始
    /// @brief Begin scope
    /// @param scope_name スコープ名 / Scope name
    /// @return 開始時刻 / Start time
    std::chrono::steady_clock::time_point beginScope(const std::string& scope_name) {
        if (!enabled_) {
            return std::chrono::steady_clock::now();
        }

        // 親スコープを取得
        std::string parent_name;
        int depth = 0;
        if (!scope_stack_.empty()) {
            parent_name = scope_stack_.back();
            depth = static_cast<int>(scope_stack_.size());
        }

        // スコープをスタックに追加
        scope_stack_.push_back(scope_name);

        // スコープデータが存在しない場合は作成
        if (scopes_.find(scope_name) == scopes_.end()) {
            scopes_[scope_name] = PerformanceScopeData(scope_name, parent_name, depth);

            // ルートスコープの場合はリストに追加
            if (parent_name.empty()) {
                root_scopes_.push_back(scope_name);
            }
        }

        // 親スコープに子として登録
        if (!parent_name.empty()) {
            scopes_[parent_name].addChild(scope_name);
        }

        return std::chrono::steady_clock::now();
    }

    /// @brief スコープの終了
    /// @brief End scope
    /// @param scope_name スコープ名 / Scope name
    /// @param start_time 開始時刻 / Start time
    void endScope(const std::string& scope_name, std::chrono::steady_clock::time_point start_time) {
        if (!enabled_) {
            return;
        }

        auto end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        double time_ms = duration.count();

        // スコープデータを更新
        auto it = scopes_.find(scope_name);
        if (it != scopes_.end()) {
            it->second.addMeasurement(time_ms);
        }

        // スタックから削除
        if (!scope_stack_.empty() && scope_stack_.back() == scope_name) {
            scope_stack_.pop_back();
        }
    }

    /// @brief フレームの開始（全スコープをリセット）
    /// @brief Begin frame (reset all scopes)
    void beginFrame() {
        if (!enabled_) {
            return;
        }

        frame_start_time_ = std::chrono::steady_clock::now();

        // 全スコープのフレームをリセット
        for (auto& pair : scopes_) {
            pair.second.resetFrame();
        }

        // スタックをクリア（念のため）
        scope_stack_.clear();
    }

    /// @brief フレームの終了
    /// @brief End frame
    void endFrame() {
        if (!enabled_) {
            return;
        }

        auto end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - frame_start_time_;
        total_frame_time_ms_ = duration.count();
    }

    /// @brief スコープデータを取得
    /// @brief Get scope data
    /// @param scope_name スコープ名 / Scope name
    /// @return スコープデータへのポインタ（存在しない場合はnullptr）
    const PerformanceScopeData* getScopeData(const std::string& scope_name) const {
        auto it = scopes_.find(scope_name);
        if (it != scopes_.end()) {
            return &it->second;
        }
        return nullptr;
    }

    /// @brief 全スコープデータを取得
    /// @brief Get all scope data
    const std::unordered_map<std::string, PerformanceScopeData>& getAllScopes() const {
        return scopes_;
    }

    /// @brief ルートスコープのリストを取得
    /// @brief Get root scope list
    const std::vector<std::string>& getRootScopes() const {
        return root_scopes_;
    }

    /// @brief 総フレーム時間を取得
    /// @brief Get total frame time
    double getTotalFrameTime() const {
        return total_frame_time_ms_;
    }

    /// @brief プロファイラを有効化
    /// @brief Enable profiler
    void enable() {
        enabled_ = true;
    }

    /// @brief プロファイラを無効化
    /// @brief Disable profiler
    void disable() {
        enabled_ = false;
    }

    /// @brief プロファイラの有効/無効を取得
    /// @brief Get profiler enabled state
    bool isEnabled() const {
        return enabled_;
    }

    /// @brief 全データをクリア
    /// @brief Clear all data
    void clear() {
        scopes_.clear();
        root_scopes_.clear();
        scope_stack_.clear();
        total_frame_time_ms_ = 0.0;
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_PERFORMANCE_PROFILER_HPP
