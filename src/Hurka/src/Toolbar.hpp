#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <SFML/Graphics.hpp>
#include "HRect.hpp"    // <-- #include "HPos.hpp"
#include "Utils.hpp"





enum TOOLBARBUTTONS { TB_NEW_FILE, TB_SAVE_FILE, TB_LOAD_FILE, TB_ONE, TB_TWO, TB_EXIT, TB_NONE };



/// //////////////////////////////////////////////
/// and up comes the Toolbar

class Toolbar
{
public:

    Toolbar(HPos *);

    HRect *getRect();

    void pushButton(int relXPos);

    void resetButton(int indexNr);

    void draw( RenderTarget& rt, HPos *viewHPos);

    HPos *getPos();

    void set_pos_by_gpix(HPos *_hpos);

    int *getVisibleSpritesTopArr();

    int whatButtonDidIPress(HPos *mousepos);

    bool isPosInsideToolbar(HPos *mousepos);

private:

    std::string cn = "Toolbar.cpp";
    HPos *pos;
    HRect *rect;

    Texture texture;    // we need one texture but
    Sprite *sprites;      // we need 12 sprites for each of the 6 buttons

    int toolbarXOffset = 240;

    int *visibleSpritesTopArr;  // Top array that controls which are visible
                                // 1 = show the top one (unpressed)
                                // 0 = show the one below (pressed)

    const int nrButtons = 6;
    const int widthPx = 16;


    unsigned int toolbarHeightPx = 20;
    unsigned int toolbarWidthPx = widthPx * nrButtons;
};


#endif
