#include "FileManager.hpp"



/// Reads the file which contains numbers, like 001,005,007
/// Puts them all into a matrix N x M
/// Parses the Matrix of integers and creates Blocks
/// while parsing, puts them in a particular order which enables rendering
/// in the isometric view
///
/// docs: readRegularFile.png
// (-+)



HurkaMap *FileManager::readRegularFile(std::string _filename)
{
    int debugLevel = 0;

    // The possible return objects, one fail and one win
    HurkaMap *emptyMap = new HurkaMap("empty", nullptr, 0,0 );
    HurkaMap *resultMap = nullptr;  // Allocated later when we have the matrix of objects (001,007,... etc)





    // The blocklist will be attached to the resultmap if all goes well

    std::list<Block *> blockList;

    std::ifstream infile;
    std::string line;

    int mapRows = 0;
    int mapCols = 0;



    if(debugLevel > 0) {
        std::cout << "\n\n*** readRegularFile( " << _filename << ") \n";
    }




    /// Verify the File


    if(!verifyFile(_filename, &mapRows, &mapCols)) {
        std::cout << "ERROR " << cn << " unable to verify the file, exiting!\n";
        return emptyMap;
    }



    if(debugLevel > 0) {
        std::cout << "The file has ROWS=" << mapRows << ", COLS=" << mapCols << "\n";
    }


    /// Open the File here

    infile.open(_filename);


    /// Get the Rows and Columns out of the map


    int MTX_ROWS=mapRows;
    int MTX_COLS=mapCols;

    if (infile.is_open()) {

            //
            /// Create a Matrix of NxM
            //

            int rows = MTX_ROWS, cols = MTX_COLS;

            //TEST funkar detta?
            int **matrix = allocateMatrix(rows, cols);

            /// Read lines from the file
            /// For every number (001,002,...) , put it in the matrix

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

            // Create output object now that we have the matrix
            resultMap = new HurkaMap(_filename, matrix, MTX_ROWS, MTX_COLS);








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
           if(debugLevel > 0) {
                std::cout << "\n\n(Loop 1)\n";
           }

           while(yDown < MTX_ROWS)
           {
               while(yUp > -1 && xRight < MTX_COLS)
               {

                   // row = yUp
                   // col = xRight


                   TextureManager *textureMgr;
                   textureMgr = textureMgr->getInstance();


                   textureName = textureMgr->getTextureNameByIndex( matrix[yUp][xRight] );



                   Block *block  = new Block({(float)xRight,(float)yUp}, textureName);

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




           if(debugLevel > 0) {
               std::cout << "\n\n(Loop 2)\n";
           }

           xDownRight++;

           xRight = xDownRight;


           while(xDownRight < MTX_COLS)
           {

               while(yUp > -1 && xRight < MTX_COLS)
               {
                   // row = yUp
                   // col = xRight

                   TextureManager *textureMgr;
                   textureMgr = textureMgr->getInstance();

                   textureName = textureMgr->getTextureNameByIndex( matrix[yUp][xRight] );    // 001 -> "HOUSE001" for instance

                   Block *block  = new Block({(float)xRight,(float)yUp}, textureName);

                   blockList.push_back(block);

                   yUp--;
                   xRight++;
               }

               xDownRight++;

               xRight = xDownRight;
               yUp = MTX_ROWS-1;
            }


            /// Put that blocklist into a HurkaMap that we will return
            if(resultMap == nullptr)  {
                std::cout << "ERROR " << cn << " resultMap is null inside readRegularFile(), stopping !\n";
                return emptyMap;
            }

            resultMap->putBlockList(blockList);

            /// Complete!!

            if(debugLevel > 0) {

                std::cout << "    blocklists complete:\n";
                //dumpBlockList(blockList);
            }


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





// (+-)
// Given rows and cols, check that they actually contain those constraints
// IF they are nullptr that check is not made

// Wantlist:   Att den kollar vilka 001,002 osv som faktiskt finns i texturlistan


bool FileManager::verifyFile(std::string _filename, int *rows, int *cols)
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
        std::cout << "ERROR " << cn << ": Line not divisible by " << divisor << ", missing comma? missing leading zeroes? every number in the format of 001,002,003?\n";
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

    if(rows != nullptr) {

        (*rows) = nrElementsM;
    }

    if(cols != nullptr) {
        (*cols) = nrElementsN;
    }

    return true;
}


// (++)
void FileManager::printWorkingDir()
{
     char cCurrentPath[FILENAME_MAX];

     if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
     {
         std::cout << errno << "\n";
        return ;
     }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    std::cout << "The current working directory is \"" << cCurrentPath << "\"\n";
}



/*
// (-+)
void dumpBlockList(std::list<Block *> _blockList)
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


*/























