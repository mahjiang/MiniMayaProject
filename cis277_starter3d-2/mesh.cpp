#include "mesh.h"
#include "nearlyequal.h"
#include "glm/gtx/string_cast.hpp"


/// Constructor
using namespace std;
Mesh::Mesh() {
    // counts
    nextHEId = 0;
    nextFId = 0;
    nextVId = 0;
    vertexCount = 0;
    triangleCount = 0;
    indexCount = 0;

    // buffer face arrays
    points = new std::vector<float>;
    normals = new std::vector<float>;
    colors = new std::vector<float>;
    indices = new std::vector<unsigned int>;
    faceList = new std::vector<Face*>;

    // buffer active points arrays
    activeVertexCount = 0;
    activeVertexPoints = new std::vector<float>;
    activeVertexColors = new std::vector<float>;
    activeVertexIndices = new std::vector<unsigned int>;

    // active element colors
    activeFaceColor = glm::vec3(.5,.5,1);
    activeEdgeColor = glm::vec3(.5, 1, .5);
    activeVertexColor = glm::vec3(1,.5,.5);
    hoverFaceColor = glm::vec3(.65, .65, 1);
}

/// buffer object counts

int Mesh::getVid() {
    int holdId = nextVId;
    nextVId++;
    return holdId;
}

int Mesh::getFid() {
    int holdId = nextFId;
    nextFId++;
    return holdId;
}

int Mesh::getHEid() {
    int holdId = nextHEId;
    nextHEId++;
    return holdId;
}

void Mesh::resetIds() {
    nextHEId = 0;
    nextFId = 0;
    nextVId = 0;
}


int Mesh::getVertexCount() {
    return vertexCount;
}

int Mesh::getTriangleCount() {
    return triangleCount;
}

unsigned int Mesh::getIndexCount() {
    return indexCount;
}

/// face list operations

int Mesh::faceCount() {
    return faceList->size();
}

void Mesh::addFace(Face *f) {
    faceList->push_back(f);
}

Face* Mesh::faceAt(int i) {
    return faceList->at(i);
}

int Mesh::removeFace(Face * f) {
    int i;
    bool found = false;
    for (i = 0; i < faceCount(); i++) {
        if (faceAt(i) == f) {
            found = true;
            break;
        }
    }
    if (found) {
        faceList->erase(faceList->begin()+i);
        return 0;
    } else return 1;
}

void Mesh::clearFaceList() {
    faceList->clear();
}




/// Draw Mesh Functions

void Mesh::bufferFace(Face *f) {
    // get face vertex count
    HalfEdge * he0 = f->halfedge;
    HalfEdge * curr = he0->next;
    int faceVertexCount = 1;
    while (curr != he0) {
        faceVertexCount++;
        curr = curr->next;
    }
    vertexCount += faceVertexCount;

    // prepare points vector
    curr = he0;
    for (int i = 0; i < faceVertexCount; i++) {
        points->push_back(curr->vert->getX());
        points->push_back(curr->vert->getY());
        points->push_back(curr->vert->getZ());
        points->push_back(1.0f);
        curr = curr->next;
    }

    // prepare colors vector
    curr = he0;
    for (int i = 0; i < faceVertexCount; i++) {
        if (f->active) {
            colors->push_back(activeFaceColor[0]);
            colors->push_back(activeFaceColor[1]);
            colors->push_back(activeFaceColor[2]);
        } else if (f->hover){
            colors->push_back(hoverFaceColor[0]);
            colors->push_back(hoverFaceColor[1]);
            colors->push_back(hoverFaceColor[2]);
        } else {
            colors->push_back(f->getColorVec()[0]);
            colors->push_back(f->getColorVec()[1]);
            colors->push_back(f->getColorVec()[2]);
        }
        colors->push_back(1.0f);
        curr = curr->next;
    }

    // TODO: fix normals
    // prepare normals vector
    curr = he0;
    for (int i = 0; i < faceVertexCount; i++) {
        normals->push_back(0.0f);
        normals->push_back(0.0f);
        normals->push_back(1.0f);
        normals->push_back(0.0f);
        curr = curr->next;
    }

    // prepare index vector
    unsigned int index0 = indexCount;
    assert(faceVertexCount >= 3);
    for (unsigned int i = 0; i < (faceVertexCount - 2); i++) {
        indices->push_back(index0);
        indexCount++;
        indices->push_back(indexCount);
        indexCount++;
        indices->push_back(indexCount);

        indexCount--;
        triangleCount++;
    }
    indexCount = indexCount + 2;
}

std::set<Vertex*>* Mesh::getVertexSet() {
    std::set<Vertex*>* vertexSet = new std::set<Vertex*>;
    for (int i = 0; i < faceCount(); i++) {
        HalfEdge * he0 = faceAt(i)->getHE();
        HalfEdge * curr = he0->next;
        int vertCount = 1;
        while (curr != he0) {
            vertCount++;
            curr = curr->next;
        }

        curr = he0;
        for (int i = 0; i < vertCount; i++) {
            vertexSet->insert(curr->vert);
            curr = curr->next;
        }
    }
    return vertexSet;
}

void Mesh::bufferActiveVertices() {
    std::set<Vertex*>* vertexSet = getVertexSet();
    std::set<Vertex*>::iterator it;
    int count = 0;
    for (it=vertexSet->begin(); it!=vertexSet->end(); ++it) {
        Vertex * v = *it;
        if (v->active == true) {
            activeVertexCount++;
            activeVertexPoints->push_back(v->getX());
            activeVertexPoints->push_back(v->getY());
            activeVertexPoints->push_back(v->getZ());
            activeVertexPoints->push_back(1.0);

            activeVertexColors->push_back(activeVertexColor[0]);
            activeVertexColors->push_back(activeVertexColor[1]);
            activeVertexColors->push_back(activeVertexColor[2]);
            activeVertexColors->push_back(1.0);

            activeVertexIndices->push_back(count);
            count++;
        }
    }
}

