#include "myglwidget.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <cstdlib>
#include <vector>

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMatrix4x4>
#include <QKeyEvent>


// This includes glm::translate, glm::rotate, and glm::scale.
// You should NOT use glm::perspective and glm::lookAt, which
// are also included!
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
static const float PI = 3.141592653589f;

void printFloatVec(std::vector<float>* arr, int vertexCount) {
    for (int i = 0; i < vertexCount * 4; i++) {
        if (i == 0) cout << endl << "[";
        else if (i % 4 == 0) cout << "] "<< endl <<"[";
        if (!((i+1) % 4 == 0)) cout << arr->at(i) << ", ";
        else cout << arr->at(i);
       // if ((i) % 4) cout << ", ";
        if (vertexCount * 4 - 1 == i) cout << "]" << endl;
    }
}

void printUIntVec(std::vector<unsigned int>* arr, int triangleCount) {
    for (int i = 0; i < triangleCount * 3; i++) {
        if (i == 0) cout << endl << "[";
        else if (i % 3 == 0) cout << "] "<< endl <<"[";
        if (!((i+1) % 3 == 0)) cout << arr->at(i) << ", ";
        else cout << arr->at(i);
       // if ((i) % 4) cout << ", ";
        if (triangleCount * 3 - 1 == i) cout << "]" << endl;
    }
}

MyGLWidget::MyGLWidget(QWidget* parent)
    : QGLWidget(parent)
{
    angle = 0;
    cam = new GLCamera;
    spin = false;
    mesh = new Mesh();
    // Tell the widget to draw 60 times per second
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(16);
    setMouseTracking(true);
    showGrid = false;
}


MyGLWidget::~MyGLWidget()
{
    // Delete the buffers that were allocated on the GPU
    // (This is like deleting pointers on the CPU)
    glDeleteBuffers(1, &vboDiffPositions);
    glDeleteBuffers(1, &vboDiffNormals);
    glDeleteBuffers(1, &vboDiffColors);
    glDeleteBuffers(1, &vboDiffIndices);
    glDeleteBuffers(1, &vboWirePositions);
    glDeleteBuffers(1, &vboWireColors);
    glDeleteBuffers(1, &vboWireIndices);
    glDeleteBuffers(1, &vboPointPositions);
    glDeleteBuffers(1, &vboPointColors);
    glDeleteBuffers(1, &vboPointIndices);
    glDeleteBuffers(1, &vboLatticePositions);
    glDeleteBuffers(1, &vboLatticeColors);
    glDeleteBuffers(1, &vboLatticeIndices);

    // Also delete the shader program
    glDeleteProgram(progDiff);
    glDeleteProgram(progWire);

    // And the VAO, if we created one
    if (vao) {
        vao->destroy();
    }
}

void MyGLWidget::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0) {
        cam->zoomIn();
    } else {
        cam->zoomOut();
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
    // http://qt-project.org/doc/qt-4.8/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        qApp->quit();
    } else if (altPressed && e->key() == Qt::Key_Up) {
        cam->north();
    } else if (altPressed && e->key() == Qt::Key_Down) {
        cam->south();
    } else if (altPressed && e->key() == Qt::Key_Left) {
        cam->east();
    } else if (altPressed && e->key() == Qt::Key_Right) {
        cam->west();
    }

    else if (e->key() == Qt::Key_Up) {
        cam->panUP();
    }  else if (e->key() == Qt::Key_Right) {
        cam->panRight();
    } else if (e->key() == Qt::Key_Down) {
        cam->panDown();
    } else if (e->key() == Qt::Key_Left) {
        cam->panLeft();
    }

    else if (e->key() == Qt::Key_I) {
        cam->zoomIn();
    } else if (e->key() == Qt::Key_O) {
        cam->zoomOut();
    } else if (e->key() == Qt::Key_S) {
        //    spin = !spin;
    } else if (e->key() == Qt::Key_Alt) {
        altPressed = true;
    }


    // Mini-Maya Camera Views
    else if (e->key() == Qt::Key_1) {
       cam->changeViewTo(cam->FRONT);\
    } else if (e->key() == Qt::Key_2) {
        cam->changeViewTo(cam->LEFT);
    } else if (e->key() == Qt::Key_3) {
        cam->changeViewTo(cam->BACK);
    } else if (e->key() == Qt::Key_4) {
        cam->changeViewTo(cam->RIGHT);
    } else if (e->key() == Qt::Key_5) {
        cam->changeViewTo(cam->TOP);
    } else if (e->key() == Qt::Key_6) {
        cam->changeViewTo(cam->BOTTOM);
    } else if (e->key() == Qt::Key_0) {
        cam->changeViewTo(cam->PERSP);
    } else if (e->key() == Qt::Key_G) {
        showGrid = !showGrid;
    } else if (e->key() == Qt::Key_R) {
        cam->resetCam();
        showGrid = false;
    }

    // Mini-Maya Camera Modes

    else if (e->key() == Qt::Key_M) {
        if (cam->currMode == cam->moveEye)
            cam->currMode = cam->moveREF;
        else if (cam->currMode == cam->moveREF)
            cam->currMode = cam->moveEye;
    }
}

void MyGLWidget::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Alt) {
        altPressed = false;
        cout << "KEY RELEASED" << endl;
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *e) {
    cam->prevX = e->x();
    cam->prevY = e->y();
    cam->currX = e->x();
    cam->currY = e->y();
    cam->currMode = cam->arcball;
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *e) {
    cam->currMode = cam->moveEye;
    cam->oldq = cam->q;
   // cam->oldq = cam->q;

    float xNDC =     (1.0*e->x() / this->geometry().width());
    float yNDC = 1 - (1.0*e->y() / this->geometry().height());

    glm::vec3 pWorld = cam->screenToWorld(xNDC, yNDC);
    rayP0 = glm::vec3(cam->eye[0],cam->eye[1],cam->eye[2]);

    if (cam->currView == cam->PERSP) {
        glm::vec4 rotP = glm::vec4(pWorld[0] + cam->perspREF[0], pWorld[1] + cam->perspREF[1], pWorld[2] + cam->perspREF[2],0.0f) * cam->rotate;
        pWorld = glm::vec3(rotP[0],rotP[1],rotP[2]);
        rotP = glm::vec4(rayP0[0] + cam->perspREF[0], rayP0[1] + cam->perspREF[1], rayP0[2] + cam->perspREF[2],0.0f) * cam->rotate;
        rayP0 = glm::vec3(rotP[0],rotP[1],rotP[2]);
    }

    glm::vec3 dir = pWorld - rayP0;
    uploadRay(rayP0, rayP0 + 1000.0f*glm::normalize(dir));

    Face* selectedFace = mesh->rayPolygonInteresction(pWorld, rayP0);
    mesh->activateFace(selectedFace);

    redrawMesh();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
    float xNDC =     (1.0*e->x() / this->geometry().width());
    float yNDC = 1 - (1.0*e->y() / this->geometry().height());

    glm::vec3 pWorld = cam->screenToWorld(xNDC, yNDC);
    rayP0 = glm::vec3(cam->eye[0],cam->eye[1],cam->eye[2]);

    if (cam->currView == cam->PERSP) {
        glm::vec4 rotP =  glm::translate(glm::vec3(cam->panY,cam->panX,0.0f)) * glm::vec4(pWorld[0], pWorld[1] , pWorld[2],0.0f) * cam->rotate;
//        cout << glm::to_string (cam->translate);
        pWorld = glm::vec3(rotP[0]  + cam->perspREF[0],rotP[1] + cam->perspREF[1],rotP[2]  + cam->perspREF[2]);
        rotP = glm::translate(glm::vec3(cam->panY,cam->panX,0.0f)) * glm::vec4(rayP0[0], rayP0[1], rayP0[2],0.0f) * cam->rotate;
        rayP0 = glm::vec3(rotP[0],rotP[1],rotP[2]);
    }

    Face* selectedFace = mesh->rayPolygonInteresction(pWorld, rayP0);
    mesh->hoverFace(selectedFace);

    if (cam->currMode == cam->arcball) {
        cam->currX = e->x();
        cam->currY = e->y();
    }

   // mesh->activateClosestEdge(pWorld);
}

