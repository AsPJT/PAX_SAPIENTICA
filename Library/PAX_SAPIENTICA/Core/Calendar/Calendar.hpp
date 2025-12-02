/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_CALENDAR_CALENDAR_HPP
#define PAX_SAPIENTICA_CORE_CALENDAR_CALENDAR_HPP

#include <PAX_SAPIENTICA/Core/Calendar/Constants.hpp>
#include <PAX_SAPIENTICA/Core/Calendar/Conversions.hpp>

namespace paxs {

    /// @brief 暦に関する定数と変換関数を統合した型
    /// @brief Unified type for calendar constants and conversion functions
    template<typename T>
    struct Calendar : public CalendarConstants<T>, public CalendarConversions<T> {
    };
}

#endif // !PAX_SAPIENTICA_CORE_CALENDAR_CALENDAR_HPP

