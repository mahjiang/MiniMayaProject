#include "glcamera.h"
#include "iostream"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#define GLM_FORCE_RADIANS

using namespace std;
static const float PI = 3.141592653589f;


/// Constructor

GLCamera::GLCamera()
{
  eye = glm::vec3(0,0,0);
  center = glm::vec3(0,0,0);
  upView = glm::vec3(0,0,1.0f);
  width = 1236.0f;
  height = 916.0f;
  p = 3.5f;
  theta = 0.0f;
  phi = PI/2;
  FOV = PI/4;
  currView = PERSP;
  orthoZoom = 400.0f;
}

float z(float x) {
    if (glm::abs(x) < .000001) return 0;
    else return x;
}

/// View/Projection Matrix Generation

glm::mat4 GLCamera::getViewMatrix() {
    eye = sphericalToCartesian(p, theta, phi);
    if (phi >= -PI && phi <= 0) upView = glm::vec3(0,0,-1.0f);
    else upView = glm::vec3(0,0,1.0f);
    // cout << "x: " << z(eye[0]) << "; y: " << z(eye[1]) << "; z: " << z(eye[2]) << "; " << endl;
    glm::mat4 view  = glm::lookAt(eye, center, upView);
    return view;
}

glm::mat4 GLCamera::getProjectionMatrix() {
    glm::mat4 projection;
    if (currView == PERSP || currView == USERDEF)
        projection = glm::perspective(1.f, width / height, 0.1f, 100.f);
    else projection = glm::ortho(-(width/orthoZoom), (width/orthoZoom), -(height/orthoZoom), (height/orthoZoom), -10.0f, 10.0f);
    //projection = glm::perspective(1.f, width / height, 0.1f, 100.f);
    return projection;
}

/// get/set width of camera viewport
void GLCamera::setWidth(float w) { width = w; }
void GLCamera::setHeight(float h) { height = h; }
float GLCamera::getWidth() { return width; }
float GLCamera::getHeight() { return height; }


/// Camera Positioning
glm::vec3 GLCamera::sphericalToCartesian(float pDist, float tAngle, float pAngle) {
    float x = pDist * glm::sin(pAngle) * glm::cos(tAngle);
    float y = pDist * glm::sin(pAngle) * glm::sin(tAngle);
    float z = pDist * glm::cos(pAngle);
    return glm::vec3(x,y,z);
}

void GLCamera::north() {
    if (currView != PERSP) return;
    phi -= .05f;
    if (phi <= -PI) phi = PI;
}
void GLCamera::south() {
    if (currView != PERSP) return;
    phi += .05f;
    if (phi >= PI) phi = -PI;
}
void GLCamera::west() {
    if (currView != PERSP) return;
    theta += .05f;
   if (theta > 2 * PI) theta = 0;
}
void GLCamera::east() {
    if (currView != PERSP) return;
    theta -=.05f;
    if (theta < 0) theta = 2 * PI;
}
void GLCamera::zoomIn() {
    if (currView == PERSP) p -= .5f;
    else orthoZoom -=25.0f;
}
void GLCamera::zoomOut() {
    if (currView == PERSP) p += .5f;
    else orthoZoom +=25.0f;

}


/// screen to world coordinates
glm::vec3 GLCamera::screenToWorld(float sx, float sy) {
    float sWidth = 449.0f;
    float sHeight = 300.0f;
    float aspect = sWidth/sHeight;
    float hFOV = atanf(tanf(FOV/2.0f) * aspect);
    glm::vec3 C = center - eye;
    glm::vec3 A = glm::cross(C, upView);
    glm::vec3 B = glm::cross(A, C);
    glm::vec3 M = eye + C;
    float distC = glm::length(C);
    glm::vec3 H = (A * distC * tanf(hFOV)) / glm::length(A);
    glm::vec3 V = (B * distC * tanf(FOV/2)) / glm::length(B);
    glm::vec3 pWorld = M + (2 * sx - 1) * H + (2 * sy - 1) * V;

    return pWorld;
}

/// Mini-Maya Views
void GLCamera::changeViewTo(view v) {
    cout << "in change view function?" << endl;
    if (v == FRONT) {
        phi = PI/2.0f;
        theta = 0;
    } else if (v == RIGHT) {
        phi = PI/2.0f;
        theta = PI/2.0f;
    } else if (v == BACK) {
        phi = PI/2.0f;
        theta = PI;
    } else if (v == LEFT) {
        phi = PI/2.0f;
        theta = PI/2.0f;
    } else if (v == TOP) {
        phi = 0.0001f;
        theta = 0;
    } else if (v == BOTTOM) {
        phi = -PI + .00001f;
        theta = 0;
    }
    p = 3.5f;
    currView = v;
}
