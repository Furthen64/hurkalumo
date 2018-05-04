#ifndef HPOS_H
#define HPOS_H

#include <iostream>
/*
    - Whoa !

      Why so many variables for a position?! Shouldn't there only be one position?

    - No. NOOOOO. I have to make it harder than it is! I have to have ABSOLUTE ISO POSITION (abs_iso_y, abs_iso_x)
      so I know where this ~thing~ is on the entire gameboard.

      I also have to know within a smaller matrix like roadnetworks where the thing is, like a bus, where that bus is
      inside that little rectangle (rel_iso_y, rel_iso_x).

    - So what's with the gpix positions?

    - Well, now that I know where they are in the ISOMETRIC game board and roadnetworks, I need to have PIXEL awareness.
    I need to know where in the entire board its at, pixelwise, and then where it's actually rendered.

    Where it's actually at: (gpix_y, gpix_x) gpix stands for "gameboard pix"



*/



/* Wishlist:


        + More constrctors - I wanna be able to create hpos on the fly based on these different set of positions!
                             By using an ENUM?

                             gpixPos = new HPos(1024,768, USE_GPIX);
                             iso_pos = new HPos(0,1, USE_GPIX);             < would be a FAULTY use of this, so, you should
                                                                              be able to spot that youve made an error by
                                                                              left hand side saying "iso" and right saying "gpix"

                              iso_pos = new HPos(0,1, USE_ISO);

*/




enum PositionTypes { USE_GPIX, USE_ISO };


class HPos
{

public:
    HPos(int _y, int _x, int positionType);
    HPos();

    int abs_iso_y;
    int abs_iso_x;
    int rel_iso_y;
    int rel_iso_x;
    int gpix_y;
    int gpix_x;

    HPos *clone();
    void dump();



private:




};

#endif
