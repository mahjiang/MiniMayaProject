#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include "myglwidget.h"
#include <set>

using namespace std;
static const float PI = 3.141592653589f;

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{   
    ui->setupUi(this);
    connect(ui->glwidget, SIGNAL(updateListSignal()), this, SLOT(updateListSlot()));
    ui->glwidget->setFocus();
    //ui->glwidget->redrawMesh();

    ui->cameraViewComboBox->addItem("Perspective");
    ui->cameraViewComboBox->addItem("Top");
    ui->cameraViewComboBox->addItem("Left");
    ui->cameraViewComboBox->addItem("Right");
    ui->cameraViewComboBox->addItem("Front");
    ui->cameraViewComboBox->addItem("Back");
    ui->cameraViewComboBox->addItem("User Defined");

}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

void MyMainWindow::on_actionQuit_triggered()
{
    close();
}

void MyMainWindow::updateListSlot() {

    for (int i = 0; i < ui->glwidget->mesh->faceCount(); i++) {
        ui->faceList->addItem(ui->glwidget->mesh->faceAt(i));
    }
    std::set<Vertex*> vertices;
    for (int i = 0; i < ui->glwidget->mesh->faceCount(); i++) {
        HalfEdge * he0 = ui->glwidget->mesh->faceAt(i)->getHE();
        HalfEdge * curr = he0->next;
        int vertCount = 1;
        while (curr != he0) {
            vertCount++;
            curr = curr->next;
        }

        curr = he0;
        for (int j = 0; j < vertCount; j++) {
            vertices.insert(curr->vert);
            ui->edgesList->addItem(curr);
            curr = curr->next;
        }
    }

    std::set<Vertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); ++it) {
         ui->vertexList->addItem(*it);
    }


    // make ray-selected face the current face in list widget
    QItemSelectionModel *smf = ui->faceList->selectionModel();
    smf->clearSelection();
    for (int i = 0; i < ui->glwidget->mesh->faceCount(); i++) {
        if (ui->glwidget->mesh->faceAt(i)->active == true) {
            ui->faceList->setCurrentItem(static_cast<QListWidgetItem *>(ui->glwidget->mesh->faceAt(i)));
            break;
        }
    }

    // make mouse-selected vertex the current vertex in list widget
    QItemSelectionModel *smv = ui->vertexList->selectionModel();
    smv->clearSelection();
    for (int i = 0; i < ui->vertexList->count(); i++) {
        Vertex * v = static_cast<Vertex *>(ui->vertexList->item(i));
        if (v->active == true) {
            ui->vertexList->setCurrentItem(static_cast<QListWidgetItem *>(v));
            break;
        }
    }
}

void MyMainWindow::on_faceList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current && previous) {
        Face * currF = static_cast<Face*>(current);
        Face * prevF = static_cast<Face*>(previous);
        currF->active = true;
        prevF->active = false;
    } else if (current) {
        Face * currF = static_cast<Face*>(current);
        currF->active = true;
    }
    ui->glwidget->redrawMesh();
}

void MyMainWindow::on_vertexList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current && previous) {
        Vertex * currV = static_cast<Vertex*>(current);
        Vertex * prevV = static_cast<Vertex*>(previous);
        prevV->active = false;
        currV->active = true;
    } else if (current) {
        Vertex * currV = static_cast<Vertex*>(current);
        currV->active = true;
    }
    ui->glwidget->redrawMesh();
}


void MyMainWindow::on_pushButton_clicked() {}


// ------------------- MESH OPERATION BUTTONS --------------------------------

void MyMainWindow::clearLists() {

    int iNum = ui->faceList->count();
    for (int i=0; i<iNum; i++)
      ui->faceList->takeItem(0);

    int vNum = ui->vertexList->count();
    for (int i=0; i<vNum; i++)
      ui->vertexList->takeItem(0);

    int eNum = ui->edgesList->count();
    for (int i=0; i<eNum; i++)
      ui->edgesList->takeItem(0);

}

void MyMainWindow::on_triangulate_clicked()
{
    if (ui->faceList->selectedItems().count() != 0) {
        Face * f = static_cast<Face *>(ui->faceList->selectedItems()[0]);
        ui->glwidget->mesh->triangulate(f);
        clearLists();
        ui->glwidget->redrawMesh();
    }
}

void MyMainWindow::on_addEdgeButton_clicked()
{
    if (ui->faceList->selectedItems().count() != 2) {
        std::cout << "Select two faces and a vertex to add an edge." << std::endl;
    }
    if (ui->faceList->selectedItems().count() == 2 && ui->vertexList->selectedItems().count() != 0) {
        Face * f = static_cast<Face *>(ui->faceList->selectedItems()[0]);
        Face * f2 = static_cast<Face *>(ui->faceList->selectedItems()[1]);
        Vertex * v = static_cast<Vertex *>(ui->vertexList->selectedItems()[0]);
        ui->glwidget->mesh->addEdge(f, f2, v);
        clearLists();
        ui->glwidget->redrawMesh();
    }
}

