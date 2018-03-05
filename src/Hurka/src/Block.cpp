
#include "Block.hpp"

//
// Block
// N x N pixels        Auto adjusted where it's positioned for drawing based upon its texture resolution
//
/// pos          = {0,1}   0=col, 1=row for example
/// texturename  = "HOUSE001" or other key , see TextureManager
/// textureMgr   = must be reference to existing TextureManager
/// (-+)
Block::Block(const Vector2f& _pos, std::string _textureName)
    :
    pos(_pos)
{


    if(_textureName == "") {
        std::cout << "ERROR " << cn << " Could not create block with empty texturename=" << _textureName << "\"!\n";
        return ;
    }


    int debugLevel = 0;



    TextureManager *textureMgr;
    textureMgr = textureMgr->getInstance();

    bool result = textureMgr->applyTexture(_textureName, &texture);






    if(!result) {
            std::cout << "ERROR Block creation, cannot find texture \"" << _textureName << "\".\n";
        return ;
    }

    sprite = Sprite(texture);

    textureSize = sprite.getTextureRect();


    if(debugLevel > 0) {
        std::cout << "height= " << textureSize.height << ", width=" << textureSize.width << "\n";
    }


    textureName = _textureName;

}


// You can also create a block by supplying texture id as an integer,
// e.g. 001 => "HOUSE001"
//
// (--)

Block::Block(const Vector2f& _pos, int _textureId)
    :
    pos(_pos)
{

    int debugLevel = 0;


    TextureManager *textureMgr;
    textureMgr = textureMgr->getInstance();


    textureName = textureMgr->getTextureNameByIndex(_textureId);


    bool result = textureMgr->applyTexture(textureName, &texture);



    if(!result) {
            std::cout << "ERROR Block creation, cannot find texture id \"" << _textureId << "\".\n";
        return ;
    }

    sprite = Sprite(texture);

    textureSize = sprite.getTextureRect();


    if(debugLevel > 0) {
        std::cout << "height= " << textureSize.height << ", width=" << textureSize.width << "\n";
    }

}



// (--)
void Block::draw( RenderTarget& rt)
{

    int x = getWindowXPos(pos.x,pos.y, textureSize.width, textureSize.height);
    int y = getWindowYPos(pos.x,pos.y, textureSize.width, textureSize.height);
    Vector2f _pos = {(float)x,(float)y};
    sprite.setPosition(_pos);
    rt.draw(sprite);


    /*
    std::cout << "-----------------block------------------\n";
    std::cout << "Name: " << textureName << "\n";
    std::cout << "ID: " << textureID << "\n";
    std::cout << "texturesize: " << textureSize.width  << " x " << textureSize.height << "\n";
    std::cout << "getXPos(" << pos.x << ", " << pos.y << ") = " << getWindowXPos(pos.x,pos.y, textureSize.width, textureSize.height) << "\n";
    std::cout << "getYPos(" << pos.x << ", " << pos.y << ") = " << getWindowYPos(pos.x,pos.y, textureSize.width, textureSize.height) << "\n";
    */
}

int Block::getTextureID()
{
    return textureID;
}


std::string Block::getTextureName()
{
    return textureName;
}


// (--)
void Block::setTextureByName(std::string _textureName)
{


    TextureManager *textureMgr;
    textureMgr = textureMgr->getInstance();


    textureName = _textureName;


    bool result = textureMgr->applyTexture(_textureName, &texture);


    if(!result) {
        std::cout << "ERROR Block creation, cannot find texture \"" << _textureName << "\".\n";
        return ;
    }

    sprite = Sprite(texture);

    textureSize = sprite.getTextureRect();

    std::cout << "height= " << textureSize.height << ", width=" << textureSize.width << "\n";


}



// (--)
// TEST
Block *Block::clone()
{

    Block *block = new Block({pos.x, pos.y}, textureName);
    return block;
}





















