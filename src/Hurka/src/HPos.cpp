#include "HPos.hpp"

/// @param _y Vertical positioning, or on the isometric board= M the sloping down and left.
/// @param _x Horizontal positioning, or on the isometric board= N the sloping down and right
/// @param positionType USE_ISO or USE_GPIX , choose what values to store away.
HPos::HPos(int _y, int _x, int positionType)
{
    if(positionType == USE_ISO) {
        abs_iso_y = _y;
        abs_iso_x = _x;
        rel_iso_y = _y;
        rel_iso_x = _x;
        gpix_y = 0;
        gpix_x = 0;
    }

    if(positionType == USE_GPIX) {
        abs_iso_y = 0;
        abs_iso_x = 0;
        rel_iso_y = 0;
        rel_iso_x = 0;
        gpix_y = _y;
        gpix_x = _x;

    }
}


HPos::HPos()
{
    abs_iso_y = 0;
    abs_iso_x = 0;
    rel_iso_y = 0;
    rel_iso_x = 0;
    gpix_y = 0;
    gpix_y = 0;
}




/// @brief Copies all the values from the original to the new one, returns newly allocated HPos
HPos *HPos::clone()
{
    HPos *_pos = new HPos(abs_iso_y, abs_iso_x, USE_ISO);
    _pos->rel_iso_y = rel_iso_y;
    _pos->rel_iso_x = rel_iso_x;
    _pos->gpix_y = gpix_y;
    _pos->gpix_x = gpix_x;

    return _pos;
}


void HPos::dump()
{
    std::cout << "Dumping HPos:\n";
    std::cout << " abs_iso   y=" << abs_iso_y << ", x=" << abs_iso_x << "\n";
    std::cout << " rel_iso   y=" << rel_iso_y << ", x=" << rel_iso_x << "\n";
    std::cout << " gpix      y=" << gpix_y << ", x=" << gpix_x << "\n";

}
