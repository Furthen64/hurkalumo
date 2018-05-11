#include "SlotPath.hpp"

// Used by Bus
// Planned by TrafficManager

SlotPath::SlotPath()
{
}


void SlotPath::drawAllSlots(sf::RenderWindow &rt, HPos *viewHPos)
{
    for(std::list<SlotPos *>::iterator itAll = slotPositions.begin(); itAll != slotPositions.end(); ++itAll)
    {
        (*itAll)->draw(rt, viewHPos);
    }

}



void SlotPath::add(SlotPos *slotpos)
{

}



void SlotPath::reset()
{
    slotPositions.clear();
}



void SlotPath::dump()
{
}


