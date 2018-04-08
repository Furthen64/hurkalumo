#include "Bus.hpp"
#include "RoadNetwork.hpp"






///Bus should do TWO updates

///     First one to update the bus moving towards the next position, which calls the first one         gameUpdate();
///     Second one to set all the position variables





// HPOSTEST
Bus::Bus(HPos *abs_iso_pos)
{


    /// Setup position      only the abs_iso positions are known    so please update all the other positions based on this
    pos = abs_iso_pos;
    update_all_position_vars_on_abs_iso();  // Design: this function should go to the HPos class

    nextPos = new HPos(0,0);


    /// Setup texture and sprite
    TextureManager *t;
    t=t->getInstance();
    texture = t->getTexture("BUS001");
    sprite = Sprite(texture);
    textureSize = sprite.getTextureRect();
}















///
/// TRAFFIC FUNCTIONS
///




// updates all the different position variables from the current iso_pos

// run this after you've set the current position!  e.g.: Bus::gameUpdate() which updates the current position.
// based on iso_posiotion
// (--)
void Bus::update_all_position_vars_on_abs_iso()
{

    // pos->abs_iso_x  and pos->abs_iso_y  are the ones we know are set, the rest are not

    pos->rel_iso_x = pos->abs_iso_x;
    pos->rel_iso_y = pos->abs_iso_y;

    // Now figure out where to draw the thing
    Vector2f vec_iso_pos = Vector2f();
    vec_iso_pos.y = pos->abs_iso_y;
    vec_iso_pos.x = pos->abs_iso_x;

    Vector2f vec_pix_pos = Vector2f();
    vec_pix_pos = Grid::convert_iso_to_gpix(vec_iso_pos, 64, 32);

    pos->gpix_y = vec_pix_pos.y;
    pos->gpix_x = vec_pix_pos.x;



    dump(nullptr);

}


// Tested once, worked
// (-+)
void Bus::update_all_nextPos_vars_on_abs_iso()
{
    // nextPos->abs_iso_x  and nextPos->abs_iso_y  are the ones we know are set, the rest are not


    nextPos->rel_iso_x = nextPos->abs_iso_x;
    nextPos->rel_iso_y = nextPos->abs_iso_y;

    // Now figure out where to draw the thing
    Vector2f vec_iso_pos = Vector2f();
    vec_iso_pos.y = nextPos->abs_iso_y;
    vec_iso_pos.x = nextPos->abs_iso_x;

    Vector2f vec_pix_pos = Vector2f();
    vec_pix_pos = Grid::convert_iso_to_gpix(vec_iso_pos, 64, 32);

    nextPos->gpix_y = vec_pix_pos.y;
    nextPos->gpix_x = vec_pix_pos.x;



    dump(nullptr);

}



// Run this last in gameUpdate()  so all the other variables are in synch with what just happened (the bus moved, the gpix variables changed)
// TEST
// (--)
void Bus::update_all_position_vars_on_gpix()
{
    std::cout << "CODE ME PLZ\n";
/*
    // pos->abs_iso_x  and pos->abs_iso_y  are the ones we know are set, the rest are not

    pos->rel_iso_x = pos->abs_iso_x;
    pos->rel_iso_y = pos->abs_iso_y;

    // Now figure out where to draw the thing
    Vector2f vec_iso_pos = Vector2f();
    vec_iso_pos.y = pos->abs_iso_y;
    vec_iso_pos.x = pos->abs_iso_x;

    Vector2f vec_pix_pos = Vector2f();
    vec_pix_pos = Grid::convert_iso_to_gpix(vec_iso_pos, 64, 32);

    pos->gpix_y = vec_pix_pos.y;
    pos->gpix_x = vec_pix_pos.x;



    dump(nullptr);*/
}



