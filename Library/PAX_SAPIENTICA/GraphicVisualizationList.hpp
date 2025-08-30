/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GRAPHIC_VISUALIZATION_LIST_HPP
#define PAX_SAPIENTICA_GRAPHIC_VISUALIZATION_LIST_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>
#include <unordered_map>

#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    // GUI 上で可視化する項目を管理
    class GraphicVisualizationList {
    private:
        std::unordered_map<std::uint_least32_t, bool> visible{}; // 可視化するかしないかを持つ辞書

    public:
        // 新しい項目を追加
        void emplace(const std::uint_least32_t& str_, const bool visible_) {
            visible.emplace(str_, visible_);
        }
        // 指定した要素の符号を反転
        void setNot(const std::uint_least32_t& str_) {
            visible[str_] = !visible[str_];
        }
        // 指定した要素の符号を設定
        void set(const std::uint_least32_t& str_, const bool visible_) {
            visible[str_] = visible_;
        }
        // 指定した要素の符号を取得
        bool operator[](const std::uint_least32_t& str_) const {
            return (visible.find(str_) != visible.end()) ? visible.at(str_) : true;
        }

    };

}

#endif // !PAX_SAPIENTICA_GRAPHIC_VISUALIZATION_LIST_HPP
