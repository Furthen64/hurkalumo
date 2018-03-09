#ifndef BUS_H
#define BUS_H

#include "SFML/Graphics.hpp"
#include "Utils.hpp"
#include "GameMatrix.hpp"
#include "TextureManager.hpp"
#include "HurkaMatrix.hpp"
#include "Block.hpp"

using namespace sf;


class Bus
{
public:

    Bus() {}
    Bus(const Vector2f& _iso_pos);

    void draw( RenderTarget& rt, Vector2u viewPos);

    void update(HurkaMatrix *roadMatrix);


    // ISO position
    Vector2f get_iso_pos();
    void set_iso_pos( Vector2f _p);
    void setNext_iso_pos( Vector2f _np);

    // Pixel position
    Vector2f get_pix_pos();
    void set_pix_pos( Vector2f _p);
    void setNext_pix_pos( Vector2f _np);


    void setRandStartingPosition(HurkaMatrix *roadMatrix);
    Vector2f rand_iso_pos(HurkaMatrix *roadMatrix);


private:

    Vector2f pix_pos;    // Current position in pixels
    Vector2f iso_pos;   // Current position in the isometric matrix
    Vector2f next_iso_pos;
    Vector2f next_pix_pos;


    float speed = 10.0f; // Percentage, 100% speed or 150% speed

    int dir = 0;    // 0= UP RIGHT      1= DOWN RIGHT       2= DOWN LEFT       3 = UP LEFT



    Texture texture;
    IntRect textureSize;

    Sprite sprite;


    std::string cn = "Bus.cpp";
};

#endif
