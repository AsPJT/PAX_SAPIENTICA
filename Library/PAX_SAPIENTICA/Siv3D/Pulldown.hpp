/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_PULLDOWN_HPP
#define PAX_SAPIENTICA_SIV3D_PULLDOWN_HPP

/*##########################################################################################

##########################################################################################*/

#include <limits>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>
#include <PAX_SAPIENTICA/Siv3D/Language.hpp>
#include <PAX_SAPIENTICA/TouchManager.hpp>

namespace paxs {

    enum class PulldownType : int {
        Empty,
        Zero, // 項目も選択 (0)
        One // 1 から選択 (1)
    };

    class Pulldown {
    public:
        using Languages = std::vector<std::string>; // 単語の各言語の書き方一覧（リンゴ Apple など ）

        Pulldown() = default;
        // X を指定したサイズに変更
        void setRectX(const std::size_t x = 0) {
            rect.x = s3d::Rect::value_type(x);
        }
        // 言語変更による更新処理
        void updateLanguage() {
            item_size = 0;
            rect.w = 0;
            for (std::size_t i = start_index; i < itemsa.size(); ++i) {
                const auto& item_front = itemsa[i].front();
                const auto& item = itemsa[i][language_index + start_language_index];
                if (item_front.size() == 0) break;
                if (item_front[0] == '>') break; // テキストファイルの項目名を除く
                if (item_size != (std::numeric_limits<std::size_t>::max)()) {
                    ++item_size; // 各項目にある選択肢を数える
                }
                // 最大の文字数からプルダウンの各項目の幅を定義
                rect.w = s3d::Max(rect.w, static_cast<s3d::int32>(font.front()(s3d::Unicode::FromUTF8(item)).region().w));
            }
            // プルダウンの
            rect.w += (padding.x * 2 + down_button_size);
        }
        // コンストラクタ
        Pulldown(
            const std::vector<Languages>& items_, // 表示するもの
            const std::size_t start_index_, // 単語の要素番号
            const std::size_t start_language_index_, // 言語の要素番号
            const std::vector<s3d::Font>& font_,
            const s3d::Point& pos_ = { 0,0 },
            PulldownType pdt_ = PulldownType::Zero, // プルダウンの種別
            const bool is_one_font_ = false)
            :
            start_index{ start_index_ }
            , start_language_index{ start_language_index_ }
            , font{ font_ }
            , itemsa{ items_ }
            , rect{ pos_, 0, (font.front().height() + padding.y * 2) }
            , pdt(pdt_)
            , is_one_font(is_one_font_) {
            updateLanguage();
            is_items.resize(item_size);
            for (auto&& is_item : is_items) {
                is_item = true;
            }
        }
        // からか判定
        bool isEmpty() const {
            return itemsa.empty();
        }

