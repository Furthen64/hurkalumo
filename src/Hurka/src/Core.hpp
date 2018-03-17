#ifndef CORE_H
#define CORE_H


#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <chrono>


// Project includes
#include "HurkaMap.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include "Grid.hpp"
#include "TextureManager.hpp"
#include "Toolbar.hpp"
#include "GameMatrix.hpp"
#include "Block.hpp"
#include "Locomotive.hpp"
#include "FileManager.hpp"
#include "Bus.hpp"


using namespace sf;



class Core
{

public:
    Core();
    ~Core();

    void boot();

    void allocateResources();

    void loadResources(std::string _mapName);

    void setup(int, int, std::string);

    void run();

    void reset();

    void clearResources();

    void updateBuses(Bus *bus, float dt,  HurkaMatrix *roadMatrix );




private:

    int debugLevel = 0;

    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    int lockFPS = true;
    int lockFPS_n = 10;

    std::string windowTitle = "HurkaLumo editor alpha-0.1";

    int mouseSensitivity = 4*ceil(100/lockFPS_n); // Pan speed in percentage

    Vector2i viewPos = {-2178,-331};

    TextureManager *textureMgr; // Singleton!

    GameMatrix *gm;
    Bus *bus = nullptr;
    Locomotive *loco = nullptr;
    Toolbar *toolbarTop = nullptr;
    Grid *grid = nullptr;
    HurkaMap *hmap = nullptr;
    HurkaMatrix *roadMatrix = nullptr;
    FileManager *fmgr;

    bool drawGm = 1;
    bool drawLoco = 0;
    bool drawBuses = 1;
    bool drawToolbar = 0;
    bool drawGrid = 1;
    bool drawBlocks = 1;



    Font font;
    Text lastClickedText;


    std::string cn = "Core.cpp";
    std::string startmapStr = "data/bustest.txt";

};



#endif