void Mesh::updateMesh() {
    vertexCount = 0;
    triangleCount = 0;
    indexCount = 0;

    points->clear();
    normals->clear();
    colors->clear();
    indices->clear();

    activeVertexCount = 0;
    activeVertexPoints->clear();
    activeVertexColors->clear();
    activeVertexIndices->clear();
    // buffer each face
    for (int i = 0; i < faceCount(); i++) {
        bufferFace(faceAt(i));
    }
    indexCount--;
    bufferActiveVertices();
    if (activeVertexCount > 1) activeVertexCount--;
}


// ************************* mesh operations ***********************************

void Mesh::triangulate(Face *f) {
    if (f->getFaceVertexCount() != 4) {
        std::cout << "splitQuad ERROR: face does not have 4 edges" << std::endl;
        return;
    }
    HalfEdge * he0 = f->getHE();
    Face *f2 = new Face(getFid());
    f2->setColorVec(glm::vec3(.7,0,.5));
    HalfEdge * hel = new HalfEdge(getHEid()); // he New Left
    HalfEdge * her = new HalfEdge(getHEid()); // he New Right
    hel->face = f;
    her->face = f2;
    //f still points to its starting edge HE0
    f2->halfedge = her; //new f2 points to HER as its new starting edge

    hel->sym = her;
    her->sym = hel;

    hel->vert = he0->next->next->vert;
    hel->next = he0->next->next->next;

    her->next = he0->next;
    her->next->face = f2;
    her->next->next->next = her;
    her->next->next->face = f2;

    he0->next = hel;
    her->vert = he0->vert;
    addFace(f2);
}

void Mesh::addEdge(Face* FA, Face* FB, Vertex* V) {

    // find HEA0 and HEB0
    HalfEdge *A_starter = FA->getHE();
    HalfEdge *B_starter = FB->getHE();

    HalfEdge *HEA0 = A_starter;
    HalfEdge *HEB0 = B_starter;

    if (HEA0->vert != V) {
        HalfEdge *temp = FA->getHE()->next;
        while (temp != A_starter) {
            if (temp->vert == V)
                HEA0 = temp;
            temp = temp->next;
        }
    }
    if (HEB0->vert != V) {
        HalfEdge *temp = FB->getHE()->next;
        while (temp != B_starter) {
            if (temp->vert == V)
                HEB0 = temp;
            temp = temp->next;
        }
    }

    // ensure that FA and FB share V
    if (HEA0->vert != HEB0->vert) {
        std::cout << "addEdge ERROR: FA and FB do not share V" << std::endl;
        return;
    }
    // ensure that V is not an endpoint of an edge that FA and FB share
    if (HEA0->next->sym == HEB0 || HEA0 == HEB0->next->sym) {
        std::cout << "addEdge ERROR: V is on an edge shared by FA and FB" << std::endl;
        return;
    }

    Vertex *V1 = new Vertex(getVid()); // V1 should start at the same coordinates as V
    V1->setPosVec(glm::vec3(V->getX(), V->getY(), V->getZ()));

    HalfEdge *HENL = new HalfEdge(getHEid()); // HE New Left
    HalfEdge *HENR = new HalfEdge(getHEid()); // HE New Right

    HENL->next = HEA0->next;
    HEA0->next = HENL;
    HENL->vert = V1;
    HENL->face = FA;
    HENL->sym = HENR;

    HENR->next = HEB0->next;
    HEB0->next = HENR;
    HENR->vert = HEA0->vert;
    HEB0->vert = V1;
    HENR->face = FB;
    HENR->sym = HENL;

    HENL->next->sym->vert = V1;
}


void Mesh::addVertex(HalfEdge* HE0) {

    Vertex *VN = new Vertex(getVid()); // New Vertex VN will sit at the midpoint of our edge

    HalfEdge *HE1 = HE0->next; // HE1 next HalfEdge to get next vertex
    HalfEdge *HE2 = HE1->sym; // HE2 other side of edge

    HalfEdge *HENL = new HalfEdge(getHEid()); // HE New Left
    HalfEdge *HENR = new HalfEdge(getHEid()); // HE New Right

    HENL->next = HE1;
    HE0->next = HENL;
    HENR->next = HE2->next;
    HE2->next = HENR;

    HENL->sym = HENR;
    HENR->sym= HENL;

    HENL->face = HE0->face;
    HENR->face = HE2->face;

    HENL->vert = VN;
    HENR->vert = HE2->vert;
    HE2->vert = VN;

    float x = (HE0->vert->getX() + HE1->vert->getX())/2;
    float y = (HE0->vert->getY() + HE1->vert->getY())/2;
    float z = (HE0->vert->getZ() + HE1->vert->getZ())/2;
    VN->setPosVec(glm::vec3(x,y,z));
}


void Mesh::deleteVertex(HalfEdge *he) {

    HalfEdge * loop = he;
    do {
      Face * face = loop->face;
      int i;
      for (i = 0; i < faceCount(); i++) {
          if (faceAt(i) == face) break;
      }
      removeFace(face);
      if (loop->next->sym != NULL)
        loop = loop->next->sym;
    } while (loop != he);

}


// ************************* intersections ***********************************


