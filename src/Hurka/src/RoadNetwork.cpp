#include "RoadNetwork.hpp"

RoadNetwork::RoadNetwork()
{
    buslist = new std::list<Bus *>();
}

// (+-)
void RoadNetwork::dump(std::string indent)
{
    hMatrix->dump(indent);
    std::cout << indent << "min_iso_y_offset=" << min_isoYOffset <<"\n";
    std::cout << indent << "min_iso_x_offset=" << min_isoXOffset<< "\n";
    std::cout << indent << "max_iso_y_offset=" << max_isoYOffset <<"\n";
    std::cout << indent << "max_iso_x_offset=" << max_isoXOffset << "\n\n";
}



/// (-+)
void RoadNetwork::addBus(Bus *_bus)
{
    buslist->push_back(_bus);
}




/// Returns absolute iso position for a bus
/// Returns -1,-1 when not found a road
///
/// Wishlist: Actually do RANDOM positioning, right now it only takes the first road it finds or after "nr" of finds
/// TEST IT A WHILE, it worked with simple testing
/// (--)
HPos *RoadNetwork::getRandomRoad_abs_iso(int findNr)
{

    HPos *iso_pos = new HPos(-1,-1, USE_ISO);

    int YMAX = max_isoYOffset - min_isoYOffset;     // Figure out the limits of the road matrix
    int XMAX = max_isoXOffset - min_isoXOffset;

    int nr = 0;

    // Loop between the limits and find first road we can see
    for(int y = 0; y <= YMAX; y++) {
        for(int x = 0; x <= XMAX; x++) {

            if(hMatrix->matrix[y][x] == 1)
            {

                // Found one!
                // Set iso positions for return object

                iso_pos->abs_iso_y = min_isoYOffset + y;
                iso_pos->abs_iso_x = min_isoXOffset + x;

                if(nr == findNr) {
                    return iso_pos;
                } else {
                    nr++;
                }
            }

        }
    }

    return iso_pos;
}





/* HPOSDELETE
Vector2f RoadNetwork::getRandomRoad_abs_iso_pos(int findNr)
{

    std::cout << "getRandomRoad abs iso pos\n------------\n";
    this->dump("   ");


    int YMAX = max_isoYOffset - min_isoYOffset;
    int XMAX = max_isoXOffset - min_isoXOffset;

    std::cout << "\n\nyMax=" << YMAX << ", xMax=" << XMAX << "\n";

    Vector2f iso_pos = Vector2f();
    iso_pos.y = -1;
    iso_pos.x = -1;


    int nr = 0;


    for(int y = 0; y <= YMAX; y++) {
        for(int x = 0; x <= XMAX; x++) {

                std::cout << "y=" << y << ", x= " << x << "\n";

            if(hMatrix->matrix[y][x] == 1)
            {
                std::cout << "=1\n";
                iso_pos.y = min_isoYOffset + y;
                iso_pos.x = min_isoXOffset + x;

                if(nr == findNr) {
                    return iso_pos;
                } else {
                    nr++;
                }
            }

        }
    }

    return iso_pos;
}
*/
