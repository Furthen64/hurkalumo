#include "Core.hpp"


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
    int result = 0;

    std::cout << "\n\n\n---------------booting-------------------\n";


    result = allocateResources();
    if(result != 0) {
        std::cout << "\n\n*** Exiting with error.\n"; return result;
    }


    result = loadResources(startmapStr);
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
    textureMgr->loadTextures();
    if(debugLevel >=1) { std::cout << " " << textureMgr->nrOfTextures() << " textures loaded!\n"; }


    viewHPos = new HPos();
    viewHPos->gpix_y = startViewPosY;
    viewHPos->gpix_x = startViewPosX;



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
int Core::loadResources(std::string mapName)
{

    std::cout << "\n\n\n---------------loadResources-------------\n";




    /// Read the map
    std::cout << "Loading map \"" << mapName << "\"\n";

    hmap = fmgr->readRegularFile(mapName,debugLevel, gm);
    if(hmap->mapName == "empty") { std::cout << "ERROR Could not read map " << mapName << ", exiting!\n"; return -1;  }


    /// Get the roads

    roadMatrix = hmap->getRoadHMatrix();
    if(debugLevel > 0) {
        roadMatrix->dump();
    }

    /// Load font
    if (!font.loadFromFile("consola.ttf"))
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



    int maxnr = trafficMgr->nrOfRoadnetworks();

    Bus *bus;

    for(int nr = 0; nr < maxnr; nr++) {
        /// Place a bus on a roadnetwork
        bus = new Bus();
        trafficMgr->addBus(bus, nr);
    }



    /// Plan a route for a Bus on a roadnetwork
    status = trafficMgr->planForBusesOnRoadNetwork(debugLevel, dijkstraFromRoad, dijkstraToRoad, dijkstraAutoEndpointsAdjust);
    if(status != 0) { return status; }



    return 0;
}






/// Run - The main loop for the editor/game
// For now it does the very basics, has some sense of Modes
// (--+)
void Core::run()
{



    HRect *rect = new HRect(0,0,10,10, 32, 64);

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

std::cout << "Hm...\n";
    RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "HurkaLumo editor 0.1-alpha");
    std::cout << "Hm...!\n";

    if(lockFPS) {
        window.setFramerateLimit(lockFPS_n);
    }

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



        // Did you let go of LMB?
        if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            alreadyButtonPressed = false;
        }



        ///
        ///     LMB
        ///


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




                mousepos = grid->findTile(gm->getHRect(), mousepos, "   ");







                //replacing (DELETE:):
                        //mousepos = Grid::convert_gpix_to_iso(mousepos, GRID_TEXTURE_HEIGHT, GRID_TEXTURE_WIDTH);




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


            } // if inside the gamematrix when clicking LMB

/*
            if (toolbarTop->within(mousePos_i.x, mousepPos_i.y) {
                toolbarTop->pushButton(0); // debug test
            }



            std::stringstream sstm;


            sstm << "WPOS(" << mousepos.gpix_y<< ", " << mousepos.gpix_x << ")\n";
            sstm << "GPOS(" << mousepos.gpix_y << ", " << mousepos.gpix_x << ")\n";

            HPos *visibleIsoPos = Grid::convert_gpix_to_iso(&mousepos, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT);

            /// Update the text
            lastClickedText.setFont(font);
            lastClickedText.setString(sstm.str());
            lastClickedText.setCharacterSize(18);
            lastClickedText.setFillColor(sf::Color::White);

            lastClickedText.setPosition(mouseWPos);

            if(debugLevel > 1)  {
                std::cout << " VIEWPOS x=" << viewHPos->gpix_x << ", y=" << viewHPos->gpix_y << "    CLICKEDPOS x=" << mousePos_i.x << ", y=" << mousePos_i.y << "\n";

                //dumpPosition(iso_pos);// HPOSADAPT
            }


            /// Pushing button on top toolbar

            // if (toolbarTop.within(mousePos_i.x, mousepPos_i.y) { pushbutton(x,y));
            toolbarTop->pushButton(0); // debug test

*/

        }





        if(gamemode != GAMEMODE_PAUSE) {


            /// Trains



            /// Buses



            trafficMgr->updateAll(viewHPos);


            /// Render

        }



        window.clear({0, 0, 0});






        if(drawGm)   {  gm->draw(window, viewHPos);  } // Draws the ground and water and suchers

        if(drawBlocks) {

            /// Iterate list of blocklists to draw them in render order
            hmap->draw(window, viewHPos);
        }


        if(drawLoco) {  loco->draw(window, viewHPos); }

        if(drawBuses) {
                trafficMgr->drawBuses(window, viewHPos);
                //bus->draw(window, viewHPos); DELETE
        }


        if(drawGrid) {  grid->draw(window, viewHPos); }

        if(drawToolbar) {   toolbarTop->draw(window, viewHPos); }


        rect->draw(window, viewHPos);

        window.draw(lastClickedText);


        // Finally push rendered buffer to screen


        window.display();       // PERFORMANCE ISSUE when we get to many thousands of Blocks            Time: 5610 ms





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

void Core::clearResources()
{
    std::cout << "\n\nclearResources()\n\n";

    /// Load the Textures


    //textureMgr->clearResources();

    delete viewHPos;
    delete fmgr;
    trafficMgr->clearResources();
    delete trafficMgr;
    delete gm;
    delete loco;
    delete toolbarTop;
    delete grid;


}






