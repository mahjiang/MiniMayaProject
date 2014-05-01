#include "glcamera.h"
#include "iostream"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/transform.hpp>
#include <cmath>
#define GLM_FORCE_RADIANS

using namespace std;
static const float PI = 3.141592653589f;


/// Constructor

GLCamera::GLCamera()
{
  eye = glm::vec3(0,0,0);
  center = glm::vec3(0,0,0);
  upView = glm::vec3(0,0,1.0f);
  width = 900.0f;
  height = 600.0f;
  FOV = PI/4;
  currX = 0;
  currY = 0;
  prevX = 0;
  prevY = 0;
  oldq = glm::vec4(0,0,1,0);
  rotate = glm::rotate(0.0f, glm::vec3(0,1,0));
  translate = glm::translate(glm::vec3(0.0f,0.0f,0.0f));
  resetCam();
}


float z(float x) {
    if (glm::abs(x) < .000001) return 0;
    else return x;
}

//void GLCamera::resizeViewport(float w, float h) {
//    width = w;
//    height = h;
//}

/// View/Projection Matrix Generation

glm::mat4 GLCamera::getViewMatrix() {
    glm::vec3 REF;// = center;
    eye = sphericalToCartesian(p, theta, phi);

    // update upview
    if (phi >= -PI && phi <= 0) upView = glm::vec3(0,0,-1.0f);
    else upView = glm::vec3(0,0,1.0f);

    if (currView == PERSP) {
        eye = sphericalToCartesian(p, theta, phi);
        REF = perspREF;
    }
    // update eye and reference point
    if (currView == FRONT) {
        eye = frontEye;
        REF = frontREF;
    }
    else if (currView == BACK) {
        eye = backEye;
        REF = backREF;
    }
    else if (currView == RIGHT) {
        eye = rightEye;
        REF = rightREF;
    }
    else if (currView == LEFT) {
        //cout << "fuck me" << endl;
        eye = leftEye;
        REF = leftREF;
    }
    else if (currView == TOP) {
        eye = topEye;
        REF = topREF;
    }
    else if (currView == BOTTOM) {
        eye = bottomEye;
        REF = bottomREF;
    }


    // return view matrix
    glm::mat4 view  = glm::lookAt(eye, REF, upView);
    //view = view * glm::rotate(2.0f * oldq[0], glm::vec3(oldq[1],oldq[2],oldq[3]));
    //cout << "view" << glm::to_string(view) << endl;
   if (currMode == arcball && currView == PERSP) {
        if (prevX != currX || prevY != currY) {
            glm::vec4 qC = arcballQuaternion(prevX,prevY,currX,currY);
            glm::mat3 inv = glm::inverse(glm::mat3(view * rotate));
            glm::vec3 objAxis = inv * glm::vec3(qC[1],qC[2],qC[3]);
            q = glm::vec4(qC[0],objAxis[0],objAxis[1],objAxis[2]);
            rotate *= glm::rotate(2.0f * qC[0], objAxis);
            prevX = currX;
            prevY = currY;
        }
   }

    translate = glm::translate(glm::vec3(panY,panX,0));
    if (currView == PERSP) return translate * view * rotate;
    else return view;
}

