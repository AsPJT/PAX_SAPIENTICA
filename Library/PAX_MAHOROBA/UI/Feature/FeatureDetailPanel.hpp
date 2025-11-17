/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_FEATURE_DETAIL_PANEL_HPP
#define PAX_MAHOROBA_FEATURE_DETAIL_PANEL_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Events/FeatureEvents.hpp>
#include <PAX_MAHOROBA/Map/Location/GenomeFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/GeographicFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonFeature.hpp>
#include <PAX_MAHOROBA/Map/Location/PlaceNameFeature.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/Feature/FeatureCloseButton.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief Feature詳細情報パネルを表示するクラス
    /// @brief Class to display Feature detail information panel
    class FeatureDetailPanel : public IWidget {
    private:
        const UILayout& ui_layout_;
        const paxs::FeatureVisibilityManager& visible_manager_;

        // 現在選択されているFeatureの情報
        const MapFeature* selected_feature_ = nullptr;
        std::string feature_name_;
        std::string feature_type_name_;
        paxs::UnorderedMap<std::uint_least32_t, std::string> extra_data_;

        // 動的に計算されるパネルの高さ
        mutable int calculated_height_ = 0;
        mutable int calculated_lines_ = 0;

        // 閉じるボタン
        FeatureCloseButton close_button_;

        /// @brief パネルのY座標を計算（下部固定）
        int calculatePanelY() const {
            const int panel_height = (calculated_height_ > 0) ? calculated_height_ : ui_layout_.feature_detail_panel_height;
            const int panel_bottom = paxg::Window::height() - ui_layout_.feature_detail_panel_bottom_margin;
            return panel_bottom - panel_height;
        }

        /// @brief 閉じるボタンの位置を更新
        /// @brief Update close button position
        void updateCloseButtonPosition() const {
            const int panel_y = calculatePanelY();
            const int close_button_size = 20;
            const int close_button_x = ui_layout_.feature_detail_panel.x +
                                      ui_layout_.feature_detail_panel.width - close_button_size - 5;
            const int close_button_y = panel_y + 5;

            // constメソッドからsetPosを呼ぶため、const_castを使用
            const_cast<FeatureCloseButton&>(close_button_).setPos(Vector2<int>{ close_button_x, close_button_y });
        }

        /// @brief 長い文字列を複数行に分割して描画
        /// @brief Draw long text with word wrapping
        /// @param font フォント / Font
        /// @param text 描画するテキスト / Text to draw
        /// @param text_x X座標 / X coordinate
        /// @param text_y Y座標 / Y coordinate
        /// @param line_height 行の高さ / Line height
        /// @param max_width 最大幅 / Maximum width
        /// @param current_line 現在の行番号（参照渡し） / Current line number (by reference)
        /// @param color 文字色 / Text color
        void drawWrappedText(
            paxg::Font* font,
            const std::string& text,
            int text_x,
            int text_y,
            int line_height,
            int max_width,
            int& current_line,
            const paxg::Color& color
        ) const {
            if (text.empty()) return;

            // 簡易的な文字列分割（文字数ベース）
            // UTF-8の文字数を大まかに見積もる（1文字あたり平均2.5バイト程度と仮定）
            const int avg_char_width = 12; // フォントサイズに応じた平均文字幅
            const int max_chars_per_line = max_width / avg_char_width;

            std::string remaining_text = text;
            while (!remaining_text.empty()) {
                // 1行に収まる文字数を計算
                std::size_t split_pos = max_chars_per_line;

                if (remaining_text.length() <= split_pos) {
                    // 残りが全て1行に収まる
                    font->draw(
                        remaining_text.c_str(),
                        paxg::Vec2i(text_x, text_y + line_height * current_line++),
                        color
                    );
                    break;
                }

                // 適切な分割位置を探す（スペースや句読点で分割）
                std::size_t last_space = remaining_text.rfind(' ', split_pos);
                std::size_t last_comma = remaining_text.rfind(',', split_pos);
                std::size_t last_jp_comma = remaining_text.rfind(reinterpret_cast<const char*>(u8"、"), split_pos);

                // 最も後ろの区切り文字を使用
                std::size_t actual_split = split_pos;
                if (last_space != std::string::npos && last_space > split_pos / 2) {
                    actual_split = last_space + 1;
                } else if (last_comma != std::string::npos && last_comma > split_pos / 2) {
                    actual_split = last_comma + 1;
                } else if (last_jp_comma != std::string::npos && last_jp_comma > split_pos / 2) {
                    actual_split = last_jp_comma + 3; // UTF-8の「、」は3バイト
                }

                // 1行分を描画
                std::string line_text = remaining_text.substr(0, actual_split);
                font->draw(
                    line_text.c_str(),
                    paxg::Vec2i(text_x, text_y + line_height * current_line++),
                    color
                );

                // 残りのテキストを更新
                remaining_text = remaining_text.substr(actual_split);
            }
        }

    public:
        FeatureDetailPanel(
            const UILayout& ui_layout,
            const paxs::FeatureVisibilityManager& visible_manager
        )
            : ui_layout_(ui_layout)
            , visible_manager_(visible_manager)
        {
            // FeatureSelectedEventを購読
            EventBus::getInstance().subscribe<FeatureSelectedEvent>(
                [this](const FeatureSelectedEvent& event) {
                    onFeatureSelected(event);
                }
            );

            // FeatureDeselectedEventを購読
            EventBus::getInstance().subscribe<FeatureDeselectedEvent>(
                [this](const FeatureDeselectedEvent& event) {
                    onFeatureDeselected(event);
                }
            );

            // 閉じるボタンのコールバックを設定
            close_button_.setOnClick([this]() {
                EventBus::getInstance().publish(FeatureDeselectedEvent());
            });
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible()) return EventHandlingResult::NotHandled();

            // 閉じるボタンの位置を更新
            updateCloseButtonPosition();

            // 閉じるボタンのヒット判定とイベント処理
            if (close_button_.isHit(event.pos)) {
                return close_button_.handleEvent(event);
            }

            return EventHandlingResult::NotHandled();
        }

        void render() const override {
            if (!isVisible()) return;
            if (selected_feature_ == nullptr) return;

            paxg::Font* font = Fonts().getFont(paxs::FontProfiles::KOYOMI);
            if (font == nullptr) return;

            font->setOutline(0, 0.6, paxg::Color(243, 243, 243));

            // パネルの動的な位置を計算
            const int line_height = 25;
            const int padding_top = 10;
            const int padding_left = 10;
            const int panel_y = calculatePanelY();

            const int text_x = ui_layout_.feature_detail_panel.x + padding_left;
            const int text_y = panel_y + padding_top;
            const int max_text_width = ui_layout_.feature_detail_panel.width - padding_left * 2;

            int current_line = 0;

            // タイトル
            font->draw(
                (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"地物詳細") : "Feature Detail",
                paxg::Vec2i(text_x, text_y + line_height * current_line++),
                paxg::Color(0, 0, 0)
            );

            // 種別
            if (!feature_type_name_.empty()) {
                const std::string type_label =
                    (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"種別: ") : "Type: ";

                drawWrappedText(
                    font,
                    type_label + feature_type_name_,
                    text_x,
                    text_y,
                    line_height,
                    max_text_width,
                    current_line,
                    paxg::Color(0, 0, 0)
                );
            }

            // 名前
            if (!feature_name_.empty()) {
                const std::string name_label =
                    (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"名称: ") : "Name: ";

                drawWrappedText(
                    font,
                    name_label + feature_name_,
                    text_x,
                    text_y,
                    line_height,
                    max_text_width,
                    current_line,
                    paxg::Color(0, 0, 0)
                );
            }

            // 追加情報（extra_data）を表示
            if (!extra_data_.empty()) {
                // 空行を追加
                current_line++;

                // 既知のカラム名のハッシュ
                const std::uint_least32_t location_hash = MurMur3::calcHash(reinterpret_cast<const char*>(u8"所在地"));
                const std::uint_least32_t type_hash = MurMur3::calcHash(reinterpret_cast<const char*>(u8"種別"));
                const std::uint_least32_t era_hash = MurMur3::calcHash(reinterpret_cast<const char*>(u8"時代"));
                const std::uint_least32_t site_hash = MurMur3::calcHash(reinterpret_cast<const char*>(u8"立地"));
                const std::uint_least32_t altitude_hash = MurMur3::calcHash(reinterpret_cast<const char*>(u8"標高"));
                const std::uint_least32_t artifact_hash = MurMur3::calcHash(reinterpret_cast<const char*>(u8"出土遺物"));

                // 既知のカラムを特定の順序で表示
                struct KnownColumn {
                    std::uint_least32_t hash;
                    const char* label;
                };
                const KnownColumn known_columns[] = {
                    {location_hash, reinterpret_cast<const char*>(u8"所在地")},
                    {type_hash, reinterpret_cast<const char*>(u8"種別")},
                    {era_hash, reinterpret_cast<const char*>(u8"時代")},
                    {site_hash, reinterpret_cast<const char*>(u8"立地")},
                    {altitude_hash, reinterpret_cast<const char*>(u8"標高")},
                    {artifact_hash, reinterpret_cast<const char*>(u8"出土遺物")}
                };

                for (const auto& col : known_columns) {
                    auto it = extra_data_.find(col.hash);
                    if (it != extra_data_.end()) {
                        const std::string display_text = std::string(col.label) + ": " + it->second;

                        drawWrappedText(
                            font,
                            display_text,
                            text_x,
                            text_y,
                            line_height,
                            max_text_width,
                            current_line,
                            paxg::Color(0, 0, 0)
                        );
                    }
                }

                // 未知のカラムを表示（既知のもの以外）
                for (const auto& [hash, value] : extra_data_) {
                    bool is_known = false;
                    for (const auto& col : known_columns) {
                        if (hash == col.hash) {
                            is_known = true;
                            break;
                        }
                    }

                    if (!is_known && !value.empty()) {
                        // ハッシュキーしかわからないので、値だけ表示
                        drawWrappedText(
                            font,
                            value,
                            text_x,
                            text_y,
                            line_height,
                            max_text_width,
                            current_line,
                            paxg::Color(100, 100, 100)
                        );
                    }
                }
            }

            // 閉じるボタンを描画（動的に計算されたパネル位置を使用）
            updateCloseButtonPosition();
            close_button_.render();

            // 計算された行数と高さを保存
            calculated_lines_ = current_line;
            const int padding_bottom = 10;
            calculated_height_ = (current_line + 1) * line_height + padding_top + padding_bottom;
        }

        Rect<int> getRect() const override {
            // 動的に計算された高さを使用
            // 下部を固定し、上に伸びるように計算
            const int panel_height = (calculated_height_ > 0) ? calculated_height_ : ui_layout_.feature_detail_panel_height;
            const int panel_y = calculatePanelY();

            return Rect<int>(
                ui_layout_.feature_detail_panel.x,
                panel_y,
                ui_layout_.feature_detail_panel.width,
                panel_height
            );
        }

        bool isHit(const paxs::Vector2<int>& pos) const override {
            if (!isVisible()) return false;

            // 閉じるボタンの位置を更新
            updateCloseButtonPosition();

            // 閉じるボタンのヒット判定
            return close_button_.isHit(paxs::Vector2<int>(pos.x, pos.y));
        }

        bool isVisible() const override {
            // 地図が表示されていて、かつFeatureが選択されている場合のみ表示
            return visible_manager_.isVisible(ViewMenu::map) && (selected_feature_ != nullptr);
        }

        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
        const char* getName() const override { return "FeatureDetailPanel"; }
        void setPos(const Vector2<int>& /*pos*/) override {}

    private:
        /// @brief FeatureSelectedEventのハンドラー
        /// @brief Handler for FeatureSelectedEvent
        void onFeatureSelected(const FeatureSelectedEvent& event) {
            selected_feature_ = event.feature;
            if (selected_feature_ == nullptr) return;

            // 現在の言語を取得
            const std::string current_lang =
                (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ? "ja-JP" : "en-US";

            // Featureの名前を取得
            feature_name_ = selected_feature_->getName(current_lang);

            // Featureの種別名を取得
            switch (selected_feature_->getType()) {
                case FeatureType::Geographic:
                    feature_type_name_ =
                        (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"地理的特徴") : "Geographic Feature";
                    break;
                case FeatureType::PlaceName:
                    feature_type_name_ =
                        (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"地名") : "Place Name";
                    break;
                case FeatureType::Genome:
                    feature_type_name_ =
                        (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"ゲノム") : "Genome";
                    break;
                case FeatureType::Person:
                    feature_type_name_ =
                        (Fonts().getSelectedLanguage().getKey() == MurMur3::calcHash("ja-JP")) ?
                        reinterpret_cast<const char*>(u8"人物") : "Person";
                    break;
                default:
                    feature_type_name_ = "";
                    break;
            }

            // LocationPointのextra_dataを取得
            extra_data_.clear();

            // 各Feature型に応じてLocationPointを取得
            switch (selected_feature_->getType()) {
                case FeatureType::Geographic: {
                    const auto* geo_feature = static_cast<const GeographicFeature*>(selected_feature_);
                    extra_data_ = geo_feature->getData().extra_data;
                    break;
                }
                case FeatureType::PlaceName: {
                    const auto* place_feature = static_cast<const PlaceNameFeature*>(selected_feature_);
                    extra_data_ = place_feature->getData().extra_data;
                    break;
                }
                case FeatureType::Genome: {
                    const auto* genome_feature = static_cast<const GenomeFeature*>(selected_feature_);
                    extra_data_ = genome_feature->getData().extra_data;
                    break;
                }
                case FeatureType::Person: {
                    // PersonFeatureはPersonLocationPointを使用するため、extra_dataの構造が異なる可能性がある
                    // TODO: PersonLocationPointにextra_dataを追加する場合はここを修正
                    break;
                }
                default:
                    break;
            }
        }

        /// @brief FeatureDeselectedEventのハンドラー
        /// @brief Handler for FeatureDeselectedEvent
        void onFeatureDeselected(const FeatureDeselectedEvent& /*event*/) {
            selected_feature_ = nullptr;
            feature_name_.clear();
            feature_type_name_.clear();
            extra_data_.clear();
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_FEATURE_DETAIL_PANEL_HPP
