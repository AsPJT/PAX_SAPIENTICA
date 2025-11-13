/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_DEBUG_CONSOLE_HPP
#define PAX_MAHOROBA_UI_DEBUG_DEBUG_CONSOLE_HPP

#include <chrono>
#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

/// @brief コンソールログアイテム（内部データ構造）
struct ConsoleLogItem {
    std::string display_text;   // 表示テキスト
    LogEvent::Level level;      // ログレベル
    double timestamp;           // タイムスタンプ（秒）
};

/// @brief デバッグコンソール（ログ履歴表示）
class DebugConsole {
private:
    std::deque<ConsoleLogItem> log_history_;
    bool visible_ = false;
    int scroll_offset_ = 0;

    // フィルター設定（ビットマスク）
    std::uint8_t filter_mask_ = 0xFF;  // デフォルトは全て表示
    static constexpr std::uint8_t FILTER_INFO = 0x01;
    static constexpr std::uint8_t FILTER_WARNING = 0x02;
    static constexpr std::uint8_t FILTER_ERROR = 0x04;

#ifdef PAXS_DEVELOPMENT
    // コマンドライン機能
    using CommandHandler = std::function<void(const std::vector<std::string>&)>;
    std::map<std::string, CommandHandler> commands_;
    std::string command_input_;
    std::deque<std::string> command_history_;
    int history_index_ = -1;
    bool input_focused_ = false;
    std::size_t cursor_position_ = 0;  // カーソル位置
    static constexpr int MAX_COMMAND_HISTORY = 50;
#endif

    // 設定定数
    static constexpr int MAX_LOG_HISTORY = 500;
    static constexpr int CONSOLE_MARGIN_X = 20;
    static constexpr int CONSOLE_MARGIN_Y = 100;
    static constexpr int CONSOLE_PADDING = 10;
    static constexpr int LINE_HEIGHT = 20;
    static constexpr int MAX_VISIBLE_LINES = 25;
    static constexpr int FILTER_BUTTON_HEIGHT = 25;
    static constexpr int FILTER_BUTTON_WIDTH = 100;
    static constexpr int FILTER_BUTTON_MARGIN = 10;
    static constexpr int COMMAND_INPUT_HEIGHT = 30;

public:
    DebugConsole() {
        EventBus::getInstance().subscribe<LogEvent>(
            [this](const LogEvent& event) {
                handleLogEvent(event);
            }
        );
#ifdef PAXS_DEVELOPMENT
        registerDefaultCommands();
#endif
    }

    ~DebugConsole() = default;

