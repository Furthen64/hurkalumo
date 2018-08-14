#include "HPos.hpp"

#include "SlotPos.hpp"  // because of Forward declaration I have to include this hpp in the .cpp file

#include "Grid.hpp"



/// \brief USE_ISO: Creates an HPos with rows and cols, assumes it's a grid sized object and creates gpix values thereof
/// \param _y Vertical positioning, or on the isometric board= M the sloping down and left.
/// \param _x Horizontal positioning, or on the isometric board= N the sloping down and right
/// \param positionType USE_ISO or USE_GPIX , choose what values to store away.
/// \param typeOfElement 0=GRID, 1=GAMEMATRIX, 2=BLOCK
// (--)
HPos::HPos(int _y, int _x, int positionType, int typeOfElement)
{

    if(positionType == USE_ISO) {

        abs_iso_y = _y;
        abs_iso_x = _x;
        rel_iso_y = _y;
        rel_iso_x = _x;



        gpix_y_topleft = Grid::convert_iso_to_gpix_y_topleft(abs_iso_y, abs_iso_x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, typeOfElement);
        gpix_x_topleft = Grid::convert_iso_to_gpix_x_topleft(abs_iso_y, abs_iso_x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, typeOfElement);


        gpix_y_middle = Grid::isoToGpixMiddleY(this, typeOfElement, 0);
        gpix_x_middle = Grid::isoToGpixMiddleX(this, typeOfElement, 0);


    }

    if(positionType == USE_GPIX) {

        // alpha-0.2: please also update the abs iso positions based on the gpix, using findTile()
        abs_iso_y = 0;
        abs_iso_x = 0;
        rel_iso_y = 0;
        rel_iso_x = 0;
        gpix_y_topleft = _y;
        gpix_x_topleft  = _x;
        wpix_y = _y;
        wpix_x = _x;

    }
}


// Defaults to the "GRID" typeOfElement
// Simplified constructor that does Constructor Delegating to the one doing anything
HPos::HPos(int _y, int _x, int positionType) : HPos(_y, _x, positionType, 0)
{
}





// (++)
HPos::HPos()
{
    abs_iso_y = 0;
    abs_iso_x = 0;
    rel_iso_y = 0;
    rel_iso_x = 0;
    gpix_y_topleft  = 0;
    gpix_y_topleft  = 0;
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
// (--) TEST needs "middle" gpix comparison too!
int HPos::compare(HPos *other)
{

    if(other->abs_iso_y == this->abs_iso_y
       &&
       other->abs_iso_x == this->abs_iso_x
       &&
       other->gpix_y_topleft  == this->gpix_y_topleft
       &&
       other->gpix_x_topleft  == this->gpix_x_topleft
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




/// \brief Used by roadnetwork.cpp in createSlotPathFromDijkstraResult
/// Makes a slotpos appear in the middle of an isometric tile/block
// (-+)
void HPos::transform_gpix_to_slotpos(SlotPos *slotpos, HPos *hpos)
{
    slotpos->hpos->gpix_y_topleft  += 12;
    slotpos->hpos->gpix_x_topleft  += 10;
}





// (--) test!!!!
void HPos::synchGpixToIsoValues(int height, int width)
{
    // this->gpix_y and gpix_x are the ones we know

    std::cout << "ERROR! synchGpixToIsoValues needs rework.... needs to use findTile, lots of data in parameters for that\n";
    return ;


    /*HPos *workpos = Grid::convert_gpix_to_iso(this, width, height);




    this->abs_iso_y = workpos->abs_iso_y;
    this->abs_iso_x = workpos->abs_iso_x;

    this->rel_iso_y = -1;
    this->rel_iso_x = -1;
*/
}


/// \brief Copies all the values from the original to the new one, returns newly allocated HPos
// (--)
HPos *HPos::clone()
{
    HPos *_pos = new HPos(abs_iso_y, abs_iso_x, USE_ISO);
    _pos->rel_iso_y = rel_iso_y;
    _pos->rel_iso_x = rel_iso_x;
    _pos->gpix_y_topleft  = gpix_y_topleft ;
    _pos->gpix_x_topleft  = gpix_x_topleft ;
    _pos->gpix_y_middle = gpix_y_middle ;
    _pos->gpix_x_middle  = gpix_x_middle ;

    return _pos;
}



// (++)
void HPos::dump(std::string ind)
{
    std::cout << ind << "{\n";
    std::cout << ind << "  abs_iso   y=" << abs_iso_y << ", x=" << abs_iso_x << "\n";
    std::cout << ind << "  rel_iso   y=" << rel_iso_y << ", x=" << rel_iso_x << "\n";
    std::cout << ind << "  gpix topleft y=" << gpix_y_topleft << ", x=" << gpix_x_topleft << "\n";
    std::cout << ind << "  gpix middle y=" << gpix_y_middle << ", x=" << gpix_x_middle << "\n";
    std::cout << ind << "}\n";
}







/// TEST FUNCTIONS

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
    A->dump("A");
    AA->dump("AA");
    assert(HPos::compareTwoAbsIso(A,AA) == 0);
    assert(HPos::compareTwoAbsIso(B,BB) == 0);
    assert(HPos::compareTwoAbsIso(C,CC) == 0);



    // Comparing GPIX values
    assert(HPos::compareTwoGpixTopLeft(D,DD) == 0);
    assert(HPos::compareTwoGpixTopLeft(E,EE) == 0);
    assert(HPos::compareTwoGpixTopLeft(F,FF) == 0);


    //std::cout << "These SHOULD fail: \n";
    //assert(HPos::compareTwoAbsIso(A,BB) == 0);
    //assert(HPos::compareTwoAbsIso(B,AA) == 0);
    //assert(HPos::compareTwoAbsIso(C,AA) == 0);

    //assert(HPos::compareTwoGpix(D,EE) == 0);
    //assert(HPos::compareTwoGpix(E,FF) == 0);
    //assert(HPos::compareTwoGpix(F,EE) == 0);




}
