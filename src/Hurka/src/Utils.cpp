#include "Utils.hpp"







void dumpPosition(Vector2f iso_pos)
{
    std::cout << "pos(" << iso_pos.y << ", " << iso_pos.x << ")\n";
}

// Seems to work for a couple of weeks now (2018-04) havent given thoughts about memory DEALLOCATION though...
// (-+)
int **allocateMatrix(int rows, int cols)
{
    std::string cn = "Utils.cpp";

    if(rows > 10000 || cols > 10000) {
            std::cout << "ERROR " << cn << " Cannot allocate matrix of that size! " << rows << ", " << cols << "\n";
        return nullptr;
    }

    int** matrix = new int*[rows];
    if (rows)
    {
        matrix[0] = new int[rows * cols];
        for (int i = 1; i < rows; ++i)
            matrix[i] = matrix[0] + i * cols;
    }


    for(int r = 0; r < rows; r++) {

        for(int c = 0; c < cols; c++) {
                matrix[r][c] = 0;

        }
    }




    return matrix;

}



/// (++)
void dumpMatrix(int** matrix, int rows, int cols, std::string indent)
{

    std::string cn = "Utils.cpp";

    if(rows > 10000 || cols > 10000) {
        std::cout << indent << "ERROR" << cn << " too big of a roadmatrix! " << rows << ", " << cols << "\n";
        return ;
    }


    std::cout << "\n\n" << indent << "Matrix: \n" << indent  << "{\n" << indent;
    for(int r = 0; r < rows; r++) {
        std::cout << "   ";
        for(int c = 0; c < cols; c++) {
                std::cout << matrix[r][c] << ", ";

        }
        std::cout << "\n" << indent;
    }
    std::cout << "}\n";


}



// Takes  a matrix (srcMtx) with its srcRows x srcCols
// Starts at row startY, column startX and copies height x width block of data from that matrix into a new one, returns new one.
//
// TESTED a few times, works fine
// Docs: Utils_copySubMatrix.png
// BUG: startx starty is wrong
// (-+)
int **copySubMatrix(int **srcMtx, int srcRows, int srcCols, int startY, int startX, int height, int width, int debugLevel)
{

    std::string cn = "Utils.cpp";

    if(debugLevel >=1) {
        std::cout << "\n\ncopySubMatrix\n----------------\n";
        std::cout << "srcRows=" << srcRows << ", srcCols=" << srcCols << ", startY=" << startY << ", startX=" << startX << ", height=" << height << ", width=" << width << "\n";
    }

    if(srcMtx == nullptr) {
        std::cout << "ERROR " << cn << " supplied nullptr to copySubMatrix!\n";
        return nullptr;
    }

    if(startY > srcRows) {
        std::cout << "ERROR " << cn << " startY > srcRows in copySubMatrix!\n";
        return nullptr;
    }

    if(startX > srcCols) {
        std::cout << "ERROR " << cn << " startX > srcCols in copySubMatrix!\n";
        return nullptr;
    }


    if( (startY + height) > srcRows) {
        std::cout << "ERROR " << cn << " (startY+height) > srcRows in copySubMatrix!\n";
        return nullptr;
    }



    if( (startX + width) > srcCols) {
        std::cout << "ERROR " << cn << " (startX+width) > srcCols in copySubMatrix!\n";
        return nullptr;
    }


    if(debugLevel >=2) {
        std::cout << "dumping srcmtx:\n";
        dumpMatrix(srcMtx, srcRows, srcCols, "   ");

    }



    /// Allocate the new matrix
    int **destMtx = nullptr;

    destMtx = allocateMatrix(height, width);



    /// Loop over original matrix and do the work
    if(debugLevel >=2) {
        std::cout << " Looping original matrix, copying only the parts we want:\n";
    }

    for(int srcY = startY, destY = 0; srcY < (startY+height); srcY++, destY++)
    {
        for(int srcX = startX, destX = 0; srcX < (startX+width); srcX++, destX++)
        {

            if(debugLevel >=2) {
                std::cout << "   srcMtx[" << srcY << "][" << srcX << "]";
                std::cout << "   destMtx[" << destY << "][" << destX << "]    = " << srcMtx[srcY][srcX] << "\n\n";
            }

            destMtx[destY][destX] = srcMtx[srcY][srcX]; // Copy the value from the original

        }
    }



    return destMtx;

}



// (++)
void initRandomizer()
{

    srand(time(0));
}


// (++)
int randBetween(int lowNr, int highNr)
{
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
