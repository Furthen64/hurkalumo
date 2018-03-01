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
#include "FileManager.hpp"
#include "HurkaMap.hpp"

#include "Constants.hpp"



// Always push RUNNING code
// If it doesn't run, just stub out the errors and make it run before pushing to a branch


// TODO: kanske ta en titt så alla Width och Height parametrar är i rätt ordning till funktioner
// TODO: lägga alla i en datastruktur som är lätt att rita upp


using namespace sf;


/// WORDS FOR LUMOHURKA
///
/// isometric matrix: The matrix of size N,M that is the gamespace, where all the things fit onto
///







/// Tests

// (++)
void waitForInput()
{
    std::cout << "\nPress enter to continue...\n";
    getchar();
}


// (--)
bool testFileManager(TextureManager *textureMgr, int debugLevel=0)
{
    FileManager fmgr;
    bool result = true;

    if(debugLevel >0) {
        std::cout << "\n\n*** Working directory ***\n";
        fmgr.printWorkingDir();
    }


    if(debugLevel >0) {

        std::cout << "\n\n*** Verifying file ***\n";
    }

    if(!fmgr.verifyFile("data/garden.txt")) {
        result = false;
    }

    if(debugLevel >0) {
        std::cout << "\n\n*** Testing reading files ***\n";
    }


    HurkaMap hmap = fmgr.readRegularFile("data/garden.txt", textureMgr);
    if(hmap.mapName== "empty") {
        result = false;
    }

    return result;
}






// (++)
void testList(TextureManager *textureMgr)
{

    std::cout << "\n\n*** testList()\n";

    std::list<Block> blockList;

    Block house001 ({0,1}, "HOUSE001", textureMgr);
    blockList.push_back(house001);
    std::cout << "    Texturename of first block: " << house001.getTextureName() << "\n";

    Block tree001( {0,0}, "TREE001", textureMgr);
    blockList.push_back(tree001);
    std::cout << "    Texturename of second block: " << tree001.getTextureName() << "\n\n";


    std::cout << "Blocklist content: \n{\n";

    // iterate over all items
    int n = 0;
    for (std::list<Block>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
    {

     std::cout << "    [" << n << "] texturename of current block: " << (*itAll).getTextureName() << "\n";

     n++;
    }
    std::cout << "}\n end of Testlist \n\n";

}

/// ///////////////////////////////////////////////////////////////
/// Start

int main()
{

    bool integrityTesting = false;


    // Should this manager be a singleton perhaps?
    TextureManager textureMgr;


    // Setup Window
    // , Style::Fullscreen);
    RenderWindow window(sf::VideoMode(800, 600), "Hurkalumo Editor 0.1-alpha");

    window.setFramerateLimit(10);





    if(integrityTesting) {
        if(!testFileManager(&textureMgr)) {
            std::cout << "ERROR: testFileManager failed. Something went wrong during integrity test of software!";
            waitForInput();
            return 0;
        }
    }

    // Nr of Cells on the grid
    int GAME_WIDTH = 64;
    int GAME_HEIGHT = 64;


    // What to draw
    bool drawGm = true;
    bool drawLoco = false;
    bool drawToolbar = false;
    bool drawGrid = true;
    bool drawBlocks = true;

    // Setup objects
    GameMatrix gm({GAME_HEIGHT,GAME_WIDTH,1});          /// high level structure of game
    Locomotive loco({10.0f, 10.0f});                    /// sample locomotive
    Toolbar toolbarTop({260.0f, 0.0f});
    Grid grid(GAME_HEIGHT, GAME_WIDTH);

    FileManager fmgr;                                   /// Used to read the garden.txt file for now





    /// Testing

    //testList(&textureMgr);





    /// Read the map



    std::cout << "\n\n** Reading garden.txt *** \n";

    HurkaMap hmap = fmgr.readRegularFile("data/garden.txt", &textureMgr);





//                  N M
//                  X Y



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

        window.clear({0, 0, 0});



        // Draw the game board
        if(drawGm)   {  gm.draw(window);  } // Draws the ground and water and suchers

        if(drawGrid) {  grid.draw(window); }

        if(drawLoco) {  loco.draw(window); }
        if(drawBlocks) {

            /// Iterate list of blocklists to draw them in renderorder

            hmap.draw(window);



        }




        if(drawToolbar) {   toolbarTop.draw(window); }


        window.display();


    }

    return 0;
}
