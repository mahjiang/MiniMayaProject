#ifndef MESH_H
#define MESH_H
#include "halfedge.h"
#include "face.h"
#include <vector>
#include <set>
#include <math.h>
#include "nearlyequal.h"

class Mesh
{
public:
    Mesh();

    /// ID values
    int nextHEId;
    int nextFId;
    int nextVId;
    int getVid();
    int getFid();
    int getHEid();
    void resetIds();

    /// buffer object counts
    int vertexCount;
    int triangleCount;
    unsigned int indexCount;
    int getVertexCount();
    int getTriangleCount();
    unsigned int getIndexCount();

    /// buffer face arrays and counts
    std::vector<float>* points;
    std::vector<float>* normals;
    std::vector<float>* colors;
    std::vector<unsigned int>* indices;
    /// container of mesh faces
    std::vector<Face*>* faceList;

    /// buffer active points arrays
    unsigned int activeVertexCount;
    std::vector<float>* activeVertexPoints;
    std::vector<float>* activeVertexColors;
    std::vector<unsigned int>* activeVertexIndices;
    std::set<Vertex*>* getVertexSet();


    /// face list operations
    bool isEmpty();
    int faceCount();
    void addFace(Face * f);
    Face* faceAt(int i);
    int removeFace(Face * f);
    void clearFaceList();

    /// Draw Mesh Functions
    glm::vec3 getNormal(HalfEdge * e);
    void bufferFace(Face * f);
    void bufferActiveVertices();
    void updateMesh();

    /// active element colors
    glm::vec3 activeFaceColor;
    glm::vec3 activeEdgeColor;
    glm::vec3 activeVertexColor;
    glm::vec3 hoverFaceColor;


    /// Mesh Operations
    void triangulate(Face * f);
    void addEdge(Face* FA, Face* FB, Vertex* V);
    void addVertex(HalfEdge* HE0);
    void deleteVertex(HalfEdge * he);

    /// Ray Polygon Intersection
    glm::vec3 getTriangleNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C);
    float rayPlaneIntersection(glm::vec3 N, glm::vec3 P0, glm::vec3 P1, glm::vec3 S);
    float areaOfTri(glm::vec3 A, glm::vec3 B, glm::vec3 C);
    bool pointInsideTri(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C);
    void rayTriangleIntersection(glm::vec3 P0, glm::vec3 P1, Vertex* A, Vertex* B, Vertex* C);

    Face* rayPolygonInteresction(glm::vec3 A, glm::vec3 B);

    void activateFace(Face* f);
    void hoverFace(Face* f);

    void activateClosestEdge(glm::vec3 pWorld);
    void activateVertex(Vertex* v);


    // ------------------------ CATMULL-CLARK SUBDIVISION --------------------
    std::vector<glm::vec3> facePoints;
    std::vector<glm::vec3> edgePoints;
    std::vector<glm::vec3> origVertices;
    std::vector<glm::vec3> updatedPoints;
    glm::vec3 facePoint(Face *f);
    glm::vec3 edgePoint(HalfEdge *he);
    glm::vec3 updatedPoint(HalfEdge *he);
    void subdivide();
    void plane();

};

#endif // MESH_H
