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

#include<memory>
#include<new>
#include<vector>

#include <PAX_SAPIENTICA/Siv3D/Init.hpp>

namespace paxs {
	
	// abstract class
	struct BaseKey {
	public:
		virtual bool isPressed() const = 0;
		virtual ~BaseKey() {}
	};

	class Key : BaseKey {
	public:
		explicit Key(s3d::Input key) : key(key) {}
		bool isPressed() const {
			return key.pressed();
		}
	private:
		s3d::Input key;
	};

	enum class KeyCode {
		A,
		Left,
		D,
		Right,
		S,
		Down,
		W,
		Up,
		Q,
		E
	};

	bool pressed(const std::vector<std::unique_ptr<BaseKey>>& keys) {
		for (auto& key : keys) {
			if (key->isPressed()) return true;
		}
		return false;
	}

	class Coordinate {
	public:
		double movement_size; // マップの移動量

		Coordinate(double x, double y, double movement_size) : x(x), y(y), movement_size(movement_size) {
			increase_x_keys.resize(2);
			increase_x_keys[0].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyA));
			increase_x_keys[1].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyLeft));

			decrease_x_keys.resize(2);
			decrease_x_keys[0].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyD));
			decrease_x_keys[1].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyRight));

			increase_y_keys.resize(2);
			increase_y_keys[0].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyS));
			increase_y_keys[1].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyDown));

			decrease_y_keys.resize(2);
			decrease_y_keys[0].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyW));
			decrease_y_keys[1].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyUp));
		}
		void update(const double width) {
			if (pressed(increase_x_keys)) increase_coordinate(x, width);
			if (pressed(decrease_x_keys)) decrease_coordinate(x, width);
			if (pressed(increase_y_keys)) increase_coordinate(y, width);
			if (pressed(decrease_y_keys)) decrease_coordinate(y, width);
		}
		double getX() const {
			return x;
		}
		double getY() const {
			return y;
		}
	private:
		double x; // マップ座標の中央X
		double y; // マップ座標の中央Y
		std::vector<std::unique_ptr<BaseKey>> increase_x_keys;
		std::vector<std::unique_ptr<BaseKey>> decrease_x_keys;
		std::vector<std::unique_ptr<BaseKey>> increase_y_keys;
		std::vector<std::unique_ptr<BaseKey>> decrease_y_keys;

		void increase_coordinate(double& value, const double width) {
			value -= (width / movement_size);
			if (value < -180.0) {
				value += 360.0;
			}
		}
		void decrease_coordinate(double& value, const double width) {
			value += (width / movement_size);
			if (value >= 180.0) {
				value -= 360.0;
			}
		}
	};

	class MapView {
	public:
		MapView() {
			enl_keys.resize(1);
			enl_keys[0].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyQ));

			esc_keys.resize(1);
			esc_keys[0].reset((BaseKey*)new(std::nothrow) Key(s3d::KeyE));
		}
		void update() {
			center.update(width);
			if (pressed(enl_keys)) {
				if (width > min_width) {
					width -= (width / expansion_size);
					height = (width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
				}
				if (width < min_width) {
					width = min_width;
					height = (width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
				}
			}
			if (pressed(esc_keys)) {
				if (width < max_width) {
					width += (width / expansion_size);
					height = (width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
				}
				if (width > max_width) {
					width = max_width;
					height = (width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height());
				}
			}
		}
		double getCenterX() const {
			return center.getX();
		}
		double getCenterY() const {
			return center.getY();
		}
		double getWidth() const {
			return width;
		}
		double getHeight() const {
			return height;
		}
	private:
		Coordinate center = Coordinate(135.0, getLatitudeToMercatorY(35.0), 200.0); // マップ座標の中央
		double width = 2.0; // マップの幅

		// 平城京
		//Coordinate center = Coordinate(135.807, 37.009/*getLatitudeToMercatorY(35)*/, 200.0); // マップ座標の中央
		//double width = 0.1; // マップの幅
		double max_width = 640.0; // マップの最大幅
		double min_width = 0.01; // マップの最小幅
		double height = (width) / double(s3d::Scene::Width()) * double(s3d::Scene::Height()); // マップの高さ
		double expansion_size = 50.0; // マップの拡大量
		std::vector<std::unique_ptr<BaseKey>> enl_keys; // 拡大キー
		std::vector<std::unique_ptr<BaseKey>> esc_keys; // 縮小キー
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