// returns normal to the triangle
glm::vec3 Mesh::getTriangleNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    return glm::normalize(glm::cross(B - A, C - A));
}

float Mesh::rayPlaneIntersection(glm::vec3 N, glm::vec3 P0, glm::vec3 P1, glm::vec3 S) {
     return glm::dot(N, (S - P0))/glm::dot(N, (P1 - P0));
}

float distBetweenPoints(glm::vec3 p, glm::vec3 q) {
    float distSquared = (p[0] - q[0])*(p[0] - q[0]) + (p[1] - q[1])*(p[1] - q[1]) + (p[2] - q[2])*(p[2] - q[2]);
    return sqrt(distSquared);
}

float Mesh::areaOfTri(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    float sideAB = distBetweenPoints(A,B);
    float sideAC = distBetweenPoints(A,C);
    float sideBC = distBetweenPoints(B,C);
    float p = (sideAB + sideAC + sideBC) / 2.0;
    float areaSquared = p * (p - sideAB) * (p - sideAC) * (p - sideBC);
    return sqrt(areaSquared);
}

bool Mesh::pointInsideTri(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    float s = areaOfTri(A,B,C);
    float s1 = areaOfTri(P,A,B) / s;
    float s2 = areaOfTri(P,A,C) / s;
    float s3 = areaOfTri(P,B,C) / s;


    if (s1 >= 0 && s1 <= 1 && s2 >= 0 && s2 <= 1 && s3 >= 0 && s3 <= 1 &&
            s1 + s2 + s3 >= .999 &&  s1 + s2 + s3 <= 1.099) {
        return true;
    }
    else return false;
}

void Mesh::rayTriangleIntersection(glm::vec3 P0, glm::vec3 P1, Vertex* A, Vertex* B, Vertex* C) {

}

Face* Mesh::rayPolygonInteresction(glm::vec3 P0, glm::vec3 P1) {
    // for each face
    Face* intersectionFace = NULL;
    float smallestT = std::numeric_limits<float>::max();
    for (int i = 0; i < faceCount(); i++) {
        HalfEdge * he0 = faceAt(i)->getHE();
        HalfEdge * curr = he0->next;
        int faceVertexCount = 1;
        while (curr != he0) {
            faceVertexCount++;
            curr = curr->next;
        }

        HalfEdge* edgeB = he0->next;
        HalfEdge* edgeC = he0->next->next;
        glm::vec3 A = glm::vec3(he0->vert->getX(),he0->vert->getY(),he0->vert->getZ());
        glm::vec3 B;
        glm::vec3 C;
        for (int j = 0; j < faceVertexCount - 2; j++) {
            // get triangle vertices
            B = glm::vec3(edgeB->vert->getX(),edgeB->vert->getY(),edgeB->vert->getZ());
            C = glm::vec3(edgeC->vert->getX(),edgeC->vert->getY(),edgeC->vert->getZ());
            // get triangle normal
            glm::vec3 normal = getTriangleNormal(A,B,C);
            // get parameter t in interersection of ray and triangle plane
            float t = rayPlaneIntersection(normal, P0, P1, A);
            if (t <= 0 || glm::dot(normal, (P1 - P0)) == 0) continue; // check if face is behind ray or parallel to ray
            glm::vec3 P = P0 + t * P1;
            bool insideTri = pointInsideTri(P, A, B, C);
            if (insideTri && t < smallestT) {
                intersectionFace = faceAt(i);
                smallestT = t;
            }
            edgeB = edgeB->next;
            edgeC = edgeC->next;
        }
    }
    return intersectionFace;
}

void Mesh::activateFace(Face *f) {
    for (int i = 0; i < faceCount(); i++) {
        faceAt(i)->active = false;
    }
    if (f != NULL) f->active = true;
}

void Mesh::hoverFace(Face *f) {
    for (int i = 0; i < faceCount(); i++) {
        faceAt(i)->hover = false;
    }
    if (f != NULL) f->hover = true;
}

void Mesh::activateVertex(Vertex *vert) {
    set<Vertex*>* vertices = getVertexSet();
    std::set<Vertex*>::iterator it;
    for (it=vertices->begin(); it!=vertices->end(); ++it) {
        Vertex * v = *it;
        v->active = false;
    }
    vert->active = true;
}


void Mesh::activateClosestEdge(glm::vec3 pWorld) {
    set<Vertex*>* vertices = getVertexSet();
    Vertex* closestVertex;
    float closestDistance = std::numeric_limits<float>::max();
    std::set<Vertex*>::iterator it;
    for (it=vertices->begin(); it!=vertices->end(); ++it) {
        Vertex * v = *it;
        float dist = glm::distance(v->getPosVec(),pWorld);
        if (dist < closestDistance) {
            closestDistance = dist;
            closestVertex = v;
        }
    }
    activateVertex(closestVertex);
}



// **************************** Catmull-Clark ********************************

glm::vec3 Mesh::facePoint(Face *f) {
    HalfEdge * he0 = f->getHE();
    HalfEdge * curr = he0;

    int vertCount = 0;
    glm::vec3 ret = glm::vec3(0);

    do {
       vertCount++;
       ret += curr->vert->getPosVec();
       curr = curr->next;
    } while (curr != he0);

    return (ret / (float)vertCount);
}