void MyGLWidget::initializeGL()
{
    // Create an OpenGL context
    initializeGLFunctions();

    // Print out some information about the current OpenGL context
    debugContextVersion();

    // The OpenGL 3.1+ Core profiles require a VAO to be bound at all times,
    // so we create one, bind it, and forget about it.
    if (context()->format().profile() != QGLFormat::CompatibilityProfile) {
        vao = new QOpenGLVertexArrayObject(this);
        vao->create();
        vao->bind();
        // This is the equivalent low-level OpenGL code:
        //glGenVertexArrays(1, &vao);
        //glBindVertexArray(vao);
    }

    // Create the buffer objects which hold vertex data on the GPU
    glGenBuffers(1, &vboDiffPositions);
    glGenBuffers(1, &vboDiffNormals);
    glGenBuffers(1, &vboDiffColors);
    glGenBuffers(1, &vboDiffIndices);
    glGenBuffers(1, &vboWirePositions);
    glGenBuffers(1, &vboWireColors);
    glGenBuffers(1, &vboWireIndices);
    glGenBuffers(1, &vboPointPositions);
    glGenBuffers(1, &vboPointColors);
    glGenBuffers(1, &vboPointIndices);
    glGenBuffers(1, &vboLatticePositions);
    glGenBuffers(1, &vboLatticeColors);
    glGenBuffers(1, &vboLatticeIndices);

    glGenBuffers(1, &vboRayPositions);
    glGenBuffers(1, &vboRayColors);
    glGenBuffers(1, &vboRayIndices);


    // Set a few settings/modes
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glPointSize(5);
    // Set the color which clears the screen between frames
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Set up the shaders
    shaderSetupDiff();
    shaderSetupWire();

    // Upload the sample data
  //    sampleUploadCube();
    //    sampleUploadWire();
     createMesh();
     //mesh->triangulate(mesh->faceAt(3));
     rayP0 = glm::vec3(0,0,0);
     rayP1 = glm::vec3(0,0,0);
     uploadRay(rayP0, rayP1);
     if (latticePoints.size()>0) uploadLattice();
     redrawMesh();
//     uploadMesh();
//     if (mesh->activeVertexCount >= 1) sampleUploadPoint();


    // INSTRUCTOR TODO: somehow wait here for a valid context on OS X
    
  //  printGLErrorLog();
}

