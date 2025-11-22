/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_UPDATABLE_HPP
#define PAX_MAHOROBA_I_UPDATABLE_HPP

#include <PAX_MAHOROBA/Map/Location/UpdateContext.hpp>

namespace paxs {

/// @brief 空間的更新が可能な地物
/// @brief Spatially updatable feature
/// @details ビューポートやズームレベルが変更された時に、スクリーン座標変換や範囲チェックを行う地物が実装するインターフェース
/// @details Interface for features that need to update when viewport or zoom level changes (screen coordinate transformation, bounds checking)
class ISpatiallyUpdatable {
public:
    virtual ~ISpatiallyUpdatable() = default;

    /// @brief 空間的パラメータ（ビューポート、ズーム）が変更された時の更新
    /// @brief Update when spatial parameters (viewport, zoom) change
    /// @param context 空間的コンテキスト / Spatial context
    /// @details 以下の処理を実行する:
    /// @details - スクリーン座標変換（経度ラップ処理含む）
    /// @details - ビューポート範囲チェック
    /// @details - ズームレベルフィルタリング
    /// @details - 表示サイズ計算
    /// @details - テクスチャサイズ取得
    virtual void updateSpatial(const SpatialContext& context) = 0;
};

/// @brief 時間的更新が可能な地物
/// @brief Temporally updatable feature
/// @details 日付が変更された時に、時間補間や時間範囲チェックを行う地物が実装するインターフェース
/// @details Interface for features that need to update when date changes (time interpolation, time range checking)
class ITemporallyUpdatable {
public:
    virtual ~ITemporallyUpdatable() = default;

    /// @brief 時間的パラメータ（日付）が変更された時の更新
    /// @brief Update when temporal parameters (date) change
    /// @param context 時間的コンテキスト / Temporal context
    /// @details 以下の処理を実行する:
    /// @details - 時間補間座標の計算（PersonFeatureなど移動する地物）
    /// @details - 時間範囲チェック
    virtual void updateTemporal(const TemporalContext& context) = 0;
};

/// @brief ローカライゼーション可能な地物
/// @brief Localizable feature
/// @details 言語やフォントが変更された時に、名前の再取得やフォント幅計算を行う地物が実装するインターフェース
/// @details Interface for features that need to update when language or font changes (name retrieval, font width calculation)
class ILocalizable {
public:
    virtual ~ILocalizable() = default;

    /// @brief ローカライゼーションパラメータ（言語、フォント）が変更された時の更新
    /// @brief Update when localization parameters (language, font) change
    /// @param context ローカライゼーションコンテキスト / Localization context
    /// @details 以下の処理を実行する:
    /// @details - 名前の再取得（Localesシステムから）
    /// @details - フォント幅・高さの計算
    /// @details - テキストサイズのキャッシュ更新
    virtual void updateLocalization(const LocalizationContext& context) = 0;
};

} // namespace paxs

#endif // !PAX_MAHOROBA_I_UPDATABLE_HPP
