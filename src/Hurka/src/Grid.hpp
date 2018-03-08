#ifndef GRID_H
#define GRID_H


#include <SFML/Graphics.hpp>

#include "Constants.hpp"



using namespace sf;

class Grid
{
public:

    Grid() {}
    Grid(int _height, int _width);

    void draw( RenderTarget& rt, Vector2u viewPos);



// (--)
// DOCS: see "GameMatrix_How_the_x_position_is_calculated.png"
// N = along the right-down axis
// M = along the left-down axis of the gameboard
// width = width of the texture
// height = height of the texture
static int getWindowXPos(int N, int M, int width, int height)
{

    int initialXOffset = 0;   // Start in the middle


    /// N = Width index in the grid
    /// M = Height index in the grid

    /// Calculate the X-offset
    // intialXOffset - where we are in the height=M index TIMES the sprite_width/2
    int xOffset = initialXOffset - M*(GRID_WIDTH/2);


    // now for every step to the right=N index we have to go right a bit

    // TODO: adjust for small width textures! all mine are 64 so... I need more examples
    int xStep = N*(width/2);


    return xOffset + xStep;
}

// (--)
// Used for Blocks and other sprites. But not the Grid. Grid is special.
// DOCS: see "GameMatrix_How_the_y_position_is_calculated.png"
// N = along the right-down axis
// M = along the left-down axis of the gameboard
// width = width of the texture
// height = height of the texture
static int getWindowYPos(int N, int M, int width, int height)
{
    int initialYOffset = GRID_HEIGHT;     // Start at the top

    int yOffset = initialYOffset;



    int yStep = 0;

    yStep = (M*GRID_HEIGHT/4) + (N*GRID_HEIGHT/4) - GRID_HEIGHT/2;

    return yOffset + yStep;
}


private:
    Texture texture;
    Sprite sprite;
    int width;
    int height;
};


#endif
