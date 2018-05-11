#include "Bus.hpp"
#include "RoadNetwork.hpp"





// Seems to work a little bit (2018-04)
// (-+)
Bus::Bus(HPos *_abs_iso_pos)
{
    slotPath = nullptr;

    /// Setup position
    set_pos_on_abs_iso(_abs_iso_pos);

    nextPos = new HPos(0,0, USE_ISO);
    set_nextPos_on_abs_iso(nextPos);


    /// Setup texture and sprite
    TextureManager *t;
    t=t->getInstance();
    texture = t->getTexture("BUS001");
    sprite = Sprite(texture);
    textureSize = sprite.getTextureRect();
}

void Bus::reset()
{
    if(slotPath != nullptr) { delete slotPath; }
    slotPath = nullptr;


    // This position
    HPos *wrk_iso_pos = new HPos(0,0, USE_ISO);
    set_pos_on_abs_iso(wrk_iso_pos);

    // Next position
    wrk_iso_pos = new HPos(0,0, USE_ISO);
    set_nextPos_on_abs_iso(wrk_iso_pos);


    // Ignore texture stuff, probably wont change since construction

}















///
/// TRAFFIC FUNCTIONS
///




/// \brief Updates all the different position variables from the current iso_pos
///
/// run this after you've set the current position!  e.g.: Bus::gameUpdate() which updates the current position.
/// (--)
void Bus::update_all_position_vars_on_abs_iso()
{

    // pos->abs_iso_x  and pos->abs_iso_y  are the ones we know are set, the rest are not
    pos->rel_iso_x = pos->abs_iso_x;
    pos->rel_iso_y = pos->abs_iso_y;


    // Calculate the GPix position
    pos->gpix_y = Grid::convert_iso_to_gpix_y(pos->abs_iso_y, pos->abs_iso_x, 64, 32, 2); // FIXME Lazy coding (64,32 thingie)
    pos->gpix_x = Grid::convert_iso_to_gpix_x(pos->abs_iso_y, pos->abs_iso_x, 64, 32, 2);

}


// Tested once, worked
// (--)
void Bus::update_all_nextPos_vars_on_abs_iso()
{
    // nextPos->abs_iso_x  and nextPos->abs_iso_y  are the ones we know are set, the rest are not
    nextPos->rel_iso_x = nextPos->abs_iso_x;
    nextPos->rel_iso_y = nextPos->abs_iso_y;

    // Calculate the GPix position
    nextPos->gpix_y = Grid::convert_iso_to_gpix_y(nextPos->abs_iso_y, nextPos->abs_iso_x, 64, 32, 2);
    nextPos->gpix_x = Grid::convert_iso_to_gpix_x(nextPos->abs_iso_y, nextPos->abs_iso_x, 64, 32, 2);
}



// Run this last in gameUpdate()  so all the other variables are in synch with what just happened (the bus moved, the gpix variables changed)

// TEST
// (--)
void Bus::update_all_position_vars_on_gpix()
{

    //std::cout << "CODE ME PLZ\n";


    //dump(nullptr);
}