glm::vec3 Mesh::edgePoint(HalfEdge *he) {
     glm::vec3 one = he->vert->getPosVec();

    HalfEdge *temp = he;
    while (temp->next != he) {
        temp = temp->next;
    }
    glm::vec3 two = temp->vert->getPosVec();

    if (he->sym == NULL) {
        cout << "plz no" << endl;
        return (one + two) / (float)2;
    }
    else {

        unsigned int i;
        for (i = 0; i < faceList->size(); i++) {
            if (faceAt(i) == he->face) break;
        }
        glm::vec3 thisFacePoint = facePoints.at(i);

        HalfEdge *s = he->sym;
        for (i = 0; i < faceList->size(); i++) {
            if (faceAt(i) == s->face) break;
        }
        glm::vec3 symFacePoint = facePoints.at(i);

        return (one + two + thisFacePoint + symFacePoint) / (float)4;
    }

}


glm::vec3 Mesh::updatedPoint(HalfEdge *he) {
/*
    glm::vec3 old_coords = he->vert->getPosVec();

    glm::vec3 avg_face_points;
    int n = 0;
    HalfEdge *loop = he;
    do {
        n++;
        // loop->face
        unsigned int i;
        for (i = 0; i < faceList->size(); i++) {
            if (faceAt(i) == loop->face) {
                avg_face_points += facePoints.at(i);
                break;
            }
        }
        if (loop->next->sym == NULL) break;
        loop = loop->next->sym;
    } while (loop != he);
    if (n == 0) avg_face_points = glm::vec3(0);
    else avg_face_points = avg_face_points / (float)n;

    glm::vec3 avg_mid_edges;
    loop = he;
    int m = 0;
    do {
        m++;
        // loop's mid point
        glm::vec3 one = loop->vert->getPosVec();
        HalfEdge *temp = loop;
        while (temp->next != loop) {                            // ** CUBE CRASHES
            temp = temp->next;
        }
        glm::vec3 two = temp->vert->getPosVec();

        avg_mid_edges += glm::vec3((one[0] + two[0])/2,
                                   (one[1] + two[1])/2,
                                   (one[2] + two[2])/2);

        if (loop->next->sym == NULL) {
            cout << "NOOOOO not for cube" << endl;
            m++;
            loop = loop->next;
            glm::vec3 three = loop->vert->getPosVec();
            avg_mid_edges += glm::vec3((one[0] + three[0])/2,
                                       (one[1] + three[1])/2,
                                       (one[2] + three[2])/2);
            break;
        }
        loop = loop->next->sym;
    } while (loop != he);

    cout << "m: "<<m << endl;
    if (m == 0) avg_mid_edges = glm::vec3(0);
    else avg_mid_edges = avg_mid_edges / (float)m;

    float m1 = ((float)m-3)/(float)m;
    float m2 = 1/(float)m;
    float m3 = 2/(float)m;

    glm::vec3 ret = (m1 * old_coords) + (m2 * avg_face_points) + (m3 * avg_mid_edges);

//    cout << old_coords[0] << old_coords[1] << old_coords[2] << endl;
//    cout << avg_face_points[0] << avg_face_points[1] << avg_face_points[2] << endl;
//    cout << avg_mid_edges[0] << avg_mid_edges[1] << avg_mid_edges[2] << endl;
//    cout << "m1 " << m1 << "m2 " << m2 << "m3 " << m3 << endl;
//    cout << "ret " << ret[0] << ret[1] << ret[2] << endl;
//    cout << " " << endl;

    return ret;*/

    glm::vec3 old_coords = he->vert->getPosVec();

    glm::vec3 avg_face_points;
    int n = 0;
    for (int i = 0; i < faceList->size(); i++) {
        HalfEdge *loop = faceAt(i)->halfedge;
        do {
            // loop->face
            if (he->vert->getPosVec() == loop->vert->getPosVec()) {
                n++;
                avg_face_points += facePoint(loop->face);
                break;
            }
            loop = loop->next;

        } while (loop != faceAt(i)->halfedge);
    }
    if (n == 0) avg_face_points = glm::vec3(0);
    else avg_face_points = avg_face_points / (float)n;
    cout << "n: "<< n << endl;


    glm::vec3 avg_mid_edges;
    HalfEdge *loop2 = he;
    int m = 0;
    do {
        m++;
        // loop2's mid point
        glm::vec3 one = loop2->vert->getPosVec();
        HalfEdge *temp = loop2;
        while (temp->next != loop2) {                            // ** CUBE CRASHES
            temp = temp->next;
        }
        glm::vec3 two = temp->vert->getPosVec();

        avg_mid_edges += glm::vec3((one[0] + two[0])/2,
                                   (one[1] + two[1])/2,
                                   (one[2] + two[2])/2);

        if (loop2->next->sym == NULL) {
            cout << "NOOOOO not for cube" << endl;
            m++;
            loop2 = loop2->next;
            glm::vec3 three = loop2->vert->getPosVec();
            avg_mid_edges += glm::vec3((one[0] + three[0])/2,
                                       (one[1] + three[1])/2,
                                       (one[2] + three[2])/2);
            break;
        }
        loop2 = loop2->next->sym;
    } while (loop2 != he);

    cout << "m: "<< m << endl;
    if (m == 0) avg_mid_edges = glm::vec3(0);
    else avg_mid_edges = avg_mid_edges / (float)m;

    float m1 = ((float)m-3)/(float)m;
    float m2 = 1/(float)m;
    float m3 = 2/(float)m;

    glm::vec3 ret = (m1 * old_coords) + (m2 * avg_face_points) + (m3 * avg_mid_edges);

    cout << old_coords[0] << old_coords[1] << old_coords[2] << endl;
    cout << avg_face_points[0] << avg_face_points[1] << avg_face_points[2] << endl;
    cout << avg_mid_edges[0] << avg_mid_edges[1] << avg_mid_edges[2] << endl;
    cout << "m1 " << m1 << "m2 " << m2 << "m3 " << m3 << endl;
    cout << "ret " << ret[0] << ret[1] << ret[2] << endl;
    cout << " " << endl;

    return ret;
}




