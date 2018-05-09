#include "TrafficManager.hpp"


TrafficManager::TrafficManager()
{
    roadNetworks = new std::list<RoadNetwork *>();
    buslist = new std::list<Bus *>();
}


/// Recursive
/// Follows connected 1:s in the fullRoadMatrix and puts 1:s in newMatrix
///
/// @param fullRoadMatrix      Already allocated HurkaMatrix, A matrix with 1:s where the road sprites are, 0:s everywhere else
/// @param newMatrix           Already allocated HurkaMatrix, empty on entry. Will be filled when function returns.
/// @param curr_iso_pos        The current iso_pos for the algorithm
/// @param min_iso_pos         Already existing hpos object, will be filled on output.
/// @param visited             BinarySearchTree already allocated, empty on entry. Will be filled when function returns.
///
/// (--)
/// HPOSTEST:


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
        curr_iso_pos->dump();
    }

    if(debugLevel >= 2) {
        std::cout <<  "\n";
    }




    /// Have we been here?
    int searchId = Node::generateID(curr_iso_pos);
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


/*
// HPOSDELETE:

void TrafficManager::follow(HurkaMatrix *fullRoadMatrix, HurkaMatrix *newMatrix, Vector2f curr_iso_pos, Vector2f *min_iso_pos,  Vector2f *max_iso_pos,  BinarySearchTree *visited, int debugLevel)
{
    std::string ind = "      ";

    if(debugLevel >= 2) {
        std::cout << ind << "follow()    currpos=";
        dumpPosition(curr_iso_pos);
    }

    if(debugLevel >= 2) {
        std::cout <<  "\n";
    }




    /// Have we been here?
    int searchId = Node::generateID(curr_iso_pos);
    if(visited->findVal(searchId,0) != -1 ) {
        return ;                                                                                 // END RECURSION
    }



    /// Setup objects


    Vector2f up_iso = curr_iso_pos;
    up_iso.y -= 1;

    Vector2f right_iso = curr_iso_pos;
    right_iso.x += 1;

    Vector2f down_iso = curr_iso_pos;
    down_iso.y += 1;

    Vector2f left_iso = curr_iso_pos;
    left_iso.x -= 1;



    /// Update objects

    // Put a "1" where we are at the moment
    newMatrix->matrix[(int)curr_iso_pos.y][(int)curr_iso_pos.x] = 1;

    // Add to visited
    visited->add(searchId,debugLevel);


    // Update the limits
    if(min_iso_pos->y > curr_iso_pos.y) {
        // new minimum! Top road
        min_iso_pos->y = curr_iso_pos.y;
    }

    if(min_iso_pos->x > curr_iso_pos.x) {
        // new minimum! left road
        min_iso_pos->x = curr_iso_pos.x;

    }


    if(max_iso_pos->x < curr_iso_pos.x) {
        // new max! right road
        max_iso_pos->x = curr_iso_pos.x;
    }

    if(max_iso_pos->y < curr_iso_pos.y) {
        // new max! down road
        max_iso_pos->y = curr_iso_pos.y;
    }









    /// Go up
    if(up_iso.y >= 0) { // Not hit the wall yet?
        if(fullRoadMatrix->matrix[(int)up_iso.y][(int)up_iso.x] == 1) { // is it a road?
                if(debugLevel >= 2) { std::cout << ind << "going up\n"; }
            follow(fullRoadMatrix, newMatrix, up_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                                 // RECURSION CALL
        }
    }


    /// Go right
    if(right_iso.x <= (fullRoadMatrix->cols-1)) {
        if(fullRoadMatrix->matrix[(int)right_iso.y][(int)right_iso.x] == 1) {
                if(debugLevel >= 2) { std::cout << ind << "going right\n";}
            follow(fullRoadMatrix, newMatrix, right_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                              // RECURSION CALL
        }
    }


    /// Go down
    if(down_iso.y <= (fullRoadMatrix->rows-1)) {
        if(fullRoadMatrix->matrix[(int)down_iso.y][(int)down_iso.x] == 1) {
            if(debugLevel >= 2) { std::cout << ind  << "going down\n"; }
            follow(fullRoadMatrix, newMatrix, down_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                               // RECURSION CALL
        }
    }



    /// Go left
    if(left_iso.x >= 0) {
        if(fullRoadMatrix->matrix[(int)left_iso.y][(int)left_iso.x] == 1) {
            if(debugLevel >= 2) { std::cout << ind  << "going left\n"; }
            follow(fullRoadMatrix, newMatrix, left_iso, min_iso_pos, max_iso_pos, visited, debugLevel);                               // RECURSION CALL
        }
    }


                                                                                                // END RECURSION
}
*/




