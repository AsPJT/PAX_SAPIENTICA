#include <iostream>

#include <PAX_SAPIENTICA/Simulation/SpaceGrid.hpp>

int main(){
    paxs::type::Coordinate<double> start(1, 1);
    paxs::type::Coordinate<double> end(2, 2);
    paxs::type::CoordinateRange<double> range(start, end);
    paxs::type::Coordinate<int_fast32_t> num(2, 2);

    paxs::SpaceGrid<double> s(range, num);
}