#include "vertex.h"
#include <sstream>

using namespace std;

/// constructors
Vertex::Vertex(int ID)  : QListWidgetItem(0)  {
    id = ID;
    active = false;

    std::ostringstream s;
    s << "Vertex: " << id + 1;
    text = s.str();
    this->setText(text.c_str());
}

Vertex::Vertex(int ID, float x, float y, float z) {
    id = ID;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    active = false;
    std::ostringstream s;
    s << "Vertex: " << id + 1;
    text = s.str();
    this->setText(text.c_str());
}

/// getters/setters
int Vertex::getID() { return id; }
float Vertex::getX() { return pos[0]; }
float Vertex::getY() { return pos[1]; }
float Vertex::getZ() { return pos[2]; }
glm::vec3 Vertex::getPosVec() { return glm::vec3(pos[0],pos[1],pos[2]); }
void Vertex::setPosVec(glm::vec3 posVec) {
    pos[0] = posVec[0];
    pos[1] = posVec[1];
    pos[2] = posVec[2];
}

std::string Vertex::getText() {
    return text;
}

/// helper functions
void Vertex::print() {
    cout << "( " << pos[0] << ", " << pos[1] << ", " << pos[2] << ", 1.0 )" << endl;
}
