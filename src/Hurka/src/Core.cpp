#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <chrono>


// Project includes
#include "Grid.hpp"
#include "TextureManager.hpp"
#include "Toolbar.hpp"
#include "GameMatrix.hpp"
#include "Block.hpp"
#include "Locomotive.hpp"

#include "Constants.hpp"



// Always push RUNNING code
// If it doesn't run, just stub out the errors and make it run before pushing to a branch


// TODO: kanske ta en titt så alla Width och Height parametrar är i rätt ordning till funktioner


using namespace sf;


/// WORDS FOR LUMOHURKA
///
/// isometric matrix: The matrix of size N,M that is the gamespace, where all the things fit onto
///










/// ///////////////////////////////////////////////////////////////
/// Start

int main()
{
    // Setup Window
    // , Style::Fullscreen);
    RenderWindow window(sf::VideoMode(800, 600), "Hurkalumo Editor 0.1-alpha");

    //window.setFramerateLimit(60);


    // Nr of Cells on the grid
    int GAME_WIDTH = 64;
    int GAME_HEIGHT = 64;


    // What to draw
    bool drawGm = true;
    bool drawLoco = true;
    bool drawToolbar = true;
    bool drawGrid = true;
    bool drawBlocks = true;
    // Setup objects
    GameMatrix gm({GAME_HEIGHT,GAME_WIDTH,1});
    Locomotive loco({10.0f, 10.0f});
    Toolbar toolbarTop({260.0f, 0.0f});
    Grid grid(GAME_HEIGHT, GAME_WIDTH);
    TextureManager textureMgr;


//                  N M
//                  X Y


    Block house001({0,0},"HOUSE001", &textureMgr);
    Block house002({2,0},"HOUSE001", &textureMgr);
    Block house003({4,0},"HOUSE001", &textureMgr);

    Block roadA01({0,2},"ROAD001", &textureMgr);
    Block roadA02({2,2},"ROAD001", &textureMgr);
    Block roadA03({4,2},"ROAD001", &textureMgr);

    Block tree001({0,4},"TREE001", &textureMgr);
    Block tree002({2,4},"TREE001", &textureMgr);
    Block tree003({4,4},"TREE001", &textureMgr);


    // Setup timing
    auto tp = std::chrono::steady_clock::now();





    drawGm = true;
    drawLoco = true;
    drawToolbar = true;
    drawGrid = false;
    drawBlocks = true;





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
        window.clear({0, 0, 0});


    // Setup objects


        // Draw the game board
        if(drawGm)   {  gm.draw(window);  } // Draws the ground and water and suchers

        if(drawLoco) {  loco.draw(window); }
        if(drawBlocks) {

            /// Add these to a list
            /// Go over which order they should be drawn in
            /// Assign them renderorder

            house001.draw(window);
            house002.draw(window);
            house003.draw(window);


            roadA01.draw(window);
            roadA02.draw(window);
            roadA03.draw(window);


            tree001.draw(window);
            tree002.draw(window);
            tree003.draw(window);


        }

        if(drawGrid) {  grid.draw(window); } // If we want a visible grid to know the borders of each cell

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


