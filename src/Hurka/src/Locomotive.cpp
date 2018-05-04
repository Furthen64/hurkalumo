#include "Locomotive.hpp"


/// //////////////////////////////////////////////
/// Locomotive



Locomotive::Locomotive()
{
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\LOCOMOTIVE.png");
    sprite = Sprite(texture);
    sprite.setTextureRect( {0,0,32,32});
}



void Locomotive::draw( RenderTarget& rt, HPos *viewpos)
{
}

