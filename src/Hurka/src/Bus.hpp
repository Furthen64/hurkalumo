#ifndef BUS_H
#define BUS_H

#include <SFML/Graphics.hpp>

class RoadNetwork;



#include "GameMatrix.hpp"
#include "TextureManager.hpp"
#include "HurkaMatrix.hpp"
#include "Block.hpp"
#include "Grid.hpp"
#include "BusRoute.hpp"
#include "SlotPath.hpp"




using namespace sf;


class Bus
{
public:

    Bus();


    void reset();
    void gameUpdate(RoadNetwork *roadnet);
    void draw( RenderTarget& rt, HPos *viewHPos, int drawSlotPositions);

    void dump(std::string );
    void dump(HPos *viewHPos);


    /// Position


    bool atPos(HPos *);

    HPos *getNowPos();

    void setRandStartingPosition(HurkaMatrix *roadMatrix);


    /// SlotPath and SlotPositions

    void setSlotPath(SlotPath *_sp);


    void setNeedsPlanning() { needsPlanning = true; }
    void resetNeedsPlanning() { needsPlanning = false; }



    /// Random Utilities

    HPos *rand_iso_pos(int maxM, int maxN);
    HPos *rand_abs_iso_pos(RoadNetwork *roadnet);



    HPos *get_next_pos();




private:

    BusRoute busRoute;        // wishlist: Contains high level route plan for the bus, going from station to station
    SlotPath *slotPath;       // Current Path for the bus

    bool needsPlanning = false;

    HPos *pos;                // Current position, used by draw() and gameUpdate()
    HPos *nextPos;


    Texture texture;
    IntRect textureSize;
    Sprite sprite;

    std::string cn = "Bus.cpp";
};

#endif
