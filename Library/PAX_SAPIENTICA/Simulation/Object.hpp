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
    template <typename T>
    class Object {
    public:
        using Vector2 = paxs::Vector2<T>;

        Object(const std::string& id, const std::string& name, const Vector2& position) : id(id), name(name), position(position) {}

        T getX() const { return position.x; }
        T getY() const { return position.y; }
        Vector2 getPosition() const { return position; }
        void setX(const T& x) { position.x = x; }
        void setY(const T& y) { position.y = y; }
        void setPosition(const Vector2& pos) { position = pos; }
        paxs::Vector2<double> getLocation(const paxs::Vector2<int>& start_position, const int z, const int pixel_size) const {
            return convertToMercatorCoordinate(start_position, position, z, pixel_size);
        }

        bool operator==(const paxs::Object<T>& a) {
            return a.position == position && a.id == id && a.name == name;
        }

    protected:
        std::string id;
        std::string name;
        Vector2 position;
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_OBJECT_HPP