// Wishlist: Takes in the roadnetwork the bus is on,
//           So whenever the bus needs to move to a new road it can use the correct offsets for that particular roadnetwork
//
// Based on what happened in setNext_pix_pos , we need to move towards that next pixel position
// (--)
void Bus::gameUpdate(RoadNetwork *roadnet)
{
    int debugLevel = 0;



    int deltaX = 0;
    int deltaY = 0;

    // Calculate the pixels between where we are now and where we are going

    if(nextPos->gpix_x > pos->gpix_x) {
        deltaX = nextPos->gpix_x - pos->gpix_x;
    } else {
        deltaX = pos->gpix_x - nextPos->gpix_x;
    }

    if(nextPos->gpix_y > pos->gpix_y) {
        deltaY = nextPos->gpix_y - pos->gpix_y;
    } else {
        deltaY = pos->gpix_y - nextPos->gpix_y;
    }


    if(debugLevel >=1) { std::cout << "DeltaY=" << deltaY << " , DeltaX=" << deltaX << "\n";}


    /// Do the actual move
    switch(dir) {


        case 0: // UP RIGHT

            // Set current position
            if(debugLevel >=1) { std::cout << "Dir= up right\n"; }
            pos->gpix_x += ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y -= ceil ( (float) (deltaY * speed/100) );

            break;

        case 1: // DOWN RIGHT
            if(debugLevel >=1) {std::cout << "Dir= down right\n";}
            pos->gpix_x += ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y += ceil ( (float) (deltaY * speed/100) );

            break;

        case 2: // DOWN LEFT
            if(debugLevel >=1) { std::cout << "Dir= down left\n";}
            pos->gpix_x -= ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y += ceil ( (float) (deltaY * speed/100) );

            break;

        case 3: // UP LEFT
            if(debugLevel >=1) { std::cout << "Dir= up left\n";}
            pos->gpix_x -= ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y -= ceil ( (float) (deltaY * speed/100) );

            break;

        case 4:
            deltaX = 0;
            deltaY = 0;
            break;


    }







    // If delta x and deltay is zero, means we have reached our destination

    if(deltaX == 0 && deltaY == 0) {
        if(debugLevel >=1) {std::cout << "bus: SHOULD I GO SOMWHERE NOW?\n";}
    }

    // Update all position_variables based on this new gpix position
    update_all_position_vars_on_gpix();

}














///
/// LOW-LEVEL POSITION FUNCTIONS
///



// Seems to work
// (-+)
void Bus::draw( RenderTarget& rt, HPos *viewHPos, int drawSlotPositions)
{

    if(drawSlotPositions) {

    }

    int x = pos->gpix_x + viewHPos->gpix_x;
    int y = pos->gpix_y + viewHPos->gpix_y;

    Vector2f _pos = {(float)x,(float)y};

    sprite.setPosition(_pos);           // OPTIMA: You could optimize by sending a reference/pointer, reuse the same workposition inside the Bus object

    rt.draw( sprite );
}



// Set the position of the bus based on abs_iso
// (-+)
void Bus::set_pos_on_abs_iso(HPos *abs_iso_pos)
{
    pos = abs_iso_pos;

    // update all the other variables
    update_all_position_vars_on_abs_iso();  // Design: this function should go to the HPos class
}







// HPOSTEST
// Sets next position based on abs_iso
// Also updates the direction of travel
// (--)
void Bus::set_nextPos_on_abs_iso( HPos *abs_iso_pos)
{

    nextPos = abs_iso_pos;

    update_all_nextPos_vars_on_abs_iso(); // update all the other variables so we can use gpix data below

    /// Find out direction of travel to that next position
    dir = 4; // Do nothing

    // Now figure out the direction for the bus
    bool rightof = false;
    bool belowof = false;
    bool topof = false;
    bool leftof = false;


    // top or below?
    if(nextPos->gpix_y < pos->gpix_y)
    {
        topof = true;
    } else if(nextPos->gpix_y > pos->gpix_y)
    {
        topof= false;
        belowof = true;
    } else {
        topof = false;
        belowof = false;
    }


    // left or right?
    if(nextPos->gpix_x < pos->gpix_x)
    {
        leftof = true;
    } else if(nextPos->gpix_x > pos->gpix_x) {
        leftof = false;
        rightof = true;
    } else {
        leftof = false;
        rightof = false;
    }



    if(topof && rightof) {
        dir = 0;
    } else if (belowof && rightof) {
        dir = 1;
    } else if (belowof && leftof) {
        dir = 2;
    } else if (topof && leftof ) {
        dir = 3;
    } else {
        dir = 4; // Do nothing!
    }
}



















/// RANDOM FUNCTIONS



// (--)
void Bus::setRandStartingPosition(HurkaMatrix *roadMatrix)
{

    // First get a random position on the road for iso_pos
    std::cout << "ERROR CONVERT TO HPOS!\n";



    /*
    HPOSDELETE:

    iso_pos = rand_iso_pos(roadMatrix);

    pix_pos = Grid::convert_iso_to_gpix(iso_pos, textureSize.width, textureSize.height, 2);
    */
}






/// @brief Gives you a random iso position from the gamematrix
/// @param maxM  Maximum in M or Y axis
/// @param maxN  Maximum in N or X axis
/// (--)
HPos *Bus::rand_iso_pos(int maxM, int maxN)
{
    int m = randBetween(0, maxM);
    int n = randBetween(0, maxN);

    HPos *_iso_pos = new HPos(m,n, USE_ISO);
    return _iso_pos;
}



