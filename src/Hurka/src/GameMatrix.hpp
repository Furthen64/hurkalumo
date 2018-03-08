#ifndef GAMEMATRIX_H
#define GAMEMATRIX_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Grid.hpp"
#include "Constants.hpp"

/// //////////////////////////////////////////////
/// (-+) GameMatrix is the grid with all the visible sprites oh shit, its the whole game set right?
/// Or something. Haven't decided yet.
///

using namespace sf;


class GameMatrix
{
public:

    GameMatrix() {}
    GameMatrix(int _height, int _width, int _textureID);

    int getWidth();

    int getHeight();


    void draw( RenderTarget& rt,  Vector2u viewPos);



    static Vector2f convert_iso_to_pix(Vector2f iso_pos, int width, int height)
    {
        Vector2f newPos;
        newPos.x = pix_pos_x(iso_pos.x, iso_pos.y, width, height);
        newPos.y = pix_pos_y(iso_pos.x, iso_pos.y, width, height);
        return newPos;
    }

     // (-+)
    // DOCS: see "GameMatrix_How_the_x_position_is_calculated.png"
    /// N = x = along the width axis
    /// M = y = along the height axis of the gameboard
    /// width = width of the texture
    /// height = height of the texture
    static int pix_pos_x(int N, int M, int width, int height)
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
    // DOCS: see "GameMatrix_How_the_y_position_is_calculated.png"
    // N = along the width axis
    // M = along the height axis of the gameboard
    // width = width of the texture
    // height = height of the texture
    static int pix_pos_y(int N, int M, int width, int height)
    {
        int initialYOffset = 0;     // Start at the top

        int yOffset = initialYOffset;



        int yStep = 0;

        if(height < GRID_HEIGHT) {
            // Smaller, IF the sprite height is less than the 64 GRID height we have to move it down
            // the division by 4 is now division by 2

            yStep =  (M*GRID_HEIGHT/4) + (N*GRID_HEIGHT/4);

        } else if(height > GRID_HEIGHT) {
            // Taller, like high buildings, make sure you start drawing HIGHER (lower y value)
             yStep = (M*GRID_HEIGHT/4) - (N*GRID_HEIGHT/4);

        } else {
            // (++)
            // Equal to the grid size
            yStep = (M*GRID_HEIGHT/4) + (N*GRID_HEIGHT/4);
        }


        return yOffset + yStep;
    }



private:
    int width;
    int height;
    int textureID;
    Sprite sprite;
    Texture texture;
};

#endif
