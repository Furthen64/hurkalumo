
#include "GameMatrix.hpp"



/// //////////////////////////////////////////////
/// (-+) GameMatrix is the grid with all the visible sprites oh shit, its the whole game set right?
/// Or something. Haven't decided yet.
///


GameMatrix::GameMatrix(int _rows, int _cols, int _textureID)
{
    rows= _rows;
    cols = _cols;

    // Figure out min and max heightPx and max widthPx of the gameboard
    calculatePxBounds();



    TextureManager *textureMgr;
    textureMgr = textureMgr->getInstance();
    int _textureId = 551;
    std::string textureName = textureMgr->getTextureNameByIndex(_textureId);

    bool result = textureMgr->applyTexture(textureName, &texture);

    if(!result) {
        std::cout << "ERROR GameMatrix creation, cannot find texture id \"" << _textureId << "\".\n";
        return ;
    }

    sprite = Sprite(texture);


    if( (rows%2 != 0) || (cols%2 != 0)) {
        std::cout << "Warning! GameMatrix height Not divisible by 2! \n";
    }
    std::cout << "1\n";
}






int GameMatrix::getCols()
{
    return cols;
}

int GameMatrix::getRows()
{
    return rows;
}




// (--) test
HRect *GameMatrix::getHRect()
{
    return new HRect(0,0, rows, cols, heightPx, widthPx);
}







/// \brief Given a searchpos with abs iso values set, checks if thats within the gamematrix
/// \param searchpos Iso abs values set
/// \return returns true if inside, false if not
// (-+)
bool GameMatrix::isPosInsideGameMatrix(HPos *searchpos)
{
    searchpos = Grid::convert_gpix_to_iso(searchpos, GRID_TEXTURE_HEIGHT, GRID_TEXTURE_WIDTH);   // fixme some dayyyy

    if(searchpos->abs_iso_y > -1
       &&
       searchpos->abs_iso_x > -1)  {

       return true;
    }

    return false;
}


/// \brief Draw the gameboard a.k.a gamematrix , the tiles upon which everything happens
/// Optimize with the ints maybe make it a const?
/// Wishlist: Add Clipping to this!
///           If you are drawing outside the viewing 800x600 px window, you could just NOT draw that sprite, make ac
///           clipping rectangle and compare to it
/// (-+)
void GameMatrix::draw( RenderTarget& rt, HPos *viewHPos)
{
    minX = 9990;
    minY = 9990;
    int x = 0;
    int y = 0;
    int actualXMin = 9999;
    int actualYMin= 9999;

    for(int Y= 0; Y<rows; Y++){

        for(int X= 0; X < cols; X++) {

            x = Grid::convert_iso_to_gpix_x(Y,X, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);
            y = Grid::convert_iso_to_gpix_y(Y,X, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);

            if( x <actualXMin) {
                actualXMin = x;
            }

            if( y < actualYMin) {
                actualYMin = y;
            }

            // Offset for viewing position
            y += viewHPos->gpix_y;
            x += viewHPos->gpix_x;


            Vector2f pos = {(float)x,(float)y};     // SFML specific for the sprite

            if(x < minX) {
                minX = x;
            }

            if(y < minY) {
                minY = y;
            }

            sprite.setPosition(pos);

            rt.draw(sprite);
        }

    }

}




// (--) test!
void GameMatrix::calculatePxBounds()
{
    std::cout <<"NOT TESTED void GameMatrix::calculatePxBounds()\n";
    // we have rows and cols, we DO NOT have startPos

    startPos = new HPos(0,0, USE_ISO);
    int minX = 9990;
    int minY = 9990;
    int maxX = 0;
    int maxY = 0;

    int x = 0;
    int y = 0;


    for(int Y= 0; Y<rows; Y++){
        for(int X= 0; X < cols; X++) {

            x = Grid::convert_iso_to_gpix_x(Y,X, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);
            y = Grid::convert_iso_to_gpix_y(Y,X, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 1);


            if(x < minX) {
                minX = x;
            }

            if(x > maxX) {
                maxX = x;
            }

            if(y < minY) {
                minY = y;
            }

            if(y > maxY) {
                maxY = y;
            }

        }

    }

    if(startPos == nullptr) {
        startPos = new HPos( minX, minY, USE_GPIX);
    } else {

        startPos->dump("   ");

        startPos->gpix_x = minX;
        startPos->gpix_y = minY;
    }



    heightPx = maxY;

    widthPx = maxX;


}

