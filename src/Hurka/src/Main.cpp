#include "Core.hpp"
#include "Utils.hpp"


/// REDESIGN
// 2018-06-22 jörgen engström     #CR29  Trying to remove annoying bug... Need to have sf::renderwindow exist before allocating any sf::texture object.





// Always push RUNNING code
//
// - If it doesn't run, just stub out the errors and make it run before pushing to a branch
// - If that is too much to do, make a new branch


/// Wishlist, wouldn't it be nice with a WindowManager holding vars like:
///         * RenderWindow
///         * inputs
///         * viewPos  so you wouldn't have to pass it along to all draw functions  =>      sfml library has a View that I should look into (no pun intended)




// My two singletons, maybe a third one on the way which carry high level settings
TextureManager* TextureManager::m_instanceSingleton = nullptr;
GLContextSingleton* GLContextSingleton::m_instanceSingleton = nullptr;




int main()
{

    // First time you run enableFallBackContext, an sf::Context will be created,
    // so that OpenGL calls have a GL Context to work with.
    // If we only create the primary windows (main menu, game window, editor window) that gets open and closed,
    // we suffer issues with SFML Texture objects...  See CR#29 at github
    enableFallbackContext();

    Core core = Core();

    LifecycleResult *lfRes = core.lifecycle();

    std::cout << "main: Core completed its entire lifecycle *** \n";

    // For now dump output, dont react to it
    lfRes->dump();


    return 0;
}
