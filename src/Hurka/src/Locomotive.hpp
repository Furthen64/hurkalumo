#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H

#include "SFML/Graphics.hpp"
#include "HPos.hpp"

using namespace sf;

/// //////////////////////////////////////////////
/// Locomotive
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
