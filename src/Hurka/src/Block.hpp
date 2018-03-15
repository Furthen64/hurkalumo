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
    Block(const Vector2f& _pos, std::string _textureName);
    Block(const Vector2f& _pos, int textureId);

    void draw( RenderTarget& rt, Vector2i viewPos);

    void dump();

    int getTextureID();

    void setTextureByName(std::string _textureName);

    Block *clone();

    std::string getTextureName();



private:
    int textureID;
    Texture texture;
    IntRect textureSize;
    std::string textureName;
    Sprite sprite;
    Vector2f pos;
    std::string cn = "Block.cpp";

};

#endif
