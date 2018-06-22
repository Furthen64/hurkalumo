#ifndef GAMEMATRIX_H
#define GAMEMATRIX_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "Grid.hpp"
#include "Constants.hpp"
#include "HRect.hpp" // <-- #include "HPos.hpp"


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


    int getCols();
    int getRows();

    int getWidthPx();
    int getHeightPx();

    bool isPosInsideGameMatrix(HPos *);

    void draw( RenderTarget& rt, HPos *viewHPos);

    void calculatePxBounds();

    HRect *getHRect();


private:

    HPos *startPos;
    int cols;
    int rows;

    int widthPx;
    int heightPx;

   // int textureID; DELETEME
    Sprite sprite;
    Texture texture;


    int minX = 7870;
    int minY = 7870;
};

#endif
