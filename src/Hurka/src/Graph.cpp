#include "Graph.hpp"

Graph::Graph(std::string _name)
{
    name = _name;
}




/// Walks the entire Graph.
/// While walking, put 1:s in a matrix wherever the road is, 0:s everything else.
/// dumpNodes = if true will do a .dump on all Nodes it find
/// Recursive.
// (-+)
HurkaMatrix *Graph::clockwiseTraverseUpFirst(Node *curr, BinarySearchTree *visited, HurkaMatrix *matrix, bool dumpNodes, int debugLevel)
{

    // Go clockwise from Top
    // TOP
    //      RIGHT
    //              DOWN
    //                          LEFT



    /// Have we already visited this node?
    int result = visited->findVal(curr->getId(), 0 );

    if(debugLevel >=2) { std::cout << "clockwiseTraverseUpFirst\n-------------------------------\n";

        std::cout << "dumping visited nodes:\n";
        visited->dumpBST();
    }



    if(result == -1) {

        if(debugLevel >= 2) { std::cout << "never seen node " << curr->getName() << " before\n"; }

        // Never seen this node before, output its value
        int m = 0;
        int n = 0;


        curr->idTo_iso_pos(curr->getId(), &m, &n);
        matrix->matrix[m][n] = 1;
        if(dumpNodes==true)
        {
            curr->dump(3);
        }
        if(debugLevel >=1 ) { std::cout << "curr id = " << curr->getId() << ", name = \"" << curr->getName() << "\"\n"; }

        // add to visited
        visited->add(curr->getId(), debugLevel);

        /// Up
        if(curr->up->to != nullptr) {
            clockwiseTraverseUpFirst(curr->up->to, visited, matrix, dumpNodes, debugLevel);
        }

        /// Right
        if(curr->right->to != nullptr) {

            clockwiseTraverseUpFirst(curr->right->to, visited, matrix,dumpNodes, debugLevel);

        }

        /// Down
        if(curr->down->to != nullptr) {


            clockwiseTraverseUpFirst(curr->down->to, visited, matrix, dumpNodes,debugLevel);

        }

        /// Left
        if(curr->left->to != nullptr) {

            clockwiseTraverseUpFirst(curr->left->to, visited, matrix, dumpNodes,debugLevel);

        }



    } else {

        // already been here
        if(debugLevel >= 1) {
            std::cout << "already been here\n";
        }
    }

    return matrix;
}



// Wishlist: Remove the Matrix code after you're done with this. Remove all unecesseary outputs
// TEST
// (--)
Node *Graph::clockwiseTraverseUpFirstFindNode(Node *curr, BinarySearchTree *visited, HurkaMatrix *matrix, int searchId, int debugLevel)
{


    Node *resultNode = nullptr;   /// THINK ABOUT THIS and life cycle through recursion! should it be passed along?


    // Go clockwise from Top
    // TOP
    //      RIGHT
    //              DOWN
    //                          LEFT



    // Have we already visited this node?
    int result = visited->findVal(curr->getId(), 0 );



    if(result == -1) {

        // Never seen this node before, check its value against searchId
        int m = 0;
        int n = 0;

        if(curr->getId() == searchId) {

            if(debugLevel >= 1) { std::cout << " ! Found it \n"; }

            // Mark it in the matrix
            curr->idTo_iso_pos(curr->getId(), &m, &n);
            matrix->matrix[m][n] = 2;

            return curr;
        } else {
            if(debugLevel >= 1) {  std::cout << "    no match on " << curr->getId() << "\n"; }
        }



        curr->idTo_iso_pos(curr->getId(), &m, &n);
        matrix->matrix[m][n] = 1;


        if(debugLevel >= 1) {  std::cout << "    curr id = " << curr->getId() << ", name = \"" << curr->getName() << "\"\n"; }

        // add to visited
        visited->add(curr->getId(), debugLevel);

        /// Up
        if(curr->up->to != nullptr) {

            resultNode = clockwiseTraverseUpFirstFindNode(curr->up->to, visited, matrix, searchId, debugLevel);
        }

        if(resultNode != nullptr) {
            return resultNode;
        }

        /// Right
        if(curr->right->to != nullptr) {

            resultNode = clockwiseTraverseUpFirstFindNode(curr->right->to, visited, matrix, searchId, debugLevel);

        }

        if(resultNode != nullptr) {
            return resultNode;
        }

        /// Down
        if(curr->down->to != nullptr) {

            resultNode = clockwiseTraverseUpFirstFindNode(curr->down->to, visited, matrix, searchId, debugLevel);

        }

        if(resultNode != nullptr) {
            return resultNode;
        }

        /// Left
        if(curr->left->to != nullptr) {

            resultNode = clockwiseTraverseUpFirstFindNode(curr->left->to, visited, matrix, searchId, debugLevel);

        }

        if(resultNode != nullptr) {
            return resultNode;
        }



    } else {

        // already been here
        if(debugLevel >= 1) {
            std::cout << "already been here\n";
        }
    }


    return resultNode;




}