// Takes in the roadnetwork the bus is on,
// So whenever the bus needs to move to a new road it can use the correct offsets for that particular roadnetwork
// TEST!
/// Based on what happened in setNext_pix_pos , we need to move towards that next pixel position
// (--)
void Bus::gameUpdate(RoadNetwork *roadnet)
{
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


    std::cout << "DeltaY=" << deltaY << " , DeltaX=" << deltaX << "\n";


    /// Do the actual move
    switch(dir) {


        case 0: // UP RIGHT

            // Set current position
            std::cout << "Dir= up right\n";
            pos->gpix_x += ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y -= ceil ( (float) (deltaY * speed/100) );
            /*pix_pos.x += ceil ( (float) (deltaX * speed/100) );
            pix_pos.y -= ceil ( (float) (deltaY * speed/100) );*/

            break;

        case 1: // DOWN RIGHT
            std::cout << "Dir= down right\n";
            pos->gpix_x += ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y += ceil ( (float) (deltaY * speed/100) );

            /*pix_pos.x += ceil((float) (deltaX * speed/100) );
            pix_pos.y += ceil ( (float) (deltaY * speed/100) );*/
            break;

        case 2: // DOWN LEFT
            std::cout << "Dir= down left\n";
            pos->gpix_x -= ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y += ceil ( (float) (deltaY * speed/100) );

            /*pix_pos.x -= ceil ( (deltaX * speed/100));
            pix_pos.y += ceil ( (deltaY * speed/100));*/
            break;

        case 3: // UP LEFT
            std::cout << "Dir= up left\n";

            pos->gpix_x -= ceil ( (float) (deltaX * speed/100) );
            pos->gpix_y -= ceil ( (float) (deltaY * speed/100) );

            /*pix_pos.x -= ceil (  ( deltaX * speed/100));
            pix_pos.y -= ceil ( ( deltaY * speed/100)  );*/
            break;

        case 4:
            deltaX = 0;
            deltaY = 0;
            break;


    }







    // If delta x and deltay is zero, means we have reached our destination

    if(deltaX == 0 && deltaY == 0) {
        std::cout << "bus: SHOULD I GO SOMWHERE NOW?\n";
    }

    // Update all position_variables based on this new gpix position
    update_all_position_vars_on_gpix();

}