void Mesh::subdivide() {

    facePoints.clear();
    edgePoints.clear();
    origVertices.clear();
    updatedPoints.clear();
    resetIds();

    for (unsigned int i = 0; i < faceList->size(); ++i) {
        facePoints.push_back(facePoint(faceAt(i)));
    }

    for (unsigned int i = 0; i < faceList->size(); ++i) {
        HalfEdge * he0 = faceAt(i)->getHE();
        HalfEdge * curr = he0->next;
        int vertCount = 1;
        while (curr != he0) {
            vertCount++;
            curr = curr->next;
        }
        curr = he0;
        for (int j = 0; j < vertCount; j++) {
            // curr is halfedge -- get its edgePoint / its vert's updatedPoint
            edgePoints.push_back(edgePoint(curr));
            origVertices.push_back(curr->vert->getPosVec());
            updatedPoints.push_back(updatedPoint(curr));

            curr = curr->next;
        }
    }

    // store colors
    std::vector<Face*> temp;
    for (int i = 0; i < faceCount(); i++)
        temp.push_back(faceAt(i));

    faceList->clear();

    // create new mesh
    for (unsigned int i = 0; i < facePoints.size(); i++) {

        std::vector<Vertex*> updatepts;
        std::vector<Vertex*> edgepts;

        Vertex *a = new Vertex(getVid(), updatedPoints.at(i*4)[0],
                updatedPoints.at(i*4)[1], updatedPoints.at(i*4)[2]);
        Vertex *b = new Vertex(getVid(), updatedPoints.at(i*4 + 1)[0],
                updatedPoints.at(i*4 + 1)[1], updatedPoints.at(i*4 + 1)[2]);
        Vertex *c = new Vertex(getVid(), updatedPoints.at(i*4 + 2)[0],
                updatedPoints.at(i*4 + 2)[1], updatedPoints.at(i*4 + 2)[2]);
        Vertex *d = new Vertex(getVid(), updatedPoints.at(i*4 + 3)[0],
                updatedPoints.at(i*4 + 3)[1], updatedPoints.at(i*4 + 3)[2]);
//        Vertex *a = new Vertex(getVid(), origVertices.at(i*4)[0],
//                origVertices.at(i*4)[1], origVertices.at(i*4)[2]);
//        Vertex *b = new Vertex(getVid(), origVertices.at(i*4 + 1)[0],
//                origVertices.at(i*4 + 1)[1], origVertices.at(i*4 + 1)[2]);
//        Vertex *c = new Vertex(getVid(), origVertices.at(i*4 + 2)[0],
//                origVertices.at(i*4 + 2)[1], origVertices.at(i*4 + 2)[2]);
//        Vertex *d = new Vertex(getVid(), origVertices.at(i*4 + 3)[0],
//                origVertices.at(i*4 + 3)[1], origVertices.at(i*4 + 3)[2]);

        updatepts.push_back(a);
        updatepts.push_back(b);
        updatepts.push_back(c);
        updatepts.push_back(d);

        Vertex * edge_point_ab = new Vertex(getVid(), edgePoints.at(i*4)[0],
                edgePoints.at(i*4)[1], edgePoints.at(i*4)[2]);
        Vertex * edge_point_bc = new Vertex(getVid(), edgePoints.at(i*4 + 1)[0],
                edgePoints.at(i*4 + 1)[1], edgePoints.at(i*4 + 1)[2]);
        Vertex * edge_point_cd = new Vertex(getVid(), edgePoints.at(i*4 + 2)[0],
                edgePoints.at(i*4 + 2)[1], edgePoints.at(i*4 + 2)[2]);
        Vertex * edge_point_da = new Vertex(getVid(), edgePoints.at(i*4 + 3)[0],
                edgePoints.at(i*4 + 3)[1], edgePoints.at(i*4 + 3)[2]);

        edgepts.push_back(edge_point_ab);
        edgepts.push_back(edge_point_bc);
        edgepts.push_back(edge_point_cd);
        edgepts.push_back(edge_point_da);

        Vertex * face_point = new Vertex(getVid(), facePoints.at(i)[0],
                facePoints.at(i)[1], facePoints.at(i)[2]);


        // create half edges
        HalfEdge * he0 = new HalfEdge(getHEid());
        HalfEdge * he1 = new HalfEdge(getHEid());
        HalfEdge * he2 = new HalfEdge(getHEid());
        HalfEdge * he3 = new HalfEdge(getHEid());
        HalfEdge * he10 = new HalfEdge(getHEid());
        HalfEdge * he11 = new HalfEdge(getHEid());
        HalfEdge * he12 = new HalfEdge(getHEid());
        HalfEdge * he13 = new HalfEdge(getHEid());
        HalfEdge * he20 = new HalfEdge(getHEid());
        HalfEdge * he21 = new HalfEdge(getHEid());
        HalfEdge * he22 = new HalfEdge(getHEid());
        HalfEdge * he23 = new HalfEdge(getHEid());
        HalfEdge * he30 = new HalfEdge(getHEid());
        HalfEdge * he31 = new HalfEdge(getHEid());
        HalfEdge * he32 = new HalfEdge(getHEid());
        HalfEdge * he33 = new HalfEdge(getHEid());

        std::vector<int> index_of_shortest_edgept_to_updatept;
        for (unsigned int j = 0; j < updatepts.size(); j++) {

            std::vector<float> distances;
            for (unsigned int k = 0; k < edgepts.size(); k++) {
                float dist = sqrt((updatepts.at(j)->getPosVec()[0] - edgepts.at(k)->getPosVec()[0]) * (updatepts.at(j)->getPosVec()[0] - edgepts.at(k)->getPosVec()[0]) +
                                  (updatepts.at(j)->getPosVec()[1] - edgepts.at(k)->getPosVec()[1]) * (updatepts.at(j)->getPosVec()[1] - edgepts.at(k)->getPosVec()[1]) +
                                  (updatepts.at(j)->getPosVec()[2] - edgepts.at(k)->getPosVec()[2]) * (updatepts.at(j)->getPosVec()[2] - edgepts.at(k)->getPosVec()[2]));
                distances.push_back(dist);
            }
            int index_short = 0;
            for (unsigned long int k = 0; k < distances.size(); k++) {
                if (distances.at(k) <= distances.at(index_short)) {
                    index_short = k;
                }
            }
            int index_short_2 = 0;
            for (unsigned long int k = 0; k < distances.size(); k++) {
                if (distances.at(k) <= distances.at(index_short_2) && k != index_short) {
                    index_short_2 = k;
                }
            }
            index_of_shortest_edgept_to_updatept.push_back(index_short);
            index_of_shortest_edgept_to_updatept.push_back(index_short_2);
        }

        // create face
        Face *f = new Face(getFid());
        f->halfedge = he0;
        f->setColorVec(temp.at(i)->color);
        // assign vertices
        he0->vert = a;
        he1->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(0));
        he2->vert = face_point;
        he3->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(1));
        // assign next half edge
        he0->next = he1;
        he1->next = he2;
        he2->next = he3;
        he3->next = he0;
        // assign face
        he0->face = f;
        he1->face = f;
        he2->face = f;
        he3->face = f;

        // create face
        Face *f1 = new Face(getFid());
        f1->halfedge = he10;
        f1->setColorVec(temp.at(i)->color);
        // assign vertices
        he10->vert = b;
        he11->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(2));
        he12->vert = face_point;
        he13->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(3));
       // assign next half edge
        he10->next = he11;
        he11->next = he12;
        he12->next = he13;
        he13->next = he10;
        // assign face
        he10->face = f1;
        he11->face = f1;
        he12->face = f1;
        he13->face = f1;

        // create face
        Face *f2 = new Face(getFid());
        f2->halfedge = he20;
        f2->setColorVec(temp.at(i)->color);
        // assign vertices
        he20->vert = c;
        he21->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(4));
        he22->vert = face_point;
        he23->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(5));
       // assign next half edge
        he20->next = he21;
        he21->next = he22;
        he22->next = he23;
        he23->next = he20;
        // assign face
        he20->face = f2;
        he21->face = f2;
        he22->face = f2;
        he23->face = f2;

        // create face
        Face *f3 = new Face(getFid());
        f3->halfedge = he30;
        f3->setColorVec(temp.at(i)->color);
        // assign vertices
        he30->vert = d;
        he31->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(6));
        he32->vert = face_point;
        he33->vert = edgepts.at(index_of_shortest_edgept_to_updatept.at(7));
       // assign next half edge
        he30->next = he33;
        he31->next = he30;
        he32->next = he31;
        he33->next = he32;
        // assign face
        he30->face = f3;
        he31->face = f3;
        he32->face = f3;
        he33->face = f3;

