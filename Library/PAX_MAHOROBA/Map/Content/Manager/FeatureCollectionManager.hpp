/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_FEATURE_COLLECTION_MANAGER_HPP
#define PAX_MAHOROBA_FEATURE_COLLECTION_MANAGER_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include <PAX_MAHOROBA/Map/Content/Feature/FlowCurveFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/GenomeFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/GeographicFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/PersonFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/PlaceNameFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/TerritoryFeature.hpp>
#include <PAX_MAHOROBA/Map/Core/MapAssetRegistry.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Map/Repository/FlowCurveRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/GenomeRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/GeographicFeatureRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/PersonRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/PlaceNameRepository.hpp>
#include <PAX_SAPIENTICA/Map/Repository/TerritoryRepository.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>

namespace paxs {

    /// @brief 地物コレクション管理クラス
    /// @brief Feature collection manager class
    ///
    /// 責務:
    /// - features_の管理
    /// - データ読み込み（Person, Geographic, PlaceName, Genome, Territory, FlowCurve）
    /// - Feature検索（ID検索、座標検索）
    /// - asset_registry_の管理
    class FeatureCollectionManager {
    private:
        std::vector<std::unique_ptr<MapFeature>> features_; ///< 地物のコレクション / Collection of features
        MapAssetRegistry asset_registry_; ///< 地図アイコンアセットレジストリ / Map icon asset registry

        /// @brief 人物データを読み込み
        void loadPersonFeatures() {
            // テクスチャを読み込み（Portraitsの設定を使用）
            const std::string portraits_path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("Portraits"));

            if (portraits_path.size() > 0) {
                asset_registry_.loadPersonIcons(portraits_path);
            }

            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_person_count = 1000;
            features_.reserve(current_capacity + estimated_person_count);

            // PersonFeatureに変換
            auto all_persons = PersonRepository::loadPersonList();
            for (const auto& [person_data, person_list] : all_persons) {
                features_.emplace_back(
                    std::make_unique<PersonFeature>(person_data, person_list)
                );
            }
        }

        /// @brief 地理的地物データを読み込み
        void loadGeographicFeatures() {
            // 容量を事前確保（地理的地物は多いため5000を確保）
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_geographic_count = 5000;
            features_.reserve(current_capacity + estimated_geographic_count);

            // GeographicFeatureに変換
            auto all_geographic_features = GeographicFeatureRepository::loadGeographicFeatureList();
            for (auto& location_data : all_geographic_features) {
                features_.emplace_back(
                    std::make_unique<GeographicFeature>(std::move(location_data))
                );
            }
        }

        /// @brief 地名データを読み込み
        void loadPlaceNameFeatures() {
            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_place_name_count = 1000;
            features_.reserve(current_capacity + estimated_place_name_count);

            // PlaceNameFeatureに変換
            auto all_place_names = PlaceNameRepository::loadPlaceNameList();
            for (auto& location_data : all_place_names) {
                features_.emplace_back(
                    std::make_unique<PlaceNameFeature>(std::move(location_data))
                );
            }
        }

        /// @brief ゲノムデータを読み込み
        void loadGenomeFeatures() {
            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_genome_count = 500;
            features_.reserve(current_capacity + estimated_genome_count);

            // GenomeFeatureに変換
            auto all_genomes = GenomeRepository::loadGenomeList();
            for (auto& genome_data : all_genomes) {
                features_.emplace_back(
                    std::make_unique<GenomeFeature>(std::move(genome_data))
                );
            }
        }

        /// @brief 領域データを読み込み
        /// @brief Load territory features
        void loadTerritoryFeatures() {
            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_territory_count = 100;
            features_.reserve(current_capacity + estimated_territory_count);

            // TerritoryFeatureに変換
            auto all_territories = TerritoryRepository::loadTerritoryList();
            for (const auto& [territory_data, territory_group] : all_territories) {
                features_.emplace_back(
                    std::make_unique<TerritoryFeature>(territory_data, territory_group)
                );
            }
        }

        /// @brief フロー曲線データを読み込み
        /// @brief Load flow curve features
        void loadFlowCurveFeatures() {
            // 容量を事前確保
            const std::size_t current_capacity = features_.capacity();
            const std::size_t estimated_flow_arrow_count = 100;
            features_.reserve(current_capacity + estimated_flow_arrow_count);

            // FlowCurveFeatureに変換
            auto all_flow_arrows = FlowCurveRepository::loadFlowCurveList();
            for (const auto& [flow_arrow_data, flow_arrow_group] : all_flow_arrows) {
                features_.emplace_back(
                    std::make_unique<FlowCurveFeature>(flow_arrow_data, flow_arrow_group)
                );
            }
        }

    public:
        FeatureCollectionManager() = default;
        ~FeatureCollectionManager() = default;

        // コピー・ムーブ禁止（unique_ptrを持つため）
        FeatureCollectionManager(const FeatureCollectionManager&) = delete;
        auto operator=(const FeatureCollectionManager&)->FeatureCollectionManager & = delete;
        FeatureCollectionManager(FeatureCollectionManager&&) = delete;
        auto operator=(FeatureCollectionManager&&)->FeatureCollectionManager & = delete;

        /// @brief 全データを読み込み
        void loadAllFeatures() {
            features_.clear();

            // MapIconsを1回だけロード（地理・ゲノム共用）
            const std::string map_icons_path = AppConfig::getInstance().getSettingPath(MurMur3::calcHash("MapIcons"));
            if (map_icons_path.size() > 0) {
                asset_registry_.loadMapIcons(map_icons_path);
            }

            // 描画順序: 領域 → フロー曲線 → 地名 → ゲノム → 地理 → 人物
            // Territories are drawn first (background layer), then flow curves
            loadTerritoryFeatures();
            // std::cout << "Territory features loaded: " << features_.size() << "\n";

            loadFlowCurveFeatures();
            // std::cout << "Flow curve features loaded: " << features_.size() << " total\n";

            loadPlaceNameFeatures();
            // std::cout << "Place name features loaded: " << features_.size() << " total\n";

            loadGenomeFeatures();
            // std::cout << "Genome features loaded: " << features_.size() << " total\n";

            loadGeographicFeatures();
            // std::cout << "Geographic features loaded: " << features_.size() << " total\n";

            loadPersonFeatures();
            // std::cout << "Person features loaded: " << features_.size() << " total\n";

            // 全アイコンをマージ
            asset_registry_.mergeCategories();
        }

        /// @brief 地物コレクションへの参照を取得
        const std::vector<std::unique_ptr<MapFeature>>& getFeatures() const {
            return features_;
        }

        /// @brief 地物コレクションへの参照を取得（非const）
        std::vector<std::unique_ptr<MapFeature>>& getFeatures() {
            return features_;
        }

        /// @brief アセットレジストリへの参照を取得
        const MapAssetRegistry& getAssetRegistry() const {
            return asset_registry_;
        }

        /// @brief IDでFeatureを検索
        MapFeature* findFeatureById(std::uint_least32_t id) {
            for (auto& feature : features_) {
                if (feature && feature->getId() == id) {
                    return feature.get();
                }
            }
            return nullptr;
        }

        /// @brief マウス座標でFeatureを検索（入力処理用）
        MapFeature* findFeatureAt(const paxs::Vector2<int>& mouse_pos) {
            for (auto& feature : features_) {
                if (feature && feature->isVisible() && feature->isHit(mouse_pos)) {
                    return feature.get();
                }
            }
            return nullptr;
        }
    };
}

#endif // !PAX_MAHOROBA_FEATURE_COLLECTION_MANAGER_HPP