// (--)
void clockwiseSearchUpFirst(Node *curr, int searchVal)
{
    std::cout << "\n\nNOT DONE!!! :(\n\n";

}


// TEST!
// (--)
void Graph::dump(int debugLevel, int dumpNodes)
{
    std::cout << "\n\nDumping all Nodes in Graph:\n";

    BinarySearchTree *visited = new BinarySearchTree();

    HurkaMatrix *matrix = new HurkaMatrix(16,16);

    matrix = clockwiseTraverseUpFirst(head, visited, matrix, dumpNodes, debugLevel);

    std::cout << "\n\nDumping Graph as Matrix:\n";

    matrix->dump();



}


// (++)
void Graph::addFirstNode(std::string _name, int _id, Vector2f _iso_pos)
{
    head =  new Node(_name, _id, _iso_pos);
}


// (++)
Node *Graph::getFirstNode()
{
    return head;
}



/// Walks the graph in one thread and return the node, return nullptr if not found
//
// Seems to work with head and most of the nodes I tried
// (+-)
Node *Graph::findNode(int searchId, int debugLevel)
{

    if(searchId < 0) {
        std::cout << cn << " ERROR! Trying findNode with a negative searchId=" << searchId << "\n";
        return nullptr;
    }

    if(debugLevel >=1) {std::cout << "\n\n *** Searching the Graph for searchId= " << searchId << "\n ------------------------\n";}


    /// Is it the head?

    if(head != nullptr) {
        if(head->getId() == searchId) {
            if(debugLevel >=1) {std::cout << "  Found it!\n";}
            return head;
        }
    }



   /// Find the Node* and return it

    BinarySearchTree *visited = new BinarySearchTree();

    HurkaMatrix *matrix = new HurkaMatrix(16,16);  // For output!

    Node *foundNode = clockwiseTraverseUpFirstFindNode(head, visited, matrix, searchId, debugLevel);

    if(foundNode == nullptr) {
        std::cout << cn << " ERROR! Could not find a node with id=" << searchId << " in the graph \n";
    }

    if(debugLevel >= 2) {
        matrix->dump();
    }

    return foundNode;

}




// TEST!
// (--)
void Graph::traverseAndReset(Node *curr, BinarySearchTree *visited)
{


    // Have we already visited this node?
    int result = visited->findVal(curr->getId(), 0 );

    if(result == -1)
    {


        curr->resetForDijkstra();
        visited->add(curr->getId(), 0);


         /// Up
        if(curr->up->to != nullptr) {

            traverseAndReset(curr->up->to, visited);
        }

        /// Right
        if(curr->right->to != nullptr) {

            traverseAndReset(curr->right->to, visited);

        }


        /// Down
        if(curr->down->to != nullptr) {

            traverseAndReset(curr->down->to, visited);

        }

        /// Left
        if(curr->left->to != nullptr) {

            traverseAndReset(curr->left->to, visited);

        }
    }

}



