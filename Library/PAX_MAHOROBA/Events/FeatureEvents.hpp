/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_EVENTS_FEATURE_EVENTS_HPP
#define PAX_MAHOROBA_EVENTS_FEATURE_EVENTS_HPP

#include <PAX_MAHOROBA/Map/Location/MapFeature.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>

namespace paxs {
    /// @brief Feature がクリックされた時のイベント
    /// @brief Event when a Feature is clicked
    struct FeatureSelectedEvent : public Event {
        const MapFeature* feature = nullptr;
        explicit FeatureSelectedEvent(const MapFeature* feature_)
            : feature(feature_) {}
    };

    /// @brief Feature の選択が解除された時のイベント
    /// @brief Event when a Feature is deselected
    struct FeatureDeselectedEvent : public Event {
        // 現時点では空でOK
        // Currently empty - may add fields in the future
    };
}

#endif // !PAX_MAHOROBA_EVENTS_FEATURE_EVENTS_HPP
