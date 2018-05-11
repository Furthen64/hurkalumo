#include "RoadNetwork.hpp"

RoadNetwork::RoadNetwork()
{
    buslist = new std::list<Bus *>();
}

// (+-)
void RoadNetwork::dump(std::string indent)
{
    hMatrix->dump(indent);
    std::cout << indent << "min_iso_y_offset=" << min_isoYOffset <<"\n";
    std::cout << indent << "min_iso_x_offset=" << min_isoXOffset<< "\n";
    std::cout << indent << "max_iso_y_offset=" << max_isoYOffset <<"\n";
    std::cout << indent << "max_iso_x_offset=" << max_isoXOffset << "\n\n";
}



/// (-+)
void RoadNetwork::addBus(Bus *_bus)
{
    buslist->push_back(_bus);
}




/// \brief Get a random road in this roadnetwork, sets both abs_iso and rel_iso
/// \param findNr FOR NOW the function takes the 5th road if you give findNr=5 for example
/// \return Position of the road or (-1,-1) when not found a road
///
/// Wishlist: Actually do RANDOM positioning, right now it only takes the first road it finds or after "nr" of finds
/// (-+)
HPos *RoadNetwork::getRandomRoad_iso(int findNr)
{

    HPos *iso_pos = new HPos(-1,-1, USE_ISO);

    int YMAX = max_isoYOffset - min_isoYOffset;     // Figure out the limits of the road matrix
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
/// (--) TEST!
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

    if(dbgLevel >= 1) { std::cout << ind << "\n\n ** createGraphFromHMatrix():\n";   }

    /// Have we been here?
    int searchId = Node::generateID(curr_iso_pos);
    if(visited->findVal(searchId,0) != -1 ) {
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

    std::cout << "  connectNewNode() where it should ...\n";



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

        std::cout << "looking at y= " << up_iso->abs_iso_y << ", x=" << up_iso->abs_iso_x << "\n";

        roadMatrix->dump();



        if(roadMatrix->matrix[(int)up_iso->abs_iso_y][(int)up_iso->abs_iso_x] == 1) { // is it a road?
            std::cout << "A\n";

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

            createGraphFromHMatrix(roadMatrix, graph, workNode2, workNode, up_iso, curr_iso_pos, visited, dbgLevel);                                     // RECURSION CALL

        }
    }


    /// Go right
    if(right_iso->abs_iso_x <= (roadMatrix->cols-1)) {
        if(roadMatrix->matrix[(int)right_iso->abs_iso_y][(int)right_iso->abs_iso_x] == 1) {

            if(dbgLevel >= 2) { std::cout << ind << "going right\n";}

            // create a new node, connect it to the right of current node
            searchId = Node::generateID(right_iso);
            workNode = currNode;
            workNode2 = workNode->attachNewNode(Node::iso_to_str(right_iso), searchId, right_iso, 1, 1, 0);


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

            workNode = currNode;
            workNode2 = workNode->attachNewNode(Node::iso_to_str(down_iso), searchId, down_iso, 1, 1, 0);

            createGraphFromHMatrix(roadMatrix, graph, workNode2, workNode, down_iso, curr_iso_pos, visited, dbgLevel);                                  // RECURSION CALL

        }
    }



    /// Go left
    if(left_iso->abs_iso_x >= 0) {
        if(roadMatrix->matrix[(int)left_iso->abs_iso_y][(int)left_iso->abs_iso_x] == 1) {

            if(dbgLevel >= 2) { std::cout << ind  << "going left\n"; }

            workNode = currNode;
            workNode2 = workNode->attachNewNode(Node::iso_to_str(left_iso), searchId, left_iso, 1, 1, 0);

            createGraphFromHMatrix(roadMatrix, graph, workNode2, workNode, left_iso, curr_iso_pos, visited, dbgLevel);                                  // RECURSION CALL

        }
    }







}










