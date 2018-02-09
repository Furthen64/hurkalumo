#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <chrono>


using namespace sf;


/// WORDS FOR LUMOHURKA
///
/// isometric matrix:           The matrix of size N,M that is the gamespace, where all the things fit onto
///



/// //////////////////////////////////////////////
/// Globals

const int SPRITE_HEIGHT = 64;
const int SPRITE_WIDTH  = 64;




/// //////////////////////////////////////////////
/// (--) GameMatrix is the grid with all the visible sprites oh shit, its the whole game set right?
/// Maaaaaaaaaaan. Cant it just be a grid of grass first to make things easy?
///


class GameMatrix
{
public:

    GameMatrix(int _height, int _width, int _textureID)
    {
        height = _height;
        width = _width;
        textureID = _textureID;


        // Hardcoded for now
        texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRASS_1.png");
        sprite = Sprite(texture);

        if( (height%2 != 0) || (width%2 != 0)) {
            std::cout << "Warning! GameMatrix height Not divisible by 2! \n";
        }


    }

    int getWidth()
    {
        return width;
    }

    int getHeight()
    {
        return height;
    }

    // (--)
    // DOCS: see "GameMatrix_How_the_x_position_is_calculated.png"
    static int getWindowXPos(int N, int M)
    {

        int initialXOffset = 400;   // Start in the middle


        /// N = Width index in the grid
        /// M = Height index in the grid

        /// Calculate the X-offset
        // intialXOffset - where we are in the height=M index TIMES the sprite_width/2
        int xOffset = initialXOffset - M*(SPRITE_WIDTH/2);


        // now for every step to the right=N index we have to go right a bit
        int xStep = N*(SPRITE_WIDTH/2);

        if(M>0) {

        }


        return xOffset + xStep;
    }

    // (--)
    // DOCS: see "GameMatrix_How_the_y_position_is_calculated.png"
    static int getWindowYPos(int N, int M)
    {
        int initialYOffset = 0;     // Start at the top

        int yOffset = initialYOffset;

        int yStep = (M*SPRITE_HEIGHT/4) + (N*SPRITE_HEIGHT/4);

        return yOffset + yStep;
    }


    void draw( RenderTarget& rt)
    {
        int x = 0;
        int y = 0;

        // What a bunny brained idea!
        for(int M= 0; M<height; M++){
            for(int N= 0; N < width; N++) {

                x = GameMatrix::getWindowXPos(N,M);
                y = GameMatrix::getWindowYPos(N,M);
                Vector2f pos = {(float)x,(float)y};

                sprite.setPosition(pos);
                rt.draw(sprite);
            }

        }

        /*

        for(int y = 0; y < 600; y+=height)
        {
            float yF = y;
            float xF = 0.f;

            sprite.setPosition({0,yF});

            for(int x = 0; x < 800; x+=width)
            {
                //yF += floor(height/2);   // Go down a bit so we get diagonal, isometric drawing
                yF += height;
                xF = x;

                sprite.setPosition({xF, yF});
                rt.draw(sprite);
            }

        }*/

    }



private:
    int width;
    int height;
    int textureID;
    Sprite sprite;
    Texture texture;
};



/// //////////////////////////////////////////////
/// (--) House
/// 64 x 64 pixels

class House
{
public:
    House(const Vector2f& _pos, int _textureID)
        :
        pos(_pos)
    {
        textureID = _textureID;

        switch(textureID)
        {
        case 1:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_001.png");
            break;
        case 2:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_002.png");
            break;
        case 3:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_003.png");
            break;
        case 4:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_004.png");
            break;
        default:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TEMPLATE.png");
            break;

        }

        sprite = Sprite(texture);
    }


