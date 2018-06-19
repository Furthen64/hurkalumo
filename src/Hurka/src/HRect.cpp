#include "HRect.hpp"

#include "Utils.hpp"



/// \brief Creates an empty rectangle with invalid values
HRect::HRect()
{
    absStart = new HPos(-1,-1, USE_ISO);
    rows = -1;
    cols = -1;
    heightPx = -1;
    widthPx = -1;
}


/// \brief Creates a rectangle given starting positions as integers
// (--)
HRect::HRect(int _absStartY,
		int _absStartX,
		int _rows,
		int _cols,
		int _heightPx,
		int _widthPx)
{


    // setup the position and sizes

	absStart = new HPos(_absStartY, _absStartX, USE_ISO);
	rows = _rows;
	cols = _cols;
	relStart = new HPos(0,0, USE_ISO);      // the relative starts at 0,0  aaaaaalways

	heightPx = _heightPx;   // Pixel height of texture (w/ mask)
	widthPx = _widthPx;     // Pixel width of texture (w/ mask)



	if(drawable) {

        texture.loadFromFile(getFullUri("data\\textures\\GRID2.png"));
        sprite = Sprite(texture);
	}
}





/// \brief Creates a rectangle given an HPos starting position
// (--)
HRect::HRect(HPos *_absStartPos,
        int _rows,
        int _cols,
        int _heightPx,
        int _widthPx)
{
    absStart = _absStartPos;
    rows = _rows;
    cols = _cols;
    heightPx = _heightPx;
    widthPx = _widthPx;

}



/// \brief Creates a rectangle given two positions, calculates rows and cols automatically
// (--)
HRect::HRect(HPos *_absStartPos, HPos *_absEndPos)
{
    absStart = _absStartPos;
    rows = _absEndPos->abs_iso_y - absStart->abs_iso_y;
    cols = _absEndPos->abs_iso_x - absStart->abs_iso_x;

    std::cout << "new HRECT:   rows=" << rows << ", cols=" << cols << "\n";

    heightPx = -1;
    widthPx = -1;        //FIXME: please update these values with a calculateBounds()

    calculateBounds();

}

void HRect::calculateBounds()
{
   int _heightPx = 0;
   int _widthPx = 0;

   // GO over the isometric field and figure out the min and max in each corner,
   // set the heihgpx and widthpx of the HRect

   std::cout << "NOT CODED YET!!!\n";

}







int HRect::nrTiles()
{
    return rows * cols;
}



// Please note: +-1 error possible, should it go to <= or to < when matching bounds? should we have a function insideXPixlesOrOnBounds which uses >= and <= ?
// (--) Test
bool HRect::insideXPixles(HPos *pxPos)
{

    int debugLevel = 1;

    int thisLeft = this->absStart->gpix_x;
    int thisRight = this->widthPx;

    int searchX = pxPos->gpix_x;

    if(debugLevel >= 1)
    {
        std::cout << "hrect.left = " << thisLeft << "    vs   pxPos.left= " << searchX <<  " vs  hrect.right= " << thisRight << "\n";
    }

    if(thisLeft < searchX && searchX < thisRight) {
        return true;
    }



    return false;
}




// (--) test
bool HRect::insideYPixles(HPos *pxPos)
{

    int thisTop   = this->absStart->gpix_y;
    int thisBottom = this->heightPx;

    int searchY = pxPos->gpix_y;

    if(thisTop < searchY && searchY < thisBottom) {
        return true;
    }



    return false;
}





// (--) test
HRect *HRect::clone()
{

    HRect *other = new HRect(this->absStart->clone(), this->rows, this->cols, this->heightPx, this->widthPx);

    other->relStart = this->relStart->clone();

    return other;
}




// (--)
void HRect::dump(std::string ind)
{
    std::cout << ind << "HRect " << absStart->absToString() << " to (" << absStart->abs_iso_y + rows << ", " << absStart->abs_iso_x + cols << ")\n";
}




