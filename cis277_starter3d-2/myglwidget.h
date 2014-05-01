#ifndef MYGLWIDGET
#define MYGLWIDGET

#include <QGLWidget>
#include <QGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QInputDialog>
#include "deformer.h"
#include "glcamera.h"
#include "grid.h"

// Use radians everywhere (no degrees!)
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "mesh.h"

class MyGLWidget : public QGLWidget, QGLFunctions
{
    Q_OBJECT

public:
    MyGLWidget(QWidget* parent);
    ~MyGLWidget();

    Mesh * mesh;

    /// Draw Model
    void createMesh();
    void uploadMesh();
    void drawMesh(const glm::mat4 &model, int count);
    void redrawMesh();
    glm::mat4 modelmat;
    std::vector<Vertex*> latticePoints;
    int selectedCP;
    void drawLatticePoints();

    GLCamera * cam; // MOVED TO PUBLIC


    // Key events
    bool altPressed;

    // Mouse events
    float prevX;
    float prevY;

signals:
    void updateListSignal();

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

public slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();
    void uploadObj();
    void exportObj();

private:
   /// GLCamera
   // GLCamera * cam;
    float angle;

   /// Model
   bool spin;

   void uploadRay(glm::vec3 P0, glm::vec3 P1);
   void drawRay(const glm::mat4 &modelmat, int count);
   glm::vec3 rayP0;
   glm::vec3 rayP1;
   float rayTheta;

    /// Mini-Maya Views
    bool showGrid;


    /// Timer for animation
    QTimer *timer;

    // Buffer objects for positions, colors, normals, indices
    QOpenGLVertexArrayObject *vao;
    GLuint vboDiffPositions;
    GLuint vboDiffNormals;
    GLuint vboDiffColors;
    GLuint vboDiffIndices;
    GLuint vboWirePositions;
    GLuint vboWireColors;
    GLuint vboWireIndices;
    GLuint vboPointPositions;
    GLuint vboPointColors;
    GLuint vboPointIndices;
    GLuint vboRayPositions;
    GLuint vboRayColors;
    GLuint vboRayIndices;
    GLuint vboLatticePositions;
    GLuint vboLatticeColors;
    GLuint vboLatticeIndices;

    // Shader attribute locations
    GLuint attrDiffPositions;
    GLuint attrDiffNormals;
    GLuint attrDiffColors;
    GLuint attrWirePositions;
    GLuint attrWireColors;
    GLuint attrRayPositions;
    GLuint attrRayColors;

    // Shader uniform locations
    GLuint unifDiffViewProj;
    GLuint unifDiffModel;
    GLuint unifDiffModelInvTranspose;
    GLuint unifWireViewProj;
    GLuint unifWireModel;

    /// Shader programs
    GLuint progDiff;
    GLuint progWire;

    /// Print out some information about the current OpenGL context
    void debugContextVersion();
    /// Set up the shaders
    void shaderSetupDiff();
    void shaderSetupWire();

    /// Upload the sample cube
    void sampleUploadCube();
    /// Upload the sample line
    void sampleUploadWire();
    /// Upload the sample point
    void sampleUploadPoint();
    /// Upload lattice points
    void uploadLattice();

    /// Draw the sample cube
    void sampleDrawCube(const glm::mat4 &modelmat, int count);
    /// Draw the sample line
    void sampleDrawWire(const glm::mat4 &modelmat, int count);
    /// Draw the sample point
    void sampleDrawPoint(const glm::mat4 &modelmat, int count);
    /// Draw lattice points
    void drawLattice(const glm::mat4 &modelmat, int count);

    /// Read a text file or resource into a string.
    std::string readFile(const char* fileName);

    /// Check for GL errors, and print and crash if there is one.
    void printGLErrorLog();

    /// Check for shader program linking errors, and print if there is one.
    void printLinkInfoLog(int prog);

    /// Check for shader compilation errors, and print if there is one.
    void printShaderInfoLog(int shader);
};

#endif
