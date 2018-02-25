#ifndef BLOCK_H
#define BLOCK_H



#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "GameMatrix.hpp"
#include "Constants.hpp"


using namespace sf;

/// //////////////////////////////////////////////
/// (--) Block
/// ? x ? pixels        Auto adjusted where its positioned for drawing based upon its texture resolution

class Block
{
public:
    Block(const Vector2f& _pos, std::string _textureName, TextureManager *textureMgr);

    void draw( RenderTarget& rt);

    int getTextureID();

     // (-+)
    // DOCS: see "GameMatrix_How_the_x_position_is_calculated.png"
    // N = along the width axis
    // M = along the height axis of the gameboard
    // width = width of the texture
    // height = height of the texture
    static int getWindowXPos(int N, int M, int width, int height)
    {

        int initialXOffset = 400;   // Start in the middle


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
    static int getWindowYPos(int N, int M, int width, int height)
    {
        int initialYOffset = 0;     // Start at the top

        int yOffset = initialYOffset;



        int yStep = 0;

        if(height < GRID_HEIGHT) {
            // Smaller, IF the sprite height is less than the 64 GRID height we have to move it down
            // the division by 4 is now division by 2

            yStep =  (M*GRID_HEIGHT/4) + (N*GRID_HEIGHT/4) + 32;

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
    int textureID;
    Texture texture;
    Sprite sprite;
    Vector2f pos;
    IntRect textureSize;

};

#endif
