/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_KEY_HPP
#define PAX_SAPIENTICA_SIV3D_KEY_HPP

/*##########################################################################################

##########################################################################################*/

#include<Array>
#include<memory>
#include<new>

namespace paxs {
	struct BaseKey {
	public:
		virtual void update() = 0;
		virtual bool getKey() const = 0;
		virtual ~BaseKey() {}
	};

	class Key : BaseKey {
	public:
		explicit Key(s3d::Input key) : key(key) {}
		void update() {
			is_pressed = key.pressed();
		}
		bool getKey() const {
			return is_pressed;
		}
	private:
		s3d::Input key;
		bool is_pressed = false;
	};

	class Input {
	public:
		explicit Input() {
			std::array<s3d::Input, 10> s3d_keys = { s3d::KeyA,s3d::KeyLeft,s3d::KeyD,s3d::KeyRight,s3d::KeyS,s3d::KeyDown,s3d::KeyW,s3d::KeyUp,s3d::KeyQ,s3d::KeyE };
			for (int i = 0; i < s3d_keys.size(); ++i) {
				keys[i].reset((BaseKey*)(new(std::nothrow) Key(s3d_keys[i])));
			}
		}
		void update() {
			for (auto& key : keys) {
				key->update();
			}
		}
	private:
		std::array<std::unique_ptr<BaseKey>, 10> keys;
	};

	// キーボード入力を更新（どの入力が必要か未確定なのでまだクラス化前）
	void updateKey(
	double& map_view_center_x, // マップ座標の中央 X
	double& map_view_center_y, // マップ座標の中央 Y
	double& map_view_width, // マップの幅
	double& map_view_max_width, // マップの最大幅
	double& map_view_min_width, // マップの最小幅
	double& map_view_height, // マップの高さ
	double& map_view_movement_size, // マップの移動量
	double& map_view_expansion_size // マップの拡大量
	) {
		if (s3d::KeyA.pressed() || s3d::KeyLeft.pressed()) {
			map_view_center_x -= (map_view_width / map_view_movement_size);
			if (map_view_center_x < -180.0) {
				map_view_center_x += 360.0;
			}
		}
		if (s3d::KeyD.pressed() || s3d::KeyRight.pressed()) {
			map_view_center_x += (map_view_width / map_view_movement_size);
			if (map_view_center_x >= 180.0) {
				map_view_center_x -= 360.0;
			}
		}
		if (s3d::KeyS.pressed() || s3d::KeyDown.pressed()) {
			map_view_center_y -= (map_view_width / map_view_movement_size);
			if (map_view_center_y < -180.0) {
				map_view_center_y = -180.0;
			}
		}
		if (s3d::KeyW.pressed() || s3d::KeyUp.pressed()) {
			map_view_center_y += (map_view_width / map_view_movement_size);
			if (map_view_center_y > 180.0) {
				map_view_center_y = 180.0;
			}
		}
		if (s3d::KeyQ.pressed()) {
			if (map_view_width > map_view_min_width) {
				map_view_width -= (map_view_width / map_view_expansion_size);
				map_view_height = (map_view_width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
			}
			if (map_view_width < map_view_min_width) {
				map_view_width = map_view_min_width;
				map_view_height = (map_view_width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
			}
		}
		if (s3d::KeyE.pressed()) {
			if (map_view_width < map_view_max_width) {
				map_view_width += (map_view_width / map_view_expansion_size);
				map_view_height = (map_view_width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
			}
			if (map_view_width > map_view_max_width) {
				map_view_width = map_view_max_width;
				map_view_height = (map_view_width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
			}
		}
	}

}

#endif // !PAX_SAPIENTICA_SIV3D_KEY_HPP