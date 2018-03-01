#ifndef HURKAMAP_H
#define HURKAMAP_H

#include <list>
#include <string>
#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "Block.hpp"

// A container for the    std::list<Block *> blockList which we will use to render all the blocks on the board


using namespace sf;

class HurkaMap
{
public:

    HurkaMap(std::string _mapName, TextureManager *_textMgr);

    void draw(RenderTarget& rt);

    void testList();

    void putBlockList(std::list<Block *> _blockList);

    Block *getBlock();

    std::string mapName;

private:

    TextureManager *textMgr;    // Lazy reference, fixme someday with redesign, singleton?
    std::list<Block *> blockList; // All the blocks we render
    std::string cn = "HurkaMap.cpp";

};


#endif