    void draw( RenderTarget& rt)
    {

        /*std::cout << "---------------------------------------\n";
        std::cout << "ID: " << textureID << "\n";
        std::cout << "getXPos(" << pos.x << ", " << pos.y << ") = " << GameMatrix::getWindowXPos(pos.x,pos.y) << "\n";
        std::cout << "getXPos(" << pos.x << ", " << pos.y << ") = " << GameMatrix::getWindowYPos(pos.x,pos.y) << "\n";
        */

        int x = GameMatrix::getWindowXPos(pos.x,pos.y);
        int y = GameMatrix::getWindowYPos(pos.x,pos.y);
        Vector2f pos = {(float)x,(float)y};
        sprite.setPosition(pos);
        rt.draw(sprite);

    }


private:
    int textureID;
    Texture texture;
    Sprite sprite;
    Vector2f pos;

};




/// //////////////////////////////////////////////
/// Grid

class Grid
{
public:
    Grid(int _height, int _width)
    {
        width = _width;
        height = _height;
        texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_1.png");
        sprite = Sprite(texture);
    }


    void draw( RenderTarget& rt)
    {
        // haha this will be hell to figure out
        // I need functions for getting the X,Y of the [N,M]th position in the matrix


        // Use a function to figure out what the REAL window X,Y is
        // given the position inside the Isometric Matrix


        int x = 0;
        int y = 0;

        // What a bunny brained idea!
        for(int M= 0; M<height; M++){
            for(int N= 0; N < width; N++) {

                x = GameMatrix::getWindowXPos(N,M);
                y = GameMatrix::getWindowYPos(N,M);
                Vector2f pos = {(float)x,(float)y};

                sprite.setPosition(pos);
                rt.draw(sprite);
            }

        }


/*
        for(int y = 0; y < 600; y+=height)
        {
            float yF = y;
            float xF = 0.f;

            sprite.setPosition({0,yF});

            for(int x = 0; x < 800; x+=width)
            {
                //yF += floor(height/2);   // Go down a bit so we get diagonal, isometric drawing
                yF += height;
                xF = x;

                sprite.setPosition({xF, yF});
                rt.draw(sprite);
            }

        }
        2018-02-09
        */

    }


private:
    Texture texture;
    Sprite sprite;
    int width;
    int height;
};



/// //////////////////////////////////////////////
/// and up comes the Toolbar

class Toolbar
{
public:
    Toolbar(const Vector2f& _pos)
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

    ~Toolbar()
    {
        delete sprites;
        delete visibleSpritesTopArr;
    }

    void pushButton(int relXPos)
    {
        // set the status of the dang thing
        visibleSpritesTopArr[0] = !visibleSpritesTopArr[0];

    }

    void draw( RenderTarget& rt)
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




    Vector2f getPos()
    {
        return pos;
    }

    int *getVisibleSpritesTopArr()
    {
        return visibleSpritesTopArr;
    }

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


/// //////////////////////////////////////////////
/// Locomotive
class Locomotive
{
public:
    Locomotive(const Vector2f& _pos)
        :
        pos(_pos)
    {
        texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\LOCOMOTIVE.png");
        sprite = Sprite(texture);
        sprite.setTextureRect( {0,0,32,32});
    }

    void draw( RenderTarget& rt) const
    {
        rt.draw( sprite );
    }
    void setDirectionConst( const Vector2f& dir)
    {
        vel = dir * speed; //??
    }

    void setDirection( Vector2f dir)
    {
        vel = dir * speed; //??
    }

    void update( float dt)
    {
        pos += vel * dt;
        sprite.setPosition(pos);
    }

