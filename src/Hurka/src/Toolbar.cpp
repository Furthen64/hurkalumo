#include "Toolbar.hpp"

/// //////////////////////////////////////////////
/// and up comes the Toolbar


Toolbar::Toolbar(const Vector2f& _pos)
    :
    pos(_pos)
{

    toolbarXOffset = pos.x;


    // get the whole texture for all the buttons
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TOOLBAR.png");



    // allocate all the sprites
    sprites = new Sprite[nrButtons*2];
    visibleSpritesTopArr = new int [nrButtons];
    for(int i = 0; i < nrButtons; i++) { visibleSpritesTopArr[i] = true; }


    // map all the sprites correctly
    // position the sprites
    float x, y;

    for(int i = 0; i < nrButtons*2; i++){
        sprites[i] = Sprite(texture);
        sprites[i].setTextureRect( {i*widthPx, 0, 16, 16} );

        x = i*widthPx+ toolbarXOffset;
        y = 0;

        sprites[i].setPosition({x,y});
    }

}

Toolbar::~Toolbar()
{
    delete sprites;
    delete visibleSpritesTopArr;
}

void Toolbar::pushButton(int relXPos)
{
    // set the status of the dang thing
    visibleSpritesTopArr[0] = !visibleSpritesTopArr[0];

}

void Toolbar::draw( RenderTarget& rt, Vector2i viewPos)
{
    // draw all the buttons individually
    for(int i = 0; i < nrButtons; i++){
        if(visibleSpritesTopArr[i] == true) {
            sprites[i].setTextureRect( {i*widthPx, 0, 16, 16} );    // Show the top one
        } else {
            sprites[i].setTextureRect( {i*widthPx, 16, 16, 16} );
        }

        // Draw it
        rt.draw( sprites[i] );
    }
}




Vector2f Toolbar::getPos()
{
    return pos;
}

int *Toolbar::getVisibleSpritesTopArr()
{
    return visibleSpritesTopArr;
}
