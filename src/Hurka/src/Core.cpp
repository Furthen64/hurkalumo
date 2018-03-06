#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <chrono>


// Project includes
#include "Utils.hpp"
#include "Grid.hpp"
#include "TextureManager.hpp"
#include "Toolbar.hpp"
#include "GameMatrix.hpp"
#include "Block.hpp"
#include "Locomotive.hpp"
#include "FileManager.hpp"
#include "HurkaMap.hpp"
#include "Bus.hpp"


#include "Constants.hpp"



// Always push RUNNING code
// If it doesn't run, just stub out the errors and make it run before pushing to a branch
// If that is too much to do, make a BRANCH!






/// GLOBALS

using namespace sf;
TextureManager* TextureManager::m_instanceSingleton = nullptr;
std::string stdMap = "data/bustest.txt";


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


    HurkaMap *hmap = fmgr.readRegularFile(stdMap);

    if(hmap->mapName== "empty") {
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
// Complete o?
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


// For now it just moves them around randomly
// TODO: needs a matrix of the ROADS so we can figure out the way to move it

// (--)
void updateBuses(Bus *bus, float dt,  int **matrix )
{

    if(matrix == nullptr) {
        std::cout << "ERROR cannot read the road matrix\n";
        return ;
    }


     int dt_i = (int)dt;


     Vector2f np = Vector2f();

     if(dt_i%2==0) {
         // Change direction

        np.x = 200;
        np.y = 200;

     } else {


        np.x = 20;
        np.y = 20;

     }






    // Figure out where the next position is in relation to current position

    bool rightof = false;
    bool belowof = false;
    bool topof = false;
    bool leftof = false;



    Vector2f dir;

            // Figure out what +-1 to do with the position
            if(bus->getPos().x < np.x) {
                rightof = true;
            }
            if(bus->getPos().x >= np.x) {
                leftof = true;
            }
            if(bus->getPos().y < np.y) {
                belowof = true;
            }
            if(bus->getPos().y >= np.y) {
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



            // Old way
            bus->setDirection(dir);

            // My new way
            bus->setNextPos(np);







    bus->update(dt);

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
    bool drawGrid = true;
    bool drawBlocks = true;

    // Setup objects
    TextureManager *textureMgr;
    textureMgr = textureMgr->getInstance();
    textureMgr->loadTextures();


    GameMatrix gm({GAME_HEIGHT,GAME_WIDTH,1});          /// high level structure of game
    Bus bus({10.0f, 10.0f});
    Locomotive loco({10.0f, 10.0f});
    Toolbar toolbarTop({260.0f, 0.0f});
    Grid grid(GAME_HEIGHT, GAME_WIDTH);

    FileManager fmgr;                                   /// Used to read the .txt file for now






    /// Read the map

    HurkaMap *hmap = fmgr.readRegularFile(stdMap);
    if(hmap->mapName == "empty") { std::cout << "ERROR Could not read the map, exiting!\n"; return 0;  }


    /// Get the roads

    int **roadMatrix = allocateMatrix(hmap->getRows(), hmap->getCols());
    if(roadMatrix == nullptr) { std::cout << "ERROR Something horrible has happened, exiting!\n "; return 0; }

    int roadMatrixRows = hmap->getRows();
    int roadMatrixCols = hmap->getCols();

    roadMatrix = hmap->getRoadMatrix();





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


        /// Update Busses

        updateBuses(&bus, dt, roadMatrix);


        //toolbarTop.update(); TODO should I do this?




        /// Render

        window.clear({0, 0, 0});


        // Draw the game board

        if(drawGm)   {  gm.draw(window);  } // Draws the ground and water and suchers



        if(drawBlocks) {

            /// Iterate list of blocklists to draw them in renderorder

            hmap->draw(window);

        }


        if(drawLoco) {  loco.draw(window); }

        bus.draw(window);


        if(drawGrid) {  grid.draw(window); }

        if(drawToolbar) {   toolbarTop.draw(window); }

        window.display();


    }


    // Delete matrixiseseses
    if(roadMatrixRows>0) {

        if (roadMatrixRows>0) delete [] roadMatrix[0];
        delete [] roadMatrix;
    }

    return 0;
}