void MyGLWidget::debugContextVersion()
{
    const char* profile;
    switch(context()->format().profile()) {
    case QGLFormat::CoreProfile:
        profile = "Core";
        break;
    case QGLFormat::CompatibilityProfile:
        profile = "Compatibility";
        break;
    default:
        profile = "None";
    }
    qDebug() << "Widget version:" <<
        format().majorVersion() << "." <<
        format().minorVersion();
    qDebug() << "Context valid:" << context()->isValid();
    qDebug() << "Format version:" <<
        context()->format().majorVersion() << "." <<
        context()->format().minorVersion();
    qDebug() << "Profile:" << profile;
    qDebug() << "  VENDOR:  " << (const char*) glGetString(GL_VENDOR);
    qDebug() << "  RENDERER:" << (const char*) glGetString(GL_RENDERER);
    qDebug() << "  VERSION: " << (const char*) glGetString(GL_VERSION);
    qDebug() << "  GLSL:    " << (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void MyGLWidget::shaderSetupDiff()
{
    // Read in the shader program source files
    std::string vertSourceS = MyGLWidget::readFile(":/shaders/diff.vert.glsl");
    const char *vertSource = vertSourceS.c_str();
    std::string fragSourceS = MyGLWidget::readFile(":/shaders/diff.frag.glsl");
    const char *fragSource = fragSourceS.c_str();

    // Tell the GPU to create new shaders and a shader program
    GLuint shadDiffVert = glCreateShader(GL_VERTEX_SHADER);
    GLuint shadDiffFrag = glCreateShader(GL_FRAGMENT_SHADER);
    progDiff = glCreateProgram();

    // Load and compiler each shader program
    // Then check to make sure the shaders complied correctly
    // - Vertex shader
    glShaderSource    (shadDiffVert, 1, &vertSource, NULL);
    glCompileShader   (shadDiffVert);
    printShaderInfoLog(shadDiffVert);
    // - Diffuse fragment shader
    glShaderSource    (shadDiffFrag, 1, &fragSource, NULL);
    glCompileShader   (shadDiffFrag);
    printShaderInfoLog(shadDiffFrag);

    // Link the shader programs together from compiled bits
    glAttachShader  (progDiff, shadDiffVert);
    glAttachShader  (progDiff, shadDiffFrag);
    glLinkProgram   (progDiff);
    printLinkInfoLog(progDiff);

    // Clean up the shaders now that they are linked
    glDetachShader(progDiff, shadDiffVert);
    glDetachShader(progDiff, shadDiffFrag);
    glDeleteShader(shadDiffVert);
    glDeleteShader(shadDiffFrag);

    // Find out what the GLSL locations are, since we can't pre-define these
    attrDiffPositions         = glGetAttribLocation (progDiff, "vs_Position");
    attrDiffNormals           = glGetAttribLocation (progDiff, "vs_Normal");
    attrDiffColors            = glGetAttribLocation (progDiff, "vs_Color");
    unifDiffViewProj          = glGetUniformLocation(progDiff, "u_ViewProj");
    unifDiffModel             = glGetUniformLocation(progDiff, "u_Model");
    unifDiffModelInvTranspose = glGetUniformLocation(progDiff, "u_ModelInvTranspose");

    //printGLErrorLog();
}

void MyGLWidget::shaderSetupWire()
{
    // Read in the shader program source files
    std::string vertSourceS = MyGLWidget::readFile(":/shaders/wire.vert.glsl");
    const char *vertSource = vertSourceS.c_str();
    std::string fragSourceS = MyGLWidget::readFile(":/shaders/wire.frag.glsl");
    const char *fragSource = fragSourceS.c_str();

    // Tell the GPU to create new shaders and a shader program
    GLuint shadWireVert = glCreateShader(GL_VERTEX_SHADER);
    GLuint shadWireFrag = glCreateShader(GL_FRAGMENT_SHADER);
    progWire = glCreateProgram();

    // Load and compiler each shader program
    // Then check to make sure the shaders complied correctly
    // - Vertex shader
    glShaderSource    (shadWireVert, 1, &vertSource, NULL);
    glCompileShader   (shadWireVert);
    printShaderInfoLog(shadWireVert);
    // - Wire fragment shader
    glShaderSource    (shadWireFrag, 1, &fragSource, NULL);
    glCompileShader   (shadWireFrag);
    printShaderInfoLog(shadWireFrag);

    // Link the shader programs together from compiled bits
    glAttachShader  (progWire, shadWireVert);
    glAttachShader  (progWire, shadWireFrag);
    glLinkProgram   (progWire);
    printLinkInfoLog(progWire);

    // Clean up the shaders now that they are linked
    glDetachShader(progWire, shadWireVert);
    glDetachShader(progWire, shadWireFrag);
    glDeleteShader(shadWireVert);
    glDeleteShader(shadWireFrag);

    // Find out what the GLSL locations are, since we can't pre-define these
    attrWirePositions         = glGetAttribLocation (progWire, "vs_Position");
    attrWireColors            = glGetAttribLocation (progWire, "vs_Color");
    unifWireViewProj          = glGetUniformLocation(progWire, "u_ViewProj");
    unifWireModel             = glGetUniformLocation(progWire, "u_Model");

    //printGLErrorLog();
}

void MyGLWidget::resizeGL(int w, int h)
{
    cam->setWidth(w);
    cam->setHeight(h);
    float width = w;
    float height = h;
    // Set the size of the viewport area
    glViewport(0, 0, width, height);

    // DEFINITIONS:
    // The "viewproj" matrix here is projection * view.

    // The "view" matrix moves our "view" of the scene relative to the camera
    // (moving the entire scene so that the camera is at the origin).
    // In the slides for the perspective case (p254), "view" is L * RU * RN * T.

    // The "projection" matrix creates the camera perspective frustum or other
    // projection (such as orthographic).
    // In the slides for the perspective case (p254), "projection" is S * H.


    // Set up viewproj matrix (precomputed; you'll need to calculated your own!)
//    glm::mat4 proj = glm::perspective(1.f, width / height, 0.1f, 100.f);
//    glm::mat4 view = glm::lookAt(glm::vec3(-3, 4, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 proj = cam->getProjectionMatrix();
    glm::mat4 view = cam->getViewMatrix();
    glm::mat4 viewproj = proj * view;

    // Upload the projection matrix
    glUseProgram(progDiff);
    glUniformMatrix4fv(unifDiffViewProj, 1, GL_FALSE, &viewproj[0][0]);
    glUseProgram(progWire);
    glUniformMatrix4fv(unifWireViewProj, 1, GL_FALSE, &viewproj[0][0]);

    //printGLErrorLog();
}

void MyGLWidget::timerUpdate()
{    // This function is called roughly 60 times per second.
    // Use it to update your scene and then tell it to redraw.
    // (Don't update your scene in paintGL, because it
    // sometimes gets called automatically by Qt.)

    // update spin angle
    // TODO: add 'spin' bool to control spin
    if (spin) angle+= .01;
    if (angle >= 2 * PI) angle = 0;
    rayTheta += .01;
    //rayP1 = cam->sphericalToCartesian(100, rayTheta, rayTheta/2);
//    Face* selectedFace = mesh->rayPolygonInteresction(rayP0, rayP1);
//    if (selectedFace != NULL) selectedFace->active = true;
    redrawMesh();

    updateGL();
}

void MyGLWidget::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create a matrix to pass to the model matrix uniform variable in the
    // vertex shader, which is used to transform the vertices in our draw call.
    // The default provided value is an identity matrix; you'll change this.
    //sampleUploadCube();
    resizeGL(cam->getWidth(), cam->getHeight());
    modelmat = glm::rotate( glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)
    );

    // Draw mesh
    drawMesh(modelmat, mesh->triangleCount);
    // Draw a cube (12 triangles)
    // sampleDrawCube(modelmat, 12);

    // Draw a wireframe
    //     Optionally, we temporarily disable depth testing; this causes these
    //     lines to appear in front of everything drawn previously.
   // sampleDrawPoint(modelmat, mesh->activeVertexCount);
    drawRay(modelmat, 1);

  glDisable(GL_DEPTH_TEST);
  Grid* grid = new Grid();
  if (cam->currView != cam->PERSP) {
      grid->generateXZGrid(0.0f);
      if (showGrid) sampleDrawWire(modelmat, grid->vertexCount);
  }
  glEnable(GL_DEPTH_TEST);
  if (cam->currView == cam->PERSP) {
      grid->generateXYGrid(0.0f);
      if (showGrid) sampleDrawWire(modelmat, grid->vertexCount);
  }
  if (latticePoints.size()>0) drawLattice(modelmat, latticePoints.size());

    // Check for any GL errors that have happened recently
    //printGLErrorLog();
}

void MyGLWidget::redrawMesh() {
    uploadMesh();
    sampleUploadWire();
   // uploadRay(rayP0, rayP1);
    if (mesh->activeVertexCount >= 1) sampleUploadPoint();
    if (latticePoints.size() > 0) uploadLattice();
    paintGL();
    emit updateListSignal();
}