    void render() const {
        if (!visible_) {
            return;
        }

        paxg::Font* font_text = Fonts().getFont(FontProfiles::UI_MEDIUM);
        if (!font_text) {
            return;
        }

        const int window_width = paxg::Window::width();
        const int window_height = paxg::Window::height();
        const int console_width = window_width - CONSOLE_MARGIN_X * 2;
        const int console_height = window_height - CONSOLE_MARGIN_Y * 2;

        // 背景パネル描画
        paxg::Rect(CONSOLE_MARGIN_X, CONSOLE_MARGIN_Y, console_width, console_height)
            .draw(paxg::Color(20, 20, 20, 230));

        // タイトルバー描画
        paxg::Rect(CONSOLE_MARGIN_X, CONSOLE_MARGIN_Y, console_width, 30)
            .draw(paxg::Color(40, 40, 40, 255));

        font_text->draw("Debug Console",
                       paxg::Vec2i(CONSOLE_MARGIN_X + CONSOLE_PADDING, CONSOLE_MARGIN_Y + 5),
                       paxg::Color(0, 0, 0));

        // フィルターボタン描画
        const int filter_y = CONSOLE_MARGIN_Y + 35;
        drawFilterButton(font_text, "INFO", FILTER_INFO,
                        CONSOLE_MARGIN_X + CONSOLE_PADDING, filter_y,
                        paxg::Color(150, 200, 255));
        drawFilterButton(font_text, "WARNING", FILTER_WARNING,
                        CONSOLE_MARGIN_X + CONSOLE_PADDING + FILTER_BUTTON_WIDTH + FILTER_BUTTON_MARGIN, filter_y,
                        paxg::Color(255, 200, 100));
        drawFilterButton(font_text, "ERROR", FILTER_ERROR,
                        CONSOLE_MARGIN_X + CONSOLE_PADDING + (FILTER_BUTTON_WIDTH + FILTER_BUTTON_MARGIN) * 2, filter_y,
                        paxg::Color(255, 100, 100));

        // ログ履歴描画（フィルタリング適用）
        const int log_area_y = CONSOLE_MARGIN_Y + 70;
        const int log_area_height = console_height - 110 - COMMAND_INPUT_HEIGHT;
        const int max_lines = log_area_height / LINE_HEIGHT;

        // フィルタリングされたログを収集
        std::vector<int> filtered_indices;
        for (int i = 0; i < static_cast<int>(log_history_.size()); ++i) {
            if (isLevelVisible(log_history_[i].level)) {
                filtered_indices.emplace_back(i);
            }
        }

        const int start_index = static_cast<int>(filtered_indices.size()) - scroll_offset_ - max_lines;
        const int end_index = static_cast<int>(filtered_indices.size()) - scroll_offset_;

        int current_y = log_area_y;
        for (int i = (std::max)(0, start_index); i < (std::min)(end_index, static_cast<int>(filtered_indices.size())); ++i) {
            const auto& item = log_history_[filtered_indices[i]];
            const paxg::Color text_color = getLevelColor(item.level);

            font_text->draw(item.display_text,
                           paxg::Vec2i(CONSOLE_MARGIN_X + CONSOLE_PADDING, current_y),
                           text_color);

            current_y += LINE_HEIGHT;
        }

        // スクロールインジケーター
        if (filtered_indices.size() > static_cast<std::size_t>(max_lines)) {
            const std::string scroll_info = "Scroll: " + std::to_string(scroll_offset_) +
                                           " / " + std::to_string(filtered_indices.size() - max_lines);
            font_text->draw(scroll_info,
                           paxg::Vec2i(CONSOLE_MARGIN_X + console_width - 200, CONSOLE_MARGIN_Y + 5),
                           paxg::Color(150, 150, 150));
        }

        // 表示件数情報
        const std::string count_info = "Showing: " + std::to_string(filtered_indices.size()) +
                                       " / " + std::to_string(log_history_.size());
        font_text->draw(count_info,
                       paxg::Vec2i(CONSOLE_MARGIN_X + console_width - 200, CONSOLE_MARGIN_Y + 25),
                       paxg::Color(0, 0, 0));

#ifdef PAXS_DEVELOPMENT
        // コマンド入力フィールド
        const int input_y = CONSOLE_MARGIN_Y + console_height - COMMAND_INPUT_HEIGHT - CONSOLE_PADDING;

        // 入力背景
        const paxg::Color input_bg_color = input_focused_
            ? paxg::Color(50, 50, 50, 255)
            : paxg::Color(35, 35, 35, 255);
        paxg::Rect(CONSOLE_MARGIN_X + CONSOLE_PADDING, input_y,
                   console_width - CONSOLE_PADDING * 2, COMMAND_INPUT_HEIGHT)
            .draw(input_bg_color);

        // プロンプト記号 ">"
        font_text->draw("> ",
                       paxg::Vec2i(CONSOLE_MARGIN_X + CONSOLE_PADDING + 5, input_y + 5),
                       paxg::Color(0, 0, 0));

        // 入力テキスト（カーソル位置にアンダースコアを挿入）
        std::string display_text = command_input_;
        if (input_focused_ && cursor_position_ <= display_text.length()) {
            display_text.insert(cursor_position_, "_");
        }
        font_text->draw(display_text,
                       paxg::Vec2i(CONSOLE_MARGIN_X + CONSOLE_PADDING + 25, input_y + 5),
                       paxg::Color(0, 0, 0));
#endif
    }

