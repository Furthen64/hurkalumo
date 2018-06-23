#include "RoadNetwork.hpp"

RoadNetwork::RoadNetwork()
{
    buslist = new std::list<Bus *>();
}



// Not sure how to do this yet
void RoadNetwork::clearResources()
{
    /*
    Bus *workBus;

    while(buslist->size() >0) {

        workBus = buslist->back();
        delete workBus;
        buslist->pop_back();    // FIXME does this one DO a delete of my object?


    }

    delete buslist;
    */
}




// (-+)
void RoadNetwork::dump(std::string indent)
{

    std::cout << "\n\n" << indent << "----------------- roadnetwork (" << min_isoYOffset << ", " << min_isoXOffset << " ------------------\n";

    std::cout << indent << "absolute iso position = " << min_isoYOffset<< ", " << min_isoXOffset<< "\n";
    std::cout << indent << "height = " << max_isoYOffset << "\n";
    std::cout << indent << "width = " << max_isoXOffset << "\n";
    std::cout << indent << "nr of buses = " << buslist->size() << "\n";
    std::cout << indent << "matrix:\n";

    std::string indent2 = indent;
    indent2 += "   ";
    hMatrix->dump(indent2);
    std::cout << "\n";


}



/// (-+)
void RoadNetwork::addBus(Bus *_bus)
{
    buslist->push_back(_bus);
}








/// \brief Searches for a Bus in this roadnetwork at the given position
// (--) test
Bus *RoadNetwork::busAtPos(HPos *searchPos)
{


    Bus *workbus;
    for(std::list<Bus *>::iterator it = buslist->begin(); it != buslist->end(); ++it)
    {

        workbus = (*it);

        if( workbus->atPos(searchPos) ) {
            return workbus;
        }

    }

    return nullptr;

}




/// \brief Get a random road in this roadnetwork, sets both abs_iso and rel_iso
/// \param findNr FOR NOW the function takes the 5th road if you give findNr=5 for example
/// \return Position of the road or (-1,-1) when not found a road
/// (-+)
HPos *RoadNetwork::getNrRoad_iso(int findNr, int debugLevel)
{

    HPos *iso_pos = new HPos(-1,-1, USE_ISO);

    // Figure out the limits of the road matrix
    int YMAX = max_isoYOffset - min_isoYOffset;
    int XMAX = max_isoXOffset - min_isoXOffset;

    int nr = 0;

    // Loop between the limits and find first road we can see
    for(int y = 0; y <= YMAX; y++) {
        for(int x = 0; x <= XMAX; x++) {

            if(hMatrix->matrix[y][x] == 1)
            {

                // Found one!
                // Set iso positions for return object

                iso_pos->abs_iso_y = min_isoYOffset + y;
                iso_pos->abs_iso_x = min_isoXOffset + x;

                iso_pos->rel_iso_y = y;
                iso_pos->rel_iso_x = x;

                if(nr == findNr) {
                    return iso_pos;
                } else {
                    nr++;
                }
            }

        }
    }

    if(nr < findNr) {
        if(debugLevel >=1) {
            std::cout << "ERROR " << cn << " getNrRoad_iso could not find a road for findNr=" << findNr << " out of = " << nr << "!\n";
        }
        return nullptr;
    }

    return iso_pos;
}



/// \brief Used by TrafficManager and Bus to create the Slotpath from A to B for that bus
/// \brief Includes the use of Dijkstra algorithm
/// \param from_rel_iso_pos relative iso positions given (will be modified!)
/// \param to_rel_iso_pos relative iso positions given (will be modified!)
// (-+)

