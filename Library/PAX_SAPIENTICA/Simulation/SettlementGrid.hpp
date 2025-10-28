/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SETTLEMENT_GRID_HPP
#define PAX_SAPIENTICA_SETTLEMENT_GRID_HPP

#include <memory>

#include <PAX_SAPIENTICA/Simulation/Settlement.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>

namespace paxs {

    /// @brief 特定のグリッドに属する集落を管理するクラス
    class SettlementGrid {
    public:
        using Vector2 = paxs::Vector2<GridType>;

        SettlementGrid() = default;

        explicit SettlementGrid(const Vector2& grid_position, std::shared_ptr<Environment> environment,
            std::mt19937& gen_) noexcept : grid_position(grid_position), environment(environment), gen(&gen_) {}

        /// @brief Add a settlement to the grid.
        /// @brief 集落をグリッドに追加
        void addSettlement(const Settlement& settlement) noexcept { settlements.emplace_back(settlement); }

        /// @brief Move a settlement to this grid.
        /// @brief 集落をこのグリッドに移動
        void moveSettlementToThis(Settlement& settlement) noexcept {
            // 他の集落とかぶらない位置を探す
            // ブラックリスト
            std::vector<Vector2> black_list(settlements.size());
#ifdef _OPENMPa
            const int settlement_size = static_cast<int>(settlements.size());
#pragma omp parallel for
            for (int i = 0; i < settlement_size; ++i) {
                black_list[i] = settlements[i].getPosition();
            }
#else
            for (std::size_t i = 0; i < settlements.size(); ++i) {
                black_list[i] = settlements[i].getPosition();
            }
#endif

            // ランダムな位置を探す
            std::uniform_int_distribution<> dis_x(grid_position.x, grid_position.x + paxs::SimulationConstants::getInstance()->cell_group_length - 1);
            std::uniform_int_distribution<> dis_y(grid_position.y, grid_position.y + paxs::SimulationConstants::getInstance()->cell_group_length - 1);
            Vector2 position;

            while (black_list.size() < SimulationConstants::getInstance()->cell_group_length * SimulationConstants::getInstance()->cell_group_length) {
                position.x = dis_x(*gen);
                position.y = dis_y(*gen);
                if (std::find(black_list.begin(), black_list.end(), position) == black_list.end()) {
                    if (environment->isLive(position)) {
                        // 居住可能
                        break;
                    } else {
                        // 居住不可
                        black_list.emplace_back(position);
                    }
                }
            }

            // 集落を移動
            if (black_list.size() == SimulationConstants::getInstance()->cell_group_length * SimulationConstants::getInstance()->cell_group_length) {
                // 居住可能な場所がない
                PAXS_WARNING("No place to live.");
                return; // 集落を追加しない
            }

            settlement.setPosition(position);
            settlements.emplace_back(settlement);
        }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        std::vector<Settlement>& getSettlements() noexcept { return settlements; }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        const std::vector<Settlement>& cgetSettlements() const noexcept { return settlements; }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        Settlement& getSettlement(const std::uint_least32_t id) noexcept {
            for (auto& settlement : settlements) {
                if (settlement.getId() == id) {
                    return settlement;
                }
            }
            const std::string message = "Settlement not found. ID: " + std::to_string(id);
            PAXS_ERROR(message);
            return settlements[0];
        }

        /// @brief Get the grid position.
        /// @brief グリッドの座標を取得
        Vector2 getGridPosition() const noexcept { return grid_position; }

        /// @brief Delete the settlement.
        /// @brief 集落を削除
        void deleteSettlement(const std::uint_least32_t id) noexcept {
            auto it = std::find_if(settlements.begin(), settlements.end(), [id](const Settlement& settlement) { return settlement.getId() == id; });
            if (it != settlements.end()) {
                (*it) = settlements.back(); // 同義 settlements.erase(it);
                settlements.pop_back();
            } else {
                const std::string message = "Settlement not found. ID: " + std::to_string(id);
                PAXS_ERROR(message);
            }
        }

        /// @brief Add a District ID to the list.
        /// @brief 地区のIDをリストに追加
        void addDistrictId(const std::uint_least8_t id) noexcept {
            // 重複チェック
            if (std::find(district_list.begin(), district_list.end(), id) == district_list.end()) {
                district_list.emplace_back(id);
            }
        }

        /// @brief Get the District list.
        /// @brief 地区のリストを取得
        std::vector<std::uint_least8_t>& getDistrictIds() noexcept { return district_list; }
        const std::vector<std::uint_least8_t>& cgetDistrictIds() const noexcept { return district_list; }

        /// @brief Check if the settlement exists and delete it if it does not.
        /// @brief 集落が存在するかどうかをチェックし、存在しない場合は削除する
        void checkSettlements() noexcept {
            settlements.erase(
                std::remove_if(
                    settlements.begin(),
                    settlements.end(),
                    [](const auto& settlement) { return settlement.getPopulation() == 0; }
                ),
                settlements.end()
            );
        }

        /// @brief Delete the agent.
        /// @brief エージェントを削除
        void deleteAgent(const HumanIndexType agent_id, const std::uint_least32_t settlement_id) noexcept {
            auto it = std::find_if(settlements.begin(), settlements.end(), [settlement_id](const Settlement& settlement) { return settlement.getId() == settlement_id; });
            if (it != settlements.end()) {
                it->deleteAgent(agent_id);
            } else {
                PAXS_ERROR("Settlement not found. ID: " + std::to_string(settlement_id));
            }
        }

        /// @brief Add the agent.
        /// @brief エージェントを追加
        void addAgent(const paxs::SettlementAgent& agent_, const std::uint_least32_t settlement_id) noexcept {
            auto it = std::find_if(settlements.begin(), settlements.end(), [settlement_id](const Settlement& settlement) { return settlement.getId() == settlement_id; });
            if (it != settlements.end()) {
                it->addAgent(agent_);
            }
            else {
                PAXS_ERROR("Settlement not found. ID: " + std::to_string(settlement_id));
            }
        }

        /// @brief Divide the settlement.
        /// @brief 集落を分割する
        void divideSettlements() noexcept {
            // 人口が最大人口を超えている集落を複数探し、分割する
            for (std::size_t i = 0; i < settlements.size(); ++i) {
                if (settlements[i].getPopulationWeight() >= 1.0) {
                    // 分割
                    Settlement divided_settlement = settlements[i].divide();
                    moveSettlementToThis(divided_settlement);
                }
            }
        }

    private:
        std::vector<Settlement> settlements{};
        std::shared_ptr<Environment> environment{};
        Vector2 grid_position{};
        std::mt19937* gen{}; // 乱数生成器
        std::vector<std::uint_least8_t> district_list{};
    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_GRID_HPP
