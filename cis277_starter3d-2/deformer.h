#ifndef DEFORMER_H
#define DEFORMER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <QInputDialog>
#include <QErrorMessage>
#include "face.h"
#include "halfedge.h"
#include "mesh.h"
#include "myglwidget.h"
#include "vertex.h"


using namespace std;

class Deformer
{
private:
    const char* filename;
    Mesh* mesh;
    /// obj vertices, faces, current material, all materials
    map<int, Vertex*> vertices;
    vector<Face*> faces;
    string currMtl;
    map<string, vector<vector<int> > > materials;

    /// lattice points
    vector<glm::vec3> pts;

    void createMesh();
public:
    Deformer();
    Deformer(string filename);
    Mesh* getMesh();
    void exportMesh(string filename, Mesh* m);
};

#endif // DEFORMER_H
