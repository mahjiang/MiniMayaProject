#ifndef GLCAMERA_H
#define GLCAMERA_H
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS

class GLCamera
{
public:
    GLCamera();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void setWidth(float w);
    void setHeight(float h);
    float getWidth();
    float getHeight();

    // move camera
    void north();
    void south();
    void east();
    void west();
    void zoomIn();
    void zoomOut();

    // Mini-Maya Views
    enum view {
        PERSP,
        TOP,
        LEFT,
        RIGHT,
        FRONT,
        BACK,
        BOTTOM,
        USERDEF
    };

    float orthoZoom;
    view currView;
    void changeViewTo(view v);


public:
    // view matrix
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 upView;

    // projection matrix
    float width;
    float height;

    // positioning
    float p;
    float theta;
    float phi;
    float FOV;
    glm::vec3 sphericalToCartesian(float rDist, float tAngle, float pAngle);

    // screen to world coordinates
    glm::vec3 screenToWorld(float sx, float sy);


};

#endif // GLCAMERA_H
