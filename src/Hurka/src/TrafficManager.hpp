#ifndef TRAFFICMANAGER_H
#define TRAFFICMANAGER_H

#include <list>
#include "Graph.hpp"        // Includes Node and Link for NODE_DIR
#include "RoadNetwork.hpp"

using namespace sf;

/// A bus rides on a particular Road Network
///
/// A road network is a matrix of the roads with an x,y offset integer so it knows where in the gamematrix it really is
/// See docs: road_networks.png



class TrafficManager
{
public:

    TrafficManager();


    // High level functions

    RoadNetwork *followAndAddToBST(HurkaMatrix *fullRoadMatrix, Vector2f curr_iso_pos, BinarySearchTree *visited, int debugLevel);

    void follow(HurkaMatrix *fullRoadMatrix, HurkaMatrix *newMatrix, Vector2f curr_iso_pos, BinarySearchTree *visited, int debugLevel);

    void parseCurrentRoads(HurkaMatrix *roadMatrix, int debugLevel);


    void dumpRoadNetworks(std::string indent);

    void updateAll();           // updates all the buses on all the roadnetworks






    // Individual
    void addRoadNetwork();

    DijkstraResult *runDijkstraOnBus(int busId);

    void planForBusesOnRoadNetwork(int roadnetId);

    void updateBusesOnRoadNetwork(int busId, int roadnetId);




private:
    std::string cn = "TrafficManager.cpp";

    std::list<RoadNetwork *> *roadNetworks;
};

#endif