// HPOSDELETE
// Assumes we are always in positive euclidian space. No negative x and y positions.
// (-+)
void Bus::setNext_pix_pos( Vector2f _np)
{
    //std::cout << "\n\n ** setNext_pix_pos()\n";
    //std::cout << "CurrPos (" << pix_pos.x << ", " << pix_pos.y << ")   Nexpos (" << next_pix_pos.x << ", " << next_pix_pos.y << ")\n";

    next_pix_pos = _np;

    dir = 4; // Do nothing

    // Now figure out the direction for the bus
    bool rightof = false;
    bool belowof = false;
    bool topof = false;
    bool leftof = false;


    if(next_pix_pos.y < pix_pos.y)
    {
        topof = true;
    } else if(next_pix_pos.y > pix_pos.y)
    {
        topof = false;
        belowof = true;
    } else {
        topof = false;
        belowof = false;
    }


    // Left of? Right of?
    if(next_pix_pos.x < pix_pos.x)
    {
        leftof = true;
    } else if(next_pix_pos.x > pix_pos.x) {
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


    //std::cout << rightof << " " << belowof << " " << topof << " " << leftof  << "  " << "direction=" << dir << " *** \n\n";

}













///
/// LOW-LEVEL POSITION FUNCTIONS
///



// Seems to work
// (-+)
void Bus::draw( RenderTarget& rt, Vector2i *viewPos)
{


    int x = pos->gpix_x + viewPos->x;
    int y = pos->gpix_y + viewPos->y;

    Vector2f _pos = {(float)x,(float)y};

    sprite.setPosition(_pos);           // OPTIMA: You could optimize by sending a reference/pointer, reuse the same workposition inside the Bus object

    rt.draw( sprite );
}



// HPOSTEST
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






void Bus::setNext_iso_pos( Vector2f _np)
{
    next_iso_pos = _np;
}








// Not needed?
void Bus::setNext_pix_pos( HPos *_np)
{

    /// Set the Next position
    nextPos = _np;

    update_all_nextPos_vars_on_abs_iso();


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



// (-+)
void Bus::setRandStartingPosition(HurkaMatrix *roadMatrix)
{

    // First get a random position on the road for iso_pos
    iso_pos = rand_iso_pos(roadMatrix);

    pix_pos = Grid::convert_iso_to_gpix(iso_pos, textureSize.width, textureSize.height);
}






/// Gives you a random iso position from the gamematrix
// (-+)
Vector2f Bus::rand_iso_pos(int maxM, int maxN)
{
    int m = randBetween(0, maxM);
    int n = randBetween(0, maxN);

    Vector2f _iso_pos;
    _iso_pos.y = m;
    _iso_pos.x = n;

    return _iso_pos;
}





/// Randomize an iso position on the RoadNetwork, use the Y and X offset to figure out the absolute position on the gamematrix
//
// (-+)
Vector2f Bus::rand_abs_iso_pos(RoadNetwork *roadnet)
{
    if(roadnet->hMatrix->rows > 10000 || roadnet->hMatrix->cols > 10000) {
        std::cout << "ERROR" << cn << " too big of a roadmatrix! " << roadnet->hMatrix->rows << ", " << roadnet->hMatrix->cols << "\n";
        return Vector2f();
    }

    bool found = false;
    int allowedAttempts = 500;
    int currAttempt = 0;

    Vector2f newPos = Vector2f();

    int r;
    int c;

    while(found == false && currAttempt < allowedAttempts) {

        r = randBetween(0, roadnet->hMatrix->rows-1);
        c = randBetween(0, roadnet->hMatrix->cols-1);

        if(roadnet->hMatrix->matrix[r][c] == 1) {

            newPos.y = r + roadnet->min_isoYOffset;
            newPos.x = c + roadnet->min_isoXOffset;

            found = true;

        }

        currAttempt++;
    }

    if(found == false) {

        std::cout << "Could not find random bus position\n";
    }

    return newPos;

}


/// Randomize an iso position from the roads
// (-+)
Vector2f Bus::rand_iso_pos(HurkaMatrix *roadMatrix)
{


    if(roadMatrix->rows > 10000 || roadMatrix->cols > 10000) {
        std::cout << "ERROR" << cn << " too big of a roadmatrix! " << roadMatrix->rows << ", " << roadMatrix->cols << "\n";
        return Vector2f();
    }

    bool found = false;
    int allowedAttempts = 500;
    int currAttempt = 0;

    Vector2f newPos = Vector2f();

    int r;
    int c;

    while(found == false && currAttempt < allowedAttempts) {

        r = randBetween(0, roadMatrix->rows-1);
        c = randBetween(0, roadMatrix->cols-1);

        if(roadMatrix->matrix[r][c] == 1) {

            newPos.y = r;
            newPos.x = c;

            found = true;

        }

        currAttempt++;
    }

    if(found == false) {

        std::cout << "Could not find random bus position\n";
    }

    return newPos;
}

// (-+)
void Bus::dump(Vector2i *viewPos)
{
    int wy = 0;
    int wx = 0;
    if(viewPos != nullptr) {
        wy = viewPos->y;
        wx = viewPos->x;
    }
    std::cout << "\n\nBus:\n";
    std::cout << "       abs_iso_y,abs_iso-x     rel_iso_y,rel_iso_x        gpix_y,gpix_x     wpix_y,wpix_x\n";
    std::cout << "   pos           " << pos->abs_iso_y << "," << pos->abs_iso_x << "                    "  << pos->rel_iso_y << ", " << pos->rel_iso_x << "                      " << pos->gpix_y << "," << pos->gpix_x <<  "            " << wy + pos->gpix_y << "," << wx + pos->gpix_x << "\n";
    std::cout << "   nextPos       " << nextPos->abs_iso_y << "," << nextPos->abs_iso_x << "                    "  << nextPos->rel_iso_y << ", " << nextPos->rel_iso_x << "                      " << nextPos->gpix_y << "," << nextPos->gpix_x <<  "            " << wy + nextPos->gpix_y << "," << wx + nextPos->gpix_x << "\n\n";

}


// Get internal pointer of what the next position for the bus is
HPos *Bus::get_next_pos()
{
    return nextPos;
}


// HPOSDELETE
Vector2f Bus::get_next_iso_pos()
{
    return next_iso_pos;
}
