#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include <list>
#include <string>


#include "Graph.hpp"
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

    HPos *getRandomRoad_iso(int findNr);

    SlotPath *createSlotPath(HPos *fromPos, HPos *toPos );

    void createGraphFromHMatrix(HurkaMatrix *roadMatrix,
                                         Graph *graph,
                                         Node *currNode,
                                         Node *prevNode,
                                         HPos *curr_iso_pos,
                                         HPos *prev_iso_pos,
                                         BinarySearchTree *visited,
                                         int dbgLevel);

    void addBus(Bus *_bus);


    int nrRows() { return hMatrix->rows; }
    int nrCols() { return hMatrix->cols; }
    void dump(std::string indent);





// Variables:

    HurkaMatrix *hMatrix;

    std::list<Bus *> *buslist;

    int min_isoYOffset;
    int min_isoXOffset;

    int max_isoYOffset;
    int max_isoXOffset;



private:

    std::string cn = "RoadNetwork.cpp";
};

#endif
