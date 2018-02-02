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
/// (--) GameMatrix
/// 64 x 32 pixels

class GameMatrix
{
public:
    GameMatrix(int _height, int _width)
    {
        height = _height;
        width = _width;

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

    static int getWindowXPos(int cellWidthPosition, int cellHeightPosition)
    {

        //            1                                      0
        //                        * 64                                * 64  /2


        return cellWidthPosition * SPRITE_WIDTH;// - (cellWidthPosition * SPRITE_HEIGHT/2);

    }

    static int getWindowYPos(int cellWidthPosition, int cellHeightPosition)
    {
        return cellHeightPosition * SPRITE_HEIGHT + (cellWidthPosition * SPRITE_WIDTH / 2);// - (cellWidthPosition * SPRITE_HEIGHT/2);
    }



private:
    int width;
    int height;
};



/// //////////////////////////////////////////////
/// (--) House
/// 64 x 32 pixels

class House
{
public:
    House(const Vector2f& _pos, int textureID)
        :
        pos(_pos)
    {
        switch(textureID)
        {
        case 1:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_001.png");
            break;
        case 2:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_002.png");
            break;
        default:
            texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TEMPLATE.png");
            break;

        }

        sprite = Sprite(texture);
    }


    void draw( RenderTarget& rt)
    {
        // haha this will be hell to figure out
        // I need functions for getting the X,Y of the [N,M]th position in the matrix
        int x = GameMatrix::getWindowXPos(pos.x,1);
        int y = GameMatrix::getWindowYPos(pos.y,1);
        Vector2f pos = {(float)x,(float)y};
        sprite.setPosition(pos);
        rt.draw(sprite);



    }


private:
    Texture texture;
    Sprite sprite;
    Vector2f pos;
    const int width = 64;
    const int height = 32;
};




/// //////////////////////////////////////////////
/// Grid on the ground
/// 64 x 32 pixels

class Grid
{
public:
    Grid(void)
    {
        texture.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRID_1.png");
        sprite = Sprite(texture);
    }


    void draw( RenderTarget& rt)
    {
        // haha this will be hell to figure out
        // I need functions for getting the X,Y of the [N,M]th position in the matrix


        // Use a function to figure out what the REAL window X,Y is
        // given the position inside the Isometric Matrix


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

    }


private:
    Texture texture;
    Sprite sprite;
    const int width = 64;
    const int height = 32;
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




    // Setup objects
    Locomotive loco({10.0f, 10.0f});
    Toolbar toolbarTop({260.0f, 0.0f});
    Grid grid;
    House house001({1,1},1);
    House house002({2,1},2);
    House house003({3,1},3);

    // Setup timing
    auto tp = std::chrono::steady_clock::now();




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

        // Draw sprites
        grid.draw(window);
        loco.draw(window);
        house001.draw(window);
        house002.draw(window);
        house003.draw(window);


        toolbarTop.draw(window);


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