void MyMainWindow::on_addVertexButton_clicked()
{
    if (ui->edgesList->selectedItems().count() != 0) {
        HalfEdge * he = static_cast<HalfEdge *>(ui->edgesList->selectedItems()[0]);
        ui->glwidget->mesh->addVertex(he);
        clearLists();
        ui->glwidget->redrawMesh();
    }
}

void MyMainWindow::on_deleteVertexButton_clicked()
{


    if (ui->vertexList->selectedItems().count() != 0) {
        Vertex * v = static_cast<Vertex *>(ui->vertexList->selectedItems()[0]);
        HalfEdge* curr;
        for (int i = 0; i < ui->glwidget->mesh->faceCount(); i++) {
            HalfEdge* he0 = ui->glwidget->mesh->faceAt(i)->getHE();
            curr = he0;
            if (curr->vert == v) break;
            curr = curr->next;
            while (curr != he0) {
                if (curr->vert == v) break;
                curr = curr->next;
            }
        }
        ui->glwidget->mesh->deleteVertex(curr);
        clearLists();
        ui->glwidget->redrawMesh();
    }
}


void MyMainWindow::on_subdivideButton_clicked()
{
    ui->glwidget->mesh->subdivide();
    clearLists();
    ui->glwidget->redrawMesh();
}

void MyMainWindow::on_planeButton_clicked()
{
    ui->glwidget->mesh->plane();
    clearLists();
    ui->glwidget->redrawMesh();
}


void MyMainWindow::on_cubeButton_clicked()
{
    ui->glwidget->createMesh();
    clearLists();
    ui->glwidget->redrawMesh();
}


// ------------------- MOVE VERTEX BUTTONS ------------------------------------

void MyMainWindow::on_moveVertexX_valueChanged(double new_x)
{
    if (ui->vertexList->selectedItems().count() != 0) {
        Vertex * v = static_cast<Vertex *>(ui->vertexList->selectedItems()[0]);
        v->setPosVec(glm::vec3(new_x, v->getY(), v->getZ()));
        ui->glwidget->redrawMesh();
    }
}

void MyMainWindow::on_moveVertexY_valueChanged(double new_y)
{
    if (ui->vertexList->selectedItems().count() != 0) {
        Vertex * v = static_cast<Vertex *>(ui->vertexList->selectedItems()[0]);
        v->setPosVec(glm::vec3(v->getX(), new_y, v->getZ()));
        ui->glwidget->redrawMesh();
    }
}

void MyMainWindow::on_moveVertexZ_valueChanged(double new_z)
{
    if (ui->vertexList->selectedItems().count() != 0) {
        Vertex * v = static_cast<Vertex *>(ui->vertexList->selectedItems()[0]);
        v->setPosVec(glm::vec3(v->getX(), v->getY(), new_z));
        ui->glwidget->redrawMesh();
    }
}

// ------------------- FACE COLOR BUTTONS -------------------------------------

void MyMainWindow::on_faceColorRed_valueChanged(double new_r)
{
    if (ui->faceList->selectedItems().count() != 0) {
        Face * f = static_cast<Face *>(ui->faceList->selectedItems()[0]);
        f->setColorVec(glm::vec3(new_r, f->getColorVec()[1], f->getColorVec()[2]));
        ui->glwidget->redrawMesh();
    }
}

void MyMainWindow::on_faceColorGreen_valueChanged(double new_g)
{
    if (ui->faceList->selectedItems().count() != 0) {
        Face * f = static_cast<Face *>(ui->faceList->selectedItems()[0]);
        f->setColorVec(glm::vec3(f->getColorVec()[0], new_g, f->getColorVec()[2]));
        ui->glwidget->redrawMesh();
    }
}

void MyMainWindow::on_faceColorBlue_valueChanged(double new_b)
{
    if (ui->faceList->selectedItems().count() != 0) {
        Face * f = static_cast<Face *>(ui->faceList->selectedItems()[0]);
        f->setColorVec(glm::vec3(f->getColorVec()[0], f->getColorVec()[1], new_b));
        ui->glwidget->redrawMesh();
        updateListSlot();
    }
}


// ----------------------- CAMERA CONTROLS -------------------------------------


