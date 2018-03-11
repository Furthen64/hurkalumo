
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

void GameMatrix::draw( RenderTarget& rt, Vector2u viewPos)
{
    int x = 0;
    int y = 0;

    // What a bunny brained idea!
    for(int M= 0; M<height; M++){
        for(int N= 0; N < width; N++) {

        // Hey! I'm using the Grid's positioning functions instead of this class's
        // because the gamematrix's green background should follow the grid
            x = Grid::convert_iso_to_pix_x(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);
            y = Grid::convert_iso_to_pix_y(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);


            // Offset for viewing position
            x += viewPos.x;
            y += viewPos.y;

            Vector2f pos = {(float)x,(float)y};

            sprite.setPosition(pos);


            rt.draw(sprite);
        }

    }

}

