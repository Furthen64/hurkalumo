#include "Locomotive.hpp"


/// //////////////////////////////////////////////
/// Locomotive

Locomotive::Locomotive(const Vector2f& _pos)
    :
    pos(_pos)
{
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\LOCOMOTIVE.png");
    sprite = Sprite(texture);
    sprite.setTextureRect( {0,0,32,32});
}

void Locomotive::draw( RenderTarget& rt) const
{
    rt.draw( sprite );
}
void Locomotive::setDirectionConst( const Vector2f& dir)
{
    vel = dir * speed; //??
}

void Locomotive::setDirection( Vector2f dir)
{
    vel = dir * speed; //??
}

void Locomotive::update( float dt)
{
    pos += vel * dt;
    sprite.setPosition(pos);
}

Vector2f Locomotive::getPos()
{
    return pos;
}

