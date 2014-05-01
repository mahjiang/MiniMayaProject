#ifndef FACE_H
#define FACE_H
#include <glm/glm.hpp>
#include "halfedge.h"
#include <string>
#include <QListWidgetItem>

class Face : public QListWidgetItem
{
public:
    Face(int ID);

    int getId();
    HalfEdge* getHE();
    glm::vec3 getColorVec();

    void setHE(HalfEdge * HE);
    void setColorVec(glm::vec3 colorVec);
    std::string getText();
    bool active;
    bool hover;

    /// helper functions
    int getFaceVertexCount();
//private:
    int id;
    HalfEdge * halfedge;
    glm::vec3 color;
    std::string text;
    //float color[3];

signals:
    void removeFaceSignal(Face * f);

};

#endif // FACE_H
