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


// TODO: texturemanager är hemsk, den slingrar sina tentakler överallt!!
//       gör om den till singleton först o främst
// TODO: kanske ta en titt så alla Width och Height parametrar är i rätt ordning till funktioner


/// GLOBALS

using namespace sf;
TextureManager* TextureManager::m_instanceSingleton = nullptr;
std::string stdMap = "data/aztec.txt";


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


// (++)
bool testFileManager(int debugLevel=1)
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

    if(!fmgr.verifyFile(stdMap, nullptr, nullptr)) {
        result = false;
    }

    if(debugLevel >0) {
        std::cout << "\n\n*** Testing reading files ***\n";
    }


    HurkaMap hmap = fmgr.readRegularFile(stdMap);
    if(hmap.mapName== "empty") {
        result = false;
    }

    return result;
}


// (-+)
bool testList(int debugLevel=0)
{

    bool result = true;

    if(debugLevel >0) {
        std::cout << "\n\n*** testList()\n";
    }

    std::list<Block> blockList;

    Block house001 ({0,1}, "HOUSE001");
    blockList.push_back(house001);

    if(debugLevel >0) {
        std::cout << "    Texturename of first block: " << house001.getTextureName() << "\n";
    }

    Block tree001( {0,0}, "TREE001");
    blockList.push_back(tree001);

    if(debugLevel >0) {
        std::cout << "    Texturename of second block: " << tree001.getTextureName() << "\n\n";
        std::cout << "Blocklist content: \n{\n";
    }

    // iterate over all items
    int n = 0;
    for (std::list<Block>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
    {
        if(n == 0) {
            if((*itAll).getTextureName() != "HOUSE001") {
                result = false;
            }

        }

        if(n == 1) {
            if((*itAll).getTextureName() != "TREE001") {
                result = false;
            }

        }

        if(debugLevel >0) {
            std::cout << "    [" << n << "] texturename of current block: " << (*itAll).getTextureName() << "\n";
        }
        n++;
    }

    if(debugLevel >0) {
        std::cout << "}\n end of Testlist \n\n";
    }

    return result;

}

/// ///////////////////////////////////////////////////////////////
/// Start


// (--)
// TODO: weeeeeeeeeeeeelll this is a big skeleton as of now
//       maybe have modes?
//      being in the menu?
//      being in the game, being in settings dialog
// Complete overhall?
///PAPER AND PEN



bool integrityTesting()
{


    if(!testFileManager()) {
        std::cout << "ERROR: testFileManager failed. Something went wrong during integrity test of software!";
        waitForInput();
        return false;
    }

    if(!testList()) {
        std::cout << "ERROR: testList failed. Something went wrong during integrity test of software!";
        waitForInput();
        return false;
    }


    return true;


}







int main()
{

    // Setup Window
    // , Style::Fullscreen);
    RenderWindow window(sf::VideoMode(800, 600), "Hurkalumo Editor 0.1-alpha");

    window.setFramerateLimit(10);


    // Nr of Cells on the grid
    int GAME_WIDTH = 64;
    int GAME_HEIGHT = 64;


    // What to draw
    bool drawGm = true;
    bool drawLoco = true;
    bool drawToolbar = false;
    bool drawGrid = false;
    bool drawBlocks = true;

    // Setup objects
    TextureManager *textureMgr;
    textureMgr = textureMgr->getInstance();
    textureMgr->loadTextures();


    GameMatrix gm({GAME_HEIGHT,GAME_WIDTH,1});          /// high level structure of game
    Locomotive loco({10.0f, 10.0f});                    /// sample locomotive
    Toolbar toolbarTop({260.0f, 0.0f});
    Grid grid(GAME_HEIGHT, GAME_WIDTH);

    FileManager fmgr;                                   /// Used to read the .txt file for now







    /// Read the map

    HurkaMap hmap = fmgr.readRegularFile(stdMap);

    if(hmap.mapName == "empty") {
        return 0;
    }




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

        if(drawBlocks) {

            /// Iterate list of blocklists to draw them in renderorder

            hmap.draw(window);



        }


        if(drawLoco) {  loco.draw(window); }

        if(drawToolbar) {   toolbarTop.draw(window); }

        window.display();


    }

    return 0;
}
