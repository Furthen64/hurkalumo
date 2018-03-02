#ifndef FILEMANAGER_H
#define FILEMANAGER_H



#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "HurkaMap.hpp"

#include "Constants.hpp"


/// //////////////////////////////////////////////
/// (-+) GameMatrix is the grid with all the visible sprites oh shit, its the whole game set right?
/// Or something. Haven't decided yet.
///

using namespace sf;



class FileManager
{
public:

    FileManager();
    HurkaMap readRegularFile(std::string _filename, TextureManager *textureMgr);
    bool verifyFile(std::string _filename, int *rows, int *cols);
    void printWorkingDir();


private:
    std::string cn = "FileManager.cpp";
};

#endif
