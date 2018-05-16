#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <stack>
#include <queue>


#include "HurkaMatrix.hpp"

#include "Node.hpp"     // Important that Node is first! Only include Node here!
#include "Link.hpp"

#include "BinarySearchTree.hpp"


struct graphNodeCompare
 {
   bool operator()(Node* l, Node* r)
   {
       //std::cout <<    "                            l->templabel=" << l->tempLabel << ",   r->templabel= " << r->tempLabel << "\n";
       if( (l->tempLabel) < (r->tempLabel) ) {
            return 0;
       }
       return 1;
   }
 };


class DijkstraResult
{
public:

    int resultInt;
    std::string resultStr;
    std::stack<Node *> shortestPath;
    std::stack<Node *> shortestPath_memcpy;


    // Seems to work just fine
    // (-+)
    std::stack<Node *> *getCopyOfShortestPathStack()
    {
        // Copy the shortestpath to new stack
        std::stack<Node *> *retStack = new std::stack<Node *>();




        // First, push the stack to another stack so we get it reversed in the end ( a bit fumbly )
        int i = 0;

        Node *node;
        while(!shortestPath.empty()) {

            node = shortestPath.top();
            shortestPath.pop();

            shortestPath_memcpy.push(node);

            i++;
        }



        // Copy back to the return stack (which will be in the right order)
        // Also Copy back to the shortestPath stack because we've deleted all of its contents in the previous loop
        while(!shortestPath_memcpy.empty()) {

            node = shortestPath_memcpy.top();
            shortestPath_memcpy.pop();
            retStack->push(node);               // Result
            shortestPath.push(node);            // copy to the original stack
        }




        return retStack;
    }


    /// Dumps the shortest path as an array with all the id's of the nodes
    /// TEST
    /// (--)
    void dumpShortestPath()
    {
        // Copy the shortestpath to shortestpath_memcpy
        int i = 0;

        Node *node;
        while(!shortestPath.empty()) {

            node = shortestPath.top();
            shortestPath.pop();

            std::cout << "[" << i << "] " << node->getId() << "\n";

            shortestPath_memcpy.push(node);

            i++;
        }



        // Copy back to the shortestpath
        while(!shortestPath_memcpy.empty()) {

            node = shortestPath_memcpy.top();
            shortestPath_memcpy.pop();
            shortestPath.push(node);
        }

    }



private:

};




/*

Graph now works
2018-03-26


How to use it:

    addFirstNode() for the head of the graph
    then you just use that head outside graph to add nodes to it like

    graph->head->up->to = this new node ive created
    graph->head->right->to = this other new node Ive created

    Graph has a very OPEN datastructure in the sense that you can poke around with it from the outside


You can put nodes in there and build a network,
Run the Dijkstra algorithm, get results of the fastest path from A to B

You can also reset the algorithm and run it again on the same graph
with new result

For examples see:

    docs\how things work\dijkstra_examples.txt




*/
class Graph
{

public:
    Graph(std::string _name, int _mapMaxRows, int _mapMaxCols);

    HurkaMatrix *clockwiseTraverseUpFirst(Node *curr, BinarySearchTree *visited, HurkaMatrix *matrix, bool dumpNodes, int debugLevel);
    Node *         clockwiseTraverseUpFirstFindNode(Node *curr, BinarySearchTree *visited, HurkaMatrix *matrix, int searchId, int debugLevel);
    void dump(int debugLevel, int dumpNodes);
    void test();
    Node *getFirstNode();
    void addFirstNode(std::string _name, int _id, HPos *_iso_pos);
    Node *findNode(int searchId, int debugLevel);
    DijkstraResult *runDijkstra(Node *startNode, Node *endNode, int debugLevel);
    void printPathFromDijkstra(DijkstraResult *dijkstraResult);
    void traverseAndReset(Node *curr, BinarySearchTree *visited);
    void resetAllNodes();




private:

    Node *head;
    int nrNodes = 0;

    int mapMaxRows;
    int mapMaxCols;

    std::string name;
    std::string cn = "Graph.cpp";



};

#endif

