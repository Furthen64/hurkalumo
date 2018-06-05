#include "FileManager.hpp"

// SOLVED:

    // BUG: I cannot understand this Bug.

    // SYMPTOM:
    // I do this std::string cn = "blargh"; in class declaration
    //  when I output cn I get segmentation fault
    // std::cout << cn << "\n";

    // ONLY when there's a file that cannot be read!
    // IF the file exists, then nothing goes wrong..


    // Maybe sheds some light on it:
    //https://stackoverflow.com/questions/33234693/strange-stdcout-behaviour-with-const-char/33234729

    // Maybe I'm using up the stackspace, and moves outside of what Im allowed?
    // Like this here fella:
    // https://forum.openframeworks.cc/t/super-difficult-c-crashes/1344/4
    // "did some further testing - it seems to be a stack overflow issue after all.
    // if i reduce the amount requested so it’s less than 4096 bytes, there’s no problem any more… "
    // "perhaps this is running inside an MPTask, which according to this Apple-Technical-Q&A have a stack size of 4k…?"
    /*t
    jag hade inte allokerat fmgr och började använda funktioner från den
    och fick riktigt jobbiga fel...


    VARFÖR ser jag inte att fmgr inte blev allokerad???


    VARFÖR kan jag använda fmgr->köraFunktioner() utan att den är allokerad???


    gaaaaaaaaaah

    Det Måste finnas något skydd mot att köra funktioner på en klass som inte är allokerad


    Relaterat?:
    https://stackoverflow.com/questions/18686519/accessing-member-function-after-calling-delete-to-an-object?noredirect=1&lq=1


    De säger man ska använda SMART pointers inte raw pointers
    haha exakt för den här anledningen....

        "Undefined Behaviour"
    */
    /*


    bool FileManager::checkYoSelf(void *ptr)
    {
        if(ptr == nullptr) {
            return false;
        }
        return false;
    }


    and then in every function (phew....), do this as the first thing you do:

        if(!checkYoSelf( (void *)this)) {
            return false;
        }


        Not something I'd want to have... So I decided to just fix the problem
        by allocating things in right order, maybe I'll have some nullptr checks in a verifiction
        function at Core.
    */



FileManager::FileManager()
{
}




// Reads a textfile which contains numbers on each line: 001,005,007\n
//                                                       001,001,000\n
// Puts them all into a matrix
// Parses the Matrix of integers and creates Blocks
// while parsing, puts them in a particular order which rendering in the isometric view
/// \return A complete HurkaMap containing the BlockList which has blocks put in the right order for rendering
//
// docs: readRegularFile.png
// docs: C:\github\FAT64_MEDIA_TOO_BIG_FOR_GITHUB\Parsing_Matrix_to_BlockList.mp4
// (-+)



// Wishlist: Having a BlockList for every Layer :3 would be easier to FIND where to insert and manipulate BLOCKs



