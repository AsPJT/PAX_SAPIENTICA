/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_COMPOSITE_SETTLEMENT_HPP
#define PAX_SAPIENTICA_COMPOSITE_SETTLEMENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/BaseGroup.hpp>

namespace paxs {

    /// @brief A class that represents a composite settlement.
    /// @brief 複合集落を表すクラス
    template <typename GridType>
    class CompositeSettlement : public BaseGroup<GridType> {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        using Object = paxs::Object<GridType>;
        using Agent = paxs::Agent<GridType>;

        constexpr explicit CompositeSettlement(const std::uint_least32_t id) noexcept : BaseGroup<GridType>(id) {}

    private:
        /// @brief 基礎集団の位置
        std::vector<Vector2> positions;

    };

}

#endif // !PAX_SAPIENTICA_COMPOSITE_SETTLEMENT_HPP