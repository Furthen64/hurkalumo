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

    HPos *getNrRoad_iso(int findNr);

    SlotPath *createSlotPath(HPos *fromPos, HPos *toPos, int debugLevel);

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



// Static Utilities



    /// \brief Given a roadMatrix (matrix with 1s for roads, 0s else) find a random position where a road is
    /// \param roadMatrix Allocated HurkaMatrix object with set values
    /// \return A random position for a road
    /// (--)
    // HPOSTEST!
    static HPos *getRandomRoad_iso_pos(HurkaMatrix *roadMatrix)
    {
        std::string cn ="RoadNetwork.hpp";
        if(roadMatrix->rows > 10000 || roadMatrix->cols > 10000) {
            std::cout << "ERROR" << cn << " too big of a roadmatrix! " << roadMatrix->rows << ", " << roadMatrix->cols << "\n";
            return nullptr;
        }

        bool found = false;
        int allowedAttempts = 500;
        int currAttempt = 0;

        HPos *newPos = new HPos(0,0,USE_ISO);

        int r;
        int c;

        while(found == false && currAttempt < allowedAttempts) {

            r = randBetween(0, roadMatrix->rows-1);
            c = randBetween(0, roadMatrix->cols-1);

            if(roadMatrix->matrix[r][c] == 1) {

                newPos->abs_iso_y = r;
                newPos->abs_iso_x = c;

                found = true;

            }

            currAttempt++;
        }

        if(found == false) {

            std::cout << "Could not find random bus position\n";
            return nullptr;
        }

        return newPos;
    }

private:

    // Internal function
    void createSlotPathFromDijkstraResult(DijkstraResult *dijkstraResult, SlotPath *slotpath);

    std::string cn = "RoadNetwork.cpp";
};

#endif
