#include "halfedge.h"
#include <sstream>

HalfEdge::HalfEdge(int ID)  : QListWidgetItem(0) {
 id = ID;
 sym = NULL;
 face = NULL;
 vert = NULL;
 next = NULL;

 std::ostringstream s;
 s << "HalfEdge: " << id + 1;
 text = s.str();
 this->setText(text.c_str());
}

int HalfEdge::getID() const {
    return id;
}


bool HalfEdge::operator< ( const HalfEdge &other ) const {
    if (id < other.getID()) return true;
    else return false;
}



