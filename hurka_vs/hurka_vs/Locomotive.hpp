#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H

#include "SFML/Graphics.hpp"
#include "HPos.hpp"

using namespace sf;


/// Locomotive
///
/// Hey!
///
/// If you need to work on Locomotive again, please take a look at Bus class for its direction and speed and stuffs.
///
class Locomotive
{
public:

    Locomotive();

    void draw( RenderTarget& rt, HPos *viewpos);

private:
    float speed = 16.0f;  // move 100 px per second
    Texture texture;
    Sprite sprite;
};

#endif
