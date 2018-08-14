#include "Core.hpp"


Core::Core()
{
}


Core::~Core()
{
    clearResources();
}


/// \brief High level function for starting up and running the editor/game
// Makes a Run(), looks at what happened in a run and responds accordingly.
// Maybe user wanted to quit, load a new map or maybe even clear the gameboard and start fresh with an empty map.
// All of this is taken care in this lifecycle function.
//
// (--) wishlist: Needs more testing
LifecycleResult *Core::lifecycle()
{

    // Alpha-0.2: Might need a better state machine than these loosy goosy variables to control the thing
    LifecycleResult *lifecycleResult = new LifecycleResult();
    bool lifecycleActive = true;
    RunResult *runResult = nullptr;
    std::string loadNextMapFullUri = "";
    int retResult;


    //
    /// Lifecycle loop
    //

    while(lifecycleActive)
    {

        std::cout << "\n\n\n\n\n\n\n\n\n";
        std::cout << "core lifecycle() ------------------------------------------\n";


        ///
        /// Regression testing
        ///
        /// Let's do unit testing of all the classes and see if hurkalumo's base is stable

        if(runRegressionTestAndExit) {




            std::cout << "  *** Running Regression tests on all classes ***\n{\n";


            std::cout << "\n\nUNIT TEST    \"HPos\" \n";
            {
                HPos *dummy = new HPos();
                dummy->testFunctions();
                delete dummy;


                HPos *dummy2 = new HPos();
                dummy2->testFunctions();
                delete dummy2;

            }



            std::cout << "\n\nUNIT TEST    \"HRect\" \n";
            std::cout << "------------------------------------------\n";
            /// HRect
            {
                HRect *hrect = new HRect();
                hrect->testFunctions();
            }


            /// HurkaMap and RoadNetwork
            std::cout << "\n\nUNIT TEST    \"HurkaMap and RoadNetwork\" \n";
            std::cout << "------------------------------------------\n";
            {
                debugLevel = 1;
                int retStatus = -1;
                std::string roadnetMap = "data\\maps\\roadnetwork_test.txt";

                enableFallbackContext();


                // Load a specific test map for the RoadNetwork class

                TextureManager *txtmgr;
                txtmgr= txtmgr->getInstance();
                retStatus = txtmgr->loadTextures();

                GameMatrix *gamematrix = new GameMatrix(64,64);

                assert(retStatus == 0);

                HurkaMap *hmap = fm->readRegularFile(getFullUri(roadnetMap),debugLevel, gamematrix);

                HurkaMatrix *hmatrix = hmap->getRoadHMatrix();

                assert(hmap != nullptr);

                assert(hmap->fullUriMapName != "empty");

                assert(hmatrix->rows < 10000);

                assert(hmatrix->cols < 10000);

                TrafficManager *tmgr = new TrafficManager();

                retStatus = tmgr->readRoadNetworksFromHMatrix(hmatrix, debugLevel);

                assert(retStatus == 0);


                // Now do spot checks where we assume there should be a road (1:s)
                assert(tmgr->nrOfRoadnetworks() >0);

                RoadNetwork *largestRn = tmgr->roadNetworkAtPos(new HPos(1,1, USE_ISO));      // At (1,1) we should find the largest roadnetwork
                RoadNetwork *smallestRn = tmgr->roadNetworkAtPos(new HPos(4,4, USE_ISO));     // At e.g. (4,4) is the other one


                assert(largestRn != nullptr);
                assert(smallestRn != nullptr);


                assert(largestRn->getMatrix()[1][1] == 0);
                assert(largestRn->getMatrix()[2][2] == 0);


                assert(largestRn->getMatrix()[0][0] == 1);
                assert(largestRn->getMatrix()[9][0] == 1);
                assert(largestRn->getMatrix()[0][6] == 1);



            }


            /// "iso -> gpix -> iso"
            std::cout << "\n\nUNIT TEST    \"iso -> gpix -> iso\"  \n";
            std::cout << "------------------------------------------\n";
            {


                // "iso -> gpix"
                HPos *isopos = new HPos(5,2, USE_ISO);

                isopos->gpix_y_middle = Grid::isoToGpixMiddleY(isopos, 0, 1);

                isopos->gpix_x_middle = Grid::isoToGpixMiddleX(isopos, 0, 1);     //Test me

                std::cout << "\n\n";
                isopos->dump(" isopos: ");

                std::cout << "\n\n";
                HPos *gpixpos = new HPos();
                gpixpos = Grid::convert_iso_to_gpix_middle(isopos);
                std::cout << "\n\n";
                gpixpos->dump(" gpix: ");




                // "gpix -> iso"
                HPos *isopos_eq = new HPos();
                RenderWindow *wPtr = new sf::RenderWindow(sf::VideoMode(320,240),"Title");
                gm = new GameMatrix(NR_GRIDS_HEIGHT, NR_GRIDS_WIDTH);
                std::cout << "gm->gethrect:\n";
                gm->getHRect()->dump("    ");

                std::cout << "Recursion call findTile:\n";
                isopos_eq = grid->findTile(gm->getHRect(),
                                           gpixpos,
                                           "   ",
                                           *wPtr,
                                           viewHPos,
                                           "");


                assert(isopos_eq != nullptr);






                // Now compare the two isopos and isopos_eq
                isopos_eq->dump(" isopos_eq: ");
                assert(isopos->compare(isopos_eq) == 0);

                wPtr->close();
            }





            std::cout << "\n\nUNIT TEST     \"gpix -> iso -> gpix\"\n";
            std::cout << "------------------------------------------\n";
            {
                // "gpix -> iso -> gpix"





            }






            lifecycleResult = new LifecycleResult(0, "OK");
            runResult = new RunResult();
            runResult->intReturn = 0;
            runResult->quitresult = RUN_RESULT_QUIT;

            allTestsSucceeded();


            std::cout << "\n *** exiting runRegressionTest.\n}\n";
            return lifecycleResult;

        }



        /// Allocate
        retResult = allocateResources();

        if(retResult != 0) {
            std::cout << "\n\n*** Exiting with error.\n";
            lifecycleResult = new LifecycleResult(-1, "allocateResources failed");
            return lifecycleResult;
        }





        /// Load Resources

        // User clicked on Load Map in previous cycle?
        if(lifecycleResult->intReturn == LF_LOAD_NEW_MAP) {
            retResult = loadResources( lifecycleResult->lfStr1);
        }

        // User clicked on New Map ?
        else if( lifecycleResult->intReturn == LF_EMPTY_MAP ) {

            retResult = loadResources( );

        } else {

            /// Default, load a starting map
            retResult = loadResources( startmapFilename);
        }
        if(retResult != 0) {
            std::cout << "\n\n*** Exiting with error.\n";
            lifecycleResult = new LifecycleResult(-1, "loadResources failed");
            return lifecycleResult;
        }




        /// Setup
        retResult = setup(1024,768, "Hurkalumo Editor 0.1-alpha");
        if(retResult != 0) {
            std::cout << "\n\n*** Exiting with error.\n";
            lifecycleResult = new LifecycleResult(-1, "setup failed");
            return lifecycleResult;
        }




        /// Startup the game / editor

        runResult = run();




        // Usecase - Quit the editor/game
        if(runResult->quitresult == RUN_RESULT_QUIT)
        {
            lifecycleActive = false;
        }

        // Usecase - User wants to load a file
        if(runResult->quitresult == RUN_RESULT_LOAD_NEW_MAP)
        {

            // Setup for next lifecycle to deal with this
            lifecycleResult->intReturn = LF_LOAD_NEW_MAP;
            lifecycleResult->lfStr1 = runResult->retStr1;         // ASSUMING it's already a full uri provided

        }

        // Usecase - User wants a new Empty Map
        if(runResult->quitresult == RUN_RESULT_NEW_MAP) {
            lifecycleResult->intReturn = LF_EMPTY_MAP;
        }




    }



    // Why did we arrive here?
    if(runResult->intReturn != -999) {


        // Check reasons




    } else {
        // We didnt even run once? Something seems off.
        std::cout << "Warning! " << cn << "core::lifecycle() why didnt we do a core::run at least once?\n";

    }




    return lifecycleResult;
}






