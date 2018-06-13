#include "Locomotive.hpp"


///
/// Locomotive
///
/// Has nothing. Needs everything. Very old code! // 2018-02 I think



Locomotive::Locomotive()
{
    texture.loadFromFile("LOCOMOTIVE.png");
    sprite = Sprite(texture);
    sprite.setTextureRect( {0,0,32,32});
}



void Locomotive::draw( RenderTarget& rt, HPos *viewpos)
{
}

