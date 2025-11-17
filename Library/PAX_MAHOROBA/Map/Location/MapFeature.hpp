/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_FEATURE_HPP
#define PAX_MAHOROBA_MAP_FEATURE_HPP

#include <cstdint>
#include <string>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

#include <PAX_MAHOROBA/Map/Location/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Location/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Location/RenderContext.hpp>

#include <PAX_SAPIENTICA/Geography/Coordinate/WrappedScreenPositions.hpp>

namespace paxs {

/// @brief 地物の基底クラス - すべての地物はこのクラスを継承する
/// @brief Base class for map features - all features inherit from this class
class MapFeature {
public:
    virtual ~MapFeature() = default;

    // ========== 基本情報 / Basic Information ==========

    /// @brief 地物の種類を取得
    /// @brief Get feature type
    /// @return 地物の種類 / Feature type
    virtual FeatureType getType() const = 0;

    /// @brief 地物のIDを取得（keyのMurMur3ハッシュ値）
    /// @brief Get feature ID (MurMur3 hash of key)
    /// @return 地物のID（ハッシュ値） / Feature ID (hash value)
    virtual std::uint_least32_t getId() const = 0;

    /// @brief 地物の名前を取得
    /// @brief Get feature name
    /// @param language 言語コード（デフォルト: "ja-JP"） / Language code (default: "ja-JP")
    /// @return 地物の名前 / Feature name
    virtual std::string getName(const std::string& language = "ja-JP") const = 0;

    /// @brief 地物の種別ハッシュを取得
    /// @brief Get feature type hash identifier
    /// @return 地物の種別ハッシュ値 / Feature type hash
    /// @note feature_type_hash = 地物の種別を識別するハッシュ値（Location Point Entity）
    /// @note 例: "青銅器", "古墳", "agent1" などの文字列のハッシュ値
    /// @note Example: Hash of strings like "青銅器" (bronze), "古墳" (tomb), "agent1"
    virtual std::uint_least32_t getFeatureTypeHash() const = 0;

    // ========== 状態管理 / State Management ==========

    /// @brief 状態を更新（座標計算、キャッシュ更新など）
    /// @brief Update state (coordinate calculation, cache update, etc.)
    /// @param context 描画コンテキスト / Render context
    virtual void update(const RenderContext& context) = 0;

    /// @brief 可視性を取得
    /// @brief Get visibility
    /// @return 可視ならtrue / True if visible
    virtual bool isVisible() const = 0;

    /// @brief 指定された時間範囲内にあるかチェック
    /// @brief Check if feature is within specified time range
    /// @param jdn ユリウス日 / Julian Day Number
    /// @return 範囲内ならtrue / True if within range
    virtual bool isInTimeRange(double jdn) const = 0;

    // ========== 座標・描画 / Coordinates & Rendering ==========

    /// @brief 経度ラップされたスクリーン座標を取得
    /// @brief Get longitude-wrapped screen positions
    /// @return 経度ラップされた3つのスクリーン座標 / Three wrapped screen positions
    virtual const WrappedScreenPositions& getScreenPositions() const = 0;

    /// @brief 表示サイズを取得
    /// @brief Get display size
    /// @return 表示サイズ（ピクセル単位） / Display size in pixels
    virtual int getDisplaySize() const = 0;

    // ========== ヒット判定 / Hit Testing ==========

    /// @brief マウス座標が地物にヒットするかチェック
    /// @brief Check if mouse position hits this feature
    /// @param mouse_pos マウス座標 / Mouse position
    /// @return ヒットしたらtrue / True if hit
    virtual bool isHit(const paxs::Vector2<int>& mouse_pos) const = 0;

    // ========== イベント処理 / Event Handling ==========

    /// @brief クリックイベント処理
    /// @brief Handle click event
    /// @param context クリックコンテキスト / Click context
    virtual void onClick(const ClickContext& context) = 0;

    /// @brief ホバーイベント処理
    /// @brief Handle hover event
    /// @param context クリックコンテキスト / Click context
    virtual void onHover(const ClickContext& context) {
        // デフォルト実装は何もしない / Default implementation does nothing
        (void)context;
    }

protected:
    bool visible_ = true;           ///< 可視性 / Visibility
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_FEATURE_HPP
