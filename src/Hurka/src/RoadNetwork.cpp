#include "RoadNetwork.hpp"

RoadNetwork::RoadNetwork()
{
    buslist = new std::list<Bus *>();
}

// (++)
void RoadNetwork::dump(std::string indent)
{
    std::cout << indent << "roadnet: \n";
    std::cout << indent << "{\n";
    hMatrix->dump(indent);
    std::cout << indent << "min_iso_y_offset=" << min_isoYOffset <<"\n";
    std::cout << indent << "min_iso_x_offset=" << min_isoXOffset<< "\n";
    std::cout << indent << "max_iso_y_offset=" << max_isoYOffset <<"\n";
    std::cout << indent << "max_iso_x_offset=" << max_isoXOffset << "\n\n";
    std::cout << indent << "}\n";
}



/// (-+)
void RoadNetwork::addBus(Bus *_bus)
{
    buslist->push_back(_bus);
}




/// \brief Get a random road in this roadnetwork, sets both abs_iso and rel_iso
/// \param findNr FOR NOW the function takes the 5th road if you give findNr=5 for example
/// \return Position of the road or (-1,-1) when not found a road
/// (-+)
HPos *RoadNetwork::getNrRoad_iso(int findNr)
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
        std::cout << "ERROR " << cn << " getNrRoad_iso could not find a road for findNr=" << findNr << " out of = " << nr << "!\n";
        return nullptr;
    }

    return iso_pos;
}



/// This is used by RoadNet to get a Graph in order to Dijkstra a Route
///
/// \brief given a matrix with roads, walk the 1:s inside the matrix and connect everything accordingly in a Graph object
/// \param roadMatrix A HurkaMatrix object, allocated, has values, any size.
/// \param graph Graph object, allocated, empty.
/// \param currNode First node in graph
/// \param prevNode Set as nullptr (internal)
/// \param curr_iso_pos The first position, must be where a road is!
/// \param curr_iso_pos Set as nullptr (internal)
/// \param graph The return object, a Graph fully connected where the roads are
/// \param visited Allocated, empty BinarySearchTree.
/// Dont forget the "FULL DUPLEX" issue... that you'd have to connect A to B and B to A
/// (--) Tested Once and worked with simple road, needs far more testing
/// RECURSIVE

