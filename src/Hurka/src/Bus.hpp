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




using namespace sf;


class Bus
{
public:

    Bus() {}
    Bus(HPos *_pos);


    void draw( RenderTarget& rt, Vector2i *viewPos);
    void dump(Vector2i *viewPos);


    /// Position

    void update_all_position_vars_on_gpix();
    void update_all_position_vars_on_abs_iso();
    void gameUpdate(RoadNetwork *roadnet);

    void update_all_nextPos_vars_on_abs_iso();

    void set_pos_on_abs_iso(HPos *abs_iso_pos);
    void set_nextPos_on_abs_iso( HPos *abs_iso_pos);




    void setNext_pix_pos( HPos *_np);   // Please replace all of these with HPos functions instead
    void setNext_iso_pos( Vector2f _np);
    void setNext_pix_pos( Vector2f _np);

    void setRandStartingPosition(HurkaMatrix *roadMatrix);

    Vector2f rand_iso_pos(HurkaMatrix *roadMatrix);
    Vector2f rand_iso_pos(int maxM, int maxN);
    Vector2f rand_abs_iso_pos(RoadNetwork *roadnet);




    HPos *get_next_pos();
    Vector2f get_next_iso_pos();




private:

    BusRoute busRoute;

    HPos *pos;
    HPos *nextPos;

//HPOSDELETE
    Vector2f pix_pos;    // Current position in pixels
    Vector2f iso_pos;   // Current position in the isometric matrix
    Vector2f next_iso_pos;
    Vector2f next_pix_pos;


    float speed = 0.2f; // Percentage, 100% speed or 150% speed

    int dir = 0;    // 0= UP RIGHT      1= DOWN RIGHT       2= DOWN LEFT       3 = UP LEFT

    Texture texture;
    IntRect textureSize;
    Sprite sprite;



    std::string cn = "Bus.cpp";
};

#endif
