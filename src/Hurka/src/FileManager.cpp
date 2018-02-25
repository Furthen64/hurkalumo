#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "FileManager.hpp"

/// //////////////////////////////////////////////
/// (--) FileManager



FileManager::FileManager()
{


}

// Regular file is the matrix as you would see this game from above without isometricness
bool FileManager::readRegularFile(std::string _filename)
{
    std::ifstream file1;
    file1.open(_filename);

    if (file1.is_open()) {

    } else {
        std::cout << "ERROR " << cn << ": Could not open file \"" << _filename << "\"!\n";
        file1.close();
        return false;
    }

    file1.close();
    return true;
}

bool FileManager::verifyFile(std::string _filename)
{

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
    int firstLineLength = 0;


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
        nrElementsM++;

        if(line.length()%divisor!=0) {
            std::cout << "ERROR " << cn << ": Line not divisible by " << divisor << ", missing comma? missing leading zeroes?\n";
            infile.close();
            return false;
        }

        std::cout << "\"" << line << "\"  linelength=" << line.length() <<"\n";






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




















