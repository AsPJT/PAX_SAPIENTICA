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

#ifdef PAXS_USING_SIV3D

#include <Siv3D.hpp>

#else
#include <string>

#include <PAX_SAPIENTICA/Siv3D/Mock.hpp>
#endif

// GUI で画面上部にあるメニューバーの種類
enum MenuBarType {
    file, edit, view, calendar, map
};
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

#else

// ラップ化するのが望ましいが、まずは s3d 名前空間で使用済みの Siv3D クラス・関数などを自前で定義（中身はから）
// namespace s3d
// {
// 	using String = std::u32string;
// 	template <typename T>
// 	String ToString(T) { return U"a"; }
// 	namespace Unicode{
// 		template <typename T>
// 		String FromUTF8(T){ return U"a"; }
// 	}
// 	namespace Window{
// 		template <typename ... Args>
// 		void Resize(Args...) {}
// 	}
// 	class Sample{
// 		template <typename ... Args>
// 		Sample(Args...){/*何もしない*/}

// 		template <typename ... Args>
// 		const Sample& draw(Args...) const { return *this; }

// 		const Sample& removeSRGBCurve() const { return *this; }
// 	};
// 	using Texture = Sample;
// 	using ColorF = Sample;
// 	using Key = Sample;
// 	using Color = Sample;
// 	using Point = Sample;
// 	using Rect = Sample;
// 	using Model = Sample;
// 	using MSRenderTexture = Sample;
// 	using DebugCamera3D = Sample;
// 	using Font = Sample;
// 	using Input = Sample;
// } // namespace s3d


#endif

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
