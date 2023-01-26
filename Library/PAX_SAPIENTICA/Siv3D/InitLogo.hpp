/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_INIT_LOGO_HPP
#define PAX_SAPIENTICA_SIV3D_INIT_LOGO_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Constant/Version.hpp>

namespace paxs {

	void initLogo() {
		s3d::Scene::SetBackground(s3d::Color{ 181, 0, 0 }); // PAX SAPIENTICA 用の背景
		s3d::System::Update();
		s3d::Window::SetTitle(s3d::Unicode::FromUTF8("PAX SAPIENTICA v") + s3d::Unicode::FromUTF8(PAX_SAPIENTICA_LIBRARY_VERSION_NAME));
		const s3d::Texture texture_tl{ U"./../../../../../Image/Logo/TitleLogo2.svg" };
		const s3d::Texture texture_tm{ U"./../../../../../Image/Logo/TitleMap.svg" };
		texture_tl.drawAt(s3d::Scene::Center()); // タイトルロゴを描画
		texture_tm.draw(s3d::Arg::bottomRight = s3d::Vec2(s3d::Scene::Width(), s3d::Scene::Height()));
		s3d::System::Update();
		texture_tl.drawAt(s3d::Scene::Center()); // タイトルロゴを描画
		texture_tm.draw(s3d::Arg::bottomRight = s3d::Vec2(s3d::Scene::Width(), s3d::Scene::Height()));
	}

}

#endif // !PAX_SAPIENTICA_SIV3D_INIT_LOGO_HPP