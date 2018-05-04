#ifndef HPOS_H
#define HPOS_H

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


class HPos
{
public:
    HPos(int _iso_y, int _iso_x);
    HPos();

    int abs_iso_y;
    int abs_iso_x;
    int rel_iso_y;
    int rel_iso_x;
    int gpix_y;
    int gpix_x;
    HPos *clone();

private:




};

#endif
