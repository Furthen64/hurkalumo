#include "TrafficManager.hpp"


TrafficManager::TrafficManager()
{
    roadNetworks = new std::list<RoadNetwork *>();
}


// Recursive
// Follows connected 1:s in the fullRoadMatrix and puts 1:s in newMatrix
//
// fullRoadMatrix:      Already allocated HurkaMatrix, A matrix with 1:s where the road sprites are, 0:s everywhere else
// newMatrix:           Already allocated HurkaMatrix, empty on entry. Will be filled when function returns.
// curr_iso_pos:        The current iso_pos for the algorithm
// min_iso_pos:         Already existing vector2f object, will be filled on output.
// visited:             BinarySearchTree already allocated, empty on entry. Will be filled when function returns.
//
// Improvements: Make curr_iso_pos a pointer

// (-+)
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


// Calls a recursive function that walks through the matrix, following the 1:s
//
// wishlist: Makeover! Minimilize by using a RoadNetwork *pointer instead of the individual parrts  (newMatrix, min_iso, max_iso...)
// (-+)
RoadNetwork *TrafficManager::followAndAddToBST(HurkaMatrix *fullRoadMatrix, Vector2f curr_iso_pos, Vector2f *min_iso_pos, Vector2f *max_iso_pos, BinarySearchTree *visited, int debugLevel)
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





// Parse the 1s in the roadmatrix, and group together them into RoadNetworks
//
// After its done, it should have populated the std::list<RoadNetwork *> *roadNetworks;
//
// (-+)         Works! I have only tested simple things though...

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

    Vector2f curr_iso;



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

// updates all the buses on all the roadnetworks
void TrafficManager::updateAll()
{
    std::cout << "NOT CODED YET             Should iterate the buses, iterate the roadnetworks, perform stuff! do next_iso_pos and next_pix_pos on buses\n";
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


