//gå igenom readRegularFile och se hur den kan göra matrix**  lika stor som GameMatrix
HurkaMap *FileManager::readRegularFile(std::string _filename, int debugLevel, GameMatrix *gameMatrix)
{

    // Returnobject HurkaMap
    HurkaMap *resultMap = nullptr;  // Allocated later when we have the matrix of objects (001,007,... etc)


    std::string ind1 = "   ";
    std::string ind2 = "      ";
    std::string ind3 = "         ";


    // The blocklist will be attached to the resultmap if all goes well

    std::list<Block *> blockList;

    std::ifstream infile;
    std::string line;

    int mapRows = 0;
    int mapCols = 0;



    if(debugLevel > 0) {
        std::cout << "\n\n*** readRegularFile( " << _filename << ") \n";
    }




    /// Verify the File and get mapRows and mapCols


    if(!verifyFile(_filename, &mapRows, &mapCols, debugLevel)) {
        std::cout << "ERROR " << cn << " unable to verify the file, exiting!\n";
        return nullptr;
    }

    if(mapRows > gameMatrix->getRows()) {
        std::cout << "ERROR " << cn << " file loaded is larger in height than the gamematrix allows for!\n";
        return nullptr;
    }

    if(mapCols > gameMatrix->getCols()) {
        std::cout << "ERROR " << cn << " file loaded is larger in width than the gamematrix allows for!\n";
        return nullptr;
    }



    if(debugLevel > 0) {
        std::cout << ind1 << "The file has ROWS=" << mapRows << ", COLS=" << mapCols << "\n";
    }


    /// Open the File here

    infile.open(_filename);


    /// Get the Rows and Columns out of the map

    if (infile.is_open())
    {

            int **matrix = allocateMatrix(gameMatrix->getRows(), gameMatrix->getCols());

            /// Read lines from the file
            /// For every number (001,002,...) , put it in the matrix

            if(debugLevel > 0) {
                std::cout << "\n" << ind1 << "Read file and put content into a Matrix\n";
            }

            unsigned int currRow = 0;
            unsigned int currCol = 0;
            unsigned int w;
            unsigned int offset = 0;

            while (std::getline(infile, line))
            {

                // Parse this line

                while( (offset+4) <= line.size())
                {

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

                std::cout << "\n\n";

                dumpMatrix(matrix, gameMatrix->getRows(), gameMatrix->getCols(), ind2);
            }

            // Create output object now that we have the matrix
            resultMap = new HurkaMap(_filename, matrix, gameMatrix->getRows(), gameMatrix->getCols());

            currRow = 0;
            currCol = 0;






            /// Now get it out to blocklists

            if(debugLevel >= 2) {

                std::cout << "\n" << ind1 << "Parse Matrix into blocklists\n";
                std::cout << ind1 <<  "---------------------\n";

            }





           /// Parse the texture matrix in a particular way to get them in the way they should be rendered

           /// PLEASE See docs:

           // C:\github\FAT64_MEDIA_TOO_BIG_FOR_GITHUB\Parsing_Matrix_to_BlockList.mp4






           int yUp = 0;
           int yDown = 0;

           int xDownRight = 0;
           int xRight = 0;

           std::string textureName;


           // Loop 1
           if(debugLevel >= 2) {
                std::cout << ind2 << "Loop 1:\n" << ind2<< "-----------\n";




               std::cout << ind3 << "yDown, yUp     xRight, xDownRight\n";
               std::cout << ind3 << "------ ----   -------   ------- \n";
           }


           while(yDown < resultMap->getRows())
           {

               while(yUp > -1 && xRight < resultMap->getCols())
               {

                   // row = yUp
                   // col = xRight


                   if(matrix[yUp][xRight] != 0) {

                       TextureManager *textureMgr;
                       textureMgr = textureMgr->getInstance();
                       textureName = textureMgr->getTextureNameByIndex( matrix[yUp][xRight] );


                       Block *block  = new Block(new HPos(yUp, xRight, USE_ISO), textureName);

                       blockList.push_back(block);

                   }


                   if(debugLevel >=2) { std::cout << ind3 <<  yDown << ",     " << yUp << "          " << xRight << ",       " << xDownRight << "\n"; }

                   yUp--;
                   xRight++;
               }



               yDown++;

               xRight = 0;
               yUp = yDown;
           }




           // Loop 2

           yUp = resultMap->getRows()-1;    // Start at the bottom, and traverse right in this "loop 2"




           if(debugLevel >= 2) {
               std::cout << "\n\n" << ind2 << "Loop 2:\n" << ind2 << "----------\n";
               std::cout << ind3;
           }

           xDownRight++;

           xRight = xDownRight;


           while(xDownRight < resultMap->getCols())
           {

               while(yUp > -1 && xRight < resultMap->getCols())
               {
                    // row = yUp
                    // col = xRight

                    if(matrix[yUp][xRight] != 0) {

                        TextureManager *textureMgr;
                        textureMgr = textureMgr->getInstance();

                        textureName = textureMgr->getTextureNameByIndex( matrix[yUp][xRight] );    // 001 -> "HOUSE001" for instance

                        Block *block  = new Block( new HPos(yUp, xRight, USE_ISO), textureName);

                        blockList.push_back(block);


                    }

                    if(debugLevel >= 2) { std::cout <<  "(" << yUp << ", " << xRight << ") ";}

                    yUp--;
                    xRight++;



               }



               if(debugLevel >= 2) {
                  std::cout << "\n";
                  std::cout << ind3;
               }


               xDownRight++;

               xRight = xDownRight;
               yUp = resultMap->getRows()-1;
            }


            /// Put that blocklist into a HurkaMap that we will return
            if(resultMap == nullptr)  {
                std::cout << "ERROR " << cn << " resultMap is null inside readRegularFile(), stopping !\n";
                return nullptr;
            }

            resultMap->putBlockList(blockList);

            /// Complete!!

    } else {
        std::cout << "ERROR " << cn << ": Could not open file \"" << _filename << "\"!\n";
        infile.close();
        return nullptr;
    }


    if(debugLevel > 1) {
        resultMap->dump("   ");
        std::cout << "\n readRegularFile complete **** \n\n";
    }

    infile.close();
    return resultMap;
}






// Given rows and cols, check that they actually contain those constraints
// IF they are nullptr that check is not made


// Wishlist: Please check that every line ends with ","
//
// Wishlist: Check that the map is an NxN matrix
// (-+)
bool FileManager::verifyFile(std::string _filename, int *rows, int *cols, int debugLevel)
{




    std::string ind = "   ";
    int nrCharsPerElement = 4;
    int nrElementsN = 0;
    int nrElementsM = 0;
    unsigned int firstLineLength = 0;
    std::string line;


    if(debugLevel > 0) {
        std::cout << "\n\n**** VerifyFile!\n";
    }


    /// Open the file
    std::ifstream infile(_filename);

    if(infile) {

        /// Get the first line
        std::getline(infile, line);


        nrElementsM++;
        firstLineLength = line.length();

        if(firstLineLength%nrCharsPerElement!=0) {  // is it equyally divisable by "4" for instance...?
            std::cout << "ERROR " << cn << ": Line not divisible by " << nrCharsPerElement << ", missing comma? missing leading zeroes? every number in the format of 001,002,003?\n";
            infile.close();
            return false;
        }

        nrElementsN = line.length()/nrCharsPerElement;

        if(debugLevel > 0) {
            std::cout << ind << " nr of elements on this line = " << nrElementsM << "\n";
        }





        /// Check rest of lines in file to see if they are the same length

        while (std::getline(infile, line))
        {
            if(debugLevel > 0) { std::cout << ind << "\"" << line << "\"  linelength=" << line.length() <<"\n"; }

            nrElementsM++;  // Increase M counter (Y, vertical axis)

            if(line.length()!= firstLineLength) {
                std::cout << "ERROR " << cn << ": Line is not same length as the first one !\n";
                infile.close();
                return false;
            }

        }



        /// Check that we have equal length on both axises (MxM)

        if(nrElementsM != nrElementsN) {
            std::cout << "ERROR " << cn << ": The file should contain an M x M matrix... " <<
              " The amount of elements on one axis should be same as the other axis. This is an " << nrElementsM << "x" << nrElementsN << "!\n";
            infile.close();
            return false;
        }

        if(rows != nullptr) {

            (*rows) = nrElementsM;
        }

        if(cols != nullptr) {
            (*cols) = nrElementsN;
        }


    } else {
        // Could not open the file
        std::cout << "ERROR !!!" << cn << ": Could not open file \"" << _filename << "\"!\n";

        return false;
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





















