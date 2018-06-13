#include "Core.hpp"


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

    core.boot();

    return 0;
}
