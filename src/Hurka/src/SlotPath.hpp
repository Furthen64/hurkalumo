#ifndef SLOTPATH_H
#define SLOTPATH_H

#include <list>
#include "SlotPos.hpp"

class SlotPath
{
public:
    SlotPath();

    void drawAllSlots(sf::RenderWindow &rt, HPos *viewHPos);
    void add(SlotPos *slotpos);
    void reset();
    void dump();

private:
    std::list<SlotPos *> slotPositions;




};



#endif
