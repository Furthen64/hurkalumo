#ifndef HRECT_H
#define HRECT_H

#include <iostream>
#include <assert.h>

#include <SFML/Graphics.hpp>

#include "HPos.hpp"

using namespace sf;



// PositionTypes { USE_GPIX, USE_ISO };

// Contains an HPos for the starting top left positiong
// and rows and cols to tell you how many nodes it is
// and height and width to tell you how in gpix how large it is on the gameboard

class HRect
{

public:

	HRect(int _absStartY,
			int _absStartX,
			int _rows,
			int _cols,
			int _heightPx,
			int _widthPx);

    HRect(HPos *_absStartPos,
			int _rows,
			int _cols,
			int _heightPx,
			int _widthPx);



    HRect(HPos *_absStartPos, HPos *_absEndPos );

    void calculateBounds();

    void draw(RenderTarget& rt, HPos *viewHPos);

    HRect();

    int nrTiles();

    void testFunctions();

	std::string relToString();
    std::string absToString();

	int compare(HRect *other);
    int compareAbsStartPoint(HRect *other);
	int compareSize(HRect *other);

	bool insideXPixles(HPos *pxPos);
	bool insideYPixles(HPos *pxPos);


	HRect *clone();
    void dump(std::string ind);
    void fullDump(std::string ind);


	// The position and sizes
	HPos *absStart;
	HPos *absEnd;
	int rows;
	int cols;
	HPos *relStart;


	// Boundaries of the rect
	HPos *topB = nullptr;
	HPos *rightB = nullptr;
	HPos *bottomB = nullptr;
	HPos *leftB = nullptr;


	int heightPx;  // FIXME remove the heightpx and widthpx ?
	int widthPx;




private:

    bool drawable = false ;       // OPTIMIZE set to false for faster HRects

    Texture texture;
    Sprite sprite;


    std::string cn = "HRect.cpp";



};

#endif