glm::mat4 GLCamera::getProjectionMatrix() {
    glm::mat4 projection;
    if (currView == PERSP || currView == USERDEF)
        projection = glm::perspective(FOV, width / height, 0.1f, 100.f);
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
    if (currView == FRONT) {
        frontEye[2] -= .1;
        frontREF[2] -= .1;
    } else if (currView == BACK) {
        backEye[2] -= .1;
        backREF[2] -= .1;
    } else if (currView == RIGHT) {
        rightEye[2] -= .1;
        rightREF[2] -= .1;
    } else if (currView == LEFT) {
        leftEye[2] -= .1;
        leftREF[2] -= .1;
    } else if (currView == TOP) {
        topEye[0] += .1;
        topREF[0] += .1;
    } else if (currView == BOTTOM) {
        bottomEye[0] += .1;
        bottomREF[0] += .1;
    } else if (currView == PERSP) {
        if (currMode == moveEye)
            phi -= .05f;
        else if(currMode == moveREF) {
            glm::vec3 dir = perspREF - perspEye;
            perspREF = dir;
        }
    }
    if (phi <= -PI) phi = PI;
}
void GLCamera::south() {
    if (currView == FRONT) {
        frontEye[2] += .1;
        frontREF[2] += .1;
    } else if (currView == BACK) {
        backEye[2] += .1;
        backREF[2] += .1;
    } else if (currView == RIGHT) {
        rightEye[2] += .1;
        rightREF[2] += .1;
    } else if (currView == LEFT) {
        leftEye[2] += .1;
        leftREF[2] += .1;
    } else if (currView == TOP) {
        topEye[0] -= .1;
        topREF[0] -= .1;
    } else if (currView == BOTTOM) {
        bottomEye[0] -= .1;
        bottomREF[0] -= .1;
    } else if (currView == PERSP) {
        if (currMode == moveEye)
            phi += .05f;
        else if (currMode == moveREF)
            perspREF += .1f;
    }
    if (phi >= PI) phi = -PI;
}
void GLCamera::west() {
    if (currView == FRONT) {
        frontEye[1] -= .1;
        frontREF[1] -= .1;
    } else if (currView == BACK) {
        backEye[1] += .1;
        backREF[1] += .1;
    } else if (currView == RIGHT) {
        rightEye[0] += .1;
        rightREF[0] += .1;
    } else if (currView == LEFT) {
        leftEye[0] -= .1;
        leftREF[0] -= .1;
    } else if (currView == TOP) {
        topEye[1] -= .1;
        topREF[1] -= .1;
    } else if (currView == BOTTOM) {
        bottomEye[1] += .1;
        bottomREF[1] += .1;
    } else if (currView == PERSP) {
        theta += .05f;
    }
   if (theta > 2 * PI) theta = 0;
}
void GLCamera::east() {

    if (currView == FRONT) {
        frontEye[1] += .1;
        frontREF[1] += .1;
    } else if (currView == BACK) {
        backEye[1] -= .1;
        backREF[1] -= .1;
    }  else if (currView == RIGHT) {
        rightEye[0] -= .1;
        rightREF[0] -= .1;
    } else if (currView == LEFT) {
        leftEye[0] += .1;
        leftREF[0] += .1;
    } else if (currView == TOP) {
        topEye[1] += .1;
        topREF[1] += .1;
    } else if (currView == BOTTOM) {
        bottomEye[1] -= .1;
        bottomREF[1] -= .1;
    } else if (currView == PERSP) {
        theta -=.05f;
    }
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
    float sWidth = 900.0f;
    float sHeight = 600.0f;
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
    p = 3.5f;
    currView = v;
}

/// Mini-Maya Arcball
glm::vec3 GLCamera::arcballVector(float x, float y) {
    glm::vec3 v = glm::vec3(1.0 * x / 900 *2.0f - 1.0,
                            1.0 * y / 600 *2.0f - 1.0,
                            0);
    v[1] = -v[1];
    float dist = v[0] * v[0] + v[1] * v[1];
    if (dist <= 1) v[2] = glm::sqrt(1.0 - dist);
    else v = glm::normalize(v);
    return v;
}

glm::vec4 GLCamera::arcballQuaternion(float pX, float pY, float cX, float cY) {
    glm::vec4 q;
    if (pX != cX || pY != cY) {
       glm::vec3 v1 = arcballVector(pX,pY);
       glm::vec3 v2 = arcballVector(cX, cY);
       float q0 = glm::acos(glm::min(1.0f,glm::dot(v2, v1)));
       glm::vec3 qV = glm::cross(v1,v2);
       glm::vec4 q = glm::vec4(q0,qV[0],qV[1],qV[2]);
       return q;
    }
}

