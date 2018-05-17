#include "Block.hpp"

//
// A Block is a thing that gets rendered on an isometric tile, like a House, Road or a Tree.
// Its texture resolution determines the correct height for positioning on that tile.
//
// Stored in a "HurkaMap"
// Used by Core





/// Block constructor
///
/// @param _hpos         An HPos object with the absolute iso positions set
/// @param texturename   Unique string to identify a texture object with, e.g. "HOUSE001", see TextureManager class
///
/// (-+)
Block::Block(HPos *_hpos, std::string _textureName)
{
    int debugLevel = 0;


    if(_textureName == "") {
        std::cout << "ERROR " << cn << " Could not create block with empty texturename=" << _textureName << "\"!\n";
        return ;
    }


    // Apply texture
    TextureManager *textureMgr;
    textureMgr = textureMgr->getInstance();

    bool result = textureMgr->applyTexture(_textureName, &texture);
    if(!result) { std::cout << "ERROR Block creation, cannot find texture \"" << _textureName << "\".\n";  return ;  }

    textureName = _textureName;



    // Set SFML sprite
    sprite = Sprite(texture);
    textureSize = sprite.getTextureRect();


    if(debugLevel > 0) {
        std::cout << "height= " << textureSize.height << ", width=" << textureSize.width << "\n";
    }


    // Now we have texture size, set the gPix positions
    set_pos_by_abs_iso(_hpos);
}


/// Block constructor, create a block by supplying texture id as an integer
///
/// @param _hpos         An HPos object with the absolute iso positions set
/// @param _textureId    Unique integer to identify a texture object with, e.g. 001 which maps to "HOUSE001", see TextureManager class
///
/// (--)
Block::Block(HPos *_hpos, int _textureId)
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

    set_pos_by_abs_iso(_hpos);

}








/// Uses the internal "hpos" object for gameworld position (e.g. it's at iso position {7,1} and at gamepix position {1000,600} )
/// and the "viewpos" viewing position object to know where we are looking at the moment
///
/// @param rt       SFML RenderWindow thingie
/// @param viewPos  A rectangle which holds the starting x and starting y for looking at the gameboard
///
/// (-+)
void Block::draw( RenderTarget& rt, HPos *viewHPos)
{
    int x = hpos->gpix_x;
    int y = hpos->gpix_y;


    // Viewing offset
    x += viewHPos->gpix_x;
    y += viewHPos->gpix_y;


    Vector2f _pos = {(float)x,(float)y};


    sprite.setPosition(_pos);

    rt.draw(sprite);

}



// (-+)
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




// Sets the position by a hpos object,
// The only thing we know is set is the "abs_iso" position data
//
// (-+)
void Block::set_pos_by_abs_iso(HPos *_abs_iso)
{


    hpos = _abs_iso;

    hpos->gpix_x = Grid::convert_iso_to_gpix_x(hpos->abs_iso_y, hpos->abs_iso_x, textureSize.width, textureSize.height, 2);
    hpos->gpix_y = Grid::convert_iso_to_gpix_y(hpos->abs_iso_y, hpos->abs_iso_x, textureSize.width, textureSize.height, 2);

}



// (++)
void Block::dump()
{


    std::cout << "-----------------block------------------\n";
    std::cout << "Name: " << textureName << "\n";
    std::cout << "ID: " << textureID << "\n";
    std::cout << "texturesize: " << textureSize.width  << " x " << textureSize.height << "\n";
    std::cout << "position:\n";
    std::cout << "  absolute iso position = " << hpos->abs_iso_y << ", " << hpos->abs_iso_x << "\n";
    std::cout << "  gameworld pixel position = " << hpos->gpix_y << ", " << hpos->gpix_x << "\n";

}





// (--)
// TEST
Block *Block::clone()
{
    std::cout << "Block.clone(): UNDEFINED BEHAVIOUR !! Needs more testing.\n";

//    Block *block = new Block({pos.x, pos.y}, textureName);

    Block *block = new Block(hpos->clone(), textureName);

    // FIXME add delete

    return block;
}




/// \return The internal Hpos pointer!
// (--)
HPos *Block::getHPos()
{
    return hpos;
}





int Block::getTextureID()
{
    return textureID;
}


std::string Block::getTextureName()
{
    return textureName;
}