    void handleLogEvent(const LogEvent& event) {
        while (log_history_.size() >= MAX_LOG_HISTORY) {
            log_history_.pop_front();
        }

        ConsoleLogItem item;
        item.display_text = formatMessage(event);
        item.level = event.level;
        item.timestamp = getCurrentTime();

        log_history_.emplace_back(item);
    }

    void scrollUp() {
        const int max_scroll = static_cast<int>(log_history_.size()) - MAX_VISIBLE_LINES;
        if (scroll_offset_ < max_scroll) {
            scroll_offset_ += 5;
        }
    }

    void scrollDown() {
        if (scroll_offset_ > 0) {
            scroll_offset_ -= 5;
        }
    }

    double getCurrentTime() const {
        using namespace std::chrono;
        auto now = steady_clock::now();
        auto duration = now.time_since_epoch();
        return duration_cast<milliseconds>(duration).count() / 1000.0;
    }

    void toggleVisible() {
        visible_ = !visible_;
#ifdef PAXS_DEVELOPMENT
        if (visible_) {
            input_focused_ = true;
        } else {
            input_focused_ = false;
        }
#endif
    }
    bool isVisible() const { return visible_; }

#ifdef PAXS_DEVELOPMENT
    /// @brief テキスト入力を処理
    /// @brief Handle text input
    void handleTextInput(char character) {
        if (!visible_ || !input_focused_) return;

        // 印字可能な文字のみ追加（カーソル位置に挿入）
        if (character >= 32 && character <= 126) {
            command_input_.insert(cursor_position_, 1, character);
            cursor_position_++;
        }
    }

    /// @brief キー入力を処理（Enter, Backspace, Up, Down, Left, Right)
    /// @brief Handle key input (Enter, Backspace, Up, Down, Left, Right)
    void handleKeyInput(int key_code) {
        if (!visible_ || !input_focused_) return;

        // Enter: コマンド実行
        if (key_code == 13) {  // Enter key
            if (!command_input_.empty()) {
                executeCommand(command_input_);

                // コマンド履歴に追加
                command_history_.emplace_back(command_input_);
                if (command_history_.size() > MAX_COMMAND_HISTORY) {
                    command_history_.pop_front();
                }

                command_input_.clear();
                cursor_position_ = 0;
                history_index_ = -1;
            }
        }
        // Backspace: カーソル位置の前の文字を削除
        else if (key_code == 8) {  // Backspace key
            if (cursor_position_ > 0 && !command_input_.empty()) {
                command_input_.erase(cursor_position_ - 1, 1);
                cursor_position_--;
            }
        }
        // Up: 履歴を遡る
        else if (key_code == 38) {  // Up arrow
            if (!command_history_.empty()) {
                if (history_index_ == -1) {
                    history_index_ = static_cast<int>(command_history_.size()) - 1;
                } else if (history_index_ > 0) {
                    history_index_--;
                }
                if (history_index_ >= 0) {
                    command_input_ = command_history_[history_index_];
                    cursor_position_ = command_input_.length();
                }
            }
        }
        // Down: 履歴を進める
        else if (key_code == 40) {  // Down arrow
            if (history_index_ != -1) {
                history_index_++;
                if (history_index_ >= static_cast<int>(command_history_.size())) {
                    history_index_ = -1;
                    command_input_.clear();
                    cursor_position_ = 0;
                } else {
                    command_input_ = command_history_[history_index_];
                    cursor_position_ = command_input_.length();
                }
            }
        }
        // Left: カーソルを左に移動
        else if (key_code == 37) {  // Left arrow
            if (cursor_position_ > 0) {
                cursor_position_--;
            }
        }
        // Right: カーソルを右に移動
        else if (key_code == 39) {  // Right arrow
            if (cursor_position_ < command_input_.length()) {
                cursor_position_++;
            }
        }
    }

    /// @brief コマンドを登録
    /// @brief Register command
    void registerCommand(const std::string& name, CommandHandler handler) {
        commands_[name] = handler;
    }

