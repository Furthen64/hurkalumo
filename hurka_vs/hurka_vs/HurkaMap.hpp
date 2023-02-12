#ifndef HURKAMAP_H
#define HURKAMAP_H

#include <list>
#include <string>
#include <SFML/Graphics.hpp>

#include "TrafficManager.hpp"           // <= utils.hpp,   <= texturemanager.hpp ,  <= block.hpp

// HurkaMap
// is a container for the std::list<Block *> blockList which we will use to render all the blocks on the board
// The list is just a flat long list of all the Blocks to be rendered. They are put in a particular order for the
// rendering process to draw them layer by layer in an isometric way... hard to explain in text.
// see Docs / Parsing_Matrix_to_BlockList.mp4


// 2018-05-17       jörgen engström         CR15 Making the Matrix size, same size as GameMatrix




using namespace sf;

class HurkaMap
{
public:

    HurkaMap(std::string _fullUriMapName, int **_matrix, int mtxRows, int mtxCols);
    HurkaMap(int mtxRows, int mtxCols);
    ~HurkaMap();


    void draw(RenderTarget& rt, HPos *viewHPos);

    void testList();

    int placeNewOrSwapRoad(HPos *roadPos, int debugLevel);

    int placeNewRoad(HPos *roadPos, int debugLevel );

    int indexInBlockList(HPos *);

    int **getMatrix();


    int layerNrInBlockList(HPos *);

    void putBlockList(std::list<Block *> _blockList);

    std::string fullUriMapName;

    HurkaMatrix *getRoadHMatrix();


    int getNrBlocks();


    // Debug utils

    void dumpEverythingAtPos(HPos *, TrafficManager *, std::string );

    void dump(std::string);

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
