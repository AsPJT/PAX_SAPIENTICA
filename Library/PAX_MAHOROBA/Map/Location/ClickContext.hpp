/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CLICK_CONTEXT_HPP
#define PAX_MAHOROBA_CLICK_CONTEXT_HPP

#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxs {

// 前方宣言
class AppStateManager;
class EventBus;

/// @brief クリックコンテキスト - クリック時に必要なパラメータをまとめた構造体
/// @brief Click context - Structure containing parameters needed for click handling
struct ClickContext {
    paxg::Vec2i mouse_pos;         ///< マウス座標 / Mouse position
    AppStateManager* app_state_manager = nullptr;  ///< アプリケーション状態管理 / App state manager
    EventBus* event_bus = nullptr; ///< イベントバス / Event bus
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CLICK_CONTEXT_HPP
