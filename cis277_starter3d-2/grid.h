#ifndef GRID_H
#define GRID_H
#include "mesh.h"

class Grid
{
public:
    Grid();
    void generateXZGrid(float stepSize);
    void generateXYGrid(float stepSize);
    void generateYZGrid(float stepSize);
    void generateXYZGrid(float stepSize);

    std::vector<float>* points;
    std::vector<float>* colors;
    std::vector<unsigned int>* indices;
    int vertexCount;
};

#endif // GRID_H
