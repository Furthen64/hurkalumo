#include "Utils.hpp"



int **allocateMatrix(int rows, int cols)
{

    if(rows > 10000 || cols > 10000) {
            std::cout << "ERROR cannot allocate matrix of that size! " << rows << ", " << cols << "\n";
        return nullptr;
    }

std::cout << "allocateMatrix(" << rows << ", " << cols << ")\n";
    int** matrix = new int*[rows];
    if (rows)
    {
        matrix[0] = new int[rows * cols];
        for (int i = 1; i < rows; ++i)
            matrix[i] = matrix[0] + i * cols;
    }



    return matrix;

}

