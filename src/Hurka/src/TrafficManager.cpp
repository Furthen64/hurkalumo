#include "TrafficManager.hpp"


TrafficManager::TrafficManager()
{
    roadNetworks = new std::list<RoadNetwork *>();
}


// Recursive!
// Follows connected 1:s in the fullRoadMatrix and puts 1:s in newMatrix
void TrafficManager::follow(HurkaMatrix *fullRoadMatrix, HurkaMatrix *newMatrix, Vector2f curr_iso_pos, BinarySearchTree *visited)
{
    std::cout << "follow()    currpos=";
    dumpPosition(curr_iso_pos);
    std::cout << "\n";

    Vector2f down_iso = curr_iso_pos;
    down_iso.y += 1;

    // Have we been here?
    int searchId = Node::generateID(curr_iso_pos);
    if(visited->findVal(searchId,0) == true ) {
        return ;                                                                        // END RECURSION
    }


    // Put a 1 where it belongs
    newMatrix->matrix[(int)curr_iso_pos.y][(int)curr_iso_pos.x] = 1;



    // Add to visited
    visited->add(searchId,0);



    // Check up


    // Check right


    // Check down
    if(fullRoadMatrix->matrix[(int)down_iso.y][(int)down_iso.x] == 1) {
        std::cout << "going down\n";
        follow(fullRoadMatrix, newMatrix, down_iso, visited);                           // RECURSION CALL
    }

    // Check left



                                                                                        // END RECURSION
}


// Calls a recursive function that walks through the matrix, following the 1:s
RoadNetwork *TrafficManager::followAndAddToBST(HurkaMatrix *fullRoadMatrix, Vector2f curr_iso_pos, BinarySearchTree *visited)
{
    std::string ind = "   ";
    std::cout << ind << "\n\nfollowAndAddToBST():\n";


    Vector2f min_iso_pos;
    min_iso_pos.y = curr_iso_pos.y;
    min_iso_pos.x = curr_iso_pos.x; // TODO FIXME               There should be code going and looking for the least value in x and y while running "follow()"

    int searchId = Node::generateID(curr_iso_pos) ;


    HurkaMatrix *hmatrix = new HurkaMatrix(fullRoadMatrix->rows, fullRoadMatrix->cols);

    follow(fullRoadMatrix, hmatrix, curr_iso_pos, visited);


    RoadNetwork *roadNetwork = new RoadNetwork(min_iso_pos.y, min_iso_pos.x);

    roadNetwork->hMatrix = hmatrix;


    return roadNetwork;
}




// Build This!!
// TEST!
//
// Parse the 1s in the roadmatrix, and group together them into RoadNetworks
//
// After its done, it should have populated the std::list<RoadNetwork *> *roadNetworks;
//
// (--)
void TrafficManager::parseCurrentRoads(HurkaMatrix *roadMatrix, int debugLevel)
{

    if(debugLevel >= 1) {
        std::cout << "\n\nparseCurrentRoads()\n";
    }

    BinarySearchTree *bst = new BinarySearchTree();

    Node *currNode = nullptr;

    int searchId = -1;

    int M_LENGTH = roadMatrix->rows;
    int N_LENGTH = roadMatrix->cols;


    if(debugLevel >=1) {
        roadMatrix->dump();
    }


    if(debugLevel >=1) {
        bst->dumpBST();
    }

    Vector2f curr_iso;



    std::cout << "\n\nIterating all the cells:\n-------------------\n";
    for(int y = 0; y < M_LENGTH; y++) {
        for(int x = 0; x < N_LENGTH; x++) {


            curr_iso.y = y;
            curr_iso.x = x;

            searchId = Node::generateID(curr_iso);

            if(debugLevel >=1) {std::cout << "Pos(" << y << ", " << x << ") id=" << searchId << "\n";}

            if(roadMatrix->matrix[y][x] == 1) {

                // Search in bst

                if(bst->findVal(searchId,0) != -1) {

                    std::cout << "Already in BST\n";
                    // found it!    Ignore it. We found it so it's already been visited.


                } else {


                    std::cout << "Running followAndAddToBst\n";

                    // Did not find it?
                    // Start a "Follow and Add" recursion, which leads to a roadNetwork

                    RoadNetwork *network = followAndAddToBST(roadMatrix, curr_iso, bst);

                    roadNetworks->push_back(network);

                }


            } // if value == 1 in matrix


        }
    }

    std::cout << "NOT DONE !!!\n";
}


///
/// High level functions
///

void TrafficManager::dumpRoadNetworks()
{
}

// updates all the buses on all the roadnetworks
void TrafficManager::updateAll()
{
}






///
/// Individual
///
void addRoadNetwork() {}

DijkstraResult *TrafficManager::runDijkstraOnBus(int busId) {
    return nullptr;
}

void TrafficManager::planForBusesOnRoadNetwork(int roadnetId){}

void TrafficManager::updateBusesOnRoadNetwork(int busId, int roadnetId) {}
