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
        using Environment = paxs::Environment<GridType>;
        using Vector2 = paxs::Vector2<GridType>;
        using Settlement = paxs::Settlement<GridType>;

        explicit SettlementGrid(const Vector2& grid_position, std::shared_ptr<Environment> environment,
         const unsigned seed = 0) noexcept : grid_position(grid_position), environment(environment), gen(seed) {}

        /// @brief Add a settlement to the grid.
        /// @brief 集落をグリッドに追加
        void addSettlement(const std::shared_ptr<Settlement>& settlement) noexcept { settlements.push_back(settlement); }

        /// @brief Move a settlement to this grid.
        /// @brief 集落をこのグリッドに移動
        void moveSettlementToThis(const std::shared_ptr<Settlement>& settlement) noexcept {
            settlements.push_back(settlement);

            // 他の集落とかぶらない位置を探す
            // ブラックリスト
            std::vector<Vector2> black_list(settlements.size());
            for (std::size_t i = 0; i < settlements.size(); ++i) {
                black_list[i] = settlements[i]->getPosition();
            }

            // ランダムな位置を探す
            std::uniform_int_distribution<> dis_x(grid_position.x, grid_position.x + paxs::grid_length - 1);
            std::uniform_int_distribution<> dis_y(grid_position.y, grid_position.y + paxs::grid_length - 1);
            Vector2 position;

            while (black_list.size() < grid_length * grid_length) {
                position.x = dis_x(gen);
                position.y = dis_y(gen);
                if (std::find(black_list.begin(), black_list.end(), position) == black_list.end()) {
                    if (environment->isLive(position)) {
                        // 居住可能
                        break;
                    } else {
                        // 居住不可
                        black_list.push_back(position);
                    }
                }
            }

            // 集落を移動
            if (black_list.size() == grid_length * grid_length) {
                // 居住可能な場所がない
                Logger logger("Save/error_log.txt");
                const std::string message = "No place to live.";
                logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
            }

            settlement->setPosition(position);
        }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        std::vector<std::shared_ptr<Settlement>>& getSettlements() noexcept { return settlements; }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        const std::vector<std::shared_ptr<Settlement>>& cgetSettlements() const noexcept { return settlements; }

        /// @brief Get the settlements.
        /// @brief 集落を取得
        std::shared_ptr<Settlement>& getSettlement(const std::uint_least32_t id) {
            for (auto& settlement : settlements) {
                if (settlement->getId() == id) {
                    return settlement;
                }
            }
            Logger logger("Save/error_log.txt");
            const std::string message = "Settlement not found. ID: " + std::to_string(id);
            logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
            throw std::runtime_error(message);
        }

        /// @brief Get the grid position.
        /// @brief グリッドの座標を取得
        Vector2 getGridPosition() const noexcept { return grid_position; }

        /// @brief Delete the settlement.
        /// @brief 集落を削除
        void deleteSettlement(const std::uint_least32_t id) {
            auto it = std::find_if(settlements.begin(), settlements.end(), [id](const std::shared_ptr<Settlement>& settlement) { return settlement->getId() == id; });
            if (it != settlements.end()) {
                settlements.erase(it);
            } else {
                Logger logger("Save/error_log.txt");
                const std::string message = "Settlement not found. ID: " + std::to_string(id);
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
        }

        /// @brief Add a Ryoseikoku ID to the list.
        /// @brief 令制国のIDをリストに追加
        void addRyoseikokuId(const std::uint_least8_t id) noexcept {
            // 重複チェック
            if (std::find(ryoseikoku_list.begin(), ryoseikoku_list.end(), id) == ryoseikoku_list.end()) {
                ryoseikoku_list.push_back(id);
            }
        }

        /// @brief Get the Ryoseikoku list.
        /// @brief 令制国のリストを取得
        std::vector<std::uint_least8_t>& getRyoseikokuIds() noexcept { return ryoseikoku_list; }
        const std::vector<std::uint_least8_t>& cgetRyoseikokuIds() const noexcept { return ryoseikoku_list; }

    private:
        std::vector<std::shared_ptr<Settlement>> settlements;
        std::shared_ptr<Environment> environment;
        Vector2 grid_position;
        std::mt19937 gen; // 乱数生成器
        std::vector<std::uint_least8_t> ryoseikoku_list;
    };

}

#endif // !PAX_SAPIENTICA_SETTLEMENT_GRID_HPP