//        cout <<he30->vert->getPosVec()[0] << " " << he30->vert->getPosVec()[1] << " " << he30->vert->getPosVec()[2] << " " << endl;
//        cout <<he31->vert->getPosVec()[0] << " " << he31->vert->getPosVec()[1] << " " << he31->vert->getPosVec()[2] << " " << endl;
//        cout <<he32->vert->getPosVec()[0] << " " << he32->vert->getPosVec()[1] << " " << he32->vert->getPosVec()[2] << " " << endl;
//        cout <<he33->vert->getPosVec()[0] << " " << he33->vert->getPosVec()[1] << " " << he33->vert->getPosVec()[2] << " " << endl;



        faceList->push_back(f);
        faceList->push_back(f1);
        faceList->push_back(f2);
        faceList->push_back(f3);

    }



    // assign sym pointers for whole mesh
    int count = 0;          // testing purposes

    for (int i = 0; i < faceCount(); ++i) {
        HalfEdge * he0 = faceAt(i)->getHE();
        HalfEdge * curr = he0->next;
        int vertCount = 1;
        while (curr != he0) {
            vertCount++;
            curr = curr->next;
        }
        curr = he0;
        for (int j = 0; j < vertCount; j++) {
            // curr is halfedge -- set it's sym (curr2)

            for (int k = 0; k < faceCount(); ++k) {
                HalfEdge * he1 = faceAt(k)->getHE();
                HalfEdge * curr2 = he1->next;
                int vertCount2 = 1;
                while (curr2 != he1) {
                    vertCount2++;
                    curr2 = curr2->next;
                }
                curr2 = he1;
                for (int l = 0; l < vertCount2; l++) {
                    // COMPARE CURR and CURR2
                    Vertex * one = curr->vert;
                    HalfEdge *temp = curr;
                    while (temp->next != curr) {
                        temp = temp->next;
                    }
                    Vertex * two = temp->vert;

                    Vertex * three = curr2->vert;
                    HalfEdge *temp2 = curr2;
                    while (temp2->next != curr2) {
                        temp2 = temp2->next;
                    }
                    Vertex * four = temp2->vert;

                    if (one->getPosVec() == three->getPosVec() &&
                            two->getPosVec() == four->getPosVec() &&
                            curr->getID() != curr2->getID()) {
                        curr->sym = curr2;
                        curr2->sym = curr;
                        count++;
                    }
                    else if (one->getPosVec() == four->getPosVec() &&
                             two->getPosVec() == three->getPosVec() &&
                             curr->getID() != curr2->getID()) {
                        curr->sym = curr2;
                        curr2->sym = curr;
                        count++;
                    }
                    curr2 = curr2->next;
                }
            }
            curr = curr->next;
        }
    }

    cout << "sym pairs made " << count << endl;


    // update points
 /*  for (unsigned int i = 0; i < faceList->size(); ++i) {
        updatedPoints.push_back(updatedPoint(faceAt(i)->halfedge));
        //faceList->at(i)->halfedge->vert->setPosVec(updatedPoint(faceList->at(i)->halfedge));
    }

    for (unsigned int i = 0; i < faceList->size(); ++i) {
        faceAt(i)->halfedge->vert->setPosVec(updatedPoints.at(i));
    }
*/

