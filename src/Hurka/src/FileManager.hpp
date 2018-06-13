#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
//#include <direct.h>   Not in linux. Really used anymore?
#include <exception>


#ifdef __linux__
    #define getCurrentDir getcwd
    #include <unistd.h>
#elif _Win32_
    #define getCurrentDir _getcwd
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "HurkaMap.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

using namespace sf;

class FileManager
{
public:
    FileManager();
    bool verifyFile(std::string _filename, int *rows, int *cols, int debugLevel);

    void printWorkingDir();
    HurkaMap *readRegularFile(std::string _filename, int debugLevel, GameMatrix *);

private:

    std::string cn = "FileManager.cpp";
};

#endif
