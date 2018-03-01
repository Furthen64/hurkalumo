#include "HurkaMap.hpp"



// (-+)
HurkaMap::HurkaMap(std::string _mapName, TextureManager *_textMgr)
{
    textMgr = _textMgr;
    mapName = _mapName;
}


// TEST
// (--)
void HurkaMap::draw(RenderTarget& rt)
{

    // iterate over all items
     for (std::list<Block *>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
     {
       (*itAll)->draw(rt);
     }

}

// (++)
void  HurkaMap::putBlockList(std::list<Block *> _blockList)
{
    blockList = _blockList;
}


// (--)
// felhantering vid tom lista t.ex
Block *HurkaMap::getBlock()
{

    Block *blockRef;

    for (std::list<Block *>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
    {

        blockRef = (*itAll);
    }

    return blockRef;
}
