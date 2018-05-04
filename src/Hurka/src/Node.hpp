#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <climits>
#include <sstream>
#include "HPos.hpp"

using namespace sf;

class Link; // Forward declaration


enum NODE_DIR {
    dir_up,
    dir_right,
    dir_down,
    dir_left
};

// A very HEAVY class
// Its use with the Dijkstra algorithm makes it so that we need to overload the "<" operator
// in order to put it in a prio queue

// It also has some member labels for the algorithm to work (labels)
class Node
{
public:
    Node(std::string _name, int _id, Vector2f _iso_pos);
    void createLink();
    void dump(int indent);

    void attachNodeUp(Node *other);
    void attachNodeRight(Node *other);
    void attachNodeDown(Node *other);
    void attachNodeLeft(Node *other);



    Node *attachNewNode(std::string _name, int _id, Vector2f _iso_pos, int weight1, int weight2, int debugLevel);

    int getId();
    std::string getName();
    void resetForDijkstra();
    int connectNodes(Node *firstNode, Node *secondNode, int relDir, int weight1, int weight2, int debugLevel);




    /// Variables
    Link *up;
    int upLabel = INT_MAX;
    Link *right;
    int rightLabel = INT_MAX;
    Link *down;
    int downLabel = INT_MAX;
    Link *left;
    int leftLabel = INT_MAX;

    // Dijkstra stuff on top of the graph
    Node *fastestPrevNode = nullptr;
    int permanentLabel = INT_MAX;
    int tempLabel = INT_MAX;



    // Hur ska den hantera tal under 10000 ? såna där M=0
    // 2018-03-20
    // (--)
    static void idTo_iso_pos(int _id, int *y, int *x)
    {
        //if(_id > 10000) {
            (*y) = _id/10000;
            (*x) = _id-(10000* (*y));
        //}

    }



    // (-+)

    static int generateID(HPos *_hpos)
    {
        if(_hpos->abs_iso_y < 0) {
            std::cout << "ERROR Cannot generateID from negative nrs in iso_pos!\n";
            return -1;
        }

        if(_hpos->abs_iso_x < 0) {
            std::cout << "ERROR Cannot generateID from negative nrs in iso_pos!\n";
            return -1;
        }

        return _hpos->abs_iso_y * 10000 + _hpos->abs_iso_x;
    }

    /*
    static int generateID(Vector2f iso_pos)
    {
        if(iso_pos.y < 0) {
            std::cout << "ERROR Cannot generateID from negative nrs in iso_pos!\n";
            return -1;
        }

        if(iso_pos.x < 0) {
            std::cout << "ERROR Cannot generateID from negative nrs in iso_pos!\n";
            return -1;
        }

        return iso_pos.y * 10000 + iso_pos.x;

    }
    */



private:
    std::string name;
    int id;
    Vector2f iso_pos;
    std::string cn = "Node.cpp";
};
