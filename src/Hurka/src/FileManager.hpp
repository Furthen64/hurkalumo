#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>

#include <exception>


#ifdef __linux__
    #include <unistd.h>
    #define getCurrentDir getcwd
#elif _Win32_
    #include <direct.h>
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

class FileManager
{
public:
    FileManager();

    bool verifyFile(std::string fullUri, int *rows, int *cols, int debugLevel);

    HurkaMap *readRegularFile(std::string fullUri, int debugLevel, GameMatrix *);

    bool saveRegularFile(std::string fullUri, int debugLevel, HurkaMap *, GameMatrix *);

    void printWorkingDir();


    static std::string convertToOutputString(int nr)
    {
        std::string str = "000";


        if(nr <100) {
            if(nr <10) {
                // 0-9

                str = "00";
                str += std::to_string(nr);



            } else {
                // 10->99
                str = "0";
                str += std::to_string(nr);
            }
        } else {
            return std::to_string(nr);
        }


        return str;
    }

private:

    std::string cn = "FileManager.cpp";
};

#endif