void RoadNetwork::createGraphFromHMatrix(HurkaMatrix *roadMatrix,
                                         Graph *graph,
                                         Node *currNode,
                                         Node *prevNode,
                                         HPos *curr_iso_pos,
                                         HPos *prev_iso_pos,
                                         BinarySearchTree *visited,
                                         int dbgLevel)
{

    // This is a Copy from TrafficManager's "follow()" function




    std::string ind = "      ";

    if(dbgLevel >= 1) {
            std::cout << ind << "\n\n ** createGraphFromHMatrix():\n" << ind << "{\n";
            std::cout << ind << "curr_iso_pos:\n";
            curr_iso_pos->dump(ind);
            std::cout << "\n";
    }

    /// Have we been here?
    int searchId = Node::generateID(curr_iso_pos);
    if(visited->findVal(searchId,0) != -1 ) {
            std::cout << ind << "been here before.\n";
        return ;                                                                                 // END RECURSION
    }



    Node *workNode = nullptr;
    Node *workNode2 = nullptr;


    /// Setup objects


    HPos  *up_iso = curr_iso_pos->clone();
    up_iso->abs_iso_y -= 1;

    HPos *right_iso = curr_iso_pos->clone();
    right_iso->abs_iso_x += 1;

    HPos *down_iso = curr_iso_pos->clone();
    down_iso->abs_iso_y += 1;

    HPos *left_iso = curr_iso_pos->clone();
    left_iso->abs_iso_x -= 1;




    /// Update objects


    // Add to visited
    visited->add(searchId,dbgLevel);


    /*
    createGraphFromHMatrix parameters:

                                 HurkaMatrix *roadMatrix,
                                 Graph *graph,
                                 Node *currNode,
                                 Node *prevNode,
                                 HPos *curr_iso_pos,
                                 HPos *prev_iso_pos,
                                 BinarySearchTree *visited,
                                 int dbgLevel)
    */



    /// Go up

    if(up_iso->abs_iso_y >= 0) { // Not hit the wall yet?


        if(roadMatrix->matrix[(int)up_iso->abs_iso_y][(int)up_iso->abs_iso_x] == 1) { // is it a road?

            if(dbgLevel >= 2) { std::cout << ind << "going up\n"; }

            // create a new node, connect it to the top of current node
            searchId = Node::generateID(up_iso);
            workNode = currNode;

            workNode2 = workNode->attachNewNode(Node::iso_to_str(up_iso),
                                                searchId,
                                                up_iso,
                                                1,
                                                1,
                                                0);

            createGraphFromHMatrix(roadMatrix,
                                   graph,
                                   workNode2,
                                   workNode,
                                   up_iso,
                                   curr_iso_pos,
                                   visited,
                                   dbgLevel);                                     // RECURSION CALL

        }
    }


    /// Go right
    if(right_iso->abs_iso_x <= (roadMatrix->cols-1)) {
        if(roadMatrix->matrix[(int)right_iso->abs_iso_y][(int)right_iso->abs_iso_x] == 1) {

            if(dbgLevel >= 2) { std::cout << ind << "going right\n";}

            // create a new node, connect it to the right of current node
            searchId = Node::generateID(right_iso);
            workNode = currNode;

            workNode2 = workNode->attachNewNode(Node::iso_to_str(right_iso),
                                                searchId,
                                                right_iso,
                                                1,
                                                1,
                                                0);


            createGraphFromHMatrix(roadMatrix,
                                   graph,
                                   workNode2,
                                   workNode,
                                   right_iso,
                                   curr_iso_pos,
                                   visited,
                                   dbgLevel);                                  // RECURSION CALL
        }
    }




    /// Go down
    if(down_iso->abs_iso_y <= (roadMatrix->rows-1)) {
        if(roadMatrix->matrix[(int)down_iso->abs_iso_y][(int)down_iso->abs_iso_x] == 1) {

            if(dbgLevel >= 2) { std::cout << ind  << "going down\n"; }


            searchId = Node::generateID(down_iso);
            workNode = currNode;


            workNode2 = workNode->attachNewNode(
                                                Node::iso_to_str(down_iso),
                                                searchId,
                                                down_iso,
                                                1,
                                                1,
                                                0);

            createGraphFromHMatrix(roadMatrix,
                                   graph,
                                   workNode2,
                                   workNode,
                                   down_iso,
                                   curr_iso_pos,
                                   visited,
                                   dbgLevel);                                  // RECURSION CALL

        }
    }



    /// Go left
    if(left_iso->abs_iso_x >= 0) {
        if(roadMatrix->matrix[(int)left_iso->abs_iso_y][(int)left_iso->abs_iso_x] == 1) {

            if(dbgLevel >= 2) { std::cout << ind  << "going left\n"; }

            searchId = Node::generateID(left_iso);
            workNode = currNode;

            workNode2 = workNode->attachNewNode(
                                                Node::iso_to_str(left_iso),
                                                searchId,
                                                left_iso,
                                                1,
                                                1,
                                                0);

            createGraphFromHMatrix(roadMatrix,
                                   graph,
                                   workNode2,
                                   workNode,
                                   left_iso,
                                   curr_iso_pos,
                                   visited,
                                   dbgLevel);                                  // RECURSION CALL

        }
    }




    if(dbgLevel >=1 ) {
            std::cout << ind << "}\n";
    }



}






/// \brief Used by highlevel function createSlotPath()
/// \param dijkstraResult A result from already executed Dijkstra
/// \param slotpath Allocated, empty.
/// TODO: This one consumes the result... can only be run once
/// (--) TEST
void RoadNetwork::createSlotPathFromDijkstraResult(DijkstraResult *dijkstraResult, SlotPath *slotpath)
{


    std::cout << "FIXME; CONSUMES THE RESULT\n";



    int dbgLevel = 1;

    if(dbgLevel >=1) { std::cout << "** createSlotPathFromDijkstraResult:\n"; }
    if(dijkstraResult == nullptr) {
        std::cout << cn << " ERROR dijkstraResult has nullptr in printPathFromDijkstra()\n";
        return ;
    }

    Node *workNode;
    HPos *workPos;
    SlotPos *workSlotpos;

    std::string str;

    // Go over the result, create slotpositions for every node

    while( ! (dijkstraResult->shortestPath.empty()) )
    {


        workNode = dijkstraResult->shortestPath.top();

        // Get the iso_pos
        workPos = workNode->getCopyOfIsoPos();

        // Store away the old ones as relative positions
        workPos->rel_iso_y = workPos->abs_iso_y;
        workPos->rel_iso_x = workPos->abs_iso_x;

        // Now adjust to full gamematrix by using roadnetworks offsets
        workPos->abs_iso_y += min_isoYOffset;
        workPos->abs_iso_x += min_isoXOffset;

        workPos->gpix_y = Grid::convert_iso_to_gpix_y(workPos->abs_iso_y, workPos->abs_iso_x, 64, 32, 2);   // rendered as a GRID
        workPos->gpix_x = Grid::convert_iso_to_gpix_x(workPos->abs_iso_y, workPos->abs_iso_x, 64, 32, 2);   // rendered as a GRID




        if(dbgLevel >=1) {
            workPos->dump("   ");
        }


        // Slot Pos
        workSlotpos = new SlotPos(workPos);

        workPos->transform_gpix_to_slotpos(workSlotpos, workPos);    // Make sure its in the middle of the block for now

        slotpath->add(workSlotpos);

        dijkstraResult->shortestPath.pop();

    }


    if(dbgLevel >=1) { std::cout << " createSlotPathFromDijkstraResult DONE ** "; }
}






