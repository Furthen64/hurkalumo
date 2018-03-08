#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <SFML/Graphics.hpp>

using namespace sf;

/// //////////////////////////////////////////////
/// and up comes the Toolbar

class Toolbar
{
public:

    Toolbar() {}
    Toolbar(const Vector2f& _pos);



    ~Toolbar();
    void pushButton(int relXPos);

    void draw( RenderTarget& rt);


    Vector2f getPos();
    int *getVisibleSpritesTopArr();

private:
    Vector2f pos;

    Texture texture;    // we need one texture but
    Sprite *sprites;      // we need 12 sprites for each of the 6 buttons

    int toolbarXOffset = 240;

    int *visibleSpritesTopArr;  // Top array that controls which are visible
                                // 1 = show the top one (unpressed)
                                // 0 = show the one below (pressed)

    const int nrButtons = 6;
    const int widthPx = 16;
};


#endif
