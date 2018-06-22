#include "SlotPath.hpp"

// Used by Bus
// Planned by TrafficManager

SlotPath::SlotPath()
{
    slotPositions = new std::list<SlotPos *>();
}


// (--)
void SlotPath::setListOfSlotPositions(std::list<SlotPos *> *_slotPositions)
{
    if(slotPositions != nullptr) {
        delete slotPositions;
        slotPositions = _slotPositions;
    }
}


// (--)
void SlotPath::addListOfSlotPositions(std::list<SlotPos *> *_slotPositions)
{
    // Concatenate the already existing list
    if(slotPositions == nullptr) {
        slotPositions = new std::list<SlotPos *>();
    }


    // Loop over _slotPositions and insert to internal list
    SlotPos *slotpos;

    for(std::list<SlotPos *>::iterator it = _slotPositions->begin(); it != _slotPositions->end(); ++it)
    {
        slotpos = (*it);
        slotPositions->push_back(slotpos);
    }

}




// (-+)
void SlotPath::drawAllSlots(sf::RenderTarget &rt, HPos *viewHPos)
{
    for(std::list<SlotPos *>::iterator itAll = slotPositions->begin(); itAll != slotPositions->end(); ++itAll)
    {
        (*itAll)->draw(rt, viewHPos);
    }

}


// (-+)
void SlotPath::add(SlotPos *slotpos)
{
    slotPositions->push_back(slotpos);
}



// (--)
bool SlotPath::hasValues()
{

    return (slotPositions->size() >0);
}


// Not yet
void SlotPath::reset()
{
    std::cout << "slotpath::reset is NOT implemented!\n";
    //slotPositions->clear();
}


// (--) TEST
void SlotPath::dump()
{
    std::cout << "\n\nDumping slotpath:\n{";
    SlotPos *slotpos;
    int nr = 0;
    for(std::list<SlotPos *>::iterator it = slotPositions->begin(); it != slotPositions->end(); ++it)
    {

        std::cout << "nr=" << nr << "\n";
        slotpos = (*it);

        slotpos->dump("   ");

        nr++;
    }

    std::cout << "\n}\n";
}





/// \brief take a step in the slotpath, consume a Gpix HPos
/// consumes that step in the std::list slotPositions!
// (--)
SlotPos *SlotPath::stepAndGetPos(int steps)
{
    SlotPos *workPos = nullptr;

    for(int nr = 0; nr < steps; nr++)
    {
        if(slotPositions->empty() ) {
            break;
        }

        // Pop the first in the list to use it
        workPos = slotPositions->front();
        slotPositions->pop_front(); // Destroy first element

    }

    nowPos = workPos;   // Store away in the slotpath as the current position
    return workPos;     // Also return it

}










































