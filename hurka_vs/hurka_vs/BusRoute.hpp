#ifndef BUSROUTE_H
#define BUSROUTE_H

#include <iostream>
#include <list>
#include "HPos.hpp"

// A busroute is a set of roads that the bus moves between from A->B

// Wishlist: Might need a cleanup of all these next_iso_pos across the whole project
class BusRoute
{
public:
    BusRoute();

    bool isAt_iso_pos(HPos *_iso_pos);       // True if the bus is at the this given position, False otherwise

    HPos *getNext_iso_pos();
    HPos *getNext_pix_pos();


private:

    std::list <HPos *> *positions;

};


#endif
