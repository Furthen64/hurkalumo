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

    int SCREEN_WIDTH = 800;
    int SCREEN_HEIGHT = 600;
    std::string windowTitle = "HurkaLumo editor alpha-0.1";

    /// BUG FIXME  , if you set this low, 32,32 game crashes! why?


    TextureManager *textureMgr; // Singleton!

    GameMatrix *gm;
    Bus *bus = nullptr;
    Locomotive *loco = nullptr;
    Toolbar *toolbarTop = nullptr;
    Grid *grid = nullptr;
    HurkaMap *hmap = nullptr;
    HurkaMatrix *roadMatrix = nullptr;
    FileManager *fmgr;

    bool drawGm = true;
    bool drawLoco = true;
    bool drawBuses = true;
    bool drawToolbar = true;
    bool drawGrid = true;
    bool drawBlocks = true;


    std::string cn = "Core.cpp";





};



#endif
