/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_OBJECT_HPP
#define PAX_SAPIENTICA_SIMULATION_OBJECT_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Simulation/ConvertToMercatorCoordinate.hpp>

namespace paxs {

    /// @brief オブジェクトを表すクラス
    /// @tparam T 座標の型
    template <typename T>
    class Object {
    public:
        using Vector2 = paxs::Vector2<T>;

        Object(const std::string& id, const std::string& name, const Vector2& position) : id(id), name(name), position(position) {}

        /// @brief オブジェクトの座標xを取得
        T getX() const { return position.x; }
        /// @brief オブジェクトの座標yを取得
        T getY() const { return position.y; }
        /// @brief オブジェクトの座標を取得
        Vector2 getPosition() const { return position; }
        /// @brief オブジェクトの座標xをセット
        void setX(const T& x) { position.x = x; }
        /// @brief オブジェクトの座標yをセット
        void setY(const T& y) { position.y = y; }
        /// @brief オブジェクトの座標をセット
        void setPosition(const Vector2& pos) { position = pos; }

        /// @brief 座標をメルカトル座標で取得
        /// @param start_position シミュレーションの左上の座標
        /// @param z ズームレベル
        /// @return メルカトル座標
        paxs::Vector2<double> getLocation(const paxs::Vector2<int>& start_position, const int z) const {
            return convertToMercatorCoordinate(start_position, position, z);
        }

        bool operator==(const paxs::Object<T>& a) {
            return a.position == position && a.id == id && a.name == name;
        }

    protected:
        std::string id; // オブジェクトのID
        std::string name; // オブジェクトの名前
        Vector2 position; // オブジェクトの座標
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_OBJECT_HPP