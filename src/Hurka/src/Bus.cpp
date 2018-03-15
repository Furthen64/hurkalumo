#include "Bus.hpp"



Bus::Bus(const Vector2f& _iso_pos)
    :
    iso_pos(_iso_pos)
{
    TextureManager *t;
    t=t->getInstance();
    texture = t->getTexture("BUS001");

    sprite = Sprite(texture);

    textureSize = sprite.getTextureRect();
}



// Ska egentligen köras i en update-funktion mestadels här,
// bara rt.draw ska finnas
void Bus::draw( RenderTarget& rt, Vector2i viewPos)
{


    int x = pix_pos.x;
    int y = pix_pos.y;

    // Viewing offset
    x += viewPos.x;
    y += viewPos.y;

    Vector2f _pos = {(float)x,(float)y};


    sprite.setPosition(_pos);

    rt.draw( sprite );
}


// (--)
void Bus::setRandStartingPosition(HurkaMatrix *roadMatrix)
{

    // First get a random position on the road for iso_pos
    iso_pos = rand_iso_pos(roadMatrix);


    // Convert it into pix_pos
    pix_pos = Grid::convert_pix_to_iso(iso_pos, textureSize.width, textureSize.height);
    //pix_pos = GameMatrix::convert_iso_to_pix(iso_pos, textureSize.width, textureSize.height);
}





// Iso position functions
Vector2f Bus::get_iso_pos()
{
    return iso_pos;
}

void Bus::set_iso_pos( Vector2f _p)
{
    iso_pos = _p;

}

void Bus::setNext_iso_pos( Vector2f _np)
{
    next_iso_pos = _np;
}


// Pixel position functions

Vector2f Bus::get_pix_pos()
{
    return pix_pos;
}

void Bus::set_pix_pos( Vector2f _p)
{
        pix_pos = _p;
}

// Assumes we are always in positive euclidian space :) no negative .x and .ys!

void Bus::setNext_pix_pos( Vector2f _np)
{
 //   std::cout << "\n\n ** setNext_pix_pos()\n";
    //std::cout << "CurrPos (" << pix_pos.x << ", " << pix_pos.y << ")   Nexpos (" << next_pix_pos.x << ", " << next_pix_pos.y << ")\n";
    next_pix_pos = _np;

    dir = 4; // Do nothing

    // Now figure out what the direction for the bus is
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

   // std::cout << rightof << " " << belowof << " " << topof << " " << leftof  << "  " << "direction=" << dir << " *** \n\n";

}

/// Based on what happened in setNext_pix_pos , we need to move towards that next pixel position
// (--)
void Bus::update(HurkaMatrix *roadMatrix)
{

//    dump();


    int deltaX = 0;
    int deltaY = 0;

   // std::cout << "BEFORE (" << pix_pos.x << ", " << pix_pos.y << ") ";


    if(next_pix_pos.x > pix_pos.x) {
        deltaX = next_pix_pos.x - pix_pos.x;
    } else {
        deltaX = pix_pos.x - next_pix_pos.x;
    }

    if(next_pix_pos.y > pix_pos.y ) {
        deltaY = next_pix_pos.y - pix_pos.y;
    } else {
        deltaY = pix_pos.y - next_pix_pos.y;
    }




    /// Do the actual move
    switch(dir) {


        case 0: // UP RIGHT


            // Set current position
            pix_pos.x += ceil ( (float) (deltaX * speed/100) );
            pix_pos.y -= ceil ( (float) (deltaY * speed/100) );

            //std::cout << deltaX << ", " << deltaY << "\n";
            break;

        case 1: // DOWN RIGHT
         //   deltaX = next_pix_pos.x - pix_pos.x;
         //   deltaY = next_pix_pos.y - pix_pos.y;

            //std::cout << deltaX << ", " << deltaY << "\n";

            // Set current position
            pix_pos.x += ceil((float) (deltaX * speed/100) );
            pix_pos.y += ceil ( (float) (deltaY * speed/100) );
            break;

        case 2: // DOWN LEFT
         //   deltaX = pix_pos.x - next_pix_pos.x;
         //   deltaY = next_pix_pos.y - pix_pos.y;

            //std::cout << deltaX << ", " << deltaY << "\n";

            // Set current position
            pix_pos.x -= ceil ( (deltaX * speed/100));
            pix_pos.y += ceil ( (deltaY * speed/100));
            break;

        case 3: // UP LEFT
          //  deltaX = pix_pos.x - next_pix_pos.x;
           // deltaY = pix_pos.y - next_pix_pos.y;

            //std::cout << deltaX << ", " << deltaY << "\n";

            // Set current position
            pix_pos.x -= ceil (  ( deltaX * speed/100));
            pix_pos.y -= ceil ( ( deltaY * speed/100)  );
            break;

        case 4:
            deltaX = 0;
            deltaY = 0;
            break;


    }



    if(deltaX == 0 && deltaY == 0) {

        setNext_iso_pos( rand_iso_pos());


        /// Old code which I really want to use, whereas the bus will get the information from the ROAD to decide where to be
        //setNext_iso_pos(rand_iso_pos(roadMatrix));
        //setNext_pix_pos(GameMatrix::convert_iso_to_pix(next_iso_pos, 64, 32));   // Bugg!
        // Fungerar denna? hmmmmmmmmmmmmmmmm ska du kanske använda Grid::convert ist?

        Vector2f _next_pix_pos;

//        _next_pix_pos.x = Grid::getWindowXPos(next_iso_pos.y, next_iso_pos.x, 64, 32);   // Not sure about those 64, 32 things... Not sure what function to use, grid or gamematrix or blocK?
// _next_pix_pos.y = Grid::getWindowYPos(next_iso_pos.y, next_iso_pos.x, 64, 32);



        setNext_pix_pos(_next_pix_pos);

    }


    /// Find out where we are in the grid and update the Bus's iso_pos
    iso_pos = Grid::convert_pix_to_iso(pix_pos, textureSize.width, textureSize.height);


}




Vector2f Bus::rand_iso_pos()
{
    int maxM = 4;
    int maxN = 4;

    int m = randBetween(0, maxM);
    int n = randBetween(0, maxN);

    Vector2f _iso_pos;
    _iso_pos.y = m;
    _iso_pos.x = n;

//    std::cout << "rand iso pos= ";
    //dumpPosition(_iso_pos);

    return _iso_pos;
}


/// Randomize a position in the GameMatrix isometric plane
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


        //std::cout << "rows= " << roadMatrix->rows << " cols= " << roadMatrix->cols << "\n";

        r = randBetween(0, roadMatrix->rows-1);
        c = randBetween(0, roadMatrix->cols-1);

        if(roadMatrix->matrix[r][c] == 1) {

            std::cout << " Found random iso_pos(" << r << ", " << c << ")\n";
            newPos.y = r;
            newPos.x = c;

            found = true;

        }

        currAttempt++;
    }


    return newPos;
}


void Bus::dump()
{
    std::cout << " bus is now at iso_pos(" << iso_pos.y <<", " << iso_pos.x << ") pix_pos(" << pix_pos.y << ", " << pix_pos.x << ")  going to  iso_pos(" << next_iso_pos.y << ", " <<
    next_iso_pos.x << ") pix_pos(" << next_pix_pos.y << ", " << next_pix_pos.x << ")\n";
}
