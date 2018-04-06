#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include <list>
#include <string>


#include "Bus.hpp"
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

    Vector2f getRandomRoad_abs_iso_pos(int findNr);


    void addBus(Bus *_bus);

    HurkaMatrix *hMatrix;


    std::list<Bus *> *buslist;


    //TODO gör en overhaul av alla positioner som är Vector2f eller lösa ints som här:
    // Gör om dom till Vector2i eller en Position klassinstans (får skapa den isf)


    int min_isoYOffset;
    int min_isoXOffset;

    int max_isoYOffset;
    int max_isoXOffset;


private:

    std::string cn = "RoadNetwork.cpp";
};

#endif