/// \brief Used by TrafficManager and Bus to create the Slotpath from A to B for that bus
/// \brief Includes the use of Dijkstra algorithm
/// \param fromPos abs_iso_pos given
/// \param toPos abs_iso_pos given
/// (--) TEST!
/// BUG: Undefined Behaviour - sometimes it crashes. Behaves weirdly. Something is off.
///     Happened when start_iso_pos was randomiszed everytime
/// Check ALL the pointers
/// Check for nullptr returns
/// Simplify

SlotPath *RoadNetwork::createSlotPath(HPos *fromPos, HPos *toPos, int debugLevel )
{

    SlotPath *slotpath = new SlotPath();
    std::string ind = "  ";



    /// Error handling

    if(fromPos == nullptr) {
        std::cout << "ERROR " << cn << " fromPos is null in createSlotPath\n";
        return nullptr;
    }

    if(toPos == nullptr) {
        std::cout << "ERROR " << cn << " toPos is null in createSlotPath\n";
        return nullptr;
    }

    std::cout << ind << "\n\n** createSlotPath \n";







    ///
    /// Make a Graph out of the RoadNetwork     (wishlist: make this as own function)
    ///

    Graph *graph = new Graph("road_network_1");
    DijkstraResult *dijkstraResult = nullptr;

    int searchId = -1;
    HPos *start_abs_iso_pos = nullptr;






    /// Create the first node in the graph (find any road to start with)

    start_abs_iso_pos = fromPos->clone();   // for now use the fromPos

    searchId = Node::generateID(start_abs_iso_pos);

    // add the first road to the graph , special!
    graph->addFirstNode(Node::iso_to_str(start_abs_iso_pos), searchId, start_abs_iso_pos);

    // find the first road in the graph so we can connect more roads to it
    Node *currNode = graph->findNode(searchId, 0);
    if(currNode == nullptr) { std::cout << "ERROR " << cn << " with createSlotPath()\n"; return nullptr; }








    // setup some objects...
    BinarySearchTree *visited = new BinarySearchTree();

    // ... so we can recursively walk the roadnetwork to get the coordinates for each road
    hMatrix->dump("  ");
    createGraphFromHMatrix(hMatrix, graph, currNode, nullptr, start_abs_iso_pos, nullptr, visited, 2);

    if(debugLevel >=1 ) {
        graph->dump(0, 1);
        std::cout << "\n\n";
    }

    std::cout << "\n\n" << ind << "NOW that we have a GRAPH do a DIJKSTRA run and see what we end up with \n";












    ///
    /// run Dijkstra
    ///

    std::cout << ind << "\n\nRunning Dijkstra 1st time:\n------------------------------\n";


    // Setup start and end positions for Dijkstra

    Node *startNode = graph->findNode( Node::genIDfrom_rel_iso(fromPos), 0);
    Node *endNode   = graph->findNode( Node::genIDfrom_rel_iso(toPos), 1);


    std::cout << "\n";
    std::cout << ind << "FROM= \n";
    startNode->dump(3);
    std::cout << "\n";

    std::cout << ind << "TO= \n";
    endNode->dump(3);

    if(startNode == nullptr || endNode == nullptr) {
        std::cout << "ERROR! Could not find the start or end node for Dijkstra\n";
        return nullptr;
    }

    std::cout << ind << "  * startNode.id=" << startNode->getId() << "\n";
    std::cout << ind << "  * endNode.id= " << endNode->getId() << "\n";


    std::cout << ind << "\n\nrunDijkstra()\n";
    std::cout << ind << "{\n";
    dijkstraResult = graph->runDijkstra(startNode, endNode, 2);
    std::cout << ind << "}\n";


    if(dijkstraResult->shortestPath.empty()) {
        return nullptr;
    }


    std::cout << ind << " Loop the result and put every in a slotpos\n";
    std::cout << ind << " Put those slotpos in slotpath\n\n";

    createSlotPathFromDijkstraResult(dijkstraResult, slotpath);


    std::cout << "\n\n";

    slotpath->dump();


    std::cout << ind << "\ncreateSlotPath done ** \n";

     return slotpath;
}



