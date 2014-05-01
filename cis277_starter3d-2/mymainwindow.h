#ifndef OPENGLDEMO_H
#define OPENGLDEMO_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <iostream>
#include "vertex.h"

namespace Ui
{
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget *parent = 0);
    ~MyMainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_faceList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_vertexList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_pushButton_clicked();

    void on_triangulate_clicked();

    void on_deleteVertexButton_clicked();

    void on_addEdgeButton_clicked();

    void on_addVertexButton_clicked();

    void on_moveVertexX_valueChanged(double arg1);

    void on_moveVertexY_valueChanged(double arg1);

    void on_moveVertexZ_valueChanged(double arg1);

    void on_faceColorRed_valueChanged(double arg1);

    void on_faceColorGreen_valueChanged(double arg1);

    void on_faceColorBlue_valueChanged(double arg1);

    void on_subdivideButton_clicked();

    void on_cameraViewComboBox_activated(const QString &arg1);

    void on_planeButton_clicked();

    void on_cubeButton_clicked();

    void on_SnapVertex_clicked();

    void on_snapEdge_clicked();

    void on_Face_clicked();


    void on_createLattice_clicked();

    void on_moveCPX_valueChanged(double arg1);

    void on_moveCPY_valueChanged(double arg1);

    void on_moveCPZ_valueChanged(double arg1);

    void on_latticeList_itemClicked(QListWidgetItem *item);

    void on_spinBox_valueChanged(int twist);

public slots:
    void updateListSlot();
    void clearLists();

private:
    void deformMesh();
    float choose(float n, float k);
    Ui::MyMainWindow *ui;
    std::vector<std::vector<std::vector<Vertex*> > > lattice;
    std::vector<Vertex*> latticeList;
    double currX, currY, currZ;
};

#endif // OPENGLDEMO_H
