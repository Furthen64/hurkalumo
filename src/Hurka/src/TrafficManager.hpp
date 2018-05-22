#ifndef TRAFFICMANAGER_H
#define TRAFFICMANAGER_H

#include <list>
#include "Graph.hpp"        // Includes Node and Link for NODE_DIR
#include "RoadNetwork.hpp"  // Includes HPos
#include "Bus.hpp"


using namespace sf;

/// A bus rides on a particular Road Network
///
/// A road network is a matrix of the roads with an x,y offset integer so it knows where in the gamematrix it really is
/// See docs: road_networks.png



class TrafficManager
{
public:

    TrafficManager();


    /// High level functions


    void drawBuses(sf::RenderWindow &rt, HPos *viewHPos);

    RoadNetwork *followAndAddToBST(HurkaMatrix *fullRoadMatrix,
                                   HPos *curr_iso_pos,
                                   HPos *min_iso_pos,
                                   HPos *max_iso_pos,
                                   BinarySearchTree *visited,
                                   int debugLevel);

    // HPOSDELETE:
    /*RoadNetwork *followAndAddToBST(HurkaMatrix *fullRoadMatrix,
                                   Vector2f curr_iso_pos,
                                   Vector2f *min_iso_pos,
                                   Vector2f *max_iso_pos,
                                   BinarySearchTree *visited,
                                   int debugLevel);
      */








    // void follow(HurkaMatrix *fullRoadMatrix, HurkaMatrix *newMatrix, Vector2f curr_iso_pos, Vector2f *min_iso_pos,  Vector2f *max_iso_pos, BinarySearchTree *visited, int debugLevel); // HPOSDELETE
    void follow(HurkaMatrix *fullRoadMatrix,
                HurkaMatrix *newMatrix,
                HPos *curr_iso_pos,
                HPos *min_iso_pos,
                HPos *max_iso_pos,
                BinarySearchTree *visited,
                int debugLevel);

    int parseCurrentRoads(HurkaMatrix *roadMatrix, int debugLevel);




    void dumpRoadNetworks(std::string , bool);

    void updateAll(HPos *viewHPos);







    /// Individual

    int nrOfRoadnetworks();

    void addRoadNetwork();
    void addBus(Bus *, int );


    DijkstraResult *runDijkstraOnBus(int busId, Vector2f *from_iso_pos, Vector2f *to_iso_pos);

    int planForBusesOnRoadNetwork(int debugLevel, int fromRoad, int toRoad);

    void updateBusesOnRoadNetwork(int busId, int roadnetId);




private:
    std::string cn = "TrafficManager.cpp";
    std::list<RoadNetwork *> *roadNetworks;

};

#endif
