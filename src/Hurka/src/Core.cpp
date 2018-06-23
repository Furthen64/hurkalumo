#include "Core.hpp"

// Design: a Core instance should only exist in Main.cpp

Core::Core()
{
}


Core::~Core()
{
    clearResources();
}


/// \brief High level function for booting up the editor
// (-+)
int Core::boot()
{

    if(runRegressionTestAndExit) {

        std::cout << "Running Regression tests on all classes ****\n{\n";
        HRect *hrect = new HRect();

        hrect->testFunctions();
        std::cout << "\nExiting runRegressionTest.\n}\n";
        return 0;

    }



    int result = 0;

    std::cout << "\n\n\n---------------booting-------------------\n";


    result = allocateResources();
    if(result != 0) {
        std::cout << "\n\n*** Exiting with error.\n"; return result;
    }


    result = loadResources(startmapFilename);
    if(result != 0) {
        std::cout << "\n\n*** Exiting with error.\n"; return result;
    }


    result = setup(800,600, "Hurkalumo Editor 0.1-alpha");
    if(result != 0) {
        std::cout << "\n\n*** Exiting with error.\n"; return result;
    }



    run();

    return result;
}

// (-+)
int Core::allocateResources()
{

    std::cout << "\n\n\n---------------allocateResources---------\n";


    /// Load the Textures
    if(debugLevel>=1) { std::cout << "Loading all the textures...\n"; }
    textureMgr = textureMgr->getInstance();

    if(textureMgr->loadTextures() != 0) {
        std::cout << "Core quitting. Could not load textures.\n";
        return -1;
    }
    if(debugLevel >=1) { std::cout << " " << textureMgr->nrOfTextures() << " textures loaded!\n"; }


    viewHPos = new HPos();
    viewHPos->gpix_y = startViewPosY;
    viewHPos->gpix_x = startViewPosX;




    /// Allocate all our Managers and Objects

    fmgr = new FileManager();

    trafficMgr = new TrafficManager();

    gm = new GameMatrix({NR_GRIDS_HEIGHT,NR_GRIDS_WIDTH,1});          /// high level structure of game

    loco = new Locomotive();

    toolbarTop = new Toolbar(new HPos(0, 600, USE_GPIX));


    grid = new Grid(NR_GRIDS_HEIGHT, NR_GRIDS_WIDTH);   // Grid overlayed on the GameMatrix to see where the isometric tiles are



    return 0;
}



// Returns 0 when ok,
// Return -1 when something failed
// (-+)
int Core::loadResources(std::string _mapName)
{

    std::cout << "\n\n\n---------------loadResources-------------\n";


    /// Read the map
    std::cout << "Loading map \"" << _mapName << "\"\n";

    hmap = fmgr->readRegularFile(getFullUri(_mapName),debugLevel, gm);
    if(hmap->fullUriMapName == "empty") { std::cout << "ERROR Could not read map " << _mapName << ", exiting!\n"; return -1;  }


    /// Get the roads

    roadMatrix = hmap->getRoadHMatrix();
    if(debugLevel > 0) {
        roadMatrix->dump();
    }

    /// Load font
    if (!font.loadFromFile(getFullUri("data\\fonts\\consola.ttf")))
    { std::cout << "ERROR " << cn << " could not load font consola.ttf.\n"; return -1; }

    return 0;
}



/// Setup - The idea is to use whatever resources that have been loaded and
/// transform them, set them up for running the editor/game.
// (-+)
int Core::setup(int width, int height, std::string title)
{
    int status = 0;

    std::cout << "\n\n\n---------------setup--------------------\n";

    initRandomizer();




    /// Parse the current situation of roads into individual road networks

    if(debugLevel >=1) {    std::cout << "\n\nParsing current Roads....\n"; }

    status = trafficMgr->parseCurrentRoads(roadMatrix, 0);

    if(debugLevel >=1) {        std::cout << "                          complete!\n"; }

    if(debugLevel >=1) {
        std::cout << "\n\nDumping the individual road networks found:\n";
        trafficMgr->dumpRoadNetworks("   ", false);
    }





    /// Place buses on all the RoadNetworks
    int maxnr = trafficMgr->nrOfRoadnetworks();
    Bus *bus;
    for(int nr = 0; nr < maxnr; nr++) {

        bus = new Bus();
        trafficMgr->addBus(bus, nr);
    }



    /// Plan a route for a Bus on a roadnetwork
    status = trafficMgr->planForBusesOnRoadNetwork(debugLevel, dijkstraFromRoad, dijkstraToRoad, dijkstraAutoEndpointsAdjust);
    if(status != 0) { return status; }



    return 0;
}






/// Run - The main loop for the editor/game

//
//  NO Multithreading,
//  Broken Input handling
//  Welcome to Alpha, guys