/// Calls a recursive function that walks through the matrix, following the 1:s
///
/// wishlist: Makeover! Minimilize by using a RoadNetwork *pointer instead of the individual parrts  (newMatrix, min_iso, max_iso...)
///
/// @param fullRoadMatrix  A matrix with all the roads as 1:s, anything else as 0:s
/// @param curr_iso_pos    The position of which to start following the 1:s
/// @param min_iso_pos     Minimum x and y limits
/// @param max_iso_pos     Maximum x and y limits
/// @param visited         used internally, just supply a new() one
/// @param debuglevel      Obvious
/// (--)


/// HPOSTEST:

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
    int newRows = max_iso_pos->abs_iso_y - min_iso_pos->abs_iso_y +1;  // lite osäker, +-1 felet
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






/*

HPOSDELETE:
RoadNetwork *TrafficManager::followAndAddToBST(HurkaMatrix *fullRoadMatrix,
                                               Vector2f curr_iso_pos,
                                               Vector2f *min_iso_pos,
                                               Vector2f *max_iso_pos,
                                               BinarySearchTree *visited,
                                               int debugLevel)
{
    std::string ind = "   ";

    if(debugLevel >= 2)  {
        std::cout << ind << "\n\nfollowAndAddToBST():\n";
    }



    /// Create a new matrix for the RoadNetwork

    HurkaMatrix *hmatrix = new HurkaMatrix(fullRoadMatrix->rows, fullRoadMatrix->cols);







    /// Follow the roads in the matrix (the 1:s)
    /// When it's done, hmatrix should contain the roadnetwork's matrix

    if(debugLevel >= 2)  {
        std::cout << ind << "\nfollow():\n" << ind << "{\n";
    }

    follow(fullRoadMatrix, hmatrix, curr_iso_pos, min_iso_pos, max_iso_pos, visited, debugLevel);



    if(debugLevel >= 2)  {
        std::cout << ind << "}\n\n";
    }



    /// Setup the return object
    int newRows = max_iso_pos->y - min_iso_pos->y +1;  // lite osäker, +-1 felet
    int newCols = max_iso_pos->x - min_iso_pos->x +1;

    /// We only want a matrix with the size containing roads, not the whitespace around it


    if(debugLevel >= 2)  {
        std::cout << "hmatrix now:\n";
        hmatrix->dump();
    }


    ///         int **copySubMatrix(int **srcMtx,    int srcRows,   int srcCols,   int startY,      int startX,  int height, int width)

    hmatrix->matrix = copySubMatrix(hmatrix->matrix, hmatrix->rows, hmatrix->cols, min_iso_pos->y, min_iso_pos->x, newRows,   newCols, debugLevel);
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

*/



/// Parse the 1:s in the roadmatrix, and group together them into RoadNetworks
/// After its done, it should have populated the std::list<RoadNetwork *> *roadNetworks;
///
/// (--)
///
/// HPOSTEST: seems to work
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

    //Vector2f curr_iso;    HPOSDELETE
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

            searchId = Node::generateID(curr_iso);

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



