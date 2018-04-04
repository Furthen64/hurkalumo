#include "HurkaMap.hpp"


// VERSION 2            2018-03-20




// (-+)
HurkaMap::HurkaMap(std::string _mapName, int **_matrix, int mtxRows, int mtxCols)
{

    mapName = _mapName;
    matrix = _matrix;
    matrixRows = mtxRows;
    matrixCols = mtxCols;
}


// TEST
// (--)
void HurkaMap::draw(RenderTarget& rt, Vector2i viewPos)
{

    // iterate over all items
     for (std::list<Block *>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
     {
       (*itAll)->draw(rt, viewPos);
     }

}

// (++)
void  HurkaMap::putBlockList(std::list<Block *> _blockList)
{
    blockList = _blockList;

    // Update Rows n Cols

}


// (--)
// Complete this,
// Error handling when empty list for instance
Block *HurkaMap::getBlock()
{

std::cout << "getBlock not done (--)\n";
    Block *blockRef = nullptr;

    for (std::list<Block *>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
    {

        blockRef = (*itAll);
    }

    return blockRef;
}



// (+-)
/// Can only be used after you have loaded a file!
///
/// This function parses the map for all the roads
/// puts a 1 where the road is and 0 where anything else is
HurkaMatrix *HurkaMap::getRoadMatrix()
{
    HurkaMatrix *newMatrix = new HurkaMatrix(matrixRows, matrixCols);

    for(int y = 0; y < matrixRows; y++)
    {

        for(int x = 0; x < matrixCols; x++)
        {


            // Is it a road?    (texture id between 100-200 )
            if( (matrix[y][x] > 99) && (matrix[y][x] < 200) ) {

                   newMatrix->matrix[y][x] = 1;

               }
               else
               {
                   // Another regular block
                   newMatrix->matrix[y][x] = 0;

               }

        }

    }


    return newMatrix;


}



HurkaMap::~HurkaMap()
{
    // Delete the matrix
    if (matrixRows>0) delete [] matrix[0];
    delete [] matrix;
}