glm::mat4 GLCamera::arcballRotationMatrix(glm::vec4 q) {
    glm::mat4 m;
    m[0][0] = 1 - 2 * q[2] * q[2] - 2 * q[3] * q[3];
    m[0][1] = 2 * q[1] * q[2] + 2 * q[0] * q[3];
    m[0][2] = 2 * q[1] * q[3] - 2 * q[0] * q[2];
    m[0][3] = 0;

    m[1][0] = 2 * q[1] * q[2] - 2 * q[0] * q[3];
    m[1][1] = 1 - 2 * q[1] * q[1] - 2 * q[3] * q[3];
    m[1][2] = 2 * q[2] * q[3] + 2 * q[0] * q[1];
    m[1][3] = 0;

    m[2][0] = 2 * q[1] * q[3] + 2 * q[0] * q[2];
    m[2][1] = 2 * q[2] * q[3] - 2 * q[0] * q[1];
    m[2][2] = 1 - 2 * q[1] * q[1] - 2 * q[2] * q[2];
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

    return m;
}

/// Mini-Maya pan functions
void GLCamera::panUP() {
    panX -= .1;
}

void GLCamera::panDown() {
    panX += .1;
}

void GLCamera::panRight() {
    panY -= .1;
}

void GLCamera::panLeft() {
    panY += .1;
}



/// Mini Maya Snap Functions
void GLCamera::snapToVertex(Vertex *v) {
    if (currView == PERSP) {
        perspREF = glm::vec3(v->getX(), v->getY(), v->getZ());
        //cout << glm::to_string(perspREF) << endl;

        panX = 0;
        panY = 0;
    }
}

void GLCamera::snapToEdge(HalfEdge *e) {
    if (currView == PERSP) {
        float x = (e->vert->getX() + e->sym->vert->getX()) / 2.0f;
        float y = (e->vert->getY() + e->sym->vert->getY()) / 2.0f;
        float z = (e->vert->getZ() + e->sym->vert->getZ()) / 2.0f;

        perspREF = glm::vec3(x,y,z);
        //cout << glm::to_string(perspREF) << endl;
        panX = 0;
        panY = 0;
    }
}

void GLCamera::snapToFace(Face *f) {
    HalfEdge* he0 = f->halfedge;
    HalfEdge* curr = he0;
    float x = 0;
    float y = 0;
    float z = 0;
    float count = 0;
    do {
        x += curr->vert->getX();
        y += curr->vert->getY();
        z += curr->vert->getZ();
        count = count + 1.0f;

        curr = curr->next;
    } while (curr != he0);

    x = x / count;
    y = y / count;
    z = z / count;

    panX = 0;
    panY = 0;
    perspREF = glm::vec3(x,y,z);
}


void GLCamera::resetCam() {
    currView = PERSP;
    currMode = moveEye;
    orthoZoom = 400.0f;
    p = 3.5f;
    theta = PI/4;
    phi = PI/4;
    oldq = glm::vec4(0,0,1,0);
    q = glm::vec4(0,0,1,0);
    rotate = glm::rotate(0.0f,glm::vec3(0,1,0));
    panX = 0;
    panY = 0;

    /// Reset all cameras
    // reset perspective cam
    perspEye = sphericalToCartesian(p,theta,phi);
    perspREF = glm::vec3(0.0f,0.0f,0.0f);
    // reset front cam
    frontEye = sphericalToCartesian(p,0,PI/2.0f);
    frontREF = glm::vec3(0.0f,0.0f,0.0f);
    // reset back cam
    backEye = sphericalToCartesian(p,PI,PI/2.0f);
    backREF = glm::vec3(0.0f,0.0f,0.0f);
    // reset right cam
    rightEye = sphericalToCartesian(p,PI/2.0f,PI/2.0f);
    rightREF = glm::vec3(0.0f,0.0f,0.0f);
    // reset left cam
    leftEye = sphericalToCartesian(p,(3*PI)/2.0f,PI/2.0f);
    leftREF = glm::vec3(0.0f,0.0f,0.0f);
    // reset top cam
    topEye = sphericalToCartesian(p,0.0f,0.0001f);
    topREF = glm::vec3(0.0f,0.0f,0.0f);
    // reset bottom cam
    bottomEye = sphericalToCartesian(p,0.0f,-PI + .0001f);
    bottomREF = glm::vec3(0.0f,0.0f,0);
}
