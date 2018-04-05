#include "TrafficManager.hpp"


TrafficManager::TrafficManager()
{
    roadNetworks = new std::list<RoadNetwork *>();
}


// Recursive!
// Follows connected 1:s in the fullRoadMatrix and puts 1:s in newMatrix
void TrafficManager::follow(HurkaMatrix *fullRoadMatrix, HurkaMatrix *newMatrix, Vector2f curr_iso_pos, BinarySearchTree *visited, int debugLevel)
{
    std::string ind = "      ";

    if(debugLevel >= 2) {
        std::cout << ind << "follow()    currpos=";
        dumpPosition(curr_iso_pos);
    }

    if(debugLevel >= 2) {
        std::cout <<  "\n";
    }





    Vector2f up_iso = curr_iso_pos;
    up_iso.y -= 1;

    Vector2f right_iso = curr_iso_pos;
    right_iso.x += 1;

    Vector2f down_iso = curr_iso_pos;
    down_iso.y += 1;

    Vector2f left_iso = curr_iso_pos;
    left_iso.x -= 1;


    // Have we been here?
    int searchId = Node::generateID(curr_iso_pos);
    if(visited->findVal(searchId,0) != -1 ) {
        return ;                                                                                 // END RECURSION
    }


    // Put a "1" where we are at the moment
    newMatrix->matrix[(int)curr_iso_pos.y][(int)curr_iso_pos.x] = 1;



    // Add to visited
    visited->add(searchId,debugLevel);







    // Check up
    if(up_iso.y >= 0) { // Not hit the wall yet?
        if(fullRoadMatrix->matrix[(int)up_iso.y][(int)up_iso.x] == 1) { // is it a road?
                if(debugLevel >= 2) { std::cout << ind << "going up\n"; }
            follow(fullRoadMatrix, newMatrix, up_iso, visited, debugLevel);                                 // RECURSION CALL
        }
    }


    // Check right
    if(right_iso.x <= (fullRoadMatrix->cols-1)) {
        if(fullRoadMatrix->matrix[(int)right_iso.y][(int)right_iso.x] == 1) {
                if(debugLevel >= 2) { std::cout << ind << "going right\n";}
            follow(fullRoadMatrix, newMatrix, right_iso, visited, debugLevel);                              // RECURSION CALL
        }
    }


    // Check down
    if(down_iso.y <= (fullRoadMatrix->rows-1)) {
        if(fullRoadMatrix->matrix[(int)down_iso.y][(int)down_iso.x] == 1) {
            if(debugLevel >= 2) { std::cout << ind  << "going down\n"; }
            follow(fullRoadMatrix, newMatrix, down_iso, visited, debugLevel);                               // RECURSION CALL
        }
    }



    // Check left
    if(left_iso.x >= 0) {
        if(fullRoadMatrix->matrix[(int)left_iso.y][(int)left_iso.x] == 1) {
            if(debugLevel >= 2) { std::cout << ind  << "going left\n"; }
            follow(fullRoadMatrix, newMatrix, left_iso, visited, debugLevel);                               // RECURSION CALL
        }
    }


                                                                                                // END RECURSION
}


// Calls a recursive function that walks through the matrix, following the 1:s
RoadNetwork *TrafficManager::followAndAddToBST(HurkaMatrix *fullRoadMatrix, Vector2f curr_iso_pos, BinarySearchTree *visited, int debugLevel)
{
    std::string ind = "   ";

    if(debugLevel >= 2)  {
        std::cout << ind << "\n\nfollowAndAddToBST():\n";
    }


    Vector2f min_iso_pos;
    min_iso_pos.y = curr_iso_pos.y;
    min_iso_pos.x = curr_iso_pos.x; // TODO FIXME               There should be code going and looking for the least value in x and y while running "follow()"


    //int searchId = Node::generateID(curr_iso_pos) ;


    HurkaMatrix *hmatrix = new HurkaMatrix(fullRoadMatrix->rows, fullRoadMatrix->cols);


    // Follow the roads in the matrix (the 1:s)
    // When it's done, hmatrix should contain the roadnetwork's matrix
    if(debugLevel >= 2)  {
        std::cout << ind << "\nfollow():\n" << ind << "{\n";
    }
    follow(fullRoadMatrix, hmatrix, curr_iso_pos, visited, debugLevel);

    if(debugLevel >= 2)  {
        std::cout << ind << "}\n\n";
    }



    // Setup the return object
    RoadNetwork *roadNetwork = new RoadNetwork(min_iso_pos.y, min_iso_pos.x);
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

                    if(debugLevel >=1) {std::cout << "Already in BST\n";}
                    // found it!    Ignore it. We found it so it's already been visited.


                } else {


                    if(debugLevel >=1) {std::cout << "Running followAndAddToBst\n";}

                    // Did not find it?
                    // Start a "Follow and Add" recursion, which returns a roadNetwork



                    RoadNetwork *network = followAndAddToBST(roadMatrix, curr_iso, bst, debugLevel);

                    roadNetworks->push_back(network);

                }


            } // if value == 1 in matrix


        }
    }

}


///
/// High level functions
///

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


























