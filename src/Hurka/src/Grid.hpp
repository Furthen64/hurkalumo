#ifndef GRID_H
#define GRID_H

#include <sstream>

#include <SFML/Graphics.hpp>

#include "HPos.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include "HRect.hpp"


using namespace sf;


/// Contains the Grid, but also a selected grid piece that will light up, allowing for selecting or something
///
///
class Grid
{
public:

    Grid() {}
    Grid(int _height, int _width);

    void draw( RenderTarget& rt, HPos *viewHPos);

    void setVisible(HPos *_pos);



    void hideVisible();



    // Search functions

    HPos *findTile(HRect *entireGameboard, HRect *relRect, HPos *searchPos, std::string ind);

    HPos *bruteForceFindTile(HRect *entireGameboard, HRect *relRect, HPos *searchPos, std::string ind);





// (-+)
// DOCS: see "GameMatrix_How_the_x_position_is_calculated.png"
// N = along the right-down axis
// M = along the left-down axis of the gameboard
// width = width of the texture
// height = height of the texture
// typeOfElement        0,1     =   GRID or GAMEMATRIX
//                        2     =   BLOCK
/// Wishlist: Make a better documentation about this


static int convert_iso_to_gpix_x(int M, int N, int width, int height, int typeOfElement)
{

    std::string cn = "Grid.hpp";
    if(width == 0 || height == 0) {
        std::cout << "ERROR " << cn << " height or width = 0 in call to convert_iso_to_pix_x!!\n";
        return -1;
    }

    int globalXOffset= 1500;
    int initialXOffset = 0;
    int xOffset = 0;
    int xStep = 0;



    if(typeOfElement == 0 || typeOfElement == 1) {
        /// GRID or GAMEMATRIX


        initialXOffset = globalXOffset + (GRID_TEXTURE_WIDTH / 4 * NR_GRIDS_HEIGHT);   // Make sure we place everything in the x-positive euclidian space


        // Calculate the X-offset
        // intialXOffset - where we are in the height=M index TIMES the sprite_width/2
        xOffset = initialXOffset - M*(GRID_TEXTURE_WIDTH/2);


        // now for every step to the right=N index we have to go right a bit

        // TODO: adjust for small width textures! all mine are 64 so... I need more examples
        xStep = N*(width/2);




    } else if (typeOfElement == 2) {


        /// BLOCK

        initialXOffset = globalXOffset + (GRID_TEXTURE_WIDTH / 4 * NR_GRIDS_WIDTH);   // Make sure we place everything in the x-positive euclidian space


        /// Calculate the X-offset
        // intialXOffset - where we are in the height=M index TIMES the sprite_width/2
        xOffset = initialXOffset - M*(GRID_TEXTURE_WIDTH/2);


        // now for every step to the right=N index we have to go right a bit

        // TODO: adjust for small width textures! all mine are 64 so... I need more examples
        xStep = N*(width/2);


    }


    return xOffset + xStep;
}


/// \param M = along the left-down axis of the gameboard
/// \param N = along the right-down axis
/// \param width = width of the texture
/// \param height = height of the texture
/// \param typeOfElement 0=GRID, 1=GAMEMATRIX, 2=BLOCK
/// DOCS: see "GameMatrix_How_the_y_position_is_calculated.png"
static int convert_iso_to_gpix_y(int M, int N, int width, int height, int typeOfElement)

{

    std::string cn = "Grid.hpp";
    if(width == 0 || height == 0) {
        std::cout << "ERROR " << cn << " height or width = 0 in call to convert_iso_to_pix_y!!\n";
        return -1;
    }

    int globalYOffset= 500;
    int initialYOffset = 0;
    int yOffset = 0;
    int yStep = 0;


    if(typeOfElement == 0 || typeOfElement == 1) {

         /// GRID or GAMEMATRIX

         initialYOffset = globalYOffset + GRID_TEXTURE_HEIGHT*2;     // Start at the top

         yOffset = initialYOffset;

         yStep = 0;

         //FIXME PERHAPS - Could be bug here... not using M and N correctly, see the codeblock just below using it in another order.

        yStep = (N*GRID_TEXTURE_WIDTH/4) + (M*GRID_TEXTURE_WIDTH/4) - GRID_TEXTURE_WIDTH/2;     // Using WIDTH, because its 64 px not 32 px like the HEIGHT

    } else if(typeOfElement == 2) {

        ///BLOCK

         initialYOffset = globalYOffset + 0;     // Start at the top

         yOffset = initialYOffset;


         yStep = 0;

        if(height < GRID_TEXTURE_WIDTH) {
            // Smaller, IF the sprite height is less than the 64 GRID height we have to move it down
            // the division by 4 is now division by 2

            yStep =  (M*GRID_TEXTURE_WIDTH/4) + (N*GRID_TEXTURE_WIDTH/4) + 32;

        } else if(height > GRID_TEXTURE_WIDTH) {
            // Taller, like high buildings, make sure you start drawing HIGHER (lower y value)
             yStep = (M*GRID_TEXTURE_WIDTH/4) - (N*GRID_TEXTURE_WIDTH/4);

        } else {
            // (++)
            // Equal to the grid size
            yStep = (M*GRID_TEXTURE_WIDTH/4) + (N*GRID_TEXTURE_WIDTH/4);
        }
    }


    return yOffset + yStep;
}



/*
static Vector2f convert_iso_to_gpix(Vector2f iso_pos, int width, int height)
{

    std::string cn = "Grid.hpp";


    Vector2f pix_pos = Vector2f();

    if(width == 0 || height == 0) {
        std::cout << "ERROR " << cn << " height or width = 0 in call to convert_iso_to_pix!!\n";
        return pix_pos;
    }

    pix_pos.x = convert_iso_to_gpix_x(iso_pos.y, iso_pos.x, width, height,0 );
    pix_pos.y = convert_iso_to_gpix_y(iso_pos.y, iso_pos.x, width, height,0);

    return pix_pos;

}*/






/// Takes gpix data, looks at the grid and finds the isometric tile position where that gameboard pixel position would be in
/// @param pix_pos  Already allocated HPos with values set on gpix positions
/// @param width    The size of the sprite
/// @param height   The size of the sprite
/// @return returns If pix_pos is inside the gameboard, return HPos with correct iso values otherwise return -1,-1 iso values
// wishlist: Make it work with the isometric tile shape
// (-+)
static HPos *convert_gpix_to_iso(HPos *pix_pos, int width, int height)
{
    int debugLevel = 0;


    std::string cn = "Grid.hpp";

    int x = 0;
    int y = 0;

    HPos *work_pos = new HPos(0,0, USE_GPIX);
    HPos *iso_pos = new HPos(-1,-1,USE_GPIX);       // Default is a -1,-1 which is the position to return if we endup with an error, like not finding it

    if(width == 0 || height == 0 ) {
        std::cout << "ERROR " << cn << " height or width = 0 in call to convert_pix_to_iso!!\n";
        return nullptr;
    }



    /// alpha-0.2: if we are inside the diamond of that cell return that iso-pos
    /// also, please change all M to Y? all N to X?

    for(int M= 0; M< NR_GRIDS_HEIGHT; M++){

        for(int N= 0; N < NR_GRIDS_WIDTH; N++) {

            x = convert_iso_to_gpix_x(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
            y = convert_iso_to_gpix_y(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);


            work_pos->gpix_y = y;
            work_pos->gpix_x = x;



            if( (pix_pos->gpix_x > work_pos->gpix_x)  && (pix_pos->gpix_x <= (work_pos->gpix_x+GRID_TEXTURE_WIDTH))) {

                if( (pix_pos->gpix_y > work_pos->gpix_y) && (pix_pos->gpix_y <= (work_pos->gpix_y+GRID_TEXTURE_HEIGHT))) {

                    // Within the square!

                    // Store away iso info
                    iso_pos->abs_iso_y = M;
                    iso_pos->abs_iso_x = N;

                    // Store away gpix info too
                    iso_pos->gpix_y = pix_pos->gpix_y;
                    iso_pos->gpix_x = pix_pos->gpix_x;

                    return iso_pos;
                }
            }

        }

    }


    // Setup for failure!

    iso_pos->abs_iso_y = -1;
    iso_pos->abs_iso_x = -1;
    iso_pos->rel_iso_y = -1;
    iso_pos->rel_iso_x = -1;


    if(debugLevel >=2) { std::cout << "Could not find the object inside any of the grid's cell!\n"; }
    return iso_pos;
}





/// HPOSDELETE:
/*
/// Är det GAME eller WINDOW position på koordinaterna?? skriv så i namnet, verkar som Game för man gör inget med viewboxen
/// TEsta!! och gör bättre, diamantform
// BUGG M och N är confused för någon anledning
static Vector2f convert_pix_to_iso(Vector2f pix_pos, int width, int height)
{

    std::string cn = "Grid.hpp";

    int x = 0;
    int y = 0;

    Vector2f work_pos = Vector2f();
    Vector2f iso_pos = Vector2f();

    if(width == 0 || height == 0 ) {
        std::cout << "ERROR " << cn << " height or width = 0 in call to convert_pix_to_iso!!\n";
        return work_pos;
    }



    /// todo - if we are inside the diamond of that cell return that iso-pos

    for(int M= 0; M< NR_GRIDS_HEIGHT; M++){

        for(int N= 0; N < NR_GRIDS_WIDTH; N++) {

            x = convert_iso_to_gpix_x(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
            y = convert_iso_to_gpix_y(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);

            work_pos.x = x;
            work_pos.y = y;

            if( (pix_pos.x > work_pos.x)  && (pix_pos.x <= (work_pos.x+GRID_TEXTURE_WIDTH))) {

                if( (pix_pos.y > work_pos.y) && (pix_pos.y <= (work_pos.y+GRID_TEXTURE_HEIGHT))) {

                    // within the square

                    iso_pos.y = M;
                    iso_pos.x = N;
                    return iso_pos;
                }
            }

        }

    }


    //std::cout << "Could not find the object inside any of the grid's cell!\n";
    return iso_pos;

}

*/





private:
    Texture texture;
    Texture textureSelected;
    Sprite sprite;

    Sprite spriteSelected;


    Vector2f selected_pix_pos;      // HPOSDELETE FIXME
    Vector2f selected_iso_pos;
    bool drawSelectedGrid = false;

    int width;
    int height;

    Font font;
    Text text;
    Text visibleGridText;

    std::string cn = "Grid.cpp";

};


#endif
