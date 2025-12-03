/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_feature_info_panel_HPP
#define PAX_MAHOROBA_feature_info_panel_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>

#include <PAX_MAHOROBA/Events/FeatureEvents.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/GenomeFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/GeographicFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/PersonFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/PlaceNameFeature.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/InteractiveUIComponent.hpp>
#include <PAX_MAHOROBA/UI/Feature/FeatureCloseButton.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief Feature詳細情報パネルを表示するクラス
    /// @brief Class to display Feature info panel
    class FeatureInfoPanel : public InteractiveUIComponent {
    private:
        // FeatureInfo domain hash
        static constexpr std::uint_least32_t feature_info_domain_hash = MurMur3::calcHash("FeatureInfo");
        static constexpr std::uint_least32_t title_key = MurMur3::calcHash("title");
        static constexpr std::uint_least32_t label_type_key = MurMur3::calcHash("label_type");
        static constexpr std::uint_least32_t label_name_key = MurMur3::calcHash("label_name");
        static constexpr std::uint_least32_t type_geographic_key = MurMur3::calcHash("type_geographic");
        static constexpr std::uint_least32_t type_place_name_key = MurMur3::calcHash("type_place_name");
        static constexpr std::uint_least32_t type_genome_key = MurMur3::calcHash("type_genome");
        static constexpr std::uint_least32_t type_person_key = MurMur3::calcHash("type_person");
        static constexpr std::uint_least32_t extra_location_key = MurMur3::calcHash("extra_location");
        static constexpr std::uint_least32_t extra_type_key = MurMur3::calcHash("extra_type");
        static constexpr std::uint_least32_t extra_era_key = MurMur3::calcHash("extra_era");
        static constexpr std::uint_least32_t extra_site_key = MurMur3::calcHash("extra_site");
        static constexpr std::uint_least32_t extra_altitude_key = MurMur3::calcHash("extra_altitude");
        static constexpr std::uint_least32_t extra_artifact_key = MurMur3::calcHash("extra_artifact");

        // データファイルで使用されているキーのハッシュ（日本語キー）
        // TODO: 英語キー対応
        static constexpr std::uint_least32_t location_hash = MurMur3::calcHash("所在地");
        static constexpr std::uint_least32_t type_hash = MurMur3::calcHash("種別");
        static constexpr std::uint_least32_t era_hash = MurMur3::calcHash("時代");
        static constexpr std::uint_least32_t site_hash = MurMur3::calcHash("立地");
        static constexpr std::uint_least32_t altitude_hash = MurMur3::calcHash("標高");
        static constexpr std::uint_least32_t artifact_hash = MurMur3::calcHash("出土遺物");

        const UILayout& ui_layout_;
        const paxs::FeatureVisibilityManager& visible_manager_;

        // 現在選択されているFeatureの情報
        const MapFeature* selected_feature_ = nullptr;
        std::string feature_name_;
        std::string feature_type_name_;
        paxs::UnorderedMap<std::uint_least32_t, std::string> extra_data_;

        // 動的に計算されるパネルの高さ
        int calculated_height_ = 0;
        int calculated_lines_ = 0;

        // 閉じるボタン
        FeatureCloseButton close_button_;

        // イベント購読（RAII対応）
        ScopedSubscription ui_layout_changed_subscription_;
        ScopedSubscription map_drag_started_subscription_;

        /// @brief パネルのY座標を計算（下部固定）
        int calculatePanelY() const {
            const int panel_height = (calculated_height_ > 0) ? calculated_height_ : ui_layout_.feature_info_panel_height;
            const int panel_bottom = paxg::Window::height() - ui_layout_.feature_info_panel_bottom_margin;
            return panel_bottom - panel_height;
        }

        /// @brief 閉じるボタンの位置を更新
        /// @brief Update close button position
        void updateCloseButtonPosition() {
            const int panel_y = calculatePanelY();
            const int close_button_size = 20;
            const int close_button_x = ui_layout_.feature_info_panel.x +
                                      ui_layout_.feature_info_panel.width - close_button_size - 5;
            const int close_button_y = panel_y + 5;

            close_button_.setPos(Vector2<int>{ close_button_x, close_button_y });
        }

        /// @brief パネルの状態を更新
        /// @brief Update panel state (positions, layout, etc.)
        void update() {
            if (!isVisible()) {
                return;
            }

            updateCloseButtonPosition();
            // 将来的に他の更新処理もここに追加可能
        }

        /// @brief テキストの必要行数を計算
        /// @brief Calculate required lines for text
        /// @param text 計算するテキスト / Text to calculate
        /// @param max_width 最大幅 / Maximum width
        /// @return 必要な行数 / Required number of lines
        static int calculateTextLines(const std::string& text, int max_width) {
            if (text.empty()) {
                return 0;
            }

            const int avg_char_width = 12;
            const int max_chars_per_line = max_width / avg_char_width;

            int line_count = 0;
            std::string remaining_text = text;

            while (!remaining_text.empty()) {
                std::size_t split_pos = max_chars_per_line;

                if (remaining_text.length() <= split_pos) {
                    line_count++;
                    break;
                }

                std::size_t last_space = remaining_text.rfind(' ', split_pos);
                std::size_t last_comma = remaining_text.rfind(',', split_pos);
                std::size_t last_jp_comma = remaining_text.rfind(reinterpret_cast<const char*>(u8"、"), split_pos);

                std::size_t actual_split = split_pos;
                if (last_space != std::string::npos && last_space > split_pos / 2) {
                    actual_split = last_space + 1;
                } else if (last_comma != std::string::npos && last_comma > split_pos / 2) {
                    actual_split = last_comma + 1;
                } else if (last_jp_comma != std::string::npos && last_jp_comma > split_pos / 2) {
                    actual_split = last_jp_comma + 3;
                }

                line_count++;
                remaining_text = remaining_text.substr(actual_split);
            }

            return line_count;
        }

        /// @brief パネルに必要な行数を計算
        /// @brief Calculate required lines for panel content
        /// @return 必要な行数 / Required number of lines
        int calculateRequiredLines() const {
            const int padding_left = 10;
            const int max_text_width = ui_layout_.feature_info_panel.width - padding_left * 2;

            int total_lines = 0;

            // タイトル（1行）
            const std::string* title_text = Fonts().getLocalesText(feature_info_domain_hash, title_key);
            if (title_text) {
                total_lines++;
            }

            // 種別
            if (!feature_type_name_.empty()) {
                const std::string* type_label = Fonts().getLocalesText(feature_info_domain_hash, label_type_key);
                if (type_label) {
                    total_lines += calculateTextLines(*type_label + " " + feature_type_name_, max_text_width);
                }
            }

            // 名前
            if (!feature_name_.empty()) {
                const std::string* name_label = Fonts().getLocalesText(feature_info_domain_hash, label_name_key);
                if (name_label) {
                    total_lines += calculateTextLines(*name_label + " " + feature_name_, max_text_width);
                }
            }

            // 追加情報
            if (!extra_data_.empty()) {
                // 空行
                total_lines++;

                // 既知のカラム
                struct KnownColumn {
                    std::uint_least32_t data_hash;
                    std::uint_least32_t label_key;
                };
                const std::array known_columns = {
                    KnownColumn(location_hash, extra_location_key),
                    KnownColumn(type_hash, extra_type_key),
                    KnownColumn(era_hash, extra_era_key),
                    KnownColumn(site_hash, extra_site_key),
                    KnownColumn(altitude_hash, extra_altitude_key),
                    KnownColumn(artifact_hash, extra_artifact_key)
                };

                for (const auto& col : known_columns) {
                    const auto* const ptr = extra_data_.try_get(col.data_hash);
                    if (ptr != nullptr) {
                        const std::string* label = Fonts().getLocalesText(feature_info_domain_hash, col.label_key);
                        if (label) {
                            total_lines += calculateTextLines(*label + ": " + *ptr, max_text_width);
                        }
                    }
                }

                // 未知のカラム
                for (const auto& [hash, value] : extra_data_) {
                    bool is_known = false;
                    for (const auto& col : known_columns) {
                        if (hash == col.data_hash) {
                            is_known = true;
                            break;
                        }
                    }

                    if (!is_known && !value.empty()) {
                        total_lines += calculateTextLines(value, max_text_width);
                    }
                }
            }

            return total_lines;
        }

        /// @brief パネルの高さを計算して更新
        /// @brief Update and update panel height
        void updatePanelHeight() {
            const int line_height = 25;
            const int padding_top = 10;
            const int padding_bottom = 10;

            const int required_lines = calculateRequiredLines();
            calculated_lines_ = required_lines;
            calculated_height_ = ((required_lines + 1) * line_height) + padding_top + padding_bottom;
        }

        /// @brief 長い文字列を複数行に分割して描画
        /// @brief Draw long text with word wrapping
        /// @param font フォント / Font
        /// @param text 描画するテキスト / Text to draw
        /// @param bounds 描画領域（x, y, width, heightを含む矩形） / Drawing bounds (rect with x, y, width, height)
        /// @param line_height 行の高さ / Line height
        /// @param current_line 現在の行番号（参照渡し） / Current line number (by reference)
        /// @param color 文字色 / Text color
        void drawWrappedText(
            paxg::Font* font,
            const std::string& text,
            const paxs::Rect<int>& bounds,
            int line_height,
            int& current_line,
            const paxg::Color& color
        ) const {
            if (text.empty()) return;

            // 簡易的な文字列分割（文字数ベース）
            // UTF-8の文字数を大まかに見積もる（1文字あたり平均2.5バイト程度と仮定）
            const int avg_char_width = 12; // フォントサイズに応じた平均文字幅
            const int max_chars_per_line = bounds.width() / avg_char_width;

            std::string remaining_text = text;
            while (!remaining_text.empty()) {
                // 1行に収まる文字数を計算
                std::size_t split_pos = max_chars_per_line;

                if (remaining_text.length() <= split_pos) {
                    // 残りが全て1行に収まる
                    font->draw(
                        remaining_text.c_str(),
                        paxs::Vector2<int>{bounds.x(), bounds.y() + line_height * current_line++},
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
                    paxs::Vector2<int>{bounds.x(), bounds.y() + line_height * current_line++},
                    color
                );

                // 残りのテキストを更新
                remaining_text = remaining_text.substr(actual_split);
            }
        }

    public:
        FeatureInfoPanel(
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

            // UILayoutChangedEventを購読
            ui_layout_changed_subscription_ = EventBus::getInstance().subscribeScoped<UILayoutChangedEvent>(
                [this](const UILayoutChangedEvent& event) {
                    (void)event;
                    update();
                }
            );

            // MapViewportDragStartedEventを購読（パネル表示中のドラッグで非表示化）
            map_drag_started_subscription_ = EventBus::getInstance().subscribeScoped<MapViewportDragStartedEvent>(
                [this](const MapViewportDragStartedEvent& event) {
                    (void)event;
                    // ドラッグ開始時、パネルが表示されていれば非表示にする
                    if (isVisible()) {
                        EventBus::getInstance().publish(FeatureDeselectedEvent());
                    }
                }
            );

            // 閉じるボタンのコールバックを設定
            close_button_.setOnClick([]() {
                EventBus::getInstance().publish(FeatureDeselectedEvent());
            });
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible()) {
                return EventHandlingResult::NotHandled();
            }

            // 閉じるボタンのヒット判定とイベント処理
            if (close_button_.isHit(event.pos)) {
                return close_button_.handleEvent(event);
            }

            return EventHandlingResult::NotHandled();
        }

        void render() const override {
            if (!isVisible() || selected_feature_ == nullptr) {
                return;
            }

            paxg::Font* font = Fonts().getFont(paxs::FontProfiles::koyomi);
            if (font == nullptr) {
                return;
            }

            font->setOutline(0, 0.6, paxg::Color(243, 243, 243));

            // パネルの動的な位置を計算
            const int line_height = 25;
            const int padding_top = 10;
            const int padding_left = 10;
            const int panel_y = calculatePanelY();

            const int text_x = ui_layout_.feature_info_panel.x + padding_left;
            const int text_y = panel_y + padding_top;
            const int max_text_width = ui_layout_.feature_info_panel.width - padding_left * 2;

            // 描画領域を定義
            const paxs::Rect<int> text_bounds(text_x, text_y, max_text_width, 0);

            int current_line = 0;

            // タイトル
            const std::string* title_text = Fonts().getLocalesText(feature_info_domain_hash, title_key);
            if (title_text) {
                font->draw(
                    *title_text,
                    paxs::Vector2<int>{text_x, text_y + line_height * current_line++},
                    paxg::Color(0, 0, 0)
                );
            }

            // 種別
            if (!feature_type_name_.empty()) {
                const std::string* type_label = Fonts().getLocalesText(feature_info_domain_hash, label_type_key);
                if (type_label) {
                    drawWrappedText(
                        font,
                        *type_label + " " + feature_type_name_,
                        text_bounds,
                        line_height,
                        current_line,
                        paxg::Color(0, 0, 0)
                    );
                }
            }

            // 名前
            if (!feature_name_.empty()) {
                const std::string* name_label = Fonts().getLocalesText(feature_info_domain_hash, label_name_key);
                if (name_label) {
                    drawWrappedText(
                        font,
                        *name_label + " " + feature_name_,
                        text_bounds,
                        line_height,
                        current_line,
                        paxg::Color(0, 0, 0)
                    );
                }
            }

            // 追加情報（extra_data）を表示
            if (!extra_data_.empty()) {
                // 空行を追加
                current_line++;

                // 既知のカラムを特定の順序で表示
                struct KnownColumn {
                    std::uint_least32_t data_hash;  // データファイルのキーハッシュ
                    std::uint_least32_t label_key;  // Localesのラベルキー
                };
                const std::array known_columns = {
                    KnownColumn(location_hash, extra_location_key),
                    KnownColumn(type_hash, extra_type_key),
                    KnownColumn(era_hash, extra_era_key),
                    KnownColumn(site_hash, extra_site_key),
                    KnownColumn(altitude_hash, extra_altitude_key),
                    KnownColumn(artifact_hash, extra_artifact_key)
                };

                for (const auto& col : known_columns) {
                    const auto* const ptr = extra_data_.try_get(col.data_hash);
                    if (ptr != nullptr) {
                        const std::string* label = Fonts().getLocalesText(feature_info_domain_hash, col.label_key);
                        if (label) {
                            const std::string display_text = *label + ": " + *ptr;
                            drawWrappedText(
                                font,
                                display_text,
                                text_bounds,
                                line_height,
                                current_line,
                                paxg::Color(0, 0, 0)
                            );
                        }
                    }
                }

                // 未知のカラムを表示（既知のもの以外）
                for (const auto& [hash, value] : extra_data_) {
                    bool is_known = false;
                    for (const auto& col : known_columns) {
                        if (hash == col.data_hash) {
                            is_known = true;
                            break;
                        }
                    }

                    if (!is_known && !value.empty()) {
                        // ハッシュキーしかわからないので、値だけ表示
                        drawWrappedText(
                            font,
                            value,
                            text_bounds,
                            line_height,
                            current_line,
                            paxg::Color(100, 100, 100)
                        );
                    }
                }
            }

            // 閉じるボタンを描画
            close_button_.render();
        }

        Rect<int> getRect() const override {
            // 動的に計算された高さを使用
            // 下部を固定し、上に伸びるように計算
            const int panel_height = (calculated_height_ > 0) ? calculated_height_ : ui_layout_.feature_info_panel_height;
            const int panel_y = calculatePanelY();

            return Rect<int>(
                ui_layout_.feature_info_panel.x,
                panel_y,
                ui_layout_.feature_info_panel.width,
                panel_height
            );
        }

        bool isHit(const paxs::Vector2<int>& pos) const override {
            if (!isVisible()) {
                return false;
            }

            // 閉じるボタンのヒット判定
            return close_button_.isHit(paxs::Vector2<int>(pos.x, pos.y));
        }

        bool isVisible() const override {
            // 地図が表示されていて、かつFeatureが選択されている場合のみ表示
            return visible_manager_.isVisible(ViewMenu::map) && (selected_feature_ != nullptr);
        }

        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
        const char* getName() const override { return "FeatureInfoPanel"; }
        void setPos(const Vector2<int>& /*pos*/) override {}

    private:
        /// @brief FeatureSelectedEventのハンドラー
        /// @brief Handler for FeatureSelectedEvent
        void onFeatureSelected(const FeatureSelectedEvent& event) {
            selected_feature_ = event.feature;
            if (selected_feature_ == nullptr) {
                return;
            }

            // Featureの名前を取得
            feature_name_ = selected_feature_->getName();

            // Featureの種別名を取得
            switch (selected_feature_->getType()) {
                case FeatureType::Geographic: {
                    const std::string* type_text = Fonts().getLocalesText(feature_info_domain_hash, type_geographic_key);
                    feature_type_name_ = (type_text != nullptr) ? *type_text : "";
                    break;
                }
                case FeatureType::PlaceName: {
                    const std::string* type_text = Fonts().getLocalesText(feature_info_domain_hash, type_place_name_key);
                    feature_type_name_ = (type_text != nullptr) ? *type_text : "";
                    break;
                }
                case FeatureType::Genome: {
                    const std::string* type_text = Fonts().getLocalesText(feature_info_domain_hash, type_genome_key);
                    feature_type_name_ = (type_text != nullptr) ? *type_text : "";
                    break;
                }
                case FeatureType::Person: {
                    const std::string* type_text = Fonts().getLocalesText(feature_info_domain_hash, type_person_key);
                    feature_type_name_ = (type_text != nullptr) ? *type_text : "";
                    break;
                }
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

            // パネルの高さを計算
            updatePanelHeight();

            // パネルの状態を更新
            update();
        }

        /// @brief FeatureDeselectedEventのハンドラー
        /// @brief Handler for FeatureDeselectedEvent
        void onFeatureDeselected(const FeatureDeselectedEvent& /*event*/) {
            selected_feature_ = nullptr;
            feature_name_.clear();
            feature_type_name_.clear();
            extra_data_.clear();

            // パネルの状態を更新
            update();
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_feature_info_panel_HPP
