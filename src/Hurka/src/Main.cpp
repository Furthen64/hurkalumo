#include "Core.hpp"


// Always push RUNNING code
// If it doesn't run, just stub out the errors and make it run before pushing to a branch
// If that is too much to do, make a BRANCH!



/// GLOBALS

TextureManager* TextureManager::m_instanceSingleton = nullptr;




/// TODO To be removed a lot from, use class Core instead!
int main()
{

    Core core = Core();

    core.boot();

    return 0;
}
