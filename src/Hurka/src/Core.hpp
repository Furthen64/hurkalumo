#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <chrono>

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
enum RUNRESULTS { RUN_RESULT_QUIT, RUN_RESULT_LOAD_NEW_MAP, RUN_RESULT_NEW_MAP };




class LifecycleResult
{
public:
    LifecycleResult() {}
    LifecycleResult(int _intReturn, std::string str1)
    {
        intReturn = _intReturn;
        lfStr1 = str1;
    }
    ~LifecycleResult() {}


    void dump()
    {
        std::cout << "lfRes:      intReturn=" << intReturn << ", lfStr1= " << lfStr1 << ", lfStr2= " << lfStr2 << ", lfStr3 = " << lfStr3 << "\n";

    }

    int intReturn;
    std::string lfStr1;
    std::string lfStr2;
    std::string lfStr3;





};

// Pretty heavy return class, used by Core::run()
class RunResult
{
public:
    RunResult() {}
    ~RunResult() {}

    void dump()
    {
        std::cout << "runResult:      quitresult= " << quitresult << ", intReturn=" << intReturn << ", retStr1= " << retStr1 << ", retStr2= " << retStr2 << ", retStr3 = " << retStr3 << "\n";

    }

    int quitresult;         // See Enum RUNRESULTS
    int intReturn = -999;       // Magic nr to know if values has been set or not (uglycode)
    char charReturn;
    std::string retStr1 ;   // Usually used for filename
    std::string retStr2 ;
    std::string retStr3 ;


private:

};



// The Core , hard as fuck
class Core
{

public:
    Core();
    ~Core();

    LifecycleResult * lifecycle();

    int allocateResources();

    int loadResources(std::string _mapName);

    int setup(int, int, std::string);

    RunResult *run();

    void reset();

    void clearResources();

    void updateTraffic();




private:



    /// All the Maps



    //std::string startmapFilename = "data\\maps\\simple.txt";
    //std::string startmapFilename = "data\\maps\\two_roads.txt";
    //std::string startmapFilename = "data\\maps\\garden.txt";
    //std::string startmapFilename = "data\\maps\\aztec.txt";                           // Works 2018-05    but BUG with drawing the tall house.
    //std::string startmapFilename = "data\\maps\\roadnetwork_test.txt";                // Works 2018-05
    //std::string startmapFilename = "data\\maps\\roadnetwork_long.txt";                // requires 43x43 Works 2018-05
    //std::string startmapFilename = "data\\maps\\bus_traffic_test.txt";                // Works 2018-05
    //std::string startmapFilename = "data\\maps\\dijkstra_test_1.txt";                 // Works!
    //std::string startmapFilename = "data\\maps\\dijkstra_test_2.txt";                 // Works!
    //std::string startmapFilename = "data\\maps\\dijkstra_test_3.txt";                 // Works!
    std::string startmapFilename = "data\\maps\\dijkstra_test_4.txt";                 // Issue with Graph/Roadnetworks... seems to make unecessary 0,0,0,0,0,0,0 rows ?
                                                                // This started when I attempted to fix the CONSUME error of the stack from dijkstraresult, 2018-05-21
                                                                // //planForBusesOnRoadNetwork go there, set debuglevel=1 and watch

    //std::string startmapFilename = "data\\maps\\dijkstra_test_5.txt";
    // dijkstra_test_5
    // Tested: lot of roadnr to roadnrs! 2018-05                                 // Works!

                // 14 = first road on left, 26 = farthest road on the right
                // 53 = very low very right

    //std::string startmapFilename = "data\\maps\\bus_traffic_test.txt";         // Bugs... somehow! not sure yet. 2018-05
    //std::string startmapFilename = "data\\maps\\bustest3.txt";                 // Works 2018-06


    int consoleMode = false;
    int runRegressionTestAndExit = false;

    int debugLevel = 0;
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    int lockFPS = true;
    int lockFPS_n = 60;






    // Modes
    int gamemode = GAMEMODE_EDITOR;
    int lmbmode = LMB_ENQUIRE;
    bool changedCanvas = false;



    std::string windowTitle = "HurkaLumo editor alpha-0.1";
    std::string cn = "Core.cpp";


    float mouseSensitivity = 3*ceil(100/lockFPS_n);    // Pan speed in percentage



    // Why these values?
    // Well I dont want to be in negative euclidian space so I offset everything. A huge gameboard with many tiles can fit on the right side inside positive x-space :)
    HPos *viewHPos;
    int startViewPosY = -331;                               // -331 works with 64 x 64
    int startViewPosX = -(NR_GRIDS_WIDTH*15+1000);          // -2178 works with 64 x 64


    // Testing for a while, remove when dijkstra is super stable (2018-05)
    bool dijkstraAutoEndpointsAdjust = true;            // If true, the fromRoad and toRoad vars below will bee adjusted automatically if out of bounds
    int dijkstraFromRoad= 0;
    int dijkstraToRoad = 40;




    TextureManager *textureMgr; // Singleton!

    TrafficManager *trafficMgr;
    GameMatrix *gm;
    Bus *bus = nullptr;
    Locomotive *loco = nullptr;
    Toolbar *toolbarTop = nullptr;
    Grid *grid = nullptr;
    HurkaMap *hmap = nullptr;
    HurkaMatrix *roadMatrix = nullptr;
    FileManager *fm;

    bool drawGm = 1;
    bool drawLoco = 0;
    bool drawBuses = 1;
    bool drawToolbar = 1;
    bool drawGrid = 1;
    bool drawBlocks = 1;

    Font font;
    Text lastClickedText;

};



#endif
