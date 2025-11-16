/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_DEBUG_VARIABLE_WATCHER_HPP
#define PAX_MAHOROBA_UI_DEBUG_DEBUG_VARIABLE_WATCHER_HPP

#include <functional>
#include <map>
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

namespace paxs {

/// @brief 変数の値を取得するための関数型
using VariableGetter = std::function<std::string()>;

/// @brief 監視対象の変数情報
struct WatchedVariable {
    std::string name;           // 変数名
    VariableGetter getter;      // 値取得関数
    std::string last_value;     // 前回の値
};

/// @brief 変数ウォッチャー（実行時の変数値を監視・表示）
/// @brief Variable watcher (monitors and displays runtime variable values)
class DebugVariableWatcher {
private:
    bool visible_ = false;
    std::map<std::string, WatchedVariable> watched_variables_;

    // 設定定数
    static constexpr int WATCHER_WIDTH = 350;
    static constexpr int WATCHER_MIN_HEIGHT = 150;
    static constexpr int WATCHER_MARGIN = 10;
    static constexpr int PADDING = 10;
    static constexpr int LINE_HEIGHT = 20;

public:
    DebugVariableWatcher() = default;
    ~DebugVariableWatcher() = default;

    /// @brief 更新処理（変数の値を更新）
    /// @brief Update process (updates variable values)
    void update() {
        for (auto& [name, var] : watched_variables_) {
            try {
                var.last_value = var.getter();
            } catch (...) {
                var.last_value = "[ERROR]";
            }
        }
    }

    /// @brief 描画処理
    /// @brief Render process
    void render() const {
        if (!visible_ || watched_variables_.empty()) return;

        paxg::Font* font_text = Fonts().getFont(FontProfiles::UI_SMALL);
        if (!font_text) return;

        const int window_width = paxg::Window::width();
        const int pos_x = window_width - WATCHER_WIDTH - WATCHER_MARGIN;
        const int pos_y = WATCHER_MARGIN + 260;  // パフォーマンスモニターの下に配置

        // 高さを動的に計算
        const int watcher_height = std::max(
            WATCHER_MIN_HEIGHT,
            static_cast<int>(watched_variables_.size()) * LINE_HEIGHT + PADDING * 2 + 30
        );

        // 背景パネル
        paxg::Rect(pos_x, pos_y, WATCHER_WIDTH, watcher_height)
            .draw(paxg::Color(20, 20, 20, 200));

        // タイトル
        font_text->draw("Variable Watcher",
                       paxg::Vec2i(pos_x + PADDING, pos_y + PADDING),
                       paxg::Color(200, 200, 200));

        // 変数リスト描画
        int current_y = pos_y + PADDING + 25;
        for (const auto& [name, var] : watched_variables_) {
            // 変数名
            font_text->draw(name + ":",
                           paxg::Vec2i(pos_x + PADDING, current_y),
                           paxg::Color(150, 200, 255));

            // 変数値
            const paxg::Color value_color = var.last_value.find("[ERROR]") != std::string::npos
                ? paxg::Color(255, 100, 100)  // エラー時は赤
                : paxg::Color(200, 255, 150);  // 通常時は緑

            font_text->draw(var.last_value,
                           paxg::Vec2i(pos_x + PADDING + 120, current_y),
                           value_color);

            current_y += LINE_HEIGHT;
        }

        // フッター（変数数を表示）
        const std::string count_info = std::to_string(watched_variables_.size()) + " variable(s)";
        font_text->draw(count_info,
                       paxg::Vec2i(pos_x + PADDING, pos_y + watcher_height - PADDING - 15),
                       paxg::Color(150, 150, 150));
    }

    /// @brief 変数を追加
    /// @brief Add variable to watch
    /// @param name 変数名
    /// @param getter 値取得関数
    void addVariable(const std::string& name, VariableGetter getter) {
        WatchedVariable var;
        var.name = name;
        var.getter = getter;
        var.last_value = "";
        watched_variables_[name] = var;
    }

    /// @brief 整数型変数を追加
    /// @brief Add integer variable to watch
    template<typename T>
    void addIntVariable(const std::string& name, const T* ptr) {
        addVariable(name, [ptr]() -> std::string {
            return std::to_string(*ptr);
        });
    }

    /// @brief 浮動小数点型変数を追加
    /// @brief Add floating-point variable to watch
    template<typename T>
    void addFloatVariable(const std::string& name, const T* ptr, int precision = 2) {
        addVariable(name, [ptr, precision]() -> std::string {
            char buffer[64];
            std::snprintf(buffer, sizeof(buffer), "%.*f", precision, static_cast<double>(*ptr));
            return std::string(buffer);
        });
    }

    /// @brief 文字列型変数を追加
    /// @brief Add string variable to watch
    void addStringVariable(const std::string& name, const std::string* ptr) {
        addVariable(name, [ptr]() -> std::string {
            return *ptr;
        });
    }

    /// @brief bool型変数を追加
    /// @brief Add boolean variable to watch
    void addBoolVariable(const std::string& name, const bool* ptr) {
        addVariable(name, [ptr]() -> std::string {
            return *ptr ? "true" : "false";
        });
    }

    /// @brief 変数を削除
    /// @brief Remove variable from watch
    /// @param name 変数名
    void removeVariable(const std::string& name) {
        watched_variables_.erase(name);
    }

    /// @brief 全変数をクリア
    /// @brief Clear all variables
    void clearVariables() {
        watched_variables_.clear();
    }

    /// @brief 監視中の変数数を取得
    /// @brief Get number of watched variables
    std::size_t getVariableCount() const {
        return watched_variables_.size();
    }

    void toggleVisible() { visible_ = !visible_; }
    bool isVisible() const { return visible_; }
    void setVisible(bool visible) { visible_ = visible; }

};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_DEBUG_VARIABLE_WATCHER_HPP
