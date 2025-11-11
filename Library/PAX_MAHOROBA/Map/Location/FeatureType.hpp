/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_FEATURE_TYPE_HPP
#define PAX_MAHOROBA_FEATURE_TYPE_HPP

namespace paxs {

/// @brief 地物の種類を表すEnum
/// @brief Feature type enumeration
enum class FeatureType {
    Person,           ///< 人物（肖像画+名前） / Person (portrait + name)
    PlaceName,        ///< 地名（テキストのみ） / Place name (text only)
    Geographic,       ///< 地理的地物（アイコン） / Geographic feature (icon)
    Settlement,       ///< 集落 / Settlement
    Model3D,          ///< 3Dモデル（古墳の石室など） / 3D model (burial chamber, etc.)
    Custom            ///< カスタム地物 / Custom feature
};

} // namespace paxs

#endif // !PAX_MAHOROBA_FEATURE_TYPE_HPP