// (-+)
int Core::allocateResources()
{

    std::cout << "\n\n\n---------------allocateResources---------\n";


    /// Load the Textures
    if(debugLevel>=1) { std::cout << "Loading all the textures...\n"; }
    textureMgr = textureMgr->getInstance();                                         // textureMgr is a singleton, this could lead to potential problems with GL_FLUSH issues that I have
                                                                                    // experienced with using the SFML library. SOmetimes Texture object calls its destructor function
                                                                                    // while not being in a gl context, even though there are windows open and running opengl.
                                                                                    // Could be that the destructor function runs inside another scope alltogether.... bah, humbug!
    if( textureMgr->nrOfTextures() <= 0 )
    {
        if(textureMgr->loadTextures() != 0) {
            std::cout << "Core quitting. Could not load textures.\n";
            return -1;
        }
        if(debugLevel >=1) { std::cout << " " << textureMgr->nrOfTextures() << " textures loaded!\n"; }
    }


    // Viewhpos is the viewport to the gamematrix,
    // if you wouldnt have it, you'd be looking at 0,0 to 1024,768 and the gamematrix starts at .. lets say 400,2500px .
    // suffice to say you would not be able to see the gamematrix
    // So.. these two values of y and x are offsets so everything gets rendered on the window canvas for you to see at somewhere around 400,2500px :)


    viewHPos = new HPos();
    viewHPos->gpix_y_topleft = startViewPosY;
    viewHPos->gpix_x_topleft = startViewPosX;




    /// Allocate all our Managers and Objects

    fm = new FileManager();                                             // Utility class for Loading and Saving maps

    trafficMgr = new TrafficManager();                                  // Controls traffic, all the roadnetworks and buses

    gm = new GameMatrix(NR_GRIDS_HEIGHT,NR_GRIDS_WIDTH);            // High level structure of the gameboard , maybe should be called that instead of gamematrix... alpha-0.2

    grid = new Grid(NR_GRIDS_HEIGHT, NR_GRIDS_WIDTH);                   // Grid is just graphics overlayed on the GameMatrix to see where the isometric tiles are, alpha-0.2 move into GameBoard?

    toolbarTop = new Toolbar(new HPos(0, 600, USE_GPIX));               // Needs to be a Gui Widget subclass or something (BETA)

    return 0;
}