        // 更新処理
        void update(const SelectLanguage& select_language, paxs::TouchManager& tm_) {
            if (isEmpty()) return;
            // 言語が変わっていたら更新処理
            if (language_index != select_language.cget()) {
                language_index = select_language.cget();
                updateLanguage();
            }
            // 左クリックで開く
            if (tm_.get(rect.leftClicked())) {
                is_open = (not is_open);
            }
            s3d::Point pos = rect.pos.movedBy(0, rect.h);
            if (is_open) { // 項目が開かれている場合
                // その項目の選択肢
                for (auto i : s3d::step(itemsa.size())) {
                    const s3d::Rect rect_tmp{ pos, rect.w, rect.h };
                    if (tm_.get(rect_tmp.leftClicked())) { // もし選択肢が左クリックされていたら
                        if (i < is_items.size()) {
                            // 項目をオンオフさせる
                            index = i;
                            is_items[i] = !(is_items[i]);
                            is_open = false;
                            break;
                        }
                    }
                    pos.y += rect.h;
                }
            }
        }
        // 描画
        void draw() const {
            const std::size_t item_index = index + start_index;
            rect.draw(s3d::Color{ 243, 243, 243 }); // プルダウンの背景を描画
            if (isEmpty())return;
            // プルダウンのふちを描画
            rect.drawFrame(1, 0, is_open ? s3d::Palette::Orange : s3d::Palette::Gray);
            s3d::Point pos = rect.pos;

            const std::size_t select_index = ((is_one_font) ? item_index : language_index);
            // 種別によって描画処理を変える
            switch (pdt) {
            case paxs::PulldownType::Empty:break;
            case paxs::PulldownType::Zero:
                // 文字を描画
                font[select_index](s3d::Unicode::FromUTF8(
                    (language_index + start_language_index < itemsa[item_index].size()) ? itemsa[item_index][language_index + start_language_index] : itemsa[item_index][start_language_index]
                )).draw(pos + padding, s3d::Palette::Black);
                break;
            case paxs::PulldownType::One:
                // 文字を描画
                font[language_index](s3d::Unicode::FromUTF8(
                    (language_index + start_language_index < itemsa[item_index].size()) ? itemsa[start_index][language_index + start_language_index] : itemsa[item_index][start_language_index]
                )).draw(pos + padding, s3d::Palette::Black);
                break;
            default:break;
            }
            // 三角形を描画
            s3d::Triangle{ (rect.x + rect.w - down_button_size / 2.0 - padding.x), (rect.y + rect.h / 2.0),
                (down_button_size * 0.5), 3.1416 }.draw(s3d::Palette::Black);
            pos.y += rect.h;
            if (is_open) {
                // 四角形を描画
                const s3d::Rect backRect{ pos, rect.w, (rect.h* item_size/*itemsa.size()*/)};
                // 影を描画
                backRect.drawShadow({ 1, 1 }, 4, 1).draw();
                //for (const auto& item : items) {
                std::size_t i = start_index;
                switch (pdt) {
                case paxs::PulldownType::Empty:break;
                case paxs::PulldownType::Zero:i = start_index; break; // 0 から始める
                case paxs::PulldownType::One:i = start_index + 1; break; // 1 から始める
                default:break;
                }
                for (; i < itemsa.size(); ++i) {
                    if (itemsa[i].front().size() == 0) break;
                    if (itemsa[i].front()[0] == '>') break;
                    const s3d::Rect rect_tmp{ pos, rect.size };
                    if (rect_tmp.mouseOver()) { // マウスカーソルが四角形の上にある場合
                        // 四角形の色を変える
                        rect_tmp.draw(s3d::Palette::Skyblue);
                    }
                    const std::size_t select_index2 = ((is_one_font) ? i : language_index);
                    // 文字を描画
                    font[select_index2](s3d::Unicode::FromUTF8(
                        (language_index + start_language_index < itemsa[i].size()) ? itemsa[i][language_index + start_language_index] : itemsa[i][start_language_index]
                    )).draw((pos + padding), s3d::Palette::Black);
                    pos.y += rect.h;
                }
                // ふちを描画
                backRect.drawFrame(1, 0, s3d::Palette::Gray);
            }
        }

        void setPos(const s3d::Point& pos) { rect.setPos(pos); }
        const s3d::Rect& getRect() const { return rect; }
        size_t getIndex() const { return index + start_index; }
        bool getIsItems(std::size_t i) const {
            if (i < is_items.size()) return is_items[i];
            return is_items.front();
        }
        std::string getItem() const {
            if (isEmpty()) return{};
            return itemsa[index + start_index].front();
        }

    private:
        std::size_t item_size = 0;
        std::size_t start_index = 0; // 単語の要素番号の開始地点
        std::size_t start_language_index = 0; // 言語の要素番号の開始地点
        std::size_t language_index = 0; // 言語の要素番号
        std::vector<s3d::Font> font;
        std::vector<Languages> itemsa; // プルダウンに使用されている単語一覧
        std::vector<bool> is_items;
        size_t index = 0;
        s3d::Size padding{ 6, 2 };
        s3d::Rect rect;
        s3d::int32 down_button_size = 20;
        bool is_open = false;
        PulldownType pdt;
        bool is_one_font = false;
    };
    // メニューバーを管理
    class MenuBar {
    public:

        using Languages = std::vector<std::string>; // 単語の各言語の書き方一覧（リンゴ Apple など ）

        void add(
            const std::vector<Languages>& menu_bar_pulldown,
            const std::size_t start_index,
            const std::size_t start_language_index,
            const std::vector<s3d::Font>& font_menu_bar) {

            if (pdv.size() != 0) {
                start_x += pdv.back().getRect().w;
            }
            pdv.emplace_back(paxs::Pulldown(
                menu_bar_pulldown,
                start_index,
                start_language_index,
                font_menu_bar,
                s3d::Point{ start_x, 0 },
                paxs::PulldownType::One));
        }

        void update(const SelectLanguage& select_language, paxs::TouchManager& tm_) {
            start_x = 0;
            for (auto& pd : pdv) {
                pd.update(select_language, tm_);
                pd.setRectX(start_x);
                start_x += pd.getRect().w;
            }
        }
        void draw() {
            for (auto& pd : pdv) {
                pd.draw();
            }
        }

        paxs::Pulldown& getPulldown(const std::size_t i) {
            return pdv[i];
        }
        const paxs::Pulldown& cgetPulldown(const std::size_t i) const {
            return pdv[i];
        }

    private:
        std::vector<paxs::Pulldown> pdv;
        std::size_t start_x = 0;
    };

}

#endif // !PAX_SAPIENTICA_SIV3D_PULLDOWN_HPP
