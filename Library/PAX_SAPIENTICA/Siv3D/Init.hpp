/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_INIT_HPP
#define PAX_SAPIENTICA_SIV3D_INIT_HPP

/*##########################################################################################

##########################################################################################*/

#include <Siv3D.hpp> // OpenSiv3D

#ifdef SIV3D_PLATFORM_PRIVATE_DEFINITION_WINDOWS
#ifndef PAXS_S3D_RESOURCE
#define PAXS_S3D_RESOURCE s3d::Resource
#endif
#else
#ifndef PAXS_S3D_RESOURCE
#define PAXS_S3D_RESOURCE
#endif
#endif

#endif // !PAX_SAPIENTICA_SIV3D_INIT_HPP