#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "FileManager.hpp"


/// FileManager



FileManager::FileManager()
{


}


/// (++)
void dumpMatrix(int** matrix, int rows, int cols)
{

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

// (--)
void dumpBlockList(std::list<Block *> _blockList, TextureManager *textureMgr)
{

    int n = 0;
    std::cout << "\n\ndumpBlockList: \n{\n";

    // iterate over all items
    for (std::list<Block *>::iterator itAll = _blockList.begin(); itAll != _blockList.end(); ++itAll)
    {
        std::cout << "    [" << n << "] TextureName of current block: " << (*itAll)->getTextureName() << "\n";
        n++;

    }
    std::cout << "}\n";

}

















// Regular file is the matrix as you would see this game from above without isometricness
//
// docs: readRegularFile.png
//
// TEST!
// (--)
HurkaMap FileManager::readRegularFile(std::string _filename, TextureManager *textureMgr)
{


    int debugLevel = 1;

    if(debugLevel > 0) {
        std::cout << "\n\n*** readRegularFile( " << _filename << ") \n";
    }

    /// Verify the File, read the file into a standard blurgh, put the blurgh into right
    /// list format and insert that into a HurkaMap. Return the hurkamap.
    HurkaMap emptyMap("empty", textureMgr);
    HurkaMap resultMap(_filename, textureMgr);

    // The blocklist will be attached to the resultmap if all goes well
    std::list<Block *> blockList;


    std::ifstream infile;
    std::string line;
    infile.open(_filename);


    // DEBUG FIXME CORRECT oh god plz fix this, read the width and height from file instead
    int MTX_ROWS=4;
    int MTX_COLS=4;

    if (infile.is_open()) {

            //
            /// Create a Matrix of NxM
            //

            int rows = MTX_ROWS, cols = MTX_COLS;
            int** matrix = new int*[rows];
            if (rows)
            {
                matrix[0] = new int[rows * cols];
                for (int i = 1; i < rows; ++i)
                    matrix[i] = matrix[0] + i * cols;
            }



            /// Read lines from the file
            /// For every chunk, put it in the matrix


            if(debugLevel > 0) {
                std::cout << "    read lines from the file\n";
            }
            unsigned int currRow = 0;
            unsigned int currCol = 0;
            unsigned int w;
            unsigned int offset = 0;

            while (std::getline(infile, line))
            {

                // Parse this line


                while( (offset+4) <= line.size()) {

                    w = stoi(line.substr(offset,3));
                    offset += 4;

                    // Insert into matrix
                    matrix[currRow][currCol] = w;

                    currCol++;
                }

                offset=0;
                currCol=0;
                currRow++;

            }



            // Output matrix

            if(debugLevel > 0) {

                std::cout << "    complete!\n    put into matrix:\n";

                dumpMatrix(matrix, MTX_ROWS, MTX_COLS);
            }








            /// Now get it out to blocklists

            currRow = 0;
            currCol = 0;

             if(debugLevel > 0) {

                std::cout << "    put matrix content into blocklists\n";

            }





            /// Parse the texture matrix in a particular way to get them in the way
            /// they should be rendered
            ///
            /// See docs: matrix_texture_into_blocklist_loop.png



           int yDown = 0;
           int xDownRight = 0;

           int yUp = 0;
           int xRight = 0;

           std::string textureName;


           // Loop 1
           std::cout << "\n\n(Loop 1)\n";
           while(yDown < MTX_ROWS)
           {
               while(yUp > -1 && xRight < MTX_COLS)
               {

                   // row = yUp
                   // col = xRight
                   textureName = textureMgr->getTextureNameByIndex( matrix[yUp][xRight] );
                   std::cout << "(" << yUp << ", " << xRight << ") \n";

                   //Block *block  = new Block({(float)yUp,(float)xRight}, textureName, textureMgr);
                   Block *block  = new Block({(float)xRight,(float)yUp}, textureName, textureMgr);

                   blockList.push_back(block);

                   yUp--;
                   xRight++;



               }

               yDown++;

               xRight = 0;
               yUp = yDown;
           }




           // Loop 2

           yUp = MTX_ROWS-1;    // Start at the bottom, and traverse right in this "loop 2"

           std::cout << "\n\n(Loop 2)\n";
           xDownRight++;
           std::cout << "1\n";
           xRight = xDownRight;
           std::cout << "2\n";

           while(xDownRight < MTX_COLS)
           {
               std::cout << "3\n";
               while(yUp > -1 && xRight < MTX_COLS)
               {
                   std::cout << "4\n";
                   std::cout << "(" << yUp << ", " << xRight << ")\n";

                   // row = yUp
                   // col = xRight

                   textureName = textureMgr->getTextureNameByIndex( matrix[yUp][xRight] );

                   std::cout << "5\n";
                   std::cout << "(" << yUp << ", " << xRight << ") \n";

                   //Block *block  = new Block({(float)yUp,(float)xRight}, textureName, textureMgr);
                   Block *block  = new Block({(float)xRight,(float)yUp}, textureName, textureMgr);

                   blockList.push_back(block);

                   yUp--;
                   xRight++;
               }

               xDownRight++;

               xRight = xDownRight;
               yUp = MTX_ROWS-1;
           }















            /// Put that blocklist into a HurkaMap that we will return
            resultMap.putBlockList(blockList);

            /// Complete!!

            if(debugLevel > 0) {

                std::cout << "    blocklists complete:\n";
                dumpBlockList(blockList, textureMgr);
            }



            // N - Cleanup of work variable Matrix



            if (rows) delete [] matrix[0];
            delete [] matrix;


    } else {
        std::cout << "ERROR " << cn << ": Could not open file \"" << _filename << "\"!\n";
        infile.close();
        return emptyMap;
    }


    if(debugLevel > 0) {
        std::cout << "\n readRegularFile complete **** \n\n";
    }

    infile.close();
    return resultMap;
}

// (-+)
bool FileManager::verifyFile(std::string _filename)
{

    int debugLevel = 0;
    if(debugLevel > 0) {
        std::cout << "\n\n**** VerifyFile\n";
    }

    std::ifstream infile(_filename);

    if (!infile.is_open()) {
        std::cout << "ERROR " << cn << ": Could not open file \"" << _filename << "\"!\n";
        infile.close();
        return false;
    }


    std::string line;

    int divisor = 4;
    int nrElementsN = 0;
    int nrElementsM = 0;
    unsigned int firstLineLength = 0;


    // Get the first line
    std::getline(infile, line);
    nrElementsM++;

    firstLineLength = line.length();

    if(firstLineLength%divisor!=0) {  // is it equyally divisable by "4" for instance...?
        std::cout << "ERROR " << cn << ": Line not divisible by " << divisor << ", missing comma? missing leading zeroes?\n";
        infile.close();
        return false;
    }

    nrElementsN = line.length()/divisor;  // Gets for instance


    // Now check the rest of them, make sure they are aligned against the first line

    while (std::getline(infile, line))
    {
        if(debugLevel > 0)
        {
            std::cout << "\"" << line << "\"  linelength=" << line.length() <<"\n";
        }



        nrElementsM++;

        if(line.length()!= firstLineLength) {
            std::cout << "ERROR " << cn << ": Line is not same length as the first one !\n";
            infile.close();
            return false;
        }

        // process pair (a,b)
    }


    if(nrElementsM != nrElementsN) {
        std::cout << "ERROR " << cn << ": The file should contain M x M matrix... " <<
          " The elements on one axis should be same as the other axis. This is an " << nrElementsM << "x" << nrElementsN << "!\n";
        infile.close();
        return false;
    }

    return true;
}


// (++)
void FileManager::printWorkingDir()
{
     char cCurrentPath[FILENAME_MAX];

     if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
     {
         std::cout << errno << "\n";
        return ;
     }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    std::cout << "The current working directory is \"" << cCurrentPath << "\"\n";
}




















