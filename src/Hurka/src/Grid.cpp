#include "Grid.hpp"



/// //////////////////////////////////////////////
/// Grid           cr 2018-02-22
// The grid follows the static grid size of 64 x 64 px


Grid::Grid(int _height, int _width)
{
    width = _width;
    height = _height;
    texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_1.png");
    sprite = Sprite(texture);


    textureSelected.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_SELECTED.png");
    spriteSelected = Sprite(textureSelected);
}


// (-+)
void Grid::draw( RenderTarget& rt, Vector2u viewPos)
{
    // haha this will be hell to figure out
    // I need functions for getting the X,Y of the [N,M]th position in the matrix


    // Use a function to figure out what the REAL window X,Y is
    // given the position inside the Isometric Matrix


    int x = 0;
    int y = 0;
    Vector2f pos = Vector2f();

    // What a bunny brained idea!
    for(int M= 0; M<height; M++){
        for(int N= 0; N < width; N++) {

            x = convert_iso_to_pix_x(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT,0);
            y = convert_iso_to_pix_y(M,N, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT,0);

            x += viewPos.x;
            y += viewPos.y;


            pos.x = x;
            pos.y = y;

            sprite.setPosition(pos);
            rt.draw(sprite);

        }

    }


    // Draw the visible grid
    x = convert_iso_to_pix_x(selected_iso_pos.y, selected_iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
    y = convert_iso_to_pix_y(selected_iso_pos.y, selected_iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);

    x += viewPos.x;
    y += viewPos.y;

    pos.x = x;
    pos.y = y;

    spriteSelected.setPosition(pos);
    rt.draw(spriteSelected);

}


// (--)
void Grid::setVisible(Vector2f iso_pos)
{
    drawSelectedGrid = true;


    selected_iso_pos = iso_pos;

    selected_pix_pos.x = convert_iso_to_pix_x(iso_pos.y, iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0);
    selected_pix_pos.y = convert_iso_to_pix_y(iso_pos.y, iso_pos.x, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT, 0 );

 //   std::cout << "Visible grid position: " << selected_pix_pos.x << " , " << selected_pix_pos.y << "\n";
}

void Grid::hideVisible()
{
    drawSelectedGrid = false;
}
