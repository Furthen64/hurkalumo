#ifndef SLOTPATH_H
#define SLOTPATH_H

#include <list>
#include "SlotPos.hpp"

class SlotPath
{
public:
    SlotPath();

    void drawAllSlots(sf::RenderTarget &rt, HPos *viewHPos);
    void add(SlotPos *slotpos);
    void reset();
    void dump();

private:
    std::list<SlotPos *> *slotPositions = nullptr;     // Is this really the best datastructure for this...? I need to use it like an Array




};



#endif
