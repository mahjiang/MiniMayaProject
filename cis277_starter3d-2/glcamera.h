#ifndef GLCAMERA_H
#define GLCAMERA_H
#include <glm/glm.hpp>
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#define GLM_FORCE_RADIANS

class GLCamera
{
public:
    GLCamera();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void resizeViewport(float width, float height);
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

    // Mini-Maya Camera Modes
    enum mode {
        moveEye,
        moveREF,
        arcball
    };

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
    view currView;
    mode currMode;
    void changeViewTo(view v);
    // Mini-Maya Cameras
    glm::vec3 perspEye;
    glm::vec3 perspREF;

    glm::vec3 frontEye;
    glm::vec3 frontREF;

    glm::vec3 backEye;
    glm::vec3 backREF;

    glm::vec3 leftEye;
    glm::vec3 leftREF;

    glm::vec3 rightEye;
    glm::vec3 rightREF;

    glm::vec3 topEye;
    glm::vec3 topREF;

    glm::vec3 bottomEye;
    glm::vec3 bottomREF;


    float orthoZoom;

    // Mini-Maya Arcball
    float arcballRotationAngle(float lat, float lon);
    glm::vec3 arcballRotationAxis(float lat, float lon);
    glm::vec3 arcballVector(float x, float y);
    glm::vec4 arcballQuaternion(float pX, float pY, float cX, float xY);
    glm::mat4 arcballRotationMatrix(glm::vec4 q);
    glm::vec4 q;   // Arcball quaternion
    glm::vec4 oldq;
    glm::mat4 rotate;
    glm::mat4 translate;
    float prevX;
    float prevY;
    float currX;
    float currY;

    // Mini-Maya Pan
    void panUP();
    void panDown();
    void panRight();
    void panLeft();
    float panX;
    float panY;

    // Mini-Maya Snap Functions
    void snapToVertex(Vertex* v);
    void snapToEdge(HalfEdge* e);
    void snapToFace(Face* f);


public:
    // view matrix
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 upView;
    glm::vec3 frontView;

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


    void resetCam();
};

#endif // GLCAMERA_H