/// \brief Given a roadMatrix (matrix with 1s for roads, 0s else) find a random position where a road is
/// \param roadMatrix Allocated HurkaMatrix object with set values
/// \return A random position for a road
/// (--)
// HPOSTEST!
HPos *Bus::rand_iso_pos(HurkaMatrix *roadMatrix)
{
    if(roadMatrix->rows > 10000 || roadMatrix->cols > 10000) {
        std::cout << "ERROR" << cn << " too big of a roadmatrix! " << roadMatrix->rows << ", " << roadMatrix->cols << "\n";
        return nullptr;
    }

    bool found = false;
    int allowedAttempts = 500;
    int currAttempt = 0;

    HPos *newPos = new HPos(0,0,USE_ISO);

    int r;
    int c;

    while(found == false && currAttempt < allowedAttempts) {

        r = randBetween(0, roadMatrix->rows-1);
        c = randBetween(0, roadMatrix->cols-1);

        if(roadMatrix->matrix[r][c] == 1) {

            newPos->abs_iso_y = r;
            newPos->abs_iso_x = c;

            found = true;

        }

        currAttempt++;
    }

    if(found == false) {

        std::cout << "Could not find random bus position\n";
        return nullptr;
    }

    return newPos;
}











/// \brief Randomize an iso position on the RoadNetwork, use the Y and X offset to figure out the absolute position on the gamematrix
/// \param roadnet
/// (---)

HPos *Bus::rand_abs_iso_pos(RoadNetwork *roadnet)
{
    // Error check input
    if(roadnet->nrRows() > 10000 || roadnet->nrCols() > 10000) {
        std::cout << "ERROR" << cn << " too big of a roadmatrix! " << roadnet->hMatrix->rows << ", " << roadnet->hMatrix->cols << "\n";
        return nullptr;
    }

    // Try and find a random position inside the roadnetwork

    bool found = false;
    int allowedAttempts = 500;
    int currAttempt = 0;

    HPos *newPos = new HPos(0,0, USE_ISO);

    int r = 0;
    int c = 0;

    while(found == false && currAttempt < allowedAttempts)
    {

        r = randBetween(0, roadnet->nrRows()-1);
        c = randBetween(0, roadnet->nrCols()-1);

        if(roadnet->hMatrix->matrix[r][c] == 1) {

            newPos->abs_iso_y = r + roadnet->min_isoYOffset;
            newPos->abs_iso_x = c + roadnet->min_isoXOffset;

            found = true;

        }

        currAttempt++;

    }


    if(found == false) {

        std::cout << "Warning - Could not find random bus position.\n";
        return nullptr;
    }

    return newPos;

}


void Bus::setSlotPath(SlotPath *_sp)
{
    slotPath = _sp;
}


// (-+)
void Bus::dump(HPos *viewHPos)
{
    int wy = 0;
    int wx = 0;
    if(viewHPos != nullptr) {
        wy = viewHPos->gpix_y;
        wx = viewHPos->gpix_x;;
    }
    std::cout << "\n\nBus:\n";
    std::cout << "       abs_iso_y,abs_iso-x     rel_iso_y,rel_iso_x        gpix_y,gpix_x     wpix_y,wpix_x\n";
    std::cout << "   pos           " << pos->abs_iso_y << "," << pos->abs_iso_x << "                    "  << pos->rel_iso_y << ", " << pos->rel_iso_x << "                      " << pos->gpix_y << "," << pos->gpix_x <<  "            " << wy + pos->gpix_y << "," << wx + pos->gpix_x << "\n";
    std::cout << "   nextPos       " << nextPos->abs_iso_y << "," << nextPos->abs_iso_x << "                    "  << nextPos->rel_iso_y << ", " << nextPos->rel_iso_x << "                      " << nextPos->gpix_y << "," << nextPos->gpix_x <<  "            " << wy + nextPos->gpix_y << "," << wx + nextPos->gpix_x << "\n\n";

}



// Get internal pointer of what the next position for the bus is
// (++)
HPos *Bus::get_next_pos()
{
    return nextPos;
}
