#include "grid.h"
#include "mesh.h"

using namespace std;

Grid::Grid() {
    points = new std::vector<float>;
    colors = new std::vector<float>;
    indices = new std::vector<unsigned int>;
    vertexCount = 0;
}

void Grid::generateYZGrid(float stepSize) {
  // Y Grid
  //  cout << stepSize << endl;
    int size = 2;
    if (stepSize < 350) stepSize = 1;
    else if (stepSize > 450 && stepSize < 550) size = 2;
    else if (stepSize > 550 && stepSize <= 650) size = 4;
    else if (stepSize > 650) size = 8;

    for (float i = -100; i <= 100; i++) {
        points->push_back(0.0f);
        points->push_back(-100.0f);
        points->push_back(i/size);
        points->push_back(1.0f);
        vertexCount++;

        points->push_back(0.0f);
        points->push_back(+100.0f);
        points->push_back(i/size);
        points->push_back(1.0f);
        vertexCount++;

        points->push_back(0.0f);
        points->push_back(i/size);
        points->push_back(-100.0f);
        points->push_back(1.0f);
        vertexCount++;


        points->push_back(0.0f);
        points->push_back(i/size);
        points->push_back(+100.0f);
        points->push_back(1.0f);
        vertexCount++;
    }

    for (float i = -100; i <= 100; i++) {
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
    }

    for (unsigned int i = 0; i <vertexCount; i++) {
        indices->push_back(i);
    }
}

void Grid::generateXYGrid(float stepSize) {
    int size = 2;
    if (stepSize < 350) stepSize = 1;
    else if (stepSize > 450 && stepSize < 550) size = 2;
    else if (stepSize > 550 && stepSize <= 650) size = 4;
    else if (stepSize > 650) size = 8;

    for (float i = -100; i <= 100; i++) {
        points->push_back(-100.0f);
        points->push_back(i/size);
        points->push_back(0.0f);
        points->push_back(1.0f);
        vertexCount++;

        points->push_back(+100.0f);
        points->push_back(i/size);
        points->push_back(0.0f);
        points->push_back(1.0f);
        vertexCount++;

        points->push_back(i/size);
        points->push_back(-100.0f);
        points->push_back(0.0f);
        points->push_back(1.0f);
        vertexCount++;


        points->push_back(i/size);
        points->push_back(+100.0f);
        points->push_back(0.0f);
        points->push_back(1.0f);
        vertexCount++;
    }

    for (float i = -100; i <= 100; i++) {
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
    }

    for (unsigned int i = 0; i <vertexCount; i++) {
        indices->push_back(i);
    }
}

void Grid::generateXZGrid(float stepSize) {
    // Y Grid
  //  cout << stepSize << endl;
    int size = 2;
    if (stepSize < 350) stepSize = 1;
    else if (stepSize > 450 && stepSize < 550) size = 2;
    else if (stepSize > 550 && stepSize <= 650) size = 4;
    else if (stepSize > 650) size = 8;

    for (float i = -100; i <= 100; i++) {
        points->push_back(-100.0f);
        points->push_back(0.0f);
        points->push_back(i/size);
        points->push_back(1.0f);
        vertexCount++;

        points->push_back(+100.0f);
        points->push_back(0.0f);
        points->push_back(i/size);
        points->push_back(1.0f);
        vertexCount++;

        points->push_back(i/size);
        points->push_back(0.0f);
        points->push_back(-100.0f);
        points->push_back(1.0f);
        vertexCount++;


        points->push_back(i/size);
        points->push_back(0.0f);
        points->push_back(+100.0f);
        points->push_back(1.0f);
        vertexCount++;



    }

    for (float i = -100; i <= 100; i++) {
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);

        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
        colors->push_back(1.0f);
    }

    for (unsigned int i = 0; i <vertexCount; i++) {
        indices->push_back(i);
    }
}



//void Grid::generateXYZGrid(float stepSize) {
//    int size = 2;
//    if (stepSize < 350) stepSize = 1;
//    else if (stepSize > 450 && stepSize < 550) size = 2;
//    else if (stepSize > 550 && stepSize <= 650) size = 4;
//    else if (stepSize > 650) size = 8;

//    for (float i = -5; i <= 5; i++) {
//        points->push_back(-5.0f);
//        points->push_back(i/size);
//        points->push_back(0.0f);
//        points->push_back(1.0f);
//        vertexCount++;

//        points->push_back(+5.0f);
//        points->push_back(i/size);
//        points->push_back(0.0f);
//        points->push_back(1.0f);
//        vertexCount++;

//        points->push_back(i/size);
//        points->push_back(-5.0f);
//        points->push_back(0.0f);
//        points->push_back(1.0f);
//        vertexCount++;

//        points->push_back(i/size);
//        points->push_back(+5.0f);
//        points->push_back(0.0f);
//        points->push_back(1.0f);
//        vertexCount++;

//        points->push_back(0.0f);
//        points->push_back(i/size);
//        points->push_back(-5.0f);
//        points->push_back(1.0f);
//        vertexCount++;

//        points->push_back(0.0f);
//        points->push_back(i/size);
//        points->push_back(+5.0f);
//        points->push_back(1.0f);
//        vertexCount++;

//        points->push_back(0.0f);
//        points->push_back(-5.0f);
//        points->push_back(i/size);
//        points->push_back(1.0f);
//        vertexCount++;

//        points->push_back(0.0f);
//        points->push_back(+5.0f);
//        points->push_back(i/size);
//        points->push_back(1.0f);
//        vertexCount++;


//    }

//    for (float i = -5; i <= 5; i++) {
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);
//        colors->push_back(1.0f);

//    }

//    for (unsigned int i = 0; i <vertexCount; i++) {
//        indices->push_back(i);
//    }
//}

