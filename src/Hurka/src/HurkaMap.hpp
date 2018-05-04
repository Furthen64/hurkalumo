#ifndef HURKAMAP_H
#define HURKAMAP_H

#include <list>
#include <string>
#include <SFML/Graphics.hpp>

#include "Utils.hpp"
#include "TextureManager.hpp"
#include "Block.hpp"

// A container for the    std::list<Block *> blockList which we will use to render all the blocks on the board


using namespace sf;

class HurkaMap
{
public:

    HurkaMap(std::string _mapName, int **_matrix, int mtxRows, int mtxCols);
    ~HurkaMap();

    void draw(RenderTarget& rt, HPos *viewHPos);

    void testList();

    void putBlockList(std::list<Block *> _blockList);

    Block *getBlock();

    std::string mapName;

    HurkaMatrix *getRoadMatrix();


    // public members

    int getRows() { return matrixRows; }
    int getCols() { return matrixCols; }
private:

    std::list<Block *> blockList; // All the blocks we render
    std::string cn = "HurkaMap.cpp";
    int **matrix;
    int matrixRows;
    int matrixCols;

};


#endif