void MyGLWidget::createMesh() {

    mesh->clearFaceList();
    mesh->resetIds();

    /// Front face
    // create vertices
    Vertex * v0 = new Vertex(mesh->getVid(), 0.5f, 0.5f, -0.5f);
    Vertex * v1 = new Vertex(mesh->getVid(), -0.5f, 0.5f, -0.5f);
    Vertex * v2 = new Vertex(mesh->getVid(),-0.5f, -0.5f, -0.5f);
    Vertex * v3 = new Vertex(mesh->getVid(), 0.5f, -0.5f, -0.5f);
    // create half edges
    HalfEdge * he0 = new HalfEdge(mesh->getHEid());
    HalfEdge * he1 = new HalfEdge(mesh->getHEid());
    HalfEdge * he2 = new HalfEdge(mesh->getHEid());
    HalfEdge * he3 = new HalfEdge(mesh->getHEid());
    v0->active = true;

    // create face
    Face * front = new Face(mesh->getFid());
    front->halfedge = he0;
    front->setColorVec(glm::vec3(0,0,1));
    // assign vertices
    he0->vert = v0;
    he1->vert = v1;
    he2->vert = v2;
    he3->vert = v3;
   // assign next half edge
    he0->next = he1;
    he1->next = he2;
    he2->next = he3;
    he3->next = he0;
    // assign face
    he0->face = front;
    he1->face = front;
    he2->face = front;
    he3->face = front;


    /// Top Face
    // create vertices
    Vertex * v4 = new Vertex(mesh->getVid(), 0.5f, 0.5f, 0.5f);
    Vertex * v5 = new Vertex(mesh->getVid(), -0.5f, 0.5f, 0.5f);
    // create half edges
    HalfEdge * he4 = new HalfEdge(mesh->getHEid());
    HalfEdge * he5 = new HalfEdge(mesh->getHEid());
    HalfEdge * he6 = new HalfEdge(mesh->getHEid());
    HalfEdge * he7 = new HalfEdge(mesh->getHEid());
    // create face
    Face * top = new Face(mesh->getFid());
    top->halfedge = he4;
    top->setColorVec(glm::vec3(0,1,1));
    // assign vertices
    he4->vert = v4;
    he5->vert = v5;
    he6->vert = v1;
    he7->vert = v0;
    // assign next half edge
    he4->next = he5;
    he5->next = he6;
    he6->next = he7;
    he7->next = he4;
    // assign face
    he4->face = top;
    he5->face = top;
    he6->face = top;
    he7->face = top;


    /// bottom face;
    // create vertices
    Vertex * v6 = new Vertex(mesh->getVid(),-0.5f, -0.5f, 0.5f);
    Vertex * v7 = new Vertex(mesh->getVid(), 0.5f, -0.5f, 0.5f);
    // create halfEdges
    HalfEdge * he8 = new HalfEdge(mesh->getHEid());
    HalfEdge * he9 = new HalfEdge(mesh->getHEid());
    HalfEdge * he10 = new HalfEdge(mesh->getHEid());
    HalfEdge * he11 = new HalfEdge(mesh->getHEid());
    // create face
    Face * bottom = new Face(mesh->getFid());
    bottom->halfedge = he8;
    bottom->setColorVec(glm::vec3(0,1,0));
    // assign vertices
    he8->vert = v3;
    he9->vert = v2;
    he10->vert = v6;
    he11->vert = v7;
    // assign next half edge
    he8->next = he9;
    he9->next = he10;
    he10->next = he11;
    he11->next = he8;
    // assign face
    he8->face = bottom;
    he9->face = bottom;
    he10->face = bottom;
    he11->face = bottom;

    /// Right Face
    // create halfEdges
    HalfEdge * he12 = new HalfEdge(mesh->getHEid());
    HalfEdge * he13 = new HalfEdge(mesh->getHEid());
    HalfEdge * he14 = new HalfEdge(mesh->getHEid());
    HalfEdge * he15 = new HalfEdge(mesh->getHEid());
    // create face
    Face * right = new Face(mesh->getFid());
    right->halfedge = he12;
    right->setColorVec(glm::vec3(1,1,0));
    // assign vertices
    he12->vert = v4;
    he13->vert = v0;
    he14->vert = v3;
    he15->vert = v7;
    // assign next half edge
    he12->next = he13;
    he13->next = he14;
    he14->next = he15;
    he15->next = he12;
    // assign face
    he12->face = right;
    he13->face = right;
    he14->face = right;
    he15->face = right;

    /// left face
    // create halfEdges
    HalfEdge * he16 = new HalfEdge(mesh->getHEid());
    HalfEdge * he17 = new HalfEdge(mesh->getHEid());
    HalfEdge * he18 = new HalfEdge(mesh->getHEid());
    HalfEdge * he19 = new HalfEdge(mesh->getHEid());
    // create face
    Face * left = new Face(mesh->getFid());
    left->halfedge = he16;
    left->setColorVec(glm::vec3(1,0,0));
    // assign vertices
    he16->vert = v1;
    he17->vert = v5;
    he18->vert = v6;
    he19->vert = v2;
    // assign next half edge
    he16->next = he17;
    he17->next = he18;
    he18->next = he19;
    he19->next = he16;
    // assign face
    he16->face = left;
    he17->face = left;
    he18->face = left;
    he19->face = left;

    /// back face
    // create halfEdges
    HalfEdge * he20 = new HalfEdge(mesh->getHEid());
    HalfEdge * he21 = new HalfEdge(mesh->getHEid());
    HalfEdge * he22 = new HalfEdge(mesh->getHEid());
    HalfEdge * he23 = new HalfEdge(mesh->getHEid());
    // create face
    Face * back = new Face(mesh->getFid());
    back->halfedge = he20;
    back->setColorVec(glm::vec3(.7,0,.8));
    // assign vertices
    he20->vert = v5;
    he21->vert = v4;
    he22->vert = v7;
    he23->vert = v6;
    // assign next half edge
    he20->next = he21;
    he21->next = he22;
    he22->next = he23;
    he23->next = he20;
    // assign face
    he20->face = back;
    he21->face = back;
    he22->face = back;
    he23->face = back;

    // assign symetric half edges
    he0->sym = he14;
    he14->sym = he0;

    he1->sym = he7;
    he7->sym = he1;

    he2->sym = he16;
    he16->sym = he2;

    he3->sym = he9;
    he9->sym = he3;

    he4->sym = he13;
    he13->sym = he4;

    he5->sym = he21;
    he21->sym = he5;

    he6->sym = he17;
    he17->sym = he6;

    he8->sym = he15;
    he15->sym = he8;

    he10->sym = he19;
    he19->sym = he10;

    he11->sym = he23;
    he23->sym = he11;

    he12->sym = he22;
    he22->sym = he12;

    he20->sym = he18;
    he18->sym = he20;

    mesh->addFace(front);
    mesh->addFace(top);
    mesh->addFace(bottom);
    mesh->addFace(right);
    mesh->addFace(left);
    mesh->addFace(back);


}

void MyGLWidget::uploadMesh() {
    mesh->updateMesh();
    // VERTEX/MESH DATA
    // ================
    // Number of vertices (4 per face)
    int VERTICES = mesh->getVertexCount();
//    cout << "VERTICES: " << VERTICES << endl;
    // Number of triangles (2 per face)
    int TRIANGLES = mesh->getTriangleCount();
//    cout << "TRIANGLES: " << TRIANGLES << endl;

//    cout << "POINTS: ";
//    printFloatVec(mesh->points, mesh->getVertexCount());

//    cout << "Normals: ";
//    printFloatVec(mesh->normals, mesh->getVertexCount());

//    cout << "POINTS: ";
//    printFloatVec(mesh->colors, mesh->getVertexCount());

//    cout << "POINTS: ";
//    printUIntVec(mesh->indices, mesh->getTriangleCount());

    // Sizes of the various array elements below.
    static const GLsizei SIZE_POS = 4 * sizeof(float);
    static const GLsizei SIZE_NOR = 4 * sizeof(float);
    static const GLsizei SIZE_COL = 4 * sizeof(float);
    static const GLsizei SIZE_TRI = 3 * sizeof(unsigned int);

    // UPLOADING CODE (GENERALLY, ONCE PER CHANGE IN DATA)
    // ===================================================


    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffPositions);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS,  &mesh->points[0][0], GL_DYNAMIC_DRAW);

    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffNormals);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_NOR,  &mesh->normals[0][0], GL_DYNAMIC_DRAW);

    // Upload vertex colors (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffColors);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_COL,  &mesh->colors[0][0], GL_DYNAMIC_DRAW);

    // Upload a list of indices to the GPU
    // (these indices are applied to both positions and colors)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboDiffIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, TRIANGLES * SIZE_TRI,  &mesh->indices[0][0], GL_DYNAMIC_DRAW);

    // Check for OpenGL errors
   // printGLErrorLog();
}

