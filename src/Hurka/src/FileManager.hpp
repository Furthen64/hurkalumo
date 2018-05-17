#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
#include <direct.h>
#include <exception>
//#define GetCurrentDir _getcwd

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
