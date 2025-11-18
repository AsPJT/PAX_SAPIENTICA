/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_DEBUG_PERFORMANCE_MONITOR_HPP
#define PAX_MAHOROBA_UI_DEBUG_DEBUG_PERFORMANCE_MONITOR_HPP

#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

namespace paxs {

/// @brief パフォーマンスモニター（FPS、メモリ使用量などを表示）
/// @brief Performance monitor (displays FPS, memory usage, etc.)
class DebugPerformanceMonitor {
private:
    bool visible_ = false;

    // FPS計測用
    std::deque<double> frame_times_;
    std::chrono::steady_clock::time_point last_frame_time_;
    double fps_ = 0.0;
    double frame_time_ms_ = 0.0;

    // FPS履歴（グラフ描画用）
    std::deque<double> fps_history_;
    static constexpr std::size_t MAX_FPS_HISTORY = 120;  // 2秒分（60FPS想定）

    // 更新間隔
    std::chrono::steady_clock::time_point last_update_time_;
    static constexpr double UPDATE_INTERVAL = 0.5;  // 0.5秒ごとに更新

    // 設定定数
    static constexpr int MONITOR_WIDTH = 300;
    static constexpr int MONITOR_HEIGHT = 250;
    static constexpr int MONITOR_MARGIN = 10;
    static constexpr int GRAPH_HEIGHT = 80;
    static constexpr int PADDING = 10;

public:
    DebugPerformanceMonitor() {
        last_frame_time_ = std::chrono::steady_clock::now();
        last_update_time_ = last_frame_time_;
    }

    ~DebugPerformanceMonitor() = default;

    /// @brief 更新処理（毎フレーム呼び出し）
    /// @brief Update process (called every frame)
    void update() {
        auto current_time = std::chrono::steady_clock::now();

        // フレーム時間を計測
        std::chrono::duration<double> frame_duration = current_time - last_frame_time_;
        double frame_time = frame_duration.count();
        last_frame_time_ = current_time;

        // フレーム時間を記録
        frame_times_.emplace_back(frame_time);
        if (frame_times_.size() > 60) {  // 直近60フレームを保持
            frame_times_.pop_front();
        }

        // 定期的にFPSを更新
        std::chrono::duration<double> update_duration = current_time - last_update_time_;
        if (update_duration.count() >= UPDATE_INTERVAL) {
            calculateFPS();
            last_update_time_ = current_time;

            // FPS履歴を更新
            fps_history_.emplace_back(fps_);
            if (fps_history_.size() > MAX_FPS_HISTORY) {
                fps_history_.pop_front();
            }
        }
    }

    /// @brief 描画処理
    /// @brief Render process
    void render() const {
        if (!visible_) {
            return;
        }

        paxg::Font* font_text = Fonts().getFont(FontProfiles::UI_SMALL);
        if (font_text == nullptr) {
            return;
        }

        const int window_width = paxg::Window::width();
        const int pos_x = window_width - MONITOR_WIDTH - MONITOR_MARGIN;
        const int pos_y = MONITOR_MARGIN;

        // 背景パネル
        paxg::Rect(pos_x, pos_y, MONITOR_WIDTH, MONITOR_HEIGHT)
            .draw(paxg::Color(20, 20, 20, 200));

        // タイトル
        font_text->draw("Performance Monitor",
                       paxg::Vec2i(pos_x + PADDING, pos_y + PADDING),
                       paxg::Color(0, 0, 0));

        // FPS情報
        const int text_y = pos_y + PADDING + 20;
        const paxg::Color fps_color = getFPSColor(fps_);

        font_text->draw("FPS: " + formatDouble(fps_, 1),
                       paxg::Vec2i(pos_x + PADDING, text_y),
                       fps_color);

        font_text->draw("Frame Time: " + formatDouble(frame_time_ms_, 2) + " ms",
                       paxg::Vec2i(pos_x + PADDING, text_y + 20),
                       paxg::Color(0, 0, 0));

        // 統計情報
        if (!frame_times_.empty()) {
            double min_fps = 1000.0;
            double max_fps = 0.0;
            for (double frame_time : frame_times_) {
                if (frame_time > 0.0) {
                    double fps = 1.0 / frame_time;
                    min_fps = (std::min)(fps, min_fps);
                    max_fps = (std::max)(fps, max_fps);
                }
            }

            font_text->draw("Min: " + formatDouble(min_fps, 1) + " / Max: " + formatDouble(max_fps, 1),
                           paxg::Vec2i(pos_x + PADDING, text_y + 40),
                           paxg::Color(0, 0, 0));
        }

        // FPSグラフ
        drawFPSGraph(pos_x + PADDING, pos_y + PADDING + 80,
                    MONITOR_WIDTH - PADDING * 2, GRAPH_HEIGHT);

        // グラフの目盛り
        font_text->draw("60",
                       paxg::Vec2i(pos_x + PADDING - 20, pos_y + PADDING + 80),
                       paxg::Color(100, 100, 100));
        font_text->draw("30",
                       paxg::Vec2i(pos_x + PADDING - 20, pos_y + PADDING + 80 + GRAPH_HEIGHT / 2),
                       paxg::Color(100, 100, 100));
        font_text->draw("0",
                       paxg::Vec2i(pos_x + PADDING - 20, pos_y + PADDING + 80 + GRAPH_HEIGHT),
                       paxg::Color(100, 100, 100));
    }