SlotPath *RoadNetwork::createSlotPath(HPos *from_rel_iso_pos, HPos *to_rel_iso_pos, int debugLevel )
{
    SlotPath *slotpath = new SlotPath();
    std::string ind = "  ";

    /// Error handling

    if(from_rel_iso_pos == nullptr) {
        std::cout << "ERROR " << cn << " from_rel_iso_pos is null in createSlotPath\n";
        return nullptr;
    }

    if(to_rel_iso_pos == nullptr) {
        std::cout << "ERROR " << cn << " to_rel_iso_pos is null in createSlotPath\n";
        return nullptr;
    }


    if(debugLevel >=2) {         std::cout << "\n**createSlotPath()\n{\n";     }

    if(debugLevel >=2) {
        std::cout << ind << "roadnetwork-maxY and maxX= (" << max_isoYOffset << ", " << max_isoXOffset<<")\n";
        std::cout << ind << "parameter from_rel_iso_pos:\n";
        from_rel_iso_pos->dump(ind);
        std::cout << ind << "parameter to_rel_iso_pos:\n";
        to_rel_iso_pos->dump(ind);
    }





    ///
    /// G R A P H
    ///

    // Make a graph out of the roadnetwork



    Graph *graph = new Graph("road_network_1", max_isoYOffset + 1, max_isoXOffset+ 1);
    DijkstraResult *dijkstraResult = nullptr;

    int searchId = -1;



    /// Create the first node in the graph (find any road to start with)
    searchId = Node::genIDfrom_rel_iso(from_rel_iso_pos);

    // add the first road to the graph , special!
    graph->addFirstNode(Node::rel_iso_to_str(from_rel_iso_pos), searchId, from_rel_iso_pos);

    // find the first road in the graph so we can connect more roads to it
    Node *currNode = graph->findNode(searchId, 0);
    if(currNode == nullptr) { std::cout << "ERROR " << cn << " with createSlotPath()\n"; return nullptr; }






    // setup some objects...
    BinarySearchTree *visited = new BinarySearchTree();

    // ... so we can recursively walk the roadnetwork to get the coordinates for each road
    if(debugLevel >=2) {
        hMatrix->dump("  ");
    }


    /// Create a Graph

    createGraphFromHMatrix(hMatrix, graph, currNode, nullptr, from_rel_iso_pos->clone(), nullptr, visited, debugLevel - 1 );

    if(debugLevel >=2 ) {
        graph->dump(0, 1);
        std::cout << "\n\n";

    }

    if(debugLevel >=2) {
       std::cout << "\n\n" << ind << "-----------\n" << ind << "NOW that we have a GRAPH do a DIJKSTRA run and see what we end up with \n";
    }












    ///
    /// D I J K S T R A
    ///







    /// Setup start and end positions

    if(debugLevel >=2) {
        std::cout << ind << "\n\nSetup start and end for Dijkstra :\n" << ind << "------------------------------\n";
    }

    Node *startNode = graph->findNode( Node::genIDfrom_rel_iso(from_rel_iso_pos), 0);
    Node *endNode   = graph->findNode( Node::genIDfrom_rel_iso(to_rel_iso_pos), 0);

    if(debugLevel >=2) {
        std::cout << "\n" << ind << "START= \n";
        startNode->dump(3);
        std::cout << ind << "  * startNode.id=" << startNode->getId() << "\n";


        std::cout << "\n\n" << ind << "END= \n";
        endNode->dump(3);
        std::cout << ind << "  * endNode.id= " << endNode->getId() << "\n";
    }

    if(startNode == nullptr || endNode == nullptr) {
        std::cout << "ERROR! Could not find the start or end node for Dijkstra\n";
        return nullptr;
    }







    if(debugLevel >=2) {
        std::cout << ind << "\n\nRunning Dijkstra :\n" << ind << "------------------------------\n";
    }


    /// Do the Dijkstra!


    dijkstraResult = graph->runDijkstra(startNode, endNode, debugLevel - 1);

    if(debugLevel >=2) {
        std::cout << ind << "}\n";
    }




    /// Handle the Result

    if(dijkstraResult->isEmpty()) {
        return nullptr;
    }
    if(debugLevel >=2) {
        std::cout << ind << " Loop the result and put every in a slotpos\n";
        std::cout << ind << " Put those slotpos in slotpath\n\n";
    }


    /// Create the SlotPath
    createSlotPathFromDijkstraResult(dijkstraResult, slotpath, debugLevel--);










    if(debugLevel >=1) { std::cout << "\n\n"; }
    if(debugLevel >=2) {
        slotpath->dump();
    }


    if(debugLevel >=1) {std::cout << ind << "\ncreateSlotPath done ** \n"; }

    return slotpath;
}



