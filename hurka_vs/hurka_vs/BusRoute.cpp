#include "BusRoute.hpp"


BusRoute::BusRoute()
{
    positions = new std::list<HPos *>();
}


// True if the bus is at the this given position, False otherwise
bool BusRoute::isAt_iso_pos(HPos *_pos)
{
    std::cout << "NOT CODED\n";
    return false;
}



HPos *BusRoute::getNext_iso_pos()
{
    std::cout << "NOT CODED\n";
    return nullptr;
}

HPos *BusRoute::getNext_pix_pos()
{
    std::cout << "NOT CODED\n";
    return nullptr;
}


