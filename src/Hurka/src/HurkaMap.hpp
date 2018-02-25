#ifndef HURKAMAP_H
#define HURKAMAP_H

#include "TextureManager.hpp"
#include "Block.hpp"
#include <list>
#include <string>


/// std::list is sequential STL container that is internally implemented as doubly linked list.
// Yes! that's what I need.



class HurkaMap
{
public:

    HurkaMap(std::string _mapName, TextureManager *_textMgr);

    void testList();

    std::string mapName;

private:

    TextureManager *textMgr;
    std::string cn = "HurkaMap.cpp";

};


#endif
