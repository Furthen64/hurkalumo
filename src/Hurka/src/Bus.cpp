#include "Bus.hpp"
#include "RoadNetwork.hpp"





// Seems to work
// (-+)
Bus::Bus()
{
    slotPath = nullptr;


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




    // Ignore texture stuff, probably wont change since construction

}















///
/// TRAFFIC FUNCTIONS
///





/// \brief Takes roadnetwork the bus is on as argument,
///  So whenever the bus needs to move to a new road it can use the correct iso offsets for that particular roadnetwork
// (--) test more... remove unecessary stuff
void Bus::gameUpdate(RoadNetwork *roadnet)
{


    if(slotPath->hasValues()) {

        // make the bus use the slotpath it now has
        SlotPos *workPos;
        workPos = slotPath->stepAndGetPos(1);   // Make 1 step

        if(workPos == nullptr) {
            std::cout << "ERROR " << cn << " gameUpdate tried getting a slotPos from slotPath but ended up with a nullptr. \n";
            return ;
        }


        // Update current Pos   (gpix only!)
        this->pos = workPos->hpos;

        // Make sure we get iso position too! use the size of a GRID to get it done
        this->pos->synchGpixToIsoValues(GRID_TEXTURE_HEIGHT, GRID_TEXTURE_WIDTH);

    }
}







// (-+)
void Bus::draw( RenderTarget& rt, HPos *viewHPos, int drawSlotPositions)
{


    if(drawSlotPositions) {
        slotPath->drawAllSlots(rt, viewHPos);
    }

    int x = pos->gpix_x + viewHPos->gpix_x;
    int y = pos->gpix_y + viewHPos->gpix_y;

    Vector2f _pos = {(float)x,(float)y};

    sprite.setPosition(_pos);           // OPTIMA: You could optimize by sending a reference/pointer, reuse the same workposition inside the Bus object

    rt.draw( sprite );
}






 ///
 ///    Random utilities
 ///




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



// (-+)
void Bus::setSlotPath(SlotPath *_sp)
{
    if(_sp == nullptr) {
        std::cout << "ERROR " << cn << "trying to set a null slotpath for a bus! bus at= " << pos->absToString() << "\n";
    }
    slotPath = _sp;
}





// (--)
void Bus::dump(std::string ind)
{

    std::cout << " --- bus " << pos->absToString() << " ------\n";
    std::cout << ind << "         abs_iso_y,abs_iso-x     rel_iso_y,rel_iso_x        gpix_y,gpix_x  \n";
    std::cout << ind << "   pos           " << pos->abs_iso_y << "," << pos->abs_iso_x << "                    "  << pos->rel_iso_y << ", " << pos->rel_iso_x << "                      " << pos->gpix_y << "," << pos->gpix_x << "\n";
    std::cout << ind << "slotpath:\n";
    slotPath->dump();
    std::cout << "\n";
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





/// \brief returns pointer to HPos that describes where the Bus is right now. Warning: internal pointer!
// (-+)
HPos *Bus::getNowPos()
{
    if(slotPath->nowPos != nullptr) {
        return slotPath->nowPos->hpos;
    }

    return nullptr;
}



// (--) test
bool Bus::atPos(HPos *searchpos)
{
    HPos *now = getNowPos();

    if(now != nullptr) {
        if(now->compareAbsIso(searchpos) == 0) {

            return true;
        }
    }

    return false;
}































