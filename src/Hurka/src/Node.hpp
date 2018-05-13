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
//
// It also has some member labels for the algorithm to work (labels)
//
// It also contains HPos for positioning in the gamematrix
class Node
{
public:
    Node(std::string _name, int _id, HPos *_rel_iso_pos);


    void createLink();
    void dump(int indent);

    Node *attachNodeUp(Node *other);
    Node *attachNodeRight(Node *other);
    Node *attachNodeDown(Node *other);
    Node *attachNodeLeft(Node *other);

    Node *attachNewNode(std::string , int , HPos *, int , int , int );


    int getId();
    std::string getName();
    void resetForDijkstra();
    int connectNodes(Node *firstNode, Node *secondNode, int relDir, int weight1, int weight2, int debugLevel);


    HPos *get_rel_iso_pos();
    HPos *get_copy_rel_iso_pos();



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



    // How should it handle id:s under 10000? where M=0?        // 2018-03-20
    // (--)
    /// \brief Converts the supplied y and x values back to their respective iso positions
    /// \param _id Searchid, like 10006, will become y=1, x=6
    /// \param y
    /// \param x
    static void idTo_iso_pos(int _id, int *y, int *x)
    {
        //if(_id > 10000) {
            (*y) = _id/10000;
            (*x) = _id-(10000* (*y));
        //}

    }





    // (-+)
    static int genIDfrom_rel_iso(HPos *_hpos)
    {
        if(_hpos->rel_iso_y < 0) {
            std::cout << "ERROR Cannot generateID from negative nrs in iso_pos!\n";
            return -1;
        }

        if(_hpos->rel_iso_y < 0) {
            std::cout << "ERROR Cannot generateID from negative nrs in iso_pos!\n";
            return -1;
        }

        return _hpos->rel_iso_y * 10000 + _hpos->rel_iso_x;
    }




    // (-+)
    static int genIDfrom_abs_iso(HPos *_hpos)
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




    // (--)
    static std::string rel_iso_to_str(HPos *hpos)
    {

        std::stringstream sstm;

        sstm << "(" << hpos->rel_iso_y << "," << hpos->rel_iso_x << ")";

        return sstm.str();
    }


    // (--)
    static std::string abs_iso_to_str(HPos *hpos)
    {
        std::stringstream sstm;

        sstm << "(" << hpos->abs_iso_y << "," << hpos->abs_iso_x << ")";

        return sstm.str();
    }





private:
    std::string name;
    int id;
    HPos *rel_iso_pos;
    std::string cn = "Node.cpp";
};
