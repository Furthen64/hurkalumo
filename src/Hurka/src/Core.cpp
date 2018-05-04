#include "Core.hpp"


Core::Core()
{
}


Core::~Core()
{
    clearResources();
}


// (--)
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
    bus = new Bus(new HPos(0,0));
    loco = new Locomotive();
    toolbarTop = new Toolbar({260.0f, 0.0f});
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

    hmap = fmgr->readRegularFile(mapName,2);
    if(hmap->mapName == "empty") { std::cout << "ERROR Could not read map " << mapName << ", exiting!\n"; return -1;  }


    /// Get the roads

    roadMatrix = hmap->getRoadMatrix();
    if(debugLevel > 0) {
        roadMatrix->dump();
    }

    /// Load font
    if (!font.loadFromFile("consola.ttf"))
    {
        std::cout << "ERROR " << cn << " could not load font.\n";
    }

    return 0;
}



// (-+)
int Core::setup(int width, int height, std::string title)
{

    std::cout << "\n\n\n---------------setup-------------------\n";

    initRandomizer();




    /// Parse the current situation of roads into individual road networks

    if(debugLevel >=1) {    std::cout << "\n\nParsing current Roads\n"; }

    trafficMgr->parseCurrentRoads(roadMatrix, 0);

    if(debugLevel >=1) {
        std::cout << "\n\nDumping the individual road networks found:\n";
        trafficMgr->dumpRoadNetworks("   ");
    }





    /// Place a bus on a roadnetwork
    trafficMgr->addBus(bus, 0);


    return 0;

}




// (-+)
void Core::run()
{
    RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "HurkaLumo editor 0.05-alpha");
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

            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window );




            /// Do not allow us to click outside the resolution of the window

            if( mousePos_i.x > SCREEN_WIDTH ) {
                    mousePos_i.x = SCREEN_WIDTH;
            }

            if( mousePos_i.y > SCREEN_HEIGHT ) {
                mousePos_i.y = SCREEN_HEIGHT;
            }

            if(mousePos_i.y < 0) {
                mousePos_i.y = 0;
            }

            if(mousePos_i.x < 0) {
                mousePos_i.x = 0;
            }




            /// Where in relation to the Center point are we clicking?
            /// Above? Below?

            if(mousePos_i.y < ceil(SCREEN_HEIGHT/2))
            {
                topof = true;
            } else if(mousePos_i.y > ceil(SCREEN_HEIGHT/2))
            {
                topof = false;
                belowof = true;
            } else {
                topof = false;
                belowof = false;
            }


            // Left of? Right of?
            if(mousePos_i.x < ceil(SCREEN_WIDTH/2)) {
                leftof = true;
            } else if(mousePos_i.x > ceil(SCREEN_WIDTH/2)) {
                leftof = false;
                rightof = true;
            } else {
                leftof = false;
                rightof = false;
            }


            // the nr of pixels between the click and the center position

            if( ceil(SCREEN_WIDTH/2) > mousePos_i.x) {
                relativeX = ceil(SCREEN_WIDTH/2) - mousePos_i.x;
            } else {
                relativeX = mousePos_i.x - ceil(SCREEN_WIDTH/2);
            }


            if( ceil(SCREEN_HEIGHT/2) > mousePos_i.y) {
                relativeY = ceil(SCREEN_HEIGHT/2) - mousePos_i.y;
            } else {
                relativeY = mousePos_i.y - ceil(SCREEN_HEIGHT/2);
            }



            // Adjust speed of panning
            // TODO FIXME maybe adjust for the ratio of screen width and height you knooow?

            relativeX =   ( (float) relativeX * mouseSensitivity/100 );
            relativeY =   ( (float) relativeY * mouseSensitivity/100 );



            /// Add or remove from the x and y offset because of the cliiicking

            if(rightof && topof) {
                viewPos.x -= relativeX;
                viewPos.y += relativeY;
            } else if(rightof && belowof) {
                viewPos.x -= relativeX;
                viewPos.y -= relativeY;
            } else if(leftof && belowof) {
                viewPos.x += relativeX;
                viewPos.y -= relativeY;
            } else if(leftof && topof) {
                viewPos.x += relativeX;
                viewPos.y += relativeY;
            } else {

            }

            if(debugLevel > 1)  {
                std::cout << " VIEWPOS x=" << viewPos.x << ", y=" << viewPos.y << "    CLICKEDPOS x=" << mousePos_i.x << ", y=" << mousePos_i.y << "\n";
            }

        }

        // Left mouse button pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {

            /// Dump the Bus position
            bus->dump(&viewPos);
            std::stringstream sstm;


            /// Get mouse position
            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window );

            Vector2f mouseWPos = Vector2f();
            mouseWPos.x = mousePos_i.x;
            mouseWPos.y = mousePos_i.y;
            sstm << "WPOS(" << mouseWPos.y << ", " << mouseWPos.x << ")\n";



            /// Redact ViewPosition rectangle from it to get back to GameMatrix positioning
            Vector2f mouseGPos = Vector2f();
            mouseGPos.x = mouseWPos.x - viewPos.x;
            mouseGPos.y = mouseWPos.y - viewPos.y;
            sstm << "GPOS(" << mouseGPos.y << ", " << mouseGPos.x << ")\n";

            Vector2f iso_pos = Vector2f();
            iso_pos = Grid::convert_pix_to_iso(mouseGPos, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT);



            /// Update the text
            lastClickedText.setFont(font);

            lastClickedText.setString(sstm.str());
            lastClickedText.setCharacterSize(12);
            lastClickedText.setFillColor(sf::Color::White);
            lastClickedText.setPosition(mouseWPos);



            grid->setVisible(iso_pos);

            if(debugLevel > 1)  {
                std::cout << " VIEWPOS x=" << viewPos.x << ", y=" << viewPos.y << "    CLICKEDPOS x=" << mousePos_i.x << ", y=" << mousePos_i.y << "\n";
                dumpPosition(iso_pos);
            }


            /// Pushing button on top toolbar

            // if (toolbarTop.within(mousePos_i.x, mousepPos_i.y) { pushbutton(x,y));
            toolbarTop->pushButton(0); // debug test



            /*



            // Old Locomotive Code
            // that made it move around towards the mouse cursor

            // Not sure I still need it ? // 2018-05



            bool rightof = false;
            bool belowof = false;
            bool topof = false;
            bool leftof = false;
            Vector2f dir;

            // Figure out what +-1 to do with the position
            if(loco->getPos().x < mouseWPos.x) {
                rightof = true;
            }
            if(loco->getPos().x >= mouseWPos.x) {
                leftof = true;
            }
            if(loco->getPos().y < mouseWPos.y) {
                belowof = true;
            }
            if(loco->getPos().y >= mouseWPos.y) {
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
            loco->setDirection(dir);*/

        }


        /// Trains



        /// Buses


        trafficMgr->updateAll(&viewPos);


        //grid->setVisible(bus->get_next_iso_pos());
        grid->setVisible(bus->get_next_pos());





















        /// Render

        window.clear({0, 0, 0});

        if(drawGm)   {  gm->draw(window, viewPos);  } // Draws the ground and water and suchers

        if(drawBlocks) {

            /// Iterate list of blocklists to draw them in render order

            hmap->draw(window, viewHPos);
        }


        if(drawLoco) {  loco->draw(window, viewHPos); }
        if(drawBuses) { bus->draw(window, &viewPos); }
        if(drawGrid) {  grid->draw(window, viewPos); }

        if(drawToolbar) {   toolbarTop->draw(window, viewPos); }

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






