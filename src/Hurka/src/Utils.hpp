#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <time.h>
#include <random>


#ifdef __linux__
    #include <unistd.h>
    #define getCurrentDir getcwd
#elif _Win32_
    #include <direct.h>
    #define getCurrentDir _getcwd
#endif

#include "HurkaMatrix.hpp"

int **allocateMatrix(int rows, int cols);
void dumpMatrix(int** matrix, int rows, int cols, std::string);
int **copySubMatrix(int **srcMtx, int srcRows, int srcCols, int startY, int startX, int height, int width,int debugLevel);
void initRandomizer();
int randBetween(int lowNr, int highNr);
void waitForInput();
std::string getFullUri(std::string relativeUri);
//bool testFileManager(int debugLevel);
//bool testList(int debugLevel);
//bool integrityTesting();
#endif
