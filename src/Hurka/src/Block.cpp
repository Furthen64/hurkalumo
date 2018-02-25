
#include "Block.hpp"

/// //////////////////////////////////////////////
/// (--) Block
/// N x N pixels        Auto adjusted where it's positioned for drawing based upon its texture resolution


Block::Block(const Vector2f& _pos, std::string _textureName, TextureManager *textureMgr)
    :
    pos(_pos)
{

    bool result = textureMgr->applyTexture(_textureName, &texture);

    if(!result) {
            std::cout << "ERROR Block creation, cannot find texture \"" << _textureName << "\".\n";
        return ;
    }

    sprite = Sprite(texture);

    textureSize = sprite.getTextureRect();


    std::cout << "height= " << textureSize.height << ", width=" << textureSize.width << "\n";

}


void Block::draw( RenderTarget& rt)
{

    /*DEBUG /
    std::cout << "---------------------------------------\n";
    std::cout << "ID: " << textureID << "\n";
    std::cout << "getXPos(" << pos.x << ", " << pos.y << ") = " << GameMatrix::getWindowXPos(pos.x,pos.y) << "\n";
    std::cout << "getXPos(" << pos.x << ", " << pos.y << ") = " << GameMatrix::getWindowYPos(pos.x,pos.y) << "\n";
    */

    int x = getWindowXPos(pos.x,pos.y, textureSize.width, textureSize.height);
    int y = getWindowYPos(pos.x,pos.y, textureSize.width, textureSize.height);
    Vector2f pos = {(float)x,(float)y};
    sprite.setPosition(pos);
    rt.draw(sprite);
}

int Block::getTextureID()
{

    return textureID;
}