void MyGLWidget::drawMesh(const glm::mat4 &model, int count) {
    // DRAWING CODE (GENERALLY, ONCE PER FRAME)
    // ========================================

    // Tell the GPU which shader program to use to draw things
    glUseProgram(progDiff);

    // Set the 4x4 model transformation matrices
    // Pointer to the first element of the array
    glUniformMatrix4fv(unifDiffModel, 1, GL_FALSE, &model[0][0]);
    // Also upload the inverse transpose for normal transformation
    const glm::mat4 modelInvTranspose = glm::inverse(glm::transpose(model));
    glUniformMatrix4fv(unifDiffModelInvTranspose, 1, GL_FALSE, &modelInvTranspose[0][0]);

    // Enable the two vertex data inputs
    glEnableVertexAttribArray(attrDiffPositions);
    glEnableVertexAttribArray(attrDiffNormals);
    glEnableVertexAttribArray(attrDiffColors);

    // Tell the GPU where the positions are: in the position buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffPositions);
    glVertexAttribPointer(attrDiffPositions, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the positions are: in the normal buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffNormals);
    glVertexAttribPointer(attrDiffNormals, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the colors are: in the color buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffColors);
    glVertexAttribPointer(attrDiffColors, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the indices are: in the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboDiffIndices);

    // Draw the triangles using the data we have given to the GPU
    glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, 0);

    // Tell the GPU we are finished drawing using position and color
    glDisableVertexAttribArray(attrDiffPositions);
    glDisableVertexAttribArray(attrDiffNormals);
    glDisableVertexAttribArray(attrDiffColors);

    // Check for OpenGL errors
    printGLErrorLog();
}

void MyGLWidget::sampleUploadCube()
{
    // Refer to the OpenGL lecture slides for extra explanation of functions

    // VERTEX/MESH DATA
    // ================

    // Number of vertices (4 per face)
    static const int VERTICES = 4 * 6;
    // Number of triangles (2 per face)
    static const int TRIANGLES = 2 * 6;

    // Sizes of the various array elements below.
    static const GLsizei SIZE_POS = 4 * sizeof(float);
    static const GLsizei SIZE_NOR = 4 * sizeof(float);
    static const GLsizei SIZE_COL = 4 * sizeof(float);
    static const GLsizei SIZE_TRI = 3 * sizeof(GLuint);

    // Initialize an array of floats to hold our cube's position data.
    // Each vertex is {x,y,z,w} where w is the homogeneous coordinate
    GLfloat positions[VERTICES][4] = {
        // v0            v1               v2               v3
        {+1, +1, +1, 1}, {-1, +1, +1, 1}, {-1, -1, +1, 1}, {+1, -1, +1, 1},
        // v0            v3               v4               v5
        {+1, +1, +1, 1}, {+1, -1, +1, 1}, {+1, -1, -1, 1}, {+1, +1, -1, 1},
        // v0            v5               v6               v1
        {+1, +1, +1, 1}, {+1, +1, -1, 1}, {-1, +1, -1, 1}, {-1, +1, +1, 1},
        // v1            v6               v7               v2
        {-1, +1, +1, 1}, {-1, +1, -1, 1}, {-1, -1, -1, 1}, {-1, -1, +1, 1},
        // v7            v4               v3               v2
        {-1, -1, -1, 1}, {+1, -1, -1, 1}, {+1, -1, +1, 1}, {-1, -1, +1, 1},
        // v4            v7               v6               v5
        {+1, -1, -1, 1}, {-1, -1, -1, 1}, {-1, +1, -1, 1}, {+1, +1, -1, 1},
    };

    // Same as above for the cube's normal data.
    GLfloat normals[VERTICES][4] = {
        // v0            v1               v2               v3
        {+0, +0, +1, 0}, {+0, +0, +1, 0}, {+0, +0, +1, 0}, {+0, +0, +1, 0},
        // v0            v3               v4               v5
        {+1, +0, +0, 0}, {+1, +0, +0, 0}, {+1, +0, +0, 0}, {+1, +0, +0, 0},
        // v0            v5               v6               v1
        {+0, +1, +0, 0}, {+0, +1, +0, 0}, {+0, +1, +0, 0}, {+0, +1, +0, 0},
        // v1            v6               v7               v2
        {-1, +0, +0, 0}, {-1, +0, +0, 0}, {-1, +0, +0, 0}, {-1, +0, +0, 0},
        // v7            v4               v3               v2
        {+0, -1, +0, 0}, {+0, -1, +0, 0}, {+0, -1, +0, 0}, {+0, -1, +0, 0},
        // v4            v7               v6               v5
        {+0, +0, -1, 0}, {+0, +0, -1, 0}, {+0, +0, -1, 0}, {+0, +0, -1, 0},
    };

    // Initialize an array of floats to hold our cube's color data
    // Color elements are in the range [0, 1], {r,g,b,a} where a is alpha
    GLfloat colors[VERTICES][4] = {
        // v0         v1            v2            v3
        {1, 1, 1, 1}, {1, 1, 0, 1}, {1, 0, 0, 1}, {1, 0, 1, 1},
        // v0         v3            v4            v5
        {1, 1, 1, 1}, {1, 0, 1, 1}, {0, 0, 1, 1}, {0, 1, 1, 1},
        // v0         v5            v6            v1
        {1, 1, 1, 1}, {0, 1, 1, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
        // v1         v6            v7            v2
        {1, 1, 0, 1}, {0, 1, 0, 1}, {0, 0, 0, 1}, {1, 0, 0, 1},
        // v7         v4            v3            v2
        {0, 0, 0, 1}, {0, 0, 1, 1}, {1, 0, 1, 1}, {1, 0, 0, 1},
        // v4         v7            v6            v5
        {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 0, 0},
    };

    // Initialize an array of six unsigned ints to hold our cube's index data
    // (2 triangles * 3 vertices/triangle = 6 indices)
    GLuint indices[TRIANGLES][3] = {
        {0, 1, 2}, {0, 2, 3},       // Front
        {4, 5, 6}, {4, 6, 7},       // Right
        {8, 9, 10}, {8, 10, 11},    // Top
        {12, 13, 14}, {12, 14, 15}, // Left
        {16, 17, 18}, {16, 18, 19}, // Bottom
        {20, 21, 22}, {20, 22, 23}, // Back
    };

    // UPLOADING CODE (GENERALLY, ONCE PER CHANGE IN DATA)
    // ===================================================

    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffPositions);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, positions, GL_DYNAMIC_DRAW);

    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffNormals);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_NOR, normals, GL_DYNAMIC_DRAW);

    // Upload vertex colors (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffColors);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_COL, colors, GL_DYNAMIC_DRAW);

    // Upload a list of indices to the GPU
    // (these indices are applied to both positions and colors)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboDiffIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, TRIANGLES * SIZE_TRI, indices, GL_DYNAMIC_DRAW);

    // Check for OpenGL errors
    //printGLErrorLog();
}

