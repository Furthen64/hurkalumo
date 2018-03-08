#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H

#include "SFML/Graphics.hpp"

using namespace sf;

/// //////////////////////////////////////////////
/// Locomotive
class Locomotive
{
public:

    Locomotive() {}
    Locomotive(const Vector2f& _pos);

    void draw( RenderTarget& rt, Vector2u viewPos);

    void setDirectionConst( const Vector2f& dir);

    void setDirection( Vector2f dir);

    void update( float dt);

    Vector2f getPos();

private:
    static constexpr float speed = 16.0f;  // move 100 px per second
    Vector2f pos;
    Vector2f vel = {0.0f, 0.0f};
    Texture texture;
    Sprite sprite;
};

#endif