// Regression test
// (--)
void HRect::testFunctions()
{
    std::cout << "\n\n HRect - testFunctions()----------------------------\n";
    HRect *rect1 = new HRect(0,0, 10,10, 32, 64);   // GRID size
    HRect *rect2 = new HRect(0,0, 10,10, 32, 64);   // GRID size
    HRect *rect3 = nullptr;
    HRect *rect4a = new HRect(new HPos(1,0, USE_ISO), 10,10, 32, 64);
    HRect *rect4b = new HRect(1,0 , 10,10, 32, 64);

    std::cout << "\n\nRECT1:\n";
    rect1->dump("  ");
    std::cout << "\n\nRECT2:\n";
    rect2->dump("  ");
    rect3 = rect1->clone();
    std::cout << "\n\nRECT3 (clone of rect1):\n";
    rect3->dump("  ");
    std::cout << "\n\nRECT4a:\n";
    rect4a->dump("  ");
    std::cout << "\n\nRECT4b:\n";
    rect4b->dump("  ");



    std::cout << "\n\nTesting absToString:\n";
    std::cout << "  " <<  rect1->absToString() << "\n";
    std::cout << "  " << rect2->absToString() << "\n";
    std::cout << "  " << rect3->absToString() << "\n";
    std::cout << "  " << rect4a->absToString() << "\n";
    std::cout << "  " << rect4b->absToString() << "\n";




    // Should not fail
    assert(rect1->compare(rect2) == 0);
    assert(rect1->compare(rect3) == 0);
    assert(rect2->compare(rect1) == 0);
    assert(rect2->compare(rect3) == 0);

    assert(rect4a->compareAbsStartPoint(rect4b) == 0);










    std::cout << "\n HRect Tests complete -------------------------\n\n";

}


// (--) Test
int HRect::compare(HRect *other)
{

    // Compare positions
    if(other->absStart->compare(this->absStart) == 0) {

        if(other->rows == this->rows &&
           other->cols == this->cols &&
           other->heightPx == this->heightPx &&
           other->widthPx == this->widthPx) {

               if(other->relStart->compare(this->relStart) == 0) {
                    return 0;
               }
           }
    }

    return -1;

}



// (--) test
int HRect::compareAbsStartPoint(HRect *other)
{
    if(other->absStart->compare(this->absStart) == 0) {
        return 0;
    }

    return -1;


}


// (--) test
int HRect::compareSize(HRect *other)
{
    if(this->rows == other->rows
       &&
       this->cols == other->cols) {
           return 0;
       }

    return -1;
}





std::string HRect::relToString()
{
    return relStart->relToString();
}



std::string HRect::absToString()
{
    std::string str = "HRect ";
    str += absStart->absToString();
    str += " to (";
    str += std::to_string(absStart->abs_iso_y + rows);
    str += ", " ;
    str += std::to_string(absStart->abs_iso_x + cols);
    str += ")";
    return str;
}









