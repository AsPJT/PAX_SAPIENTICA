/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_GEOGRAPHIC_INFORMATION_HPP
#define PAX_SAPIENTICA_SIMULATION_GEOGRAPHIC_INFORMATION_HPP

/*##########################################################################################

##########################################################################################*/


namespace paxs {
    class GeographicInformation {
	public:
		enum class WaterType : char {
			Sea, // 海
			Lake, // 湖
			River, // 河川
		};
		
		GeographicInformation(const bool is_land = false, const int elevation = 0, const double slope = 0, const char geology = 0, const char soil = 0, const char soil_temperature = 0, const WaterType water_type = WaterType::Sea) :
			is_land(is_land), elevation(elevation), slope(slope), geology(geology), soil(soil), soil_temperature(soil_temperature), water_type(water_type) {}
		bool isLand() const { return is_land; }
		void setSlope(const double slope_) { this->slope = slope_; }
	private:
		bool is_land; // 陸地かどうか
		int elevation; // 標高
		double slope; // 傾斜
		char geology; // 地質
		char soil; // 土壌
		char soil_temperature; // 土壌温度
		WaterType water_type; // 水域の種類
	};
}

#endif // !PAX_SAPIENTICA_SIMULATION_GEOGRAPHIC_INFORMATION_HPP