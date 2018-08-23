#include "HurkaMap.hpp"
#include "Node.hpp"




// Used for a clean hurkamap with no data
// (--) untested
HurkaMap::HurkaMap(int mtxRows, int mtxCols)
{

    fullUriMapName = "";
    matrix = allocateMatrix(mtxRows, mtxCols);
    matrixRows = mtxRows;
    matrixCols = mtxCols;
}


// Has been working for quite some time now, months
// (+-)
HurkaMap::HurkaMap(std::string _fullUriMapName, int **_matrix, int mtxRows, int mtxCols)
{

    fullUriMapName = _fullUriMapName;
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





// Needs two loops to iterate and graphically output the data,
// if you imagine an isometric tile, it has a top part and a bottom part:
//
//                                  /\     Loop 1
//                                 /  \ ____________
//                                 \  /
//                                  \/     Loop 2
// (--)  test more plz
void HurkaMap::dump(std::string ind)
{
    Block *workBlock = nullptr;


    std::cout << ind << "{\n";
    std::cout << ind << "  ";
    for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
    {
        workBlock = (*it);
        std::cout << workBlock->getHPos()->absToString() << " ";
    }
    std::cout << "\n" << ind << "}\n";
}



/// \brief Will figure out what is under the mouse cursor and run .dump() function on
///        all the objects.
// (-+)
void HurkaMap::dumpEverythingAtPos(HPos *searchPos, TrafficManager *tm, std::string ind)
{

    Block *workBlock = nullptr;
    std::string ind2 = ind;
    ind2 += "   ";

    if(searchPos == nullptr) {
        std::cout << "dumpEverythingAtPos() searchpos = nullptr!\n";
        return ;
    }

    std::cout << ind << "---- POS " << searchPos->absToString() << " ---------\n";
    std::cout << ind << "{\n";



    // Blocks?


    for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
    {

        workBlock = (*it);

        if(workBlock->getHPos()->compareAbsIso(searchPos) == 0) {

            workBlock->minimaldump(ind2);

        }

    }



    // roadNetwork?

    RoadNetwork *roadnet = tm->roadNetworkAtPos(searchPos);

    if(roadnet != nullptr) {

        roadnet->dump(ind2);
    }


    // Clicked on a bus ON that roadnetwork?
    if(roadnet != nullptr) {

        Bus *bus = roadnet->busAtPos(searchPos);        // alpha-0.2: What if there are several buses at the same iso tile? Loop!

        if(bus != nullptr) {

            bus->dump(ind2);
        }

    }

    std::cout << ind << "}\n";
}




// (--)
int **HurkaMap::getMatrix()
{
    return matrix;
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
/// \param roadPos Allocated, values set, position of the new road you want to place
/// \return -1 on failure, 0 on OK
// BUG 2018-05  It generates a tonne of blocks when I just add one. It doubles the amount of blocks being rendered.             Solved.
//              SOLVED by ending a loop that just kept on going.
//
// (--+)            Placement works, renders OK
//                  Changing road type works

int HurkaMap::placeNewOrSwapRoad(HPos *roadPos, int debugLevel)
{

    std::string ind = "  ";

    if(debugLevel >=1 ) {
        std::cout << "placeNewOrSwapRoad()\n{\n";
        std::cout << ind << " - roadPos:   " << roadPos->absToString() << "\n";
    }



    std::string textureStr = "          ";
    Block *workBlock = nullptr;
    int nr = 0;
    int searchPosID = -1;
    int currPosID = -1;
    bool createdNewBlock = false;
    std::list<Block *>::iterator itContinue;





    searchPosID = Node::genIDfrom_abs_iso(roadPos);



    /// Try and find position in the Blocklist

    nr = indexInBlockList(roadPos);

    // If already exist
    if(nr != -1) {

        for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
        {

            // Current Block on position
            workBlock = (*it);

            currPosID = Node::genIDfrom_abs_iso(workBlock->getHPos());



            if(searchPosID == currPosID) {


                textureStr = workBlock->getTextureName();

                if(debugLevel >=2) {
                    std::cout << ind << " Found match! " << searchPosID << " vs " << currPosID << "\n";

                    std::cout << ind << " TextureName = \"" << textureStr << "\", TextureID=" << workBlock->getTextureID() << "\n";
                }



                // Already a Road, rotate among the different kinds

                if(textureStr == "ROAD001") {
                    textureStr = "ROAD002";
                } else if(textureStr == "ROAD002") {
                    textureStr = "ROAD003";
                } else if(textureStr == "ROAD003") {
                    textureStr = "ROAD004";
                } else if(textureStr == "ROAD004") {
                    textureStr = "ROAD005";
                } else if(textureStr == "ROAD005") {
                    textureStr = "ROAD006";
                } else if(textureStr == "ROAD006") {
                    textureStr = "ROAD007";
                } else if(textureStr == "ROAD007") {
                    textureStr = "ROAD008";
                } else if(textureStr == "ROAD008") {
                    textureStr = "ROAD009";
                } else if(textureStr == "ROAD009") {
                    textureStr = "ROAD010";
                } else if(textureStr == "ROAD010") {
                    textureStr = "ROAD001";                     // And Around we go!
                }



                (*it) = new Block(roadPos, textureStr);     // BUG FIXME something crashes if you try wrong texturename

                if(debugLevel >=1) {
                    std::cout << ind << " - UPDATED!\n";
                    std::cout << "\n}\n\n";
                }

                return 0;
            }

        }
    }





    /// If it does not exist

    bool sameLayer = false;
    int searchPosLayer = 0;
    int currLayer = 0;

    searchPosLayer = Grid::layerNrInBlockList(roadPos);
    if(debugLevel >=2) {
        std::cout << ind << " - parameter \"roadpos\"'s searchPosLayer= " << searchPosLayer << "\n\n";
    }


    if(debugLevel >=2) {
        std::cout << ind << "Searching for blocks already existing on that layernr\n" << ind << "---------------------\n";
    }

    if(nr == -1) {

        /// Find the right layer for it

        for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
        {
            workBlock = (*it);


            // First find a block on the same layer

            if(!sameLayer) {
                currLayer = Grid::layerNrInBlockList(workBlock->getHPos());


                if(currLayer == searchPosLayer) {

                   if(debugLevel >=2) {
                        std::cout << ind << "This workblock is at same layer:\n";
                        workBlock->getHPos()->dump("   ");
                        std::cout << "\n";
                   }

                   sameLayer = true;
                }
            }


            if(sameLayer) {

                // Now find the right position in the blocklist here at this layer

                if(HPos::ArightOfB(workBlock->getHPos(), roadPos) ) {

                    if(debugLevel >=2) {
                        std::cout << ind << "Comparing with ArightOFB(workblock, roadpos):\n";
                        workBlock->getHPos()->dump("  ");
                        roadPos->dump("  ");
                    }

                    if(debugLevel >=2) {
                        std::cout << " - Found a block that matches layernr, and is RIGHT of our searched \"roadpos\".\n";
                    }


                    blockList.insert(it, new Block(roadPos, "ROAD001"));
                    if(debugLevel >=1) {
                        std::cout << ind << "- INSERTED!\n";
                        std::cout << "\n}\n\n";
                    }

                    return 0;
                }

                // Reset flag

                sameLayer = false;
            }
        }


        if(!createdNewBlock)
        {

            // Could NOT find a block at our search layer, find out if there is a Block further down

            if(debugLevel >=2) {
                std::cout << "\n" << ind << "  - Nothing found.";

                std::cout << "\n\n" << ind << "Could NOT find a block at our search layer, find out if there is a Block further down\n";
                std::cout << ind << "---------------------------------------------------------------\n";

            }



            for(std::list<Block *>::iterator it = blockList.begin(); it != blockList.end(); ++it)
            {

                workBlock = (*it);

                currLayer = Grid::layerNrInBlockList(workBlock->getHPos());

                if(currLayer > searchPosLayer) {

                    // We are there, now insert our block just before this one
                    if(debugLevel >=2) { std::cout << "Found a right place for this Block!\n"; }

                    blockList.insert(it, new Block(roadPos, "ROAD001"));       // Inserts BEFORE the current iterator


                    createdNewBlock = true;


                    if(debugLevel >=1) {  std::cout << ind << " - INSERTED! \n}\n\n"; }

                    return 0;
                }

            }

        }









        if(!createdNewBlock) {

            if(debugLevel >=2 ) {
                std::cout << "\n";
                std::cout << ind << " - Nothing found.\n\n";

                std::cout << ind << "Could NOT find a block beneath us, so lets just go ahead and create at the back of the blocklist!\n";
                std::cout << ind << "-------------------------------------------------------------------------------------------------------\n";
            }
            blockList.push_back(new Block(roadPos, "ROAD001"));
            createdNewBlock = true;

            if(debugLevel >=1) {
                std::cout << ind << " - INSERTED.\n\n";
                std::cout << "\n}\n\n";
            }


            return 0;

        }

    }


    std::cout << ind << " Sorry, could not help you out. Bug?\n";


    return -1;
}








// (--)
int HurkaMap::placeNewRoad(HPos *roadPos, int debugLevel )
{

    std::cout << "NOT IMPLEMEENTED!\n";
    return 0;
}






// (++)
void  HurkaMap::putBlockList(std::list<Block *> _blockList)
{
    blockList = _blockList;

    // Update Rows n Cols

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