// (--)
void Graph::resetAllNodes()
{
    BinarySearchTree *visited = new BinarySearchTree();

    traverseAndReset(this->head, visited);    // Runs a recursive reset function
}





// I have tested starting with
//
//   (0,1)        Which is a single road directly connected to an intersection      Works!
//   (1,1)        Which is an intersection                                          Works!
//
// Tested also from maany points to points in my road network, and it all seems to work
//
//
// DO i need to put nodes so many times in the VisitedNodes? Seems like a byproduct of old code?

// (--+)
DijkstraResult *Graph::runDijkstra(Node *startNode, Node *endNode, int debugLevel)
{

    std::string ind1 = "   ";
    std::string ind2 = "       ";
    std::string ind3 = "           ";
    std::string ind4 = "              ";

    int steps = 0;
    int maxSteps = 400;
    int done = false;
    int lastOne = false;

    Node *workNode = nullptr;
    DijkstraResult *result = new DijkstraResult();
    BinarySearchTree *visitedNodes = new BinarySearchTree();

    // We need a Priority Queue to see which has the lowest value tempLabel (shortest total distance as of now)
    std::priority_queue<Node *, std::vector<Node *>, graphNodeCompare> workList;



    /// FIXME, TODO: Run resetDijkstra for all Nodes
    //resetAllNodes();


    /// setup the first node
    startNode->permanentLabel = 0;
    startNode->fastestPrevNode = nullptr;

    if(debugLevel >=1) {
        std::cout << "\n";
        std::cout << ind1 << "Setup startNode\n" << ind1 << "------------------\n";
        std::cout << ind2 << startNode->getName() << ", " << startNode->getId() << "\n";
        std::cout << ind2 << "startNode.permanent=0\n";
    }

    /// Which vertices can we reach from the start?



    if(startNode->up->to != nullptr) {

        // assign temporary label to this node          put the distance (weight) in that box!
        startNode->up->to->tempLabel = startNode->up->weight;
        startNode->up->to->fastestPrevNode = startNode;

        if(debugLevel >= 2) { std::cout << ind2 << "startNode ----(" << startNode->up->weight << ")----> up\n";  }

        // Now that we have a temporaryLabel value, put it on the worklist
        if(debugLevel >=1) {
            std::cout << ind3 << "--> push up ( " << startNode->up->to->getName() << ", " << startNode->up->to->getId() << ")  tempLabel= " << startNode->up->to->tempLabel
            << ", fastestPrevNode= " << startNode->up->to->fastestPrevNode->getId() << "\n\n";
        }


        workList.push(startNode->up->to);
        visitedNodes->add(startNode->up->to->getId(),0);

    }
    if(startNode->right->to != nullptr) {
        // assign temporary label to this node
        startNode->right->to->tempLabel = startNode->right->weight;
        startNode->right->to->fastestPrevNode = startNode;

        if(debugLevel >= 2) { std::cout << ind2 << "startNode ----(" << startNode->right->weight << ")----> right\n";  }

        if(debugLevel >=1) {
            std::cout << ind3 << "--> push right ( " << startNode->right->to->getName() << ", " << startNode->right->to->getId() << ")  tempLabel= " << startNode->right->to->tempLabel
            << ", fastestPrevNode= " << startNode->right->to->fastestPrevNode->getId() << "\n\n";
        }
        workList.push(startNode->right->to);
        visitedNodes->add(startNode->right->to->getId(),0);



    }
    if(startNode->down->to != nullptr) {
        // assign temporary label to this node
        startNode->down->to->tempLabel = startNode->down->weight;
        startNode->down->to->fastestPrevNode = startNode;

        if(debugLevel >= 2) { std::cout << ind2 << "startNode ----(" << startNode->down->weight << ")----> down\n";  }

        if(debugLevel >=1) {
            std::cout << ind3 << "--> push down ( " << startNode->down->to->getName() << ", " << startNode->down->to->getId() << ")  tempLabel= " << startNode->down->to->tempLabel
            << ", fastestPrevNode= " << startNode->down->to->fastestPrevNode->getId() << "\n\n";
        }
        workList.push(startNode->down->to);
        visitedNodes->add(startNode->down->to->getId(),0);

    }
    if(startNode->left->to != nullptr) {
        // assign temporary label to this node
        startNode->left->to->tempLabel = startNode->left->weight;
        startNode->left->to->fastestPrevNode = startNode;

        if(debugLevel >= 2) { std::cout << ind2 << "startNode ----(" << startNode->left->weight << ")----> left\n";  }

        if(debugLevel >=1) {
            std::cout << ind3 << "--> push left ( " << startNode->left->to->getName() << ", " << startNode->left->to->getId() << ")  tempLabel= " << startNode->left->to->tempLabel
            << ", fastestPrevNode= " << startNode->left->to->fastestPrevNode->getId() << "\n\n";
        }
        workList.push(startNode->left->to);
        visitedNodes->add(startNode->left->to->getId(),0);
    }


    // startNode is now permanent
    visitedNodes->add(startNode->getId(), 0);












    steps = 0;
    done = false;

    if(debugLevel >=1 ) {
        std::cout << "\n";
        std::cout << ind1 << "Loop begins {\n" << ind1 << "--------------------\n" << ind1 << "\n";
    }

    while(!done) {



        ///
        /// Select the vertex with the smallest temporary label and
        /// make its label permanent (change worknode to this)
        ///



        if(workList.empty()){
            if(debugLevel >=1) {
                std::cout << ind2 << "Worklist is empty, exiting loop\n" << ind1 << "}\n";
            }
            done = true;
            break;
        }

        if(debugLevel >= 2) {

            std::cout << "\n\n";
            std::cout << ind2<<"Get worknode from Prio Queue with the smallest temporary label and \n";
            std::cout << ind2<<"make its label permanent, change worknode to this\n";
            std::cout << ind2<<"-----------------------------------------------------------------------------\n";
        }


        // Istället för att söka från workNode och runtomkring
        // sätt worknode till vad du poppar från workList (prio kön)
        workNode = workList.top();
        workList.pop();

        if(debugLevel >=1){
            std::cout << ind2 << "workNode = worklist.pop()  (" << workNode->getName() << ", " << workNode->getId() << ")\n";
        }


        /// ... make its label permanent
        workNode->permanentLabel = workNode->tempLabel;
        visitedNodes->add(workNode->getId(),0);

        if(debugLevel >=1) {
            std::cout << ind2 << "workNode.permanent = " << workNode->tempLabel << " (" << workNode->getName() << ", " << workNode->getId() << ")\n";
            std::cout << ind2 << "workNode.fastestPrevNode = " << workNode->fastestPrevNode->getId() << "\n\n";
        }
















        ///
        /// Now work with the node you just made Permanent."workNode"
        /// Put temporary labels on each node you can reach from this node
        ///

        if(debugLevel >=2) {
            std::cout << ind2 << "\n\n";
            std::cout << ind2 << "Now work with the node you just made Permanent \"workNode\". \n";
            std::cout << ind2 << "Put temporary labels on each node you can reach from this node.\n";
            std::cout << ind2 << "--------------------------------------------------------------------\n";
        } else if(debugLevel >=1) {
            std::cout << ind2 << "Look around current node, update temp labels\n";
        }


        if(workNode->up->to != nullptr) {

        ///
        /// UP
        ///

            // Is it already Permanent?

            if(workNode->up->to->permanentLabel == INT_MAX) {


                // Store sum of "permanent and direct distance from it"
                if(workNode->up->to->tempLabel > (workNode->permanentLabel + workNode->up->weight)) {

                    if(debugLevel >= 1) {
                        std::cout << ind3 << "new minimum! workNode --> up (value before=" << workNode->up->to->tempLabel << ", after=" <<
                        (workNode->permanentLabel + workNode->up->weight) << ")\n";
                    }

                    // New minimum, update tempLabel and also the fastest Prev Node
                    workNode->up->to->tempLabel = workNode->permanentLabel + workNode->up->weight;
                    workNode->up->to->fastestPrevNode = workNode;



                }


                // Now that we have a temporaryLabel value, put it on the worklist

                if(visitedNodes->findVal(workNode->up->to->getId(),0) == -1)  {

                    // did not find it , push it to work list!
                    if(debugLevel >=1) {
                        std::cout << ind3 << "--> push up ( " << workNode->up->to->getName() << ", " << workNode->up->to->getId() << ")  tempLabel= " << workNode->up->to->tempLabel
                            << ", fastestPrevNode= " << workNode->up->to->fastestPrevNode->getId() << "\n\n";
                    }

                    workList.push(workNode->up->to);
                    visitedNodes->add(workNode->up->to->getId(),0);

                }



            } else {
                if(debugLevel >=2) { std::cout << ind3 << "Already permanented: workNode->up      (" << workNode->up->to->getName() << ", .permanent= " << workNode->up->to->permanentLabel << " \n"; }
            }
        }





        ///
        /// RIGHT
        ///



        if(workNode->right->to != nullptr) {


            if(workNode->right->to->permanentLabel == INT_MAX) {



                if(workNode->right->to->tempLabel > (workNode->permanentLabel + workNode->right->weight)) {

                    if(debugLevel >= 1) {
                        std::cout << ind3 << "new minimum! workNode --> right (value before=" << workNode->right->to->tempLabel << ", after=" <<
                        (workNode->permanentLabel + workNode->right->weight) << ")\n";
                    }

                    // New minimum, update tempLabel and also the fastest Prev Node
                    workNode->right->to->tempLabel = workNode->permanentLabel + workNode->right->weight;
                    workNode->right->to->fastestPrevNode = workNode;

                }

                // Now that we have a temporaryLabel value, put it on the worklist

                if(visitedNodes->findVal(workNode->right->to->getId(),0) == -1)  {

                    // did not find it , push it to work list!
                    if(debugLevel >=1) {
                        std::cout << ind3 << "--> push right ( " << workNode->right->to->getName() << ", " << workNode->right->to->getId() << ")  tempLabel= " << workNode->right->to->tempLabel
                            << ", fastestPrevNode= " << workNode->right->to->fastestPrevNode->getId() << "\n\n";
                    }


                    workList.push(workNode->right->to);
                    visitedNodes->add(workNode->right->to->getId(),0);
                }




            } else {
                if(debugLevel >=2) { std::cout << ind3 << "Already permanented: workNode->right      (" << workNode->right->to->getName() << ", .permanent= " << workNode->right->to->permanentLabel << " \n"; }
            }

        }





        ///
        /// DOWN
        ///


        if(workNode->down->to != nullptr) {

            if(workNode->down->to->permanentLabel == INT_MAX) {


                if(workNode->down->to->tempLabel > (workNode->permanentLabel + workNode->down->weight)) {


                    if(debugLevel >= 1) {
                        std::cout << ind3 << "new minimum! workNode --> down (value before=" << workNode->down->to->tempLabel << ", after=" <<
                        (workNode->permanentLabel + workNode->down->weight) << ")\n";


                    }

                    // New minimum, update tempLabel and also the fastest Prev Node
                    workNode->down->to->tempLabel = workNode->permanentLabel + workNode->down->weight;
                    workNode->down->to->fastestPrevNode = workNode;

                }



                if(visitedNodes->findVal(workNode->down->to->getId(),0) == -1)  {

                    // did not find it , push it to work list!
                    if(debugLevel >=1) {
                        std::cout << ind3 << "--> push down ( " << workNode->down->to->getName() << ", " << workNode->down->to->getId() << ")  tempLabel= " << workNode->down->to->tempLabel
                            << ", fastestPrevNode= " << workNode->down->to->fastestPrevNode->getId() << "\n\n";
                    }

                    workList.push(workNode->down->to);
                    visitedNodes->add(workNode->down->to->getId(),0);
                }

            } else {
                if(debugLevel >=2) { std::cout << ind3 << "Already permanented: workNode->down      (" << workNode->down->to->getName() << ", .permanent= " << workNode->down->to->permanentLabel << " \n"; }

            }


        }





        ///
        /// LEFT
        ///


        if(workNode->left->to != nullptr) {


            if(workNode->left->to->permanentLabel == INT_MAX) {


                if(workNode->left->to->tempLabel > (workNode->permanentLabel +  workNode->left->weight)) {

                    if(debugLevel >= 1) {
                        std::cout << ind3 << "new minimum! workNode --> left (value before=" << workNode->left->to->tempLabel << ", after=" <<
                        (workNode->permanentLabel + workNode->left->weight) << ")\n";
                    }

                    // New minimum, update tempLabel and also the fastest Prev Node
                    workNode->left->to->tempLabel = workNode->permanentLabel +  workNode->left->weight;
                    workNode->left->to->fastestPrevNode = workNode;
                }



                if(visitedNodes->findVal(workNode->left->to->getId(),0) == -1)  {

                    // did not find it , push it to work list!
                    if(debugLevel >=1) {
                        std::cout << ind3 << "--> push left ( " << workNode->left->to->getName() << ", " << workNode->left->to->getId() << ")  tempLabel= " << workNode->left->to->tempLabel
                            << ", fastestPrevNode= " << workNode->left->to->fastestPrevNode->getId() << "\n\n";
                    }


                    workList.push(workNode->left->to);
                    visitedNodes->add(workNode->left->to->getId(),0);
                }


            } else {
               if(debugLevel >=2) { std::cout << ind3 << "Already permanented: workNode->left         (" << workNode->left->to->getName() << ", .permanent= " << workNode->left->to->permanentLabel << " \n"; }


            }


        }


        ///
        /// Exiting ?
        ///

        steps++;

        if(steps > maxSteps) {
            done = true;
        }



        // Step 2 in exiting
        if(lastOne && workList.empty() ) {
            done = true;
        }

        // Step 1  in exiting
        if(workList.empty() == true) {
            lastOne = true;
        }



    }



    ///
    /// Return the result
    ///

    workNode = endNode;
    if(debugLevel >=1 ){
        std::cout << ind1 << "Result output\n";
        std::cout << ind1 << "--------------------------\n";
    }


    workNode = endNode;

    done = false;

    // Go backwards from the endNode and push all the fastestPrevNodes until we reach the startNode
    // Push to stack in the result

    while(!done) {

        result->shortestPath.push(workNode);


        if(workNode == startNode) {
            done = true;
            break;
        }

        workNode = workNode->fastestPrevNode;
    }

    // Store the total fastest distance from start to end
    result->resultInt = endNode->permanentLabel;


    if(debugLevel >=1 ) {
            std::cout << "\n";
    }


    if(debugLevel >=2) {
        // Dumping result
        std::cout << "Dumping result:\n";
        result->dumpShortestPath();
    }




    // OBSERVE, when we return we have changed the state of nodes in the graph ... should it be like that?

    return result;

}


/// Prints the path from left (start) to right (end)
// (-+)
void Graph::printPathFromDijkstra(DijkstraResult *dijkstraResult)
{

    if(dijkstraResult == nullptr) {
        std::cout << cn << " ERROR dijkstraResult has nullptr in printPathFromDijkstra()\n";
        return ;
    }

    Node *workNode;
    std::string str;

    // Look at the result
    while( ! (dijkstraResult->shortestPath.empty()) )
    {
        workNode = dijkstraResult->shortestPath.top();

        str = dijkstraResult->shortestPath.top()->getName();

        dijkstraResult->shortestPath.pop();

        if(! (dijkstraResult->shortestPath.empty())) {
            std::cout << str << " ==> ";
        } else {
            std::cout << str << "\n";
        }

    }

}
