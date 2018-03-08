#include "Bus.hpp"


/// //////////////////////////////////////////////
/// Locomotive


/// Jag förstår ingenting av det här, bara norpat

Bus::Bus(const Vector2f& _pos)
    :
    pos(_pos)
{
    TextureManager *t;
    t=t->getInstance();
    texture = t->getTexture("BUS001");

    sprite = Sprite(texture);
//    sprite.setTextureRect( {0,0,32,32});
}

void Bus::draw( RenderTarget& rt) const
{
    rt.draw( sprite );
}
void Bus::setDirectionConst( const Vector2f& dir)
{
    vel = dir * speed; //??
}

void Bus::setDirection( Vector2f dir)
{
    vel = dir * speed; //??
}

void Bus::update( float dt)
{
    pos += vel * dt;
    sprite.setPosition(pos);
}

Vector2f Bus::getPos()
{
    return pos;
}

void Bus::setPos( Vector2f _p)
{
        pos = _p;
}

void Bus::setNextPos( Vector2f _np)
{
    nextPos = _np;
}

Vector2f Bus::randStartingPos(HurkaMatrix *roadMatrix)
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

            // FIXME 16 16 ?
            newPos.y = GameMatrix::getWindowYPos(r, c, 32, 32);
            newPos.x = GameMatrix::getWindowXPos(r, c, 32, 32);
            found = true;

        }

        currAttempt++;
    }


    return newPos;
}