void MyGLWidget::uploadRay(glm::vec3 P0, glm::vec3 P1) {
    // Refer to the OpenGL lecture slides for extra explanation of functions

    // VERTEX/MESH DATA
    // ================

    // Number of vertices
    static const int VERTICES = 2;
    // Number of lines
    static const int LINES = 1;

    // Sizes of the various array elements below.
    static const GLsizei SIZE_POS = 4 * sizeof(float);
    static const GLsizei SIZE_COL = 4 * sizeof(float);
    static const GLsizei SIZE_LINE = 2 * sizeof(GLuint);

    // Initialize an array of floats to hold our cube's position data.
    // Each vertex is {x,y,z,w} where w is the homogeneous coordinate
    GLfloat positions[VERTICES][4] = {
        {P0[0], P0[1], P0[2], 1}, {P1[0], P1[1], P1[2], 1},
    };

    // Initialize an array of floats to hold our cube's color data
    // Color elements are in the range [0, 1], {r,g,b,a} where a is alpha
    GLfloat colors[VERTICES][4] = {
        {1, 1, 1, 1}, {1, 1, 1, 1},
    };

    // Initialize an array of six unsigned ints to hold our cube's index data
    GLuint indices[LINES][2] = {
        {0, 1},
    };

    // UPLOADING CODE (GENERALLY, ONCE PER CHANGE IN DATA)
    // ===================================================

    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboRayPositions);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, positions, GL_DYNAMIC_DRAW);

    // Upload vertex colors (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboRayColors);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_COL, colors, GL_DYNAMIC_DRAW);

    // Upload a list of indices to the GPU
    // (these indices are applied to both positions and colors)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRayIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, LINES * SIZE_LINE, indices, GL_DYNAMIC_DRAW);

    // Check for OpenGL errors
    //printGLErrorLog();
}

void MyGLWidget::sampleUploadWire()
{
    // Refer to the OpenGL lecture slides for extra explanation of functions

    // VERTEX/MESH DATA
    // ================
    Grid* grid = new Grid();
    if (cam->currView == cam->TOP || cam->currView == cam->BOTTOM || cam->currView == cam->PERSP)
        grid->generateXYGrid(cam->orthoZoom);
    else if (cam->currView == cam->BACK || cam->currView == cam->FRONT)
        grid->generateYZGrid(cam->orthoZoom);
    else if (cam->currView == cam->LEFT || cam->currView == cam->RIGHT)
        grid->generateXZGrid(cam->orthoZoom);



    // Number of vertices
    static const int VERTICES = grid->vertexCount;
    // Number of lines
    static const int LINES = VERTICES / 2;

    // Sizes of the various array elements below.
    static const GLsizei SIZE_POS = 4 * sizeof(float);
    static const GLsizei SIZE_COL = 4 * sizeof(float);
    static const GLsizei SIZE_LINE = 2 * sizeof(GLuint);

    // Initialize an array of floats to hold our cube's position data.
    // Each vertex is {x,y,z,w} where w is the homogeneous coordinate
//    GLfloat positions[VERTICES][4] = {
//        // left/right
//        {0, -100, +2.5, 1}, {0, +100, +2.5, 1},
//        {0, -100, +2.0, 1}, {0, +100, +2.0, 1},
//        {0, -100, +1.5, 1}, {0, +100, +1.5, 1},

//        {0, -100, +1.0, 1}, {0, +100, +1.0, 1},
//        {0, -100, +0.5, 1}, {0, +100, +0.5, 1},
//        {0, -100,   +0, 1}, {0, +100,   +0, 1},
//        {0, -100, -0.5, 1}, {0, +100, -0.5, 1},
//        {0, -100, -1.0, 1}, {0, +100, -1.0, 1},

//        {0, -100, -1.5, 1}, {0, +100, -1.5, 1},
//        {0, -100, -2.0, 1}, {0, +100, -2.0, 1},
//        {0, -100, -2.5, 1}, {0, +100, -2.5, 1},

//        // up/down

//        {0, +2.5, -100, 1}, {0, +2.5, +100, 1},
//        {0, +2.0, -100, 1}, {0, +2.0, +100, 1},
//        {0, +1.5, -100, 1}, {0, +1.5, +100, 1},

//        {0, +1.0, -100, 1}, {0,+1.0, +100, 1},
//        {0, +0.5, -100, 1}, {0,+0.5, +100, 1},
//        {0,   +0, -100, 1}, {0,  +0, +100, 1},
//        {0, -0.5, -100, 1}, {0,-0.5, +100, 1},
//        {0, -1.0, -100, 1}, {0,-1.0, +100, 1},

//        {0, -1.5, -100, 1}, {0, -1.5, +100, 1},
//        {0, -2.0, -100, 1}, {0, -2.0, +100, 1},
//        {0, -2.5, -100, 1}, {0, -2.5, +100, 1}
//    };

//    // Initialize an array of floats to hold our cube's color data
//    // Color elements are in the range [0, 1], {r,g,b,a} where a is alpha
//    GLfloat colors[VERTICES][4] = {
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},

//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1},
//        {1, 1, 1, 1}, {1, 1, 1, 1}
//    };

//    // Initialize an array of six unsigned ints to hold our cube's index data
//    GLuint indices[LINES][2] = {
//        {0, 1}, {2,3}, {4,5}, {6,7}, {8,9},
//        {10, 11}, {12,13}, {14,15}, {16,17}, {18,19},
//        {20, 21}, {22,23}, {24,25}, {26,27}, {28,29},
//        {30, 31}, {32,33}, {34,35}, {36,37}, {38,39},
//        {40, 41}, {42,43}
//    };

    // UPLOADING CODE (GENERALLY, ONCE PER CHANGE IN DATA)
    // ===================================================

    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboWirePositions);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, &((*grid->points).at(0)), GL_DYNAMIC_DRAW);

    // Upload vertex colors (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboWireColors);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_COL, &((*grid->colors).at(0)), GL_DYNAMIC_DRAW);

    // Upload a list of indices to the GPU
    // (these indices are applied to both positions and colors)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboWireIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, LINES * SIZE_LINE, &((*grid->indices).at(0)), GL_DYNAMIC_DRAW);


//    cout << "vertcount: " << VERTICES << endl;
//    cout << "linescount: " << LINES << endl;

    // Check for OpenGL errors
    //printGLErrorLog();
}

void MyGLWidget::sampleUploadPoint()
{
    // Refer to the OpenGL lecture slides for extra explanation of functions

    // VERTEX/MESH DATA
    // ================
    // Number of vertices
    int VERTICES = mesh->activeVertexCount;

    // Sizes of the various array elements below.
    static const GLsizei SIZE_POS = 4 * sizeof(float);
    static const GLsizei SIZE_COL = 4 * sizeof(float);
    static const GLsizei SIZE_LINE = 2 * sizeof(unsigned int);

    // Initialize an array of floats to hold our cube's position data.
    // Each vertex is {x,y,z,w} where w is the homogeneous coordinate
//    GLfloat positions[VERTICES][4] = {
//        {+1, +1, +1, 1},
//    };

    // Initialize an array of floats to hold our cube's color data
    // Color elements are in the range [0, 1], {r,g,b,a} where a is alpha
//    GLfloat colors[VERTICES][4] = {
//        {1, 1, 1, 1},
//    };

    // Initialize an array of six unsigned ints to hold our cube's index data
//    GLuint indices[VERTICES] = {
//        0,
//    };

    // UPLOADING CODE (GENERALLY, ONCE PER CHANGE IN DATA)
    // ===================================================

    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboPointPositions);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, &((mesh->activeVertexPoints)->at(0)), GL_DYNAMIC_DRAW);

    // Upload vertex colors (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboPointColors);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_COL, &((mesh->activeVertexColors)->at(0)), GL_DYNAMIC_DRAW);

    // Upload a list of indices to the GPU
    // (these indices are applied to both positions and colors)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPointIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTICES * SIZE_LINE, &((*mesh->activeVertexIndices).at(0)), GL_DYNAMIC_DRAW);

    // Check for OpenGL errors
    //printGLErrorLog();
}