void MyMainWindow::on_cameraViewComboBox_activated(const QString &arg1)
{
    if (arg1 == "Front")
        ui->glwidget->cam->changeViewTo(ui->glwidget->cam->FRONT);
    else if (arg1 == "Back")
        ui->glwidget->cam->changeViewTo(ui->glwidget->cam->BACK);
    else if (arg1 == "Right")
        ui->glwidget->cam->changeViewTo(ui->glwidget->cam->RIGHT);
    else if (arg1 == "Left")
        ui->glwidget->cam->changeViewTo(ui->glwidget->cam->LEFT);
    else if (arg1 == "Top")
        ui->glwidget->cam->changeViewTo(ui->glwidget->cam->TOP);
    else if (arg1 == "Bottom")
        ui->glwidget->cam->changeViewTo(ui->glwidget->cam->BOTTOM);
    else if (arg1 == "Perspective")
        ui->glwidget->cam->changeViewTo(ui->glwidget->cam->PERSP);

}


void MyMainWindow::on_SnapVertex_clicked()
{
    if (ui->vertexList->selectedItems().count() > 0) {
        Vertex* v = static_cast<Vertex *>(ui->vertexList->selectedItems()[0]);
        ui->glwidget->cam->snapToVertex(v);
    }
}

void MyMainWindow::on_snapEdge_clicked()
{
    if (ui->edgesList->selectedItems().count() > 0) {
        HalfEdge* e = static_cast<HalfEdge *>(ui->edgesList->selectedItems()[0]);
        ui->glwidget->cam->snapToEdge(e);
    }
}


void MyMainWindow::on_Face_clicked()
{
    if (ui->faceList->selectedItems().count() > 0) {
        Face* f = static_cast<Face *>(ui->faceList->selectedItems()[0]);
        ui->glwidget->cam->snapToFace(f);
    }
}



void MyMainWindow::on_createLattice_clicked()
{
    while (ui->latticeList->count() > 0) {
        ui->latticeList->takeItem(0);
    }
    latticeList.clear();
    lattice.clear();

    float l = ui->numLatticeX->value();
    float m = ui->numLatticeY->value();
    float n = ui->numLatticeZ->value();

    std::vector<Vertex*> z(n, NULL);
    std::vector<std::vector<Vertex* > > yz(m, z);
    std::vector<std::vector<std::vector<Vertex*> > > xyz (l, yz);
    lattice = xyz;

    Mesh *mesh = ui->glwidget->mesh;
    set<Vertex*>* verticesTemp = mesh->getVertexSet();
    set<Vertex*> vertices = *verticesTemp;
    set<Vertex*>::iterator it;

    //find bounding box
    float minX, maxX, minY, maxY, minZ, maxZ;
    for (it = vertices.begin(); it != vertices.end(); ++it) {
        Vertex* v = *it;
        if (v->getX() < minX)
            minX = v->getX();
        else if (v->getX() > maxX)
            maxX = v->getX();
        if (v->getY() < minY)
            minY = v->getY();
        else if (v->getY() > maxY)
            maxY = v->getY();
        if (v->getZ() < minZ)
            minZ = v->getZ();
        else if (v->getZ() > maxZ)
            maxZ = v->getZ();
    }

    float sLength = maxX-minX;
    float tLength = maxY-minY;
    float uLength = maxZ-minZ;
    float x0 = minX;
    float y0 = minY;
    float z0 = minZ;
    glm::vec3 S(sLength, 0, 0);
    glm::vec3 T(0, tLength, 0);
    glm::vec3 U(0, 0, uLength);
    glm::vec3 P0(x0, y0, z0);

    for (it = vertices.begin(); it != vertices.end(); ++it) {
        Vertex* v = *it;
        v->s = glm::dot(glm::cross(T,U), (v->getPosVec()-P0))/glm::dot(glm::cross(T,U),S);
        v->t = glm::dot(glm::cross(U,S), (v->getPosVec()-P0))/glm::dot(glm::cross(U,S),T);
        v->u = glm::dot(glm::cross(S,T), (v->getPosVec()-P0))/glm::dot(glm::cross(S,T),U);
    }

    // create mesh with appropriate dimensions
    Mesh *latticeMesh = new Mesh();
    float xInt = (maxX - minX)/(l-1);
    float yInt = (maxY - minY)/(m-1);
    float zInt = (maxZ - minZ)/(n-1);
    for (float i = 0; i < l; i++) {
        float x = i * xInt + minX;
        for (float j = 0; j < m; j++) {
            float y = j * yInt + minY;
            for (float k = 0; k < n; k++) {
                float z = k * zInt + minZ;
                Vertex *v = new Vertex(latticeMesh->getVid(), x, y, z);
                latticeList.push_back(v);
                lattice[i][j][k] = v;
            }
        }
    }

    ui->glwidget->latticePoints = latticeList;
    for (unsigned int i = 0; i < latticeList.size(); i++) {
        ui->latticeList->addItem(latticeList.at(i));
    }
    ui->spinBox->setValue(0);
    ui->glwidget->redrawMesh();
}

