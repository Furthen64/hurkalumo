#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <time.h>
#include <random>

#include <SFML/Graphics.hpp>

#include "HurkaMatrix.hpp"

using namespace sf;


void dumpPosition(Vector2f iso_pos);
int **allocateMatrix(int rows, int cols);
void dumpMatrix(int** matrix, int rows, int cols, std::string);
void initRandomizer();
int randBetween(int lowNr, int highNr);
void waitForInput();

//bool testFileManager(int debugLevel);
//bool testList(int debugLevel);
//bool integrityTesting();
#endif
