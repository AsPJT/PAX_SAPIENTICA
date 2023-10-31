/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SETTLEMENT_GRID_HPP
#define PAX_SAPIENTICA_SETTLEMENT_GRID_HPP

/*##########################################################################################

##########################################################################################*/

#include <memory>

#include <PAX_SAPIENTICA/Simulation/Settlement.hpp>

namespace paxs {

    /// @brief 特定のグリッドに属する集落を管理するクラス
    template <typename GridType>
    class SettlementGrid {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Settlement = paxs::Settlement<GridType>;

        explicit SettlementGrid(const Vector2& grid_position) noexcept : grid_position(grid_position) {}

        /// @brief Add a settlement to the grid.
        /// @brief 集落をグリッドに追加
        void addSettlement(const std::shared_ptr<Settlement>& settlement) noexcept { settlements.push_back(settlement); }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        std::vector<std::shared_ptr<Settlement>>& getSettlements() noexcept { return settlements; }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        const std::vector<std::shared_ptr<Settlement>>& cgetSettlements() const noexcept { return settlements; }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        std::shared_ptr<Settlement>& getSettlement(const std::uint_least32_t id) noexcept {
            for (auto& settlement : settlements) {
                if (settlement->getId() == id) {
                    return settlement;
                }
            }
            Logger logger("Save/error_log.txt");
            const std::string message = "Settlement not found. ID: " + std::to_string(id);
            logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
        }

        /// @brief Get the grid position.
        /// @brief グリッドの座標を取得
        Vector2 getGridPosition() const noexcept { return grid_position; }

    private:
        std::vector<std::shared_ptr<Settlement>> settlements;
        Vector2 grid_position;

        /// @brief Delete the settlement.
        /// @brief 集落を削除
        void deleteSettlement(const std::uint_least32_t id) noexcept {
            auto it = std::find_if(settlements.begin(), settlements.end(), [id](const std::shared_ptr<Settlement>& settlement) { return settlement->getId() == id; });
            if (it != settlements.end()) {
                settlements.erase(it);
            } else {
                Logger logger("Save/error_log.txt");
                const std::string message = "Settlement not found. ID: " + std::to_string(id);
                logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
            }
        }

    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_GRID_HPP
