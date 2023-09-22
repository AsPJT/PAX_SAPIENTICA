/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GROUP_SIMULATOR_HPP
#define PAX_SAPIENTICA_GROUP_SIMULATOR_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/BasicGroup.hpp>
#include <PAX_SAPIENTICA/Simulation/CompositeSettlement.hpp>
#include <PAX_SAPIENTICA/Simulation/UnitGroup.hpp>

namespace paxs {

    template <typename GridType>
    class GroupSimulator
    {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Environment = paxs::Environment<GridType>;
        using Agent = paxs::Agent<GridType>;

        constexpr explicit GroupSimulator() = default;
        explicit GroupSimulator(const std::string& setting_file_path,
        const Vector2& start_position, const Vector2& end_position, const int z, const unsigned seed = 0)
        : environment(std::make_unique<Environment>(setting_file_path, start_position, end_position, z)), gen(seed) {
            if (z <= 0) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Z must be greater than 0.";
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
            if (start_position.x < 0 || start_position.y < 0 || end_position.x < 0 || end_position.y < 0) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Start position and end position must be greater than or equal to 0.";
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }
        }

    private:
        std::vector<BaseGroup<GridType>> groups; // グループ
        std::shared_ptr<Environment> environment; // 環境
        std::mt19937 gen; // 乱数生成器

    };

}

#endif // !PAX_SAPIENTICA_GROUP_SIMULATOR_HPP
