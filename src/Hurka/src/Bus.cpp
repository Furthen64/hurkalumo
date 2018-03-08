#include "Bus.hpp"





Bus::Bus(const Vector2f& _iso_pos)
    :
    iso_pos(_iso_pos)
{
    TextureManager *t;
    t=t->getInstance();
    texture = t->getTexture("BUS001");

    textureSize = sprite.getTextureRect();

    sprite = Sprite(texture);
}



// Ska egentligen köras i en update-funktion mestadels här,
// bara rt.draw ska finnas
void Bus::draw( RenderTarget& rt, Vector2u viewPos)
{

    int x = Block::getWindowXPos(iso_pos.x,iso_pos.y, textureSize.width, textureSize.height);
    int y = Block::getWindowYPos(iso_pos.x,iso_pos.y, textureSize.width, textureSize.height);


    // Viewing offset
    x += viewPos.x;
    y += viewPos.y;

    Vector2f _pos = {(float)x,(float)y};

    sprite.setPosition(_pos);


    rt.draw( sprite );
}


void Bus::setRandStartingPosition(HurkaMatrix *roadMatrix)
{

    // First get a random position on the road for iso_pos
    iso_pos = rand_iso_pos(roadMatrix);


    // Convert it into pix_pos
    pix_pos = GameMatrix::convert_iso_to_pix(iso_pos, textureSize.width, textureSize.height);
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

void Bus::setNext_pix_pos( Vector2f _np)
{
    next_pix_pos = _np;
}

//
void Bus::update()
{
    // From the current iso_pos, go to the next iso_pos by changing the pix_pos.
    // FIXME TODO: Du måste ha en funktion som konverterar från pix_pos till iso_pos för att veta om man bytt iso-ruta

    pix_pos.x += 1;
    pix_pos.y += 1;


    if(pix_pos.x > 1000) {
        iso_pos.x = 0;
        iso_pos.y = 0;
    }


}



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


        std::cout << "rows= " << roadMatrix->rows << " cols= " << roadMatrix->cols << "\n";

        r = randBetween(0, roadMatrix->rows-1);
        c = randBetween(0, roadMatrix->cols-1);

        std::cout << "Randomizing bus start (" << r << ", " << c << ")\n";


        if(roadMatrix->matrix[r][c] == 1) {

            newPos.y = r;
            newPos.x = c;

            found = true;

        }

        currAttempt++;
    }


    return newPos;
}
