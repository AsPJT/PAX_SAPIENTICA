/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <PAX_SAPIENTICA/Simulation/BaseGroup.hpp>

#ifndef PAX_SAPIENTICA_UNIT_HPP
#define PAX_SAPIENTICA_UNIT_HPP

/*##########################################################################################

##########################################################################################*/

namespace paxs {

    /// @brief A class that represents a unit group.
    /// @brief 単位集団を表すクラス
    template <typename GridType>
    class UnitGroup : public BaseGroup<GridType>
    {
    public:
        using Vector2 = paxs::Vector2<GridType>;
        constexpr explicit UnitGroup(const std::string& id, const Vector2& position) noexcept : BaseGroup<GridType>(id), position(position) {}
    private:
        /// @brief 単位集団の位置
        Vector2 position;
    };

}

#endif // !PAX_SAPIENTICA_UNIT_HPP