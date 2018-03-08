#include "Core.hpp"


// Always push RUNNING code
// If it doesn't run, just stub out the errors and make it run before pushing to a branch
// If that is too much to do, make a BRANCH!


/// Vore det inte najs med en WindowManager som håller
///         * RenderWindow
///         * Inputs
///         * Vector2u viewPos  så man slipper skicka med den till alla .draw() ?


/// GLOBALS

TextureManager* TextureManager::m_instanceSingleton = nullptr;




/// TODO To be removed a lot from, use class Core instead!
int main()
{

    Core core = Core();

    core.boot();

    return 0;
}