/*
HPOSDELETE:
void TrafficManager::parseCurrentRoads(HurkaMatrix *roadMatrix, int debugLevel)
{

    if(debugLevel >= 1) {
        std::cout << "\n\nparseCurrentRoads()\n----------------------------------\n";
    }

    BinarySearchTree *bst = new BinarySearchTree();
    Vector2f min_iso_pos = Vector2f();
    Vector2f max_iso_pos = Vector2f();





    int searchId = -1;

    int M_LENGTH = roadMatrix->rows;
    int N_LENGTH = roadMatrix->cols;


    if(debugLevel >=1) {
        roadMatrix->dump();
    }


    if(debugLevel >=2) {
        bst->dump();
    }

    //Vector2f curr_iso;    HPOSDELETE
    HPos *curr_iso;



    /// Start your engines!
    /// Walk the Matrix

    // Reset the variables for each roadNetwork:
    min_iso_pos.y = roadMatrix->rows;
    min_iso_pos.x = roadMatrix->cols;

    max_iso_pos.y = 0;
    max_iso_pos.x = 0;




    if(debugLevel >=1) {std::cout << "\n\nIterating all the cells:\n-------------------\n";}
    for(int y = 0; y < M_LENGTH; y++) {
        for(int x = 0; x < N_LENGTH; x++) {


            curr_iso.y = y;
            curr_iso.x = x;

            searchId = Node::generateID(curr_iso);

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


                    RoadNetwork *network = followAndAddToBST(roadMatrix, curr_iso, &min_iso_pos, &max_iso_pos, bst, debugLevel);

                    if(network == nullptr) {
                        std::cout << "ERROR " << cn << " parseCurrentRoads() failed while executing: followAndAddToBST(), got nullptr.\n";
                        std::cout << "Given roadmatrix:\n";
                        roadMatrix->dump();
                    }

                    network->min_isoYOffset = min_iso_pos.y;
                    network->min_isoXOffset = min_iso_pos.x;

                    network->max_isoYOffset = max_iso_pos.y;
                    network->max_isoXOffset = max_iso_pos.x;



                    /// Store a completed roadnetwork
                    roadNetworks->push_back(network);


                    /// Reset for the next round
                    min_iso_pos.y = roadMatrix->rows;
                    min_iso_pos.x = roadMatrix->cols;

                    max_iso_pos.y = 0;
                    max_iso_pos.x = 0;

                }


            } // if value == 1 in matrix


        }
    }

}
*/






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

void TrafficManager::updateAll(HPos *viewHPos)
{


    RoadNetwork *currRoadnet = nullptr;
    Bus *currBus = nullptr;

    for(std::list<RoadNetwork *>::iterator roadsIter=roadNetworks->begin(); roadsIter != roadNetworks->end(); ++roadsIter)
    {

        currRoadnet = (*roadsIter);

        for(std::list<Bus *>::iterator busIter=currRoadnet->buslist->begin(); busIter != currRoadnet->buslist->end(); ++busIter)
        {

            currBus = (*busIter);

            // Find out which network it is on
            currBus->gameUpdate(currRoadnet);
        }
    }

}






///
/// Individual
///
void addRoadNetwork() {}

DijkstraResult *TrafficManager::runDijkstraOnBus(int busId, Vector2f *from_iso_pos, Vector2f *to_iso_pos) {
    std::cout << "NOT CODED YET             Given a busid , find the bus in the buslist and run dijkstra on A->B, \n";
    return nullptr;
}

void TrafficManager::planForBusesOnRoadNetwork(int roadnetId){
    std::cout << "NOT CODED YET             Run dijkstra on all buses, do trafic planning? a route? should have a route class or struct\n";
}

void TrafficManager::updateBusesOnRoadNetwork(int busId, int roadnetId) {
     std::cout << "NOT CODED YET            Update all the buses on a given roadnet? not sure I need this... hm..\n";
}




// Wishlist: better finding of a particular roadnetwork
// (--) Good enough for alpha
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





    // Figure out from- and to positions

    HPos *fromPos = currNet->getRandomRoad_abs_iso(0);
    HPos *toPos   = currNet->getRandomRoad_abs_iso(5);


    _bus->set_pos_on_abs_iso(fromPos);
    _bus->set_nextPos_on_abs_iso(toPos);

/*


            // Setup current position
            HPos *fromPos = new HPos(bus_iso_pos_from.y, bus_iso_pos_from.x);

            _bus->set_pos_on_abs_iso(fromPos);



            // Setup next position

            HPos *toPos = new HPos(bus_iso_pos_to.y, bus_iso_pos_to.x);

            _bus->set_nextPos_on_abs_iso(toPos);


*/




    /// Finally add it to the right roadnet
    currNet->addBus(_bus);
}

























