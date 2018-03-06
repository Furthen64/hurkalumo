
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



// (-+)
void GameMatrix::draw( RenderTarget& rt)
{
    int x = 0;
    int y = 0;

    // What a bunny brained idea!
    for(int M= 0; M<height; M++){
        for(int N= 0; N < width; N++) {

        // Hey! I'm using the Grid's positioning functions instead of this class's
        // because the gamematrix's green background should follow the grid
            x = Grid::getWindowXPos(N,M, GRID_WIDTH, GRID_HEIGHT);
            y = Grid::getWindowYPos(N,M, GRID_WIDTH, GRID_HEIGHT);
            Vector2f pos = {(float)x,(float)y};

            sprite.setPosition(pos);
            rt.draw(sprite);
        }

    }

}

