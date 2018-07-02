#include "Toolbar.hpp"

#define TOOLBAR_BUTTONS 6
///
/// This is the main toolbar that you see in the top of the editor (maybe even in the game later on)
/// So far it has not much functionality, it just sits there- Some code in Core class for Clicking on Buttons




Toolbar::Toolbar(HPos *_hpos)
{



    // Create and Set position
    pos = new HPos();
    set_pos_by_gpix(_hpos);



    // Create a pixel rectangle
    rect = new HRect(pos, new HPos( (int) (pos->gpix_y + toolbarHeightPx), (int) (pos->gpix_x + toolbarWidthPx) , USE_GPIX) );

    toolbarXOffset = pos->gpix_x;

    // get the whole texture for all the buttons
    texture.loadFromFile(getFullUri("data\\textures\\TOOLBAR.png"));



    // allocate all the sprites
    sprites = new Sprite[nrButtons*2];
    visibleSpritesTopArr = new int [nrButtons];
    for(int i = 0; i < nrButtons; i++) { visibleSpritesTopArr[i] = true; }


    // map all the sprites correctly
    // position the sprites
    float x, y;

    for(int i = 0; i < nrButtons*2; i++){
        sprites[i] = Sprite(texture);
        sprites[i].setTextureRect( {i*widthPx, 0, 16, 16} );        // HARDCODED

        x = i*widthPx+ toolbarXOffset;
        y = 0;

        sprites[i].setPosition({x,y});
    }

}




// (-+)
void Toolbar::set_pos_by_gpix(HPos *_hpos)
{
    pos->gpix_y = _hpos->gpix_y;
    pos->gpix_x = _hpos->gpix_x;
}




// used by Core
// (--)
void Toolbar::pushButton(int indexNr)
{
    if(indexNr > nrButtons) {
        std::cout << "ERROR " << cn << " pushButton index out of bounds\n";
    }

    // set the status of the dang thing
    visibleSpritesTopArr[indexNr] = !visibleSpritesTopArr[indexNr];
}

// used by Core
// (--)
void Toolbar::resetButton(int indexNr)
{
    visibleSpritesTopArr[indexNr] = false;
}




// Draws only the visible sprites according to var visibleSpritesTopArr
// (-+)
void Toolbar::draw( RenderTarget& rt, HPos *viewHPos)
{
    // draw all the buttons individually
    for(int i = 0; i < nrButtons; i++){
        if(visibleSpritesTopArr[i] == true) {
            sprites[i].setTextureRect( {i*widthPx, 0, 16, 16} );    // Show the top one         HARDCODED
        } else {
            sprites[i].setTextureRect( {i*widthPx, 16, 16, 16} );       // HARDCODED
        }

        rt.draw( sprites[i] );
    }
}




/// \param mousepos An hpos with gpix values set
// (--) TEST
bool Toolbar::isPosInsideToolbar(HPos *mousepos)
{

    if(mousepos == nullptr) { std::cout << "ERROR " << cn << " isPosInsideToolbar nullptr!\n"; return false; }

    if(rect->absStart == nullptr) { std::cout << "ERROR " << cn << " toolbar's rect->absStart is nullptr!\n"; return false; }
    if(rect->absEnd == nullptr) { std::cout << "ERROR " << cn << " toolbar's rect->absEnd is nullptr!\n"; return false; }



    std::cout << "isPosInsideToolbar()------------!\n";
    std::string ind = "   ";

    int tbStartY = rect->absStart->gpix_y;
    int tbStartX = rect->absStart->gpix_x;
    int tbEndY = rect->absEnd->gpix_y;
    int tbEndX = rect->absEnd->gpix_x;


    std::cout << " mouse wpix ( " << mousepos->wpix_y << "," << mousepos->wpix_x << " )\n";
    std::cout << " toolbar rect gpix ( " << rect->absStart->gpix_y << "," << rect->absStart->gpix_x << " ) -> ( " << rect->absEnd->gpix_y  << ", " << rect->absEnd->gpix_x << ")\n";



    // FIXME make rect functions for this, call it "isGpixPosInsideRect" or something
    if(mousepos->wpix_y >= tbStartY && mousepos->wpix_y <= tbEndY) {
        if(mousepos->wpix_x >= tbStartX && mousepos->wpix_x <= tbEndX) {

            std::cout << "\n\n\n";
            return true;
        }
    }


    std::cout << "\n\n\n";

    return false;
}


// used by Core
// (--) test!
int Toolbar::whatButtonDidIPress(HPos *mousepos)
{
    int toolbarInts [TOOLBAR_BUTTONS] {TB_NEW_FILE, TB_SAVE_FILE, TB_LOAD_FILE, TB_ONE, TB_TWO, TB_EXIT};

    if(mousepos == nullptr) { std::cout << "ERROR " << cn << " isPosInsideToolbar nullptr!\n"; return false; }
    if(rect->absStart == nullptr) { std::cout << "ERROR " << cn << " toolbar's rect->absStart is nullptr!\n"; return false; }
    if(rect->absEnd == nullptr) { std::cout << "ERROR " << cn << " toolbar's rect->absEnd is nullptr!\n"; return false; }



    std::cout << "what button did I press()------------!\n";
    std::string ind = "   ";

    int buttonStartY = rect->absStart->gpix_y;
    int buttonStartX = rect->absStart->gpix_x;
    int buttonEndY = rect->absEnd->gpix_y;
    int buttonEndX = rect->absEnd->gpix_x;


    for(int n = 0; n < TOOLBAR_BUTTONS; n++) {

        buttonStartY = rect->absStart->gpix_y;
        buttonStartX = rect->absStart->gpix_x + (n *16);

        buttonEndY = rect->absEnd->gpix_y;
        buttonEndX = buttonStartX + (n * 16) + 16;

        if(mousepos->wpix_y >= buttonStartY && mousepos->wpix_y <= buttonEndY) {
            if(mousepos->wpix_x >= buttonStartX && mousepos->wpix_x <= buttonEndX) {

                std::cout << "n=" << n << ",  toolbarInts[n]=" << toolbarInts[n] << " pressed\n";

                return toolbarInts[n];
            }
        }
    }

    std::cout << "Warning" << cn << " whatButtonDidIPress returns TB_NONE\n";
    return TB_NONE;
}




// (++)
HPos *Toolbar::getPos() { return pos; }
HRect *Toolbar::getRect() {     return rect; }
int *Toolbar::getVisibleSpritesTopArr() {     return visibleSpritesTopArr; }