    Vector2f getPos()
    {
        return pos;
    }

private:
    static constexpr float speed = 100.0f;  // move 100 px per second
    Vector2f pos;
    Vector2f vel = {0.0f, 0.0f};
    Texture texture;
    Sprite sprite;
};



/// ///////////////////////////////////////////////////////////////
/// Start

int main()
{
    // Setup Window
    // , Style::Fullscreen);
    RenderWindow window(sf::VideoMode(800, 600), "Hurkalumo Editor 0.1-alpha");

    //window.setFramerateLimit(60);

    int GAME_WIDTH = 64;
    int GAME_HEIGHT = 64;


    // What to draw
    bool drawGm = true;
    bool drawLoco = true;
    bool drawToolbar = true;
    bool drawGrid = true;
    bool drawHouses = true;
    // Setup objects
    GameMatrix gm({GAME_HEIGHT,GAME_WIDTH,1});
    Locomotive loco({10.0f, 10.0f});
    Toolbar toolbarTop({260.0f, 0.0f});
    Grid grid(GAME_HEIGHT, GAME_WIDTH);



//                  N M
//                  X Y
    House house001({0,0},1);
    House house002({1,0},1);
    House house003({2,0},1);

    House house004({0,0},3);
    House house005({0,1},3);
    House house006({0,2},3);

    House house007({1,1},4);
    House house008({2,1},4);
    House house009({3,1},4);

    // Setup timing
    auto tp = std::chrono::steady_clock::now();





    drawGm = true;
    drawLoco = false;
    drawToolbar = false;
    drawGrid = true;
    drawHouses = false;





    /// Main Loop

    while (window.isOpen())
    {

        /// Get events

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }



        // I dont understand this
        float dt;
        {
            const auto newTp = std::chrono::steady_clock::now();
            dt = std::chrono::duration<float>(newTp - tp).count();
            tp = newTp;
        };


        /// Update

        // Get mouse input
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos_i = sf::Mouse::getPosition(window); // window is a sf::Window

            Vector2f mousePos_f = Vector2f();
            mousePos_f.x = mousePos_i.x;
            mousePos_f.y = mousePos_i.y;


            /// Pushing button on top toolbar

            // if (toolbarTop.within(mousePos_i.x, mousepPos_i.y) { pushbutton(x,y));
            toolbarTop.pushButton(0); // debug test


            bool rightof = false;
            bool belowof = false;
            bool topof = false;
            bool leftof = false;
            Vector2f dir;

            // Figure out what +-1 to do with the position
            if(loco.getPos().x < mousePos_f.x) {
                rightof = true;
            }
            if(loco.getPos().x >= mousePos_f.x) {
                leftof = true;
            }
            if(loco.getPos().y < mousePos_f.y) {
                belowof = true;
            }
            if(loco.getPos().y >= mousePos_f.y) {
                topof = true;
            }

            if(rightof) {
                dir.x += 1.0f;
            }
            if(leftof) {
                dir.x -= 1.0f;
            }
            if(belowof) {
                dir.y += 0.5f;
            }
            if(topof) {
                dir.y -= 0.5f;
            }

            // if we are right of the object

            loco.setDirection(dir);


        }

        loco.update(dt);
        //toolbarTop.update(); TODO should I do this?




        /// Render
        window.clear({198, 198, 198});


    // Setup objects


        // Draw the game board
        if(drawGm)   {  gm.draw(window);  } // Draws the ground and water and suchers
        if(drawGrid) {  grid.draw(window); } // If we want a visible grid to know the borders of each cell
        if(drawLoco) {  loco.draw(window); }
        if(drawHouses) {

            /// Add these to a list
            /// Go over which order they should be drawn in
            /// Assign them renderorder

            house001.draw(window);
            house002.draw(window);
            house003.draw(window);

            house004.draw(window);
            house005.draw(window);
            house006.draw(window);

            house007.draw(window);
            house008.draw(window);
            house009.draw(window);

        }

        if(drawToolbar) {   toolbarTop.draw(window); }


        window.display();


    }

    return 0;
}

/*
KodBös:


    RectangleShape shape1(Vector2f(20.f,20.f));
    shape1.setPosition(Vector2f(50.f, 50.f));
    shape1.setFillColor(Color(100, 250, 50));

    shape1.setOutlineThickness(10);
    shape1.setOutlineColor(sf::Color(10, 10, 10));

  texture.loadFromFile("ANIM_001.png");
        sprite = Sprite(texture);
        sprite.setTextureRect( {0,0,30,30});




  sf::Rect rect1(0.f, 0.f);
    rect1.setFillColor(sf::Color(100, 250, 50));
    sf::Rect rect2(320.f,240.f);


     window.draw(shape1);
        window.draw(sprite);
*/