void MyGLWidget::sampleDrawCube(const glm::mat4 &model, int count)
{
    // DRAWING CODE (GENERALLY, ONCE PER FRAME)
    // ========================================

    // Tell the GPU which shader program to use to draw things
    glUseProgram(progDiff);

    // Set the 4x4 model transformation matrices
    // Pointer to the first element of the array
    glUniformMatrix4fv(unifDiffModel, 1, GL_FALSE, &model[0][0]);
    // Also upload the inverse transpose for normal transformation
    const glm::mat4 modelInvTranspose = glm::inverse(glm::transpose(model));
    glUniformMatrix4fv(unifDiffModelInvTranspose, 1, GL_FALSE, &modelInvTranspose[0][0]);

    // Enable the two vertex data inputs
    glEnableVertexAttribArray(attrDiffPositions);
    glEnableVertexAttribArray(attrDiffNormals);
    glEnableVertexAttribArray(attrDiffColors);

    // Tell the GPU where the positions are: in the position buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffPositions);
    glVertexAttribPointer(attrDiffPositions, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the positions are: in the normal buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffNormals);
    glVertexAttribPointer(attrDiffNormals, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the colors are: in the color buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboDiffColors);
    glVertexAttribPointer(attrDiffColors, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the indices are: in the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboDiffIndices);

    // Draw the triangles using the data we have given to the GPU
    glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, 0);

    // Tell the GPU we are finished drawing using position and color
    glDisableVertexAttribArray(attrDiffPositions);
    glDisableVertexAttribArray(attrDiffNormals);
    glDisableVertexAttribArray(attrDiffColors);

    // Check for OpenGL errors
    //printGLErrorLog();
}


void MyGLWidget::drawRay(const glm::mat4 &model, int count)
{
    // DRAWING CODE (GENERALLY, ONCE PER FRAME)
    // ========================================
    glm::mat4 identity(1.0);
    // Tell the GPU which shader program to use to draw things
    glUseProgram(progWire);

    // Set the 4x4 model transformation matrices
    // Pointer to the first element of the array
    glUniformMatrix4fv(unifWireModel, 1, GL_FALSE, &identity[0][0]);
    // Also upload the inverse transpose for normal transformation

    // Enable the two vertex data inputs
    glEnableVertexAttribArray(attrWirePositions);
    glEnableVertexAttribArray(attrWireColors);

    // Tell the GPU where the positions are: in the position buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboRayPositions);
    glVertexAttribPointer(attrWirePositions, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the colors are: in the color buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboRayColors);
    glVertexAttribPointer(attrWireColors, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the indices are: in the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRayIndices);

    // Draw the triangles using the data we have given to the GPU
    glDrawElements(GL_LINES, count * 2, GL_UNSIGNED_INT, 0);

    // Tell the GPU we are finished drawing using position and color
    glDisableVertexAttribArray(attrWirePositions);
    glDisableVertexAttribArray(attrWireColors);

    // Check for OpenGL errors
    //printGLErrorLog();
}


void MyGLWidget::sampleDrawWire(const glm::mat4 &model, int count)
{
    // DRAWING CODE (GENERALLY, ONCE PER FRAME)
    // ========================================

    // Tell the GPU which shader program to use to draw things
    glUseProgram(progWire);

    // Set the 4x4 model transformation matrices
    // Pointer to the first element of the array
    glUniformMatrix4fv(unifWireModel, 1, GL_FALSE, &model[0][0]);
    // Also upload the inverse transpose for normal transformation

    // Enable the two vertex data inputs
    glEnableVertexAttribArray(attrWirePositions);
    glEnableVertexAttribArray(attrWireColors);

    // Tell the GPU where the positions are: in the position buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboWirePositions);
    glVertexAttribPointer(attrWirePositions, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the colors are: in the color buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboWireColors);
    glVertexAttribPointer(attrWireColors, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the indices are: in the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboWireIndices);

    // Draw the triangles using the data we have given to the GPU
    glDrawElements(GL_LINES, count * 2, GL_UNSIGNED_INT, 0);

    // Tell the GPU we are finished drawing using position and color
    glDisableVertexAttribArray(attrWirePositions);
    glDisableVertexAttribArray(attrWireColors);

    // Check for OpenGL errors
    //printGLErrorLog();
}

void MyGLWidget::sampleDrawPoint(const glm::mat4 &model, int count)
{
    // DRAWING CODE (GENERALLY, ONCE PER FRAME)
    // ========================================

    // Tell the GPU which shader program to use to draw things
    glUseProgram(progWire);

    // Set the 4x4 model transformation matrices
    // Pointer to the first element of the array
    glUniformMatrix4fv(unifWireModel, 1, GL_FALSE, &model[0][0]);
    // Also upload the inverse transpose for normal transformation

    // Enable the two vertex data inputs
    glEnableVertexAttribArray(attrWirePositions);
    glEnableVertexAttribArray(attrWireColors);

    // Tell the GPU where the positions are: in the position buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboPointPositions);
    glVertexAttribPointer(attrWirePositions, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the colors are: in the color buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboPointColors);
    glVertexAttribPointer(attrWireColors, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the indices are: in the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPointIndices);

    // Draw the triangles using the data we have given to the GPU
    glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, 0);

    // Tell the GPU we are finished drawing using position and color
    glDisableVertexAttribArray(attrWirePositions);
    glDisableVertexAttribArray(attrWireColors);

    // Check for OpenGL errors
    //printGLErrorLog();
}

std::string MyGLWidget::readFile(const char *filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Failed to load file: " << filename << std::endl;
        exit(1);
    }
    QString source = QTextStream(&file).readAll();
    std::string ret = source.toStdString();
    //std::cout << ret << std::endl;
    file.close();
    return ret;
}

void MyGLWidget::printGLErrorLog()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << error << ": ";
        const char * e =
            error == GL_INVALID_OPERATION             ? "GL_INVALID_OPERATION" :
            error == GL_INVALID_ENUM                  ? "GL_INVALID_ENUM" :
            error == GL_INVALID_VALUE                 ? "GL_INVALID_VALUE" :
            error
                                                        == GL_INVALID_INDEX                 ? "GL_INVALID_INDEX" :
            QString::number(error).toUtf8().constData();
        std::cerr << e << std::endl;
        // Throwing here allows us to use the debugger to track down the error.
#ifndef __APPLE__
        throw;
#endif
    }
}

void MyGLWidget::printLinkInfoLog(int prog)
{
    GLint linked;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL LINK ERROR" << std::endl;

    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}

void MyGLWidget::printShaderInfoLog(int shader)
{
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL COMPILE ERROR" << std::endl;

    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}

