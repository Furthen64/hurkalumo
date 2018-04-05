#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include <string>

#include "HurkaMatrix.hpp"

/// Used by TrafficManager
///
/// A bus rides on a particular Road Network
///
/// A road network is a matrix of the roads with an x,y offset integer so it knows where in the gamematrix it really is
/// See docs: road_networks.png


class RoadNetwork
{
public:

    RoadNetwork();
    void dump(std::string indent);

    HurkaMatrix *hMatrix;


    //TODO g�r en overhaul av alla positioner som �r Vector2f eller l�sa ints som h�r:
    // G�r om dom till Vector2i eller en Position klassinstans (f�r skapa den isf)


    int min_isoYOffset;
    int min_isoXOffset;

    int max_isoYOffset;
    int max_isoXOffset;


private:

    std::string cn = "RoadNetwork.cpp";
};

#endif
