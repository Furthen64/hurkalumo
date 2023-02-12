#include <string>
#include <iostream>


class Node;  // Forward declaration to Node but no include of node.hpp

class Link
{
public:
    Link();
    void connectNodeUp(Node *);
    Node *from;
    Node *to;
    int weight;

    private:
};
