#ifndef GAMEMATRIX_H
#define GAMEMATRIX_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Grid.hpp"
#include "Constants.hpp"
#include "HPos.hpp"


///
/// GameMatrix
///
/// GameMatrix is the "container" for the entire gameboard.
/// For now it does not contain much more than the tiles to show you the size of the board. It's mostly Core that holds all the objects.
///



using namespace sf;


class GameMatrix
{
public:

    GameMatrix() {}
    GameMatrix(int _height, int _width, int _textureID);

    int getWidth();
    int getHeight();

    bool isPosInsideGameMatrix(HPos *);

    void draw( RenderTarget& rt, HPos *viewHPos);



private:
    int width;
    int height;
    int textureID;
    Sprite sprite;
    Texture texture;


    int minX = 7870;
    int minY = 7870;
};

#endif
