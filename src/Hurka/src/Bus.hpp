#ifndef BUS_H
#define BUS_H

#include "SFML/Graphics.hpp"



//du vill inkludera roadnetwork men då blir det CYCLICAL INCLUSION ERROR,... då inkluderar man allt från TrafficManager , the top Dog

class RoadNetwork;


#include "Utils.hpp"
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

    Bus() {}
    Bus(HPos *);


    void reset();
    void gameUpdate(RoadNetwork *roadnet);
    void draw( RenderTarget& rt, HPos *viewHPos, int drawSlotPositions);
    void dump(HPos *viewHPos);


    /// Position

    void update_all_position_vars_on_gpix();
    void update_all_position_vars_on_abs_iso();
    void update_all_nextPos_vars_on_abs_iso();


    void set_pos_on_abs_iso(HPos *abs_iso_pos);
    void set_nextPos_on_abs_iso( HPos *abs_iso_pos);

    void setNext_pix_pos( HPos *_np); // not useful? delete? (2018-04)

    void setRandStartingPosition(HurkaMatrix *roadMatrix);


    /// SlotPath and SlotPositions

    void setSlotPath(SlotPath *_sp);


    void setNeedsPlanning() { needsPlanning = true; }
    void resetNeedsPlanning() { needsPlanning = false; }



    /// Random Utilities

    HPos *getRandomRoad_iso_pos(HurkaMatrix *roadMatrix);
    HPos *rand_iso_pos(int maxM, int maxN); // HPOSTEST!
    HPos *rand_abs_iso_pos(RoadNetwork *roadnet); // HPOSTEST!

    /*Vector2f rand_iso_pos(HurkaMatrix *roadMatrix);       HPOSDELETE
    Vector2f rand_iso_pos(int maxM, int maxN);
    Vector2f rand_abs_iso_pos(RoadNetwork *roadnet);*/

    HPos *get_next_pos();




private:

    BusRoute busRoute;

    HPos *pos;
    HPos *nextPos;

    SlotPath *slotPath;


    bool needsPlanning = false;
    /*
    //HPOSDELETE:
    Vector2f pix_pos;    // Current position in pixels
    Vector2f iso_pos;   // Current position in the isometric matrix
    Vector2f next_iso_pos;
    Vector2f next_pix_pos;*/


    float speed = 0.2f; // Percentage, 100% speed or 150% speed

    int dir = 0;    // 0= UP RIGHT      1= DOWN RIGHT       2= DOWN LEFT       3 = UP LEFT

    Texture texture;
    IntRect textureSize;
    Sprite sprite;



    std::string cn = "Bus.cpp";
};

#endif
