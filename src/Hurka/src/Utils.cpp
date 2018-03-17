#include "Utils.hpp"



void dumpPosition(Vector2f iso_pos)
{
    std::cout << "pos(" << iso_pos.y << ", " << iso_pos.x << ")\n";
}

int **allocateMatrix(int rows, int cols)
{

    if(rows > 10000 || cols > 10000) {
            std::cout << "ERROR cannot allocate matrix of that size! " << rows << ", " << cols << "\n";
        return nullptr;
    }

    // std::cout << "allocateMatrix(" << rows << ", " << cols << ")\n";
    int** matrix = new int*[rows];
    if (rows)
    {
        matrix[0] = new int[rows * cols];
        for (int i = 1; i < rows; ++i)
            matrix[i] = matrix[0] + i * cols;
    }



    return matrix;

}



/// (++)
void dumpMatrix(int** matrix, int rows, int cols)
{

    std::string cn = "Utils.cpp";

    if(rows > 10000 || cols > 10000) {
        std::cout << "ERROR" << cn << " too big of a roadmatrix! " << rows << ", " << cols << "\n";
        return ;
    }



    std::cout << "\n\n Matrix: \n{\n";
    for(int r = 0; r < rows; r++) {
        std::cout << "   ";
        for(int c = 0; c < cols; c++) {
                std::cout << matrix[r][c] << ", ";

        }
        std::cout << "\n";
    }
    std::cout << "}\n";


}

void initRandomizer()
{

    srand(time(0));
}

int randBetween(int lowNr, int highNr)
{


/*
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(lowNr,highNr); // guaranteed unbiased

    auto random_integer = uni(rng);

    return random_integer;*/
    return rand()%(highNr-lowNr + 1) + lowNr;
}



// (++)
void waitForInput()
{
    std::cout << "\nPress enter to continue...\n";
    getchar();
}



/// TESTS
/*


These tests were commented out BECAUSE
Filemanager.hpp includes utils.hpp
utils.hpp ioncludes Filemanager.hpp

    CYCLIC INCLUSION

        CYCLIC INCLUSION

            CYCLIC INCLUSION



// (++)
bool testFileManager(int debugLevel)
{

    std::string stdMap = "data/bustest.txt";


    FileManager fmgr;
    bool result = true;

    if(debugLevel >0) {
        std::cout << "\n\n*** Working directory ***\n";
        fmgr.printWorkingDir();
    }


    if(debugLevel >0) {
        std::cout << "\n\n*** Verifying file ***\n";
    }

    if(!fmgr.verifyFile(stdMap, nullptr, nullptr)) {
        result = false;
    }

    if(debugLevel >0) {
        std::cout << "\n\n*** Testing reading files ***\n";
    }


//    HurkaMap *hmap = fmgr.readRegularFile(stdMap);

    if(hmap->mapName== "empty") {
        result = false;
    }

    return result;
}


// (-+)
bool testList(int debugLevel)
{

    bool result = true;

    if(debugLevel >0) {
        std::cout << "\n\n*** testList()\n";
    }

    std::list<Block> blockList;

    Block house001 ({0,1}, "HOUSE001");
    blockList.push_back(house001);

    if(debugLevel >0) {
        std::cout << "    Texturename of first block: " << house001.getTextureName() << "\n";
    }

    Block tree001( {0,0}, "TREE001");
    blockList.push_back(tree001);

    if(debugLevel >0) {
        std::cout << "    Texturename of second block: " << tree001.getTextureName() << "\n\n";
        std::cout << "Blocklist content: \n{\n";
    }

    // iterate over all items
    int n = 0;
    for (std::list<Block>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
    {
        if(n == 0) {
            if((*itAll).getTextureName() != "HOUSE001") {
                result = false;
            }

        }

        if(n == 1) {
            if((*itAll).getTextureName() != "TREE001") {
                result = false;
            }

        }

        if(debugLevel >0) {
            std::cout << "    [" << n << "] texturename of current block: " << (*itAll).getTextureName() << "\n";
        }
        n++;
    }

    if(debugLevel >0) {
        std::cout << "}\n end of Testlist \n\n";
    }

    return result;
_getcwd
}



bool integrityTesting()
{


    if(!testFileManager(0)) {
        std::cout << "ERROR: testFileManager failed. Something went wrong during integrity test of software!";
        waitForInput();
        return false;
    }

    if(!testList(0)) {
        std::cout << "ERROR: testList failed. Something went wrong during integrity test of software!";
        waitForInput();
        return false;
    }


    return true;


}
*/