    /// @brief 入力フォーカスを設定
    /// @brief Set input focus
    void setInputFocus(bool focused) {
        input_focused_ = focused;
    }

    /// @brief 入力フォーカスを取得
    /// @brief Get input focus
    bool isInputFocused() const {
        return input_focused_;
    }
#endif

private:
    std::string formatMessage(const LogEvent& event) const {
        std::string level_str;
        switch(event.level) {
            case LogEvent::Level::Info:    level_str = "[INFO]   "; break;
            case LogEvent::Level::Warning: level_str = "[WARNING]"; break;
            case LogEvent::Level::Error:   level_str = "[ERROR]  "; break;
        }

        std::string short_filename = event.filename;
        size_t last_slash = short_filename.find_last_of("/\\");
        if (last_slash != std::string::npos) {
            short_filename = short_filename.substr(last_slash + 1);
        }

        return level_str + " " + event.message +
               " (" + short_filename + ":" + std::to_string(event.line) + ")";
    }

    paxg::Color getLevelColor(LogEvent::Level level) const {
        switch(level) {
            case LogEvent::Level::Info:    return paxg::Color(75, 100, 127);
            case LogEvent::Level::Warning: return paxg::Color(127, 100, 50);
            case LogEvent::Level::Error:   return paxg::Color(127, 50, 50);
            default: return paxg::Color(200, 200, 200);
        }
    }

    /// @brief ログレベルがフィルターで表示対象かチェック
    bool isLevelVisible(LogEvent::Level level) const {
        switch(level) {
            case LogEvent::Level::Info:    return (filter_mask_ & FILTER_INFO) != 0;
            case LogEvent::Level::Warning: return (filter_mask_ & FILTER_WARNING) != 0;
            case LogEvent::Level::Error:   return (filter_mask_ & FILTER_ERROR) != 0;
            default: return true;
        }
    }

    /// @brief フィルターボタンを描画
    void drawFilterButton(paxg::Font* font, const std::string& label, std::uint8_t filter_bit,
                         int x, int y, const paxg::Color& color) const {
        const bool is_active = (filter_mask_ & filter_bit) != 0;
        const paxg::Color bg_color = is_active ? color : paxg::Color(80, 80, 80);
        const paxg::Color text_color = is_active ? paxg::Color(0, 0, 0) : paxg::Color(150, 150, 150);

        // ボタン背景
        paxg::Rect(x, y, FILTER_BUTTON_WIDTH, FILTER_BUTTON_HEIGHT).draw(bg_color);

        // ボタンテキスト
        font->draw(label, paxg::Vec2i(x + 10, y + 3), text_color);
    }

public:
    /// @brief フィルターを切り替え
    void toggleFilter(LogEvent::Level level) {
        switch(level) {
            case LogEvent::Level::Info:
                filter_mask_ ^= FILTER_INFO;
                break;
            case LogEvent::Level::Warning:
                filter_mask_ ^= FILTER_WARNING;
                break;
            case LogEvent::Level::Error:
                filter_mask_ ^= FILTER_ERROR;
                break;
        }
        // スクロール位置をリセット
        scroll_offset_ = 0;
    }

