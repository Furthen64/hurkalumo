#include "RoadNetwork.hpp"

RoadNetwork::RoadNetwork()
{
}

// (-+)
void RoadNetwork::dump(std::string indent)
{
    hMatrix->dump(indent);
    std::cout << indent << "min_iso_y_offset=" << min_isoYOffset <<"\n";
    std::cout << indent << "min_iso_x_offset=" << min_isoXOffset<< "\n";
    std::cout << indent << "max_iso_y_offset=" << max_isoYOffset <<"\n";
    std::cout << indent << "max_iso_x_offset=" << max_isoXOffset << "\n\n";
}


