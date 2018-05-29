#ifndef HRECT_H
#define HRECT_H

#include <iostream>
#include <assert.h> 

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
			int _relStartY, 
			int _relStartX, 
			int _height,
			int _width);
			
    HRect(HPos *_absStartPos, 
			int _rows, 
			int _cols, 
			HPos *_relStartPos, 
			int _height,
			int _width);
			
    HRect();
    
	std::string relToString();
    std::string absToString();
    
	int compare(HRect *other);
    int compareAbsStartPoint(HRect *other);
	int compareSize(HRect *other);
	
	HRect *clone();
    void dump(std::string ind);

	
	// The position and sizes
	HPos *absStart;
	int rows;
	int cols;
	HPos *relStart;
	int heightPx;
	int widthPx;
	
	
    

private:



};

#endif