    /// @brief マウスクリックでフィルターボタンをチェック
    bool handleFilterButtonClick(int mouse_x, int mouse_y) {
        if (!visible_) return false;

        const int filter_y = CONSOLE_MARGIN_Y + 35;
        const int button_x1 = CONSOLE_MARGIN_X + CONSOLE_PADDING;
        const int button_x2 = button_x1 + FILTER_BUTTON_WIDTH + FILTER_BUTTON_MARGIN;
        const int button_x3 = button_x2 + FILTER_BUTTON_WIDTH + FILTER_BUTTON_MARGIN;

        // INFO ボタン
        if (mouse_x >= button_x1 && mouse_x < button_x1 + FILTER_BUTTON_WIDTH &&
            mouse_y >= filter_y && mouse_y < filter_y + FILTER_BUTTON_HEIGHT) {
            toggleFilter(LogEvent::Level::Info);
            return true;
        }

        // WARNING ボタン
        if (mouse_x >= button_x2 && mouse_x < button_x2 + FILTER_BUTTON_WIDTH &&
            mouse_y >= filter_y && mouse_y < filter_y + FILTER_BUTTON_HEIGHT) {
            toggleFilter(LogEvent::Level::Warning);
            return true;
        }

        // ERROR ボタン
        if (mouse_x >= button_x3 && mouse_x < button_x3 + FILTER_BUTTON_WIDTH &&
            mouse_y >= filter_y && mouse_y < filter_y + FILTER_BUTTON_HEIGHT) {
            toggleFilter(LogEvent::Level::Error);
            return true;
        }

        return false;
    }

private:
#ifdef PAXS_DEVELOPMENT
    /// @brief コマンドを実行
    /// @brief Execute command
    void executeCommand(const std::string& input) {
        // コマンドをパース
        std::vector<std::string> args = parseCommand(input);
        if (args.empty()) return;

        const std::string& command_name = args[0];

        // コマンドを実行
        auto it = commands_.find(command_name);
        if (it != commands_.end()) {
            try {
                it->second(args);
            } catch (const std::exception& e) {
                PAXS_ERROR("Command execution failed: " + std::string(e.what()));
            } catch (...) {
                PAXS_ERROR("Command execution failed: unknown error");
            }
        } else {
            PAXS_WARNING("Unknown command: " + command_name);
        }
    }

    /// @brief コマンド文字列をパース
    /// @brief Parse command string
    std::vector<std::string> parseCommand(const std::string& input) const {
        std::vector<std::string> args;
        std::istringstream iss(input);
        std::string token;

        while (iss >> token) {
            args.emplace_back(token);
        }

        return args;
    }

    /// @brief デフォルトコマンドを登録
    /// @brief Register default commands
    void registerDefaultCommands() {
        // clear コマンド: ログ履歴をクリア
        registerCommand("clear", [this](const std::vector<std::string>& /*args*/) {
            log_history_.clear();
            scroll_offset_ = 0;
            PAXS_INFO("Console cleared");
        });

        // help コマンド: 利用可能なコマンドを表示
        registerCommand("help", [this](const std::vector<std::string>& /*args*/) {
            PAXS_INFO("Available commands:");
            for (const auto& [cmd_name, _] : commands_) {
                PAXS_INFO("  " + cmd_name);
            }
        });

        // filter コマンド: フィルターを切り替え
        registerCommand("filter", [this](const std::vector<std::string>& args) {
            if (args.size() < 2) {
                PAXS_WARNING("Usage: filter <info|warning|error|all>");
                return;
            }

            const std::string& level = args[1];
            if (level == "info") {
                filter_mask_ ^= FILTER_INFO;
                PAXS_INFO("INFO filter toggled");
            } else if (level == "warning") {
                filter_mask_ ^= FILTER_WARNING;
                PAXS_INFO("WARNING filter toggled");
            } else if (level == "error") {
                filter_mask_ ^= FILTER_ERROR;
                PAXS_INFO("ERROR filter toggled");
            } else if (level == "all") {
                filter_mask_ = 0xFF;
                PAXS_INFO("All filters enabled");
            } else {
                PAXS_WARNING("Unknown filter level: " + level);
            }
            scroll_offset_ = 0;
        });

        // history コマンド: コマンド履歴を表示
        registerCommand("history", [this](const std::vector<std::string>& /*args*/) {
            if (command_history_.empty()) {
                PAXS_INFO("No command history");
                return;
            }
            PAXS_INFO("Command history:");
            for (std::size_t i = 0; i < command_history_.size(); ++i) {
                PAXS_INFO("  " + std::to_string(i + 1) + ": " + command_history_[i]);
            }
        });
    }
#endif // PAXS_DEVELOPMENT
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_DEBUG_CONSOLE_HPP