    /// @brief 現在のFPSを取得
    /// @brief Get current FPS
    double getFPS() const { return fps_; }

    void toggleVisible() { visible_ = !visible_; }
    bool isVisible() const { return visible_; }
    void setVisible(bool visible) { visible_ = visible; }

private:
    /// @brief FPSを計算
    void calculateFPS() {
        if (frame_times_.empty()) {
            fps_ = 0.0;
            frame_time_ms_ = 0.0;
            return;
        }

        // 平均フレーム時間を計算
        double total_time = 0.0;
        for (double ft : frame_times_) {
            total_time += ft;
        }
        double avg_frame_time = total_time / frame_times_.size();

        frame_time_ms_ = avg_frame_time * 1000.0;
        fps_ = (avg_frame_time > 0.0) ? (1.0 / avg_frame_time) : 0.0;
    }

    /// @brief FPSグラフを描画
    void drawFPSGraph(int x, int y, int width, int height) const {
        if (fps_history_.empty()) return;

        // グラフ背景
        paxg::Rect(x, y, width, height).draw(paxg::Color(40, 40, 40));

        // 60FPSの基準線
        const int line_60fps_y = y + height - static_cast<int>((60.0 / 60.0) * height);
        paxg::Rect(x, line_60fps_y, width, 1).draw(paxg::Color(0, 255, 0, 100));

        // 30FPSの基準線
        const int line_30fps_y = y + height - static_cast<int>((30.0 / 60.0) * height);
        paxg::Rect(x, line_30fps_y, width, 1).draw(paxg::Color(255, 255, 0, 100));

        // FPSグラフを描画
        const int bar_width = (std::max)(1, width / static_cast<int>(MAX_FPS_HISTORY));
        for (std::size_t i = 0; i < fps_history_.size(); ++i) {
            const double fps = fps_history_[i];
            const int bar_height = static_cast<int>((fps / 60.0) * height);
            const int clamped_height = (std::min)(bar_height, height);

            const int bar_x = x + static_cast<int>(i) * bar_width;
            const int bar_y = y + height - clamped_height;

            const paxg::Color bar_color = getFPSColor(fps);
            paxg::Rect(bar_x, bar_y, bar_width, clamped_height).draw(bar_color);
        }
    }

    /// @brief FPSに応じた色を取得
    paxg::Color getFPSColor(double fps) const {
        if (fps >= 55.0) return paxg::Color(0, 127, 0);      // 緑：良好
        if (fps >= 45.0) return paxg::Color(100, 127, 0);    // 黄緑：やや良好
        if (fps >= 30.0) return paxg::Color(127, 127, 0);    // 黄：普通
        if (fps >= 20.0) return paxg::Color(127, 75, 0);    // オレンジ：やや悪い
        return paxg::Color(127, 0, 0);                       // 赤：悪い
    }

    /// @brief 数値をフォーマット
    std::string formatDouble(double value, int precision) const {
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "%.*f", precision, value);
        return std::string(buffer);
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_DEBUG_PERFORMANCE_MONITOR_HPP