void holdOldCode() {
    //    Vertex * v0 = new Vertex(mesh->getVid(), +1.0f, +1.0f, +1.0f);
    //    Vertex * v1 = new Vertex(mesh->getVid(), -1.0f, +1.0f, +1.0f);
    //    Vertex * v2 = new Vertex(mesh->getVid(), -1.0f, -1.0f, +1.0f);

    //      Vertex * v0 = new Vertex(mesh->getVid(), +1.0f, +1.0f, +0.9f);
    //      Vertex * v1 = new Vertex(mesh->getVid(), -1.0f, +1.0f, +0.9f);
    //      Vertex * v2 = new Vertex(mesh->getVid(), -1.0f, -1.0f, +0.9f);
    //      Vertex * v3 = new Vertex(mesh->getVid(), 0.0f, -1.0f, +0.9f);
    //      Vertex * v4 = new Vertex(mesh->getVid(), +1.0f, 0.0f, +0.9f);



    //    HalfEdge * he0 = new HalfEdge(mesh->getHEid());
    //    HalfEdge * he1 = new HalfEdge(mesh->getHEid());
    //    HalfEdge * he2 = new HalfEdge(mesh->getHEid());
    //    HalfEdge * he3 = new HalfEdge(mesh->getHEid());
    //    HalfEdge * he4 = new HalfEdge(mesh->getHEid());


    //    Face * test = new Face(mesh->getFid());
    //    test->halfedge = he0;
    //    test->setColorVec(glm::vec3(0.0f,0.0f,0.0f));

    //    he0->vert = v0;
    //    he1->vert = v1;
    //    he2->vert = v2;
    //    he3->vert = v3;
    //    he4->vert = v4;


    //    he0->next = he1;
    //    he1->next = he2;
    //    he2->next = he3;
    //    he3->next = he4;
    //    he4->next = he0;

    //    he0->face = test;
    //    he1->face = test;
    //    he2->face = test;
    //    he3->face = test;
    //    he4->face = test;

    //    mesh->addFace(test);

    //    Vertex * v01 = new Vertex(mesh->getVid(), +1.0f, +1.0f, +1.0f);
    //    Vertex * v11 = new Vertex(mesh->getVid(), -1.0f, +1.0f, +1.0f);
    //    Vertex * v21 = new Vertex(mesh->getVid(), -1.0f, -1.0f, +1.0f);
    //    Vertex * v31 = new Vertex(mesh->getVid(), 0.0f, -1.0f, +1.0f);
    //    Vertex * v41 = new Vertex(mesh->getVid(), +1.0f, 0.0f, +1.0f);



    //  HalfEdge * he01 = new HalfEdge(mesh->getHEid());
    //  HalfEdge * he11 = new HalfEdge(mesh->getHEid());
    //  HalfEdge * he21 = new HalfEdge(mesh->getHEid());
    //  HalfEdge * he31 = new HalfEdge(mesh->getHEid());
    //  HalfEdge * he41 = new HalfEdge(mesh->getHEid());


    //  Face * test1 = new Face(mesh->getFid());
    //  test1->halfedge = he01;
    //  test1->setColorVec(glm::vec3(1.0f,0.0f,0.0f));

    //  he01->vert = v01;
    //  he11->vert = v11;
    //  he21->vert = v21;
    //  he31->vert = v31;
    //  he41->vert = v41;


    //  he01->next = he11;
    //  he11->next = he21;
    //  he21->next = he31;
    //  he31->next = he41;
    //  he41->next = he01;

    //  he01->face = test1;
    //  he11->face = test1;
    //  he21->face = test1;
    //  he31->face = test1;
    //  he41->face = test1;

    //  mesh->addFace(test1);

}


void MyGLWidget::uploadObj()
{
    string filename = QInputDialog::getText(this, "New Object", "Enter the file (.obj format) containing the polygon data").toStdString();
    Deformer *d = new Deformer(filename);
    if (d->getMesh() != NULL) {
        mesh = d->getMesh();

    }
    redrawMesh();
}

void MyGLWidget::exportObj()
{
    string filename = QInputDialog::getText(this, "Export Object", "Enter the file name (.obj format) of the mesh you wish to export").toStdString();
    Deformer *d = new Deformer();
    d->exportMesh(filename, mesh);
}

void MyGLWidget::uploadLattice()
{
    // VERTEX/MESH DATA
    // ================
    // Number of vertices
    int VERTICES = latticePoints.size();

    // Sizes of the various array elements below.
    static const GLsizei SIZE_POS = 4 * sizeof(float);
    static const GLsizei SIZE_COL = 4 * sizeof(float);
    static const GLsizei SIZE_LINE = 2 * sizeof(GLuint);

    GLfloat positions[VERTICES][4];
    GLfloat colors[VERTICES][4];
    GLuint indices[VERTICES];
    for (int i = 0; i < VERTICES; i++) {
        Vertex *v = latticePoints[i];
        //cout<<v->getX()<<", "<<v->getY()<<", "<<v->getZ()<<endl;
        positions[i][0] = v->getX();
        positions[i][1] = v->getY();
        positions[i][2] = v->getZ();
        positions[i][3] = 1;

        if (selectedCP != i) {
            colors[i][0] = 1;
            colors[i][1] = 1;
            colors[i][2] = 1;
            colors[i][3] = 1;
        } else {
            colors[i][0] = 0;
            colors[i][1] = 1;
            colors[i][2] = 0;
            colors[i][3] = 1;
        }

        indices[i] = i;
    }

    // UPLOADING CODE (GENERALLY, ONCE PER CHANGE IN DATA)
    // ===================================================

    // Upload vertex positions (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboLatticePositions);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, positions, GL_DYNAMIC_DRAW);

    // Upload vertex colors (times 3 components) to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboLatticeColors);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_COL, colors, GL_DYNAMIC_DRAW);

    // Upload a list of indices to the GPU
    // (these indices are applied to both positions and colors)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboLatticeIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTICES * SIZE_LINE, indices, GL_DYNAMIC_DRAW);

}

void MyGLWidget::drawLattice(const glm::mat4 &model, int count)
{
    // DRAWING CODE (GENERALLY, ONCE PER FRAME)
    // ========================================

    // Tell the GPU which shader program to use to draw things
    glUseProgram(progWire);

    // Set the 4x4 model transformation matrices
    // Pointer to the first element of the array
    glUniformMatrix4fv(unifWireModel, 1, GL_FALSE, &model[0][0]);
    // Also upload the inverse transpose for normal transformation

    // Enable the two vertex data inputs
    glEnableVertexAttribArray(attrWirePositions);
    glEnableVertexAttribArray(attrWireColors);

    // Tell the GPU where the positions are: in the position buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboLatticePositions);
    glVertexAttribPointer(attrWirePositions, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the colors are: in the color buffer (4 components each)
    glBindBuffer(GL_ARRAY_BUFFER, vboLatticeColors);
    glVertexAttribPointer(attrWireColors, 4, GL_FLOAT, false, 0, NULL);

    // Tell the GPU where the indices are: in the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboLatticeIndices);

    // Draw the triangles using the data we have given to the GPU
    glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, 0);

    // Tell the GPU we are finished drawing using position and color
    glDisableVertexAttribArray(attrWirePositions);
    glDisableVertexAttribArray(attrWireColors);

}

void MyGLWidget::drawLatticePoints()
{
    uploadLattice();
    drawLattice(modelmat, latticePoints.size());
}
