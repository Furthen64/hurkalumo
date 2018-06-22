#include "Core.hpp"



/// REDESIGN
// 2018-06-22 jörgen engström     #CR24  Trying to remove annoying bug... Need to have sf::renderwindow exist before allocating any sf::texture object.





// Always push RUNNING code
//
// - If it doesn't run, just stub out the errors and make it run before pushing to a branch
// - If that is too much to do, make a new branch


/// Wishlist, wouldn't it be nice with a WindowManager holding vars like:
///         * RenderWindow
///         * inputs
///         * viewPos  so you wouldn't have to pass it along to all draw functions








/// GLOBALS

TextureManager* TextureManager::m_instanceSingleton = nullptr;


int main()
{

    // Start up an OpenGL context for the application

    sf::Window logWindow;
    logWindow.create(sf::VideoMode(320, 240), "Log for HurkaLumo");

    Core core = Core();

    core.boot();

    logWindow.close();

    return 0;
}