/// This is used by RoadNet to get a Graph in order to Dijkstra a Route
///
/// \brief given a matrix with roads, walk the 1:s inside the matrix and connect everything accordingly in a Graph object
/// \param roadMatrix A HurkaMatrix object, allocated, has values, any size.
/// \param graph Allocated, empty. graph will be the return object, a Graph fully connected where the roads are
/// \param currNode First node in graph
/// \param prevNode Set as nullptr (internal)
/// \param curr_rel_iso_pos The first position, must be where a road is. Relative positioning. can be modified!
/// \param prev_rel_iso_pos set as nullptr.
/// \param visited Allocated, empty BinarySearchTree.
/// \return see Graph



//
// Bug  2018-05-13  It only runs attachNewNode , so when an old node is found
//                  a new node is created. Like in dijkstra_test_1.txt where
//                  the road loops in on itself                                 SOLVED!

// Test 2018-05-16  I followed quite complex graph that
//                  was created and it all looked fine to me.                   WORKS!
// RECURSIVE
// (-+)
void RoadNetwork::createGraphFromHMatrix(HurkaMatrix *roadMatrix,
                                         Graph *graph,
                                         Node *currNode,
                                         Node *prevNode,
                                         HPos *curr_rel_iso_pos,
                                         HPos *prev_rel_iso_pos,
                                         BinarySearchTree *visited,
                                         int dbgLevel)
{

    // This is a Copy from TrafficManager's "follow()" function
    // Maybe that also suffers this bug CR5 2018-05-13?




    std::string ind = "      ";

    if(dbgLevel >= 1) {
            std::cout << ind << "-----------------------------------------\n";
            std::cout << ind << curr_rel_iso_pos->relToString() << " entering!\n";
    }

    /// Have we been here?
    int searchId = Node::genIDfrom_rel_iso(curr_rel_iso_pos);

    if(visited->findVal(searchId,0) != -1 ) {
        if(dbgLevel >=1) { std::cout << ind << "Been here before, breaking.\n\n"; }
        return ;                                                                                 // END RECURSION
    }



    Node *workNode = nullptr;
    Node *workNode2 = nullptr;


    /// Setup objects


    HPos  *up_iso = curr_rel_iso_pos->clone();
    up_iso->rel_iso_y -= 1;

    up_iso->rel_iso_y = up_iso->rel_iso_y;
    up_iso->rel_iso_x = up_iso->rel_iso_x;



    HPos *right_iso = curr_rel_iso_pos->clone();
    right_iso->rel_iso_x += 1;

    right_iso->rel_iso_y = right_iso->rel_iso_y;
    right_iso->rel_iso_x = right_iso->rel_iso_x;




    HPos *down_iso = curr_rel_iso_pos->clone();
    down_iso->rel_iso_y += 1;

    down_iso->rel_iso_y = down_iso->rel_iso_y;
    down_iso->rel_iso_x = down_iso->rel_iso_x;



    HPos *left_iso = curr_rel_iso_pos->clone();
    left_iso->rel_iso_x -= 1;

    left_iso->rel_iso_y = left_iso->rel_iso_y;
    left_iso->rel_iso_x = left_iso->rel_iso_x;




    /// Update objects


    // Add to visited
    visited->add(searchId,0);


    /*
    createGraphFromHMatrix parameters:

                                        (HurkaMatrix *roadMatrix,
                                         Graph *graph,
                                         Node *currNode,
                                         Node *prevNode,
                                         HPos *curr_rel_iso_pos,
                                         HPos *prev_rel_iso_pos,
                                         BinarySearchTree *visited,
                                         int dbgLevel)
    */



    /// Go up

    if(up_iso->rel_iso_y >= 0) { // Not hit the wall yet?
        if(roadMatrix->matrix[(int)up_iso->rel_iso_y][(int)up_iso->rel_iso_x] == 1) { // is it a road?

            if(dbgLevel >= 2) { std::cout << ind << curr_rel_iso_pos->relToString() << " is going up\n"; }


            // check if we already have this node Up in the graph
            searchId = Node::genIDfrom_rel_iso(up_iso);

            workNode = currNode;



            if(visited->findVal(searchId,0) != -1 ) {

                // YES - run attachNode

                // Find it in the graph:

                workNode2 = graph->findNode(searchId, 0);

                if(workNode2 == nullptr) {
                    std::cout << "ERROR " << cn << " should not be possible, createGraphFromHMatrix fail to find a node that it was able to locate just a couple of steps ago.. weird\n";
                    return ;
                }

                workNode->attachNodeUp(workNode2);


            } else {

                // NO - we have to run attachNewNode

                workNode2 = workNode->attachNewNode(Node::rel_iso_to_str(up_iso),
                                                    searchId,
                                                    up_iso,
                                                    1,
                                                    1,
                                                    0);

            }



            // TEST... also not sure if we run attachNodeUp if we need to run this createGraph recursion call again because we are already there in the stack?
            // Now move along to the next one
            createGraphFromHMatrix(roadMatrix,
                                       graph,
                                       workNode2,       /* currNode */
                                       workNode,
                                       up_iso,
                                       curr_rel_iso_pos,
                                       visited,
                                       dbgLevel);                                                                // RECURSION CALL


        }
    }


    /// Go right
    if(right_iso->rel_iso_x <= (roadMatrix->cols-1)) {
        if(roadMatrix->matrix[(int)right_iso->rel_iso_y][(int)right_iso->rel_iso_x] == 1) {


            if(dbgLevel >= 2) { std::cout << ind << curr_rel_iso_pos->relToString() << " is going right\n"; }

            searchId = Node::genIDfrom_rel_iso(right_iso);

            workNode = currNode;

            if(visited->findVal(searchId,0) != -1 ) {

                // YES - run attachNode

                workNode2 = graph->findNode(searchId, 0);

                if(workNode2 == nullptr) {
                    std::cout << "ERROR " << cn << " should not be possible, createGraphFromHMatrix fail to find a node that it was able to locate just a couple of steps ago.. weird\n";
                    return ;
                }

                workNode->attachNodeRight(workNode2);


            } else {


                // NO - we have to run attachNewNode

                workNode2 = workNode->attachNewNode(Node::rel_iso_to_str(right_iso),
                                                    searchId,
                                                    right_iso,
                                                    1,
                                                    1,
                                                    0);
            }




            createGraphFromHMatrix(roadMatrix,
                                   graph,
                                   workNode2,   /* currNode*/
                                   workNode,
                                   right_iso,
                                   curr_rel_iso_pos,
                                   visited,
                                   dbgLevel);                                                     // RECURSION CALL
        }
    }




    /// Go down
    if(down_iso->rel_iso_y <= (roadMatrix->rows-1)) {
        if(roadMatrix->matrix[(int)down_iso->rel_iso_y][(int)down_iso->rel_iso_x] == 1) {


            if(dbgLevel >= 2) { std::cout << ind << curr_rel_iso_pos->relToString() << " is going down\n"; }

            searchId = Node::genIDfrom_rel_iso(down_iso);

            workNode = currNode;


            if(visited->findVal(searchId,0) != -1 ) {

                // YES - run attachNode

                workNode2 = graph->findNode(searchId, 0);


                if(workNode2 == nullptr) {
                    std::cout << "ERROR " << cn << " should not be possible, createGraphFromHMatrix fail to find a node that it was able to locate just a couple of steps ago.. weird\n";
                    return ;
                }

                workNode->attachNodeDown(workNode2);


            } else {

                // NO - we have to run attachNewNode

                workNode2 = workNode->attachNewNode(
                                                Node::rel_iso_to_str(down_iso),
                                                searchId,
                                                down_iso,
                                                1,
                                                1,
                                                0);
            }



            createGraphFromHMatrix(roadMatrix,
                                   graph,
                                   workNode2,
                                   workNode,
                                   down_iso,
                                   curr_rel_iso_pos,
                                   visited,
                                   dbgLevel);                                  // RECURSION CALL

        }
    }



    /// Go left
    if(left_iso->rel_iso_x >= 0) {
        if(roadMatrix->matrix[(int)left_iso->rel_iso_y][(int)left_iso->rel_iso_x] == 1) {


            if(dbgLevel >= 2) { std::cout << ind << curr_rel_iso_pos->relToString() << " is going left\n"; }

            searchId = Node::genIDfrom_rel_iso(left_iso);

            workNode = currNode;

            if(visited->findVal(searchId,0) != -1 ) {

                // YES - run attachNode

                workNode2 = graph->findNode(searchId, 0);

                if(workNode2 == nullptr) {
                    std::cout << "ERROR " << cn << " should not be possible, createGraphFromHMatrix fail to find a node that it was able to locate just a couple of steps ago.. weird\n";
                    return ;
                }


                // Finally connect it , both ways
                workNode->attachNodeLeft(workNode2);


            } else {

                // NO - we have to run attachNewNode

                workNode2 = workNode->attachNewNode(
                                                Node::rel_iso_to_str(left_iso),
                                                searchId,
                                                left_iso,
                                                1,
                                                1,
                                                0);
            }

            createGraphFromHMatrix(roadMatrix,
                                   graph,
                                   workNode2,
                                   workNode,
                                   left_iso,
                                   curr_rel_iso_pos,
                                   visited,
                                   dbgLevel);                                  // RECURSION CALL

        }
    }
}













