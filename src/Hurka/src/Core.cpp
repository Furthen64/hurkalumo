#include "Core.hpp"


Core::Core()
{
}


Core::~Core()
{
    clearResources();
}


void Core::boot()
{
    std::cout << "\n\n\n---------------booting-------------------\n";
    allocateResources();
    loadResources("data/bustest.txt");
    setup(800,600, "Hurkalumo Editor 0.1-alpha");
    run();
}

void Core::allocateResources()
{

    std::cout << "\n\n\n---------------allocateResources-------------------\n";
}

void Core::loadResources(std::string mapName)
{

    std::cout << "\n\n\n---------------loadResources-------------------\n";

    /// Load the Textures
    textureMgr = textureMgr->getInstance();
    textureMgr->loadTextures();


    /// Read the map

    hmap = fmgr->readRegularFile(mapName);
    if(hmap->mapName == "empty") { std::cout << "ERROR Could not read map " << mapName << ", exiting!\n"; return ;  }


    /// Get the roads

    roadMatrix = hmap->getRoadMatrix();
    if(debugLevel > 0) {
        roadMatrix->dump();
    }


}

void Core::setup(int width, int height, std::string title)
{

    std::cout << "\n\n\n---------------setup-------------------\n";

    int GAME_WIDTH = 64;    // nr of cells in the game board
    int GAME_HEIGHT = 64;

    initRandomizer();

    fmgr = new FileManager();
    gm = new GameMatrix({GAME_HEIGHT,GAME_WIDTH,1});          /// high level structure of game
    bus = new Bus({10.0f, 10.0f});
    loco = new Locomotive({10.0f, 10.0f});
    toolbarTop = new Toolbar({260.0f, 0.0f});
    grid = new Grid(GAME_HEIGHT, GAME_WIDTH);


    /// Place them Buses
    bus->setPos(bus->randStartingPos(roadMatrix));

}


void Core::run()
{
    RenderWindow window(sf::VideoMode(800, 600), "HurkaLumo editor 0.1-alpha");
    window.setFramerateLimit(10);

    std::cout << "\n\n\n---------------run-------------------\n";

    if(!window.isOpen()) {
        std::cout << "ERROR " << cn << " sf::window is not open!\n";
        return ;
    }
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

        // Get mouse input
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {

            ///FIXME TESTA Funkar detta med *window ???

            sf::Vector2i mousePos_i = sf::Mouse::getPosition( window ); // window is a sf::Window

            Vector2f mousePos_f = Vector2f();
            mousePos_f.x = mousePos_i.x;
            mousePos_f.y = mousePos_i.y;


            /// Pushing button on top toolbar

            // if (toolbarTop.within(mousePos_i.x, mousepPos_i.y) { pushbutton(x,y));
            toolbarTop->pushButton(0); // debug test


            bool rightof = false;
            bool belowof = false;
            bool topof = false;
            bool leftof = false;
            Vector2f dir;

            // Figure out what +-1 to do with the position
            if(loco->getPos().x < mousePos_f.x) {
                rightof = true;
            }
            if(loco->getPos().x >= mousePos_f.x) {
                leftof = true;
            }
            if(loco->getPos().y < mousePos_f.y) {
                belowof = true;
            }
            if(loco->getPos().y >= mousePos_f.y) {
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

            loco->setDirection(dir);

        }

        loco->update(1);


        /// Update Busses

        updateBuses(bus, 1, roadMatrix);

        //toolbarTop.update(); TODO should I do this?

        /// Render

        window.clear({0, 0, 0});


        // Draw the game board

        if(drawGm)   {  gm->draw(window);  } // Draws the ground and water and suchers



        if(drawBlocks) {

            /// Iterate list of blocklists to draw them in renderorder

            hmap->draw(window);

        }


        if(drawLoco) {  loco->draw(window); }

        if(drawBuses) { bus->draw(window); }


        if(drawGrid) {  grid->draw(window); }

        if(drawToolbar) {   toolbarTop->draw(window); }

        window.display();



    } // while window is open


}


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


    std::cout << "JÖRGEN, Skapa clearResources()\n";
}





// For now it just moves them around randomly
// TODO: needs a matrix of the ROADS so we can figure out the way to move it

// (--)
void Core::updateBuses(Bus *bus, float dt,  HurkaMatrix *roadMatrix )
{

    if(roadMatrix->isAllocated() == false) {
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





/// ------------------------------------------------





/*



/// Main Loop


delete roadMatrix;
*/