// (--)
int Core::loadResources()
{
    // Start fresh, no file loading
    return loadResources("");
}




/// \brief Loads map and creates a HurkaMap object out of it, also gets roadmatrix
// Returns 0 when ok,
// Return -1 when something failed
// (-+)
int Core::loadResources(std::string _mapName)
{

    std::cout << "\n\n\n---------------loadResources-------------\n";

    bool loadFromFile = true;

    if(_mapName == "") {
        loadFromFile = false;
    }


    int retStatus = 0;


    /// Load from file into hmap object
    if(loadFromFile) {
        retStatus = loadMap(_mapName, false);

        if(retStatus != 0) {
            std::cout << "ERROR " << cn << " loadResources failed while loading map. Aborting\n";
            return -1;
        }
    } else {

        /// Start with a clean hmap object

        hmap = new HurkaMap(gm->getRows(), gm->getCols());
    }



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


// Used by LoadResources and the Toolbar when someone clicks on load map

/// \return Returns -1 when failed, 0 when OK
// (-+)
int Core::loadMap(std::string _mapName, bool fullUriProvided)
{


    std::cout << "Loading map \"" << _mapName << "\"\n";
    if(fullUriProvided) {
        hmap = fm->readRegularFile(_mapName,debugLevel, gm);
    } else {
        hmap = fm->readRegularFile(getFullUri(_mapName),debugLevel, gm);
    }

    if(hmap->fullUriMapName == "empty") { std::cout << "ERROR Could not read map " << _mapName << ", exiting!\n"; return -1;  }



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

    status = trafficMgr->readRoadNetworksFromHMatrix(roadMatrix, 0);

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








//
//  NO Multithreading,
//  Broken Input handling
//  Welcome to Alpha, guys!
// (--+)

// 2018-07-05           Fat64   CR#28       Adding some type of Lifecycle and Result classes to this thing.

// Called from core:lifecycle

/// \brief Run - The main loop for the editor/game
RunResult *Core::run()
{
    RunResult *runResult  = new RunResult();



    std::string ind1 = "   ";
    std::string ind2 = "      ";
    std::string ind3 = "         ";
    std::string ind4 = "            ";


    std::string userLoadFileStr = "\\data\\maps\\dijkstra1.txt";



    /// Prepare for Running the Main Loop

    // Input Control (should be own class)
    bool alreadyButtonPressed = false;
    int inputCooldown = 0;

    //int inputCooldownCyclesPaused = 256000; // how many cycles for input cooldown (cycles goes faster when its got nottin to do, so we need more steps!)
    int inputCooldownCyclesPaused = 10; // how many cycles for input cooldown (cycles goes faster when its got nottin to do, so we need more steps!)

    int inputCooldownCyclesEditor = 10;  // how many cycles for input cooldown
    bool inputCooldownActive = false;


    /// Main Window


    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, desktop.bitsPerPixel), "HurkaLumo editor 0.1-alpha");

    // Move the control of glContext to this window
    disableFallbackContext();
    window.setActive(true);



    if(lockFPS) {
        window.setFramerateLimit(lockFPS_n);
    }


    // Check OPENGL for old versions or something off

    if(debugLevel >=2) {
        ContextSettings settings = window.getSettings();
        std::cout << "SFML window->OpenGL version used: " << settings.majorVersion << " - " << settings.minorVersion << "\n";
    }


    if(!window.isOpen()) {
        std::cout << "ERROR " << cn << " sf::window is not open!\n";
        runResult->retStr1 = "sf::window is not open\n";
        runResult->intReturn = -1;
        return runResult;
    }

    std::cout << "\n\n\n---------------run--------------------\n";












    /// //////////////////////////////////////////////////////////////////////////////
    /// Main Loop
    ///


    std::cout << "\n\n\n---------------run--------------------\n";

    while (window.isOpen())
    {

        /// Get events

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {


                // usecase: User clicked on Close Window
                runResult->retStr1 = "User clicked on Close Window.";
                runResult->intReturn = 0;
                runResult->quitresult = RUN_RESULT_QUIT;

                window.close();
            }
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

            if( mousepos.gpix_x_topleft > SCREEN_WIDTH ) {
                    mousepos.gpix_x_topleft = SCREEN_WIDTH;
            }

            if( mousepos.gpix_y_topleft > SCREEN_HEIGHT ) {
                mousepos.gpix_y_topleft = SCREEN_HEIGHT;
            }

            if(mousepos.gpix_y_topleft < 0) {
                mousepos.gpix_y_topleft = 0;
            }

            if(mousepos.gpix_x_topleft < 0) {
                mousepos.gpix_x_topleft = 0;
            }




            /// Where in relation to the Center point are we clicking?
            /// Above? Below?

            if(mousepos.gpix_y_topleft < ceil(SCREEN_HEIGHT/2))
            {
                topof = true;
            } else if(mousepos.gpix_y_topleft > ceil(SCREEN_HEIGHT/2))
            {
                topof = false;
                belowof = true;
            } else {
                topof = false;
                belowof = false;
            }


            // Left of? Right of?
            if(mousepos.gpix_x_topleft < ceil(SCREEN_WIDTH/2)) {
                leftof = true;
            } else if(mousepos.gpix_x_topleft > ceil(SCREEN_WIDTH/2)) {
                leftof = false;
                rightof = true;
            } else {
                leftof = false;
                rightof = false;
            }


            // the nr of pixels between the click and the center position of screen

            if( ceil(SCREEN_WIDTH/2) > mousepos.gpix_x_topleft) {
                relativeX = ceil(SCREEN_WIDTH/2) - mousepos.gpix_x_topleft;
            } else {
                relativeX = mousepos.gpix_x_topleft - ceil(SCREEN_WIDTH/2);
            }


            if( ceil(SCREEN_HEIGHT/2) > mousepos.gpix_y_topleft) {
                relativeY = ceil(SCREEN_HEIGHT/2) - mousepos.gpix_y_topleft;
            } else {
                relativeY = mousepos.gpix_y_topleft - ceil(SCREEN_HEIGHT/2);
            }



            // Adjust speed of panning
            // that 120, 80 is adjustment for the ratio of screen width and height


            if(lockFPS) {
                relativeX =   ( (float) relativeX * mouseSensitivity/120 );
                relativeY =   ( (float) relativeY * mouseSensitivity/80 );

            } else {
                relativeX =   ( (float) relativeX *0.02 );
                relativeY =   ( (float) relativeY *0.05 );
            }



            /// Add or remove from the x- and y-offset because of the clicking

            if(rightof && topof) {
                viewHPos->gpix_x_topleft -= relativeX;
                viewHPos->gpix_y_topleft += relativeY;
            } else if(rightof && belowof) {
                viewHPos->gpix_x_topleft -= relativeX;
                viewHPos->gpix_y_topleft -= relativeY;
            } else if(leftof && belowof) {
                viewHPos->gpix_x_topleft += relativeX;
                viewHPos->gpix_y_topleft -= relativeY;
            } else if(leftof && topof) {
                viewHPos->gpix_x_topleft += relativeX;
                viewHPos->gpix_y_topleft += relativeY;
            } else {

            }

            if(debugLevel > 1)  {
                std::cout << ind1 << " VIEWPOS x=" << viewHPos->gpix_x_topleft << ", y=" << viewHPos->gpix_y_topleft << "    CLICKEDPOS x=" << mousePos_i.x << ", y=" << mousePos_i.y << "\n";
            }
        }









        ///
        ///     LMB
        ///

        // Did you let go of LMB?
        if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            alreadyButtonPressed = false;
            toolbarTop->resetButtons();
        }



        /// Left mouse button pressed                                                       even in paused
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !alreadyButtonPressed)
        {

            alreadyButtonPressed = true;


            // Get mouse position
            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window );
            HPos *mousepos = new HPos(mousePos_i.y, mousePos_i.x, USE_GPIX);

            // Redact ViewPosition rectangle from it in order to get to GameMatrix positioning

            mousepos->gpix_y_topleft -= viewHPos->gpix_y_topleft;
            mousepos->gpix_x_topleft -= viewHPos->gpix_x_topleft;







            // Clicked on a Toolbar Button?

            if( toolbarTop->isPosInsideToolbar(mousepos) == true )
            {
                int tbResult = toolbarTop->pushButton(mousepos);

                std::ofstream outfile;
                std::string line;


                switch(tbResult)
                {


                    case TB_NEW_FILE:
                        std::cout << "User clicked New\n";
                        runResult->intReturn = 0;
                        runResult->quitresult = RUN_RESULT_NEW_MAP;
                        window.close();

                        break;
                    case TB_LOAD_FILE:
                        {
                        std::cout << "User clicked Load File\n";
                        std::string mapNameLoad = "\\data\\maps\\_default_.txt";
                        runResult->intReturn = 0;
                        runResult->quitresult = RUN_RESULT_LOAD_NEW_MAP;
                        runResult->retStr1 = mapNameLoad;

                        window.close();

                        break;
                        }
                    case TB_SAVE_FILE:
                        {
                        std::cout << "User clicked Save File\n";
                        std::string fullUri;
                        fullUri = getFullUri(DEFAULT_FILENAME);
                        fm->saveRegularFile(fullUri, debugLevel-1, hmap, gm);
                        break;
                        }

                    case TB_EXIT:
                        // usecase: User clicked on Quit
                        runResult->retStr1 = "User clicked on Quit.";
                        runResult->intReturn = 0;
                        runResult->quitresult = RUN_RESULT_QUIT;

                        window.close();

                        break;
                    default:
                        std::cout << "Warning! Not reacting to user pushing toolbar button\n";
                        break;
                }

                if(tbResult == TB_SAVE_FILE) {

                }
            }



            // Is it inside the gamematrix?
            if( gm->isPosInsideGameMatrix(mousepos))
            {


                // Find out what iso tile you clicked on        (runs recursive function, has a limiter of how many levels deep it can go)


                mousepos = grid->findTile(gm->getHRect(), mousepos, "   ", window, viewHPos, "");

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
            trafficMgr->updateAll(viewHPos);
        }






        /// Render

        window.clear({0, 0, 0});

        if(drawGm) {gm->draw(window, viewHPos);}

        if(drawBlocks) {
            /// Iterate list of blocklists to draw them in render order
            hmap->draw(window, viewHPos);
        }

        if(drawGrid) {  grid->draw(window, viewHPos); }


        if(drawLoco) {  loco->draw(window, viewHPos); }

        if(drawBuses) {
            trafficMgr->drawBuses(window, viewHPos);
        }

        if(drawToolbar) {   toolbarTop->draw(window, viewHPos); }




        window.display();      // Performance issue when we get to many thousands of Blocks. Time: 5610 ms
                               // Note: this will draw a blank screen if you forgot to paint something to canvas this frame around, making it flicker



    } // while window is open




    // Now that the Window is closed, move over the glcontext to the global context
    enableFallbackContext();

    runResult->dump();

    return runResult;   // Go back to core::lifecycle with this information
}



