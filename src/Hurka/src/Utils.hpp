#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <time.h>
#include <random>

#include "HurkaMatrix.hpp"

int **allocateMatrix(int rows, int cols);
void dumpMatrix(int** matrix, int rows, int cols, std::string);
int **copySubMatrix(int **srcMtx, int srcRows, int srcCols, int startY, int startX, int height, int width,int debugLevel);
void initRandomizer();
int randBetween(int lowNr, int highNr);
void waitForInput();

//bool testFileManager(int debugLevel);
//bool testList(int debugLevel);
//bool integrityTesting();
#endif
