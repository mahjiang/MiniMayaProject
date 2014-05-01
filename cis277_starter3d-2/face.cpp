#include "face.h"
#include <sstream>



Face::Face(int ID) : QListWidgetItem(0) {
    id = ID;
    color = glm::vec3(0.0f,0.0f,1.0f);
    active = false;
    hover = false;

    std::ostringstream s;
    s << "Face: " << id + 1;
    text = s.str();
    this->setText(text.c_str());
}

int Face::getId() { return id; }
glm::vec3 Face::getColorVec() { return color; }
HalfEdge* Face::getHE() { return halfedge; }

void Face::setHE(HalfEdge * HE) {
    halfedge = HE;
}

void Face::setColorVec(glm::vec3 colorVec) {
    color = colorVec;
}

std::string Face::getText() {
    return text;
}

/// helper functions
int Face::getFaceVertexCount() {
    HalfEdge * he0 = halfedge;
    HalfEdge * curr = he0->next;
    int faceVertexCount = 1;
    while (curr != he0) {
        faceVertexCount++;
        curr = curr->next;
    }
    return faceVertexCount;
}