// Not sure how I want this to work
// And having issues with SFML doing weird error messsages.... I disable all delete (https://en.sfml-dev.org/forums/index.php?topic=10247.0)
void Core::clearResources()
{
  /*
    delete viewHPos;
    delete fm;
    trafficMgr->clearResources();
    delete trafficMgr;
    delete gm;
    delete loco;
    delete toolbarTop;
    delete grid;
  */
}







void Core::allTestsSucceeded()
{




    std::cout <<  "   _   _ _    _            _                                     _          _   _ \n";
    std::cout <<   "  /_\\ | | |  | |_ ___  ___| |_ ___    ___ _   _  ___ ___ ___  __| | ___  __| | / \\\n",
    std::cout <<   " //_\\\\| | |  | __/ _ \\/ __| __/ __|  / __| | | |/ __/ __/ _ \\/ _` |/ _ \\/ _` |/  /\n",
    std::cout <<   "/  _  \\ | |  | ||  __/\\__ \\ |_\\__ \\  \\__ \\ |_| | (_| (_|  __/ (_| |  __/ (_| /\\_/ \n",
    std::cout <<   "\\_/ \\_/_|_|   \\__\\___||___/\\__|___/  |___/\\__,_|\\___\\___\\___|\\__,_|\\___|\\__,_\\/   \n",
    std::cout <<   "                                                                                  \n";



    /*
     _   _ _    _            _                                     _          _   _
      /_\ | | |  | |_ ___  ___| |_ ___    ___ _   _  ___ ___ ___  __| | ___  __| | / \
     //_\\| | |  | __/ _ \/ __| __/ __|  / __| | | |/ __/ __/ _ \/ _` |/ _ \/ _` |/  /
    /  _  \ | |  | ||  __/\__ \ |_\__ \  \__ \ |_| | (_| (_|  __/ (_| |  __/ (_| /\_/
    \_/ \_/_|_|   \__\___||___/\__|___/  |___/\__,_|\___\___\___|\__,_|\___|\__,_\/

    */

}
