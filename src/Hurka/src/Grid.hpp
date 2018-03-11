#ifndef GRID_H
#define GRID_H


#include <SFML/Graphics.hpp>

#include "GameMatrix.hpp"
#include "Constants.hpp"
#include "Utils.hpp"



using namespace sf;


/// Contains the Grid, but also a selected grid piece that will light up, allowing for selecting or something
///
///
class Grid
{
public:

    Grid() {}
    Grid(int _height, int _width);

    void draw( RenderTarget& rt, Vector2u viewPos);

    void setVisible(Vector2f iso_pos);

    void hideVisible();




// (--)
// DOCS: see "GameMatrix_How_the_x_position_is_calculated.png"
// N = along the right-down axis
// M = along the left-down axis of the gameboard
// width = width of the texture
// height = height of the texture
static int getWindowXPos(int M, int N, int width, int height)
{




/// TEST , du måste testa och uppdatera alla dessa funktioner.... synkronisera dom

    int initialXOffset = GRID_TEXTURE_WIDTH / 4 * NR_GRIDS_HEIGHT;   // Make sure we place everything in the x-positive euclidian space


    // N = Width index in the grid
    // M = Height index in the grid

    // Calculate the X-offset
    // intialXOffset - where we are in the height=M index TIMES the sprite_width/2
    int xOffset = initialXOffset - N*(GRID_TEXTURE_WIDTH/2);


    // now for every step to the right=N index we have to go right a bit

    // TODO: adjust for small width textures! all mine are 64 so... I need more examples
    int xStep = M*(width/2);


    return xOffset + xStep;
}

// (--)
// Used for Blocks and other sprites. But not the Grid. Grid is special.
// DOCS: see "GameMatrix_How_the_y_position_is_calculated.png"
// N = along the right-down axis
// M = along the left-down axis of the gameboard
// width = width of the texture
// height = height of the texture
static int getWindowYPos(int M, int N, int width, int height)
{
    int initialYOffset = GRID_TEXTURE_HEIGHT*2;     // Start at the top

    int yOffset = initialYOffset;

    int yStep = 0;

    yStep = (N*GRID_TEXTURE_WIDTH/4) + (M*GRID_TEXTURE_WIDTH/4) - GRID_TEXTURE_WIDTH/2;     // Using WIDTH, because its 64 px not 32 px like the HEIGHT

    return yOffset + yStep;
}


// (--)
/// TEsta!! och gör bättre, diamantform
static Vector2f convert_pix_to_iso(Vector2f pix_pos, int width, int height)
{

std::string cn = "Grid.hpp";

    std::cout << "bus pix pos = ";
    dumpPosition(pix_pos);


    int x = 0;
    int y = 0;
    Vector2f pos = Vector2f();
    Vector2f iso_pos = Vector2f();

    if(width == 0 || height == 0 ) {
        std::cout << "ERROR " << cn << " height or width = 0 in call to convert_pix_to_iso!!\n";
        return pos;
    }


    // if we are inside the diamond of that cell return that iso-pos
    for(int M= 0; M< NR_GRIDS_HEIGHT; M++){
        for(int N= 0; N < NR_GRIDS_WIDTH; N++) {

            x = getWindowXPos(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT);
            y = getWindowYPos(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT);

            pos.x = x;
            pos.y = y;

            if( (pix_pos.x > pos.x)  && (pix_pos.x <= (pos.x+GRID_TEXTURE_WIDTH))) {

                if( (pix_pos.y > pos.y) && (pix_pos.y <= (pix_pos.y+GRID_TEXTURE_HEIGHT))) {

                    std::cout << "found it at pix pos ";
                    dumpPosition(pos);
                    // within the square

                    iso_pos.y = M;
                    iso_pos.x = N;
                    return iso_pos;
                }
            }

        }

    }

    std::cout << "Could not find the object inside any of the grid's cell!\n";
    return iso_pos;

}


private:
    Texture texture;
    Texture textureSelected;
    Sprite sprite;

    Sprite spriteSelected;
    Vector2f selected_pix_pos;
    Vector2f selected_iso_pos;
    bool drawSelectedGrid = false;

    int width;
    int height;
};


#endif