/// Uses the internal start "hpos" object for gameworld position (e.g. it's at iso position {7,1} and at gamepix position {1000,600} )
/// and the "viewpos" viewing position object to know where we are looking at the moment
///
/// @param rt       SFML RenderWindow thingie
/// @param viewPos  A rectangle which holds the starting x and starting y for looking at the gameboard
// (--)
void HRect::draw(RenderTarget& rt, HPos *viewHPos)
{
    if(drawable) {

        // Create Grid objects and draw them as you would the grid, but with different texture

        for(int Y=this->absStart->abs_iso_y; Y < (this->absStart->abs_iso_y + this->rows); Y++) {

            for(int X=this->absStart->abs_iso_x; X < (this->absStart->abs_iso_x + this->cols); X++) {


            }
        }


    }
    /*

    int x = this->absStart->gpix_x;
    int y = this->absStart->gpix_y;


    // Viewing offset
    x += viewHPos->gpix_x;
    y += viewHPos->gpix_y;


    Vector2f _pos = {(float)x,(float)y};


    sprite.setPosition(_pos);

    rt.draw(sprite);
        */
}









	/*

/// @param _y Vertical positioning, or on the isometric board= M the sloping down and left.
/// @param _x Horizontal positioning, or on the isometric board= N the sloping down and right
/// @param positionType USE_ISO or USE_GPIX , choose what values to store away.
/// (++)
HPos::HPos(int _y, int _x, int positionType)
{
    if(positionType == USE_ISO) {
        abs_iso_y = _y;
        abs_iso_x = _x;
        rel_iso_y = _y;
        rel_iso_x = _x;
        gpix_y = 0;
        gpix_x = 0;
    }

    if(positionType == USE_GPIX) {
        abs_iso_y = 0;
        abs_iso_x = 0;
        rel_iso_y = 0;
        rel_iso_x = 0;
        gpix_y = _y;
        gpix_x = _x;

    }
}


// (++)
HPos::HPos()
{
    abs_iso_y = 0;
    abs_iso_x = 0;
    rel_iso_y = 0;
    rel_iso_x = 0;
    gpix_y = 0;
    gpix_y = 0;
}



// (+-)
std::string HPos::absToString()
{
    std::string str = "(";

    str += std::to_string(abs_iso_y);

    str += "," ;

    str += std::to_string(abs_iso_x);

    str += ")";

    return str;
}





// (-+)
std::string HPos::relToString()
{
    std::string str = "(";

    str += std::to_string(rel_iso_y);

    str += "," ;

    str += std::to_string(rel_iso_x);

    str += ")";

    return str;
}



/// \brief If "other" is an exact match to "this", we return 0.
/// \return 0 on exact match, -1 if not.
// (--) TEST
int HPos::compare(HPos *other)
{

    if(other->abs_iso_y == this->abs_iso_y
       &&
       other->abs_iso_x == this->abs_iso_x
       &&
       other->gpix_y == this->gpix_y
       &&
       other->gpix_x == this->gpix_x
       &&
       other->rel_iso_y == this->rel_iso_y
       &&
       other->rel_iso_x == this->rel_iso_x
    )
    {
        return 0;
    }

    return -1;
}



/// \brief If "Other" has exact match with "this" on absolute_iso positioning, return 0.
/// \return 0 on match, -1 if not.
// (--)
int HPos::compareAbsIso(HPos *other)
{

    if(other->abs_iso_y == this->abs_iso_y
       &&
       other->abs_iso_x == this->abs_iso_x)
    {
        return 0;
    }

    return -1;

}




// Used by roadnetwork.cpp in createSlotPathFromDijkstraResult
// Makes a slotpos appear in the middle of an isometric tile/block
// (-+)
void HPos::transform_gpix_to_slotpos(SlotPos *slotpos, HPos *hpos)
{
    slotpos->hpos->gpix_y += 12;
    slotpos->hpos->gpix_x += 10;
}




/// @brief Copies all the values from the original to the new one, returns newly allocated HPos
// (-+) Seems to work.. // 2018-05-10
HPos *HPos::clone()
{
    HPos *_pos = new HPos(abs_iso_y, abs_iso_x, USE_ISO);
    _pos->rel_iso_y = rel_iso_y;
    _pos->rel_iso_x = rel_iso_x;
    _pos->gpix_y = gpix_y;
    _pos->gpix_x = gpix_x;

    return _pos;
}



// (++)
void HPos::dump(std::string ind)
{
    std::cout << ind << "{\n";
    std::cout << ind << "  abs_iso   y=" << abs_iso_y << ", x=" << abs_iso_x << "\n";
    std::cout << ind << "  rel_iso   y=" << rel_iso_y << ", x=" << rel_iso_x << "\n";
    std::cout << ind << "  gpix      y=" << gpix_y << ", x=" << gpix_x << "\n";
    std::cout << ind << "}\n";
}







/// TEST FUNCTIONS


// private function
void HPos::testFunctions()
{


    HPos *A = new HPos(0,0,USE_ISO);
    HPos *B = new HPos(0,1,USE_ISO);
    HPos *C = new HPos(2,2,USE_ISO);

    HPos *D = new HPos(320,240,USE_GPIX);
    HPos *E = new HPos(480,240,USE_GPIX);
    HPos *F = new HPos(240,480,USE_GPIX);


    HPos *AA = new HPos(0,0,USE_ISO);
    HPos *BB = new HPos(0,1,USE_ISO);
    HPos *CC = new HPos(2,2,USE_ISO);

    HPos *DD = new HPos(320,240,USE_GPIX);
    HPos *EE = new HPos(480,240,USE_GPIX);
    HPos *FF = new HPos(240,480,USE_GPIX);




    // Comparing ISO values
    assert(HPos::compareTwoAbsIso(A,AA) == 0);
    assert(HPos::compareTwoAbsIso(B,BB) == 0);
    assert(HPos::compareTwoAbsIso(C,CC) == 0);



    // Comparing GPIX values
    assert(HPos::compareTwoGpix(D,DD) == 0);
    assert(HPos::compareTwoGpix(E,EE) == 0);
    assert(HPos::compareTwoGpix(F,FF) == 0);


    std::cout << "These SHOULD fail: \n";

    //assert(HPos::compareTwoAbsIso(A,BB) == 0);
    //assert(HPos::compareTwoAbsIso(B,AA) == 0);
    //assert(HPos::compareTwoAbsIso(C,AA) == 0);

    //assert(HPos::compareTwoGpix(D,EE) == 0);
    //assert(HPos::compareTwoGpix(E,FF) == 0);
    //assert(HPos::compareTwoGpix(F,EE) == 0);




}
*/

