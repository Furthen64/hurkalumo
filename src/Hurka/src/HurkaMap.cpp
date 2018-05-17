#include "HurkaMap.hpp"
#include "Node.hpp"


// VERSION 3            2018-05-16
// VERSION 2            2018-03-20




// (-+)
HurkaMap::HurkaMap(std::string _mapName, int **_matrix, int mtxRows, int mtxCols)
{

    mapName = _mapName;
    matrix = _matrix;
    matrixRows = mtxRows;
    matrixCols = mtxCols;
}



/// \brief Will find the index, the int position in the blocklist, where a HPos * matches up
/// \param blockpos Allocated, values set, a position on the gameboard where an object is drawn (a Block)
/// \return index on OK, -1 on FAIL
// See  docs\ HurkaMap - indexInList.png
// (-+)
int HurkaMap::indexInBlockList(HPos *blockpos)
{
    int debugLevel = 0;
    int nr = 0;
    Block *workBlock = nullptr;
    int searchId1 = -1;
    int searchId2 = -1;
    bool foundMatch = false;



    if(blockpos == nullptr) {
        return -1;
    }

    if(debugLevel >=1) {
            std::cout << "** indexInBlockList() \n{";
            std::cout << "param blockpos:\n";
            blockpos->dump("  ");
    }



    // Iterate over the blocklist, find the searchId


    searchId1 = Node::genIDfrom_abs_iso(blockpos);

    for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
    {
        workBlock = (*it);

        searchId2 = Node::genIDfrom_abs_iso( workBlock->getHPos() );

        if(searchId1 == searchId2) {
            if(debugLevel >=1) {
                std::cout << "match!\n";
            }
            foundMatch = true;
            break;
        }

        nr++;

    }


    if(!foundMatch) {
        if(debugLevel >=1) {
            std::cout << "could not find position in blockList...\n}\n";

        }
        return -1;
    }


    if(debugLevel >=1) {
        std::cout <<"\n}\n";
    }


    return nr;
}




// If you look at the gamematrix like this:
//
//
//                           (0,0)
//                       (1,0)   (0,1)
//                    (2,0)  (1,1)  (0,2)
//                  (3,0) (2,1) (1,2) (0,3)
//
// You could put a layernr on each layer  (each new line here in this asciiart)
//
//
//
//   Layer0                  (0,0)
//   Layer1              (1,0)   (0,1)
//   Layer2           (2,0)  (1,1)  (0,2)
//   Layer3         (3,0) (2,1) (1,2) (0,3)
//   ...
//
// Thats what this function does, it finds out on what layer a particular HPos would be.
/// \brief Finds out on what layer to render a particular block, given its HPos position
/// \param searchPos Allocated with values, abs_iso

// Please synch this with the readRegularFile .... needs to copy this documentation  to that one

// (--)
int HurkaMap::layerNrInBlockList(HPos *searchPos)
{

    int debugLevel = 1;
    // we know the  Y max in this shiiiit
    // make loop1 and loop2 like in "readRegularFile"

    // If you generate a whole isometric gamematrix again... like from a matrix
    // you could easily tell where a HPos would be , because you'd have all positions..
    //

    // So you STOP when you hit that HPos


    int layerNr = 0;
    HPos *workPos = nullptr;
    std::string searchPosStr = searchPos->absToString();
    std::string ind1 = "   ";



    // Loop 1:
    int yDown = 0;
    int yUp = 0;
    int xRight = 0;



    // Loop 2:

    int xDownRight = 0;



    if(debugLevel >=1) {

        std::cout << "\n\n* layerNrInBlockList()\n{";

        std::cout << ind1 << "matrixRows= " << matrixRows << ", matrixCols= " << matrixCols << "\n\n";

    }





    if(debugLevel >=1) { std::cout << ind1 << "Loop1:\n" << ind1 << "----------\n"; }

    std::cout << ind1 << "layerNr=" << layerNr << "\n";

    while(yDown < matrixRows)
    {
        while(yUp > -1 && xRight < matrixCols)
        {

            // matrix coordinates:
            //   row = yUp
            //   col = xRight

            workPos = new HPos( yUp, xRight, USE_ISO);

            std::cout << ind1 << "At= " << workPos->absToString() << " vs Search=" << searchPosStr << " \n";


            if(workPos->compareAbsIso(searchPos) == 0) {
                std::cout << ind1 << "   Match found!\n";
                return layerNr;
            }


            yUp--;
            xRight++;
        }



        layerNr++;
        std::cout << ind1 << "layerNr=" << layerNr << "\n";


        // Go down the matrix in the Y axis
        yDown++;

        // Reset x iterator
        xRight = 0;

        // Now yUp starts a step lower down
        yUp = yDown;
    }



    std::cout << ind1 << "\n\nLoop2:\n" << ind1 << "-------------\n";


    std::cout << ind1 << "layerNr=" << layerNr << "\n";


    yUp = matrixRows-1; // Start at the Bottom Left

    xDownRight++;

    xRight = xDownRight;

    while(xDownRight < matrixCols)
    {
        while(yUp > -1 && xRight < matrixCols)
        {
            workPos = new HPos(yUp, xRight, USE_ISO);

            std::cout << ind1 << "At= " << workPos->absToString() << " vs Search=" << searchPosStr << " \n";

            if(workPos->compareAbsIso(searchPos) == 0) {
                std::cout << ind1 << "   Match found!\n";
                return layerNr;
            }

            yUp--;      // going upwards...
            xRight++;   // ...and right

        }

        layerNr++;
        std::cout << ind1 << "layerNr=" << layerNr << "\n";

        xDownRight++;

        xRight = xDownRight;
        yUp = matrixRows -1;
    }



    if(debugLevel >=1) {
        std::cout << "\n}\n\n";
    }


    return 0;







}



