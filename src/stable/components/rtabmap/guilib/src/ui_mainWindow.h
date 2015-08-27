/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "../include/rtabmap/gui/CloudViewer.h"
#include "../include/rtabmap/gui/ImageView.h"
#include "../include/rtabmap/gui/LoopClosureViewer.h"
#include "ConsoleWidget.h"
#include "GraphViewer.h"
#include "MapVisibilityWidget.h"
#include "StatsToolBox.h"
#include "utilite/UPlot.h"

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QAction *actionExit;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionPreferences;
    QAction *actionStart;
    QAction *actionPause;
    QAction *actionPause_on_match;
    QAction *actionStop;
    QAction *actionDump_the_memory;
    QAction *actionPause_when_a_loop_hypothesis_is_rejected;
    QAction *actionClear_cache;
    QAction *actionAuto_screen_capture;
    QAction *actionDump_the_prediction_matrix;
    QAction *actionGenerate_map;
    QAction *actionDelete_memory;
    QAction *actionOpen_working_directory;
    QAction *action16_9;
    QAction *action16_10;
    QAction *action4_3;
    QAction *action360p;
    QAction *action480p;
    QAction *action720p;
    QAction *action1080p;
    QAction *action240p;
    QAction *actionUsbCamera;
    QAction *actionImageFiles;
    QAction *actionVideo;
    QAction *actionDatabase;
    QAction *actionJdeRobotInterfaces;
    QAction *actionGenerate_local_map;
    QAction *actionPrint_loop_closure_IDs_to_console;
    QAction *actionSave_point_cloud;
    QAction *actionDownload_all_clouds;
    QAction *actionPause_on_local_loop_detection;
    QAction *actionStore_keyframe_information;
    QAction *actionSLAM_mode;
    QAction *actionLocalization_mode;
    QAction *actionReset_Odometry;
    QAction *actionView_high_res_point_cloud;
    QAction *actionTrigger_a_new_map;
    QAction *actionGenerate_TORO_graph_graph;
    QAction *actionDownload_graph;
    QAction *actionScreenshot;
    QAction *actionOpenNI_PCL;
    QAction *actionFreenect;
    QAction *actionOpenNI_CV;
    QAction *actionOpenNI_CV_ASUS;
    QAction *actionOpenNI2;
    QAction *actionOpenNI_PCL_ASUS;
    QAction *actionData_recorder;
    QAction *actionExport_2D_Grid_map_bmp_png;
    QAction *actionExport_2D_scans_ply_pcd;
    QAction *actionView_scans;
    QAction *actionOpen_database;
    QAction *actionNew_database;
    QAction *actionClose_database;
    QAction *actionEdit_database;
    QAction *actionPost_processing;
    QAction *actionSave_GUI_config;
    QAction *actionFreenect2;
    QAction *actionStereoDC1394;
    QAction *actionOpenNI2_kinect;
    QAction *actionOpenNI2_sense;
    QAction *actionStereoFlyCapture2;
    QAction *actionSend_goal;
    QWidget *widget_mainWindow;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuAdvanced;
    QMenu *menu6;
    QMenu *menuProcess;
    QMenu *menuSelect_source;
    QMenu *menuImage;
    QMenu *menuRGB_D_camera;
    QMenu *menuKinect_for_Xbox_360;
    QMenu *menuXtion_PRO_LIVE;
    QMenu *menuSense_3D_scanner;
    QMenu *menuKinect_v2;
    QMenu *menuBumblebee2;
    QMenu *menuWindow;
    QMenu *menuShow_view;
    QMenu *menuAspect_ratio_2;
    QMenu *menuTools;
    QStatusBar *statusbar;
    QDockWidget *dockWidget_posterior;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    UPlot *posteriorPlot;
    QToolBar *toolBar;
    QDockWidget *dockWidget_statsV2;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_8;
    QGridLayout *gridLayout;
    QDoubleSpinBox *doubleSpinBox_stats_timeLimit;
    QLabel *label_24;
    QLabel *label_stats_source;
    QDoubleSpinBox *doubleSpinBox_stats_imgRate;
    QLabel *label_45;
    QLabel *label_elapsedTime;
    QLabel *label_20;
    QLabel *label_stats_imageNumber;
    QLabel *label_12;
    QLabel *label_stats_loopClosuresDetected;
    QLabel *label_38;
    QLabel *label_stats_loopClosuresReactivatedDetected;
    QLabel *label_15;
    QLabel *label_stats_loopClosuresRejected;
    QLabel *doubleSpinBox_stats_imgRate_label;
    QLabel *label_timeLimit;
    QDoubleSpinBox *doubleSpinBox_stats_detectionRate;
    QLabel *doubleSpinBox_stats_imgRate_label_2;
    rtabmap::StatsToolBox *statsToolBox;
    QSpacerItem *verticalSpacer_2;
    QDockWidget *dockWidget_likelihood;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_6;
    UPlot *likelihoodPlot;
    QDockWidget *dockWidget_console;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_5;
    rtabmap::ConsoleWidget *widget_console;
    QDockWidget *dockWidget_rawlikelihood;
    QWidget *dockWidgetContents_6;
    QVBoxLayout *verticalLayout_4;
    UPlot *rawLikelihoodPlot;
    QDockWidget *dockWidget_cloudViewer;
    QWidget *dockWidgetContents_5;
    QVBoxLayout *verticalLayout_3;
    rtabmap::CloudViewer *widget_cloudViewer;
    QDockWidget *dockWidget_loopClosureViewer;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_7;
    rtabmap::LoopClosureViewer *widget_loopClosureViewer;
    QDockWidget *dockWidget_mapVisibility;
    QWidget *dockWidgetContents_8;
    QVBoxLayout *verticalLayout_9;
    rtabmap::MapVisibilityWidget *widget_mapVisibility;
    QDockWidget *dockWidget_graphViewer;
    QWidget *dockWidgetContents_9;
    QVBoxLayout *verticalLayout_10;
    rtabmap::GraphViewer *graphicsView_graphView;
    QDockWidget *dockWidget_imageView;
    QWidget *layout_imageview;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_refId;
    rtabmap::ImageView *imageView_source;
    QVBoxLayout *verticalLayout;
    QLabel *label_matchId;
    rtabmap::ImageView *imageView_loopClosure;
    QDockWidget *dockWidget_odometry;
    QWidget *dockWidgetContents_10;
    QVBoxLayout *verticalLayout_11;
    rtabmap::ImageView *imageView_odometry;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->resize(1012, 711);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/RTAB-Map.ico"), QSize(), QIcon::Normal, QIcon::Off);
        mainWindow->setWindowIcon(icon);
        mainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        actionExit = new QAction(mainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/system-log-out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon1);
        actionHelp = new QAction(mainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionHelp->setEnabled(false);
        actionAbout = new QAction(mainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionPreferences = new QAction(mainWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        actionStart = new QAction(mainWindow);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/Play1Normal.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon2);
        actionPause = new QAction(mainWindow);
        actionPause->setObjectName(QString::fromUtf8("actionPause"));
        actionPause->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PauseNormalRed.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon3);
        actionPause_on_match = new QAction(mainWindow);
        actionPause_on_match->setObjectName(QString::fromUtf8("actionPause_on_match"));
        actionPause_on_match->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/PauseOnLoop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause_on_match->setIcon(icon4);
        actionStop = new QAction(mainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/Stop1NormalYellow.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon5);
        actionDump_the_memory = new QAction(mainWindow);
        actionDump_the_memory->setObjectName(QString::fromUtf8("actionDump_the_memory"));
        actionPause_when_a_loop_hypothesis_is_rejected = new QAction(mainWindow);
        actionPause_when_a_loop_hypothesis_is_rejected->setObjectName(QString::fromUtf8("actionPause_when_a_loop_hypothesis_is_rejected"));
        actionPause_when_a_loop_hypothesis_is_rejected->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/PauseLoopRejected.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause_when_a_loop_hypothesis_is_rejected->setIcon(icon6);
        actionClear_cache = new QAction(mainWindow);
        actionClear_cache->setObjectName(QString::fromUtf8("actionClear_cache"));
        actionAuto_screen_capture = new QAction(mainWindow);
        actionAuto_screen_capture->setObjectName(QString::fromUtf8("actionAuto_screen_capture"));
        actionAuto_screen_capture->setCheckable(true);
        actionDump_the_prediction_matrix = new QAction(mainWindow);
        actionDump_the_prediction_matrix->setObjectName(QString::fromUtf8("actionDump_the_prediction_matrix"));
        actionGenerate_map = new QAction(mainWindow);
        actionGenerate_map->setObjectName(QString::fromUtf8("actionGenerate_map"));
        actionDelete_memory = new QAction(mainWindow);
        actionDelete_memory->setObjectName(QString::fromUtf8("actionDelete_memory"));
        actionOpen_working_directory = new QAction(mainWindow);
        actionOpen_working_directory->setObjectName(QString::fromUtf8("actionOpen_working_directory"));
        action16_9 = new QAction(mainWindow);
        action16_9->setObjectName(QString::fromUtf8("action16_9"));
        action16_10 = new QAction(mainWindow);
        action16_10->setObjectName(QString::fromUtf8("action16_10"));
        action4_3 = new QAction(mainWindow);
        action4_3->setObjectName(QString::fromUtf8("action4_3"));
        action360p = new QAction(mainWindow);
        action360p->setObjectName(QString::fromUtf8("action360p"));
        action480p = new QAction(mainWindow);
        action480p->setObjectName(QString::fromUtf8("action480p"));
        action720p = new QAction(mainWindow);
        action720p->setObjectName(QString::fromUtf8("action720p"));
        action1080p = new QAction(mainWindow);
        action1080p->setObjectName(QString::fromUtf8("action1080p"));
        action240p = new QAction(mainWindow);
        action240p->setObjectName(QString::fromUtf8("action240p"));
        actionUsbCamera = new QAction(mainWindow);
        actionUsbCamera->setObjectName(QString::fromUtf8("actionUsbCamera"));
        actionUsbCamera->setCheckable(true);
        actionImageFiles = new QAction(mainWindow);
        actionImageFiles->setObjectName(QString::fromUtf8("actionImageFiles"));
        actionImageFiles->setCheckable(true);
        actionVideo = new QAction(mainWindow);
        actionVideo->setObjectName(QString::fromUtf8("actionVideo"));
        actionVideo->setCheckable(true);
        actionDatabase = new QAction(mainWindow);
        actionDatabase->setObjectName(QString::fromUtf8("actionDatabase"));
        actionDatabase->setCheckable(true);
        actionJdeRobotInterfaces = new QAction(mainWindow);
        actionJdeRobotInterfaces->setObjectName(QString::fromUtf8("actionJdeRobotInterfaces"));
        actionJdeRobotInterfaces->setCheckable(true);
        actionGenerate_local_map = new QAction(mainWindow);
        actionGenerate_local_map->setObjectName(QString::fromUtf8("actionGenerate_local_map"));
        actionPrint_loop_closure_IDs_to_console = new QAction(mainWindow);
        actionPrint_loop_closure_IDs_to_console->setObjectName(QString::fromUtf8("actionPrint_loop_closure_IDs_to_console"));
        actionSave_point_cloud = new QAction(mainWindow);
        actionSave_point_cloud->setObjectName(QString::fromUtf8("actionSave_point_cloud"));
        actionDownload_all_clouds = new QAction(mainWindow);
        actionDownload_all_clouds->setObjectName(QString::fromUtf8("actionDownload_all_clouds"));
        actionPause_on_local_loop_detection = new QAction(mainWindow);
        actionPause_on_local_loop_detection->setObjectName(QString::fromUtf8("actionPause_on_local_loop_detection"));
        actionPause_on_local_loop_detection->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/PauseOnLocalLoop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause_on_local_loop_detection->setIcon(icon7);
        actionStore_keyframe_information = new QAction(mainWindow);
        actionStore_keyframe_information->setObjectName(QString::fromUtf8("actionStore_keyframe_information"));
        actionStore_keyframe_information->setCheckable(true);
        actionSLAM_mode = new QAction(mainWindow);
        actionSLAM_mode->setObjectName(QString::fromUtf8("actionSLAM_mode"));
        actionSLAM_mode->setCheckable(true);
        actionLocalization_mode = new QAction(mainWindow);
        actionLocalization_mode->setObjectName(QString::fromUtf8("actionLocalization_mode"));
        actionLocalization_mode->setCheckable(true);
        actionReset_Odometry = new QAction(mainWindow);
        actionReset_Odometry->setObjectName(QString::fromUtf8("actionReset_Odometry"));
        actionView_high_res_point_cloud = new QAction(mainWindow);
        actionView_high_res_point_cloud->setObjectName(QString::fromUtf8("actionView_high_res_point_cloud"));
        actionTrigger_a_new_map = new QAction(mainWindow);
        actionTrigger_a_new_map->setObjectName(QString::fromUtf8("actionTrigger_a_new_map"));
        actionGenerate_TORO_graph_graph = new QAction(mainWindow);
        actionGenerate_TORO_graph_graph->setObjectName(QString::fromUtf8("actionGenerate_TORO_graph_graph"));
        actionDownload_graph = new QAction(mainWindow);
        actionDownload_graph->setObjectName(QString::fromUtf8("actionDownload_graph"));
        actionScreenshot = new QAction(mainWindow);
        actionScreenshot->setObjectName(QString::fromUtf8("actionScreenshot"));
        actionOpenNI_PCL = new QAction(mainWindow);
        actionOpenNI_PCL->setObjectName(QString::fromUtf8("actionOpenNI_PCL"));
        actionOpenNI_PCL->setCheckable(true);
        actionFreenect = new QAction(mainWindow);
        actionFreenect->setObjectName(QString::fromUtf8("actionFreenect"));
        actionFreenect->setCheckable(true);
        actionOpenNI_CV = new QAction(mainWindow);
        actionOpenNI_CV->setObjectName(QString::fromUtf8("actionOpenNI_CV"));
        actionOpenNI_CV->setCheckable(true);
        actionOpenNI_CV_ASUS = new QAction(mainWindow);
        actionOpenNI_CV_ASUS->setObjectName(QString::fromUtf8("actionOpenNI_CV_ASUS"));
        actionOpenNI_CV_ASUS->setCheckable(true);
        actionOpenNI2 = new QAction(mainWindow);
        actionOpenNI2->setObjectName(QString::fromUtf8("actionOpenNI2"));
        actionOpenNI2->setCheckable(true);
        actionOpenNI_PCL_ASUS = new QAction(mainWindow);
        actionOpenNI_PCL_ASUS->setObjectName(QString::fromUtf8("actionOpenNI_PCL_ASUS"));
        actionOpenNI_PCL_ASUS->setCheckable(true);
        actionData_recorder = new QAction(mainWindow);
        actionData_recorder->setObjectName(QString::fromUtf8("actionData_recorder"));
        actionExport_2D_Grid_map_bmp_png = new QAction(mainWindow);
        actionExport_2D_Grid_map_bmp_png->setObjectName(QString::fromUtf8("actionExport_2D_Grid_map_bmp_png"));
        actionExport_2D_scans_ply_pcd = new QAction(mainWindow);
        actionExport_2D_scans_ply_pcd->setObjectName(QString::fromUtf8("actionExport_2D_scans_ply_pcd"));
        actionView_scans = new QAction(mainWindow);
        actionView_scans->setObjectName(QString::fromUtf8("actionView_scans"));
        actionOpen_database = new QAction(mainWindow);
        actionOpen_database->setObjectName(QString::fromUtf8("actionOpen_database"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_database->setIcon(icon8);
        actionNew_database = new QAction(mainWindow);
        actionNew_database->setObjectName(QString::fromUtf8("actionNew_database"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/document-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_database->setIcon(icon9);
        actionClose_database = new QAction(mainWindow);
        actionClose_database->setObjectName(QString::fromUtf8("actionClose_database"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose_database->setIcon(icon10);
        actionEdit_database = new QAction(mainWindow);
        actionEdit_database->setObjectName(QString::fromUtf8("actionEdit_database"));
        actionPost_processing = new QAction(mainWindow);
        actionPost_processing->setObjectName(QString::fromUtf8("actionPost_processing"));
        actionSave_GUI_config = new QAction(mainWindow);
        actionSave_GUI_config->setObjectName(QString::fromUtf8("actionSave_GUI_config"));
        actionFreenect2 = new QAction(mainWindow);
        actionFreenect2->setObjectName(QString::fromUtf8("actionFreenect2"));
        actionFreenect2->setCheckable(true);
        actionStereoDC1394 = new QAction(mainWindow);
        actionStereoDC1394->setObjectName(QString::fromUtf8("actionStereoDC1394"));
        actionStereoDC1394->setCheckable(true);
        actionOpenNI2_kinect = new QAction(mainWindow);
        actionOpenNI2_kinect->setObjectName(QString::fromUtf8("actionOpenNI2_kinect"));
        actionOpenNI2_kinect->setCheckable(true);
        actionOpenNI2_sense = new QAction(mainWindow);
        actionOpenNI2_sense->setObjectName(QString::fromUtf8("actionOpenNI2_sense"));
        actionOpenNI2_sense->setCheckable(true);
        actionStereoFlyCapture2 = new QAction(mainWindow);
        actionStereoFlyCapture2->setObjectName(QString::fromUtf8("actionStereoFlyCapture2"));
        actionSend_goal = new QAction(mainWindow);
        actionSend_goal->setObjectName(QString::fromUtf8("actionSend_goal"));
        widget_mainWindow = new QWidget(mainWindow);
        widget_mainWindow->setObjectName(QString::fromUtf8("widget_mainWindow"));
        mainWindow->setCentralWidget(widget_mainWindow);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1012, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuAdvanced = new QMenu(menuEdit);
        menuAdvanced->setObjectName(QString::fromUtf8("menuAdvanced"));
        menu6 = new QMenu(menubar);
        menu6->setObjectName(QString::fromUtf8("menu6"));
        menuProcess = new QMenu(menubar);
        menuProcess->setObjectName(QString::fromUtf8("menuProcess"));
        menuSelect_source = new QMenu(menuProcess);
        menuSelect_source->setObjectName(QString::fromUtf8("menuSelect_source"));
        menuImage = new QMenu(menuSelect_source);
        menuImage->setObjectName(QString::fromUtf8("menuImage"));
        menuRGB_D_camera = new QMenu(menuSelect_source);
        menuRGB_D_camera->setObjectName(QString::fromUtf8("menuRGB_D_camera"));
        menuKinect_for_Xbox_360 = new QMenu(menuRGB_D_camera);
        menuKinect_for_Xbox_360->setObjectName(QString::fromUtf8("menuKinect_for_Xbox_360"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/kinect_xbox_360.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuKinect_for_Xbox_360->setIcon(icon11);
        menuXtion_PRO_LIVE = new QMenu(menuRGB_D_camera);
        menuXtion_PRO_LIVE->setObjectName(QString::fromUtf8("menuXtion_PRO_LIVE"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/xtion_pro_live.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuXtion_PRO_LIVE->setIcon(icon12);
        menuSense_3D_scanner = new QMenu(menuRGB_D_camera);
        menuSense_3D_scanner->setObjectName(QString::fromUtf8("menuSense_3D_scanner"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/sense.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuSense_3D_scanner->setIcon(icon13);
        menuKinect_v2 = new QMenu(menuRGB_D_camera);
        menuKinect_v2->setObjectName(QString::fromUtf8("menuKinect_v2"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/kinect_xbox_one.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuKinect_v2->setIcon(icon14);
        menuBumblebee2 = new QMenu(menuRGB_D_camera);
        menuBumblebee2->setObjectName(QString::fromUtf8("menuBumblebee2"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/images/bumblebee2.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuBumblebee2->setIcon(icon15);
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuShow_view = new QMenu(menuWindow);
        menuShow_view->setObjectName(QString::fromUtf8("menuShow_view"));
        menuAspect_ratio_2 = new QMenu(menuWindow);
        menuAspect_ratio_2->setObjectName(QString::fromUtf8("menuAspect_ratio_2"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainWindow->setStatusBar(statusbar);
        dockWidget_posterior = new QDockWidget(mainWindow);
        dockWidget_posterior->setObjectName(QString::fromUtf8("dockWidget_posterior"));
        dockWidget_posterior->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        posteriorPlot = new UPlot(dockWidgetContents);
        posteriorPlot->setObjectName(QString::fromUtf8("posteriorPlot"));

        horizontalLayout->addWidget(posteriorPlot);

        dockWidget_posterior->setWidget(dockWidgetContents);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_posterior);
        toolBar = new QToolBar(mainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidget_statsV2 = new QDockWidget(mainWindow);
        dockWidget_statsV2->setObjectName(QString::fromUtf8("dockWidget_statsV2"));
        dockWidget_statsV2->setFloating(true);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setVerticalSpacing(2);
        doubleSpinBox_stats_timeLimit = new QDoubleSpinBox(dockWidgetContents_3);
        doubleSpinBox_stats_timeLimit->setObjectName(QString::fromUtf8("doubleSpinBox_stats_timeLimit"));
        doubleSpinBox_stats_timeLimit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_stats_timeLimit->setDecimals(0);
        doubleSpinBox_stats_timeLimit->setMaximum(99999);
        doubleSpinBox_stats_timeLimit->setSingleStep(50);
        doubleSpinBox_stats_timeLimit->setValue(450);

        gridLayout->addWidget(doubleSpinBox_stats_timeLimit, 3, 1, 1, 1);

        label_24 = new QLabel(dockWidgetContents_3);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout->addWidget(label_24, 0, 0, 1, 1);

        label_stats_source = new QLabel(dockWidgetContents_3);
        label_stats_source->setObjectName(QString::fromUtf8("label_stats_source"));
        label_stats_source->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_stats_source, 0, 1, 1, 1);

        doubleSpinBox_stats_imgRate = new QDoubleSpinBox(dockWidgetContents_3);
        doubleSpinBox_stats_imgRate->setObjectName(QString::fromUtf8("doubleSpinBox_stats_imgRate"));
        doubleSpinBox_stats_imgRate->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_stats_imgRate->setDecimals(1);
        doubleSpinBox_stats_imgRate->setMaximum(100);
        doubleSpinBox_stats_imgRate->setSingleStep(0.1);
        doubleSpinBox_stats_imgRate->setValue(2);

        gridLayout->addWidget(doubleSpinBox_stats_imgRate, 1, 1, 1, 1);

        label_45 = new QLabel(dockWidgetContents_3);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setWordWrap(true);

        gridLayout->addWidget(label_45, 4, 0, 1, 1);

        label_elapsedTime = new QLabel(dockWidgetContents_3);
        label_elapsedTime->setObjectName(QString::fromUtf8("label_elapsedTime"));
        label_elapsedTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_elapsedTime, 4, 1, 1, 1);

        label_20 = new QLabel(dockWidgetContents_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout->addWidget(label_20, 5, 0, 1, 1);

        label_stats_imageNumber = new QLabel(dockWidgetContents_3);
        label_stats_imageNumber->setObjectName(QString::fromUtf8("label_stats_imageNumber"));
        label_stats_imageNumber->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_stats_imageNumber, 5, 1, 1, 1);

        label_12 = new QLabel(dockWidgetContents_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 6, 0, 1, 1);

        label_stats_loopClosuresDetected = new QLabel(dockWidgetContents_3);
        label_stats_loopClosuresDetected->setObjectName(QString::fromUtf8("label_stats_loopClosuresDetected"));
        label_stats_loopClosuresDetected->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_stats_loopClosuresDetected, 6, 1, 1, 1);

        label_38 = new QLabel(dockWidgetContents_3);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout->addWidget(label_38, 7, 0, 1, 1);

        label_stats_loopClosuresReactivatedDetected = new QLabel(dockWidgetContents_3);
        label_stats_loopClosuresReactivatedDetected->setObjectName(QString::fromUtf8("label_stats_loopClosuresReactivatedDetected"));
        label_stats_loopClosuresReactivatedDetected->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_stats_loopClosuresReactivatedDetected, 7, 1, 1, 1);

        label_15 = new QLabel(dockWidgetContents_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 8, 0, 1, 1);

        label_stats_loopClosuresRejected = new QLabel(dockWidgetContents_3);
        label_stats_loopClosuresRejected->setObjectName(QString::fromUtf8("label_stats_loopClosuresRejected"));
        label_stats_loopClosuresRejected->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_stats_loopClosuresRejected, 8, 1, 1, 1);

        doubleSpinBox_stats_imgRate_label = new QLabel(dockWidgetContents_3);
        doubleSpinBox_stats_imgRate_label->setObjectName(QString::fromUtf8("doubleSpinBox_stats_imgRate_label"));

        gridLayout->addWidget(doubleSpinBox_stats_imgRate_label, 1, 0, 1, 1);

        label_timeLimit = new QLabel(dockWidgetContents_3);
        label_timeLimit->setObjectName(QString::fromUtf8("label_timeLimit"));
        label_timeLimit->setWordWrap(true);

        gridLayout->addWidget(label_timeLimit, 3, 0, 1, 1);

        doubleSpinBox_stats_detectionRate = new QDoubleSpinBox(dockWidgetContents_3);
        doubleSpinBox_stats_detectionRate->setObjectName(QString::fromUtf8("doubleSpinBox_stats_detectionRate"));
        doubleSpinBox_stats_detectionRate->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_stats_detectionRate->setDecimals(1);
        doubleSpinBox_stats_detectionRate->setMaximum(100);
        doubleSpinBox_stats_detectionRate->setSingleStep(0.1);
        doubleSpinBox_stats_detectionRate->setValue(2);

        gridLayout->addWidget(doubleSpinBox_stats_detectionRate, 2, 1, 1, 1);

        doubleSpinBox_stats_imgRate_label_2 = new QLabel(dockWidgetContents_3);
        doubleSpinBox_stats_imgRate_label_2->setObjectName(QString::fromUtf8("doubleSpinBox_stats_imgRate_label_2"));

        gridLayout->addWidget(doubleSpinBox_stats_imgRate_label_2, 2, 0, 1, 1);

        gridLayout->setColumnStretch(0, 10);

        verticalLayout_8->addLayout(gridLayout);

        statsToolBox = new rtabmap::StatsToolBox(dockWidgetContents_3);
        statsToolBox->setObjectName(QString::fromUtf8("statsToolBox"));

        verticalLayout_8->addWidget(statsToolBox);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_2);

        dockWidget_statsV2->setWidget(dockWidgetContents_3);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_statsV2);
        dockWidget_likelihood = new QDockWidget(mainWindow);
        dockWidget_likelihood->setObjectName(QString::fromUtf8("dockWidget_likelihood"));
        dockWidget_likelihood->setFloating(false);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        verticalLayout_6 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        likelihoodPlot = new UPlot(dockWidgetContents_4);
        likelihoodPlot->setObjectName(QString::fromUtf8("likelihoodPlot"));

        verticalLayout_6->addWidget(likelihoodPlot);

        dockWidget_likelihood->setWidget(dockWidgetContents_4);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_likelihood);
        dockWidget_console = new QDockWidget(mainWindow);
        dockWidget_console->setObjectName(QString::fromUtf8("dockWidget_console"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        widget_console = new rtabmap::ConsoleWidget(dockWidgetContents_2);
        widget_console->setObjectName(QString::fromUtf8("widget_console"));

        verticalLayout_5->addWidget(widget_console);

        dockWidget_console->setWidget(dockWidgetContents_2);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_console);
        dockWidget_rawlikelihood = new QDockWidget(mainWindow);
        dockWidget_rawlikelihood->setObjectName(QString::fromUtf8("dockWidget_rawlikelihood"));
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QString::fromUtf8("dockWidgetContents_6"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_6);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        rawLikelihoodPlot = new UPlot(dockWidgetContents_6);
        rawLikelihoodPlot->setObjectName(QString::fromUtf8("rawLikelihoodPlot"));

        verticalLayout_4->addWidget(rawLikelihoodPlot);

        dockWidget_rawlikelihood->setWidget(dockWidgetContents_6);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_rawlikelihood);
        dockWidget_cloudViewer = new QDockWidget(mainWindow);
        dockWidget_cloudViewer->setObjectName(QString::fromUtf8("dockWidget_cloudViewer"));
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_cloudViewer = new rtabmap::CloudViewer(dockWidgetContents_5);
        widget_cloudViewer->setObjectName(QString::fromUtf8("widget_cloudViewer"));

        verticalLayout_3->addWidget(widget_cloudViewer);

        dockWidget_cloudViewer->setWidget(dockWidgetContents_5);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_cloudViewer);
        dockWidget_loopClosureViewer = new QDockWidget(mainWindow);
        dockWidget_loopClosureViewer->setObjectName(QString::fromUtf8("dockWidget_loopClosureViewer"));
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        verticalLayout_7 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        widget_loopClosureViewer = new rtabmap::LoopClosureViewer(dockWidgetContents_7);
        widget_loopClosureViewer->setObjectName(QString::fromUtf8("widget_loopClosureViewer"));

        verticalLayout_7->addWidget(widget_loopClosureViewer);

        dockWidget_loopClosureViewer->setWidget(dockWidgetContents_7);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_loopClosureViewer);
        dockWidget_mapVisibility = new QDockWidget(mainWindow);
        dockWidget_mapVisibility->setObjectName(QString::fromUtf8("dockWidget_mapVisibility"));
        dockWidget_mapVisibility->setFloating(true);
        dockWidgetContents_8 = new QWidget();
        dockWidgetContents_8->setObjectName(QString::fromUtf8("dockWidgetContents_8"));
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_8);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        widget_mapVisibility = new rtabmap::MapVisibilityWidget(dockWidgetContents_8);
        widget_mapVisibility->setObjectName(QString::fromUtf8("widget_mapVisibility"));

        verticalLayout_9->addWidget(widget_mapVisibility);

        dockWidget_mapVisibility->setWidget(dockWidgetContents_8);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_mapVisibility);
        dockWidget_graphViewer = new QDockWidget(mainWindow);
        dockWidget_graphViewer->setObjectName(QString::fromUtf8("dockWidget_graphViewer"));
        dockWidgetContents_9 = new QWidget();
        dockWidgetContents_9->setObjectName(QString::fromUtf8("dockWidgetContents_9"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_9);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        graphicsView_graphView = new rtabmap::GraphViewer(dockWidgetContents_9);
        graphicsView_graphView->setObjectName(QString::fromUtf8("graphicsView_graphView"));

        verticalLayout_10->addWidget(graphicsView_graphView);

        dockWidget_graphViewer->setWidget(dockWidgetContents_9);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_graphViewer);
        dockWidget_imageView = new QDockWidget(mainWindow);
        dockWidget_imageView->setObjectName(QString::fromUtf8("dockWidget_imageView"));
        dockWidget_imageView->setAllowedAreas(Qt::AllDockWidgetAreas);
        layout_imageview = new QWidget();
        layout_imageview->setObjectName(QString::fromUtf8("layout_imageview"));
        horizontalLayout_2 = new QHBoxLayout(layout_imageview);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_refId = new QLabel(layout_imageview);
        label_refId->setObjectName(QString::fromUtf8("label_refId"));
        label_refId->setMinimumSize(QSize(0, 18));
        label_refId->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_refId);

        imageView_source = new rtabmap::ImageView(layout_imageview);
        imageView_source->setObjectName(QString::fromUtf8("imageView_source"));

        verticalLayout_2->addWidget(imageView_source);

        verticalLayout_2->setStretch(1, 1);

        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_matchId = new QLabel(layout_imageview);
        label_matchId->setObjectName(QString::fromUtf8("label_matchId"));
        label_matchId->setMinimumSize(QSize(0, 18));
        label_matchId->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_matchId);

        imageView_loopClosure = new rtabmap::ImageView(layout_imageview);
        imageView_loopClosure->setObjectName(QString::fromUtf8("imageView_loopClosure"));

        verticalLayout->addWidget(imageView_loopClosure);

        verticalLayout->setStretch(1, 1);

        horizontalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 1);
        dockWidget_imageView->setWidget(layout_imageview);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_imageView);
        dockWidget_odometry = new QDockWidget(mainWindow);
        dockWidget_odometry->setObjectName(QString::fromUtf8("dockWidget_odometry"));
        dockWidgetContents_10 = new QWidget();
        dockWidgetContents_10->setObjectName(QString::fromUtf8("dockWidgetContents_10"));
        verticalLayout_11 = new QVBoxLayout(dockWidgetContents_10);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        imageView_odometry = new rtabmap::ImageView(dockWidgetContents_10);
        imageView_odometry->setObjectName(QString::fromUtf8("imageView_odometry"));

        verticalLayout_11->addWidget(imageView_odometry);

        dockWidget_odometry->setWidget(dockWidgetContents_10);
        mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_odometry);
        toolBar_2 = new QToolBar(mainWindow);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        mainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuProcess->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuWindow->menuAction());
        menubar->addAction(menu6->menuAction());
        menuFile->addAction(actionNew_database);
        menuFile->addAction(actionOpen_database);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_GUI_config);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_point_cloud);
        menuFile->addAction(actionExport_2D_scans_ply_pcd);
        menuFile->addAction(actionExport_2D_Grid_map_bmp_png);
        menuFile->addSeparator();
        menuFile->addAction(actionClose_database);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionDownload_all_clouds);
        menuEdit->addAction(actionDownload_graph);
        menuEdit->addAction(actionClear_cache);
        menuEdit->addSeparator();
        menuEdit->addAction(actionView_high_res_point_cloud);
        menuEdit->addAction(actionView_scans);
        menuEdit->addSeparator();
        menuEdit->addAction(actionDelete_memory);
        menuEdit->addSeparator();
        menuEdit->addAction(actionOpen_working_directory);
        menuEdit->addAction(menuAdvanced->menuAction());
        menuAdvanced->addAction(actionDump_the_memory);
        menuAdvanced->addAction(actionDump_the_prediction_matrix);
        menuAdvanced->addAction(actionGenerate_map);
        menuAdvanced->addAction(actionGenerate_local_map);
        menuAdvanced->addAction(actionGenerate_TORO_graph_graph);
        menuAdvanced->addAction(actionPrint_loop_closure_IDs_to_console);
        menuAdvanced->addAction(actionSend_goal);
        menu6->addAction(actionHelp);
        menu6->addSeparator();
        menu6->addAction(actionAbout);
        menuProcess->addAction(menuSelect_source->menuAction());
        menuProcess->addSeparator();
        menuProcess->addAction(actionStart);
        menuProcess->addAction(actionPause);
        menuProcess->addAction(actionStop);
        menuProcess->addSeparator();
        menuProcess->addAction(actionPause_on_match);
        menuProcess->addAction(actionPause_when_a_loop_hypothesis_is_rejected);
        menuProcess->addAction(actionPause_on_local_loop_detection);
        menuProcess->addSeparator();
        menuProcess->addAction(actionStore_keyframe_information);
        menuProcess->addSeparator();
        menuProcess->addAction(actionSLAM_mode);
        menuProcess->addAction(actionLocalization_mode);
        menuProcess->addSeparator();
        menuProcess->addAction(actionReset_Odometry);
        menuProcess->addAction(actionTrigger_a_new_map);
        menuSelect_source->addAction(menuRGB_D_camera->menuAction());
        menuSelect_source->addAction(menuImage->menuAction());
        menuSelect_source->addAction(actionDatabase);
        menuImage->addAction(actionUsbCamera);
        menuImage->addAction(actionImageFiles);
        menuImage->addAction(actionVideo);
        menuRGB_D_camera->addAction(menuKinect_for_Xbox_360->menuAction());
        menuRGB_D_camera->addAction(menuXtion_PRO_LIVE->menuAction());
        menuRGB_D_camera->addAction(menuSense_3D_scanner->menuAction());
        menuRGB_D_camera->addAction(menuKinect_v2->menuAction());
        menuRGB_D_camera->addAction(menuBumblebee2->menuAction());
        menuRGB_D_camera->addAction(actionJdeRobotInterfaces);
        menuKinect_for_Xbox_360->addAction(actionFreenect);
        menuKinect_for_Xbox_360->addAction(actionOpenNI2_kinect);
        menuKinect_for_Xbox_360->addAction(actionOpenNI_PCL);
        menuKinect_for_Xbox_360->addAction(actionOpenNI_CV);
        menuXtion_PRO_LIVE->addAction(actionOpenNI2);
        menuXtion_PRO_LIVE->addAction(actionOpenNI_PCL_ASUS);
        menuXtion_PRO_LIVE->addAction(actionOpenNI_CV_ASUS);
        menuSense_3D_scanner->addAction(actionOpenNI2_sense);
        menuKinect_v2->addAction(actionFreenect2);
        menuBumblebee2->addAction(actionStereoDC1394);
        menuBumblebee2->addAction(actionStereoFlyCapture2);
        menuWindow->addAction(menuShow_view->menuAction());
        menuWindow->addAction(actionScreenshot);
        menuWindow->addAction(actionAuto_screen_capture);
        menuWindow->addAction(menuAspect_ratio_2->menuAction());
        menuWindow->addSeparator();
        menuWindow->addAction(actionPreferences);
        menuShow_view->addSeparator();
        menuAspect_ratio_2->addAction(action16_9);
        menuAspect_ratio_2->addAction(action16_10);
        menuAspect_ratio_2->addAction(action4_3);
        menuAspect_ratio_2->addSeparator();
        menuAspect_ratio_2->addAction(action1080p);
        menuAspect_ratio_2->addAction(action720p);
        menuAspect_ratio_2->addAction(action480p);
        menuAspect_ratio_2->addAction(action360p);
        menuAspect_ratio_2->addAction(action240p);
        menuTools->addAction(actionEdit_database);
        menuTools->addSeparator();
        menuTools->addAction(actionData_recorder);
        menuTools->addSeparator();
        menuTools->addAction(actionPost_processing);
        toolBar->addAction(actionClose_database);
        toolBar->addAction(actionNew_database);
        toolBar->addAction(actionOpen_database);
        toolBar_2->addAction(actionStart);
        toolBar_2->addAction(actionPause);
        toolBar_2->addAction(actionStop);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "RTAB-Map : Real-Time Appearance-Based Mapping", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("mainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("mainWindow", "Help", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("mainWindow", "About...", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("mainWindow", "Preferences...", 0, QApplication::UnicodeUTF8));
        actionStart->setText(QApplication::translate("mainWindow", "Start", 0, QApplication::UnicodeUTF8));
        actionPause->setText(QApplication::translate("mainWindow", "Pause", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPause->setToolTip(QApplication::translate("mainWindow", "Pause", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPause_on_match->setText(QApplication::translate("mainWindow", "Pause on loop closure detection", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("mainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        actionDump_the_memory->setText(QApplication::translate("mainWindow", "Dump the memory", 0, QApplication::UnicodeUTF8));
        actionPause_when_a_loop_hypothesis_is_rejected->setText(QApplication::translate("mainWindow", "Pause on loop closure rejection", 0, QApplication::UnicodeUTF8));
        actionClear_cache->setText(QApplication::translate("mainWindow", "Clear the cache", 0, QApplication::UnicodeUTF8));
        actionAuto_screen_capture->setText(QApplication::translate("mainWindow", "Auto screen capture...", 0, QApplication::UnicodeUTF8));
        actionDump_the_prediction_matrix->setText(QApplication::translate("mainWindow", "Dump the prediction matrix", 0, QApplication::UnicodeUTF8));
        actionGenerate_map->setText(QApplication::translate("mainWindow", "Generate graph map (*.dot)...", 0, QApplication::UnicodeUTF8));
        actionDelete_memory->setText(QApplication::translate("mainWindow", "Delete memory", 0, QApplication::UnicodeUTF8));
        actionOpen_working_directory->setText(QApplication::translate("mainWindow", "Show working directory in file browser", 0, QApplication::UnicodeUTF8));
        action16_9->setText(QApplication::translate("mainWindow", "16:9", 0, QApplication::UnicodeUTF8));
        action16_10->setText(QApplication::translate("mainWindow", "16:10", 0, QApplication::UnicodeUTF8));
        action4_3->setText(QApplication::translate("mainWindow", "4:3", 0, QApplication::UnicodeUTF8));
        action360p->setText(QApplication::translate("mainWindow", "360p", 0, QApplication::UnicodeUTF8));
        action480p->setText(QApplication::translate("mainWindow", "480p", 0, QApplication::UnicodeUTF8));
        action720p->setText(QApplication::translate("mainWindow", "720p", 0, QApplication::UnicodeUTF8));
        action1080p->setText(QApplication::translate("mainWindow", "1080p", 0, QApplication::UnicodeUTF8));
        action240p->setText(QApplication::translate("mainWindow", "240p", 0, QApplication::UnicodeUTF8));
        actionUsbCamera->setText(QApplication::translate("mainWindow", "Usb camera", 0, QApplication::UnicodeUTF8));
        actionImageFiles->setText(QApplication::translate("mainWindow", "Images...", 0, QApplication::UnicodeUTF8));
        actionVideo->setText(QApplication::translate("mainWindow", "Video...", 0, QApplication::UnicodeUTF8));
        actionDatabase->setText(QApplication::translate("mainWindow", "Database...", 0, QApplication::UnicodeUTF8));
        actionJdeRobotInterfaces->setText(QApplication::translate("mainWindow", "JdeRobot Interfaces", 0, QApplication::UnicodeUTF8));
        actionGenerate_local_map->setText(QApplication::translate("mainWindow", "Generate graph local map (*.dot)...", 0, QApplication::UnicodeUTF8));
        actionPrint_loop_closure_IDs_to_console->setText(QApplication::translate("mainWindow", "Print loop closure IDs to console", 0, QApplication::UnicodeUTF8));
        actionSave_point_cloud->setText(QApplication::translate("mainWindow", "Export 3D clouds (*.ply *.pcd)...", 0, QApplication::UnicodeUTF8));
        actionDownload_all_clouds->setText(QApplication::translate("mainWindow", "Download all clouds (update cache)", 0, QApplication::UnicodeUTF8));
        actionPause_on_local_loop_detection->setText(QApplication::translate("mainWindow", "Pause on local loop closure detection", 0, QApplication::UnicodeUTF8));
        actionStore_keyframe_information->setText(QApplication::translate("mainWindow", "Store rgb+d and pose of keyframes to HOME/rtabmap_keyframes", 0, QApplication::UnicodeUTF8));
        actionSLAM_mode->setText(QApplication::translate("mainWindow", "Mapping", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSLAM_mode->setToolTip(QApplication::translate("mainWindow", "Simultaneous Localization And Mapping (SLAM)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLocalization_mode->setText(QApplication::translate("mainWindow", "Localization", 0, QApplication::UnicodeUTF8));
        actionReset_Odometry->setText(QApplication::translate("mainWindow", "Reset Odometry", 0, QApplication::UnicodeUTF8));
        actionView_high_res_point_cloud->setText(QApplication::translate("mainWindow", "View high-res point clouds...", 0, QApplication::UnicodeUTF8));
        actionTrigger_a_new_map->setText(QApplication::translate("mainWindow", "Trigger a new map", 0, QApplication::UnicodeUTF8));
        actionGenerate_TORO_graph_graph->setText(QApplication::translate("mainWindow", "Generate TORO graph (*.graph)...", 0, QApplication::UnicodeUTF8));
        actionDownload_graph->setText(QApplication::translate("mainWindow", "Download graph only", 0, QApplication::UnicodeUTF8));
        actionScreenshot->setText(QApplication::translate("mainWindow", "Take a screenshot", 0, QApplication::UnicodeUTF8));
        actionOpenNI_PCL->setText(QApplication::translate("mainWindow", "OpenNI-PCL", 0, QApplication::UnicodeUTF8));
        actionFreenect->setText(QApplication::translate("mainWindow", "Freenect", 0, QApplication::UnicodeUTF8));
        actionOpenNI_CV->setText(QApplication::translate("mainWindow", "OpenNI-CV", 0, QApplication::UnicodeUTF8));
        actionOpenNI_CV_ASUS->setText(QApplication::translate("mainWindow", "OpenNI-CV-ASUS", 0, QApplication::UnicodeUTF8));
        actionOpenNI2->setText(QApplication::translate("mainWindow", "OpenNI2", 0, QApplication::UnicodeUTF8));
        actionOpenNI_PCL_ASUS->setText(QApplication::translate("mainWindow", "OpenNI-PCL", 0, QApplication::UnicodeUTF8));
        actionData_recorder->setText(QApplication::translate("mainWindow", "Data recorder...", 0, QApplication::UnicodeUTF8));
        actionExport_2D_Grid_map_bmp_png->setText(QApplication::translate("mainWindow", "Export 2D grid map (*.png *.bmp)...", 0, QApplication::UnicodeUTF8));
        actionExport_2D_scans_ply_pcd->setText(QApplication::translate("mainWindow", "Export 2D scans (*.ply *.pcd)...", 0, QApplication::UnicodeUTF8));
        actionView_scans->setText(QApplication::translate("mainWindow", "View scans...", 0, QApplication::UnicodeUTF8));
        actionOpen_database->setText(QApplication::translate("mainWindow", "Open database...", 0, QApplication::UnicodeUTF8));
        actionNew_database->setText(QApplication::translate("mainWindow", "New database...", 0, QApplication::UnicodeUTF8));
        actionClose_database->setText(QApplication::translate("mainWindow", "Close database", 0, QApplication::UnicodeUTF8));
        actionEdit_database->setText(QApplication::translate("mainWindow", "Edit database...", 0, QApplication::UnicodeUTF8));
        actionPost_processing->setText(QApplication::translate("mainWindow", "Post-processing...", 0, QApplication::UnicodeUTF8));
        actionSave_GUI_config->setText(QApplication::translate("mainWindow", "Save config", 0, QApplication::UnicodeUTF8));
        actionFreenect2->setText(QApplication::translate("mainWindow", "Freenect2", 0, QApplication::UnicodeUTF8));
        actionStereoDC1394->setText(QApplication::translate("mainWindow", "StereoDC1394", 0, QApplication::UnicodeUTF8));
        actionOpenNI2_kinect->setText(QApplication::translate("mainWindow", "OpenNI2", 0, QApplication::UnicodeUTF8));
        actionOpenNI2_sense->setText(QApplication::translate("mainWindow", "OpenNI2", 0, QApplication::UnicodeUTF8));
        actionStereoFlyCapture2->setText(QApplication::translate("mainWindow", "StereoFlyCapture2", 0, QApplication::UnicodeUTF8));
        actionSend_goal->setText(QApplication::translate("mainWindow", "Send a goal...", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("mainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("mainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuAdvanced->setTitle(QApplication::translate("mainWindow", "Advanced", 0, QApplication::UnicodeUTF8));
        menu6->setTitle(QApplication::translate("mainWindow", "?", 0, QApplication::UnicodeUTF8));
        menuProcess->setTitle(QApplication::translate("mainWindow", "Detection", 0, QApplication::UnicodeUTF8));
        menuSelect_source->setTitle(QApplication::translate("mainWindow", "Select source", 0, QApplication::UnicodeUTF8));
        menuImage->setTitle(QApplication::translate("mainWindow", "Image-only", 0, QApplication::UnicodeUTF8));
        menuRGB_D_camera->setTitle(QApplication::translate("mainWindow", "RGB-D camera", 0, QApplication::UnicodeUTF8));
        menuKinect_for_Xbox_360->setTitle(QApplication::translate("mainWindow", "Kinect", 0, QApplication::UnicodeUTF8));
        menuXtion_PRO_LIVE->setTitle(QApplication::translate("mainWindow", "Xtion PRO LIVE", 0, QApplication::UnicodeUTF8));
        menuSense_3D_scanner->setTitle(QApplication::translate("mainWindow", "Sense 3D scanner", 0, QApplication::UnicodeUTF8));
        menuKinect_v2->setTitle(QApplication::translate("mainWindow", "Kinect v2", 0, QApplication::UnicodeUTF8));
        menuBumblebee2->setTitle(QApplication::translate("mainWindow", "Bumblebee2", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("mainWindow", "Window", 0, QApplication::UnicodeUTF8));
        menuShow_view->setTitle(QApplication::translate("mainWindow", "Show view", 0, QApplication::UnicodeUTF8));
        menuAspect_ratio_2->setTitle(QApplication::translate("mainWindow", "Aspect ratio", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("mainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        dockWidget_posterior->setWindowTitle(QApplication::translate("mainWindow", "A posteriori PDF", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("mainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        dockWidget_statsV2->setWindowTitle(QApplication::translate("mainWindow", "Statistics", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_stats_timeLimit->setSuffix(QApplication::translate("mainWindow", " ms", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("mainWindow", "Source", 0, QApplication::UnicodeUTF8));
        label_stats_source->setText(QApplication::translate("mainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_stats_imgRate->setSuffix(QApplication::translate("mainWindow", " Hz", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("mainWindow", "Elapsed time (hh:mm:ss)", 0, QApplication::UnicodeUTF8));
        label_elapsedTime->setText(QApplication::translate("mainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("mainWindow", "Current image id [Map id]", 0, QApplication::UnicodeUTF8));
        label_stats_imageNumber->setText(QApplication::translate("mainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("mainWindow", "Loop closures detected", 0, QApplication::UnicodeUTF8));
        label_stats_loopClosuresDetected->setText(QApplication::translate("mainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("mainWindow", "Loop closures detected \n"
"(only reactivated ones)", 0, QApplication::UnicodeUTF8));
        label_stats_loopClosuresReactivatedDetected->setText(QApplication::translate("mainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("mainWindow", "Loop closures rejected", 0, QApplication::UnicodeUTF8));
        label_stats_loopClosuresRejected->setText(QApplication::translate("mainWindow", "0", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_stats_imgRate_label->setText(QApplication::translate("mainWindow", "Image rate", 0, QApplication::UnicodeUTF8));
        label_timeLimit->setText(QApplication::translate("mainWindow", "Time limit processing", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_stats_detectionRate->setSuffix(QApplication::translate("mainWindow", " Hz", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_stats_imgRate_label_2->setText(QApplication::translate("mainWindow", "RTAB-Map update rate", 0, QApplication::UnicodeUTF8));
        dockWidget_likelihood->setWindowTitle(QApplication::translate("mainWindow", "Likelihood", 0, QApplication::UnicodeUTF8));
        dockWidget_console->setWindowTitle(QApplication::translate("mainWindow", "Console", 0, QApplication::UnicodeUTF8));
        dockWidget_rawlikelihood->setWindowTitle(QApplication::translate("mainWindow", "Raw Likelihood", 0, QApplication::UnicodeUTF8));
        dockWidget_cloudViewer->setWindowTitle(QApplication::translate("mainWindow", "3D Map", 0, QApplication::UnicodeUTF8));
        dockWidget_loopClosureViewer->setWindowTitle(QApplication::translate("mainWindow", "3D Loop closure", 0, QApplication::UnicodeUTF8));
        dockWidget_mapVisibility->setWindowTitle(QApplication::translate("mainWindow", "Map visibility", 0, QApplication::UnicodeUTF8));
        dockWidget_graphViewer->setWindowTitle(QApplication::translate("mainWindow", "Graph view", 0, QApplication::UnicodeUTF8));
        dockWidget_imageView->setWindowTitle(QApplication::translate("mainWindow", "Loop closure detection", 0, QApplication::UnicodeUTF8));
        label_refId->setText(QString());
        label_matchId->setText(QString());
        dockWidget_odometry->setWindowTitle(QApplication::translate("mainWindow", "Odometry", 0, QApplication::UnicodeUTF8));
        toolBar_2->setWindowTitle(QApplication::translate("mainWindow", "toolBar_2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
