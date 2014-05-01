#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <QListWidgetItem>


class Vertex : public QListWidgetItem
{
public:
    Vertex(int ID);
    Vertex(int ID, float x, float y, float z);
    glm::vec3 getPosVec();
    void print();
    int getID();
    std::string getText();
    float getX();
    float getY();
    float getZ();
    void setPosVec(glm::vec3 posVec);
    float pos[3];
    int id;
    bool active;
    float s, t, u;
    std::string text;
};

#endif // VERTEX_H