void MyMainWindow::on_latticeList_itemClicked(QListWidgetItem *current)
{
    Vertex * currV = static_cast<Vertex*>(current);
    currV->active = true;
    currX = currV->getX();
    currY = currV->getY();
    currZ = currV->getZ();
    ui->moveCPX->setValue(currV->getX());
    ui->moveCPY->setValue(currV->getY());
    ui->moveCPZ->setValue(currV->getZ());
    ui->glwidget->selectedCP = ui->latticeList->currentRow();
    ui->glwidget->redrawMesh();
}


void MyMainWindow::on_moveCPX_valueChanged(double new_x)
{
    if (ui->latticeList->selectedItems().count() != 0) {
        Vertex * v = static_cast<Vertex *>(ui->latticeList->selectedItems()[0]);
        if (new_x != currX) {
            v->setPosVec(glm::vec3(new_x, v->getY(), v->getZ()));
            currX = new_x;
            deformMesh();
            ui->glwidget->redrawMesh();
        }
    }
}

void MyMainWindow::on_moveCPY_valueChanged(double new_y)
{
    if (ui->latticeList->selectedItems().count() != 0) {
        Vertex * v = static_cast<Vertex *>(ui->latticeList->selectedItems()[0]);
        if (new_y != currY) {
            v->setPosVec(glm::vec3(v->getX(), new_y, v->getZ()));
            currY = new_y;
            deformMesh();
            ui->glwidget->redrawMesh();
        }
    }
}

void MyMainWindow::on_moveCPZ_valueChanged(double new_z)
{
    if (ui->latticeList->selectedItems().count() != 0) {
        Vertex * v = static_cast<Vertex *>(ui->latticeList->selectedItems()[0]);
        if (new_z != currZ) {
            v->setPosVec(glm::vec3(v->getX(), v->getY(), new_z));
            currZ = new_z;
            deformMesh();
            ui->glwidget->redrawMesh();
        }
    }
}

void MyMainWindow::deformMesh()
{
    float l = ui->numLatticeX->value()-1;
    float m = ui->numLatticeY->value()-1;
    float n = ui->numLatticeZ->value()-1;

    Mesh *mesh = ui->glwidget->mesh;
    set<Vertex*>* verticesTemp = mesh->getVertexSet();
    set<Vertex*> vertices = *verticesTemp;
    set<Vertex*>::iterator it;
    for (it = vertices.begin(); it != vertices.end(); ++it) {
        Vertex* v = *it;
        glm::vec3 position(0.0f);

        for (int i = 0; i <= l; i++) {
            float sWeight = choose(l,i) * pow((1.0f - v->s), (l - i)) * pow(v->s, i);
            glm::vec3 temp1(0.0f);

            for (int j = 0; j <= m; j++) {
                float tWeight = choose(m,j) * pow((1.0f - v->t), (m - j)) * pow(v->t, j);
                glm::vec3 temp2(0.0f);

                for (int k = 0; k <= n; k++) {
                    float uWeight = choose(n,k) * pow((1.0f - v->u), (n - k)) * pow (v->u, k);
                    Vertex* CP = lattice[i][j][k];
                    glm::vec3 temp3(uWeight * CP->getX(), uWeight * CP->getY(), uWeight * CP->getZ());
                    temp2 += temp3;
                }
                temp1 += (tWeight * temp2);
            }
            position += (sWeight * temp1);
        }
        v->setPosVec(position);
    }
}

float MyMainWindow::choose(float n, float k)
{
    if(0 == k || n == k){
        return 1;
    } else if (k > n){
        return 0;
    } else if (k > (n - k)){
        k = n-k;
    } else if (k == 1){
        return n;
    }
    float nCk = 1;
    for(int i=1;i<=k;++i){
        nCk *= (n - (k - i));
        if (nCk < 0)
            return -1;
        nCk /= i;
    }
    return nCk;
}

void MyMainWindow::on_spinBox_valueChanged(int twist)
{
    if (lattice.size() > 0) {
        float l = ui->numLatticeX->value();
        float m = ui->numLatticeY->value();
        float n = ui->numLatticeZ->value();

        float baseAngle = (twist) * (PI/180);

        for (int i = 0; i < l; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < n; k++) {
                    float twistAngle = baseAngle * (1 + k/(n-1));
                    Vertex* cp = lattice[i][j][k];
                    glm::vec3 newCP ((cp->getX()*cos(twistAngle))-(cp->getY()*sin(twistAngle)),
                                     (cp->getX()*sin(twistAngle))+(cp->getY()*cos(twistAngle)),
                                     cp->getZ());
                    lattice[i][j][k]->setPosVec(newCP);
                }
            }
        }
        deformMesh();
        ui->glwidget->redrawMesh();
    }
}
