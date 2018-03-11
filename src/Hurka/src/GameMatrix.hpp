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



private:
    int width;
    int height;
    int textureID;
    Sprite sprite;
    Texture texture;
};

#endif