//    f->halfedge->vert->setPosVec(glm::vec3(updatedPoints.at(i*4)[0], updatedPoints.at(i*4)[1], updatedPoints.at(i*4)[2]));
//    f1->halfedge->vert->setPosVec(glm::vec3(updatedPoints.at(i*4 + 1)[0], updatedPoints.at(i*4 + 1)[1], updatedPoints.at(i*4 + 1)[2]));
//    f2->halfedge->vert->setPosVec(glm::vec3(updatedPoints.at(i*4 + 2)[0], updatedPoints.at(i*4 + 2)[1], updatedPoints.at(i*4 + 2)[2]));
//    f3->halfedge->vert->setPosVec(glm::vec3(updatedPoints.at(i*4 + 3)[0], updatedPoints.at(i*4 + 3)[1], updatedPoints.at(i*4 + 3)[2]));

}

/*
void Mesh::subdivide() {

    facePoints.clear();
    edgePoints.clear();
    updatedPoints.clear();
    resetIds();

    for (unsigned int i = 0; i < faceList->size(); ++i) {
        facePoints.push_back(facePoint(faceAt(i)));
    }

    std::vector<Face*> *newFaceList = new std::vector<Face*>;

    for (unsigned int i = 0; i < faceList->size(); ++i) {

        // FACEPOINT
        glm::vec3 facept = facePoints.at(i);
        Vertex * facevert = new Vertex(getVid());
        facevert->setPosVec(facept);

        HalfEdge * he0 = faceAt(i)->getHE();
        HalfEdge * curr = he0->next;
        int vertCount = 1;
        while (curr != he0) {
            vertCount++;
            curr = curr->next;
        }
        curr = he0;
        for (int j = 0; j < vertCount; j++) {
            // curr is halfedge
            // EDGEPOINT
            addVertex(curr);                                            // ** PLANE CRASHES

            curr->next->vert->setPosVec(edgePoint(curr->next));
            cout << "here edgepoint loop" << endl;

            curr = curr->next->next;
        }


        // QUAD
        if (vertCount == 4) {
            cout << "here start quad" << endl;

            // connect facePoint to edgePoint
            curr = he0;
            std::vector<HalfEdge*> nexts;     // store original loop
            for (int j =0; j < vertCount - 1; j++) {
                curr = curr->next->next;
                nexts.push_back(curr);
            }

            curr = he0;

            HalfEdge * halfie0 = new HalfEdge(getHEid());
            halfie0->vert = facevert;
            curr->next->next = halfie0;

            HalfEdge * halfie02 = new HalfEdge(getHEid());
            halfie02->vert = nexts.at(2)->next->vert;
            halfie02->next = curr;
            halfie0->next = halfie02;

            curr = nexts.at(0);

            HalfEdge * halfie1 = new HalfEdge(getHEid());
            halfie1->vert = facevert;
            curr->next->next = halfie1;

            HalfEdge * halfie12 = new HalfEdge(getHEid());
            halfie12->vert = he0->next->vert;
            halfie12->next = curr;
            halfie1->next = halfie12;

            curr = nexts.at(1);

            HalfEdge * halfie2 = new HalfEdge(getHEid());
            halfie2->vert = facevert;
            curr->next->next = halfie2;

            HalfEdge * halfie22 = new HalfEdge(getHEid());
            halfie22->vert = nexts.at(0)->next->vert;
            halfie22->next = curr;
            halfie2->next = halfie22;

            curr = nexts.at(2);

            HalfEdge * halfie3 = new HalfEdge(getHEid());
            halfie3->vert = facevert;
            curr->next->next = halfie3;

            HalfEdge * halfie32 = new HalfEdge(getHEid());
            halfie32->vert = nexts.at(1)->next->vert;
            halfie32->next = curr;
            halfie3->next = halfie32;

            halfie0->sym = halfie12;
            halfie12->sym = halfie0;
            halfie1->sym = halfie22;
            halfie22->sym = halfie1;
            halfie2->sym = halfie32;
            halfie32->sym = halfie2;
            halfie0->sym = halfie12;
            halfie3->sym = halfie02;
            halfie02->sym = halfie3;

            Face *f = new Face(getFid());
            f->halfedge = he0;
            f->setColorVec(faceAt(i)->color);
            he0->face = f;
            he0->next->face = f;
            he0->next->next->face = f;
            he0->next->next->next->face = f;

            Face *f1 = new Face(getFid());
            f1->halfedge = nexts.at(0);
            f1->setColorVec(faceAt(i)->color);
            nexts.at(0)->face = f1;
            nexts.at(0)->next->face = f1;
            nexts.at(0)->next->next->face = f1;
            nexts.at(0)->next->next->next->face = f1;

            Face *f2 = new Face(getFid());
            f2->halfedge = nexts.at(1);
            f2->setColorVec(faceAt(i)->color);
            nexts.at(1)->face = f2;
            nexts.at(1)->next->face = f2;
            nexts.at(1)->next->next->face = f2;
            nexts.at(1)->next->next->next->face = f2;

            Face *f3 = new Face(getFid());
            f3->halfedge = nexts.at(2);
            f3->setColorVec(faceAt(i)->color);
            nexts.at(2)->face = f3;
            nexts.at(2)->next->face = f3;
            nexts.at(2)->next->next->face = f3;
            nexts.at(2)->next->next->next->face = f3;


//            cout << f->halfedge->vert->getPosVec()[0] << f->halfedge->vert->getPosVec()[1] <<
//                    f->halfedge->vert->getPosVec()[2] << endl;
//            cout << f->halfedge->next->vert->getPosVec()[0] << f->halfedge->next->vert->getPosVec()[1] <<
//                    f->halfedge->next->vert->getPosVec()[2] << endl;
//            cout << f->halfedge->next->next->vert->getPosVec()[0] << f->halfedge->next->next->vert->getPosVec()[1] <<
//                    f->halfedge->next->next->vert->getPosVec()[2] << endl;
//            cout << f->halfedge->next->next->next->vert->getPosVec()[0] << f->halfedge->next->next->next->vert->getPosVec()[1] <<
//                    f->halfedge->next->next->next->vert->getPosVec()[2] << endl;


            // UPDATEDPOiNTS
            he0->vert->setPosVec(updatedPoint(he0));
            nexts.at(0)->vert->setPosVec(updatedPoint(nexts.at(0)));
            nexts.at(1)->vert->setPosVec(updatedPoint(nexts.at(1)));
            nexts.at(2)->vert->setPosVec(updatedPoint(nexts.at(2)));


            newFaceList->push_back(f);
            newFaceList->push_back(f1);
            newFaceList->push_back(f2);
            newFaceList->push_back(f3);

        }
        // TRIANGLE
        else if (vertCount == 3) {

        }
        // N-GON
        else {
            cout << "ERROR: Subdivision only works for quads and triangles!" << endl;
            return;
        }


    }

    // set faces
    faceList->clear();
    faceList = newFaceList;




    // sym pointers for whole shape
    int count = 0;          // testing purposes

    for (int i = 0; i < faceCount(); ++i) {
        HalfEdge * he0 = faceAt(i)->getHE();
        HalfEdge * curr = he0->next;
        int vertCount = 1;
        while (curr != he0) {
            vertCount++;
            curr = curr->next;
        }
        curr = he0;
        for (int j = 0; j < vertCount; j++) {
            // curr is halfedge -- set it's sym (curr2)

            for (int k = 0; k < faceCount(); ++k) {
                HalfEdge * he1 = faceAt(k)->getHE();
                HalfEdge * curr2 = he1->next;
                int vertCount2 = 1;
                while (curr2 != he1) {
                    vertCount2++;
                    curr2 = curr2->next;
                }
                curr2 = he1;
                for (int l = 0; l < vertCount2; l++) {
                    // COMPARE CURR and CURR2
                    glm::vec3 one = curr->vert->getPosVec();
                    HalfEdge *temp = curr;
                    while (temp->next != curr) {
                        temp = temp->next;
                    }
                    glm::vec3 two = temp->vert->getPosVec();

                    glm::vec3 three = curr2->vert->getPosVec();
                    HalfEdge *temp2 = curr2;
                    while (temp2->next != curr2) {
                        temp2 = temp2->next;
                    }
                    glm::vec3 four = temp2->vert->getPosVec();


                    if (one == three && two == four && curr->getID() != curr2->getID()) {
                        curr->sym = curr2;
                        curr2->sym = curr;
                        count++;
                    }
                    else if (one == four && two == three && curr->getID() != curr2->getID()) {
                        curr->sym = curr2;
                        curr2->sym = curr;
                        count++;
                    }

                    curr2 = curr2->next;

                }
            }


            curr = curr->next;
        }
    }

    cout << "sym pairs made " << count << endl;

}*/







