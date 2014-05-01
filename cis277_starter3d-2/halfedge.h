#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "vertex.h"

class Face;
class HalfEdge : public QListWidgetItem
{
public:
    HalfEdge(int ID);
    Face * face;
    Vertex * vert;
    HalfEdge * next;
    HalfEdge * sym;

    int getID() const;
    std::string getText();
    virtual bool operator< ( const HalfEdge &other ) const;

private:
    int id;
    std::string text;
};

#endif // HALFEDGE_H