// (--+)
void Core::run()
{



    std::string ind1 = "   ";
    std::string ind2 = "      ";
    std::string ind3 = "         ";
    std::string ind4 = "            ";






    // Input Control (should be own class)
    bool alreadyButtonPressed = false;
    int inputCooldown = 0;

    //int inputCooldownCyclesPaused = 256000; // how many cycles for input cooldown (cycles goes faster when its got nottin to do, so we need more steps!)
    int inputCooldownCyclesPaused = 10; // how many cycles for input cooldown (cycles goes faster when its got nottin to do, so we need more steps!)

    int inputCooldownCyclesEditor = 10;  // how many cycles for input cooldown
    bool inputCooldownActive = false;


    // Main Window
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "HurkaLumo editor 0.1-alpha");

    if(lockFPS) {
        window.setFramerateLimit(lockFPS_n);
    }


    // Check OPENGL for old versions or something off

    ContextSettings settings = window.getSettings();

    std::cout << "OpenGL: " << settings.majorVersion << " - " << settings.minorVersion << "\n";

    std::cout << "\n\n\n---------------run--------------------\n";

    if(!window.isOpen()) {
        std::cout << "ERROR " << cn << " sf::window is not open!\n";
        return ;
    }













    /// //////////////////////////////////////////////////////////////////////////////
    /// Main Loop
    ///




    while (window.isOpen())
    {




        /// Get events

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }



        /// Spacebar pressed

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !inputCooldownActive) {

            if(gamemode == GAMEMODE_EDITOR) {
                gamemode = GAMEMODE_PAUSE;
                std::cout << "\n\n       ****   PAUSED    ***** \n\n\n";
            } else {
                gamemode = GAMEMODE_EDITOR;
                std::cout << "\n\n       ****   RESUMED   ***** \n\n\n";
            }


            inputCooldownActive = true;
        }



        if(inputCooldownActive) {
            inputCooldown++;
        }


        if(gamemode == GAMEMODE_EDITOR) {
            if(inputCooldown >= inputCooldownCyclesEditor) {
                // RESET
                inputCooldown = 0;
                inputCooldownActive = false;
            }
        }

        if(gamemode == GAMEMODE_PAUSE) {
                //std::cout << inputCooldown << " vs " << inputCooldownCyclesPaused << "\n";
            if(inputCooldown >= inputCooldownCyclesPaused) {
                inputCooldown = 0;
                inputCooldownActive = false;
            }

        }









        ///
        /// RMB
        ///




        /// Right mouse button pressed - Pan the map                            even in paused mode

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {



            // CR - Break out all this panning stuff

            bool rightof = false;
            bool belowof = false;
            bool topof = false;
            bool leftof = false;

            unsigned int relativeX = 0;
            unsigned int relativeY = 0;


            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window );     // SFML Specific...
            HPos mousepos = HPos(mousePos_i.y, mousePos_i.x, USE_GPIX);



            /// Do not allow us to click outside the resolution of the window

            if( mousepos.gpix_x > SCREEN_WIDTH ) {
                    mousepos.gpix_x = SCREEN_WIDTH;
            }

            if( mousepos.gpix_y > SCREEN_HEIGHT ) {
                mousepos.gpix_y = SCREEN_HEIGHT;
            }

            if(mousepos.gpix_y < 0) {
                mousepos.gpix_y = 0;
            }

            if(mousepos.gpix_x < 0) {
                mousepos.gpix_x = 0;
            }




            /// Where in relation to the Center point are we clicking?
            /// Above? Below?

            if(mousepos.gpix_y < ceil(SCREEN_HEIGHT/2))
            {
                topof = true;
            } else if(mousepos.gpix_y > ceil(SCREEN_HEIGHT/2))
            {
                topof = false;
                belowof = true;
            } else {
                topof = false;
                belowof = false;
            }


            // Left of? Right of?
            if(mousepos.gpix_x < ceil(SCREEN_WIDTH/2)) {
                leftof = true;
            } else if(mousepos.gpix_x > ceil(SCREEN_WIDTH/2)) {
                leftof = false;
                rightof = true;
            } else {
                leftof = false;
                rightof = false;
            }


            // the nr of pixels between the click and the center position of screen

            if( ceil(SCREEN_WIDTH/2) > mousepos.gpix_x) {
                relativeX = ceil(SCREEN_WIDTH/2) - mousepos.gpix_x;
            } else {
                relativeX = mousepos.gpix_x - ceil(SCREEN_WIDTH/2);
            }


            if( ceil(SCREEN_HEIGHT/2) > mousepos.gpix_y) {
                relativeY = ceil(SCREEN_HEIGHT/2) - mousepos.gpix_y;
            } else {
                relativeY = mousepos.gpix_y - ceil(SCREEN_HEIGHT/2);
            }



            // Adjust speed of panning
            // that 120, 80 is adjustment for the ratio of screen width and height

            relativeX =   ( (float) relativeX * mouseSensitivity/120 );
            relativeY =   ( (float) relativeY * mouseSensitivity/80 );



            /// Add or remove from the x- and y-offset because of the clicking

            if(rightof && topof) {
                viewHPos->gpix_x -= relativeX;
                viewHPos->gpix_y += relativeY;
            } else if(rightof && belowof) {
                viewHPos->gpix_x -= relativeX;
                viewHPos->gpix_y -= relativeY;
            } else if(leftof && belowof) {
                viewHPos->gpix_x += relativeX;
                viewHPos->gpix_y -= relativeY;
            } else if(leftof && topof) {
                viewHPos->gpix_x += relativeX;
                viewHPos->gpix_y += relativeY;
            } else {

            }

            if(debugLevel > 1)  {
                std::cout << ind1 << " VIEWPOS x=" << viewHPos->gpix_x << ", y=" << viewHPos->gpix_y << "    CLICKEDPOS x=" << mousePos_i.x << ", y=" << mousePos_i.y << "\n";
            }

        }









        ///
        ///     LMB
        ///

        // Did you let go of LMB?
        if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            alreadyButtonPressed = false;
        }




    // CR7 - For now trying out the "findGrid" function             //2018-06-02



        /// Left mouse button pressed                                                       even in paused
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !alreadyButtonPressed)
        {

            alreadyButtonPressed = true;


            /// Get mouse position
            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window );             // SFML Specific
            HPos *mousepos = new HPos(mousePos_i.y, mousePos_i.x, USE_GPIX);

            // Redact ViewPosition rectangle from it in order to get to GameMatrix positioning
            mousepos->gpix_y -= viewHPos->gpix_y;
            mousepos->gpix_x -= viewHPos->gpix_x;



            // Is it inside the gamematrix?
            if( gm->isPosInsideGameMatrix(mousepos) == false) {

                // Do nothing

            } else {



                // Find out what iso tile you clicked on



                std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n2018-06-19 debugging:\n";



                window.clear({0, 0, 0});
                gm->draw(window, viewHPos);
                mousepos = grid->findTile(gm->getHRect(), mousepos, "   ", window, viewHPos);
                changedCanvas = true;

                if(mousepos != nullptr) {


                    /// LMB Action!

                    // When you click on the left mousebutton many things can happen throughout different modes,
                    // alpha-0.2: Might need to have more consideration to MODES.

                    switch(lmbmode)
                    {
                        case LMB_CLICK_CREATE_OR_SWAP:

                            grid->setVisible(mousepos);     // Light up the current tile

                            hmap->placeNewOrSwapRoad(mousepos, debugLevel);     // Place new road or Change existing

                            break;

                        case LMB_CLICK_CREATE:

                            grid->setVisible(mousepos);

                            hmap->placeNewRoad(mousepos, debugLevel);     // Place new road ONLY if there is a blank space there

                            break;

                        case LMB_ENQUIRE:
                            grid->setVisible(mousepos);

                            // Find out what's under the cursor
                            hmap->dumpEverythingAtPos(mousepos, trafficMgr,  ind1);

                            break;

                        case LMB_PANNING:
                            break;
                    }

                } else {
                    //std::cout << cn << "Mousepos is nullptr\n";
                }




            } // if inside the gamematrix when clicking LMB
        }





        if(gamemode != GAMEMODE_PAUSE) {


            /// Trains

            /// Buses


            // FIXME enable this line of code again: trafficMgr->updateAll(viewHPos);



        }






        /// Render




        //if(drawGrid) {  grid->draw(window, viewHPos); }

        /*

        if(drawBlocks) {

            /// Iterate list of blocklists to draw them in render order
            hmap->draw(window, viewHPos);
        }


        if(drawLoco) {  loco->draw(window, viewHPos); }

        if(drawBuses) {
            trafficMgr->drawBuses(window, viewHPos);
        }




        if(drawToolbar) {   toolbarTop->draw(window, viewHPos); }




        window.draw(lastClickedText);


*/



    // Render what has been drawn to buffer
    if(changedCanvas) {
         window.display();      // PERFORMANCE ISSUE when we get to many thousands of Blocks            Time: 5610 ms
                                // WILL DRAW A BLANK SCREEN if you forgot to paint something to canvas this frame around
                                // so I added variable changedCanvas (see CR#7)
         changedCanvas = false;
    }





    } // while window is open


}


// (--)
void Core::reset()
{
    std::cout << "\n\n\n---------------** RESET **-------------------\n";
    // Clear resources
    // Boot game
    // Allocate
    // Load
    // Setup
    // Run
}



// Not sure how I want this to work
// And having issues with SFML doing weird error messsages.... I disable all delete (https://en.sfml-dev.org/forums/index.php?topic=10247.0)
void Core::clearResources()
{
  /*
    delete viewHPos;
    delete fmgr;
    trafficMgr->clearResources();
    delete trafficMgr;
    delete gm;
    delete loco;
    delete toolbarTop;
    delete grid;
  */

}






