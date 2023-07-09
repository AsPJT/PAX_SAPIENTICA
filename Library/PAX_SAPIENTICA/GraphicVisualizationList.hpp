/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GRAPHIC_VISUALIZATION_LIST_HPP
#define PAX_SAPIENTICA_GRAPHIC_VISUALIZATION_LIST_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>
#include <unordered_map>

namespace paxs {

	// 
	class GraphicVisualizationList {
	private:
		std::unordered_map<std::string, bool> visible{};

		public:

			void emplace(const std::string& str_, const bool visible_) {
				visible.emplace(str_, visible_);
			}

			void setNot(const std::string& str_) {
				visible[str_] = !visible[str_];
			}
			void set(const std::string& str_, const bool visible_) {
				visible[str_] = visible_;
			}

			bool operator[](const std::string& str_) const {
				return visible.at(str_);
			}


	};

}

#endif // !PAX_SAPIENTICA_GRAPHIC_VISUALIZATION_LIST_HPP