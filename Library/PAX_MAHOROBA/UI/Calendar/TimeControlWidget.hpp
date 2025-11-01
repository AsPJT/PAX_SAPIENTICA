/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_TIME_CONTROL_WIDGET_HPP
#define PAX_MAHOROBA_CALENDAR_TIME_CONTROL_WIDGET_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {

    /// @brief 時間操作ウィジェット - 再生/一時停止/逆再生、時間移動アイコンを管理
    /// @brief Time Control Widget - Manages playback/pause/reverse and time navigation icons
    class TimeControlWidget : public IWidget {
    public:
        // 時間操作アイコンのサイズ
        int arrow_time_icon_size = 40; // 再生/停止/逆再生アイコンの大きさ
        int time_icon_size = 50;       // 時間移動アイコンの大きさ

        // アイコンの間隔
        int arrow_icon_move_x = int(arrow_time_icon_size * 1.2);
        int icon_move_x = int(time_icon_size * 1.1);
        int arrow_icon_move_y = int(arrow_time_icon_size * 1.1);
        int icon_move_y = int(time_icon_size * 1.1);

        // 時間操作パネルの高さを取得
        int getHeight() const {
            return arrow_time_icon_size + time_icon_size * 2;
        }

    private:
        // 状態更新（クリック判定などの入力処理）
        void updateState(paxs::InputStateManager& input_state_manager) {
            int icon_start_x = pos_.x();
            int icon_start_y = pos_.y();

            // 再生コントロールの更新
            updatePlaybackControls(icon_start_x, icon_start_y, input_state_manager);
            icon_start_y += arrow_icon_move_y;

            // 時間移動（過去へ）の更新
            updateBackwardTimeControls(icon_start_x, icon_start_y, input_state_manager);
            icon_start_y += icon_move_y;

            // 時間移動（未来へ）の更新
            updateForwardTimeControls(icon_start_x, icon_start_y, input_state_manager);
        }

        // 全てのコントロールを描画
        void drawAllControls() {
            int icon_start_x = pos_.x();
            int icon_start_y = pos_.y();

            // 再生コントロールの描画
            drawPlaybackControlsOnly(icon_start_x, icon_start_y);
            icon_start_y += arrow_icon_move_y;

            // 時間移動（過去へ）の描画
            drawBackwardTimeControlsOnly(icon_start_x, icon_start_y);
            icon_start_y += icon_move_y;

            // 時間移動（未来へ）の描画
            drawForwardTimeControlsOnly(icon_start_x, icon_start_y);
        }

        // 再生コントロールの更新処理
        void updatePlaybackControls(int start_x, int y, paxs::InputStateManager& input_state_manager) {
            int x = start_x;

            // 逆再生ボタン
            paxg::Vec2i pos1(paxg::Window::width() - x, y);
            if (input_state_manager.get(paxg::Rect{ pos1, paxg::Vec2i(arrow_time_icon_size, arrow_time_icon_size) }.leftClicked())) {
                koyomi_->move_forward_in_time = false;
                koyomi_->go_back_in_time = true;
            }
            x -= arrow_icon_move_x;

            // 停止ボタン
            paxg::Vec2i pos2(paxg::Window::width() - x, y);
            if (input_state_manager.get(paxg::Rect{ pos2, paxg::Vec2i(arrow_time_icon_size, arrow_time_icon_size) }.leftClicked())) {
                koyomi_->move_forward_in_time = false;
                koyomi_->go_back_in_time = false;
            }
            x -= arrow_icon_move_x;

            // 再生ボタン
            paxg::Vec2i pos3(paxg::Window::width() - x, y);
            if (input_state_manager.get(paxg::Rect{ pos3, paxg::Vec2i(arrow_time_icon_size, arrow_time_icon_size) }.leftClicked())) {
                koyomi_->move_forward_in_time = true;
                koyomi_->go_back_in_time = false;
            }
        }

        // 再生コントロールの描画のみ
        void drawPlaybackControlsOnly(int start_x, int y) {
            int x = start_x;

            // 逆再生ボタン
            paxg::Vec2i pos1(paxg::Window::width() - x, y);
            texture_dictionary_->at(MurMur3::calcHash("texture_reverse_playback")).resizedDraw(arrow_time_icon_size, pos1);
            x -= arrow_icon_move_x;

            // 停止ボタン
            paxg::Vec2i pos2(paxg::Window::width() - x, y);
            texture_dictionary_->at(MurMur3::calcHash("texture_stop")).resizedDraw(arrow_time_icon_size, pos2);
            x -= arrow_icon_move_x;

            // 再生ボタン
            paxg::Vec2i pos3(paxg::Window::width() - x, y);
            texture_dictionary_->at(MurMur3::calcHash("texture_playback")).resizedDraw(arrow_time_icon_size, pos3);
        }

        // 過去への時間移動の更新処理
        void updateBackwardTimeControls(int start_x, int y, paxs::InputStateManager& input_state_manager) {
            const struct { const char* key; double delta; } buttons[] = {
                {"texture_d_l", -1},
                {"texture_m_l", -(365.2422 / 12.0)},
                {"texture_y_l", -365.2422},
                {"texture_10y_l", -(365.2422 * 10)},
                {"texture_c_l", -(365.2422 * 100)},
                {"texture_10c_l", -(365.2422 * 1000)},
                {"texture_100c_l", -(365.2422 * 10000)}
            };

            int x = start_x;
            for (const auto& btn : buttons) {
                paxg::Vec2i pos(paxg::Window::width() - x, y);
                if (input_state_manager.get(paxg::Rect{ pos, paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_->jdn.getDay() += btn.delta;
                    koyomi_->calcDate();
                }
                x -= icon_move_x;
            }
        }

        // 過去への時間移動の描画のみ
        void drawBackwardTimeControlsOnly(int start_x, int y) {
            const char* texture_keys[] = {
                "texture_d_l", "texture_m_l", "texture_y_l", "texture_10y_l",
                "texture_c_l", "texture_10c_l", "texture_100c_l"
            };

            int x = start_x;
            for (const char* key : texture_keys) {
                paxg::Vec2i pos(paxg::Window::width() - x, y);
                texture_dictionary_->at(MurMur3::calcHash(key)).resizedDraw(time_icon_size, pos);
                x -= icon_move_x;
            }
        }

        // 未来への時間移動の更新処理
        void updateForwardTimeControls(int start_x, int y, paxs::InputStateManager& input_state_manager) {
            const struct { const char* key; double delta; } buttons[] = {
                {"texture_d_r", 1},
                {"texture_m_r", (365.2422 / 12.0)},
                {"texture_y_r", 365.2422},
                {"texture_10y_r", (365.2422 * 10)},
                {"texture_c_r", (365.2422 * 100)},
                {"texture_10c_r", (365.2422 * 1000)},
                {"texture_100c_r", (365.2422 * 10000)}
            };

            int x = start_x;
            for (const auto& btn : buttons) {
                paxg::Vec2i pos(paxg::Window::width() - x, y);
                if (input_state_manager.get(paxg::Rect{ pos, paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_->jdn.getDay() += btn.delta;
                    koyomi_->calcDate();
                }
                x -= icon_move_x;
            }
        }

        // 未来への時間移動の描画のみ
        void drawForwardTimeControlsOnly(int start_x, int y) {
            const char* texture_keys[] = {
                "texture_d_r", "texture_m_r", "texture_y_r", "texture_10y_r",
                "texture_c_r", "texture_10c_r", "texture_100c_r"
            };

            int x = start_x;
            for (const char* key : texture_keys) {
                paxg::Vec2i pos(paxg::Window::width() - x, y);
                texture_dictionary_->at(MurMur3::calcHash(key)).resizedDraw(time_icon_size, pos);
                x -= icon_move_x;
            }
        }

    private:
        // IWidget用の状態管理
        bool visible_ = true;
        bool enabled_ = true;
        paxg::Vec2i pos_{0, 0};

        // 描画に必要な参照（nullptrチェック必須）
        const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>* texture_dictionary_ = nullptr;
        paxs::Koyomi* koyomi_ = nullptr;

    public:
        // IWidget インターフェースの実装
        bool handleInput(const InputEvent& event) override;
        void render() override;

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(pos_.y()),
                static_cast<float>(icon_move_x * 7), // 7つのアイコン
                static_cast<float>(getHeight())
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
        }

        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }

        const char* getName() const override { return "TimeControlPanel"; }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }
        bool isAvailable() const override { return true; }

        // TimeControlPanel固有の初期化メソッド
        void setReferences(
            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dictionary,
            paxs::Koyomi& koyomi
        ) {
            texture_dictionary_ = &texture_dictionary;
            koyomi_ = &koyomi;
        }
    };

    // IWidget メソッドの実装（クラス外定義）
    inline bool TimeControlWidget::handleInput(const InputEvent& event) {
        if (!visible_ || !enabled_ || !texture_dictionary_ || !koyomi_) return false;
        if (event.input_state_manager == nullptr) return false;

        // クリック判定などの更新処理
        updateState(*event.input_state_manager);
        return true;
    }

    inline void TimeControlWidget::render() {
        if (!visible_ || !texture_dictionary_ || !koyomi_) return;

        // 描画処理を実行
        drawAllControls();
    }

} // namespace paxs

#endif // !PAX_MAHOROBA_TIME_CONTROL_WIDGET_HPP
