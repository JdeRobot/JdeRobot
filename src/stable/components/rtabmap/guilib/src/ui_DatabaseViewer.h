/********************************************************************************
** Form generated from reading UI file 'DatabaseViewer.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATABASEVIEWER_H
#define UI_DATABASEVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "GraphViewer.h"
#include "rtabmap/gui/CloudViewer.h"
#include "rtabmap/gui/ImageView.h"

QT_BEGIN_NAMESPACE

class Ui_DatabaseViewer
{
public:
    QAction *actionOpen_database;
    QAction *actionQuit;
    QAction *actionGenerate_graph_dot;
    QAction *actionGenerate_graph_only_weighted_locations;
    QAction *actionClean_database;
    QAction *actionGenerate_local_graph_dot;
    QAction *actionClean_local_graph;
    QAction *actionUpdate_base_ids;
    QAction *actionGenerate_3D_map_pcd;
    QAction *actionExport;
    QAction *actionExtract_images;
    QAction *actionView_3D_map;
    QAction *actionGenerate_TORO_graph_graph;
    QAction *actionRefine_all_neighbor_links;
    QAction *actionRefine_all_loop_closure_links;
    QAction *actionDetect_more_loop_closures;
    QAction *actionVisual_Refine_all_neighbor_links;
    QAction *actionVisual_Refine_all_loop_closure_links;
    QAction *actionSave_config;
    QAction *actionReset_all_changes;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_6;
    rtabmap::ImageView *graphicsView_A;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QGridLayout *gridLayout;
    QLabel *label_parentsA_2;
    QLabel *label_parentsA;
    QLabel *label_childrenA_2;
    QLabel *label_childrenA;
    QLabel *label_childrenA_4;
    QLabel *label_labelA;
    QLabel *label_childrenA_6;
    QLabel *label_stampA;
    QLabel *label_childrenA_8;
    QLabel *label_weightA;
    QLabel *label_childrenA_10;
    QLabel *label_mapA;
    QLabel *label_childrenA_12;
    QLabel *label_poseA;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_indexA;
    QLabel *label_idA;
    QSlider *horizontalSlider_A;
    QVBoxLayout *verticalLayout_5;
    rtabmap::ImageView *graphicsView_B;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QLabel *label_parentsB;
    QLabel *label_parentsA_4;
    QLabel *label_childrenA_3;
    QLabel *label_childrenB;
    QLabel *label_childrenA_5;
    QLabel *label_labelB;
    QLabel *label_childrenA_7;
    QLabel *label_stampB;
    QLabel *label_childrenA_9;
    QLabel *label_weightB;
    QLabel *label_childrenA_11;
    QLabel *label_mapB;
    QLabel *label_childrenA_13;
    QLabel *label_poseB;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_indexB;
    QLabel *label_idB;
    QSlider *horizontalSlider_B;
    QDialogButtonBox *buttonBox;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QDockWidget *dockWidget_constraints;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_8;
    rtabmap::CloudViewer *constraintsViewer;
    QGridLayout *gridLayout_6;
    QLabel *label;
    QSlider *horizontalSlider_neighbors;
    QLabel *label_6;
    QSlider *horizontalSlider_loops;
    QLabel *label_33;
    QLabel *label_type;
    QLabel *label_16;
    QLabel *label_constraint;
    QLabel *label_18;
    QLabel *label_variance;
    QCheckBox *checkBox_showOptimized;
    QLabel *label_constraint_opt;
    QLabel *label_32;
    QCheckBox *checkBox_show3DWords;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_refineVisually;
    QPushButton *pushButton_refine;
    QPushButton *pushButton_reset;
    QPushButton *pushButton_add;
    QPushButton *pushButton_reject;
    QSpacerItem *horizontalSpacer;
    QDockWidget *dockWidget_graphView;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_9;
    rtabmap::GraphViewer *graphViewer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_iterations;
    QSlider *horizontalSlider_iterations;
    QGridLayout *gridLayout_5;
    QLabel *label_loopClosures;
    QLabel *label_timeGrid;
    QLabel *label_timeOptimization;
    QSpinBox *spinBox_optimizationsFrom;
    QLabel *label_52;
    QLabel *label_39;
    QLabel *label_41;
    QLabel *label_10;
    QLabel *label_45;
    QLabel *label_pathLength;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_optimizeFrom;
    QCheckBox *checkBox_spanAllMaps;
    QLabel *label_nodes;
    QDockWidget *dockWidget_parameters;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_10;
    QToolBox *toolBox;
    QWidget *page;
    QGridLayout *gridLayout_4;
    QSpinBox *spinBox_icp_decimation;
    QLabel *label_14;
    QDoubleSpinBox *doubleSpinBox_icp_maxDepth;
    QLabel *label_17;
    QDoubleSpinBox *doubleSpinBox_icp_voxel;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBox_icp_maxCorrespDistance;
    QLabel *label_12;
    QDoubleSpinBox *doubleSpinBox_icp_minCorrespondenceRatio;
    QLabel *label_46;
    QSpinBox *spinBox_icp_iteration;
    QLabel *label_15;
    QCheckBox *checkBox_icp_p2plane;
    QLabel *label_11;
    QSpinBox *spinBox_icp_normalKSearch;
    QLabel *label_20;
    QCheckBox *checkBox_icp_2d;
    QLabel *label_27;
    QSpacerItem *verticalSpacer;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_15;
    QGridLayout *gridLayout_3;
    QSpinBox *spinBox_visual_iteration;
    QCheckBox *checkBox_visual_2d;
    QDoubleSpinBox *doubleSpinBox_visual_maxDepth;
    QLabel *label_26;
    QLabel *label_21;
    QLabel *label_28;
    QDoubleSpinBox *doubleSpinBox_visual_maxCorrespDistance;
    QDoubleSpinBox *doubleSpinBox_visual_nndr;
    QLabel *label_19;
    QSpinBox *spinBox_visual_minCorrespondences;
    QLabel *label_22;
    QLabel *label_23;
    QGroupBox *groupBox_visual_recomputeFeatures;
    QGridLayout *gridLayout_11;
    QLabel *label_24;
    QComboBox *comboBox_featureType;
    QLabel *label_25;
    QComboBox *comboBox_nnType;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_3;
    QGridLayout *gridLayout_7;
    QCheckBox *checkBox_2dslam;
    QLabel *label_48;
    QCheckBox *checkBox_ignoreUserLoop;
    QLabel *label_7;
    QSpinBox *spinBox_iterations;
    QSpinBox *spinBox_optimizationDepth;
    QLabel *label_35;
    QCheckBox *checkBox_ignoreGlobalLoop;
    QCheckBox *checkBox_ignorePoseCorrection;
    QCheckBox *checkBox_ignoreLocalLoopTime;
    QComboBox *comboBox_graphOptimizer;
    QLabel *label_8;
    QLabel *label_50;
    QLabel *label_47;
    QSpacerItem *verticalSpacer_3;
    QCheckBox *checkBox_ignoreLocalLoopSpace;
    QLabel *label_49;
    QLabel *label_40;
    QCheckBox *checkBox_ignoreCovariance;
    QLabel *label_51;
    QLabel *label_34;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_16;
    QGridLayout *gridLayout_9;
    QDoubleSpinBox *doubleSpinBox_gridCellSize;
    QLabel *label_36;
    QLabel *label_44;
    QCheckBox *checkBox_gridErode;
    QGroupBox *groupBox_gridFromProjection;
    QGridLayout *gridLayout_8;
    QSpinBox *spinBox_projDecimation;
    QLabel *label_37;
    QDoubleSpinBox *doubleSpinBox_projMaxDepth;
    QLabel *label_38;
    QGroupBox *groupBox_posefiltering;
    QGridLayout *gridLayout_12;
    QDoubleSpinBox *doubleSpinBox_posefilteringAngle;
    QDoubleSpinBox *doubleSpinBox_posefilteringRadius;
    QLabel *label_42;
    QLabel *label_43;
    QSpacerItem *verticalSpacer_4;
    QWidget *page_5;
    QGridLayout *gridLayout_10;
    QDoubleSpinBox *doubleSpinBox_detectMore_radius;
    QLabel *label_29;
    QDoubleSpinBox *doubleSpinBox_detectMore_angle;
    QLabel *label_30;
    QSpinBox *spinBox_detectMore_iterations;
    QLabel *label_31;
    QSpacerItem *verticalSpacer_5;
    QDockWidget *dockWidget_stereoView;
    QWidget *dockWidgetContents_5;
    QHBoxLayout *horizontalLayout_6;
    rtabmap::ImageView *graphicsView_stereo;
    rtabmap::CloudViewer *stereoViewer;
    QDockWidget *dockWidget_view3d;
    QWidget *dockWidgetContents_6;
    QHBoxLayout *horizontalLayout_8;
    rtabmap::CloudViewer *widget_cloudA;
    rtabmap::CloudViewer *widget_cloudB;

    void setupUi(QMainWindow *DatabaseViewer)
    {
        if (DatabaseViewer->objectName().isEmpty())
            DatabaseViewer->setObjectName(QString::fromUtf8("DatabaseViewer"));
        DatabaseViewer->resize(1285, 804);
        DatabaseViewer->setLayoutDirection(Qt::LeftToRight);
        actionOpen_database = new QAction(DatabaseViewer);
        actionOpen_database->setObjectName(QString::fromUtf8("actionOpen_database"));
        actionQuit = new QAction(DatabaseViewer);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionGenerate_graph_dot = new QAction(DatabaseViewer);
        actionGenerate_graph_dot->setObjectName(QString::fromUtf8("actionGenerate_graph_dot"));
        actionGenerate_graph_only_weighted_locations = new QAction(DatabaseViewer);
        actionGenerate_graph_only_weighted_locations->setObjectName(QString::fromUtf8("actionGenerate_graph_only_weighted_locations"));
        actionClean_database = new QAction(DatabaseViewer);
        actionClean_database->setObjectName(QString::fromUtf8("actionClean_database"));
        actionGenerate_local_graph_dot = new QAction(DatabaseViewer);
        actionGenerate_local_graph_dot->setObjectName(QString::fromUtf8("actionGenerate_local_graph_dot"));
        actionClean_local_graph = new QAction(DatabaseViewer);
        actionClean_local_graph->setObjectName(QString::fromUtf8("actionClean_local_graph"));
        actionUpdate_base_ids = new QAction(DatabaseViewer);
        actionUpdate_base_ids->setObjectName(QString::fromUtf8("actionUpdate_base_ids"));
        actionGenerate_3D_map_pcd = new QAction(DatabaseViewer);
        actionGenerate_3D_map_pcd->setObjectName(QString::fromUtf8("actionGenerate_3D_map_pcd"));
        actionExport = new QAction(DatabaseViewer);
        actionExport->setObjectName(QString::fromUtf8("actionExport"));
        actionExtract_images = new QAction(DatabaseViewer);
        actionExtract_images->setObjectName(QString::fromUtf8("actionExtract_images"));
        actionView_3D_map = new QAction(DatabaseViewer);
        actionView_3D_map->setObjectName(QString::fromUtf8("actionView_3D_map"));
        actionGenerate_TORO_graph_graph = new QAction(DatabaseViewer);
        actionGenerate_TORO_graph_graph->setObjectName(QString::fromUtf8("actionGenerate_TORO_graph_graph"));
        actionRefine_all_neighbor_links = new QAction(DatabaseViewer);
        actionRefine_all_neighbor_links->setObjectName(QString::fromUtf8("actionRefine_all_neighbor_links"));
        actionRefine_all_loop_closure_links = new QAction(DatabaseViewer);
        actionRefine_all_loop_closure_links->setObjectName(QString::fromUtf8("actionRefine_all_loop_closure_links"));
        actionDetect_more_loop_closures = new QAction(DatabaseViewer);
        actionDetect_more_loop_closures->setObjectName(QString::fromUtf8("actionDetect_more_loop_closures"));
        actionVisual_Refine_all_neighbor_links = new QAction(DatabaseViewer);
        actionVisual_Refine_all_neighbor_links->setObjectName(QString::fromUtf8("actionVisual_Refine_all_neighbor_links"));
        actionVisual_Refine_all_loop_closure_links = new QAction(DatabaseViewer);
        actionVisual_Refine_all_loop_closure_links->setObjectName(QString::fromUtf8("actionVisual_Refine_all_loop_closure_links"));
        actionSave_config = new QAction(DatabaseViewer);
        actionSave_config->setObjectName(QString::fromUtf8("actionSave_config"));
        actionReset_all_changes = new QAction(DatabaseViewer);
        actionReset_all_changes->setObjectName(QString::fromUtf8("actionReset_all_changes"));
        centralwidget = new QWidget(DatabaseViewer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_7 = new QVBoxLayout(centralwidget);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, 0, -1);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        graphicsView_A = new rtabmap::ImageView(centralwidget);
        graphicsView_A->setObjectName(QString::fromUtf8("graphicsView_A"));

        verticalLayout_6->addWidget(graphicsView_A);

        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 172, 184));
        gridLayout = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_parentsA_2 = new QLabel(scrollAreaWidgetContents_2);
        label_parentsA_2->setObjectName(QString::fromUtf8("label_parentsA_2"));

        gridLayout->addWidget(label_parentsA_2, 0, 0, 1, 1);

        label_parentsA = new QLabel(scrollAreaWidgetContents_2);
        label_parentsA->setObjectName(QString::fromUtf8("label_parentsA"));

        gridLayout->addWidget(label_parentsA, 0, 1, 1, 1);

        label_childrenA_2 = new QLabel(scrollAreaWidgetContents_2);
        label_childrenA_2->setObjectName(QString::fromUtf8("label_childrenA_2"));

        gridLayout->addWidget(label_childrenA_2, 1, 0, 1, 1);

        label_childrenA = new QLabel(scrollAreaWidgetContents_2);
        label_childrenA->setObjectName(QString::fromUtf8("label_childrenA"));

        gridLayout->addWidget(label_childrenA, 1, 1, 1, 1);

        label_childrenA_4 = new QLabel(scrollAreaWidgetContents_2);
        label_childrenA_4->setObjectName(QString::fromUtf8("label_childrenA_4"));

        gridLayout->addWidget(label_childrenA_4, 3, 0, 1, 1);

        label_labelA = new QLabel(scrollAreaWidgetContents_2);
        label_labelA->setObjectName(QString::fromUtf8("label_labelA"));

        gridLayout->addWidget(label_labelA, 3, 1, 1, 1);

        label_childrenA_6 = new QLabel(scrollAreaWidgetContents_2);
        label_childrenA_6->setObjectName(QString::fromUtf8("label_childrenA_6"));

        gridLayout->addWidget(label_childrenA_6, 6, 0, 1, 1);

        label_stampA = new QLabel(scrollAreaWidgetContents_2);
        label_stampA->setObjectName(QString::fromUtf8("label_stampA"));

        gridLayout->addWidget(label_stampA, 6, 1, 1, 1);

        label_childrenA_8 = new QLabel(scrollAreaWidgetContents_2);
        label_childrenA_8->setObjectName(QString::fromUtf8("label_childrenA_8"));

        gridLayout->addWidget(label_childrenA_8, 2, 0, 1, 1);

        label_weightA = new QLabel(scrollAreaWidgetContents_2);
        label_weightA->setObjectName(QString::fromUtf8("label_weightA"));

        gridLayout->addWidget(label_weightA, 2, 1, 1, 1);

        label_childrenA_10 = new QLabel(scrollAreaWidgetContents_2);
        label_childrenA_10->setObjectName(QString::fromUtf8("label_childrenA_10"));

        gridLayout->addWidget(label_childrenA_10, 4, 0, 1, 1);

        label_mapA = new QLabel(scrollAreaWidgetContents_2);
        label_mapA->setObjectName(QString::fromUtf8("label_mapA"));

        gridLayout->addWidget(label_mapA, 4, 1, 1, 1);

        label_childrenA_12 = new QLabel(scrollAreaWidgetContents_2);
        label_childrenA_12->setObjectName(QString::fromUtf8("label_childrenA_12"));

        gridLayout->addWidget(label_childrenA_12, 5, 0, 1, 1);

        label_poseA = new QLabel(scrollAreaWidgetContents_2);
        label_poseA->setObjectName(QString::fromUtf8("label_poseA"));

        gridLayout->addWidget(label_poseA, 5, 1, 1, 1);

        gridLayout->setColumnStretch(1, 1);
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_6->addWidget(scrollArea_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setContentsMargins(12, 12, 12, 12);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_4->addWidget(label_2);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_4->addWidget(label_3);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_indexA = new QLabel(centralwidget);
        label_indexA->setObjectName(QString::fromUtf8("label_indexA"));

        verticalLayout_3->addWidget(label_indexA);

        label_idA = new QLabel(centralwidget);
        label_idA->setObjectName(QString::fromUtf8("label_idA"));

        verticalLayout_3->addWidget(label_idA);


        horizontalLayout->addLayout(verticalLayout_3);

        horizontalSlider_A = new QSlider(centralwidget);
        horizontalSlider_A->setObjectName(QString::fromUtf8("horizontalSlider_A"));
        horizontalSlider_A->setOrientation(Qt::Horizontal);
        horizontalSlider_A->setTickPosition(QSlider::TicksAbove);

        horizontalLayout->addWidget(horizontalSlider_A);


        verticalLayout_6->addLayout(horizontalLayout);

        verticalLayout_6->setStretch(0, 1);

        horizontalLayout_3->addLayout(verticalLayout_6);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        graphicsView_B = new rtabmap::ImageView(centralwidget);
        graphicsView_B->setObjectName(QString::fromUtf8("graphicsView_B"));

        verticalLayout_5->addWidget(graphicsView_B);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 172, 184));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_parentsB = new QLabel(scrollAreaWidgetContents);
        label_parentsB->setObjectName(QString::fromUtf8("label_parentsB"));

        gridLayout_2->addWidget(label_parentsB, 0, 1, 1, 1);

        label_parentsA_4 = new QLabel(scrollAreaWidgetContents);
        label_parentsA_4->setObjectName(QString::fromUtf8("label_parentsA_4"));

        gridLayout_2->addWidget(label_parentsA_4, 0, 0, 1, 1);

        label_childrenA_3 = new QLabel(scrollAreaWidgetContents);
        label_childrenA_3->setObjectName(QString::fromUtf8("label_childrenA_3"));

        gridLayout_2->addWidget(label_childrenA_3, 1, 0, 1, 1);

        label_childrenB = new QLabel(scrollAreaWidgetContents);
        label_childrenB->setObjectName(QString::fromUtf8("label_childrenB"));

        gridLayout_2->addWidget(label_childrenB, 1, 1, 1, 1);

        label_childrenA_5 = new QLabel(scrollAreaWidgetContents);
        label_childrenA_5->setObjectName(QString::fromUtf8("label_childrenA_5"));

        gridLayout_2->addWidget(label_childrenA_5, 3, 0, 1, 1);

        label_labelB = new QLabel(scrollAreaWidgetContents);
        label_labelB->setObjectName(QString::fromUtf8("label_labelB"));

        gridLayout_2->addWidget(label_labelB, 3, 1, 1, 1);

        label_childrenA_7 = new QLabel(scrollAreaWidgetContents);
        label_childrenA_7->setObjectName(QString::fromUtf8("label_childrenA_7"));

        gridLayout_2->addWidget(label_childrenA_7, 6, 0, 1, 1);

        label_stampB = new QLabel(scrollAreaWidgetContents);
        label_stampB->setObjectName(QString::fromUtf8("label_stampB"));

        gridLayout_2->addWidget(label_stampB, 6, 1, 1, 1);

        label_childrenA_9 = new QLabel(scrollAreaWidgetContents);
        label_childrenA_9->setObjectName(QString::fromUtf8("label_childrenA_9"));

        gridLayout_2->addWidget(label_childrenA_9, 2, 0, 1, 1);

        label_weightB = new QLabel(scrollAreaWidgetContents);
        label_weightB->setObjectName(QString::fromUtf8("label_weightB"));

        gridLayout_2->addWidget(label_weightB, 2, 1, 1, 1);

        label_childrenA_11 = new QLabel(scrollAreaWidgetContents);
        label_childrenA_11->setObjectName(QString::fromUtf8("label_childrenA_11"));

        gridLayout_2->addWidget(label_childrenA_11, 4, 0, 1, 1);

        label_mapB = new QLabel(scrollAreaWidgetContents);
        label_mapB->setObjectName(QString::fromUtf8("label_mapB"));

        gridLayout_2->addWidget(label_mapB, 4, 1, 1, 1);

        label_childrenA_13 = new QLabel(scrollAreaWidgetContents);
        label_childrenA_13->setObjectName(QString::fromUtf8("label_childrenA_13"));

        gridLayout_2->addWidget(label_childrenA_13, 5, 0, 1, 1);

        label_poseB = new QLabel(scrollAreaWidgetContents);
        label_poseB->setObjectName(QString::fromUtf8("label_poseB"));

        gridLayout_2->addWidget(label_poseB, 5, 1, 1, 1);

        gridLayout_2->setColumnStretch(1, 1);
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_5->addWidget(scrollArea);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setContentsMargins(12, 12, 12, 12);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_indexB = new QLabel(centralwidget);
        label_indexB->setObjectName(QString::fromUtf8("label_indexB"));

        verticalLayout_2->addWidget(label_indexB);

        label_idB = new QLabel(centralwidget);
        label_idB->setObjectName(QString::fromUtf8("label_idB"));

        verticalLayout_2->addWidget(label_idB);


        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalSlider_B = new QSlider(centralwidget);
        horizontalSlider_B->setObjectName(QString::fromUtf8("horizontalSlider_B"));
        horizontalSlider_B->setOrientation(Qt::Horizontal);
        horizontalSlider_B->setTickPosition(QSlider::TicksAbove);

        horizontalLayout_2->addWidget(horizontalSlider_B);


        verticalLayout_5->addLayout(horizontalLayout_2);

        verticalLayout_5->setStretch(0, 1);

        horizontalLayout_3->addLayout(verticalLayout_5);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_7->addLayout(horizontalLayout_3);

        buttonBox = new QDialogButtonBox(centralwidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout_7->addWidget(buttonBox);

        verticalLayout_7->setStretch(0, 2);
        DatabaseViewer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DatabaseViewer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1285, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        DatabaseViewer->setMenuBar(menubar);
        dockWidget_constraints = new QDockWidget(DatabaseViewer);
        dockWidget_constraints->setObjectName(QString::fromUtf8("dockWidget_constraints"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        constraintsViewer = new rtabmap::CloudViewer(dockWidgetContents);
        constraintsViewer->setObjectName(QString::fromUtf8("constraintsViewer"));

        verticalLayout_8->addWidget(constraintsViewer);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_6->addWidget(label, 0, 0, 1, 1);

        horizontalSlider_neighbors = new QSlider(dockWidgetContents);
        horizontalSlider_neighbors->setObjectName(QString::fromUtf8("horizontalSlider_neighbors"));
        horizontalSlider_neighbors->setOrientation(Qt::Horizontal);
        horizontalSlider_neighbors->setTickPosition(QSlider::TicksAbove);

        gridLayout_6->addWidget(horizontalSlider_neighbors, 0, 1, 1, 1);

        label_6 = new QLabel(dockWidgetContents);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_6->addWidget(label_6, 1, 0, 1, 1);

        horizontalSlider_loops = new QSlider(dockWidgetContents);
        horizontalSlider_loops->setObjectName(QString::fromUtf8("horizontalSlider_loops"));
        horizontalSlider_loops->setOrientation(Qt::Horizontal);
        horizontalSlider_loops->setTickPosition(QSlider::TicksAbove);

        gridLayout_6->addWidget(horizontalSlider_loops, 1, 1, 1, 1);

        label_33 = new QLabel(dockWidgetContents);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_6->addWidget(label_33, 2, 0, 1, 1);

        label_type = new QLabel(dockWidgetContents);
        label_type->setObjectName(QString::fromUtf8("label_type"));

        gridLayout_6->addWidget(label_type, 2, 1, 1, 1);

        label_16 = new QLabel(dockWidgetContents);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_6->addWidget(label_16, 3, 0, 1, 1);

        label_constraint = new QLabel(dockWidgetContents);
        label_constraint->setObjectName(QString::fromUtf8("label_constraint"));

        gridLayout_6->addWidget(label_constraint, 3, 1, 1, 1);

        label_18 = new QLabel(dockWidgetContents);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_6->addWidget(label_18, 4, 0, 1, 1);

        label_variance = new QLabel(dockWidgetContents);
        label_variance->setObjectName(QString::fromUtf8("label_variance"));

        gridLayout_6->addWidget(label_variance, 4, 1, 1, 1);

        checkBox_showOptimized = new QCheckBox(dockWidgetContents);
        checkBox_showOptimized->setObjectName(QString::fromUtf8("checkBox_showOptimized"));

        gridLayout_6->addWidget(checkBox_showOptimized, 5, 0, 1, 1);

        label_constraint_opt = new QLabel(dockWidgetContents);
        label_constraint_opt->setObjectName(QString::fromUtf8("label_constraint_opt"));

        gridLayout_6->addWidget(label_constraint_opt, 5, 1, 1, 1);

        label_32 = new QLabel(dockWidgetContents);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_6->addWidget(label_32, 6, 0, 1, 1);

        checkBox_show3DWords = new QCheckBox(dockWidgetContents);
        checkBox_show3DWords->setObjectName(QString::fromUtf8("checkBox_show3DWords"));

        gridLayout_6->addWidget(checkBox_show3DWords, 6, 1, 1, 1);

        gridLayout_6->setColumnStretch(1, 1);

        verticalLayout_8->addLayout(gridLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        pushButton_refineVisually = new QPushButton(dockWidgetContents);
        pushButton_refineVisually->setObjectName(QString::fromUtf8("pushButton_refineVisually"));

        horizontalLayout_5->addWidget(pushButton_refineVisually);

        pushButton_refine = new QPushButton(dockWidgetContents);
        pushButton_refine->setObjectName(QString::fromUtf8("pushButton_refine"));

        horizontalLayout_5->addWidget(pushButton_refine);

        pushButton_reset = new QPushButton(dockWidgetContents);
        pushButton_reset->setObjectName(QString::fromUtf8("pushButton_reset"));

        horizontalLayout_5->addWidget(pushButton_reset);

        pushButton_add = new QPushButton(dockWidgetContents);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));

        horizontalLayout_5->addWidget(pushButton_add);

        pushButton_reject = new QPushButton(dockWidgetContents);
        pushButton_reject->setObjectName(QString::fromUtf8("pushButton_reject"));

        horizontalLayout_5->addWidget(pushButton_reject);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_8->addLayout(horizontalLayout_5);

        verticalLayout_8->setStretch(0, 1);
        dockWidget_constraints->setWidget(dockWidgetContents);
        DatabaseViewer->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_constraints);
        dockWidget_graphView = new QDockWidget(DatabaseViewer);
        dockWidget_graphView->setObjectName(QString::fromUtf8("dockWidget_graphView"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        graphViewer = new rtabmap::GraphViewer(dockWidgetContents_2);
        graphViewer->setObjectName(QString::fromUtf8("graphViewer"));

        verticalLayout_9->addWidget(graphViewer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_iterations = new QLabel(dockWidgetContents_2);
        label_iterations->setObjectName(QString::fromUtf8("label_iterations"));

        horizontalLayout_4->addWidget(label_iterations);

        horizontalSlider_iterations = new QSlider(dockWidgetContents_2);
        horizontalSlider_iterations->setObjectName(QString::fromUtf8("horizontalSlider_iterations"));
        horizontalSlider_iterations->setOrientation(Qt::Horizontal);
        horizontalSlider_iterations->setTickPosition(QSlider::TicksAbove);

        horizontalLayout_4->addWidget(horizontalSlider_iterations);


        verticalLayout_9->addLayout(horizontalLayout_4);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_loopClosures = new QLabel(dockWidgetContents_2);
        label_loopClosures->setObjectName(QString::fromUtf8("label_loopClosures"));

        gridLayout_5->addWidget(label_loopClosures, 6, 0, 1, 1);

        label_timeGrid = new QLabel(dockWidgetContents_2);
        label_timeGrid->setObjectName(QString::fromUtf8("label_timeGrid"));

        gridLayout_5->addWidget(label_timeGrid, 4, 0, 1, 1);

        label_timeOptimization = new QLabel(dockWidgetContents_2);
        label_timeOptimization->setObjectName(QString::fromUtf8("label_timeOptimization"));

        gridLayout_5->addWidget(label_timeOptimization, 3, 0, 1, 1);

        spinBox_optimizationsFrom = new QSpinBox(dockWidgetContents_2);
        spinBox_optimizationsFrom->setObjectName(QString::fromUtf8("spinBox_optimizationsFrom"));

        gridLayout_5->addWidget(spinBox_optimizationsFrom, 0, 0, 1, 1);

        label_52 = new QLabel(dockWidgetContents_2);
        label_52->setObjectName(QString::fromUtf8("label_52"));

        gridLayout_5->addWidget(label_52, 5, 1, 1, 1);

        label_39 = new QLabel(dockWidgetContents_2);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout_5->addWidget(label_39, 3, 1, 1, 1);

        label_41 = new QLabel(dockWidgetContents_2);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_5->addWidget(label_41, 6, 1, 1, 1);

        label_10 = new QLabel(dockWidgetContents_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_5->addWidget(label_10, 2, 1, 1, 1);

        label_45 = new QLabel(dockWidgetContents_2);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout_5->addWidget(label_45, 4, 1, 1, 1);

        label_pathLength = new QLabel(dockWidgetContents_2);
        label_pathLength->setObjectName(QString::fromUtf8("label_pathLength"));

        gridLayout_5->addWidget(label_pathLength, 2, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_optimizeFrom = new QLabel(dockWidgetContents_2);
        label_optimizeFrom->setObjectName(QString::fromUtf8("label_optimizeFrom"));

        horizontalLayout_7->addWidget(label_optimizeFrom);

        checkBox_spanAllMaps = new QCheckBox(dockWidgetContents_2);
        checkBox_spanAllMaps->setObjectName(QString::fromUtf8("checkBox_spanAllMaps"));
        checkBox_spanAllMaps->setChecked(true);

        horizontalLayout_7->addWidget(checkBox_spanAllMaps);

        horizontalLayout_7->setStretch(1, 1);

        gridLayout_5->addLayout(horizontalLayout_7, 0, 1, 1, 1);

        label_nodes = new QLabel(dockWidgetContents_2);
        label_nodes->setObjectName(QString::fromUtf8("label_nodes"));

        gridLayout_5->addWidget(label_nodes, 5, 0, 1, 1);

        gridLayout_5->setColumnStretch(1, 1);

        verticalLayout_9->addLayout(gridLayout_5);

        dockWidget_graphView->setWidget(dockWidgetContents_2);
        DatabaseViewer->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_graphView);
        dockWidget_parameters = new QDockWidget(DatabaseViewer);
        dockWidget_parameters->setObjectName(QString::fromUtf8("dockWidget_parameters"));
        dockWidget_parameters->setFloating(false);
        dockWidget_parameters->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        toolBox = new QToolBox(dockWidgetContents_3);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 312, 314));
        gridLayout_4 = new QGridLayout(page);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        spinBox_icp_decimation = new QSpinBox(page);
        spinBox_icp_decimation->setObjectName(QString::fromUtf8("spinBox_icp_decimation"));
        spinBox_icp_decimation->setMinimum(1);
        spinBox_icp_decimation->setMaximum(16);
        spinBox_icp_decimation->setValue(8);

        gridLayout_4->addWidget(spinBox_icp_decimation, 0, 0, 1, 1);

        label_14 = new QLabel(page);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_4->addWidget(label_14, 0, 1, 1, 1);

        doubleSpinBox_icp_maxDepth = new QDoubleSpinBox(page);
        doubleSpinBox_icp_maxDepth->setObjectName(QString::fromUtf8("doubleSpinBox_icp_maxDepth"));
        doubleSpinBox_icp_maxDepth->setDecimals(2);
        doubleSpinBox_icp_maxDepth->setMaximum(100);
        doubleSpinBox_icp_maxDepth->setValue(2);

        gridLayout_4->addWidget(doubleSpinBox_icp_maxDepth, 1, 0, 1, 1);

        label_17 = new QLabel(page);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_4->addWidget(label_17, 1, 1, 1, 1);

        doubleSpinBox_icp_voxel = new QDoubleSpinBox(page);
        doubleSpinBox_icp_voxel->setObjectName(QString::fromUtf8("doubleSpinBox_icp_voxel"));
        doubleSpinBox_icp_voxel->setDecimals(3);
        doubleSpinBox_icp_voxel->setMaximum(1);
        doubleSpinBox_icp_voxel->setValue(0.01);

        gridLayout_4->addWidget(doubleSpinBox_icp_voxel, 2, 0, 1, 1);

        label_13 = new QLabel(page);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_4->addWidget(label_13, 2, 1, 1, 1);

        doubleSpinBox_icp_maxCorrespDistance = new QDoubleSpinBox(page);
        doubleSpinBox_icp_maxCorrespDistance->setObjectName(QString::fromUtf8("doubleSpinBox_icp_maxCorrespDistance"));
        doubleSpinBox_icp_maxCorrespDistance->setDecimals(3);
        doubleSpinBox_icp_maxCorrespDistance->setMinimum(0.001);
        doubleSpinBox_icp_maxCorrespDistance->setMaximum(1);
        doubleSpinBox_icp_maxCorrespDistance->setValue(0.05);

        gridLayout_4->addWidget(doubleSpinBox_icp_maxCorrespDistance, 4, 0, 1, 1);

        label_12 = new QLabel(page);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_4->addWidget(label_12, 4, 1, 1, 1);

        doubleSpinBox_icp_minCorrespondenceRatio = new QDoubleSpinBox(page);
        doubleSpinBox_icp_minCorrespondenceRatio->setObjectName(QString::fromUtf8("doubleSpinBox_icp_minCorrespondenceRatio"));
        doubleSpinBox_icp_minCorrespondenceRatio->setDecimals(2);
        doubleSpinBox_icp_minCorrespondenceRatio->setMinimum(0);
        doubleSpinBox_icp_minCorrespondenceRatio->setMaximum(1);
        doubleSpinBox_icp_minCorrespondenceRatio->setValue(0);

        gridLayout_4->addWidget(doubleSpinBox_icp_minCorrespondenceRatio, 5, 0, 1, 1);

        label_46 = new QLabel(page);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        gridLayout_4->addWidget(label_46, 5, 1, 1, 1);

        spinBox_icp_iteration = new QSpinBox(page);
        spinBox_icp_iteration->setObjectName(QString::fromUtf8("spinBox_icp_iteration"));
        spinBox_icp_iteration->setMinimum(1);
        spinBox_icp_iteration->setMaximum(1000);
        spinBox_icp_iteration->setValue(30);

        gridLayout_4->addWidget(spinBox_icp_iteration, 6, 0, 1, 1);

        label_15 = new QLabel(page);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_4->addWidget(label_15, 6, 1, 1, 1);

        checkBox_icp_p2plane = new QCheckBox(page);
        checkBox_icp_p2plane->setObjectName(QString::fromUtf8("checkBox_icp_p2plane"));
        checkBox_icp_p2plane->setChecked(false);

        gridLayout_4->addWidget(checkBox_icp_p2plane, 7, 0, 1, 1);

        label_11 = new QLabel(page);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_4->addWidget(label_11, 7, 1, 1, 1);

        spinBox_icp_normalKSearch = new QSpinBox(page);
        spinBox_icp_normalKSearch->setObjectName(QString::fromUtf8("spinBox_icp_normalKSearch"));
        spinBox_icp_normalKSearch->setMinimum(1);
        spinBox_icp_normalKSearch->setMaximum(1000);
        spinBox_icp_normalKSearch->setValue(20);

        gridLayout_4->addWidget(spinBox_icp_normalKSearch, 8, 0, 1, 1);

        label_20 = new QLabel(page);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_4->addWidget(label_20, 8, 1, 1, 1);

        checkBox_icp_2d = new QCheckBox(page);
        checkBox_icp_2d->setObjectName(QString::fromUtf8("checkBox_icp_2d"));
        checkBox_icp_2d->setChecked(false);

        gridLayout_4->addWidget(checkBox_icp_2d, 9, 0, 1, 1);

        label_27 = new QLabel(page);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_4->addWidget(label_27, 9, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 10, 0, 1, 1);

        gridLayout_4->setColumnStretch(1, 1);
        toolBox->addItem(page, QString::fromUtf8("ICP"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 351, 331));
        verticalLayout_15 = new QVBoxLayout(page_2);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        spinBox_visual_iteration = new QSpinBox(page_2);
        spinBox_visual_iteration->setObjectName(QString::fromUtf8("spinBox_visual_iteration"));
        spinBox_visual_iteration->setMinimum(1);
        spinBox_visual_iteration->setMaximum(1000);
        spinBox_visual_iteration->setValue(30);

        gridLayout_3->addWidget(spinBox_visual_iteration, 4, 0, 1, 1);

        checkBox_visual_2d = new QCheckBox(page_2);
        checkBox_visual_2d->setObjectName(QString::fromUtf8("checkBox_visual_2d"));

        gridLayout_3->addWidget(checkBox_visual_2d, 0, 0, 1, 1);

        doubleSpinBox_visual_maxDepth = new QDoubleSpinBox(page_2);
        doubleSpinBox_visual_maxDepth->setObjectName(QString::fromUtf8("doubleSpinBox_visual_maxDepth"));
        doubleSpinBox_visual_maxDepth->setDecimals(2);
        doubleSpinBox_visual_maxDepth->setMaximum(100);
        doubleSpinBox_visual_maxDepth->setValue(4);

        gridLayout_3->addWidget(doubleSpinBox_visual_maxDepth, 5, 0, 1, 1);

        label_26 = new QLabel(page_2);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_3->addWidget(label_26, 1, 1, 1, 1);

        label_21 = new QLabel(page_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_3->addWidget(label_21, 2, 1, 1, 1);

        label_28 = new QLabel(page_2);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_3->addWidget(label_28, 0, 1, 1, 1);

        doubleSpinBox_visual_maxCorrespDistance = new QDoubleSpinBox(page_2);
        doubleSpinBox_visual_maxCorrespDistance->setObjectName(QString::fromUtf8("doubleSpinBox_visual_maxCorrespDistance"));
        doubleSpinBox_visual_maxCorrespDistance->setDecimals(3);
        doubleSpinBox_visual_maxCorrespDistance->setMinimum(0.001);
        doubleSpinBox_visual_maxCorrespDistance->setMaximum(1);
        doubleSpinBox_visual_maxCorrespDistance->setValue(0.02);

        gridLayout_3->addWidget(doubleSpinBox_visual_maxCorrespDistance, 3, 0, 1, 1);

        doubleSpinBox_visual_nndr = new QDoubleSpinBox(page_2);
        doubleSpinBox_visual_nndr->setObjectName(QString::fromUtf8("doubleSpinBox_visual_nndr"));
        doubleSpinBox_visual_nndr->setDecimals(1);
        doubleSpinBox_visual_nndr->setMinimum(0.1);
        doubleSpinBox_visual_nndr->setMaximum(1);
        doubleSpinBox_visual_nndr->setSingleStep(0.1);
        doubleSpinBox_visual_nndr->setValue(0.6);

        gridLayout_3->addWidget(doubleSpinBox_visual_nndr, 1, 0, 1, 1);

        label_19 = new QLabel(page_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_3->addWidget(label_19, 5, 1, 1, 1);

        spinBox_visual_minCorrespondences = new QSpinBox(page_2);
        spinBox_visual_minCorrespondences->setObjectName(QString::fromUtf8("spinBox_visual_minCorrespondences"));
        spinBox_visual_minCorrespondences->setMinimum(3);
        spinBox_visual_minCorrespondences->setMaximum(1000);
        spinBox_visual_minCorrespondences->setValue(10);

        gridLayout_3->addWidget(spinBox_visual_minCorrespondences, 2, 0, 1, 1);

        label_22 = new QLabel(page_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_3->addWidget(label_22, 3, 1, 1, 1);

        label_23 = new QLabel(page_2);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_3->addWidget(label_23, 4, 1, 1, 1);

        gridLayout_3->setColumnStretch(1, 1);

        verticalLayout_15->addLayout(gridLayout_3);

        groupBox_visual_recomputeFeatures = new QGroupBox(page_2);
        groupBox_visual_recomputeFeatures->setObjectName(QString::fromUtf8("groupBox_visual_recomputeFeatures"));
        groupBox_visual_recomputeFeatures->setCheckable(true);
        gridLayout_11 = new QGridLayout(groupBox_visual_recomputeFeatures);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        label_24 = new QLabel(groupBox_visual_recomputeFeatures);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_11->addWidget(label_24, 0, 1, 1, 1);

        comboBox_featureType = new QComboBox(groupBox_visual_recomputeFeatures);
        comboBox_featureType->setObjectName(QString::fromUtf8("comboBox_featureType"));

        gridLayout_11->addWidget(comboBox_featureType, 0, 0, 1, 1);

        label_25 = new QLabel(groupBox_visual_recomputeFeatures);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_11->addWidget(label_25, 1, 1, 1, 1);

        comboBox_nnType = new QComboBox(groupBox_visual_recomputeFeatures);
        comboBox_nnType->setObjectName(QString::fromUtf8("comboBox_nnType"));

        gridLayout_11->addWidget(comboBox_nnType, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_2, 2, 0, 1, 1);

        gridLayout_11->setColumnStretch(1, 1);

        verticalLayout_15->addWidget(groupBox_visual_recomputeFeatures);

        toolBox->addItem(page_2, QString::fromUtf8("Visual constraint"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setGeometry(QRect(0, -118, 330, 304));
        gridLayout_7 = new QGridLayout(page_3);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        checkBox_2dslam = new QCheckBox(page_3);
        checkBox_2dslam->setObjectName(QString::fromUtf8("checkBox_2dslam"));
        checkBox_2dslam->setChecked(false);

        gridLayout_7->addWidget(checkBox_2dslam, 3, 0, 1, 1);

        label_48 = new QLabel(page_3);
        label_48->setObjectName(QString::fromUtf8("label_48"));

        gridLayout_7->addWidget(label_48, 6, 2, 1, 1);

        checkBox_ignoreUserLoop = new QCheckBox(page_3);
        checkBox_ignoreUserLoop->setObjectName(QString::fromUtf8("checkBox_ignoreUserLoop"));
        checkBox_ignoreUserLoop->setChecked(false);

        gridLayout_7->addWidget(checkBox_ignoreUserLoop, 9, 0, 1, 1);

        label_7 = new QLabel(page_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_7->addWidget(label_7, 1, 2, 1, 1);

        spinBox_iterations = new QSpinBox(page_3);
        spinBox_iterations->setObjectName(QString::fromUtf8("spinBox_iterations"));
        spinBox_iterations->setMinimum(1);
        spinBox_iterations->setMaximum(1000);
        spinBox_iterations->setValue(100);

        gridLayout_7->addWidget(spinBox_iterations, 1, 1, 1, 1);

        spinBox_optimizationDepth = new QSpinBox(page_3);
        spinBox_optimizationDepth->setObjectName(QString::fromUtf8("spinBox_optimizationDepth"));
        spinBox_optimizationDepth->setMinimum(0);
        spinBox_optimizationDepth->setMaximum(1000);
        spinBox_optimizationDepth->setValue(0);

        gridLayout_7->addWidget(spinBox_optimizationDepth, 2, 1, 1, 1);

        label_35 = new QLabel(page_3);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_7->addWidget(label_35, 5, 2, 1, 1);

        checkBox_ignoreGlobalLoop = new QCheckBox(page_3);
        checkBox_ignoreGlobalLoop->setObjectName(QString::fromUtf8("checkBox_ignoreGlobalLoop"));
        checkBox_ignoreGlobalLoop->setChecked(false);

        gridLayout_7->addWidget(checkBox_ignoreGlobalLoop, 6, 0, 1, 1);

        checkBox_ignorePoseCorrection = new QCheckBox(page_3);
        checkBox_ignorePoseCorrection->setObjectName(QString::fromUtf8("checkBox_ignorePoseCorrection"));
        checkBox_ignorePoseCorrection->setChecked(false);

        gridLayout_7->addWidget(checkBox_ignorePoseCorrection, 5, 0, 1, 1);

        checkBox_ignoreLocalLoopTime = new QCheckBox(page_3);
        checkBox_ignoreLocalLoopTime->setObjectName(QString::fromUtf8("checkBox_ignoreLocalLoopTime"));
        checkBox_ignoreLocalLoopTime->setChecked(false);

        gridLayout_7->addWidget(checkBox_ignoreLocalLoopTime, 8, 0, 1, 1);

        comboBox_graphOptimizer = new QComboBox(page_3);
        comboBox_graphOptimizer->setObjectName(QString::fromUtf8("comboBox_graphOptimizer"));
        comboBox_graphOptimizer->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_7->addWidget(comboBox_graphOptimizer, 0, 0, 1, 2);

        label_8 = new QLabel(page_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_7->addWidget(label_8, 2, 2, 1, 1);

        label_50 = new QLabel(page_3);
        label_50->setObjectName(QString::fromUtf8("label_50"));

        gridLayout_7->addWidget(label_50, 9, 2, 1, 1);

        label_47 = new QLabel(page_3);
        label_47->setObjectName(QString::fromUtf8("label_47"));

        gridLayout_7->addWidget(label_47, 7, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_3, 10, 0, 1, 1);

        checkBox_ignoreLocalLoopSpace = new QCheckBox(page_3);
        checkBox_ignoreLocalLoopSpace->setObjectName(QString::fromUtf8("checkBox_ignoreLocalLoopSpace"));
        checkBox_ignoreLocalLoopSpace->setChecked(false);

        gridLayout_7->addWidget(checkBox_ignoreLocalLoopSpace, 7, 0, 1, 1);

        label_49 = new QLabel(page_3);
        label_49->setObjectName(QString::fromUtf8("label_49"));

        gridLayout_7->addWidget(label_49, 8, 2, 1, 1);

        label_40 = new QLabel(page_3);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        gridLayout_7->addWidget(label_40, 0, 2, 1, 1);

        checkBox_ignoreCovariance = new QCheckBox(page_3);
        checkBox_ignoreCovariance->setObjectName(QString::fromUtf8("checkBox_ignoreCovariance"));
        checkBox_ignoreCovariance->setChecked(false);

        gridLayout_7->addWidget(checkBox_ignoreCovariance, 4, 0, 1, 1);

        label_51 = new QLabel(page_3);
        label_51->setObjectName(QString::fromUtf8("label_51"));

        gridLayout_7->addWidget(label_51, 3, 2, 1, 1);

        label_34 = new QLabel(page_3);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_7->addWidget(label_34, 4, 2, 1, 1);

        toolBox->addItem(page_3, QString::fromUtf8("Graph optimization"));
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        page_4->setGeometry(QRect(0, 0, 248, 319));
        verticalLayout_16 = new QVBoxLayout(page_4);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        doubleSpinBox_gridCellSize = new QDoubleSpinBox(page_4);
        doubleSpinBox_gridCellSize->setObjectName(QString::fromUtf8("doubleSpinBox_gridCellSize"));
        doubleSpinBox_gridCellSize->setMinimum(0.01);
        doubleSpinBox_gridCellSize->setMaximum(5);
        doubleSpinBox_gridCellSize->setSingleStep(0.01);
        doubleSpinBox_gridCellSize->setValue(0.05);

        gridLayout_9->addWidget(doubleSpinBox_gridCellSize, 0, 0, 1, 1);

        label_36 = new QLabel(page_4);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_9->addWidget(label_36, 0, 1, 1, 1);

        label_44 = new QLabel(page_4);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        gridLayout_9->addWidget(label_44, 1, 1, 1, 1);

        checkBox_gridErode = new QCheckBox(page_4);
        checkBox_gridErode->setObjectName(QString::fromUtf8("checkBox_gridErode"));

        gridLayout_9->addWidget(checkBox_gridErode, 1, 0, 1, 1);

        gridLayout_9->setColumnStretch(1, 1);

        verticalLayout_16->addLayout(gridLayout_9);

        groupBox_gridFromProjection = new QGroupBox(page_4);
        groupBox_gridFromProjection->setObjectName(QString::fromUtf8("groupBox_gridFromProjection"));
        groupBox_gridFromProjection->setCheckable(true);
        groupBox_gridFromProjection->setChecked(false);
        gridLayout_8 = new QGridLayout(groupBox_gridFromProjection);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        spinBox_projDecimation = new QSpinBox(groupBox_gridFromProjection);
        spinBox_projDecimation->setObjectName(QString::fromUtf8("spinBox_projDecimation"));
        spinBox_projDecimation->setMinimum(1);
        spinBox_projDecimation->setMaximum(8);
        spinBox_projDecimation->setValue(4);

        gridLayout_8->addWidget(spinBox_projDecimation, 0, 0, 1, 1);

        label_37 = new QLabel(groupBox_gridFromProjection);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_8->addWidget(label_37, 0, 1, 1, 1);

        doubleSpinBox_projMaxDepth = new QDoubleSpinBox(groupBox_gridFromProjection);
        doubleSpinBox_projMaxDepth->setObjectName(QString::fromUtf8("doubleSpinBox_projMaxDepth"));
        doubleSpinBox_projMaxDepth->setDecimals(1);
        doubleSpinBox_projMaxDepth->setMinimum(0.4);
        doubleSpinBox_projMaxDepth->setMaximum(15);
        doubleSpinBox_projMaxDepth->setSingleStep(1);
        doubleSpinBox_projMaxDepth->setValue(4);

        gridLayout_8->addWidget(doubleSpinBox_projMaxDepth, 1, 0, 1, 1);

        label_38 = new QLabel(groupBox_gridFromProjection);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_8->addWidget(label_38, 1, 1, 1, 1);

        gridLayout_8->setColumnStretch(1, 1);

        verticalLayout_16->addWidget(groupBox_gridFromProjection);

        groupBox_posefiltering = new QGroupBox(page_4);
        groupBox_posefiltering->setObjectName(QString::fromUtf8("groupBox_posefiltering"));
        groupBox_posefiltering->setCheckable(true);
        groupBox_posefiltering->setChecked(false);
        gridLayout_12 = new QGridLayout(groupBox_posefiltering);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        doubleSpinBox_posefilteringAngle = new QDoubleSpinBox(groupBox_posefiltering);
        doubleSpinBox_posefilteringAngle->setObjectName(QString::fromUtf8("doubleSpinBox_posefilteringAngle"));
        doubleSpinBox_posefilteringAngle->setDecimals(0);
        doubleSpinBox_posefilteringAngle->setMaximum(180);
        doubleSpinBox_posefilteringAngle->setValue(30);

        gridLayout_12->addWidget(doubleSpinBox_posefilteringAngle, 4, 0, 1, 1);

        doubleSpinBox_posefilteringRadius = new QDoubleSpinBox(groupBox_posefiltering);
        doubleSpinBox_posefilteringRadius->setObjectName(QString::fromUtf8("doubleSpinBox_posefilteringRadius"));
        doubleSpinBox_posefilteringRadius->setMinimum(0.01);
        doubleSpinBox_posefilteringRadius->setValue(0.1);

        gridLayout_12->addWidget(doubleSpinBox_posefilteringRadius, 3, 0, 1, 1);

        label_42 = new QLabel(groupBox_posefiltering);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        gridLayout_12->addWidget(label_42, 3, 1, 1, 1);

        label_43 = new QLabel(groupBox_posefiltering);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        gridLayout_12->addWidget(label_43, 4, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_4, 5, 0, 1, 1);

        gridLayout_12->setColumnStretch(1, 1);

        verticalLayout_16->addWidget(groupBox_posefiltering);

        toolBox->addItem(page_4, QString::fromUtf8("Grid/cloud generation"));
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        page_5->setGeometry(QRect(0, 0, 201, 126));
        gridLayout_10 = new QGridLayout(page_5);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        doubleSpinBox_detectMore_radius = new QDoubleSpinBox(page_5);
        doubleSpinBox_detectMore_radius->setObjectName(QString::fromUtf8("doubleSpinBox_detectMore_radius"));
        doubleSpinBox_detectMore_radius->setDecimals(2);
        doubleSpinBox_detectMore_radius->setMaximum(10);
        doubleSpinBox_detectMore_radius->setSingleStep(0.1);
        doubleSpinBox_detectMore_radius->setValue(0.3);

        gridLayout_10->addWidget(doubleSpinBox_detectMore_radius, 0, 0, 1, 1);

        label_29 = new QLabel(page_5);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_10->addWidget(label_29, 0, 1, 1, 1);

        doubleSpinBox_detectMore_angle = new QDoubleSpinBox(page_5);
        doubleSpinBox_detectMore_angle->setObjectName(QString::fromUtf8("doubleSpinBox_detectMore_angle"));
        doubleSpinBox_detectMore_angle->setDecimals(0);
        doubleSpinBox_detectMore_angle->setMaximum(180);
        doubleSpinBox_detectMore_angle->setSingleStep(1);
        doubleSpinBox_detectMore_angle->setValue(30);

        gridLayout_10->addWidget(doubleSpinBox_detectMore_angle, 1, 0, 1, 1);

        label_30 = new QLabel(page_5);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_10->addWidget(label_30, 1, 1, 1, 1);

        spinBox_detectMore_iterations = new QSpinBox(page_5);
        spinBox_detectMore_iterations->setObjectName(QString::fromUtf8("spinBox_detectMore_iterations"));
        spinBox_detectMore_iterations->setMinimum(1);
        spinBox_detectMore_iterations->setMaximum(100);
        spinBox_detectMore_iterations->setValue(1);

        gridLayout_10->addWidget(spinBox_detectMore_iterations, 2, 0, 1, 1);

        label_31 = new QLabel(page_5);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_10->addWidget(label_31, 2, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_10->addItem(verticalSpacer_5, 3, 0, 1, 1);

        gridLayout_10->setColumnStretch(1, 1);
        toolBox->addItem(page_5, QString::fromUtf8("Detect more loop closures"));

        verticalLayout_10->addWidget(toolBox);

        dockWidget_parameters->setWidget(dockWidgetContents_3);
        DatabaseViewer->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_parameters);
        dockWidget_stereoView = new QDockWidget(DatabaseViewer);
        dockWidget_stereoView->setObjectName(QString::fromUtf8("dockWidget_stereoView"));
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        horizontalLayout_6 = new QHBoxLayout(dockWidgetContents_5);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        graphicsView_stereo = new rtabmap::ImageView(dockWidgetContents_5);
        graphicsView_stereo->setObjectName(QString::fromUtf8("graphicsView_stereo"));

        horizontalLayout_6->addWidget(graphicsView_stereo);

        stereoViewer = new rtabmap::CloudViewer(dockWidgetContents_5);
        stereoViewer->setObjectName(QString::fromUtf8("stereoViewer"));

        horizontalLayout_6->addWidget(stereoViewer);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 1);
        dockWidget_stereoView->setWidget(dockWidgetContents_5);
        DatabaseViewer->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_stereoView);
        dockWidget_view3d = new QDockWidget(DatabaseViewer);
        dockWidget_view3d->setObjectName(QString::fromUtf8("dockWidget_view3d"));
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QString::fromUtf8("dockWidgetContents_6"));
        horizontalLayout_8 = new QHBoxLayout(dockWidgetContents_6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        widget_cloudA = new rtabmap::CloudViewer(dockWidgetContents_6);
        widget_cloudA->setObjectName(QString::fromUtf8("widget_cloudA"));

        horizontalLayout_8->addWidget(widget_cloudA);

        widget_cloudB = new rtabmap::CloudViewer(dockWidgetContents_6);
        widget_cloudB->setObjectName(QString::fromUtf8("widget_cloudB"));

        horizontalLayout_8->addWidget(widget_cloudB);

        horizontalLayout_8->setStretch(0, 1);
        horizontalLayout_8->setStretch(1, 1);
        dockWidget_view3d->setWidget(dockWidgetContents_6);
        DatabaseViewer->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_view3d);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menuFile->addAction(actionOpen_database);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_config);
        menuFile->addSeparator();
        menuFile->addAction(actionExport);
        menuFile->addAction(actionExtract_images);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuEdit->addAction(actionGenerate_graph_dot);
        menuEdit->addAction(actionGenerate_local_graph_dot);
        menuEdit->addAction(actionGenerate_TORO_graph_graph);
        menuEdit->addSeparator();
        menuEdit->addAction(actionDetect_more_loop_closures);
        menuEdit->addAction(actionVisual_Refine_all_neighbor_links);
        menuEdit->addAction(actionRefine_all_neighbor_links);
        menuEdit->addAction(actionVisual_Refine_all_loop_closure_links);
        menuEdit->addAction(actionRefine_all_loop_closure_links);
        menuEdit->addSeparator();
        menuEdit->addAction(actionReset_all_changes);
        menuEdit->addSeparator();
        menuEdit->addAction(actionView_3D_map);
        menuEdit->addAction(actionGenerate_3D_map_pcd);

        retranslateUi(DatabaseViewer);

        toolBox->setCurrentIndex(2);
        comboBox_featureType->setCurrentIndex(0);
        comboBox_nnType->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(DatabaseViewer);
    } // setupUi

    void retranslateUi(QMainWindow *DatabaseViewer)
    {
        DatabaseViewer->setWindowTitle(QApplication::translate("DatabaseViewer", "Database viewer", 0, QApplication::UnicodeUTF8));
        actionOpen_database->setText(QApplication::translate("DatabaseViewer", "Open database", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("DatabaseViewer", "Quit", 0, QApplication::UnicodeUTF8));
        actionGenerate_graph_dot->setText(QApplication::translate("DatabaseViewer", "Generate graph (*.dot) ...", 0, QApplication::UnicodeUTF8));
        actionGenerate_graph_only_weighted_locations->setText(QApplication::translate("DatabaseViewer", "Generate graph (only weighted locations) ...", 0, QApplication::UnicodeUTF8));
        actionClean_database->setText(QApplication::translate("DatabaseViewer", "Clean database", 0, QApplication::UnicodeUTF8));
        actionGenerate_local_graph_dot->setText(QApplication::translate("DatabaseViewer", "Generate local graph (.dot) ...", 0, QApplication::UnicodeUTF8));
        actionClean_local_graph->setText(QApplication::translate("DatabaseViewer", "Clean local graph ...", 0, QApplication::UnicodeUTF8));
        actionUpdate_base_ids->setText(QApplication::translate("DatabaseViewer", "Update base ids", 0, QApplication::UnicodeUTF8));
        actionGenerate_3D_map_pcd->setText(QApplication::translate("DatabaseViewer", "Export 3D map (*.pcd) ...", 0, QApplication::UnicodeUTF8));
        actionExport->setText(QApplication::translate("DatabaseViewer", "Export...", 0, QApplication::UnicodeUTF8));
        actionExtract_images->setText(QApplication::translate("DatabaseViewer", "Extract images...", 0, QApplication::UnicodeUTF8));
        actionView_3D_map->setText(QApplication::translate("DatabaseViewer", "View 3D map...", 0, QApplication::UnicodeUTF8));
        actionGenerate_TORO_graph_graph->setText(QApplication::translate("DatabaseViewer", "Generate TORO graph (*.graph)...", 0, QApplication::UnicodeUTF8));
        actionRefine_all_neighbor_links->setText(QApplication::translate("DatabaseViewer", "ICP: Refine all neighbor links...", 0, QApplication::UnicodeUTF8));
        actionRefine_all_loop_closure_links->setText(QApplication::translate("DatabaseViewer", "ICP: Refine all loop closure links...", 0, QApplication::UnicodeUTF8));
        actionDetect_more_loop_closures->setText(QApplication::translate("DatabaseViewer", "Detect more loop closures...", 0, QApplication::UnicodeUTF8));
        actionVisual_Refine_all_neighbor_links->setText(QApplication::translate("DatabaseViewer", "Visual: Refine all neighbor links...", 0, QApplication::UnicodeUTF8));
        actionVisual_Refine_all_loop_closure_links->setText(QApplication::translate("DatabaseViewer", "Visual: Refine all loop closure links...", 0, QApplication::UnicodeUTF8));
        actionSave_config->setText(QApplication::translate("DatabaseViewer", "Save config", 0, QApplication::UnicodeUTF8));
        actionReset_all_changes->setText(QApplication::translate("DatabaseViewer", "Reset all changes", 0, QApplication::UnicodeUTF8));
        label_parentsA_2->setText(QApplication::translate("DatabaseViewer", "Parents", 0, QApplication::UnicodeUTF8));
        label_parentsA->setText(QString());
        label_childrenA_2->setText(QApplication::translate("DatabaseViewer", "Children", 0, QApplication::UnicodeUTF8));
        label_childrenA->setText(QString());
        label_childrenA_4->setText(QApplication::translate("DatabaseViewer", "Label", 0, QApplication::UnicodeUTF8));
        label_labelA->setText(QString());
        label_childrenA_6->setText(QApplication::translate("DatabaseViewer", "Stamp", 0, QApplication::UnicodeUTF8));
        label_stampA->setText(QString());
        label_childrenA_8->setText(QApplication::translate("DatabaseViewer", "Weight", 0, QApplication::UnicodeUTF8));
        label_weightA->setText(QString());
        label_childrenA_10->setText(QApplication::translate("DatabaseViewer", "Map ID", 0, QApplication::UnicodeUTF8));
        label_mapA->setText(QString());
        label_childrenA_12->setText(QApplication::translate("DatabaseViewer", "Pose", 0, QApplication::UnicodeUTF8));
        label_poseA->setText(QString());
        label_2->setText(QApplication::translate("DatabaseViewer", "Index :", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DatabaseViewer", "Id :", 0, QApplication::UnicodeUTF8));
        label_indexA->setText(QApplication::translate("DatabaseViewer", "indexA", 0, QApplication::UnicodeUTF8));
        label_idA->setText(QApplication::translate("DatabaseViewer", "idA", 0, QApplication::UnicodeUTF8));
        label_parentsB->setText(QString());
        label_parentsA_4->setText(QApplication::translate("DatabaseViewer", "Parents", 0, QApplication::UnicodeUTF8));
        label_childrenA_3->setText(QApplication::translate("DatabaseViewer", "Children", 0, QApplication::UnicodeUTF8));
        label_childrenB->setText(QString());
        label_childrenA_5->setText(QApplication::translate("DatabaseViewer", "Label", 0, QApplication::UnicodeUTF8));
        label_labelB->setText(QString());
        label_childrenA_7->setText(QApplication::translate("DatabaseViewer", "Stamp", 0, QApplication::UnicodeUTF8));
        label_stampB->setText(QString());
        label_childrenA_9->setText(QApplication::translate("DatabaseViewer", "Weight", 0, QApplication::UnicodeUTF8));
        label_weightB->setText(QString());
        label_childrenA_11->setText(QApplication::translate("DatabaseViewer", "Map ID", 0, QApplication::UnicodeUTF8));
        label_mapB->setText(QString());
        label_childrenA_13->setText(QApplication::translate("DatabaseViewer", "Pose", 0, QApplication::UnicodeUTF8));
        label_poseB->setText(QString());
        label_5->setText(QApplication::translate("DatabaseViewer", "Index :", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DatabaseViewer", "Id :", 0, QApplication::UnicodeUTF8));
        label_indexB->setText(QApplication::translate("DatabaseViewer", "indexB", 0, QApplication::UnicodeUTF8));
        label_idB->setText(QApplication::translate("DatabaseViewer", "idB", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("DatabaseViewer", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("DatabaseViewer", "Edit", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("DatabaseViewer", "View", 0, QApplication::UnicodeUTF8));
        dockWidget_constraints->setWindowTitle(QApplication::translate("DatabaseViewer", "Constraints view", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DatabaseViewer", "Neighbor links", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DatabaseViewer", "Loop closure links", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("DatabaseViewer", "Type", 0, QApplication::UnicodeUTF8));
        label_type->setText(QString());
        label_16->setText(QApplication::translate("DatabaseViewer", "Transform", 0, QApplication::UnicodeUTF8));
        label_constraint->setText(QString());
        label_18->setText(QApplication::translate("DatabaseViewer", "\317\203 (rot, trans)", 0, QApplication::UnicodeUTF8));
        label_variance->setText(QString());
        checkBox_showOptimized->setText(QApplication::translate("DatabaseViewer", "Optimized", 0, QApplication::UnicodeUTF8));
        label_constraint_opt->setText(QString());
        label_32->setText(QApplication::translate("DatabaseViewer", "Show 3D words", 0, QApplication::UnicodeUTF8));
        checkBox_show3DWords->setText(QString());
        pushButton_refineVisually->setText(QApplication::translate("DatabaseViewer", "Refine (Visual)", 0, QApplication::UnicodeUTF8));
        pushButton_refine->setText(QApplication::translate("DatabaseViewer", "Refine (ICP)", 0, QApplication::UnicodeUTF8));
        pushButton_reset->setText(QApplication::translate("DatabaseViewer", "Reset", 0, QApplication::UnicodeUTF8));
        pushButton_add->setText(QApplication::translate("DatabaseViewer", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_reject->setText(QApplication::translate("DatabaseViewer", "Reject", 0, QApplication::UnicodeUTF8));
        dockWidget_graphView->setWindowTitle(QApplication::translate("DatabaseViewer", "Graph view", 0, QApplication::UnicodeUTF8));
        label_iterations->setText(QApplication::translate("DatabaseViewer", "#", 0, QApplication::UnicodeUTF8));
        label_loopClosures->setText(QString());
        label_timeGrid->setText(QString());
        label_timeOptimization->setText(QString());
        label_52->setText(QApplication::translate("DatabaseViewer", "Nodes", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("DatabaseViewer", "Time optimization (s)", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("DatabaseViewer", "Loop closures (G, LS, LT, U)", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DatabaseViewer", "Total path length (m)", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("DatabaseViewer", "Time grid (s)", 0, QApplication::UnicodeUTF8));
        label_pathLength->setText(QString());
        label_optimizeFrom->setText(QApplication::translate("DatabaseViewer", "Graph root", 0, QApplication::UnicodeUTF8));
        checkBox_spanAllMaps->setText(QApplication::translate("DatabaseViewer", "Span to all maps", 0, QApplication::UnicodeUTF8));
        label_nodes->setText(QString());
        dockWidget_parameters->setWindowTitle(QApplication::translate("DatabaseViewer", "Parameters", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("DatabaseViewer", "Decimation", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_icp_maxDepth->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("DatabaseViewer", "Max depth", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_icp_voxel->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("DatabaseViewer", "Voxel", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_icp_maxCorrespDistance->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("DatabaseViewer", "Max correspondence distance", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_icp_minCorrespondenceRatio->setSuffix(QString());
        label_46->setText(QApplication::translate("DatabaseViewer", "Min correspondence ratio", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("DatabaseViewer", "Iteration", 0, QApplication::UnicodeUTF8));
        checkBox_icp_p2plane->setText(QString());
        label_11->setText(QApplication::translate("DatabaseViewer", "Point to plane", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("DatabaseViewer", "Normal K neighbors", 0, QApplication::UnicodeUTF8));
        checkBox_icp_2d->setText(QString());
        label_27->setText(QApplication::translate("DatabaseViewer", "2D icp (laser scans required)", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("DatabaseViewer", "ICP", 0, QApplication::UnicodeUTF8));
        checkBox_visual_2d->setText(QString());
        doubleSpinBox_visual_maxDepth->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("DatabaseViewer", "NNDR", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("DatabaseViewer", "Min correspondences", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("DatabaseViewer", "2D transform (x,y,yaw)", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_visual_maxCorrespDistance->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_visual_nndr->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("DatabaseViewer", "Max feature depth", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("DatabaseViewer", "Max correspondence distance", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("DatabaseViewer", "Iteration", 0, QApplication::UnicodeUTF8));
        groupBox_visual_recomputeFeatures->setTitle(QApplication::translate("DatabaseViewer", "Re-extract features", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("DatabaseViewer", "Feature type", 0, QApplication::UnicodeUTF8));
        comboBox_featureType->clear();
        comboBox_featureType->insertItems(0, QStringList()
         << QApplication::translate("DatabaseViewer", "SURF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "SIFT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "ORB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "FAST+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "FAST+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "GFTT+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "GFTT+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "BRISK", 0, QApplication::UnicodeUTF8)
        );
        label_25->setText(QApplication::translate("DatabaseViewer", "Nearest neighbor type", 0, QApplication::UnicodeUTF8));
        comboBox_nnType->clear();
        comboBox_nnType->insertItems(0, QStringList()
         << QApplication::translate("DatabaseViewer", "FLANN Naive", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "FLANN Kd-Tree", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "FLANN LSH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "Brute force", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "Brute force GPU", 0, QApplication::UnicodeUTF8)
        );
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("DatabaseViewer", "Visual constraint", 0, QApplication::UnicodeUTF8));
        checkBox_2dslam->setText(QString());
        label_48->setText(QApplication::translate("DatabaseViewer", "Ignore global loop closures", 0, QApplication::UnicodeUTF8));
        checkBox_ignoreUserLoop->setText(QString());
        label_7->setText(QApplication::translate("DatabaseViewer", "Iterations", 0, QApplication::UnicodeUTF8));
        spinBox_iterations->setSuffix(QString());
        spinBox_optimizationDepth->setSuffix(QString());
        label_35->setText(QApplication::translate("DatabaseViewer", "Ignore pose correction", 0, QApplication::UnicodeUTF8));
        checkBox_ignoreGlobalLoop->setText(QString());
        checkBox_ignorePoseCorrection->setText(QString());
        checkBox_ignoreLocalLoopTime->setText(QString());
        comboBox_graphOptimizer->clear();
        comboBox_graphOptimizer->insertItems(0, QStringList()
         << QApplication::translate("DatabaseViewer", "TORO", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DatabaseViewer", "g2o", 0, QApplication::UnicodeUTF8)
        );
        label_8->setText(QApplication::translate("DatabaseViewer", "Depth (0=inf)", 0, QApplication::UnicodeUTF8));
        label_50->setText(QApplication::translate("DatabaseViewer", "Ignore user loop closures", 0, QApplication::UnicodeUTF8));
        label_47->setText(QApplication::translate("DatabaseViewer", "Ignore local loop closures (space)", 0, QApplication::UnicodeUTF8));
        checkBox_ignoreLocalLoopSpace->setText(QString());
        label_49->setText(QApplication::translate("DatabaseViewer", "Ignore local loop closures (time)", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("DatabaseViewer", "Strategy", 0, QApplication::UnicodeUTF8));
        checkBox_ignoreCovariance->setText(QString());
        label_51->setText(QApplication::translate("DatabaseViewer", "2d SLAM", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("DatabaseViewer", "Ignore covariance", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("DatabaseViewer", "Graph optimization", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_gridCellSize->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("DatabaseViewer", "Grid cell size", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("DatabaseViewer", "Errode", 0, QApplication::UnicodeUTF8));
        checkBox_gridErode->setText(QString());
        groupBox_gridFromProjection->setTitle(QApplication::translate("DatabaseViewer", "Grid from 3D projection", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("DatabaseViewer", "Cloud decimation", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_projMaxDepth->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("DatabaseViewer", "Cloud max depth", 0, QApplication::UnicodeUTF8));
        groupBox_posefiltering->setTitle(QApplication::translate("DatabaseViewer", "Pose filtering", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_posefilteringAngle->setSuffix(QApplication::translate("DatabaseViewer", " degrees", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_posefilteringRadius->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("DatabaseViewer", "Radius", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("DatabaseViewer", "Angle", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_4), QApplication::translate("DatabaseViewer", "Grid/cloud generation", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_detectMore_radius->setSuffix(QApplication::translate("DatabaseViewer", " m", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("DatabaseViewer", "Radius", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_detectMore_angle->setSuffix(QApplication::translate("DatabaseViewer", " degrees", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("DatabaseViewer", "Angle", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("DatabaseViewer", "Iterations", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_5), QApplication::translate("DatabaseViewer", "Detect more loop closures", 0, QApplication::UnicodeUTF8));
        dockWidget_stereoView->setWindowTitle(QApplication::translate("DatabaseViewer", "Stereo view", 0, QApplication::UnicodeUTF8));
        dockWidget_view3d->setWindowTitle(QApplication::translate("DatabaseViewer", "3D view", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DatabaseViewer: public Ui_DatabaseViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATABASEVIEWER_H
