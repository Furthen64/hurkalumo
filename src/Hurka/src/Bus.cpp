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


void Bus::setNextPos( Vector2f _np)
{
    nextPos = _np;
}
