#include "Grid.hpp"



/// //////////////////////////////////////////////
/// Grid           cr 2018-02-22
// The grid follows the static grid size of 64 x 64 px


Grid::Grid(int _height, int _width)
{
    width = _width;
    height = _height;
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_1.png");
    sprite = Sprite(texture);
}


void Grid::draw( RenderTarget& rt)
{
    // haha this will be hell to figure out
    // I need functions for getting the X,Y of the [N,M]th position in the matrix


    // Use a function to figure out what the REAL window X,Y is
    // given the position inside the Isometric Matrix


    int x = 0;
    int y = 0;

    // What a bunny brained idea!
    for(int M= 0; M<height; M++){
        for(int N= 0; N < width; N++) {

            x = getWindowXPos(N,M, GRID_WIDTH, GRID_HEIGHT);
            y = getWindowYPos(N,M, GRID_WIDTH, GRID_HEIGHT);
            Vector2f pos = {(float)x,(float)y};

            sprite.setPosition(pos);
            rt.draw(sprite);
        }

    }



}


