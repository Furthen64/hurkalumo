#include "TrafficManager.hpp"



// (++)
TrafficManager::TrafficManager()
{
    roadNetworks = new std::list<RoadNetwork *>();
}






// (-+) Works with at least One bus on One roadnet
void TrafficManager::drawBuses(sf::RenderWindow &rt, HPos *viewHPos)
{
    RoadNetwork *currRoadnet = nullptr;

    for(std::list<RoadNetwork *>::iterator roadsIter=roadNetworks->begin(); roadsIter != roadNetworks->end(); ++roadsIter)
    {

        currRoadnet = (*roadsIter);

        for(std::list<Bus *>::iterator busIter=currRoadnet->buslist->begin(); busIter != currRoadnet->buslist->end(); ++busIter)
        {
            (*busIter)->draw(rt, viewHPos, true);
        }
    }

}


















/// Follows connected 1:s in the fullRoadMatrix and puts 1:s in newMatrix
///
/// @param fullRoadMatrix      Already allocated HurkaMatrix, A matrix with 1:s where the road sprites are, 0:s everywhere else
/// @param newMatrix           Already allocated HurkaMatrix, empty on entry, same size as fullRoadMatrix. Populated with values when function returns.
/// @param curr_iso_pos        The current absolute iso_pos for the algorithm
/// @param min_iso_pos         Already existing hpos object, will be filled on output.
/// @param visited             BinarySearchTree already allocated, empty on entry. Will be filled when function returns.
///
/// (-+) Worked once
/// Might be buggy, look at CR5 bug we had... um and look at the function createSlotpath
/// RECURSIVE
void TrafficManager::follow(HurkaMatrix *fullRoadMatrix,
                            HurkaMatrix *newMatrix,
                            HPos *curr_iso_pos,
                            HPos *min_iso_pos,
                            HPos *max_iso_pos,
                            BinarySearchTree *visited,
                            int debugLevel)
{
    std::string ind = "      ";

    if(debugLevel >= 2) {
        std::cout << ind << "follow()    currpos=";
        curr_iso_pos->dump(ind);
    }

    if(debugLevel >= 2) {
        std::cout <<  "\n";
    }




    /// Have we been here?
    int searchId = Node::genIDfrom_abs_iso(curr_iso_pos);
    if(visited->findVal(searchId,0) != -1 ) {
        return ;                                                                                 // END RECURSION
    }



    /// Setup objects


    HPos  *up_iso = curr_iso_pos->clone();      // SPEEDUP could probably do without cloning and stuff and allocating all these HPos's
    up_iso->abs_iso_y -= 1;

    HPos *right_iso = curr_iso_pos->clone();
    right_iso->abs_iso_x += 1;

    HPos *down_iso = curr_iso_pos->clone();
    down_iso->abs_iso_y += 1;

    HPos *left_iso = curr_iso_pos->clone();
    left_iso->abs_iso_x -= 1;



    /// Update objects

    // Put a "1" where we are at the moment
    newMatrix->matrix[(int)curr_iso_pos->abs_iso_y][(int)curr_iso_pos->abs_iso_x] = 1;

    // Add to visited
    visited->add(searchId,debugLevel);


    // Update the limits
    if(min_iso_pos->abs_iso_y > curr_iso_pos->abs_iso_y) {
        // new minimum! Top road
        min_iso_pos->abs_iso_y = curr_iso_pos->abs_iso_y;
    }

    if(min_iso_pos->abs_iso_x > curr_iso_pos->abs_iso_x) {
        // new minimum! left road
        min_iso_pos->abs_iso_x = curr_iso_pos->abs_iso_x;

    }


    if(max_iso_pos->abs_iso_x < curr_iso_pos->abs_iso_x) {
        // new max! right road
        max_iso_pos->abs_iso_x = curr_iso_pos->abs_iso_x;
    }

    if(max_iso_pos->abs_iso_y < curr_iso_pos->abs_iso_y) {
        // new max! down road
        max_iso_pos->abs_iso_y = curr_iso_pos->abs_iso_y;
    }









    /// Go up
    if(up_iso->abs_iso_y >= 0) { // Not hit the wall yet?
        if(fullRoadMatrix->matrix[(int)up_iso->abs_iso_y][(int)up_iso->abs_iso_x] == 1) { // is it a road?
            if(debugLevel >= 2) { std::cout << ind << "going up\n"; }
            follow(fullRoadMatrix, newMatrix, up_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                                 // RECURSION CALL
        }
    }


    /// Go right
    if(right_iso->abs_iso_x <= (fullRoadMatrix->cols-1)) {
        if(fullRoadMatrix->matrix[(int)right_iso->abs_iso_y][(int)right_iso->abs_iso_x] == 1) {
            if(debugLevel >= 2) { std::cout << ind << "going right\n";}
            follow(fullRoadMatrix, newMatrix, right_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                              // RECURSION CALL
        }
    }


    /// Go down
    if(down_iso->abs_iso_y <= (fullRoadMatrix->rows-1)) {
        if(fullRoadMatrix->matrix[(int)down_iso->abs_iso_y][(int)down_iso->abs_iso_x] == 1) {
            if(debugLevel >= 2) { std::cout << ind  << "going down\n"; }
            follow(fullRoadMatrix, newMatrix, down_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                               // RECURSION CALL
        }
    }



    /// Go left
    if(left_iso->abs_iso_x >= 0) {
        if(fullRoadMatrix->matrix[(int)left_iso->abs_iso_y][(int)left_iso->abs_iso_x] == 1) {
            if(debugLevel >= 2) { std::cout << ind  << "going left\n"; }
            follow(fullRoadMatrix, newMatrix, left_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                               // RECURSION CALL
        }
    }


                                                                                                // END RECURSION
}













// Calls a recursive function that walks through the matrix, following the 1:s
//
// wishlist: Makeover! Minimilize by using a RoadNetwork *pointer instead of the individual parts  (newMatrix, min_iso, max_iso...)
//
/// @param fullRoadMatrix  A matrix with all the roads as 1:s, anything else as 0:s
/// @param curr_iso_pos    The position of which to start following the 1:s
/// @param min_iso_pos     Minimum x and y limits
/// @param max_iso_pos     Maximum x and y limits
/// @param visited         used internally, just supply a new() one
/// @param debuglevel      Obvious

// test:    Worked once
// (-+)
RoadNetwork *TrafficManager::followAndAddToBST(HurkaMatrix *fullRoadMatrix,
                                               HPos *curr_iso_pos,
                                               HPos *min_iso_pos,
                                               HPos *max_iso_pos,
                                               BinarySearchTree *visited,
                                               int debugLevel)
{
    std::string ind = "   ";

    if(debugLevel >= 2)  {
        std::cout << ind << "\n\nfollowAndAddToBST():\n";
    }



    /// Create an empty matrix for the RoadNetwork

    HurkaMatrix *hmatrix = new HurkaMatrix(fullRoadMatrix->rows, fullRoadMatrix->cols);







    /// Follow the roads in the original matrix (the 1:s)
    /// When it's done, "hmatrix" should contain the roadnetwork's matrix

    if(debugLevel >= 2)  {
        std::cout << ind << "\nfollow():\n" << ind << "{\n";
    }

    follow(fullRoadMatrix, hmatrix, curr_iso_pos, min_iso_pos, max_iso_pos, visited, debugLevel);



    if(debugLevel >= 2)  {
        std::cout << ind << "}\n\n";
    }



    /// Setup the return object
    int newRows = max_iso_pos->abs_iso_y - min_iso_pos->abs_iso_y +1;  // lite os�ker, +-1 felet
    int newCols = max_iso_pos->abs_iso_x - min_iso_pos->abs_iso_x +1;

    /// We only want a matrix with the size containing roads, not the whitespace around it


    if(debugLevel >= 2)  {
        std::cout << "hmatrix now:\n";
        hmatrix->dump();
    }


    ///         int **copySubMatrix(int **srcMtx,    int srcRows,   int srcCols,   int startY,      int startX,  int height, int width)

    hmatrix->matrix = copySubMatrix(hmatrix->matrix, hmatrix->rows, hmatrix->cols, min_iso_pos->abs_iso_y, min_iso_pos->abs_iso_x, newRows,   newCols, debugLevel);
    if(hmatrix->matrix == nullptr) {
        std::cout << "ERROR " << cn << " Something went wrong while copying a subsection of a matrix to another matrix!\n";
        return nullptr;
    }
    hmatrix->rows = newRows;
    hmatrix->cols = newCols;


    if(debugLevel >= 2)  {
        std::cout << "New matrix after sub:\n";
        hmatrix->dump();
    }



    RoadNetwork *roadNetwork = new RoadNetwork();
    roadNetwork->hMatrix = hmatrix;

    if(debugLevel >= 2)  {
        std::cout << ind << "Completed a roadnetwork:\n";
        roadNetwork->dump(ind);
    }




    return roadNetwork;
}







/// Parse the 1:s in the roadmatrix, and group together them into RoadNetworks
/// After its done, it should have populated the std::list<RoadNetwork *> *roadNetworks;
///
/// (-+)
void TrafficManager::parseCurrentRoads(HurkaMatrix *roadMatrix, int debugLevel)
{

    if(debugLevel >= 1) {
        std::cout << "\n\nparseCurrentRoads()\n----------------------------------\n";
    }

    BinarySearchTree *bst = new BinarySearchTree();
    HPos *min_iso_pos = new HPos(0,0, USE_ISO);
    HPos *max_iso_pos = new HPos(0,0, USE_ISO);


    int searchId = -1;

    int M_LENGTH = roadMatrix->rows;
    int N_LENGTH = roadMatrix->cols;


    if(debugLevel >=1) {
        roadMatrix->dump();
    }


    if(debugLevel >=2) {
        bst->dump();
    }

    HPos *curr_iso = new HPos();



    /// Start your engines!
    /// Walk the Matrix

    // Reset the variables for each roadNetwork:
    min_iso_pos->abs_iso_y = roadMatrix->rows;
    min_iso_pos->abs_iso_x = roadMatrix->cols;

    max_iso_pos->abs_iso_y = 0;
    max_iso_pos->abs_iso_x = 0;




    if(debugLevel >=1) {std::cout << "\n\nIterating all the cells:\n-------------------\n";}
    for(int y = 0; y < M_LENGTH; y++) {
        for(int x = 0; x < N_LENGTH; x++) {


            curr_iso->abs_iso_y = y;
            curr_iso->abs_iso_x = x;

            searchId = Node::genIDfrom_abs_iso(curr_iso);

            if(debugLevel >=1) {std::cout << "Pos(" << y << ", " << x << ") id=" << searchId << "\n";}

            if(roadMatrix->matrix[y][x] == 1) {

                // Search in bst

                if(bst->findVal(searchId,0) != -1) {

                    /// found it!    Ignore it. We found it so it's already been visited.

                    if(debugLevel >=1) {std::cout << "Already in BST\n";}



                } else {


                    if(debugLevel >=1) {std::cout << "Running followAndAddToBst\n";}

                    /// Did not find it?
                    /// Start a "Follow and Add" recursion, which returns a roadNetwork


                    RoadNetwork *network = followAndAddToBST(roadMatrix, curr_iso, min_iso_pos, max_iso_pos, bst, debugLevel);

                    if(network == nullptr) {
                        std::cout << "ERROR " << cn << " parseCurrentRoads() failed while executing: followAndAddToBST(), got nullptr.\n";
                        std::cout << "Given roadmatrix:\n";
                        roadMatrix->dump();
                    }

                    network->min_isoYOffset = min_iso_pos->abs_iso_y;
                    network->min_isoXOffset = min_iso_pos->abs_iso_x;

                    network->max_isoYOffset = max_iso_pos->abs_iso_y;
                    network->max_isoXOffset = max_iso_pos->abs_iso_x;



                    /// Store a completed roadnetwork
                    roadNetworks->push_back(network);


                    /// Reset for the next round
                    min_iso_pos->abs_iso_y = roadMatrix->rows;
                    min_iso_pos->abs_iso_x = roadMatrix->cols;

                    max_iso_pos->abs_iso_y = 0;
                    max_iso_pos->abs_iso_x = 0;

                }


            } // if value == 1 in matrix


        }
    }

}








///
/// High level functions
///
// Tested, works
// (-+)
void TrafficManager::dumpRoadNetworks(std::string indent)
{

    std::string indent2 = indent + "   ";

    std::cout << indent << "\n\nTrafficManager's current Road Networks:\n------------------------------------\n";

    int nr = 0;





    for (std::list<RoadNetwork *>::iterator it=roadNetworks->begin(); it != roadNetworks->end(); ++it) {

        RoadNetwork *currNet = (*it);

        std::cout << "\n\n" << indent << "Road Network nr " << nr << ":\n";

        currNet->dump(indent2);

        nr++;
    }

}

/// \brief updates all the buses on all the roadnetworks with their gameUpdate() function
/// \param viewHPos Current viewing position
/// (--)
void TrafficManager::updateAll(HPos *viewHPos)
{

    int dbgLevel = 1;


    RoadNetwork *currRoadnet = nullptr;
    Bus *currBus = nullptr;

    for(std::list<RoadNetwork *>::iterator roadsIter=roadNetworks->begin(); roadsIter != roadNetworks->end(); ++roadsIter)
    {

        currRoadnet = (*roadsIter);

        for(std::list<Bus *>::iterator busIter=currRoadnet->buslist->begin(); busIter != currRoadnet->buslist->end(); ++busIter)
        {
            if(dbgLevel>=1) { std::cout << "Updating a bus\n";}

            currBus = (*busIter);

            // Find out which network it is on
            currBus->gameUpdate(currRoadnet);
        }
    }

}






///
/// Individual
///


void addRoadNetwork()
{
}


// (--) Make
DijkstraResult *TrafficManager::runDijkstraOnBus(int busId, Vector2f *from_iso_pos, Vector2f *to_iso_pos)
{
    std::cout << "NOT CODED YET             Given a busid , find the bus in the buslist and run dijkstra on A->B, \n";
    return nullptr;
}





/// \brief Makes plans for how all the buses should move by iterating the roadNetworks datastructures and finding all the buses.

// Testing:
//          (2018-05-13) "dijkstra_test_1.txt"                     Works!
//          (2018-05-13) "dijkstra_test_2.txt"                     Works! alpha-0.1
//
// (--+)
void TrafficManager::planForBusesOnRoadNetwork(int debugLevel, int fromRoad, int toRoad)
{
    std::string ind = "  ";
    RoadNetwork *roadnet = nullptr;
    Bus *bus = nullptr;
    SlotPath *slotpath;
    HPos *abs_iso_pos_A = nullptr;
    HPos *abs_iso_pos_B  = nullptr;

    HPos *rel_iso_pos_A = nullptr;
    HPos *rel_iso_pos_B  = nullptr;

    std::cout << "\n** planForBusesOnRoadNetwork:\n";





    /// Error check

    if(roadNetworks->size() <= 0) {
        std::cout << "note: " << cn << " there are no roadnetworks to work with.\n";
        return ;
    }



    /// Loop all the RoadNetworks

    for(std::list<RoadNetwork *>::iterator itRn = roadNetworks->begin(); itRn != roadNetworks->end(); ++itRn)
    {
        roadnet = (*itRn);

        if(roadnet == nullptr) {
            std::cout << "ERROR " << cn << " roadnet is null in planForBusesOnRoadNetwork when iterating roadNetworks\n";
            return ;
        }



        /// Loop all the RoadNetwork's Buses

        for(std::list<Bus *>::iterator itBus = roadnet->buslist->begin(); itBus != roadnet->buslist->end(); ++itBus)
        {


            bus = (*itBus);

            if(bus == nullptr) {
                std::cout << "ERROR " << cn << " one of the buses in the roadnet's buslist is nullptr.\n";
                return ;
            }





            /// Set A to B positions

            // Wishlist: Needs something sensible to go on for start and end position ...
            // Like a Bus Station


            std::cout << ind << "note: Hardcoded A to B \n";

            abs_iso_pos_A  = roadnet->getNrRoad_iso(fromRoad);

            if(abs_iso_pos_A == nullptr) {
                std::cout << "ERROR " << cn << " could not set start position\n";
                return ;
            }

            abs_iso_pos_B  = roadnet->getNrRoad_iso(toRoad);

            abs_iso_pos_B = new HPos(4,4,USE_ISO);

            if(abs_iso_pos_B == nullptr) {
                std::cout << "ERROR " << cn << " could not set end position\n";
                return ;
            }



            /// For now, make sure we have no absolute positions when entering createSlotPath()

            rel_iso_pos_A = abs_iso_pos_A->clone();
            rel_iso_pos_B = abs_iso_pos_B->clone();

            rel_iso_pos_A->abs_iso_x = 0;
            rel_iso_pos_A->abs_iso_y = 0;




            /// Run Dijkstra to generate the path

            slotpath = roadnet->createSlotPath(rel_iso_pos_A, rel_iso_pos_B, debugLevel);

            bus->setSlotPath(slotpath);



        }
    }

    std::cout << "\n\n planForBusesOnRoadNetwork done ** \n";








}

void TrafficManager::updateBusesOnRoadNetwork(int busId, int roadnetId) {
     std::cout << "NOT CODED YET            Update all the buses on a given roadnet? not sure I need this... hm..\n";
}





/// \brief adds a bus to the roadnetwork you search for
/// \param _bus Allocated bus
/// \param _roadnetId Find nr "roadnetId" of all the roadnets, good enough for alpha-0.1
/// Wishlist: better finding of a particular roadnetwork
/// (--)
void TrafficManager::addBus(Bus *_bus, int roadnetId)
{
    std::cout << "Adding bus to trafficmanager\n";

    // iterate over the roadnets and find a roadnet
    RoadNetwork *currNet = nullptr;

    int nr = 0;


    // Find the RoadNetwork
    for( std::list<RoadNetwork *>::iterator it=roadNetworks->begin(); it != roadNetworks->end(); ++it)
    {
        currNet = (*it);
        if(roadnetId == nr) {
            it=roadNetworks->end();
        }


        nr++;

    }

    if(currNet == nullptr) {
        std::cout << "ERROR " << cn << " We got no roads...\n";
        return ;
    }



    /// Finally add it to the right roadnet
    currNet->addBus(_bus);
}





























