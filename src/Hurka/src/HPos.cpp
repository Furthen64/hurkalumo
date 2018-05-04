#include "HPos.hpp"

HPos::HPos(int _y, int _x)
{
    abs_iso_y = _y;
    abs_iso_x = _x;
    rel_iso_y = _y;
    rel_iso_x = _x;
    gpix_y = 42;
    gpix_x  = 55;


}


HPos *HPos::clone()
{
    HPos *_pos = new HPos(abs_iso_y, abs_iso_x);
    _pos->rel_iso_y = rel_iso_y;
    _pos->rel_iso_x = rel_iso_x;
    _pos->gpix_y = gpix_y;
    _pos->gpix_x = gpix_x;

    return _pos;
}
