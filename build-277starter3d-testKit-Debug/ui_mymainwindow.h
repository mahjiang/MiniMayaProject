/********************************************************************************
** Form generated from reading UI file 'mymainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYMAINWINDOW_H
#define UI_MYMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "myglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MyMainWindow
{
public:
    QAction *actionQuit;
    QWidget *centralWidget;
    MyGLWidget *glwidget;
    QTabWidget *Tabs;
    QWidget *lists;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *faceList;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QListWidget *edgesList;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QListWidget *vertexList;
    QWidget *mesh_tab;
    QGroupBox *MeshOperations;
    QPushButton *triangulate;
    QPushButton *deleteVertexButton;
    QPushButton *addEdgeButton;
    QPushButton *addVertexButton;
    QGroupBox *MoveVertex;
    QDoubleSpinBox *moveVertexX;
    QDoubleSpinBox *moveVertexY;
    QDoubleSpinBox *moveVertexZ;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QGroupBox *FaceColor;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *faceColorRed;
    QDoubleSpinBox *faceColorGreen;
    QDoubleSpinBox *faceColorBlue;
    QPushButton *subdivideButton;
    QGroupBox *groupBox;
    QPushButton *planeButton;
    QPushButton *cubeButton;
    QWidget *tab_2;
    QLabel *label_10;
    QComboBox *cameraViewComboBox;
    QWidget *layoutWidget3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_11;
    QPushButton *SnapVertex;
    QPushButton *snapEdge;
    QPushButton *Face;
    QWidget *Deformer;
    QPushButton *pushButton;
    QLabel *label_19;
    QDoubleSpinBox *moveCPY;
    QDoubleSpinBox *moveCPX;
    QSpinBox *numLatticeZ;
    QSpinBox *numLatticeX;
    QLabel *label_14;
    QPushButton *pushButton_2;
    QLabel *label_12;
    QLabel *label_15;
    QLabel *label_18;
    QListWidget *latticeList;
    QLabel *label_16;
    QLabel *label_13;
    QSpinBox *numLatticeY;
    QLabel *label_17;
    QPushButton *createLattice;
    QDoubleSpinBox *moveCPZ;
    QLabel *label_20;
    QSpinBox *spinBox;
    QLabel *label_21;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MyMainWindow)
    {
        if (MyMainWindow->objectName().isEmpty())
            MyMainWindow->setObjectName(QStringLiteral("MyMainWindow"));
        MyMainWindow->resize(1200, 657);
        MyMainWindow->setMouseTracking(true);
        actionQuit = new QAction(MyMainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        centralWidget = new QWidget(MyMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        glwidget = new MyGLWidget(centralWidget);
        glwidget->setObjectName(QStringLiteral("glwidget"));
        glwidget->setGeometry(QRect(10, 10, 900, 600));
        glwidget->setMinimumSize(QSize(300, 300));
        glwidget->setMaximumSize(QSize(16777215, 16777215));
        glwidget->setMouseTracking(true);
        glwidget->setFocusPolicy(Qt::ClickFocus);
        glwidget->setLayoutDirection(Qt::LeftToRight);
        Tabs = new QTabWidget(centralWidget);
        Tabs->setObjectName(QStringLiteral("Tabs"));
        Tabs->setGeometry(QRect(930, 10, 261, 611));
        lists = new QWidget();
        lists->setObjectName(QStringLiteral("lists"));
        layoutWidget = new QWidget(lists);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 241, 171));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        faceList = new QListWidget(layoutWidget);
        faceList->setObjectName(QStringLiteral("faceList"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(faceList->sizePolicy().hasHeightForWidth());
        faceList->setSizePolicy(sizePolicy);
        faceList->setFocusPolicy(Qt::ClickFocus);
        faceList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        faceList->setLayoutMode(QListView::SinglePass);
        faceList->setSortingEnabled(false);

        verticalLayout->addWidget(faceList);

        layoutWidget1 = new QWidget(lists);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 190, 241, 181));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        edgesList = new QListWidget(layoutWidget1);
        edgesList->setObjectName(QStringLiteral("edgesList"));
        sizePolicy.setHeightForWidth(edgesList->sizePolicy().hasHeightForWidth());
        edgesList->setSizePolicy(sizePolicy);
        edgesList->setFocusPolicy(Qt::ClickFocus);
        edgesList->setSortingEnabled(false);

        verticalLayout_2->addWidget(edgesList);

        layoutWidget2 = new QWidget(lists);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 380, 241, 191));
        verticalLayout_3 = new QVBoxLayout(layoutWidget2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_3);

        vertexList = new QListWidget(layoutWidget2);
        vertexList->setObjectName(QStringLiteral("vertexList"));
        sizePolicy.setHeightForWidth(vertexList->sizePolicy().hasHeightForWidth());
        vertexList->setSizePolicy(sizePolicy);
        vertexList->setFocusPolicy(Qt::ClickFocus);
        vertexList->setSortingEnabled(false);

        verticalLayout_3->addWidget(vertexList);

        Tabs->addTab(lists, QString());
        mesh_tab = new QWidget();
        mesh_tab->setObjectName(QStringLiteral("mesh_tab"));
        MeshOperations = new QGroupBox(mesh_tab);
        MeshOperations->setObjectName(QStringLiteral("MeshOperations"));
        MeshOperations->setGeometry(QRect(30, 10, 201, 141));
        triangulate = new QPushButton(MeshOperations);
        triangulate->setObjectName(QStringLiteral("triangulate"));
        triangulate->setGeometry(QRect(20, 30, 81, 51));
        deleteVertexButton = new QPushButton(MeshOperations);
        deleteVertexButton->setObjectName(QStringLiteral("deleteVertexButton"));
        deleteVertexButton->setEnabled(true);
        deleteVertexButton->setGeometry(QRect(100, 80, 81, 51));
        addEdgeButton = new QPushButton(MeshOperations);
        addEdgeButton->setObjectName(QStringLiteral("addEdgeButton"));
        addEdgeButton->setGeometry(QRect(100, 30, 81, 51));
        addVertexButton = new QPushButton(MeshOperations);
        addVertexButton->setObjectName(QStringLiteral("addVertexButton"));
        addVertexButton->setGeometry(QRect(20, 80, 81, 51));
        MoveVertex = new QGroupBox(mesh_tab);
        MoveVertex->setObjectName(QStringLiteral("MoveVertex"));
        MoveVertex->setGeometry(QRect(30, 160, 201, 121));
        moveVertexX = new QDoubleSpinBox(MoveVertex);
        moveVertexX->setObjectName(QStringLiteral("moveVertexX"));
        moveVertexX->setGeometry(QRect(60, 30, 91, 24));
        moveVertexX->setMinimum(-5);
        moveVertexX->setMaximum(5);
        moveVertexX->setSingleStep(0.5);
        moveVertexY = new QDoubleSpinBox(MoveVertex);
        moveVertexY->setObjectName(QStringLiteral("moveVertexY"));
        moveVertexY->setGeometry(QRect(60, 60, 91, 24));
        moveVertexY->setMinimum(-5);
        moveVertexY->setMaximum(5);
        moveVertexY->setSingleStep(0.5);
        moveVertexZ = new QDoubleSpinBox(MoveVertex);
        moveVertexZ->setObjectName(QStringLiteral("moveVertexZ"));
        moveVertexZ->setGeometry(QRect(60, 90, 91, 24));
        moveVertexZ->setMinimum(-5);
        moveVertexZ->setMaximum(5);
        moveVertexZ->setSingleStep(0.5);
        label_4 = new QLabel(MoveVertex);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 30, 62, 16));
        label_5 = new QLabel(MoveVertex);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 60, 62, 16));
        label_6 = new QLabel(MoveVertex);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(40, 90, 62, 16));
        FaceColor = new QGroupBox(mesh_tab);
        FaceColor->setObjectName(QStringLiteral("FaceColor"));
        FaceColor->setGeometry(QRect(30, 300, 201, 121));
        label_7 = new QLabel(FaceColor);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(40, 30, 62, 16));
        label_8 = new QLabel(FaceColor);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(40, 60, 62, 16));
        label_9 = new QLabel(FaceColor);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(40, 90, 62, 16));
        faceColorRed = new QDoubleSpinBox(FaceColor);
        faceColorRed->setObjectName(QStringLiteral("faceColorRed"));
        faceColorRed->setGeometry(QRect(60, 30, 91, 24));
        faceColorRed->setMaximum(1);
        faceColorRed->setSingleStep(0.1);
        faceColorGreen = new QDoubleSpinBox(FaceColor);
        faceColorGreen->setObjectName(QStringLiteral("faceColorGreen"));
        faceColorGreen->setGeometry(QRect(60, 60, 91, 24));
        faceColorGreen->setSingleStep(0.1);
        faceColorBlue = new QDoubleSpinBox(FaceColor);
        faceColorBlue->setObjectName(QStringLiteral("faceColorBlue"));
        faceColorBlue->setGeometry(QRect(60, 90, 91, 24));
        faceColorBlue->setSingleStep(0.1);
        subdivideButton = new QPushButton(mesh_tab);
        subdivideButton->setObjectName(QStringLiteral("subdivideButton"));
        subdivideButton->setGeometry(QRect(70, 430, 121, 41));
        groupBox = new QGroupBox(mesh_tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(30, 480, 201, 91));
        planeButton = new QPushButton(groupBox);
        planeButton->setObjectName(QStringLiteral("planeButton"));
        planeButton->setGeometry(QRect(20, 30, 81, 51));
        cubeButton = new QPushButton(groupBox);
        cubeButton->setObjectName(QStringLiteral("cubeButton"));
        cubeButton->setGeometry(QRect(100, 30, 81, 51));
        Tabs->addTab(mesh_tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(70, 20, 91, 16));
        cameraViewComboBox = new QComboBox(tab_2);
        cameraViewComboBox->setObjectName(QStringLiteral("cameraViewComboBox"));
        cameraViewComboBox->setGeometry(QRect(70, 40, 121, 26));
        layoutWidget3 = new QWidget(tab_2);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(40, 90, 181, 131));
        verticalLayout_4 = new QVBoxLayout(layoutWidget3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget3);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_4->addWidget(label_11);

        SnapVertex = new QPushButton(layoutWidget3);
        SnapVertex->setObjectName(QStringLiteral("SnapVertex"));

        verticalLayout_4->addWidget(SnapVertex);

        snapEdge = new QPushButton(layoutWidget3);
        snapEdge->setObjectName(QStringLiteral("snapEdge"));

        verticalLayout_4->addWidget(snapEdge);

        Face = new QPushButton(layoutWidget3);
        Face->setObjectName(QStringLiteral("Face"));

        verticalLayout_4->addWidget(Face);

        Tabs->addTab(tab_2, QString());
        Deformer = new QWidget();
        Deformer->setObjectName(QStringLiteral("Deformer"));
        pushButton = new QPushButton(Deformer);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(70, 10, 121, 23));
        label_19 = new QLabel(Deformer);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(30, 90, 46, 13));
        label_19->setAlignment(Qt::AlignCenter);
        moveCPY = new QDoubleSpinBox(Deformer);
        moveCPY->setObjectName(QStringLiteral("moveCPY"));
        moveCPY->setGeometry(QRect(100, 500, 51, 22));
        moveCPY->setMinimum(-99);
        moveCPY->setSingleStep(0.1);
        moveCPX = new QDoubleSpinBox(Deformer);
        moveCPX->setObjectName(QStringLiteral("moveCPX"));
        moveCPX->setGeometry(QRect(30, 500, 51, 22));
        moveCPX->setMinimum(-99);
        moveCPX->setSingleStep(0.1);
        numLatticeZ = new QSpinBox(Deformer);
        numLatticeZ->setObjectName(QStringLiteral("numLatticeZ"));
        numLatticeZ->setGeometry(QRect(170, 110, 51, 21));
        numLatticeZ->setMinimum(2);
        numLatticeZ->setMaximum(5);
        numLatticeX = new QSpinBox(Deformer);
        numLatticeX->setObjectName(QStringLiteral("numLatticeX"));
        numLatticeX->setGeometry(QRect(30, 110, 51, 21));
        numLatticeX->setMinimum(2);
        numLatticeX->setMaximum(5);
        label_14 = new QLabel(Deformer);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(170, 90, 46, 13));
        label_14->setAlignment(Qt::AlignCenter);
        pushButton_2 = new QPushButton(Deformer);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(70, 40, 121, 23));
        label_12 = new QLabel(Deformer);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 70, 231, 21));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(Deformer);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(30, 480, 46, 13));
        label_15->setAlignment(Qt::AlignCenter);
        label_18 = new QLabel(Deformer);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(50, 450, 151, 21));
        label_18->setFont(font);
        label_18->setAlignment(Qt::AlignCenter);
        latticeList = new QListWidget(Deformer);
        latticeList->setObjectName(QStringLiteral("latticeList"));
        latticeList->setGeometry(QRect(30, 220, 191, 221));
        label_16 = new QLabel(Deformer);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(100, 480, 46, 13));
        label_16->setAlignment(Qt::AlignCenter);
        label_13 = new QLabel(Deformer);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(100, 90, 46, 13));
        label_13->setAlignment(Qt::AlignCenter);
        numLatticeY = new QSpinBox(Deformer);
        numLatticeY->setObjectName(QStringLiteral("numLatticeY"));
        numLatticeY->setGeometry(QRect(100, 110, 51, 21));
        numLatticeY->setMinimum(2);
        numLatticeY->setMaximum(5);
        label_17 = new QLabel(Deformer);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(170, 480, 46, 13));
        label_17->setAlignment(Qt::AlignCenter);
        createLattice = new QPushButton(Deformer);
        createLattice->setObjectName(QStringLiteral("createLattice"));
        createLattice->setGeometry(QRect(30, 140, 191, 23));
        moveCPZ = new QDoubleSpinBox(Deformer);
        moveCPZ->setObjectName(QStringLiteral("moveCPZ"));
        moveCPZ->setGeometry(QRect(170, 500, 51, 22));
        moveCPZ->setMinimum(-99);
        moveCPZ->setSingleStep(0.1);
        label_20 = new QLabel(Deformer);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(30, 180, 191, 21));
        label_20->setFont(font);
        label_20->setAlignment(Qt::AlignCenter);
        spinBox = new QSpinBox(Deformer);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(101, 550, 51, 22));
        label_21 = new QLabel(Deformer);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(100, 530, 46, 13));
        label_21->setFont(font);
        label_21->setAlignment(Qt::AlignCenter);
        Tabs->addTab(Deformer, QString());
        MyMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MyMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MyMainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);

        retranslateUi(MyMainWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), glwidget, SLOT(uploadObj()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), glwidget, SLOT(exportObj()));

        Tabs->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MyMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MyMainWindow)
    {
        MyMainWindow->setWindowTitle(QApplication::translate("MyMainWindow", "OpenGLDemo", 0));
        actionQuit->setText(QApplication::translate("MyMainWindow", "Quit", 0));
        actionQuit->setShortcut(QApplication::translate("MyMainWindow", "Ctrl+Q", 0));
        label->setText(QApplication::translate("MyMainWindow", "Faces", 0));
        label_2->setText(QApplication::translate("MyMainWindow", "Edges", 0));
        label_3->setText(QApplication::translate("MyMainWindow", "Vertices", 0));
        Tabs->setTabText(Tabs->indexOf(lists), QApplication::translate("MyMainWindow", "Lists", 0));
        MeshOperations->setTitle(QApplication::translate("MyMainWindow", "Mesh Operations", 0));
        triangulate->setText(QApplication::translate("MyMainWindow", "Split \n"
"Quad", 0));
        deleteVertexButton->setText(QApplication::translate("MyMainWindow", "Delete \n"
"Vertex", 0));
        addEdgeButton->setText(QApplication::translate("MyMainWindow", "Add \n"
"Edge", 0));
        addVertexButton->setText(QApplication::translate("MyMainWindow", "Add \n"
"Vertex", 0));
        MoveVertex->setTitle(QApplication::translate("MyMainWindow", "Move Vertex", 0));
        label_4->setText(QApplication::translate("MyMainWindow", "X:", 0));
        label_5->setText(QApplication::translate("MyMainWindow", "Y:", 0));
        label_6->setText(QApplication::translate("MyMainWindow", "Z:", 0));
        FaceColor->setTitle(QApplication::translate("MyMainWindow", "Change Face Color", 0));
        label_7->setText(QApplication::translate("MyMainWindow", "R:", 0));
        label_8->setText(QApplication::translate("MyMainWindow", "G:", 0));
        label_9->setText(QApplication::translate("MyMainWindow", "B:", 0));
        subdivideButton->setText(QApplication::translate("MyMainWindow", "Smooth Mesh", 0));
        groupBox->setTitle(QApplication::translate("MyMainWindow", "Default Meshes", 0));
        planeButton->setText(QApplication::translate("MyMainWindow", "Plane", 0));
        cubeButton->setText(QApplication::translate("MyMainWindow", "Cube", 0));
        Tabs->setTabText(Tabs->indexOf(mesh_tab), QApplication::translate("MyMainWindow", "Mesh", 0));
        label_10->setText(QApplication::translate("MyMainWindow", "Camera View:", 0));
        cameraViewComboBox->setCurrentText(QString());
        label_11->setText(QApplication::translate("MyMainWindow", "Snap Camera to Selected:", 0));
        SnapVertex->setText(QApplication::translate("MyMainWindow", "Vertex", 0));
        snapEdge->setText(QApplication::translate("MyMainWindow", "Edge", 0));
        Face->setText(QApplication::translate("MyMainWindow", "Face", 0));
        Tabs->setTabText(Tabs->indexOf(tab_2), QApplication::translate("MyMainWindow", "Cam", 0));
        pushButton->setText(QApplication::translate("MyMainWindow", "Upload Object", 0));
        label_19->setText(QApplication::translate("MyMainWindow", "X", 0));
        label_14->setText(QApplication::translate("MyMainWindow", "Z", 0));
        pushButton_2->setText(QApplication::translate("MyMainWindow", "Export Object", 0));
        label_12->setText(QApplication::translate("MyMainWindow", "Free-Form Deformation Lattice", 0));
        label_15->setText(QApplication::translate("MyMainWindow", "X", 0));
        label_18->setText(QApplication::translate("MyMainWindow", "Move Control Point", 0));
        label_16->setText(QApplication::translate("MyMainWindow", "Y", 0));
        label_13->setText(QApplication::translate("MyMainWindow", "Y", 0));
        label_17->setText(QApplication::translate("MyMainWindow", "Z", 0));
        createLattice->setText(QApplication::translate("MyMainWindow", "Create Lattice", 0));
        label_20->setText(QApplication::translate("MyMainWindow", "Control Points", 0));
        label_21->setText(QApplication::translate("MyMainWindow", "Twist", 0));
        Tabs->setTabText(Tabs->indexOf(Deformer), QApplication::translate("MyMainWindow", "Deform", 0));
        menuFile->setTitle(QApplication::translate("MyMainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MyMainWindow: public Ui_MyMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYMAINWINDOW_H
