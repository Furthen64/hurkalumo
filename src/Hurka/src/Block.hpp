#ifndef BLOCK_H
#define BLOCK_H



#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "GameMatrix.hpp"
#include "Constants.hpp"


using namespace sf;

/// //////////////////////////////////////////////
/// (--) Block
/// ? x ? pixels        Auto adjusted where its positioned for drawing based upon its texture resolution

class Block
{
public:
    Block(const Vector2f& _pos, std::string _textureName, TextureManager *textureMgr);

    void draw( RenderTarget& rt);

private:
    int textureID;
    Texture texture;
    Sprite sprite;
    Vector2f pos;
    IntRect textureSize;

};

#endif
