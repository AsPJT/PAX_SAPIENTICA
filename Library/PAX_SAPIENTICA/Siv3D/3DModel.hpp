/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_3D_MODEL_HPP
#define PAX_SAPIENTICA_SIV3D_3D_MODEL_HPP

/*##########################################################################################

##########################################################################################*/

#include <Siv3D.hpp> // OpenSiv3D v0.6.6

namespace paxs {

	class Graphics3DModel {
	private:
		int rot = 0;

		s3d::ColorF backgroundColor;

		// モデルデータをロード
		s3d::Model sekishitsu_model;

		s3d::MSRenderTexture renderTexture{ 
			s3d::Size{500,300}/*s3d::Scene::Size()*/, 
			s3d::TextureFormat::R8G8B8A8_Unorm_SRGB, 
			s3d::HasDepth::Yes };

		s3d::DebugCamera3D camera{ 
			s3d::Size{500,300}/*s3d::Graphics3D::GetRenderTargetSize()*/, 
			40.0 / 180.0 * 3.1416, 
			s3d::Vec3{ 0, 3, -16 } };


	public:
		Graphics3DModel() {

			backgroundColor = s3d::ColorF{ 1,1,1,0 }.removeSRGBCurve();

			// モデルデータをロード
			sekishitsu_model = s3d::Model{ U"./../../../../../Data/3DModel/KofunOBJ/Model/Sekishitsu/KamoKitaKofun/KamoKitaKofun.obj" };

			// モデルに付随するテクスチャをアセット管理に登録
			s3d::Model::RegisterDiffuseTextures(sekishitsu_model, s3d::TextureDesc::MippedSRGB);

		}

		void updateRotation() {
			//camera.update(4.0);
			s3d::Graphics3D::SetCameraTransform(camera);

			// 3D シーンの描画
			const s3d::ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
			// モデルの描画
			++rot;
			if (rot >= 360) rot = 0;
			//const s3d::ScopedRenderStates3D renderStates{ s3d::BlendState::OpaqueAlphaToCoverage, s3d::RasterizerState::SolidCullNone };
			sekishitsu_model.draw(s3d::Vec3{ 0, 0, 0 }, s3d::Quaternion::RotateY(rot / 180.0 * 3.1416));
			// RenderTexture を 2D シーンに描画
			s3d::Graphics3D::Flush();
			renderTexture.resolve();
			renderTexture.draw();
		}

	};

}

#endif // !PAX_SAPIENTICA_SIV3D_3D_MODEL_HPP