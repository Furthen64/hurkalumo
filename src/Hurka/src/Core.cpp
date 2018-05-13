#include "Core.hpp"


Core::Core()
{
}


Core::~Core()
{
    clearResources();
}


// (-+)
int Core::boot()
{
    int result = 0;

    std::cout << "\n\n\n---------------booting-------------------\n";


    result = allocateResources();
    if(result != 0) {
        std::cout << "\n\n*** Exiting.\n"; return result;
    }


    result = loadResources(startmapStr);
    if(result != 0) {
        std::cout << "\n\n*** Exiting.\n"; return result;
    }


    result = setup(800,600, "Hurkalumo Editor 0.1-alpha");
    if(result != 0) {
        std::cout << "\n\n*** Exiting.\n"; return result;
    }



    run();

    return result;
}

// (--)
int Core::allocateResources()
{

    std::cout << "\n\n\n---------------allocateResources-------------------\n";


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

    bus = new Bus(new HPos(0,0, USE_GPIX));

    loco = new Locomotive();



    toolbarTop = new Toolbar(new HPos(0, 260, USE_GPIX));


    grid = new Grid(NR_GRIDS_HEIGHT, NR_GRIDS_WIDTH);



    return 0;
}




// Returns 0 when ok,
// Return -1 when something failed
// (--)
int Core::loadResources(std::string mapName)
{

    std::cout << "\n\n\n---------------loadResources-------------------\n";




    /// Read the map
    std::cout << "Loading map \"" << mapName << "\"\n";

    hmap = fmgr->readRegularFile(mapName,debugLevel);
    if(hmap->mapName == "empty") { std::cout << "ERROR Could not read map " << mapName << ", exiting!\n"; return -1;  }


    /// Get the roads

    roadMatrix = hmap->getRoadMatrix();
    if(debugLevel > 0) {
        roadMatrix->dump();
    }

    /// Load font
    if (!font.loadFromFile("consola.ttf"))
    {
        std::cout << "ERROR " << cn << " could not load font consola.ttf.\n";
        return -1;
    }

    return 0;
}



/// Setup - The idea is to use whatever resources that have been loaded and
/// transform them, set them up for running the editor/game.
/// (-+)
int Core::setup(int width, int height, std::string title)
{

    std::cout << "\n\n\n---------------setup-------------------\n";

    initRandomizer();




    /// Parse the current situation of roads into individual road networks

    if(debugLevel >=1) {    std::cout << "\n\nParsing current Roads\n"; }

    trafficMgr->parseCurrentRoads(roadMatrix, 0);

    if(debugLevel >=2) {
        std::cout << "\n\nDumping the individual road networks found:\n";
        trafficMgr->dumpRoadNetworks("   ");
    }





    /// Place a bus on a roadnetwork
    trafficMgr->addBus(bus, 0);




    /// Plan a route for a Bus on a roadnetwork
    trafficMgr->planForBusesOnRoadNetwork(debugLevel);


    return 0;

}


/// Run - The main loop for the editor/game
/// Wishlist: Add modes: like intro_menu and exiting_menu and main_loop or something
// (-+)
void Core::run()
{
    RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "HurkaLumo editor 0.06-alpha");
    if(lockFPS) {
        window.setFramerateLimit(lockFPS_n);
    }

    std::cout << "\n\n\n---------------run-------------------\n";

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



        /// Update


        // Right mouse button pressed - Pan the map
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {

            bool rightof = false;
            bool belowof = false;
            bool topof = false;
            bool leftof = false;

            unsigned int relativeX = 0;
            unsigned int relativeY = 0;


            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window ); // SFML Specific...
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
            // TODO FIXME maybe adjust for the ratio of screen width and height you knooow?

            relativeX =   ( (float) relativeX * mouseSensitivity/100 );
            relativeY =   ( (float) relativeY * mouseSensitivity/100 );



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
                std::cout << " VIEWPOS x=" << viewHPos->gpix_x << ", y=" << viewHPos->gpix_y << "    CLICKEDPOS x=" << mousePos_i.x << ", y=" << mousePos_i.y << "\n";
            }

        }

        // Left mouse button pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            /*

            /// Dump the Bus position
            bus->dump(viewHPos);
            std::stringstream sstm;


            /// Get mouse position
            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window ); // SFML Specific....
            HPos mousepos = HPos(mousePos_i.y, mousePos_i.x, USE_GPIX);




            Vector2f mouseWPos = Vector2f(); // SFML specific...
            mouseWPos.y = mousePos_i.y;
            mouseWPos.x = mousePos_i.x;

            sstm << "WPOS(" << mousepos.gpix_y<< ", " << mousepos.gpix_x << ")\n";



            /// Redact ViewPosition rectangle from it in order to get to GameMatrix positioning

            mousepos.gpix_y -= viewHPos->gpix_y;
            mousepos.gpix_x -= viewHPos->gpix_x;


            sstm << "GPOS(" << mousepos.gpix_y << ", " << mousepos.gpix_x << ")\n";



            HPos *visibleIsoPos = Grid::convert_gpix_to_iso(&mousepos, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT);



            /// Update the text
            lastClickedText.setFont(font);
            lastClickedText.setString(sstm.str());
            lastClickedText.setCharacterSize(18);
            lastClickedText.setFillColor(sf::Color::White);

            lastClickedText.setPosition(mouseWPos);



            /// Light up the current tile
            grid->setVisible(visibleIsoPos);

            if(debugLevel > 1)  {
                std::cout << " VIEWPOS x=" << viewHPos->gpix_x << ", y=" << viewHPos->gpix_y << "    CLICKEDPOS x=" << mousePos_i.x << ", y=" << mousePos_i.y << "\n";

                //dumpPosition(iso_pos);// HPOSADAPT
            }


            /// Pushing button on top toolbar

            // if (toolbarTop.within(mousePos_i.x, mousepPos_i.y) { pushbutton(x,y));
            toolbarTop->pushButton(0); // debug test
*/



        }


        /// Trains



        /// Buses


        trafficMgr->updateAll(viewHPos);


        //grid->setVisible(bus->get_next_iso_pos());

        HPos *busNowPos = bus->getNowPos();

        if(busNowPos != nullptr) {
            grid->setVisible(busNowPos);
        }





















        /// Render

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

        window.draw(lastClickedText);

        /// Finally push rendered buffer to screen

        window.display();



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


}






