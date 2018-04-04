#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include <string>

#include "HurkaMatrix.hpp"


/// A bus rides on a particular Road Network
///
/// A road network is a matrix of the roads with an x,y offset integer so it knows where in the gamematrix it really is
/// See docs: road_networks.png

class RoadNetwork
{
public:

    RoadNetwork(int _isoYOffset, int _isoXOffset);

    HurkaMatrix *hMatrix;

    int isoYOffset;
    int isoXOffset;


private:

    std::string cn = "RoadNetwork.cpp";
};

#endif
