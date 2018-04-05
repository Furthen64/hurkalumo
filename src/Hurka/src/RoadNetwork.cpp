#include "RoadNetwork.hpp"

RoadNetwork::RoadNetwork(int _isoYOffset, int _isoXOffset)
{
    isoYOffset = _isoYOffset;
    isoXOffset = _isoXOffset;
}

// (-+)
void RoadNetwork::dump(std::string indent)
{
    hMatrix->dump(indent);
    std::cout << indent << "iso_y_offset=" << isoYOffset <<"\n";
    std::cout << indent << "iso_x_offset=" << isoXOffset << "\n";
}