// Needs two loops to iterate and graphically output the data,
// if you imagine an isometric tile, it has a top part and a bottom part:
//
//                                  /\     Loop 1
//                                 /  \ ____________
//                                 \  /
//                                  \/     Loop 2
// (--) TEST
void HurkaMap::dump(std::string ind)
{
    std::cout << ind << "** Dumping Hurkamap: \n{\n";


    int nr = 0;
    int div = 0;
    std::string indent = "                                     ";
    bool loop1 = true;
    int nrLoop1 = 0;

    Block *workBlock = nullptr;

    for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
    {
        workBlock = (*it);
        std::cout << workBlock->getHPos()->absToString() << " ";


    }


    std::cout << ind << "\n} ** \n";
}






// (-+)
void HurkaMap::draw(RenderTarget& rt, HPos *viewHPos)
{
    // iterate over all items
     for (std::list<Block *>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
     {
       (*itAll)->draw(rt, viewHPos);
     }

}




/// \brief Places a Road in the gamematrix at given position, or if one already exists, Swaps its current road to another road

// BUG 2018-05  It generates a tonne of blocks when I just add one. It doubles the amount of blocks being rendered.             Solved.
//              SOLVED by ending a loop that just kept on going.

// (--)

// TEST
int HurkaMap::placeNewOrSwapRoad(HPos *roadPos, int debugLevel)
{


    if(debugLevel >=1 ) {
        std::cout << "** placeNewOrSwapRoad()\n{\n";
        std::cout << "   param roadPos:   " << roadPos->absToString() << "\n";
    }



    Block *workBlock = nullptr;

    int nr = indexInBlockList(roadPos);


    int layerNr = layerNrInBlockList(roadPos);

    std::cout << "layerNrInBlockList(roadpos) gave us= " << layerNr << "\n\n";

    std::cout << "indexinblocklist(roadPos) gave us= " << nr << "\n\n";


    int searchId1 = -1;
    int searchId2 = -1;


    bool createdNewBlock = false;


    searchId1 = Node::genIDfrom_abs_iso(roadPos);


    // If already exist
    if(nr != -1) {


            // Wishlist: Replace the existing or swap or rotate a Road like in OpenTTD

            // For now, just set a road there

            for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
            {

                // Current Block on position
                workBlock = (*it);

                searchId2 = Node::genIDfrom_abs_iso(workBlock->getHPos());

                if(searchId1 == searchId2) {

                    std::cout << "textureName=" << workBlock->getTextureName() << "\n";
                    std::cout << "textureId=" << workBlock->getTextureName() << "\n";

                    //  (*it) = new Block(roadPos, "ROAD001");     // BUG FIXME something crashes if you try wrong textuername
                    (*it) = new Block(roadPos, "HOUSE001");     // BUG FIXME something crashes if you try wrong textuername


                    break;


                }


            }

    }





    // If it does not exist

    if(nr == -1) {

            for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
            {
                workBlock = (*it);

                searchId2 = Node::genIDfrom_abs_iso(workBlock->getHPos());

                if(searchId2 > searchId1) {
                    // We are there! lets insert a new object into the list

                    blockList.insert(it, new Block(roadPos, "HOUSE001"));                           // FIXME Im not sure... how this should be done. also see below

                    createdNewBlock = true;

                    break;
                }
            }

            if(!createdNewBlock) {

                // Oh? well then we must be placing one last in the blocklist
                //blockList.push_back(new Block(roadPos, "ROAD001"));
                blockList.push_back(new Block(roadPos, "HOUSE001"));                            // FIXME Not sure this is right... Renderorder lol?

            }
    }





    if(debugLevel >=2) {
        std::cout << "\n blocklist contents:\n";
        dump("   ");
    }



    if(debugLevel >=1) {     std::cout << "\n} **\n"; }




    return 0;
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



int HurkaMap::getNrBlocks()
{
    return blockList.size();
}


// (+-)
/// Can only be used after you have loaded a file!
///
/// This function parses the map for all the roads
/// puts a 1 where the road is and 0 where anything else is
HurkaMatrix *HurkaMap::getRoadHMatrix()
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
