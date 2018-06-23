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
#include "TrafficManager.hpp"       // ==> RoadNetwork.hpp
                                    // ==> Bus.hpp
#include "GameMatrix.hpp"
#include "Block.hpp"
#include "Locomotive.hpp"
#include "FileManager.hpp"



using namespace sf;





enum GAMEMODES { GAMEMODE_EDITOR, GAMEMODE_PAUSE };
enum LMBMODES  { LMB_CLICK_CREATE_OR_SWAP, LMB_CLICK_CREATE, LMB_PANNING, LMB_ENQUIRE };


class Core
{

public:
    Core();
    ~Core();

    int boot();

    int allocateResources();

    int loadResources(std::string _mapName);

    int setup(int, int, std::string);

    void run();

    void reset();

    void clearResources();

    void updateTraffic();




private:



    /// All the Maps



    //std::string startmapFilename = "data/simple.txt";
    //std::string startmapFilename = "data/two_roads.txt";
    //std::string startmapFilename = "data/garden.txt";
    //std::string startmapFilename = "data/aztec.txt";                           // Works 2018-05    but BUG with drawing the tall house.
    //std::string startmapFilename = "data/roadnetwork_test.txt";                // Works 2018-05
    //std::string startmapFilename = "data/bus_traffic_test.txt";                // Works 2018-05
    //std::string startmapFilename = "data/dijkstra_test_1.txt";                 // Works!
    //std::string startmapFilename = "data/dijkstra_test_2.txt";                 // Works!
    //std::string startmapFilename = "data/dijkstra_test_3.txt";                 // Works!
   // std::string startmapFilename = "data/dijkstra_test_4.txt";                 // Issue with Graph/Roadnetworks... seems to make unecessary 0,0,0,0,0,0,0 rows ?
                                                                // This started when I attempted to fix the CONSUME error of the stack from dijkstraresult, 2018-05-21
                                                                // //planForBusesOnRoadNetwork go there, set debuglevel=1 and watch

    //std::string startmapFilename = "data/dijkstra_test_5.txt";
    // dijkstra_test_5
    // Tested: lot of roadnr to roadnrs! 2018-05                                 // Works!

                // 14 = first road on left, 26 = farthest road on the right
                // 53 = very low very right

    //std::string startmapFilename = "data/bus_traffic_test.txt";              // Bugs... somehow! not sure yet. 2018-05
    std::string startmapFilename = "data\\maps\\bustest3.txt";                 // Bugs




    int consoleMode = false;
    int runRegressionTestAndExit = false;

    int debugLevel = 0;
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    int lockFPS = false;
    int lockFPS_n = 5;






    // Modes
    int gamemode = GAMEMODE_EDITOR;
    int lmbmode = LMB_ENQUIRE;

    bool changedCanvas = false;



    std::string windowTitle = "HurkaLumo editor alpha-0.1";
    std::string cn = "Core.cpp";


    // int mouseSensitivity = 3*ceil(100/lockFPS_n);    // Pan speed in percentage

    int mouseSensitivity = 1;    // Pan speed in percentage


    // Why these values?
    // Well I dont want to be in negative euclidian space so I offset everything so a huge gameboard can fit on the right side, inside positive x-space :)
    HPos *viewHPos;
    int startViewPosY = -331;                               // -331 works with 64 x 64
    int startViewPosX = -(NR_GRIDS_WIDTH*15+1000);          // -2178 works with 64 x 64


    // Testing for a while, remove when dijkstra is super stable (2018-05)
    bool dijkstraAutoEndpointsAdjust = true;            // If true, the fromRoad and toRoad vars below will bee adjusted automatically if out of bounds
    int dijkstraFromRoad= 0;
    int dijkstraToRoad = 4;




    TextureManager *textureMgr; // Singleton!

    TrafficManager *trafficMgr;
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

};



#endif
