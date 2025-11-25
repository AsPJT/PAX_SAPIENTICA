/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_PLATFORM_HPP
#define PAX_SAPIENTICA_CORE_PLATFORM_HPP

#if defined(_WIN32)
#define PAXS_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define PAXS_PLATFORM_MACOS
#elif defined(__LINUX__)
#define PAXS_PLATFORM_LINUX
#elif defined(__ANDROID__)
#define PAXS_PLATFORM_ANDROID
#endif

#endif // !PAX_SAPIENTICA_CORE_PLATFORM_HPP

