/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_DEBUG_LAYER_HPP
#define PAX_MAHOROBA_UI_DEBUG_DEBUG_LAYER_HPP

#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_MAHOROBA/UI/Debug/DebugNotificationManager.hpp>
#include <PAX_MAHOROBA/UI/Debug/DebugConsole.hpp>
#include <PAX_MAHOROBA/UI/Debug/DebugPerformanceMonitor.hpp>
#include <PAX_MAHOROBA/UI/Debug/DebugVariableWatcher.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

/// @brief デバッグレイヤー（デバッグ機能の統合管理）
/// @brief Debug layer (integrated management of debug features)
class DebugLayer : public IRenderable {
private:
    DebugNotificationManager notification_manager_;
    DebugConsole console_;
    DebugPerformanceMonitor performance_monitor_;
    DebugVariableWatcher variable_watcher_;
    bool visible_ = true;

public:
    DebugLayer() = default;
    ~DebugLayer() override = default;

    void render() const override {
        if (!visible_) {
            return;
        }

        // コンソールが表示されている場合は通知を非表示
        if (!console_.isVisible()) {
            notification_manager_.render();
        }
        console_.render();

        // パフォーマンスモニター
        performance_monitor_.render();

        // 変数ウォッチャー
        variable_watcher_.render();
    }

    /// @brief 更新処理（毎フレーム呼び出し）
    /// @brief Update process (called every frame)
    void update(double current_time) {
        notification_manager_.update(current_time);
        performance_monitor_.update();
        variable_watcher_.update();
    }

    /// @brief マウス移動イベントを処理
    /// @brief Handle mouse move event
    void handleMouseMove(const Vector2<int>& mouse_pos) {
        notification_manager_.handleMouseMove(mouse_pos);

        // 将来の拡張例:
        // console_.handleMouseMove(mouse_pos);
    }

    /// @brief マウスクリックイベントを処理
    /// @brief Handle mouse click event
    void handleMouseClick(const Vector2<int>& mouse_pos) {
        // コンソールが表示されている場合、フィルターボタンのクリックをチェック
        if (console_.isVisible()) {
            if (console_.handleFilterButtonClick(mouse_pos)) {
                return;  // フィルターボタンがクリックされた
            }
        }

        notification_manager_.handleMouseClick(mouse_pos);
    }

    /// @brief 現在時刻を取得
    /// @brief Get current time
    double getCurrentTime() const {
        return notification_manager_.getCurrentTime();
    }

    /// @brief コンソールの表示/非表示を切り替え
    /// @brief Toggle console visibility
    void toggleConsole() {
        console_.toggleVisible();
    }

    /// @brief パフォーマンスモニターの表示/非表示を切り替え
    /// @brief Toggle performance monitor visibility
    void togglePerformanceMonitor() {
        performance_monitor_.toggleVisible();
    }

    /// @brief 変数ウォッチャーの表示/非表示を切り替え
    /// @brief Toggle variable watcher visibility
    void toggleVariableWatcher() {
        variable_watcher_.toggleVisible();
    }

    /// @brief デバッグコンソールを取得
    DebugConsole& getConsole() {
        return console_;
    }
    const DebugConsole& getConsole() const {
        return console_;
    }

    /// @brief パフォーマンスモニターを取得
    DebugPerformanceMonitor& getPerformanceMonitor() {
        return performance_monitor_;
    }
    const DebugPerformanceMonitor& getPerformanceMonitor() const {
        return performance_monitor_;
    }

    /// @brief 変数ウォッチャーを取得
    DebugVariableWatcher& getVariableWatcher() {
        return variable_watcher_;
    }
    const DebugVariableWatcher& getVariableWatcher() const {
        return variable_watcher_;
    }

    RenderLayer getLayer() const override { return RenderLayer::Debug; }
    bool isVisible() const override { return visible_; }

};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_DEBUG_LAYER_HPP
