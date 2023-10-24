﻿/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INIT_HPP
#define PAX_MAHOROBA_INIT_HPP

/*##########################################################################################

##########################################################################################*/

#ifdef PAXS_USING_SIV3D

#include <Siv3D.hpp>

#else
#include <string>

#include <PAX_MAHOROBA/Mock.hpp>
#endif

// マップの種類（暫定的な地図）
enum MapType {
    base,
    landand_sea, land_and_water,
    soil, soil_temperature,
    ryosei_country, ryosei_line,
    slope,
    river1, river2, line1, line2
};

#ifdef PAXS_USING_SIV3D
#ifdef SIV3D_PLATFORM_PRIVATE_DEFINITION_WINDOWS
#ifndef PAXS_S3D_RESOURCE
#define PAXS_S3D_RESOURCE s3d::Resource
#endif
#else
#ifndef PAXS_S3D_RESOURCE
#define PAXS_S3D_RESOURCE
#endif
#endif
#endif

#endif // !PAX_MAHOROBA_INIT_HPP
