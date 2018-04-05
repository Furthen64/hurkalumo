#include "Grid.hpp"



/// //////////////////////////////////////////////
/// Grid           cr 2018-02-22
// The grid follows the static grid size of 64 x 64 px


// (--)
Grid::Grid(int _height, int _width)
{
    width = _width;
    height = _height;
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_1.png");
    sprite = Sprite(texture);


    textureSelected.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_SELECTED.png");
    spriteSelected = Sprite(textureSelected);

    // Load font
    if (!font.loadFromFile("consola.ttf"))
    {
        std::cout << "ERROR " << cn << " could not load font.\n";
    }
}


// (-+)
void Grid::draw( RenderTarget& rt, Vector2i viewPos)
{
    int gx = 0;
    int gy = 0;
    int wx = 0;
    int wy = 0;

    Vector2f wPos = Vector2f();
    /*Vector2f firstGPos = Vector2f();        // Game position of the first grid cell
    Vector2f firstWPos = Vector2f();        // Window position of the first grid cell

    bool once = false;

    */


    for(int M= 0; M<height; M++){
        for(int N= 0; N < width; N++) {

            gx = convert_iso_to_gpix_x(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT,0);
            gy = convert_iso_to_gpix_y(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT,0);

  /*          if(!once)
            {
                firstGPos.x = gx;
                firstGPos.y = gy;
            }
*/
            wx = gx + viewPos.x;
            wy = gy + viewPos.y;

/*            if(!once) {
                firstWPos.x = wx;
                firstWPos.y = wy;
                once = true;
            }
*/

            wPos.x = wx;
            wPos.y = wy;

            sprite.setPosition(wPos);
            rt.draw(sprite);

        }

    }


    // Draw the selected grid that should be more visible
    gx = convert_iso_to_gpix_x(selected_iso_pos.y, selected_iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
    gy = convert_iso_to_gpix_y(selected_iso_pos.y, selected_iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);

    wx = gx+ viewPos.x;
    wy = gy+ viewPos.y;

    wPos.x = wx;
    wPos.y = wy;

    spriteSelected.setPosition(wPos);
    rt.draw(spriteSelected);



    /// Also draw coordinates in text for the top one
    /*
    text.setFont(font);

    std::stringstream sstm;
    sstm << "GPOS(" << firstGPos.y << ", " << firstGPos.x << ")\n";
    sstm << "WPOS(" << firstWPos.y << ", " << firstWPos.x << ")\n";

    std::string txtStr = sstm.str();
    text.setString(txtStr );
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);


    // inside the main loop, between window.clear() and window.display()
    text.setPosition(firstWPos);
    rt.draw(text);
*/


}


// (--)
void Grid::setVisible(Vector2f iso_pos)
{
    drawSelectedGrid = true;


    selected_iso_pos = iso_pos;

    selected_pix_pos.x = convert_iso_to_gpix_x(iso_pos.y, iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
    selected_pix_pos.y = convert_iso_to_gpix_y(iso_pos.y, iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0 );



 //   std::cout << "Visible grid position: " << selected_pix_pos.x << " , " << selected_pix_pos.y << "\n";
}

void Grid::hideVisible()
{
    drawSelectedGrid = false;
}
