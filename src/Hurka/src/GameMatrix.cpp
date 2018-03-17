
#include "GameMatrix.hpp"



/// //////////////////////////////////////////////
/// (-+) GameMatrix is the grid with all the visible sprites oh shit, its the whole game set right?
/// Or something. Haven't decided yet.
///




GameMatrix::GameMatrix(int _height, int _width, int _textureID)
{
    height = _height;
    width = _width;
    textureID = _textureID;


    // Hardcoded for now
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRASS_1.png");
    sprite = Sprite(texture);

    if( (height%2 != 0) || (width%2 != 0)) {
        std::cout << "Warning! GameMatrix height Not divisible by 2! \n";
    }


}

int GameMatrix::getWidth()
{
    return width;
}

int GameMatrix::getHeight()
{
    return height;
}



/// (--)
/// Optimize with the ints maybe make it a const?


/// Add Clipping to this!
// If you are drawing outside the viewing 800x600 px window, you could just NOT draw that sprite, make ac
// clipping rectangle and compare to it


void GameMatrix::draw( RenderTarget& rt, Vector2i viewPos)
{
    minX = 9990;
    minY = 9990;
    int x = 0;
    int y = 0;
    int actualXMin = 9999;
    int actualYMin= 9999;

    // What a bunny brained idea!
    for(int M= 0; M<height; M++){
        for(int N= 0; N < width; N++) {

        // Hey! I'm using the Grid's positioning functions instead of this class's
        // because the gamematrix's green background should follow the grid
            x = Grid::convert_iso_to_gpix_x(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);
            y = Grid::convert_iso_to_gpix_y(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);

            if( x <actualXMin) {
                actualXMin = x;
            }

            if( y < actualYMin) {
                actualYMin = y;
            }

            // Offset for viewing position
            x += viewPos.x;
            y += viewPos.y;

            Vector2f pos = {(float)x,(float)y};

            if(x < minX) {
                minX = x;
            }

            if(y < minY) {
                minY = y;
            }

            sprite.setPosition(pos);


            rt.draw(sprite);
        }

    }




  //  std::cout << "DRAW gamematrix (min y= " << minY << ", minX=" << minX << ")  but actual pix pos (" << actualXMin << "," << actualYMin << ")\n";

}

