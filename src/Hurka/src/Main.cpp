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
    Core core = Core();

    LifecycleResult *lfRes = core.lifecycle();

    std::cout << "main: Core completed its entire lifecycle *** \n";

    // For now dump output, dont react to it
    lfRes->dump();



    return 0;
}



 /* bugtest
    // Start up an OpenGL context for the application and then close it(?) testing stuff... 2018-07

        sf::Window logWindow;
        logWindow.create(sf::VideoMode(320, 240), "Log for HurkaLumo");
        logWindow.close();

        sf::Texture text;

        text.loadFromFile(getFullUri("data\\textures\\LOCOMOTIVE.png"));
        sf::Sprite sprite = Sprite(text);
        sprite.setTextureRect( {0,0,32,32});


        std::cout << "press any key\n";
        getchar();
        return 0;


    */
