#include <string>
#include <iostream>


class Node;

class Link
{
public:
    Link();
    connectNodeUp(Node *);
    Node *from;
    Node *to;
    int weight;

    private:
};