/// Used by TrafficManager and Bus to create the slot path from A to B for that bus
///
/// (--) TEST!
SlotPath *RoadNetwork::createSlotPath(HPos *fromPos, HPos *toPos )
{

    SlotPath *slotpath = new SlotPath();
    int dbgLevel = 1;

    std::string ind = "  ";
    std::cout << ind << "\n\n** createSlotPath \n";



    /// TODO: YOu have to break this up to a Function
    /// --->  CR8





    ///
    /// Make a Graph out of the RoadNetwork
    ///

    Graph *graph = new Graph("road_network_1");
    DijkstraResult *dijkstraResult = nullptr;

    int searchId = -1;
    HPos *start_abs_iso_pos = nullptr;






    // create the first road

    std::cout << ind <<  "Please find the first position of the road! \n";

    start_abs_iso_pos = new HPos(0, 0, USE_ISO);
    searchId = Node::generateID(start_abs_iso_pos);

    // add the first road to the graph , special!
    graph->addFirstNode(Node::iso_to_str(start_abs_iso_pos), searchId, start_abs_iso_pos);

    // find the first road in the graph so we can connect more roads to it
    Node *currNode = graph->findNode(searchId, 0);
    if(currNode == nullptr) { std::cout << "ERROR " << cn << " with createSlotPath()\n"; return nullptr; }








    // setup some objects...
    BinarySearchTree *visited = new BinarySearchTree();



    // ... so we can recursively walk the roadnetwork to get the coordinates for each road
    createGraphFromHMatrix(hMatrix, graph, currNode, nullptr, start_abs_iso_pos, nullptr, visited, dbgLevel);


    if(dbgLevel >=1 ) {
        graph->dump(0, 1);
        std::cout << "\n\n";
    }




    std::cout << "\n\n" << ind << "NOW that we have a GRAPH do a DIJKSTRA run and see what we end up with \n";











    // Plussa på alla rel_positioner med min_abs_isoX och Y
    // för att få riktiga koordinater sen i globala matrixen


    std::cout << "Dont forget to adjkust to the global matrix\n";





    ///
    /// run Dijkstra
    ///

    std::cout << "\n\nRunning Dijkstra 1st time:\n------------------------------\n";


    // Setup start and end positions for Dijkstra

    graph->dump(0,0);

    Node *startNode = graph->findNode( Node::genIDfrom_rel_iso(fromPos), 0);

    toPos->dump();
    Node *endNode   = graph->findNode ( Node::genIDfrom_rel_iso(toPos) , 1);
    endNode->dump(3);





    if(startNode == nullptr || endNode == nullptr) {
        std::cout << "ERROR! Could not find the start or end node for Dijkstra\n";
        return nullptr;
    }

    std::cout << "  * startNode.id=" << startNode->getId() << "\n";
    std::cout << "  * endNode.id= " << endNode->getId() << "\n";


    dijkstraResult = graph->runDijkstra(startNode, endNode, 2);

    if(dijkstraResult->shortestPath.empty()) {
        return nullptr;
    }

    std::cout << "\n";
    std::cout << "The shortest distance is=" << dijkstraResult->resultInt << "\n";
    std::cout << "The path is: \n    ";

    graph->printPathFromDijkstra(dijkstraResult);

    std::cout << "\n";








    std::cout << ind << "\ncreateSlotPath done ** \n";

    return slotpath;
}



/* HPOSDELETE
Vector2f RoadNetwork::getRandomRoad_abs_iso_pos(int findNr)
{

    std::cout << "getRandomRoad abs iso pos\n------------\n";
    this->dump("   ");


    int YMAX = max_isoYOffset - min_isoYOffset;
    int XMAX = max_isoXOffset - min_isoXOffset;

    std::cout << "\n\nyMax=" << YMAX << ", xMax=" << XMAX << "\n";

    Vector2f iso_pos = Vector2f();
    iso_pos.y = -1;
    iso_pos.x = -1;


    int nr = 0;


    for(int y = 0; y <= YMAX; y++) {
        for(int x = 0; x <= XMAX; x++) {

                std::cout << "y=" << y << ", x= " << x << "\n";

            if(hMatrix->matrix[y][x] == 1)
            {
                std::cout << "=1\n";
                iso_pos.y = min_isoYOffset + y;
                iso_pos.x = min_isoXOffset + x;

                if(nr == findNr) {
                    return iso_pos;
                } else {
                    nr++;
                }
            }

        }
    }

    return iso_pos;
}
*/