// ********* pre-made mesh *********
void Mesh::plane() {
    faceList->clear();
    resetIds();

    /// Front face
    // create vertices
    Vertex * v0 = new Vertex(getVid(), 0.5f, 0.5f, -0.5f);
    Vertex * v1 = new Vertex(getVid(), -0.5f, 0.5f, -0.5f);
    Vertex * v2 = new Vertex(getVid(),-0.5f, -0.5f, -0.5f);
    Vertex * v3 = new Vertex(getVid(), 0.5f, -0.5f, -0.5f);
    // create half edges
    HalfEdge * he0 = new HalfEdge(getHEid());
    HalfEdge * he1 = new HalfEdge(getHEid());
    HalfEdge * he2 = new HalfEdge(getHEid());
    HalfEdge * he3 = new HalfEdge(getHEid());
    v0->active = true;
    // create face
    Face * front = new Face(getFid());
    front->halfedge = he0;
    front->setColorVec(glm::vec3(0,0,1));
    // assign vertices
    he0->vert = v0;
    he1->vert = v1;
    he2->vert = v2;
    he3->vert = v3;
    // assign next half edge
    he0->next = he1;
    he1->next = he2;
    he2->next = he3;
    he3->next = he0;
    // assign face
    he0->face = front;
    he1->face = front;
    he2->face = front;
    he3->face = front;

    faceList->push_back(front);
}
