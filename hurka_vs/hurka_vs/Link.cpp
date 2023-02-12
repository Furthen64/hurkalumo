#include "Link.hpp"

// OBSERVE! You cannot use Node's functions in here... error: invalid use of incomplete type 'class Node'

Link::Link()
{
    from = nullptr;
    to = nullptr;
    weight = 0;
}
