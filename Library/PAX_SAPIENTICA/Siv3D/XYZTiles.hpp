/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_XYZ_TILES_HPP
#define PAX_SAPIENTICA_SIV3D_XYZ_TILES_HPP

/*##########################################################################################

##########################################################################################*/

#include <cmath>

namespace paxs {
	
	class XYZTile {
	public:
		// XYZ タイルの 1 つのセルのメルカトル座標を保持
		// 基本的に Z = 19 は無い
		std::vector<MapVec2D> xyz_tile_pos_list;
		// XYZ タイルの画像の情報を保持
		std::vector<s3d::Texture> xyz_tile_texture_list;

		XYZTile(const double map_view_width,
			const double map_view_height,
			const double map_view_center_x,
			const double map_view_center_y) {
			xyz_tile_start_cell = {
				int((((map_view_center_x - map_view_width / 2) + 180.0) / 360.0) * xyz_tile_z_num),
				int(((360.0 - ((map_view_center_y + map_view_height / 2) + 180.0)) / 360.0) * xyz_tile_z_num)
			};
			xyz_tile_end_cell = {
				int((((map_view_center_x + map_view_width / 2) + 180.0) / 360.0) * xyz_tile_z_num),
				int(((360.0 - ((map_view_center_y - map_view_height / 2) + 180.0)) / 360.0) * xyz_tile_z_num)
			};

			xyz_tile_cell_num = {
			(xyz_tile_end_cell.x - xyz_tile_start_cell.x),
			(xyz_tile_end_cell.y - xyz_tile_start_cell.y)
			};

			xyz_tile_cell_all_num = (xyz_tile_cell_num.x + 1) * (xyz_tile_cell_num.y + 1);

			// 画面上の XYZ タイルのメルカトル座標を初期化
			xyz_tile_pos_list.resize(xyz_tile_cell_all_num);
			xyz_tile_texture_list.resize(xyz_tile_cell_all_num);
			for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
				for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
					xyz_tile_pos_list[k] =
						//xyz_tile_pos_list[i * xyz_tile_cell_num.x + j] =
						MapVec2D{ j * 360.0 / xyz_tile_z_num - 180.0,
				(360.0 - i * 360.0 / xyz_tile_z_num) - 180.0 };
				}
			}
		}
		// タイルを更新
		void update(const double map_view_width,
			const double map_view_height,
			const double map_view_center_x,
			const double map_view_center_y) {
			// 拡大率が変わった場合、拡大率にあわせて取得する地図の大きさを変える
			if (current_map_view_width != map_view_width) {
				xyz_tile_z = int(-std::log2(map_view_width) + 11.0);
				xyz_tile_z_num = int(std::pow(2, xyz_tile_z));
				current_map_view_width = map_view_width;
			}

			// 画像を更新する必要があるか
			bool need_update = false;

			const MapVec2 xyz_tile_new_start_cell = { int((((map_view_center_x - map_view_width / 2) + 180.0) / 360.0) * xyz_tile_z_num), int(((360.0 - ((map_view_center_y + map_view_height / 2) + 180.0)) / 360.0) * xyz_tile_z_num) };
			if (xyz_tile_new_start_cell != xyz_tile_start_cell) {
				xyz_tile_start_cell = xyz_tile_new_start_cell;
				need_update = true;
			}
			const MapVec2 xyz_tile_new_end_cell = MapVec2{ int((((map_view_center_x + map_view_width / 2) + 180.0) / 360.0) * xyz_tile_z_num), int(((360.0 - ((map_view_center_y - map_view_height / 2) + 180.0)) / 360.0) * xyz_tile_z_num) };
			if (xyz_tile_new_end_cell != xyz_tile_end_cell) {
				xyz_tile_end_cell = xyz_tile_new_end_cell;
				need_update = true;
			}

			// もしタイルが更新されていたら更新処理
			if (!need_update) return;
			xyz_tile_cell_num = {
				(xyz_tile_end_cell.x - xyz_tile_start_cell.x),
				(xyz_tile_end_cell.y - xyz_tile_start_cell.y)
			};
			xyz_tile_cell_all_num = (xyz_tile_cell_num.x + 1) * (xyz_tile_cell_num.y + 1);
			xyz_tile_pos_list.resize(xyz_tile_cell_all_num);
			xyz_tile_texture_list.resize(xyz_tile_cell_all_num);


			for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
				for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
					xyz_tile_pos_list[k] =
						MapVec2D{ j * 360.0 / xyz_tile_z_num - 180.0,
				(360.0 - i * 360.0 / xyz_tile_z_num) - 180.0 };
				}
			}


			for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
				for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
					const s3d::URL new_url =
						s3d::String(map_url_name)
						+ s3d::String(U"/") + s3d::ToString(xyz_tile_z)
						+ s3d::String(U"/") + s3d::ToString((j + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U"/") + s3d::ToString((i + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U".png");

					const s3d::FilePath new_saveFilePath = s3d::String(U"./SavedMap/") + map_name
						+ s3d::String(U"_") + s3d::ToString(xyz_tile_z)
						+ s3d::String(U"_") + s3d::ToString((j + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U"_") + s3d::ToString((i + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U".png");

					// ファイルを同期ダウンロード
					// ステータスコードが 200 (OK) なら
					xyz_tile_texture_list[k] = s3d::Texture{ new_saveFilePath };
					if (!xyz_tile_texture_list[k] && s3d::SimpleHTTP::Save(new_url, new_saveFilePath).isOK()) {
						xyz_tile_texture_list[k] = s3d::Texture{ new_saveFilePath };
					}
				}
			}
		}
		MapVec2 getStartCell()const {
			return xyz_tile_start_cell;
		}
		MapVec2 getEndCell()const {
			return xyz_tile_end_cell;
		}
		int getZ()const {
			return xyz_tile_z;
		}
		int getZNum()const {
			return xyz_tile_z_num;
		}
	private:
		// 画面の幅に最適な XYZ タイルの Z を格納
		int xyz_tile_z = 2;
		// 2 の xyz_tile_z 乗
		int xyz_tile_z_num = int(std::pow(2, xyz_tile_z));
		// XYZ タイルの画面上の始点セル
		MapVec2 xyz_tile_start_cell;
		// XYZ タイルの画面上の終点セル
		MapVec2 xyz_tile_end_cell;
		// XYZ タイルの画面上のセルの数
		MapVec2 xyz_tile_cell_num;
		// XYZ タイルの画面上のセルの総数
		int xyz_tile_cell_all_num;

		const s3d::String map_url_name = U"https://tile.mierune.co.jp/mierune"; // URL
		const s3d::String map_name = U"mierune";
		// 1フレーム前のマップの幅
		double current_map_view_width = 0;
	};

	// 後にクラス化する
	void updateXYZTiles(
		int& xyz_tile_z,
		int& xyz_tile_z_num,
		const double map_view_width,
		const double map_view_height,
		const double map_view_center_x,
		const double map_view_center_y,
		MapVec2& xyz_tile_start_cell,
		MapVec2& xyz_tile_end_cell,
		MapVec2& xyz_tile_cell_num,
		int& xyz_tile_cell_all_num,
		std::vector<MapVec2D>& xyz_tile_pos_list,
		std::vector<s3d::Texture>& xyz_tile_texture_list,
		const s3d::String& map_url_name,
		const s3d::String map_name
	) {

		// 拡大率にあわせて取得する地図の大きさを変える
		xyz_tile_z = int(- std::log2(map_view_width) + 11.0);
		xyz_tile_z_num = int(std::pow(2, xyz_tile_z));


		bool is_new_tile = false;

		const int xyz_tile_new_start_x_cell = int((((map_view_center_x - map_view_width / 2) + 180.0) / 360.0) * xyz_tile_z_num);
		if (xyz_tile_new_start_x_cell != xyz_tile_start_cell.x) {
			xyz_tile_start_cell.x = xyz_tile_new_start_x_cell;
			is_new_tile = true;
		}
		const int xyz_tile_new_start_y_cell = int(((360.0 - ((map_view_center_y + map_view_height / 2) + 180.0)) / 360.0) * xyz_tile_z_num);
		if (xyz_tile_new_start_y_cell != xyz_tile_start_cell.y) {
			xyz_tile_start_cell.y = xyz_tile_new_start_y_cell;
			is_new_tile = true;
		}
		const int xyz_tile_new_end_x_cell = int((((map_view_center_x + map_view_width / 2) + 180.0) / 360.0) * xyz_tile_z_num);
		if (xyz_tile_new_end_x_cell != xyz_tile_end_cell.x) {
			xyz_tile_end_cell.x = xyz_tile_new_end_x_cell;
			is_new_tile = true;
		}
		const int xyz_tile_new_end_y_cell = int(((360.0 - ((map_view_center_y - map_view_height / 2) + 180.0)) / 360.0) * xyz_tile_z_num);
		if (xyz_tile_new_end_y_cell != xyz_tile_end_cell.y) {
			xyz_tile_end_cell.y = xyz_tile_new_end_y_cell;
			is_new_tile = true;
		}
		// もしタイルが更新されていたら更新処理
		if (is_new_tile) {
			xyz_tile_cell_num = {
				(xyz_tile_end_cell.x - xyz_tile_start_cell.x),
				(xyz_tile_end_cell.y - xyz_tile_start_cell.y)
			};
			xyz_tile_cell_all_num = (xyz_tile_cell_num.x + 1) * (xyz_tile_cell_num.y + 1);
			xyz_tile_pos_list.resize(xyz_tile_cell_all_num);
			xyz_tile_texture_list.resize(xyz_tile_cell_all_num);


			for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
				for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
					xyz_tile_pos_list[k] =
						MapVec2D{ j * 360.0 / xyz_tile_z_num - 180.0,
				(360.0 - i * 360.0 / xyz_tile_z_num) - 180.0 };
				}
			}


			for (int i = xyz_tile_start_cell.y, k = 0; i <= xyz_tile_end_cell.y; ++i) {
				for (int j = xyz_tile_start_cell.x; j <= xyz_tile_end_cell.x; ++j, ++k) {
					const s3d::URL new_url =
						s3d::String(map_url_name)
						+ s3d::String(U"/") + s3d::ToString(xyz_tile_z)
						+ s3d::String(U"/") + s3d::ToString((j + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U"/") + s3d::ToString((i + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U".png");

					const s3d::FilePath new_saveFilePath = s3d::String(U"./SavedMap/") + map_name
						+ s3d::String(U"_") + s3d::ToString(xyz_tile_z)
						+ s3d::String(U"_") + s3d::ToString((j + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U"_") + s3d::ToString((i + xyz_tile_z_num) % xyz_tile_z_num)
						+ s3d::String(U".png");

					// ファイルを同期ダウンロード
					// ステータスコードが 200 (OK) なら
					xyz_tile_texture_list[k] = s3d::Texture{ new_saveFilePath };
					if (!xyz_tile_texture_list[k] && s3d::SimpleHTTP::Save(new_url, new_saveFilePath).isOK()) {
						xyz_tile_texture_list[k] = s3d::Texture{ new_saveFilePath };
					}

				}
			}
		}

	}

}

#endif // !PAX_SAPIENTICA_SIV3D_XYZ_TILES_HPP