/// \brief Used by highlevel function createSlotPath()
/// \param dijkstraResult A result from already executed Dijkstra
/// \param slotpath Allocated, empty.

// Wishlist: alpha-0.2: please make it more traffic situation aware, and direction aware, when creating all those slotpositions
// (-+)
void RoadNetwork::createSlotPathFromDijkstraResult(DijkstraResult *dijkstraResult, SlotPath *slotpath, int debugLevel)
{

    if(debugLevel >=1) { std::cout << "createSlotPathFromDijkstraResult()\n{"; }

    if(dijkstraResult == nullptr) {
        std::cout << cn << " ERROR dijkstraResult has nullptr in printPathFromDijkstra()\n";
        return ;
    }

    Node *workNode;
    HPos *workPos;
    std::list<SlotPos *> *slotPositions = nullptr;
    std::string str;

    // Go over the result, create slotpositions for every node
    std::stack<Node *> *stacker = dijkstraResult->getCopyOfShortestPathStack();


    while( !stacker->empty())
    {

        workNode = stacker->top();

        // Get the iso_pos
        workPos = workNode->get_copy_rel_iso_pos();


        // Now adjust to full gamematrix by using roadnetworks offsets
        workPos->abs_iso_y = workPos->rel_iso_y + min_isoYOffset;
        workPos->abs_iso_x = workPos->rel_iso_x + min_isoXOffset;

        workPos->gpix_y = Grid::convert_iso_to_gpix_y(workPos->abs_iso_y, workPos->abs_iso_x, 64, 32, 2);   // rendered as a GRID
        workPos->gpix_x = Grid::convert_iso_to_gpix_x(workPos->abs_iso_y, workPos->abs_iso_x, 64, 32, 2);   // rendered as a GRID



        // Now we have a gpix point of reference for generating all the tiny steps the bus will take inside a road

        // alpha-0.2:  make this better
        slotPositions = SlotPath::getSlotPosesOnBlockSituation(workPos, 101, 0);


        slotpath->addListOfSlotPositions(slotPositions);


        stacker->pop();

    }


    if(debugLevel >=1) { std::cout << "\n}\n"; }
}





