/********************************************************************************
** Form generated from reading UI file 'preferencesDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "utilite/UPlot.h"

QT_BEGIN_NAMESPACE

class Ui_preferencesDialog
{
public:
    QVBoxLayout *verticalLayout_26;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_18;
    QTreeView *treeView;
    QVBoxLayout *verticalLayout_17;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_16;
    QVBoxLayout *verticalLayout_24;
    QStackedWidget *stackedWidget;
    QWidget *page_22;
    QVBoxLayout *verticalLayout_29;
    QGroupBox *groupBox_generalSettingsGui0;
    QVBoxLayout *verticalLayout_28;
    QGridLayout *gridLayout_39;
    QCheckBox *general_checkBox_imagesKept;
    QLabel *label_59;
    QCheckBox *checkBox_beep;
    QLabel *label_88;
    QLabel *label_102;
    QCheckBox *checkBox_notifyWhenNewGlobalPathIsReceived;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_31;
    QCheckBox *checkBox_imageRejectedShown;
    QLabel *label_105;
    QCheckBox *checkBox_imageHighestHypShown;
    QLabel *label_73;
    QLabel *label_70;
    QCheckBox *checkBox_verticalLayoutUsed;
    QLabel *label_222;
    QCheckBox *checkBox_posteriorGraphView;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_38;
    QSpinBox *spinBox_odomQualityWarnThr;
    QLabel *label_86;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *pushButton_loadConfig;
    QPushButton *pushButton_saveConfig;
    QPushButton *pushButton_resetConfig;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_24;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_42;
    QGroupBox *groupBox_cloudRendering1;
    QVBoxLayout *verticalLayout_31;
    QGroupBox *groupBox_poseFiltering;
    QVBoxLayout *verticalLayout_37;
    QLabel *label_33;
    QGridLayout *gridLayout_17;
    QDoubleSpinBox *doubleSpinBox_cloudFilterRadius;
    QLabel *label_37;
    QDoubleSpinBox *doubleSpinBox_cloudFilterAngle;
    QLabel *label_48;
    QGroupBox *groupBox_gridMap;
    QVBoxLayout *verticalLayout_45;
    QLabel *label_167;
    QGridLayout *gridLayout_20;
    QDoubleSpinBox *doubleSpinBox_map_opacity;
    QCheckBox *checkBox_map_shown;
    QLabel *label_map_shown;
    QDoubleSpinBox *doubleSpinBox_map_resolution;
    QLabel *label_159;
    QLabel *label_170;
    QCheckBox *checkBox_map_occupancyFrom3DCloud;
    QLabel *label_210;
    QLabel *label_224;
    QCheckBox *checkBox_map_erode;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox_showScans;
    QSpinBox *spinBox_ptsize_scan;
    QLabel *label_108;
    QLabel *label_154;
    QLabel *label_137;
    QCheckBox *checkBox_showClouds;
    QCheckBox *checkBox_showOdomClouds;
    QLabel *label_119;
    QDoubleSpinBox *doubleSpinBox_voxelSize;
    QDoubleSpinBox *doubleSpinBox_voxelSize_odom;
    QLabel *label_107;
    QSpinBox *spinBox_decimation;
    QSpinBox *spinBox_decimation_odom;
    QDoubleSpinBox *doubleSpinBox_maxDepth;
    QDoubleSpinBox *doubleSpinBox_maxDepth_odom;
    QLabel *label_132;
    QDoubleSpinBox *doubleSpinBox_opacity;
    QDoubleSpinBox *doubleSpinBox_opacity_odom;
    QLabel *label_155;
    QSpinBox *spinBox_ptsize;
    QSpinBox *spinBox_ptsize_odom;
    QLabel *label_157;
    QCheckBox *checkBox_showOdomScans;
    QLabel *label_110;
    QDoubleSpinBox *doubleSpinBox_opacity_scan;
    QCheckBox *checkBox_meshing;
    QDoubleSpinBox *doubleSpinBox_opacity_odom_scan;
    QLabel *label_156;
    QSpinBox *spinBox_ptsize_odom_scan;
    QLabel *label_158;
    QLabel *label_168;
    QLabel *label_85;
    QDoubleSpinBox *doubleSpinBox_gp3Radius;
    QDoubleSpinBox *doubleSpinBox_mlsRadius;
    QLabel *label_169;
    QLabel *label_87;
    QCheckBox *checkBox_mls;
    QLabel *label_71;
    QSpinBox *spinBox_normalKSearch;
    QLabel *label_213;
    QCheckBox *checkBox_showGraphs;
    QSpacerItem *verticalSpacer;
    QWidget *page_8;
    QVBoxLayout *verticalLayout_43;
    QGroupBox *groupBox_logging1;
    QGridLayout *gridLayout_40;
    QComboBox *comboBox_loggerLevel;
    QLabel *label_60;
    QComboBox *comboBox_loggerEventLevel;
    QLabel *label_61;
    QComboBox *comboBox_loggerPauseLevel;
    QLabel *label_62;
    QCheckBox *checkBox_logger_printTime;
    QLabel *label_67;
    QComboBox *comboBox_loggerType;
    QLabel *label_68;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_27;
    QGroupBox *groupBox_source0;
    QVBoxLayout *verticalLayout_7;
    QGridLayout *gridLayout_28;
    QDoubleSpinBox *general_doubleSpinBox_imgRate;
    QLabel *label_3;
    QLabel *label_17;
    QCheckBox *source_mirroring;
    QGroupBox *groupBox_sourceImage;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_5;
    QComboBox *source_comboBox_image_type;
    QLabel *label_20;
    QLabel *label_10;
    QLabel *label_11;
    QSpinBox *source_spinBox_imgWidth;
    QSpinBox *source_spinBox_imgheight;
    QStackedWidget *stackedWidget_image;
    QWidget *page_7;
    QVBoxLayout *verticalLayout_30;
    QGroupBox *source_groupBox_usbDevice;
    QGridLayout *gridLayout_6;
    QLabel *label_38;
    QSpinBox *source_usbDevice_spinBox_id;
    QSpacerItem *verticalSpacer_8;
    QWidget *page_5;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *source_groupBox_images;
    QGridLayout *gridLayout_7;
    QToolButton *source_images_toolButton_selectSource;
    QLineEdit *source_images_lineEdit_path;
    QSpinBox *source_images_spinBox_startPos;
    QLabel *label_21;
    QCheckBox *source_images_refreshDir;
    QLabel *label_34;
    QSpacerItem *verticalSpacer_9;
    QWidget *page_6;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *source_groupBox_video;
    QGridLayout *gridLayout_8;
    QToolButton *source_video_toolButton_selectSource;
    QLineEdit *source_video_lineEdit_path;
    QSpacerItem *verticalSpacer_10;
    QGroupBox *groupBox_sourceDatabase;
    QGridLayout *gridLayout_9;
    QToolButton *toolButton_dbViewer;
    QCheckBox *source_checkBox_ignoreOdometry;
    QToolButton *source_database_toolButton_selectSource;
    QLineEdit *source_database_lineEdit_path;
    QLabel *label_58;
    QSpinBox *source_spinBox_databaseStartPos;
    QLabel *label_72;
    QLabel *label_80;
    QCheckBox *source_checkBox_ignoreGoalDelay;
    QLabel *label_90;
    QCheckBox *source_checkBox_useDbStamps;
    QGroupBox *groupBox_sourceOpenni;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_40;
    QGridLayout *gridLayout_55;
    QComboBox *comboBox_cameraRGBD;
    QPushButton *pushButton_calibrate;
    QLabel *label_46;
    QPushButton *pushButton_test_rgbd_camera;
    QLabel *label_228;
    QGroupBox *groupBox_openni2;
    QGridLayout *gridLayout_54;
    QCheckBox *openni2_autoWhiteBalance;
    QLabel *label_217;
    QCheckBox *openni2_autoExposure;
    QLabel *label_218;
    QSpinBox *openni2_exposure;
    QLabel *label_219;
    QSpinBox *openni2_gain;
    QLabel *label_220;
    QCheckBox *openni2_mirroring;
    QLabel *label_223;
    QGroupBox *groupBox_freenect2;
    QGridLayout *gridLayout_56;
    QLabel *label_230;
    QComboBox *comboBox_freenect2Format;
    QFormLayout *formLayout;
    QLineEdit *lineEdit_openniDevice;
    QLabel *label_39;
    QLineEdit *lineEdit_openniLocalTransform;
    QLabel *label_42;
    QLabel *label_229;
    QCheckBox *checkbox_rgbd_colorOnly;
    QSpacerItem *verticalSpacer_7;
    QWidget *page_20;
    QVBoxLayout *verticalLayout_23;
    QGroupBox *groupBox_rtabmap_basic0;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *general_doubleSpinBox_detectionRate_2;
    QCheckBox *general_checkBox_SLAM_mode_2;
    QLabel *label_79;
    QCheckBox *general_checkBox_activateRGBD_2;
    QLabel *label_activateRGBD_2;
    QCheckBox *general_checkBox_publishStats_2;
    QLabel *label_publishStat;
    QLabel *label_134;
    QSpinBox *general_spinBox_imagesBufferSize_2;
    QLabel *label_112;
    QDoubleSpinBox *general_doubleSpinBox_timeThr_2;
    QLabel *label_timeLimit_2;
    QDoubleSpinBox *general_doubleSpinBox_hardThr_2;
    QLabel *label_121;
    QDoubleSpinBox *doubleSpinBox_similarityThreshold_2;
    QLabel *label_similarity_2;
    QSpinBox *general_spinBox_maxStMemSize_2;
    QLabel *label_126;
    QSpacerItem *verticalSpacer_5;
    QWidget *page;
    QVBoxLayout *verticalLayout_44;
    QGroupBox *groupBox_rtabmap_advanced0;
    QVBoxLayout *verticalLayout_15;
    QGridLayout *gridLayout_43;
    QCheckBox *general_checkBox_SLAM_mode;
    QLabel *label_92;
    QDoubleSpinBox *general_doubleSpinBox_detectionRate;
    QLabel *label_135;
    QSpinBox *general_spinBox_imagesBufferSize;
    QLabel *label_84;
    QCheckBox *general_checkBox_startNewMapOnLoopClosure;
    QLabel *label_185;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_44;
    QDoubleSpinBox *general_doubleSpinBox_timeThr;
    QLabel *label_timeLimit;
    QSpinBox *general_spinBox_memoryThr;
    QLabel *label_maxWmSize;
    QDoubleSpinBox *general_doubleSpinBox_hardThr;
    QLabel *label_93;
    QDoubleSpinBox *general_doubleSpinBox_loopRatio;
    QLabel *label_96;
    QGroupBox *groupBox_publishing;
    QGridLayout *gridLayout_45;
    QCheckBox *general_checkBox_publishRawData;
    QLabel *label_91;
    QCheckBox *general_checkBox_publishPdf;
    QLabel *label_106;
    QCheckBox *general_checkBox_publishLikelihood;
    QLabel *label_116;
    QGroupBox *groupBox_statistics;
    QGridLayout *gridLayout_46;
    QCheckBox *general_checkBox_statisticLogsBufferedInRAM;
    QLabel *label_143;
    QCheckBox *general_checkBox_statisticLoggedHeaders;
    QLabel *label_209;
    QGridLayout *gridLayout_4;
    QLabel *label_workingDirectory;
    QToolButton *toolButton_workingDirectory;
    QLineEdit *lineEdit_workingDirectory;
    QSpacerItem *verticalSpacer_4;
    QWidget *page_23;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_bayes1;
    QVBoxLayout *verticalLayout_22;
    QLabel *label;
    QLabel *label_76;
    QGridLayout *gridLayout_24;
    QLabel *label_65;
    QLabel *label_66;
    QLabel *label_prediction_sum;
    QLineEdit *lineEdit_bayes_predictionLC;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_63;
    UPlot *predictionPlot;
    QGridLayout *gridLayout_22;
    QCheckBox *checkBox_bayes_fullPredictionUpdate;
    QDoubleSpinBox *general_doubleSpinBox_vp;
    QLabel *label_113;
    QLabel *label_142;
    QLabel *label_82;
    QCheckBox *checkBox_kp_tfIdfLikelihoodUsed;
    QSpacerItem *verticalSpacer_23;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_memory1;
    QVBoxLayout *verticalLayout_10;
    QGridLayout *gridLayout_42;
    QSpinBox *spinBox_imageDecimation;
    QDoubleSpinBox *general_doubleSpinBox_recentWmRatio;
    QSpinBox *general_spinBox_maxStMemSize;
    QLabel *label_retrieved_4;
    QLabel *label_29;
    QLabel *label_ratioRecent;
    QSpinBox *general_spinBox_maxRetrieved;
    QLabel *label_retrieved;
    QCheckBox *general_checkBox_generateIds;
    QLabel *label_retrieved_2;
    QCheckBox *general_checkBox_badSignaturesIgnored;
    QLabel *label_retrieved_3;
    QCheckBox *general_checkBox_initWMWithAllNodes;
    QLabel *label_retrieved_5;
    QCheckBox *general_checkBox_keepRawData;
    QLabel *label_retrieved_6;
    QLabel *label_retrieved_7;
    QCheckBox *general_checkBox_transferSortingByWeightId;
    QLabel *label_retrieved_8;
    QDoubleSpinBox *general_doubleSpinBox_laserScanVoxel;
    QGroupBox *groupBox_rehearsal2;
    QGridLayout *gridLayout_12;
    QCheckBox *general_checkBox_RehearsalIdUpdatedToNewOne;
    QLabel *label_similarity;
    QDoubleSpinBox *doubleSpinBox_similarityThreshold;
    QLabel *label_rehearsalIdUpdate;
    QLabel *label_rehearsalIdUpdate_2;
    QCheckBox *rgdb_rehearsalWeightIgnoredWhileMoving;
    QSpacerItem *verticalSpacer_6;
    QWidget *page_24;
    QVBoxLayout *verticalLayout_33;
    QGroupBox *groupBox_database2;
    QGridLayout *gridLayout_11;
    QCheckBox *general_checkBox_keepBinaryData;
    QLabel *label_keepRawData;
    QCheckBox *checkBox_dbInMemory;
    QSpinBox *spinBox_dbCacheSize;
    QLabel *label_75;
    QComboBox *comboBox_dbJournalMode;
    QLabel *label_77;
    QLabel *label_74;
    QLabel *label_89;
    QLabel *label_120;
    QComboBox *comboBox_dbSynchronous;
    QComboBox *comboBox_dbTempStore;
    QLabel *label_keepRehearsed;
    QCheckBox *general_checkBox_keepNotLinkedNodes;
    QSpacerItem *verticalSpacer_12;
    QWidget *page_9;
    QVBoxLayout *verticalLayout_58;
    QGroupBox *groupBox_signature_keypoint1;
    QVBoxLayout *verticalLayout_12;
    QGridLayout *gridLayout_41;
    QComboBox *comboBox_detector_strategy;
    QLabel *label_117;
    QDoubleSpinBox *surf_doubleSpinBox_maxDepth;
    QLabel *label_53;
    QSpinBox *surf_spinBox_wordsPerImageTarget;
    QLabel *label_22;
    QDoubleSpinBox *surf_doubleSpinBox_ratioBadSign;
    QLabel *label_81;
    QLineEdit *lineEdit_kp_roi;
    QLabel *label_101;
    QDoubleSpinBox *doubleSpinBox_kp_roi0;
    QLabel *label_97;
    QDoubleSpinBox *doubleSpinBox_kp_roi1;
    QLabel *label_98;
    QDoubleSpinBox *doubleSpinBox_kp_roi2;
    QLabel *label_99;
    QDoubleSpinBox *doubleSpinBox_kp_roi3;
    QLabel *label_100;
    QGroupBox *groupBox_dictionary_strategy2;
    QGridLayout *gridLayout_18;
    QToolButton *toolButton_dictionaryPath;
    QLabel *label_78;
    QDoubleSpinBox *surf_doubleSpinBox_nndrRatio;
    QLineEdit *lineEdit_dictionaryPath;
    QLabel *label_dictionaryPath;
    QCheckBox *checkBox_dictionary_incremental;
    QLabel *label_12;
    QComboBox *comboBox_dictionary_strategy;
    QLabel *label_incrementalDict;
    QLabel *label_94;
    QCheckBox *checkBox_kp_parallelized;
    QCheckBox *checkBox_kp_newWordsComparedTogether;
    QLabel *label_186;
    QGroupBox *groupBox_9;
    QVBoxLayout *verticalLayout_59;
    QLabel *label_35;
    QGridLayout *gridLayout_32;
    QSpinBox *subpix_winSize;
    QLabel *label_200;
    QSpinBox *subpix_iterations;
    QLabel *label_203;
    QDoubleSpinBox *subpix_eps;
    QLabel *label_204;
    QSpacerItem *verticalSpacer_15;
    QWidget *page_13;
    QVBoxLayout *verticalLayout_38;
    QGroupBox *groupBox_detector_surf2;
    QGridLayout *gridLayout_15;
    QCheckBox *surf_checkBox_upright;
    QLabel *label_41;
    QLabel *label_8;
    QLabel *label_surf_checkBox_gpuKeypointsRatio;
    QLabel *label_surf_checkBox_gpuVersion;
    QSpinBox *surf_spinBox_octaves;
    QCheckBox *surf_checkBox_gpuVersion;
    QLabel *label_49;
    QDoubleSpinBox *surf_doubleSpinBox_gpuKeypointsRatio;
    QDoubleSpinBox *surf_doubleSpinBox_hessianThr;
    QCheckBox *checkBox_surfExtended;
    QSpinBox *surf_spinBox_octaveLayers;
    QLabel *label_26;
    QLabel *label_4;
    QSpacerItem *verticalSpacer_17;
    QWidget *page_14;
    QVBoxLayout *verticalLayout_39;
    QGroupBox *groupBox_detector_sift2;
    QGridLayout *gridLayout_19;
    QSpinBox *sift_spinBox_nFeatures;
    QLabel *label_129;
    QDoubleSpinBox *sift_doubleSpinBox_contrastThr;
    QLabel *label_5;
    QLabel *label_30;
    QDoubleSpinBox *sift_doubleSpinBox_edgeThr;
    QDoubleSpinBox *sift_doubleSpinBox_sigma;
    QSpinBox *sift_spinBox_nOctaveLayers;
    QLabel *label_130;
    QLabel *label_131;
    QSpacerItem *verticalSpacer_19;
    QWidget *page_17;
    QVBoxLayout *verticalLayout_14;
    QGroupBox *groupBox_detector_fast2;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_2;
    QSpinBox *fastThreshold;
    QLabel *label_56;
    QCheckBox *fastSuppressNonMax;
    QLabel *label_69;
    QCheckBox *fastGpu;
    QLabel *label_fastGPU;
    QDoubleSpinBox *fastKeypointRatio;
    QLabel *label_fastGPUKptRatio;
    QSpacerItem *verticalSpacer_16;
    QWidget *page_15;
    QVBoxLayout *verticalLayout_40;
    QGroupBox *groupBox_detector_brief2;
    QGridLayout *gridLayout_16;
    QSpinBox *briefBytes;
    QLabel *label_52;
    QSpacerItem *verticalSpacer_22;
    QWidget *page_18;
    QVBoxLayout *verticalLayout_35;
    QGroupBox *groupBox_detector_orb2;
    QGridLayout *gridLayout_14;
    QDoubleSpinBox *doubleSpinBox_ORBScaleFactor;
    QLabel *label_32;
    QSpinBox *spinBox_ORBNLevels;
    QLabel *label_43;
    QSpinBox *spinBox_ORBEdgeThreshold;
    QLabel *label_95;
    QSpinBox *spinBox_ORBFirstLevel;
    QLabel *label_127;
    QSpinBox *spinBox_ORBWTA_K;
    QLabel *label_128;
    QSpinBox *spinBox_ORBScoreType;
    QLabel *label_140;
    QSpinBox *spinBox_ORBPatchSize;
    QLabel *label_147;
    QCheckBox *checkBox_ORBGpu;
    QLabel *label_orbGpu;
    QSpacerItem *verticalSpacer_18;
    QWidget *page_11;
    QVBoxLayout *verticalLayout_36;
    QGroupBox *groupBox_detector_freak2;
    QGridLayout *gridLayout_13;
    QCheckBox *checkBox_FREAKOrientationNormalized;
    QLabel *label_6;
    QCheckBox *checkBox_FREAKScaleNormalized;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_FREAKPatternScale;
    QLabel *label_13;
    QSpinBox *spinBox_FREAKNOctaves;
    QLabel *label_23;
    QSpacerItem *verticalSpacer_13;
    QWidget *page_16;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_detector_gftt2;
    QGridLayout *gridLayout_21;
    QDoubleSpinBox *doubleSpinBox_GFTT_qualityLevel;
    QDoubleSpinBox *doubleSpinBox_GFTT_minDistance;
    QCheckBox *checkBox_GFTT_useHarrisDetector;
    QLabel *label_172;
    QDoubleSpinBox *doubleSpinBox_GFTT_k;
    QLabel *label_173;
    QSpinBox *spinBox_GFTT_blockSize;
    QLabel *label_175;
    QLabel *label_176;
    QLabel *label_177;
    QSpacerItem *verticalSpacer_25;
    QWidget *page_25;
    QVBoxLayout *verticalLayout_47;
    QGroupBox *groupBox_detector_brisk2;
    QGridLayout *gridLayout_26;
    QLabel *label_191;
    QDoubleSpinBox *doubleSpinBox_BRISK_patterScale;
    QLabel *label_188;
    QSpinBox *spinBox_BRISK_octaves;
    QLabel *label_189;
    QSpinBox *spinBox_BRISK_thresh;
    QSpacerItem *verticalSpacer_26;
    QWidget *page_19;
    QVBoxLayout *verticalLayout_13;
    QGroupBox *groupBox_vh_strategy1;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout_10;
    QComboBox *comboBox_vh_strategy;
    QLabel *label_50;
    QGroupBox *groupBox_vh_epipolar2;
    QGridLayout *gridLayout;
    QSpinBox *surf_spinBox_matchCountMinAccepted;
    QLabel *label_25;
    QDoubleSpinBox *surf_doubleSpinBox_ransacParam1;
    QLabel *label_27;
    QDoubleSpinBox *surf_doubleSpinBox_ransacParam2;
    QLabel *label_28;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_10;
    QVBoxLayout *verticalLayout_19;
    QGroupBox *groupBox_slam1;
    QVBoxLayout *verticalLayout_25;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *general_checkBox_activateRGBD;
    QLabel *label_activateRGBD;
    QLabel *label_83;
    QGroupBox *groupBox_slam_update;
    QGridLayout *gridLayout_47;
    QDoubleSpinBox *rgdb_linearUpdate;
    QLabel *label_153;
    QDoubleSpinBox *rgdb_angularUpdate;
    QLabel *label_152;
    QDoubleSpinBox *rgdb_newMapOdomChange;
    QLabel *label_163;
    QCheckBox *odomScanHistory;
    QLabel *label_scanMatching;
    QLabel *label_scanMatching_3;
    QSpinBox *spinBox_maxLocalLocationsRetrieved;
    QDoubleSpinBox *localDetection_radius;
    QLabel *label_space2;
    QLabel *label_scanMatching_5;
    QDoubleSpinBox *rgdb_localImmunizationRatio;
    QGroupBox *groupBox_graphOptimization;
    QGridLayout *gridLayout_49;
    QComboBox *graphOptimization_type;
    QLabel *label_31;
    QSpinBox *graphOptimization_iterations;
    QCheckBox *graphOptimization_covarianceIgnored;
    QLabel *label_141;
    QCheckBox *graphOptimization_fromGraphEnd;
    QLabel *label_151;
    QFrame *line;
    QCheckBox *graphOptimization_slam2d;
    QLabel *label_164;
    QLabel *label_45;
    QLabel *label_183;
    QLabel *label_211;
    QGroupBox *groupBox_localDetection_time;
    QVBoxLayout *verticalLayout_21;
    QLabel *label_54;
    QGroupBox *groupBox_localDetection_space;
    QVBoxLayout *verticalLayout_48;
    QLabel *label_space1_2;
    QGridLayout *gridLayout_50;
    QDoubleSpinBox *localDetection_pathFilteringRadius;
    QSpinBox *localDetection_maxDiffID;
    QLabel *label_space3_2;
    QCheckBox *checkBox_localSpaceLinksKeptInWM;
    QLabel *label_scanMatching_2;
    QLabel *label_space3_3;
    QLabel *label_scanMatching_4;
    QCheckBox *checkBox_localSpacePathOdomPosesUsed;
    QGroupBox *groupBox_12;
    QVBoxLayout *verticalLayout_61;
    QLabel *label_space2_3;
    QGridLayout *gridLayout_51;
    QDoubleSpinBox *graphPlan_goalReachedRadius;
    QLabel *label_space2_2;
    QCheckBox *graphPlan_planWithNearNodesLinked;
    QLabel *label_space3_4;
    QLabel *label_space3_5;
    QCheckBox *graphPlan_goalsSavedInUserData;
    QSpacerItem *verticalSpacer_11;
    QWidget *page_21;
    QVBoxLayout *verticalLayout_50;
    QGroupBox *groupBox_visualTransform2;
    QVBoxLayout *verticalLayout_41;
    QLabel *label_16;
    QGridLayout *gridLayout_23;
    QSpinBox *loopClosure_bowMinInliers;
    QLabel *label_2;
    QDoubleSpinBox *loopClosure_bowInlierDistance;
    QLabel *label_14;
    QSpinBox *loopClosure_bowIterations;
    QLabel *label_15;
    QDoubleSpinBox *loopClosure_bowMaxDepth;
    QLabel *label_146;
    QCheckBox *loopClosure_bowForce2D;
    QLabel *label_184;
    QComboBox *globalDetection_icpType;
    QLabel *label_57;
    QLabel *label_658;
    QCheckBox *loopClosure_bowEpipolarGeometry;
    QLabel *label_664;
    QDoubleSpinBox *loopClosure_bowEpipolarGeometryVar;
    QGroupBox *groupBox_reextract;
    QVBoxLayout *verticalLayout_46;
    QLabel *label_182;
    QLabel *label_171;
    QLabel *label_174;
    QGridLayout *gridLayout_25;
    QComboBox *reextract_nn;
    QDoubleSpinBox *reextract_nndrRatio;
    QSpinBox *reextract_maxFeatures;
    QLabel *label_180;
    QLabel *label_179;
    QLabel *label_181;
    QComboBox *reextract_type;
    QLabel *label_178;
    QSpacerItem *verticalSpacer_20;
    QWidget *page_27;
    QVBoxLayout *verticalLayout_52;
    QGroupBox *groupBox_icp2;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_7;
    QGridLayout *gridLayout_48;
    QDoubleSpinBox *globalDetection_icpMaxTranslation;
    QLabel *label_51;
    QLabel *label_55;
    QDoubleSpinBox *globalDetection_icpMaxRotation;
    QGroupBox *groupBox_loopClosure_icp3;
    QGridLayout *gridLayout_53;
    QSpinBox *loopClosure_icpIterations;
    QLabel *label_124;
    QSpinBox *loopClosure_icpSamples;
    QSpinBox *loopClosure_icpDecimation;
    QDoubleSpinBox *loopClosure_icpMaxCorrespondenceDistance;
    QLabel *label_109;
    QDoubleSpinBox *loopClosure_icpMaxDepth;
    QLabel *label_115;
    QDoubleSpinBox *loopClosure_icpVoxelSize;
    QLabel *label_125;
    QLabel *label_118;
    QLabel *label_122;
    QDoubleSpinBox *loopClosure_icpRatio;
    QLabel *label_133;
    QCheckBox *loopClosure_icpPointToPlane;
    QLabel *label_144;
    QSpinBox *loopClosure_icpPointToPlaneNormals;
    QLabel *label_212;
    QGroupBox *groupBox_loopClosure_icp2;
    QGridLayout *gridLayout_52;
    QDoubleSpinBox *loopClosure_icp2MaxCorrespondenceDistance;
    QLabel *label_138;
    QSpinBox *loopClosure_icp2Iterations;
    QLabel *label_139;
    QDoubleSpinBox *loopClosure_icp2Ratio;
    QLabel *label_148;
    QDoubleSpinBox *loopClosure_icp2Voxel;
    QLabel *label_150;
    QSpacerItem *verticalSpacer_21;
    QWidget *page_29;
    QVBoxLayout *verticalLayout_57;
    QGroupBox *groupBox_stereo2;
    QVBoxLayout *verticalLayout_56;
    QLabel *label_187;
    QGroupBox *groupBox_10;
    QGridLayout *gridLayout_33;
    QSpinBox *stereo_flow_winSize;
    QLabel *label_205;
    QSpinBox *stereo_flow_iterations;
    QLabel *label_206;
    QDoubleSpinBox *stereo_flow_eps;
    QLabel *label_207;
    QLabel *label_208;
    QSpinBox *stereo_flow_maxLevel;
    QGridLayout *gridLayout_35;
    QDoubleSpinBox *stereo_maxSlope;
    QLabel *label_215;
    QSpacerItem *verticalSpacer_29;
    QWidget *page_12;
    QVBoxLayout *verticalLayout_32;
    QGroupBox *groupBox_odometry1;
    QVBoxLayout *verticalLayout_34;
    QLabel *label_114;
    QGridLayout *gridLayout_27;
    QLabel *label_160;
    QLabel *label_47;
    QComboBox *odom_strategy;
    QSpinBox *odom_countdown;
    QCheckBox *odom_fillInfoData;
    QComboBox *odom_type;
    QLabel *label_44;
    QLabel *label_103;
    QCheckBox *odom_force2D;
    QLabel *label_196;
    QLabel *label_221;
    QPushButton *pushButton_testOdometry;
    QLabel *label_161;
    QLabel *label_162;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_37;
    QSpinBox *odom_minInliers;
    QLabel *label_145;
    QDoubleSpinBox *odom_inlierDistance;
    QLabel *label_149;
    QSpinBox *odom_iterations;
    QLabel *label_36;
    QSpinBox *odom_refine_iterations;
    QLabel *label_111;
    QLabel *label_225;
    QDoubleSpinBox *odom_pnpReprojError;
    QCheckBox *odom_pnpEstimation;
    QLabel *label_226;
    QLabel *label_227;
    QComboBox *odom_pnpFlags;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_36;
    QSpinBox *odom_maxFeatures;
    QLabel *label_123;
    QLineEdit *lineEdit_odom_roi;
    QLabel *label_136;
    QLabel *label_104;
    QDoubleSpinBox *odom_maxDepth;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_60;
    QLabel *label_216;
    QGridLayout *gridLayout_34;
    QSpinBox *odom_subpix_winSize;
    QLabel *label_197;
    QSpinBox *odom_subpix_iterations;
    QLabel *label_198;
    QDoubleSpinBox *odom_subpix_eps;
    QLabel *label_214;
    QSpacerItem *verticalSpacer_14;
    QWidget *page_26;
    QVBoxLayout *verticalLayout_54;
    QGroupBox *groupBox_odometryBOW2;
    QGridLayout *gridLayout_29;
    QSpinBox *odom_localHistory;
    QLabel *label_190;
    QComboBox *odom_bin_nn;
    QLabel *label_201;
    QDoubleSpinBox *odom_bin_nndrRatio;
    QLabel *label_202;
    QSpacerItem *verticalSpacer_27;
    QWidget *page_28;
    QVBoxLayout *verticalLayout_55;
    QGroupBox *groupBox_odometryFlow2;
    QVBoxLayout *verticalLayout_53;
    QLabel *label_199;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_30;
    QSpinBox *odom_flow_winSize;
    QLabel *label_192;
    QSpinBox *odom_flow_iterations;
    QLabel *label_193;
    QLabel *label_194;
    QSpinBox *odom_flow_maxLevel;
    QLabel *label_195;
    QDoubleSpinBox *odom_flow_eps;
    QSpacerItem *verticalSpacer_28;
    QWidget *page_30;
    QVBoxLayout *verticalLayout_169;
    QGroupBox *groupBox_odometryMono2;
    QVBoxLayout *verticalLayout_168;
    QLabel *label_661;
    QGridLayout *gridLayout_166;
    QDoubleSpinBox *doubleSpinBox_minFlow;
    QLabel *label_659;
    QDoubleSpinBox *doubleSpinBox_minTranslation;
    QLabel *label_660;
    QLabel *label_662;
    QDoubleSpinBox *doubleSpinBox_minInitTranslation;
    QLabel *label_663;
    QDoubleSpinBox *doubleSpinBox_maxVariance;
    QSpacerItem *verticalSpacer_30;
    QHBoxLayout *horizontalLayout;
    QDialogButtonBox *buttonBox_local;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_basic;
    QRadioButton *radioButton_advanced;
    QDialogButtonBox *buttonBox_global;

    void setupUi(QDialog *preferencesDialog)
    {
        if (preferencesDialog->objectName().isEmpty())
            preferencesDialog->setObjectName(QString::fromUtf8("preferencesDialog"));
        preferencesDialog->resize(1058, 725);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(preferencesDialog->sizePolicy().hasHeightForWidth());
        preferencesDialog->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/RTAB-Map.ico"), QSize(), QIcon::Normal, QIcon::Off);
        preferencesDialog->setWindowIcon(icon);
        verticalLayout_26 = new QVBoxLayout(preferencesDialog);
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        treeView = new QTreeView(preferencesDialog);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setMinimumSize(QSize(160, 0));
        treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView->header()->setVisible(false);

        verticalLayout_18->addWidget(treeView);


        horizontalLayout_2->addLayout(verticalLayout_18);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(0);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        scrollArea = new QScrollArea(preferencesDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 755, 1557));
        verticalLayout_16 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_16->setSpacing(0);
        verticalLayout_16->setContentsMargins(0, 0, 0, 0);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setSpacing(0);
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        stackedWidget = new QStackedWidget(scrollAreaWidgetContents);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setFrameShadow(QFrame::Raised);
        page_22 = new QWidget();
        page_22->setObjectName(QString::fromUtf8("page_22"));
        verticalLayout_29 = new QVBoxLayout(page_22);
        verticalLayout_29->setObjectName(QString::fromUtf8("verticalLayout_29"));
        groupBox_generalSettingsGui0 = new QGroupBox(page_22);
        groupBox_generalSettingsGui0->setObjectName(QString::fromUtf8("groupBox_generalSettingsGui0"));
        verticalLayout_28 = new QVBoxLayout(groupBox_generalSettingsGui0);
        verticalLayout_28->setObjectName(QString::fromUtf8("verticalLayout_28"));
        gridLayout_39 = new QGridLayout();
        gridLayout_39->setObjectName(QString::fromUtf8("gridLayout_39"));
        general_checkBox_imagesKept = new QCheckBox(groupBox_generalSettingsGui0);
        general_checkBox_imagesKept->setObjectName(QString::fromUtf8("general_checkBox_imagesKept"));
        general_checkBox_imagesKept->setChecked(true);

        gridLayout_39->addWidget(general_checkBox_imagesKept, 0, 0, 1, 1);

        label_59 = new QLabel(groupBox_generalSettingsGui0);
        label_59->setObjectName(QString::fromUtf8("label_59"));
        label_59->setWordWrap(true);

        gridLayout_39->addWidget(label_59, 0, 1, 1, 1);

        checkBox_beep = new QCheckBox(groupBox_generalSettingsGui0);
        checkBox_beep->setObjectName(QString::fromUtf8("checkBox_beep"));

        gridLayout_39->addWidget(checkBox_beep, 1, 0, 1, 1);

        label_88 = new QLabel(groupBox_generalSettingsGui0);
        label_88->setObjectName(QString::fromUtf8("label_88"));
        label_88->setWordWrap(true);

        gridLayout_39->addWidget(label_88, 1, 1, 1, 1);

        label_102 = new QLabel(groupBox_generalSettingsGui0);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setWordWrap(true);

        gridLayout_39->addWidget(label_102, 2, 1, 1, 1);

        checkBox_notifyWhenNewGlobalPathIsReceived = new QCheckBox(groupBox_generalSettingsGui0);
        checkBox_notifyWhenNewGlobalPathIsReceived->setObjectName(QString::fromUtf8("checkBox_notifyWhenNewGlobalPathIsReceived"));
        checkBox_notifyWhenNewGlobalPathIsReceived->setChecked(true);

        gridLayout_39->addWidget(checkBox_notifyWhenNewGlobalPathIsReceived, 2, 0, 1, 1);

        gridLayout_39->setColumnStretch(1, 1);

        verticalLayout_28->addLayout(gridLayout_39);

        groupBox_5 = new QGroupBox(groupBox_generalSettingsGui0);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_31 = new QGridLayout(groupBox_5);
        gridLayout_31->setObjectName(QString::fromUtf8("gridLayout_31"));
        checkBox_imageRejectedShown = new QCheckBox(groupBox_5);
        checkBox_imageRejectedShown->setObjectName(QString::fromUtf8("checkBox_imageRejectedShown"));
        checkBox_imageRejectedShown->setChecked(true);

        gridLayout_31->addWidget(checkBox_imageRejectedShown, 1, 0, 1, 1);

        label_105 = new QLabel(groupBox_5);
        label_105->setObjectName(QString::fromUtf8("label_105"));
        label_105->setWordWrap(true);

        gridLayout_31->addWidget(label_105, 2, 1, 1, 1);

        checkBox_imageHighestHypShown = new QCheckBox(groupBox_5);
        checkBox_imageHighestHypShown->setObjectName(QString::fromUtf8("checkBox_imageHighestHypShown"));
        checkBox_imageHighestHypShown->setChecked(false);

        gridLayout_31->addWidget(checkBox_imageHighestHypShown, 2, 0, 1, 1);

        label_73 = new QLabel(groupBox_5);
        label_73->setObjectName(QString::fromUtf8("label_73"));
        label_73->setWordWrap(true);

        gridLayout_31->addWidget(label_73, 1, 1, 1, 1);

        label_70 = new QLabel(groupBox_5);
        label_70->setObjectName(QString::fromUtf8("label_70"));
        label_70->setWordWrap(false);

        gridLayout_31->addWidget(label_70, 0, 1, 1, 1);

        checkBox_verticalLayoutUsed = new QCheckBox(groupBox_5);
        checkBox_verticalLayoutUsed->setObjectName(QString::fromUtf8("checkBox_verticalLayoutUsed"));
        checkBox_verticalLayoutUsed->setChecked(true);

        gridLayout_31->addWidget(checkBox_verticalLayoutUsed, 0, 0, 1, 1);

        label_222 = new QLabel(groupBox_5);
        label_222->setObjectName(QString::fromUtf8("label_222"));
        label_222->setWordWrap(true);

        gridLayout_31->addWidget(label_222, 3, 1, 1, 1);

        checkBox_posteriorGraphView = new QCheckBox(groupBox_5);
        checkBox_posteriorGraphView->setObjectName(QString::fromUtf8("checkBox_posteriorGraphView"));
        checkBox_posteriorGraphView->setChecked(true);

        gridLayout_31->addWidget(checkBox_posteriorGraphView, 3, 0, 1, 1);

        gridLayout_31->setColumnStretch(1, 1);

        verticalLayout_28->addWidget(groupBox_5);

        groupBox = new QGroupBox(groupBox_generalSettingsGui0);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_38 = new QGridLayout(groupBox);
        gridLayout_38->setObjectName(QString::fromUtf8("gridLayout_38"));
        spinBox_odomQualityWarnThr = new QSpinBox(groupBox);
        spinBox_odomQualityWarnThr->setObjectName(QString::fromUtf8("spinBox_odomQualityWarnThr"));
        spinBox_odomQualityWarnThr->setMaximum(9999);
        spinBox_odomQualityWarnThr->setValue(50);

        gridLayout_38->addWidget(spinBox_odomQualityWarnThr, 0, 0, 1, 1);

        label_86 = new QLabel(groupBox);
        label_86->setObjectName(QString::fromUtf8("label_86"));
        label_86->setWordWrap(true);

        gridLayout_38->addWidget(label_86, 0, 1, 1, 1);

        gridLayout_38->setColumnStretch(1, 1);

        verticalLayout_28->addWidget(groupBox);

        groupBox_2 = new QGroupBox(groupBox_generalSettingsGui0);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_9 = new QHBoxLayout(groupBox_2);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        pushButton_loadConfig = new QPushButton(groupBox_2);
        pushButton_loadConfig->setObjectName(QString::fromUtf8("pushButton_loadConfig"));

        horizontalLayout_9->addWidget(pushButton_loadConfig);

        pushButton_saveConfig = new QPushButton(groupBox_2);
        pushButton_saveConfig->setObjectName(QString::fromUtf8("pushButton_saveConfig"));

        horizontalLayout_9->addWidget(pushButton_saveConfig);

        pushButton_resetConfig = new QPushButton(groupBox_2);
        pushButton_resetConfig->setObjectName(QString::fromUtf8("pushButton_resetConfig"));

        horizontalLayout_9->addWidget(pushButton_resetConfig);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_4);


        verticalLayout_28->addWidget(groupBox_2);


        verticalLayout_29->addWidget(groupBox_generalSettingsGui0);

        verticalSpacer_24 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_29->addItem(verticalSpacer_24);

        stackedWidget->addWidget(page_22);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_42 = new QVBoxLayout(page_2);
        verticalLayout_42->setObjectName(QString::fromUtf8("verticalLayout_42"));
        groupBox_cloudRendering1 = new QGroupBox(page_2);
        groupBox_cloudRendering1->setObjectName(QString::fromUtf8("groupBox_cloudRendering1"));
        verticalLayout_31 = new QVBoxLayout(groupBox_cloudRendering1);
        verticalLayout_31->setObjectName(QString::fromUtf8("verticalLayout_31"));
        groupBox_poseFiltering = new QGroupBox(groupBox_cloudRendering1);
        groupBox_poseFiltering->setObjectName(QString::fromUtf8("groupBox_poseFiltering"));
        groupBox_poseFiltering->setCheckable(true);
        groupBox_poseFiltering->setChecked(true);
        verticalLayout_37 = new QVBoxLayout(groupBox_poseFiltering);
        verticalLayout_37->setObjectName(QString::fromUtf8("verticalLayout_37"));
        label_33 = new QLabel(groupBox_poseFiltering);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setWordWrap(true);

        verticalLayout_37->addWidget(label_33);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        doubleSpinBox_cloudFilterRadius = new QDoubleSpinBox(groupBox_poseFiltering);
        doubleSpinBox_cloudFilterRadius->setObjectName(QString::fromUtf8("doubleSpinBox_cloudFilterRadius"));
        doubleSpinBox_cloudFilterRadius->setMinimum(0.01);
        doubleSpinBox_cloudFilterRadius->setValue(0.1);

        gridLayout_17->addWidget(doubleSpinBox_cloudFilterRadius, 0, 0, 1, 1);

        label_37 = new QLabel(groupBox_poseFiltering);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_17->addWidget(label_37, 0, 1, 1, 1);

        doubleSpinBox_cloudFilterAngle = new QDoubleSpinBox(groupBox_poseFiltering);
        doubleSpinBox_cloudFilterAngle->setObjectName(QString::fromUtf8("doubleSpinBox_cloudFilterAngle"));
        doubleSpinBox_cloudFilterAngle->setDecimals(0);
        doubleSpinBox_cloudFilterAngle->setMaximum(180);
        doubleSpinBox_cloudFilterAngle->setValue(30);

        gridLayout_17->addWidget(doubleSpinBox_cloudFilterAngle, 1, 0, 1, 1);

        label_48 = new QLabel(groupBox_poseFiltering);
        label_48->setObjectName(QString::fromUtf8("label_48"));

        gridLayout_17->addWidget(label_48, 1, 1, 1, 1);

        gridLayout_17->setColumnStretch(1, 1);

        verticalLayout_37->addLayout(gridLayout_17);


        verticalLayout_31->addWidget(groupBox_poseFiltering);

        groupBox_gridMap = new QGroupBox(groupBox_cloudRendering1);
        groupBox_gridMap->setObjectName(QString::fromUtf8("groupBox_gridMap"));
        groupBox_gridMap->setCheckable(false);
        groupBox_gridMap->setChecked(false);
        verticalLayout_45 = new QVBoxLayout(groupBox_gridMap);
        verticalLayout_45->setObjectName(QString::fromUtf8("verticalLayout_45"));
        label_167 = new QLabel(groupBox_gridMap);
        label_167->setObjectName(QString::fromUtf8("label_167"));
        label_167->setWordWrap(true);

        verticalLayout_45->addWidget(label_167);

        gridLayout_20 = new QGridLayout();
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        doubleSpinBox_map_opacity = new QDoubleSpinBox(groupBox_gridMap);
        doubleSpinBox_map_opacity->setObjectName(QString::fromUtf8("doubleSpinBox_map_opacity"));
        doubleSpinBox_map_opacity->setMinimum(0.75);
        doubleSpinBox_map_opacity->setMaximum(1);
        doubleSpinBox_map_opacity->setValue(0.75);

        gridLayout_20->addWidget(doubleSpinBox_map_opacity, 2, 0, 1, 1);

        checkBox_map_shown = new QCheckBox(groupBox_gridMap);
        checkBox_map_shown->setObjectName(QString::fromUtf8("checkBox_map_shown"));
        checkBox_map_shown->setChecked(false);

        gridLayout_20->addWidget(checkBox_map_shown, 0, 0, 1, 1);

        label_map_shown = new QLabel(groupBox_gridMap);
        label_map_shown->setObjectName(QString::fromUtf8("label_map_shown"));

        gridLayout_20->addWidget(label_map_shown, 0, 1, 1, 1);

        doubleSpinBox_map_resolution = new QDoubleSpinBox(groupBox_gridMap);
        doubleSpinBox_map_resolution->setObjectName(QString::fromUtf8("doubleSpinBox_map_resolution"));
        doubleSpinBox_map_resolution->setMinimum(0.01);
        doubleSpinBox_map_resolution->setMaximum(1);
        doubleSpinBox_map_resolution->setValue(0.05);

        gridLayout_20->addWidget(doubleSpinBox_map_resolution, 1, 0, 1, 1);

        label_159 = new QLabel(groupBox_gridMap);
        label_159->setObjectName(QString::fromUtf8("label_159"));

        gridLayout_20->addWidget(label_159, 1, 1, 1, 1);

        label_170 = new QLabel(groupBox_gridMap);
        label_170->setObjectName(QString::fromUtf8("label_170"));

        gridLayout_20->addWidget(label_170, 2, 1, 1, 1);

        checkBox_map_occupancyFrom3DCloud = new QCheckBox(groupBox_gridMap);
        checkBox_map_occupancyFrom3DCloud->setObjectName(QString::fromUtf8("checkBox_map_occupancyFrom3DCloud"));
        checkBox_map_occupancyFrom3DCloud->setChecked(false);

        gridLayout_20->addWidget(checkBox_map_occupancyFrom3DCloud, 3, 0, 1, 1);

        label_210 = new QLabel(groupBox_gridMap);
        label_210->setObjectName(QString::fromUtf8("label_210"));
        label_210->setWordWrap(true);

        gridLayout_20->addWidget(label_210, 3, 1, 1, 1);

        label_224 = new QLabel(groupBox_gridMap);
        label_224->setObjectName(QString::fromUtf8("label_224"));
        label_224->setWordWrap(true);

        gridLayout_20->addWidget(label_224, 4, 1, 1, 1);

        checkBox_map_erode = new QCheckBox(groupBox_gridMap);
        checkBox_map_erode->setObjectName(QString::fromUtf8("checkBox_map_erode"));
        checkBox_map_erode->setChecked(false);

        gridLayout_20->addWidget(checkBox_map_erode, 4, 0, 1, 1);

        gridLayout_20->setColumnStretch(1, 1);

        verticalLayout_45->addLayout(gridLayout_20);


        verticalLayout_31->addWidget(groupBox_gridMap);

        frame = new QFrame(groupBox_cloudRendering1);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        checkBox_showScans = new QCheckBox(frame);
        checkBox_showScans->setObjectName(QString::fromUtf8("checkBox_showScans"));
        checkBox_showScans->setChecked(true);

        gridLayout_2->addWidget(checkBox_showScans, 9, 0, 1, 1);

        spinBox_ptsize_scan = new QSpinBox(frame);
        spinBox_ptsize_scan->setObjectName(QString::fromUtf8("spinBox_ptsize_scan"));
        spinBox_ptsize_scan->setMinimum(1);
        spinBox_ptsize_scan->setMaximum(64);

        gridLayout_2->addWidget(spinBox_ptsize_scan, 11, 0, 1, 1);

        label_108 = new QLabel(frame);
        label_108->setObjectName(QString::fromUtf8("label_108"));
        label_108->setWordWrap(true);

        gridLayout_2->addWidget(label_108, 4, 2, 1, 1);

        label_154 = new QLabel(frame);
        label_154->setObjectName(QString::fromUtf8("label_154"));
        label_154->setAlignment(Qt::AlignCenter);
        label_154->setWordWrap(true);

        gridLayout_2->addWidget(label_154, 0, 0, 1, 1);

        label_137 = new QLabel(frame);
        label_137->setObjectName(QString::fromUtf8("label_137"));
        label_137->setAlignment(Qt::AlignCenter);
        label_137->setWordWrap(true);

        gridLayout_2->addWidget(label_137, 0, 1, 1, 1);

        checkBox_showClouds = new QCheckBox(frame);
        checkBox_showClouds->setObjectName(QString::fromUtf8("checkBox_showClouds"));
        checkBox_showClouds->setChecked(true);

        gridLayout_2->addWidget(checkBox_showClouds, 1, 0, 1, 1);

        checkBox_showOdomClouds = new QCheckBox(frame);
        checkBox_showOdomClouds->setObjectName(QString::fromUtf8("checkBox_showOdomClouds"));
        checkBox_showOdomClouds->setChecked(true);

        gridLayout_2->addWidget(checkBox_showOdomClouds, 1, 1, 1, 1);

        label_119 = new QLabel(frame);
        label_119->setObjectName(QString::fromUtf8("label_119"));
        label_119->setWordWrap(true);

        gridLayout_2->addWidget(label_119, 1, 2, 1, 1);

        doubleSpinBox_voxelSize = new QDoubleSpinBox(frame);
        doubleSpinBox_voxelSize->setObjectName(QString::fromUtf8("doubleSpinBox_voxelSize"));
        doubleSpinBox_voxelSize->setDecimals(3);
        doubleSpinBox_voxelSize->setMaximum(1);
        doubleSpinBox_voxelSize->setSingleStep(0.01);
        doubleSpinBox_voxelSize->setValue(0);

        gridLayout_2->addWidget(doubleSpinBox_voxelSize, 3, 0, 1, 1);

        doubleSpinBox_voxelSize_odom = new QDoubleSpinBox(frame);
        doubleSpinBox_voxelSize_odom->setObjectName(QString::fromUtf8("doubleSpinBox_voxelSize_odom"));
        doubleSpinBox_voxelSize_odom->setDecimals(3);
        doubleSpinBox_voxelSize_odom->setMaximum(1);
        doubleSpinBox_voxelSize_odom->setSingleStep(0.01);
        doubleSpinBox_voxelSize_odom->setValue(0);

        gridLayout_2->addWidget(doubleSpinBox_voxelSize_odom, 3, 1, 1, 1);

        label_107 = new QLabel(frame);
        label_107->setObjectName(QString::fromUtf8("label_107"));
        label_107->setWordWrap(true);

        gridLayout_2->addWidget(label_107, 3, 2, 1, 1);

        spinBox_decimation = new QSpinBox(frame);
        spinBox_decimation->setObjectName(QString::fromUtf8("spinBox_decimation"));
        spinBox_decimation->setMinimum(1);
        spinBox_decimation->setMaximum(32);
        spinBox_decimation->setValue(4);

        gridLayout_2->addWidget(spinBox_decimation, 4, 0, 1, 1);

        spinBox_decimation_odom = new QSpinBox(frame);
        spinBox_decimation_odom->setObjectName(QString::fromUtf8("spinBox_decimation_odom"));
        spinBox_decimation_odom->setMinimum(1);
        spinBox_decimation_odom->setMaximum(32);
        spinBox_decimation_odom->setValue(2);

        gridLayout_2->addWidget(spinBox_decimation_odom, 4, 1, 1, 1);

        doubleSpinBox_maxDepth = new QDoubleSpinBox(frame);
        doubleSpinBox_maxDepth->setObjectName(QString::fromUtf8("doubleSpinBox_maxDepth"));
        doubleSpinBox_maxDepth->setDecimals(1);
        doubleSpinBox_maxDepth->setMaximum(100);
        doubleSpinBox_maxDepth->setSingleStep(0.1);
        doubleSpinBox_maxDepth->setValue(4);

        gridLayout_2->addWidget(doubleSpinBox_maxDepth, 5, 0, 1, 1);

        doubleSpinBox_maxDepth_odom = new QDoubleSpinBox(frame);
        doubleSpinBox_maxDepth_odom->setObjectName(QString::fromUtf8("doubleSpinBox_maxDepth_odom"));
        doubleSpinBox_maxDepth_odom->setDecimals(1);
        doubleSpinBox_maxDepth_odom->setMaximum(100);
        doubleSpinBox_maxDepth_odom->setSingleStep(0.1);
        doubleSpinBox_maxDepth_odom->setValue(0);

        gridLayout_2->addWidget(doubleSpinBox_maxDepth_odom, 5, 1, 1, 1);

        label_132 = new QLabel(frame);
        label_132->setObjectName(QString::fromUtf8("label_132"));
        label_132->setWordWrap(true);

        gridLayout_2->addWidget(label_132, 5, 2, 1, 1);

        doubleSpinBox_opacity = new QDoubleSpinBox(frame);
        doubleSpinBox_opacity->setObjectName(QString::fromUtf8("doubleSpinBox_opacity"));
        doubleSpinBox_opacity->setDecimals(2);
        doubleSpinBox_opacity->setMaximum(1);
        doubleSpinBox_opacity->setSingleStep(0.1);
        doubleSpinBox_opacity->setValue(1);

        gridLayout_2->addWidget(doubleSpinBox_opacity, 6, 0, 1, 1);

        doubleSpinBox_opacity_odom = new QDoubleSpinBox(frame);
        doubleSpinBox_opacity_odom->setObjectName(QString::fromUtf8("doubleSpinBox_opacity_odom"));
        doubleSpinBox_opacity_odom->setDecimals(2);
        doubleSpinBox_opacity_odom->setMaximum(1);
        doubleSpinBox_opacity_odom->setSingleStep(0.1);
        doubleSpinBox_opacity_odom->setValue(1);

        gridLayout_2->addWidget(doubleSpinBox_opacity_odom, 6, 1, 1, 1);

        label_155 = new QLabel(frame);
        label_155->setObjectName(QString::fromUtf8("label_155"));
        label_155->setWordWrap(true);

        gridLayout_2->addWidget(label_155, 6, 2, 1, 1);

        spinBox_ptsize = new QSpinBox(frame);
        spinBox_ptsize->setObjectName(QString::fromUtf8("spinBox_ptsize"));
        spinBox_ptsize->setMinimum(1);
        spinBox_ptsize->setMaximum(64);

        gridLayout_2->addWidget(spinBox_ptsize, 7, 0, 1, 1);

        spinBox_ptsize_odom = new QSpinBox(frame);
        spinBox_ptsize_odom->setObjectName(QString::fromUtf8("spinBox_ptsize_odom"));
        spinBox_ptsize_odom->setMinimum(1);
        spinBox_ptsize_odom->setMaximum(64);

        gridLayout_2->addWidget(spinBox_ptsize_odom, 7, 1, 1, 1);

        label_157 = new QLabel(frame);
        label_157->setObjectName(QString::fromUtf8("label_157"));
        label_157->setWordWrap(true);

        gridLayout_2->addWidget(label_157, 7, 2, 1, 1);

        checkBox_showOdomScans = new QCheckBox(frame);
        checkBox_showOdomScans->setObjectName(QString::fromUtf8("checkBox_showOdomScans"));
        checkBox_showOdomScans->setChecked(true);

        gridLayout_2->addWidget(checkBox_showOdomScans, 9, 1, 1, 1);

        label_110 = new QLabel(frame);
        label_110->setObjectName(QString::fromUtf8("label_110"));
        label_110->setWordWrap(true);

        gridLayout_2->addWidget(label_110, 9, 2, 1, 1);

        doubleSpinBox_opacity_scan = new QDoubleSpinBox(frame);
        doubleSpinBox_opacity_scan->setObjectName(QString::fromUtf8("doubleSpinBox_opacity_scan"));
        doubleSpinBox_opacity_scan->setDecimals(2);
        doubleSpinBox_opacity_scan->setMaximum(1);
        doubleSpinBox_opacity_scan->setSingleStep(0.1);
        doubleSpinBox_opacity_scan->setValue(1);

        gridLayout_2->addWidget(doubleSpinBox_opacity_scan, 10, 0, 1, 1);

        checkBox_meshing = new QCheckBox(frame);
        checkBox_meshing->setObjectName(QString::fromUtf8("checkBox_meshing"));

        gridLayout_2->addWidget(checkBox_meshing, 13, 0, 1, 1);

        doubleSpinBox_opacity_odom_scan = new QDoubleSpinBox(frame);
        doubleSpinBox_opacity_odom_scan->setObjectName(QString::fromUtf8("doubleSpinBox_opacity_odom_scan"));
        doubleSpinBox_opacity_odom_scan->setDecimals(2);
        doubleSpinBox_opacity_odom_scan->setMaximum(1);
        doubleSpinBox_opacity_odom_scan->setSingleStep(0.1);
        doubleSpinBox_opacity_odom_scan->setValue(1);

        gridLayout_2->addWidget(doubleSpinBox_opacity_odom_scan, 10, 1, 1, 1);

        label_156 = new QLabel(frame);
        label_156->setObjectName(QString::fromUtf8("label_156"));
        label_156->setWordWrap(true);

        gridLayout_2->addWidget(label_156, 10, 2, 1, 1);

        spinBox_ptsize_odom_scan = new QSpinBox(frame);
        spinBox_ptsize_odom_scan->setObjectName(QString::fromUtf8("spinBox_ptsize_odom_scan"));
        spinBox_ptsize_odom_scan->setMinimum(1);
        spinBox_ptsize_odom_scan->setMaximum(64);

        gridLayout_2->addWidget(spinBox_ptsize_odom_scan, 11, 1, 1, 1);

        label_158 = new QLabel(frame);
        label_158->setObjectName(QString::fromUtf8("label_158"));
        label_158->setWordWrap(true);

        gridLayout_2->addWidget(label_158, 11, 2, 1, 1);

        label_168 = new QLabel(frame);
        label_168->setObjectName(QString::fromUtf8("label_168"));
        label_168->setWordWrap(true);

        gridLayout_2->addWidget(label_168, 15, 2, 1, 1);

        label_85 = new QLabel(frame);
        label_85->setObjectName(QString::fromUtf8("label_85"));
        label_85->setWordWrap(true);

        gridLayout_2->addWidget(label_85, 17, 2, 1, 1);

        doubleSpinBox_gp3Radius = new QDoubleSpinBox(frame);
        doubleSpinBox_gp3Radius->setObjectName(QString::fromUtf8("doubleSpinBox_gp3Radius"));
        doubleSpinBox_gp3Radius->setDecimals(3);
        doubleSpinBox_gp3Radius->setMaximum(1);
        doubleSpinBox_gp3Radius->setSingleStep(0.01);
        doubleSpinBox_gp3Radius->setValue(0.04);

        gridLayout_2->addWidget(doubleSpinBox_gp3Radius, 15, 0, 1, 1);

        doubleSpinBox_mlsRadius = new QDoubleSpinBox(frame);
        doubleSpinBox_mlsRadius->setObjectName(QString::fromUtf8("doubleSpinBox_mlsRadius"));
        doubleSpinBox_mlsRadius->setDecimals(3);
        doubleSpinBox_mlsRadius->setMaximum(1);
        doubleSpinBox_mlsRadius->setSingleStep(0.01);
        doubleSpinBox_mlsRadius->setValue(0.04);

        gridLayout_2->addWidget(doubleSpinBox_mlsRadius, 18, 0, 1, 1);

        label_169 = new QLabel(frame);
        label_169->setObjectName(QString::fromUtf8("label_169"));
        label_169->setWordWrap(true);

        gridLayout_2->addWidget(label_169, 13, 2, 1, 1);

        label_87 = new QLabel(frame);
        label_87->setObjectName(QString::fromUtf8("label_87"));
        label_87->setWordWrap(true);

        gridLayout_2->addWidget(label_87, 18, 2, 1, 1);

        checkBox_mls = new QCheckBox(frame);
        checkBox_mls->setObjectName(QString::fromUtf8("checkBox_mls"));
        checkBox_mls->setChecked(false);

        gridLayout_2->addWidget(checkBox_mls, 17, 0, 1, 1);

        label_71 = new QLabel(frame);
        label_71->setObjectName(QString::fromUtf8("label_71"));
        label_71->setWordWrap(true);

        gridLayout_2->addWidget(label_71, 14, 2, 1, 1);

        spinBox_normalKSearch = new QSpinBox(frame);
        spinBox_normalKSearch->setObjectName(QString::fromUtf8("spinBox_normalKSearch"));
        spinBox_normalKSearch->setValue(20);

        gridLayout_2->addWidget(spinBox_normalKSearch, 14, 0, 1, 1);

        label_213 = new QLabel(frame);
        label_213->setObjectName(QString::fromUtf8("label_213"));
        label_213->setWordWrap(true);

        gridLayout_2->addWidget(label_213, 12, 2, 1, 1);

        checkBox_showGraphs = new QCheckBox(frame);
        checkBox_showGraphs->setObjectName(QString::fromUtf8("checkBox_showGraphs"));
        checkBox_showGraphs->setChecked(true);

        gridLayout_2->addWidget(checkBox_showGraphs, 12, 0, 1, 1);

        gridLayout_2->setColumnStretch(2, 1);

        verticalLayout_31->addWidget(frame);


        verticalLayout_42->addWidget(groupBox_cloudRendering1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_42->addItem(verticalSpacer);

        stackedWidget->addWidget(page_2);
        page_8 = new QWidget();
        page_8->setObjectName(QString::fromUtf8("page_8"));
        verticalLayout_43 = new QVBoxLayout(page_8);
        verticalLayout_43->setObjectName(QString::fromUtf8("verticalLayout_43"));
        groupBox_logging1 = new QGroupBox(page_8);
        groupBox_logging1->setObjectName(QString::fromUtf8("groupBox_logging1"));
        gridLayout_40 = new QGridLayout(groupBox_logging1);
        gridLayout_40->setObjectName(QString::fromUtf8("gridLayout_40"));
        comboBox_loggerLevel = new QComboBox(groupBox_logging1);
        comboBox_loggerLevel->setObjectName(QString::fromUtf8("comboBox_loggerLevel"));
        comboBox_loggerLevel->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_40->addWidget(comboBox_loggerLevel, 0, 0, 1, 1);

        label_60 = new QLabel(groupBox_logging1);
        label_60->setObjectName(QString::fromUtf8("label_60"));
        label_60->setWordWrap(true);

        gridLayout_40->addWidget(label_60, 0, 1, 1, 1);

        comboBox_loggerEventLevel = new QComboBox(groupBox_logging1);
        comboBox_loggerEventLevel->setObjectName(QString::fromUtf8("comboBox_loggerEventLevel"));
        comboBox_loggerEventLevel->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_40->addWidget(comboBox_loggerEventLevel, 1, 0, 1, 1);

        label_61 = new QLabel(groupBox_logging1);
        label_61->setObjectName(QString::fromUtf8("label_61"));
        label_61->setWordWrap(true);

        gridLayout_40->addWidget(label_61, 1, 1, 1, 1);

        comboBox_loggerPauseLevel = new QComboBox(groupBox_logging1);
        comboBox_loggerPauseLevel->setObjectName(QString::fromUtf8("comboBox_loggerPauseLevel"));
        comboBox_loggerPauseLevel->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_40->addWidget(comboBox_loggerPauseLevel, 2, 0, 1, 1);

        label_62 = new QLabel(groupBox_logging1);
        label_62->setObjectName(QString::fromUtf8("label_62"));
        label_62->setWordWrap(true);

        gridLayout_40->addWidget(label_62, 2, 1, 1, 1);

        checkBox_logger_printTime = new QCheckBox(groupBox_logging1);
        checkBox_logger_printTime->setObjectName(QString::fromUtf8("checkBox_logger_printTime"));
        checkBox_logger_printTime->setChecked(true);

        gridLayout_40->addWidget(checkBox_logger_printTime, 3, 0, 1, 1);

        label_67 = new QLabel(groupBox_logging1);
        label_67->setObjectName(QString::fromUtf8("label_67"));
        label_67->setWordWrap(true);

        gridLayout_40->addWidget(label_67, 3, 1, 1, 1);

        comboBox_loggerType = new QComboBox(groupBox_logging1);
        comboBox_loggerType->setObjectName(QString::fromUtf8("comboBox_loggerType"));
        comboBox_loggerType->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_40->addWidget(comboBox_loggerType, 4, 0, 1, 1);

        label_68 = new QLabel(groupBox_logging1);
        label_68->setObjectName(QString::fromUtf8("label_68"));
        label_68->setWordWrap(true);

        gridLayout_40->addWidget(label_68, 4, 1, 1, 1);

        gridLayout_40->setColumnStretch(1, 1);

        verticalLayout_43->addWidget(groupBox_logging1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_43->addItem(verticalSpacer_2);

        stackedWidget->addWidget(page_8);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        verticalLayout_27 = new QVBoxLayout(page_4);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        groupBox_source0 = new QGroupBox(page_4);
        groupBox_source0->setObjectName(QString::fromUtf8("groupBox_source0"));
        verticalLayout_7 = new QVBoxLayout(groupBox_source0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        gridLayout_28 = new QGridLayout();
        gridLayout_28->setObjectName(QString::fromUtf8("gridLayout_28"));
        general_doubleSpinBox_imgRate = new QDoubleSpinBox(groupBox_source0);
        general_doubleSpinBox_imgRate->setObjectName(QString::fromUtf8("general_doubleSpinBox_imgRate"));
        general_doubleSpinBox_imgRate->setDecimals(1);
        general_doubleSpinBox_imgRate->setMaximum(100);
        general_doubleSpinBox_imgRate->setSingleStep(0.1);
        general_doubleSpinBox_imgRate->setValue(30);

        gridLayout_28->addWidget(general_doubleSpinBox_imgRate, 0, 0, 1, 1);

        label_3 = new QLabel(groupBox_source0);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_28->addWidget(label_3, 0, 1, 1, 1);

        label_17 = new QLabel(groupBox_source0);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_28->addWidget(label_17, 1, 1, 1, 1);

        source_mirroring = new QCheckBox(groupBox_source0);
        source_mirroring->setObjectName(QString::fromUtf8("source_mirroring"));

        gridLayout_28->addWidget(source_mirroring, 1, 0, 1, 1);

        gridLayout_28->setColumnStretch(1, 1);

        verticalLayout_7->addLayout(gridLayout_28);

        groupBox_sourceImage = new QGroupBox(groupBox_source0);
        groupBox_sourceImage->setObjectName(QString::fromUtf8("groupBox_sourceImage"));
        groupBox_sourceImage->setCheckable(true);
        groupBox_sourceImage->setChecked(false);
        verticalLayout_5 = new QVBoxLayout(groupBox_sourceImage);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        source_comboBox_image_type = new QComboBox(groupBox_sourceImage);
        source_comboBox_image_type->setObjectName(QString::fromUtf8("source_comboBox_image_type"));

        gridLayout_5->addWidget(source_comboBox_image_type, 0, 0, 1, 1);

        label_20 = new QLabel(groupBox_sourceImage);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setWordWrap(true);

        gridLayout_5->addWidget(label_20, 0, 1, 1, 1);

        label_10 = new QLabel(groupBox_sourceImage);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setWordWrap(true);

        gridLayout_5->addWidget(label_10, 1, 1, 1, 1);

        label_11 = new QLabel(groupBox_sourceImage);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setWordWrap(true);

        gridLayout_5->addWidget(label_11, 2, 1, 1, 1);

        source_spinBox_imgWidth = new QSpinBox(groupBox_sourceImage);
        source_spinBox_imgWidth->setObjectName(QString::fromUtf8("source_spinBox_imgWidth"));
        source_spinBox_imgWidth->setMinimum(0);
        source_spinBox_imgWidth->setMaximum(999999999);
        source_spinBox_imgWidth->setSingleStep(160);
        source_spinBox_imgWidth->setValue(0);

        gridLayout_5->addWidget(source_spinBox_imgWidth, 1, 0, 1, 1);

        source_spinBox_imgheight = new QSpinBox(groupBox_sourceImage);
        source_spinBox_imgheight->setObjectName(QString::fromUtf8("source_spinBox_imgheight"));
        source_spinBox_imgheight->setMinimum(0);
        source_spinBox_imgheight->setMaximum(999999999);
        source_spinBox_imgheight->setSingleStep(120);
        source_spinBox_imgheight->setValue(0);

        gridLayout_5->addWidget(source_spinBox_imgheight, 2, 0, 1, 1);

        gridLayout_5->setColumnStretch(1, 1);

        verticalLayout_5->addLayout(gridLayout_5);

        stackedWidget_image = new QStackedWidget(groupBox_sourceImage);
        stackedWidget_image->setObjectName(QString::fromUtf8("stackedWidget_image"));
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        verticalLayout_30 = new QVBoxLayout(page_7);
        verticalLayout_30->setObjectName(QString::fromUtf8("verticalLayout_30"));
        source_groupBox_usbDevice = new QGroupBox(page_7);
        source_groupBox_usbDevice->setObjectName(QString::fromUtf8("source_groupBox_usbDevice"));
        gridLayout_6 = new QGridLayout(source_groupBox_usbDevice);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_38 = new QLabel(source_groupBox_usbDevice);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_6->addWidget(label_38, 0, 1, 1, 1);

        source_usbDevice_spinBox_id = new QSpinBox(source_groupBox_usbDevice);
        source_usbDevice_spinBox_id->setObjectName(QString::fromUtf8("source_usbDevice_spinBox_id"));
        source_usbDevice_spinBox_id->setMinimum(-1);
        source_usbDevice_spinBox_id->setMaximum(999999999);
        source_usbDevice_spinBox_id->setValue(0);

        gridLayout_6->addWidget(source_usbDevice_spinBox_id, 0, 0, 1, 1);

        gridLayout_6->setColumnStretch(1, 1);

        verticalLayout_30->addWidget(source_groupBox_usbDevice);

        verticalSpacer_8 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_30->addItem(verticalSpacer_8);

        stackedWidget_image->addWidget(page_7);
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        verticalLayout_6 = new QVBoxLayout(page_5);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        source_groupBox_images = new QGroupBox(page_5);
        source_groupBox_images->setObjectName(QString::fromUtf8("source_groupBox_images"));
        gridLayout_7 = new QGridLayout(source_groupBox_images);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        source_images_toolButton_selectSource = new QToolButton(source_groupBox_images);
        source_images_toolButton_selectSource->setObjectName(QString::fromUtf8("source_images_toolButton_selectSource"));

        gridLayout_7->addWidget(source_images_toolButton_selectSource, 0, 0, 1, 1);

        source_images_lineEdit_path = new QLineEdit(source_groupBox_images);
        source_images_lineEdit_path->setObjectName(QString::fromUtf8("source_images_lineEdit_path"));
        source_images_lineEdit_path->setReadOnly(false);

        gridLayout_7->addWidget(source_images_lineEdit_path, 0, 1, 1, 1);

        source_images_spinBox_startPos = new QSpinBox(source_groupBox_images);
        source_images_spinBox_startPos->setObjectName(QString::fromUtf8("source_images_spinBox_startPos"));
        source_images_spinBox_startPos->setMinimum(0);
        source_images_spinBox_startPos->setMaximum(999999999);

        gridLayout_7->addWidget(source_images_spinBox_startPos, 1, 0, 1, 1);

        label_21 = new QLabel(source_groupBox_images);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_7->addWidget(label_21, 1, 1, 1, 1);

        source_images_refreshDir = new QCheckBox(source_groupBox_images);
        source_images_refreshDir->setObjectName(QString::fromUtf8("source_images_refreshDir"));

        gridLayout_7->addWidget(source_images_refreshDir, 2, 0, 1, 1);

        label_34 = new QLabel(source_groupBox_images);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_7->addWidget(label_34, 2, 1, 1, 1);

        gridLayout_7->setColumnStretch(1, 10);

        verticalLayout_6->addWidget(source_groupBox_images);

        verticalSpacer_9 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_9);

        stackedWidget_image->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QString::fromUtf8("page_6"));
        verticalLayout_8 = new QVBoxLayout(page_6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        source_groupBox_video = new QGroupBox(page_6);
        source_groupBox_video->setObjectName(QString::fromUtf8("source_groupBox_video"));
        gridLayout_8 = new QGridLayout(source_groupBox_video);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        source_video_toolButton_selectSource = new QToolButton(source_groupBox_video);
        source_video_toolButton_selectSource->setObjectName(QString::fromUtf8("source_video_toolButton_selectSource"));

        gridLayout_8->addWidget(source_video_toolButton_selectSource, 0, 0, 1, 1);

        source_video_lineEdit_path = new QLineEdit(source_groupBox_video);
        source_video_lineEdit_path->setObjectName(QString::fromUtf8("source_video_lineEdit_path"));
        source_video_lineEdit_path->setReadOnly(false);

        gridLayout_8->addWidget(source_video_lineEdit_path, 0, 1, 1, 1);

        gridLayout_8->setColumnStretch(1, 10);

        verticalLayout_8->addWidget(source_groupBox_video);

        verticalSpacer_10 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_10);

        stackedWidget_image->addWidget(page_6);

        verticalLayout_5->addWidget(stackedWidget_image);


        verticalLayout_7->addWidget(groupBox_sourceImage);

        groupBox_sourceDatabase = new QGroupBox(groupBox_source0);
        groupBox_sourceDatabase->setObjectName(QString::fromUtf8("groupBox_sourceDatabase"));
        groupBox_sourceDatabase->setCheckable(true);
        groupBox_sourceDatabase->setChecked(false);
        gridLayout_9 = new QGridLayout(groupBox_sourceDatabase);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        toolButton_dbViewer = new QToolButton(groupBox_sourceDatabase);
        toolButton_dbViewer->setObjectName(QString::fromUtf8("toolButton_dbViewer"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/mag_glass.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_dbViewer->setIcon(icon1);

        gridLayout_9->addWidget(toolButton_dbViewer, 0, 2, 1, 1);

        source_checkBox_ignoreOdometry = new QCheckBox(groupBox_sourceDatabase);
        source_checkBox_ignoreOdometry->setObjectName(QString::fromUtf8("source_checkBox_ignoreOdometry"));

        gridLayout_9->addWidget(source_checkBox_ignoreOdometry, 1, 0, 1, 1);

        source_database_toolButton_selectSource = new QToolButton(groupBox_sourceDatabase);
        source_database_toolButton_selectSource->setObjectName(QString::fromUtf8("source_database_toolButton_selectSource"));

        gridLayout_9->addWidget(source_database_toolButton_selectSource, 0, 0, 1, 1);

        source_database_lineEdit_path = new QLineEdit(groupBox_sourceDatabase);
        source_database_lineEdit_path->setObjectName(QString::fromUtf8("source_database_lineEdit_path"));

        gridLayout_9->addWidget(source_database_lineEdit_path, 0, 1, 1, 1);

        label_58 = new QLabel(groupBox_sourceDatabase);
        label_58->setObjectName(QString::fromUtf8("label_58"));

        gridLayout_9->addWidget(label_58, 4, 1, 1, 1);

        source_spinBox_databaseStartPos = new QSpinBox(groupBox_sourceDatabase);
        source_spinBox_databaseStartPos->setObjectName(QString::fromUtf8("source_spinBox_databaseStartPos"));
        source_spinBox_databaseStartPos->setMinimum(0);
        source_spinBox_databaseStartPos->setMaximum(999999999);

        gridLayout_9->addWidget(source_spinBox_databaseStartPos, 4, 0, 1, 1);

        label_72 = new QLabel(groupBox_sourceDatabase);
        label_72->setObjectName(QString::fromUtf8("label_72"));
        label_72->setWordWrap(true);

        gridLayout_9->addWidget(label_72, 1, 1, 1, 1);

        label_80 = new QLabel(groupBox_sourceDatabase);
        label_80->setObjectName(QString::fromUtf8("label_80"));
        label_80->setWordWrap(true);

        gridLayout_9->addWidget(label_80, 3, 1, 1, 1);

        source_checkBox_ignoreGoalDelay = new QCheckBox(groupBox_sourceDatabase);
        source_checkBox_ignoreGoalDelay->setObjectName(QString::fromUtf8("source_checkBox_ignoreGoalDelay"));

        gridLayout_9->addWidget(source_checkBox_ignoreGoalDelay, 3, 0, 1, 1);

        label_90 = new QLabel(groupBox_sourceDatabase);
        label_90->setObjectName(QString::fromUtf8("label_90"));
        label_90->setWordWrap(true);

        gridLayout_9->addWidget(label_90, 2, 1, 1, 1);

        source_checkBox_useDbStamps = new QCheckBox(groupBox_sourceDatabase);
        source_checkBox_useDbStamps->setObjectName(QString::fromUtf8("source_checkBox_useDbStamps"));

        gridLayout_9->addWidget(source_checkBox_useDbStamps, 2, 0, 1, 1);


        verticalLayout_7->addWidget(groupBox_sourceDatabase);

        groupBox_sourceOpenni = new QGroupBox(groupBox_source0);
        groupBox_sourceOpenni->setObjectName(QString::fromUtf8("groupBox_sourceOpenni"));
        groupBox_sourceOpenni->setCheckable(true);
        groupBox_sourceOpenni->setChecked(true);
        verticalLayout_11 = new QVBoxLayout(groupBox_sourceOpenni);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        label_40 = new QLabel(groupBox_sourceOpenni);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setWordWrap(true);

        verticalLayout_11->addWidget(label_40);

        gridLayout_55 = new QGridLayout();
        gridLayout_55->setObjectName(QString::fromUtf8("gridLayout_55"));
        comboBox_cameraRGBD = new QComboBox(groupBox_sourceOpenni);
        comboBox_cameraRGBD->setObjectName(QString::fromUtf8("comboBox_cameraRGBD"));
        comboBox_cameraRGBD->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_55->addWidget(comboBox_cameraRGBD, 0, 1, 1, 1);

        pushButton_calibrate = new QPushButton(groupBox_sourceOpenni);
        pushButton_calibrate->setObjectName(QString::fromUtf8("pushButton_calibrate"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_calibrate->sizePolicy().hasHeightForWidth());
        pushButton_calibrate->setSizePolicy(sizePolicy1);

        gridLayout_55->addWidget(pushButton_calibrate, 1, 1, 1, 1);

        label_46 = new QLabel(groupBox_sourceOpenni);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setWordWrap(true);

        gridLayout_55->addWidget(label_46, 1, 2, 1, 1);

        pushButton_test_rgbd_camera = new QPushButton(groupBox_sourceOpenni);
        pushButton_test_rgbd_camera->setObjectName(QString::fromUtf8("pushButton_test_rgbd_camera"));
        sizePolicy1.setHeightForWidth(pushButton_test_rgbd_camera->sizePolicy().hasHeightForWidth());
        pushButton_test_rgbd_camera->setSizePolicy(sizePolicy1);

        gridLayout_55->addWidget(pushButton_test_rgbd_camera, 2, 1, 1, 1);

        label_228 = new QLabel(groupBox_sourceOpenni);
        label_228->setObjectName(QString::fromUtf8("label_228"));
        label_228->setWordWrap(true);

        gridLayout_55->addWidget(label_228, 0, 2, 1, 1);

        gridLayout_55->setColumnStretch(2, 1);

        verticalLayout_11->addLayout(gridLayout_55);

        groupBox_openni2 = new QGroupBox(groupBox_sourceOpenni);
        groupBox_openni2->setObjectName(QString::fromUtf8("groupBox_openni2"));
        gridLayout_54 = new QGridLayout(groupBox_openni2);
        gridLayout_54->setObjectName(QString::fromUtf8("gridLayout_54"));
        openni2_autoWhiteBalance = new QCheckBox(groupBox_openni2);
        openni2_autoWhiteBalance->setObjectName(QString::fromUtf8("openni2_autoWhiteBalance"));
        openni2_autoWhiteBalance->setChecked(true);

        gridLayout_54->addWidget(openni2_autoWhiteBalance, 0, 0, 1, 1);

        label_217 = new QLabel(groupBox_openni2);
        label_217->setObjectName(QString::fromUtf8("label_217"));
        label_217->setWordWrap(true);

        gridLayout_54->addWidget(label_217, 0, 1, 1, 1);

        openni2_autoExposure = new QCheckBox(groupBox_openni2);
        openni2_autoExposure->setObjectName(QString::fromUtf8("openni2_autoExposure"));
        openni2_autoExposure->setChecked(true);

        gridLayout_54->addWidget(openni2_autoExposure, 1, 0, 1, 1);

        label_218 = new QLabel(groupBox_openni2);
        label_218->setObjectName(QString::fromUtf8("label_218"));
        label_218->setWordWrap(true);

        gridLayout_54->addWidget(label_218, 1, 1, 1, 1);

        openni2_exposure = new QSpinBox(groupBox_openni2);
        openni2_exposure->setObjectName(QString::fromUtf8("openni2_exposure"));
        openni2_exposure->setMaximum(65535);

        gridLayout_54->addWidget(openni2_exposure, 2, 0, 1, 1);

        label_219 = new QLabel(groupBox_openni2);
        label_219->setObjectName(QString::fromUtf8("label_219"));
        label_219->setWordWrap(true);

        gridLayout_54->addWidget(label_219, 2, 1, 1, 1);

        openni2_gain = new QSpinBox(groupBox_openni2);
        openni2_gain->setObjectName(QString::fromUtf8("openni2_gain"));
        openni2_gain->setMaximum(1000);
        openni2_gain->setValue(100);

        gridLayout_54->addWidget(openni2_gain, 3, 0, 1, 1);

        label_220 = new QLabel(groupBox_openni2);
        label_220->setObjectName(QString::fromUtf8("label_220"));
        label_220->setWordWrap(true);

        gridLayout_54->addWidget(label_220, 3, 1, 1, 1);

        openni2_mirroring = new QCheckBox(groupBox_openni2);
        openni2_mirroring->setObjectName(QString::fromUtf8("openni2_mirroring"));
        openni2_mirroring->setChecked(false);

        gridLayout_54->addWidget(openni2_mirroring, 4, 0, 1, 1);

        label_223 = new QLabel(groupBox_openni2);
        label_223->setObjectName(QString::fromUtf8("label_223"));
        label_223->setWordWrap(true);

        gridLayout_54->addWidget(label_223, 4, 1, 1, 1);

        gridLayout_54->setColumnStretch(1, 1);

        verticalLayout_11->addWidget(groupBox_openni2);

        groupBox_freenect2 = new QGroupBox(groupBox_sourceOpenni);
        groupBox_freenect2->setObjectName(QString::fromUtf8("groupBox_freenect2"));
        gridLayout_56 = new QGridLayout(groupBox_freenect2);
        gridLayout_56->setObjectName(QString::fromUtf8("gridLayout_56"));
        label_230 = new QLabel(groupBox_freenect2);
        label_230->setObjectName(QString::fromUtf8("label_230"));
        label_230->setWordWrap(true);

        gridLayout_56->addWidget(label_230, 0, 1, 1, 1);

        comboBox_freenect2Format = new QComboBox(groupBox_freenect2);
        comboBox_freenect2Format->setObjectName(QString::fromUtf8("comboBox_freenect2Format"));
        comboBox_freenect2Format->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_56->addWidget(comboBox_freenect2Format, 0, 0, 1, 1);

        gridLayout_56->setColumnStretch(1, 1);

        verticalLayout_11->addWidget(groupBox_freenect2);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        lineEdit_openniDevice = new QLineEdit(groupBox_sourceOpenni);
        lineEdit_openniDevice->setObjectName(QString::fromUtf8("lineEdit_openniDevice"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lineEdit_openniDevice);

        label_39 = new QLabel(groupBox_sourceOpenni);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setWordWrap(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, label_39);

        lineEdit_openniLocalTransform = new QLineEdit(groupBox_sourceOpenni);
        lineEdit_openniLocalTransform->setObjectName(QString::fromUtf8("lineEdit_openniLocalTransform"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lineEdit_openniLocalTransform);

        label_42 = new QLabel(groupBox_sourceOpenni);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setWordWrap(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, label_42);

        label_229 = new QLabel(groupBox_sourceOpenni);
        label_229->setObjectName(QString::fromUtf8("label_229"));
        label_229->setWordWrap(true);

        formLayout->setWidget(2, QFormLayout::FieldRole, label_229);

        checkbox_rgbd_colorOnly = new QCheckBox(groupBox_sourceOpenni);
        checkbox_rgbd_colorOnly->setObjectName(QString::fromUtf8("checkbox_rgbd_colorOnly"));
        checkbox_rgbd_colorOnly->setChecked(false);

        formLayout->setWidget(2, QFormLayout::LabelRole, checkbox_rgbd_colorOnly);


        verticalLayout_11->addLayout(formLayout);


        verticalLayout_7->addWidget(groupBox_sourceOpenni);


        verticalLayout_27->addWidget(groupBox_source0);

        verticalSpacer_7 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_27->addItem(verticalSpacer_7);

        stackedWidget->addWidget(page_4);
        page_20 = new QWidget();
        page_20->setObjectName(QString::fromUtf8("page_20"));
        verticalLayout_23 = new QVBoxLayout(page_20);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        groupBox_rtabmap_basic0 = new QGroupBox(page_20);
        groupBox_rtabmap_basic0->setObjectName(QString::fromUtf8("groupBox_rtabmap_basic0"));
        gridLayout_3 = new QGridLayout(groupBox_rtabmap_basic0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        general_doubleSpinBox_detectionRate_2 = new QDoubleSpinBox(groupBox_rtabmap_basic0);
        general_doubleSpinBox_detectionRate_2->setObjectName(QString::fromUtf8("general_doubleSpinBox_detectionRate_2"));
        general_doubleSpinBox_detectionRate_2->setDecimals(1);
        general_doubleSpinBox_detectionRate_2->setValue(1);

        gridLayout_3->addWidget(general_doubleSpinBox_detectionRate_2, 3, 0, 1, 1);

        general_checkBox_SLAM_mode_2 = new QCheckBox(groupBox_rtabmap_basic0);
        general_checkBox_SLAM_mode_2->setObjectName(QString::fromUtf8("general_checkBox_SLAM_mode_2"));
        general_checkBox_SLAM_mode_2->setChecked(true);

        gridLayout_3->addWidget(general_checkBox_SLAM_mode_2, 0, 0, 1, 1);

        label_79 = new QLabel(groupBox_rtabmap_basic0);
        label_79->setObjectName(QString::fromUtf8("label_79"));
        label_79->setWordWrap(true);

        gridLayout_3->addWidget(label_79, 0, 1, 1, 1);

        general_checkBox_activateRGBD_2 = new QCheckBox(groupBox_rtabmap_basic0);
        general_checkBox_activateRGBD_2->setObjectName(QString::fromUtf8("general_checkBox_activateRGBD_2"));
        general_checkBox_activateRGBD_2->setChecked(true);

        gridLayout_3->addWidget(general_checkBox_activateRGBD_2, 1, 0, 1, 1);

        label_activateRGBD_2 = new QLabel(groupBox_rtabmap_basic0);
        label_activateRGBD_2->setObjectName(QString::fromUtf8("label_activateRGBD_2"));
        label_activateRGBD_2->setWordWrap(true);

        gridLayout_3->addWidget(label_activateRGBD_2, 1, 1, 1, 1);

        general_checkBox_publishStats_2 = new QCheckBox(groupBox_rtabmap_basic0);
        general_checkBox_publishStats_2->setObjectName(QString::fromUtf8("general_checkBox_publishStats_2"));
        general_checkBox_publishStats_2->setChecked(true);

        gridLayout_3->addWidget(general_checkBox_publishStats_2, 2, 0, 1, 1);

        label_publishStat = new QLabel(groupBox_rtabmap_basic0);
        label_publishStat->setObjectName(QString::fromUtf8("label_publishStat"));
        label_publishStat->setWordWrap(true);

        gridLayout_3->addWidget(label_publishStat, 2, 1, 1, 1);

        label_134 = new QLabel(groupBox_rtabmap_basic0);
        label_134->setObjectName(QString::fromUtf8("label_134"));
        label_134->setWordWrap(true);

        gridLayout_3->addWidget(label_134, 3, 1, 1, 1);

        general_spinBox_imagesBufferSize_2 = new QSpinBox(groupBox_rtabmap_basic0);
        general_spinBox_imagesBufferSize_2->setObjectName(QString::fromUtf8("general_spinBox_imagesBufferSize_2"));
        general_spinBox_imagesBufferSize_2->setMaximum(999);
        general_spinBox_imagesBufferSize_2->setValue(1);

        gridLayout_3->addWidget(general_spinBox_imagesBufferSize_2, 4, 0, 1, 1);

        label_112 = new QLabel(groupBox_rtabmap_basic0);
        label_112->setObjectName(QString::fromUtf8("label_112"));
        label_112->setWordWrap(true);

        gridLayout_3->addWidget(label_112, 4, 1, 1, 1);

        general_doubleSpinBox_timeThr_2 = new QDoubleSpinBox(groupBox_rtabmap_basic0);
        general_doubleSpinBox_timeThr_2->setObjectName(QString::fromUtf8("general_doubleSpinBox_timeThr_2"));
        general_doubleSpinBox_timeThr_2->setDecimals(0);
        general_doubleSpinBox_timeThr_2->setMaximum(10000);
        general_doubleSpinBox_timeThr_2->setSingleStep(50);
        general_doubleSpinBox_timeThr_2->setValue(700);

        gridLayout_3->addWidget(general_doubleSpinBox_timeThr_2, 5, 0, 1, 1);

        label_timeLimit_2 = new QLabel(groupBox_rtabmap_basic0);
        label_timeLimit_2->setObjectName(QString::fromUtf8("label_timeLimit_2"));
        label_timeLimit_2->setWordWrap(true);

        gridLayout_3->addWidget(label_timeLimit_2, 5, 1, 1, 1);

        general_doubleSpinBox_hardThr_2 = new QDoubleSpinBox(groupBox_rtabmap_basic0);
        general_doubleSpinBox_hardThr_2->setObjectName(QString::fromUtf8("general_doubleSpinBox_hardThr_2"));
        general_doubleSpinBox_hardThr_2->setMaximum(1);
        general_doubleSpinBox_hardThr_2->setSingleStep(0.01);
        general_doubleSpinBox_hardThr_2->setValue(0.95);

        gridLayout_3->addWidget(general_doubleSpinBox_hardThr_2, 6, 0, 1, 1);

        label_121 = new QLabel(groupBox_rtabmap_basic0);
        label_121->setObjectName(QString::fromUtf8("label_121"));
        label_121->setWordWrap(true);

        gridLayout_3->addWidget(label_121, 6, 1, 1, 1);

        doubleSpinBox_similarityThreshold_2 = new QDoubleSpinBox(groupBox_rtabmap_basic0);
        doubleSpinBox_similarityThreshold_2->setObjectName(QString::fromUtf8("doubleSpinBox_similarityThreshold_2"));
        doubleSpinBox_similarityThreshold_2->setMaximum(1);
        doubleSpinBox_similarityThreshold_2->setValue(0.2);

        gridLayout_3->addWidget(doubleSpinBox_similarityThreshold_2, 7, 0, 1, 1);

        label_similarity_2 = new QLabel(groupBox_rtabmap_basic0);
        label_similarity_2->setObjectName(QString::fromUtf8("label_similarity_2"));
        label_similarity_2->setWordWrap(true);

        gridLayout_3->addWidget(label_similarity_2, 7, 1, 1, 1);

        general_spinBox_maxStMemSize_2 = new QSpinBox(groupBox_rtabmap_basic0);
        general_spinBox_maxStMemSize_2->setObjectName(QString::fromUtf8("general_spinBox_maxStMemSize_2"));
        general_spinBox_maxStMemSize_2->setMinimum(1);
        general_spinBox_maxStMemSize_2->setMaximum(9999);
        general_spinBox_maxStMemSize_2->setValue(15);

        gridLayout_3->addWidget(general_spinBox_maxStMemSize_2, 8, 0, 1, 1);

        label_126 = new QLabel(groupBox_rtabmap_basic0);
        label_126->setObjectName(QString::fromUtf8("label_126"));
        label_126->setWordWrap(true);

        gridLayout_3->addWidget(label_126, 8, 1, 1, 1);

        gridLayout_3->setColumnStretch(1, 1);

        verticalLayout_23->addWidget(groupBox_rtabmap_basic0);

        verticalSpacer_5 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_23->addItem(verticalSpacer_5);

        stackedWidget->addWidget(page_20);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_44 = new QVBoxLayout(page);
        verticalLayout_44->setObjectName(QString::fromUtf8("verticalLayout_44"));
        groupBox_rtabmap_advanced0 = new QGroupBox(page);
        groupBox_rtabmap_advanced0->setObjectName(QString::fromUtf8("groupBox_rtabmap_advanced0"));
        verticalLayout_15 = new QVBoxLayout(groupBox_rtabmap_advanced0);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        gridLayout_43 = new QGridLayout();
        gridLayout_43->setObjectName(QString::fromUtf8("gridLayout_43"));
        general_checkBox_SLAM_mode = new QCheckBox(groupBox_rtabmap_advanced0);
        general_checkBox_SLAM_mode->setObjectName(QString::fromUtf8("general_checkBox_SLAM_mode"));
        general_checkBox_SLAM_mode->setChecked(true);

        gridLayout_43->addWidget(general_checkBox_SLAM_mode, 0, 0, 1, 1);

        label_92 = new QLabel(groupBox_rtabmap_advanced0);
        label_92->setObjectName(QString::fromUtf8("label_92"));
        label_92->setWordWrap(true);

        gridLayout_43->addWidget(label_92, 0, 1, 1, 1);

        general_doubleSpinBox_detectionRate = new QDoubleSpinBox(groupBox_rtabmap_advanced0);
        general_doubleSpinBox_detectionRate->setObjectName(QString::fromUtf8("general_doubleSpinBox_detectionRate"));
        general_doubleSpinBox_detectionRate->setDecimals(1);
        general_doubleSpinBox_detectionRate->setValue(1);

        gridLayout_43->addWidget(general_doubleSpinBox_detectionRate, 1, 0, 1, 1);

        label_135 = new QLabel(groupBox_rtabmap_advanced0);
        label_135->setObjectName(QString::fromUtf8("label_135"));
        label_135->setWordWrap(true);

        gridLayout_43->addWidget(label_135, 1, 1, 1, 1);

        general_spinBox_imagesBufferSize = new QSpinBox(groupBox_rtabmap_advanced0);
        general_spinBox_imagesBufferSize->setObjectName(QString::fromUtf8("general_spinBox_imagesBufferSize"));
        general_spinBox_imagesBufferSize->setMaximum(999);
        general_spinBox_imagesBufferSize->setValue(1);

        gridLayout_43->addWidget(general_spinBox_imagesBufferSize, 2, 0, 1, 1);

        label_84 = new QLabel(groupBox_rtabmap_advanced0);
        label_84->setObjectName(QString::fromUtf8("label_84"));
        label_84->setWordWrap(true);

        gridLayout_43->addWidget(label_84, 2, 1, 1, 1);

        general_checkBox_startNewMapOnLoopClosure = new QCheckBox(groupBox_rtabmap_advanced0);
        general_checkBox_startNewMapOnLoopClosure->setObjectName(QString::fromUtf8("general_checkBox_startNewMapOnLoopClosure"));
        general_checkBox_startNewMapOnLoopClosure->setChecked(true);

        gridLayout_43->addWidget(general_checkBox_startNewMapOnLoopClosure, 3, 0, 1, 1);

        label_185 = new QLabel(groupBox_rtabmap_advanced0);
        label_185->setObjectName(QString::fromUtf8("label_185"));
        label_185->setWordWrap(true);

        gridLayout_43->addWidget(label_185, 3, 1, 1, 1);

        gridLayout_43->setColumnStretch(1, 1);

        verticalLayout_15->addLayout(gridLayout_43);

        groupBox_6 = new QGroupBox(groupBox_rtabmap_advanced0);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout_44 = new QGridLayout(groupBox_6);
        gridLayout_44->setObjectName(QString::fromUtf8("gridLayout_44"));
        general_doubleSpinBox_timeThr = new QDoubleSpinBox(groupBox_6);
        general_doubleSpinBox_timeThr->setObjectName(QString::fromUtf8("general_doubleSpinBox_timeThr"));
        general_doubleSpinBox_timeThr->setDecimals(0);
        general_doubleSpinBox_timeThr->setMaximum(10000);
        general_doubleSpinBox_timeThr->setSingleStep(50);
        general_doubleSpinBox_timeThr->setValue(700);

        gridLayout_44->addWidget(general_doubleSpinBox_timeThr, 0, 0, 1, 1);

        label_timeLimit = new QLabel(groupBox_6);
        label_timeLimit->setObjectName(QString::fromUtf8("label_timeLimit"));
        label_timeLimit->setWordWrap(true);

        gridLayout_44->addWidget(label_timeLimit, 0, 1, 1, 1);

        general_spinBox_memoryThr = new QSpinBox(groupBox_6);
        general_spinBox_memoryThr->setObjectName(QString::fromUtf8("general_spinBox_memoryThr"));
        general_spinBox_memoryThr->setMaximum(99999);
        general_spinBox_memoryThr->setValue(0);

        gridLayout_44->addWidget(general_spinBox_memoryThr, 1, 0, 1, 1);

        label_maxWmSize = new QLabel(groupBox_6);
        label_maxWmSize->setObjectName(QString::fromUtf8("label_maxWmSize"));
        label_maxWmSize->setWordWrap(true);

        gridLayout_44->addWidget(label_maxWmSize, 1, 1, 1, 1);

        general_doubleSpinBox_hardThr = new QDoubleSpinBox(groupBox_6);
        general_doubleSpinBox_hardThr->setObjectName(QString::fromUtf8("general_doubleSpinBox_hardThr"));
        general_doubleSpinBox_hardThr->setMaximum(1);
        general_doubleSpinBox_hardThr->setSingleStep(0.01);
        general_doubleSpinBox_hardThr->setValue(0.95);

        gridLayout_44->addWidget(general_doubleSpinBox_hardThr, 2, 0, 1, 1);

        label_93 = new QLabel(groupBox_6);
        label_93->setObjectName(QString::fromUtf8("label_93"));
        label_93->setWordWrap(true);

        gridLayout_44->addWidget(label_93, 2, 1, 1, 1);

        general_doubleSpinBox_loopRatio = new QDoubleSpinBox(groupBox_6);
        general_doubleSpinBox_loopRatio->setObjectName(QString::fromUtf8("general_doubleSpinBox_loopRatio"));
        general_doubleSpinBox_loopRatio->setMaximum(1);
        general_doubleSpinBox_loopRatio->setSingleStep(0.01);
        general_doubleSpinBox_loopRatio->setValue(0.7);

        gridLayout_44->addWidget(general_doubleSpinBox_loopRatio, 3, 0, 1, 1);

        label_96 = new QLabel(groupBox_6);
        label_96->setObjectName(QString::fromUtf8("label_96"));
        label_96->setWordWrap(true);

        gridLayout_44->addWidget(label_96, 3, 1, 1, 1);

        gridLayout_44->setColumnStretch(1, 1);

        verticalLayout_15->addWidget(groupBox_6);

        groupBox_publishing = new QGroupBox(groupBox_rtabmap_advanced0);
        groupBox_publishing->setObjectName(QString::fromUtf8("groupBox_publishing"));
        groupBox_publishing->setCheckable(true);
        gridLayout_45 = new QGridLayout(groupBox_publishing);
        gridLayout_45->setObjectName(QString::fromUtf8("gridLayout_45"));
        general_checkBox_publishRawData = new QCheckBox(groupBox_publishing);
        general_checkBox_publishRawData->setObjectName(QString::fromUtf8("general_checkBox_publishRawData"));
        general_checkBox_publishRawData->setChecked(true);

        gridLayout_45->addWidget(general_checkBox_publishRawData, 0, 0, 1, 1);

        label_91 = new QLabel(groupBox_publishing);
        label_91->setObjectName(QString::fromUtf8("label_91"));

        gridLayout_45->addWidget(label_91, 0, 1, 1, 1);

        general_checkBox_publishPdf = new QCheckBox(groupBox_publishing);
        general_checkBox_publishPdf->setObjectName(QString::fromUtf8("general_checkBox_publishPdf"));
        general_checkBox_publishPdf->setChecked(true);

        gridLayout_45->addWidget(general_checkBox_publishPdf, 1, 0, 1, 1);

        label_106 = new QLabel(groupBox_publishing);
        label_106->setObjectName(QString::fromUtf8("label_106"));

        gridLayout_45->addWidget(label_106, 1, 1, 1, 1);

        general_checkBox_publishLikelihood = new QCheckBox(groupBox_publishing);
        general_checkBox_publishLikelihood->setObjectName(QString::fromUtf8("general_checkBox_publishLikelihood"));
        general_checkBox_publishLikelihood->setChecked(true);

        gridLayout_45->addWidget(general_checkBox_publishLikelihood, 2, 0, 1, 1);

        label_116 = new QLabel(groupBox_publishing);
        label_116->setObjectName(QString::fromUtf8("label_116"));

        gridLayout_45->addWidget(label_116, 2, 1, 1, 1);

        gridLayout_45->setColumnStretch(1, 1);

        verticalLayout_15->addWidget(groupBox_publishing);

        groupBox_statistics = new QGroupBox(groupBox_rtabmap_advanced0);
        groupBox_statistics->setObjectName(QString::fromUtf8("groupBox_statistics"));
        groupBox_statistics->setCheckable(true);
        gridLayout_46 = new QGridLayout(groupBox_statistics);
        gridLayout_46->setObjectName(QString::fromUtf8("gridLayout_46"));
        general_checkBox_statisticLogsBufferedInRAM = new QCheckBox(groupBox_statistics);
        general_checkBox_statisticLogsBufferedInRAM->setObjectName(QString::fromUtf8("general_checkBox_statisticLogsBufferedInRAM"));
        general_checkBox_statisticLogsBufferedInRAM->setChecked(true);

        gridLayout_46->addWidget(general_checkBox_statisticLogsBufferedInRAM, 0, 0, 1, 1);

        label_143 = new QLabel(groupBox_statistics);
        label_143->setObjectName(QString::fromUtf8("label_143"));
        label_143->setWordWrap(true);

        gridLayout_46->addWidget(label_143, 0, 1, 1, 1);

        general_checkBox_statisticLoggedHeaders = new QCheckBox(groupBox_statistics);
        general_checkBox_statisticLoggedHeaders->setObjectName(QString::fromUtf8("general_checkBox_statisticLoggedHeaders"));
        general_checkBox_statisticLoggedHeaders->setChecked(true);

        gridLayout_46->addWidget(general_checkBox_statisticLoggedHeaders, 1, 0, 1, 1);

        label_209 = new QLabel(groupBox_statistics);
        label_209->setObjectName(QString::fromUtf8("label_209"));
        label_209->setWordWrap(true);

        gridLayout_46->addWidget(label_209, 1, 1, 1, 1);

        gridLayout_46->setColumnStretch(1, 1);

        verticalLayout_15->addWidget(groupBox_statistics);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_workingDirectory = new QLabel(groupBox_rtabmap_advanced0);
        label_workingDirectory->setObjectName(QString::fromUtf8("label_workingDirectory"));
        label_workingDirectory->setWordWrap(true);

        gridLayout_4->addWidget(label_workingDirectory, 0, 0, 1, 1);

        toolButton_workingDirectory = new QToolButton(groupBox_rtabmap_advanced0);
        toolButton_workingDirectory->setObjectName(QString::fromUtf8("toolButton_workingDirectory"));

        gridLayout_4->addWidget(toolButton_workingDirectory, 0, 1, 1, 1);

        lineEdit_workingDirectory = new QLineEdit(groupBox_rtabmap_advanced0);
        lineEdit_workingDirectory->setObjectName(QString::fromUtf8("lineEdit_workingDirectory"));
        lineEdit_workingDirectory->setReadOnly(true);

        gridLayout_4->addWidget(lineEdit_workingDirectory, 0, 2, 1, 1);


        verticalLayout_15->addLayout(gridLayout_4);


        verticalLayout_44->addWidget(groupBox_rtabmap_advanced0);

        verticalSpacer_4 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_44->addItem(verticalSpacer_4);

        stackedWidget->addWidget(page);
        page_23 = new QWidget();
        page_23->setObjectName(QString::fromUtf8("page_23"));
        verticalLayout_2 = new QVBoxLayout(page_23);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_bayes1 = new QGroupBox(page_23);
        groupBox_bayes1->setObjectName(QString::fromUtf8("groupBox_bayes1"));
        verticalLayout_22 = new QVBoxLayout(groupBox_bayes1);
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        label = new QLabel(groupBox_bayes1);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_22->addWidget(label);

        label_76 = new QLabel(groupBox_bayes1);
        label_76->setObjectName(QString::fromUtf8("label_76"));
        label_76->setWordWrap(true);

        verticalLayout_22->addWidget(label_76);

        gridLayout_24 = new QGridLayout();
        gridLayout_24->setObjectName(QString::fromUtf8("gridLayout_24"));
        label_65 = new QLabel(groupBox_bayes1);
        label_65->setObjectName(QString::fromUtf8("label_65"));

        gridLayout_24->addWidget(label_65, 0, 0, 1, 1);

        label_66 = new QLabel(groupBox_bayes1);
        label_66->setObjectName(QString::fromUtf8("label_66"));

        gridLayout_24->addWidget(label_66, 1, 0, 1, 1);

        label_prediction_sum = new QLabel(groupBox_bayes1);
        label_prediction_sum->setObjectName(QString::fromUtf8("label_prediction_sum"));

        gridLayout_24->addWidget(label_prediction_sum, 1, 1, 1, 1);

        lineEdit_bayes_predictionLC = new QLineEdit(groupBox_bayes1);
        lineEdit_bayes_predictionLC->setObjectName(QString::fromUtf8("lineEdit_bayes_predictionLC"));
        lineEdit_bayes_predictionLC->setReadOnly(false);

        gridLayout_24->addWidget(lineEdit_bayes_predictionLC, 0, 1, 1, 1);


        verticalLayout_22->addLayout(gridLayout_24);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_63 = new QLabel(groupBox_bayes1);
        label_63->setObjectName(QString::fromUtf8("label_63"));

        horizontalLayout_6->addWidget(label_63);

        predictionPlot = new UPlot(groupBox_bayes1);
        predictionPlot->setObjectName(QString::fromUtf8("predictionPlot"));

        horizontalLayout_6->addWidget(predictionPlot);

        horizontalLayout_6->setStretch(1, 1);

        verticalLayout_22->addLayout(horizontalLayout_6);

        gridLayout_22 = new QGridLayout();
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        checkBox_bayes_fullPredictionUpdate = new QCheckBox(groupBox_bayes1);
        checkBox_bayes_fullPredictionUpdate->setObjectName(QString::fromUtf8("checkBox_bayes_fullPredictionUpdate"));

        gridLayout_22->addWidget(checkBox_bayes_fullPredictionUpdate, 0, 0, 1, 1);

        general_doubleSpinBox_vp = new QDoubleSpinBox(groupBox_bayes1);
        general_doubleSpinBox_vp->setObjectName(QString::fromUtf8("general_doubleSpinBox_vp"));
        general_doubleSpinBox_vp->setMaximum(1);
        general_doubleSpinBox_vp->setSingleStep(0.01);
        general_doubleSpinBox_vp->setValue(0.9);

        gridLayout_22->addWidget(general_doubleSpinBox_vp, 1, 0, 1, 1);

        label_113 = new QLabel(groupBox_bayes1);
        label_113->setObjectName(QString::fromUtf8("label_113"));
        label_113->setWordWrap(true);

        gridLayout_22->addWidget(label_113, 1, 1, 1, 1);

        label_142 = new QLabel(groupBox_bayes1);
        label_142->setObjectName(QString::fromUtf8("label_142"));
        label_142->setWordWrap(true);

        gridLayout_22->addWidget(label_142, 0, 1, 1, 1);

        label_82 = new QLabel(groupBox_bayes1);
        label_82->setObjectName(QString::fromUtf8("label_82"));
        label_82->setWordWrap(true);

        gridLayout_22->addWidget(label_82, 2, 1, 1, 1);

        checkBox_kp_tfIdfLikelihoodUsed = new QCheckBox(groupBox_bayes1);
        checkBox_kp_tfIdfLikelihoodUsed->setObjectName(QString::fromUtf8("checkBox_kp_tfIdfLikelihoodUsed"));
        checkBox_kp_tfIdfLikelihoodUsed->setChecked(true);

        gridLayout_22->addWidget(checkBox_kp_tfIdfLikelihoodUsed, 2, 0, 1, 1);

        gridLayout_22->setColumnStretch(1, 1);

        verticalLayout_22->addLayout(gridLayout_22);


        verticalLayout_2->addWidget(groupBox_bayes1);

        verticalSpacer_23 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_23);

        stackedWidget->addWidget(page_23);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_4 = new QVBoxLayout(page_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_memory1 = new QGroupBox(page_3);
        groupBox_memory1->setObjectName(QString::fromUtf8("groupBox_memory1"));
        verticalLayout_10 = new QVBoxLayout(groupBox_memory1);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        gridLayout_42 = new QGridLayout();
        gridLayout_42->setObjectName(QString::fromUtf8("gridLayout_42"));
        spinBox_imageDecimation = new QSpinBox(groupBox_memory1);
        spinBox_imageDecimation->setObjectName(QString::fromUtf8("spinBox_imageDecimation"));
        spinBox_imageDecimation->setMinimum(1);
        spinBox_imageDecimation->setMaximum(16);

        gridLayout_42->addWidget(spinBox_imageDecimation, 8, 0, 1, 1);

        general_doubleSpinBox_recentWmRatio = new QDoubleSpinBox(groupBox_memory1);
        general_doubleSpinBox_recentWmRatio->setObjectName(QString::fromUtf8("general_doubleSpinBox_recentWmRatio"));
        general_doubleSpinBox_recentWmRatio->setMinimumSize(QSize(50, 0));
        general_doubleSpinBox_recentWmRatio->setMaximum(1);
        general_doubleSpinBox_recentWmRatio->setSingleStep(0.01);
        general_doubleSpinBox_recentWmRatio->setValue(0.2);

        gridLayout_42->addWidget(general_doubleSpinBox_recentWmRatio, 1, 0, 1, 1);

        general_spinBox_maxStMemSize = new QSpinBox(groupBox_memory1);
        general_spinBox_maxStMemSize->setObjectName(QString::fromUtf8("general_spinBox_maxStMemSize"));
        general_spinBox_maxStMemSize->setMinimum(1);
        general_spinBox_maxStMemSize->setMaximum(9999);
        general_spinBox_maxStMemSize->setValue(15);

        gridLayout_42->addWidget(general_spinBox_maxStMemSize, 0, 0, 1, 1);

        label_retrieved_4 = new QLabel(groupBox_memory1);
        label_retrieved_4->setObjectName(QString::fromUtf8("label_retrieved_4"));
        label_retrieved_4->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved_4, 6, 1, 1, 1);

        label_29 = new QLabel(groupBox_memory1);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setWordWrap(false);

        gridLayout_42->addWidget(label_29, 0, 1, 1, 1);

        label_ratioRecent = new QLabel(groupBox_memory1);
        label_ratioRecent->setObjectName(QString::fromUtf8("label_ratioRecent"));
        label_ratioRecent->setWordWrap(true);

        gridLayout_42->addWidget(label_ratioRecent, 1, 1, 1, 1);

        general_spinBox_maxRetrieved = new QSpinBox(groupBox_memory1);
        general_spinBox_maxRetrieved->setObjectName(QString::fromUtf8("general_spinBox_maxRetrieved"));
        general_spinBox_maxRetrieved->setMaximum(999);
        general_spinBox_maxRetrieved->setSingleStep(1);
        general_spinBox_maxRetrieved->setValue(2);

        gridLayout_42->addWidget(general_spinBox_maxRetrieved, 2, 0, 1, 1);

        label_retrieved = new QLabel(groupBox_memory1);
        label_retrieved->setObjectName(QString::fromUtf8("label_retrieved"));
        label_retrieved->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved, 2, 1, 1, 1);

        general_checkBox_generateIds = new QCheckBox(groupBox_memory1);
        general_checkBox_generateIds->setObjectName(QString::fromUtf8("general_checkBox_generateIds"));
        general_checkBox_generateIds->setChecked(true);

        gridLayout_42->addWidget(general_checkBox_generateIds, 4, 0, 1, 1);

        label_retrieved_2 = new QLabel(groupBox_memory1);
        label_retrieved_2->setObjectName(QString::fromUtf8("label_retrieved_2"));
        label_retrieved_2->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved_2, 4, 1, 1, 1);

        general_checkBox_badSignaturesIgnored = new QCheckBox(groupBox_memory1);
        general_checkBox_badSignaturesIgnored->setObjectName(QString::fromUtf8("general_checkBox_badSignaturesIgnored"));
        general_checkBox_badSignaturesIgnored->setChecked(false);

        gridLayout_42->addWidget(general_checkBox_badSignaturesIgnored, 5, 0, 1, 1);

        label_retrieved_3 = new QLabel(groupBox_memory1);
        label_retrieved_3->setObjectName(QString::fromUtf8("label_retrieved_3"));
        label_retrieved_3->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved_3, 5, 1, 1, 1);

        general_checkBox_initWMWithAllNodes = new QCheckBox(groupBox_memory1);
        general_checkBox_initWMWithAllNodes->setObjectName(QString::fromUtf8("general_checkBox_initWMWithAllNodes"));
        general_checkBox_initWMWithAllNodes->setChecked(false);

        gridLayout_42->addWidget(general_checkBox_initWMWithAllNodes, 6, 0, 1, 1);

        label_retrieved_5 = new QLabel(groupBox_memory1);
        label_retrieved_5->setObjectName(QString::fromUtf8("label_retrieved_5"));
        label_retrieved_5->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved_5, 7, 1, 1, 1);

        general_checkBox_keepRawData = new QCheckBox(groupBox_memory1);
        general_checkBox_keepRawData->setObjectName(QString::fromUtf8("general_checkBox_keepRawData"));
        general_checkBox_keepRawData->setChecked(false);

        gridLayout_42->addWidget(general_checkBox_keepRawData, 7, 0, 1, 1);

        label_retrieved_6 = new QLabel(groupBox_memory1);
        label_retrieved_6->setObjectName(QString::fromUtf8("label_retrieved_6"));
        label_retrieved_6->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved_6, 8, 1, 1, 1);

        label_retrieved_7 = new QLabel(groupBox_memory1);
        label_retrieved_7->setObjectName(QString::fromUtf8("label_retrieved_7"));
        label_retrieved_7->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved_7, 3, 1, 1, 1);

        general_checkBox_transferSortingByWeightId = new QCheckBox(groupBox_memory1);
        general_checkBox_transferSortingByWeightId->setObjectName(QString::fromUtf8("general_checkBox_transferSortingByWeightId"));
        general_checkBox_transferSortingByWeightId->setChecked(true);

        gridLayout_42->addWidget(general_checkBox_transferSortingByWeightId, 3, 0, 1, 1);

        label_retrieved_8 = new QLabel(groupBox_memory1);
        label_retrieved_8->setObjectName(QString::fromUtf8("label_retrieved_8"));
        label_retrieved_8->setWordWrap(true);

        gridLayout_42->addWidget(label_retrieved_8, 9, 1, 1, 1);

        general_doubleSpinBox_laserScanVoxel = new QDoubleSpinBox(groupBox_memory1);
        general_doubleSpinBox_laserScanVoxel->setObjectName(QString::fromUtf8("general_doubleSpinBox_laserScanVoxel"));
        general_doubleSpinBox_laserScanVoxel->setMinimumSize(QSize(50, 0));
        general_doubleSpinBox_laserScanVoxel->setDecimals(3);
        general_doubleSpinBox_laserScanVoxel->setMaximum(1);
        general_doubleSpinBox_laserScanVoxel->setSingleStep(0.01);
        general_doubleSpinBox_laserScanVoxel->setValue(0);

        gridLayout_42->addWidget(general_doubleSpinBox_laserScanVoxel, 9, 0, 1, 1);

        gridLayout_42->setColumnStretch(1, 1);

        verticalLayout_10->addLayout(gridLayout_42);

        groupBox_rehearsal2 = new QGroupBox(groupBox_memory1);
        groupBox_rehearsal2->setObjectName(QString::fromUtf8("groupBox_rehearsal2"));
        gridLayout_12 = new QGridLayout(groupBox_rehearsal2);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        general_checkBox_RehearsalIdUpdatedToNewOne = new QCheckBox(groupBox_rehearsal2);
        general_checkBox_RehearsalIdUpdatedToNewOne->setObjectName(QString::fromUtf8("general_checkBox_RehearsalIdUpdatedToNewOne"));

        gridLayout_12->addWidget(general_checkBox_RehearsalIdUpdatedToNewOne, 2, 0, 1, 1);

        label_similarity = new QLabel(groupBox_rehearsal2);
        label_similarity->setObjectName(QString::fromUtf8("label_similarity"));
        label_similarity->setWordWrap(true);

        gridLayout_12->addWidget(label_similarity, 0, 1, 1, 1);

        doubleSpinBox_similarityThreshold = new QDoubleSpinBox(groupBox_rehearsal2);
        doubleSpinBox_similarityThreshold->setObjectName(QString::fromUtf8("doubleSpinBox_similarityThreshold"));
        doubleSpinBox_similarityThreshold->setMaximum(1);
        doubleSpinBox_similarityThreshold->setValue(0.2);

        gridLayout_12->addWidget(doubleSpinBox_similarityThreshold, 0, 0, 1, 1);

        label_rehearsalIdUpdate = new QLabel(groupBox_rehearsal2);
        label_rehearsalIdUpdate->setObjectName(QString::fromUtf8("label_rehearsalIdUpdate"));
        label_rehearsalIdUpdate->setWordWrap(true);

        gridLayout_12->addWidget(label_rehearsalIdUpdate, 2, 1, 1, 1);

        label_rehearsalIdUpdate_2 = new QLabel(groupBox_rehearsal2);
        label_rehearsalIdUpdate_2->setObjectName(QString::fromUtf8("label_rehearsalIdUpdate_2"));
        label_rehearsalIdUpdate_2->setWordWrap(true);

        gridLayout_12->addWidget(label_rehearsalIdUpdate_2, 3, 1, 1, 1);

        rgdb_rehearsalWeightIgnoredWhileMoving = new QCheckBox(groupBox_rehearsal2);
        rgdb_rehearsalWeightIgnoredWhileMoving->setObjectName(QString::fromUtf8("rgdb_rehearsalWeightIgnoredWhileMoving"));

        gridLayout_12->addWidget(rgdb_rehearsalWeightIgnoredWhileMoving, 3, 0, 1, 1);

        gridLayout_12->setColumnStretch(1, 1);

        verticalLayout_10->addWidget(groupBox_rehearsal2);


        verticalLayout_4->addWidget(groupBox_memory1);

        verticalSpacer_6 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_6);

        stackedWidget->addWidget(page_3);
        page_24 = new QWidget();
        page_24->setObjectName(QString::fromUtf8("page_24"));
        verticalLayout_33 = new QVBoxLayout(page_24);
        verticalLayout_33->setObjectName(QString::fromUtf8("verticalLayout_33"));
        groupBox_database2 = new QGroupBox(page_24);
        groupBox_database2->setObjectName(QString::fromUtf8("groupBox_database2"));
        gridLayout_11 = new QGridLayout(groupBox_database2);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        general_checkBox_keepBinaryData = new QCheckBox(groupBox_database2);
        general_checkBox_keepBinaryData->setObjectName(QString::fromUtf8("general_checkBox_keepBinaryData"));
        general_checkBox_keepBinaryData->setChecked(true);

        gridLayout_11->addWidget(general_checkBox_keepBinaryData, 0, 0, 1, 1);

        label_keepRawData = new QLabel(groupBox_database2);
        label_keepRawData->setObjectName(QString::fromUtf8("label_keepRawData"));
        label_keepRawData->setWordWrap(true);

        gridLayout_11->addWidget(label_keepRawData, 0, 1, 1, 1);

        checkBox_dbInMemory = new QCheckBox(groupBox_database2);
        checkBox_dbInMemory->setObjectName(QString::fromUtf8("checkBox_dbInMemory"));

        gridLayout_11->addWidget(checkBox_dbInMemory, 2, 0, 1, 1);

        spinBox_dbCacheSize = new QSpinBox(groupBox_database2);
        spinBox_dbCacheSize->setObjectName(QString::fromUtf8("spinBox_dbCacheSize"));
        spinBox_dbCacheSize->setMinimum(10);
        spinBox_dbCacheSize->setMaximum(999999);
        spinBox_dbCacheSize->setSingleStep(100);
        spinBox_dbCacheSize->setValue(2000);

        gridLayout_11->addWidget(spinBox_dbCacheSize, 3, 0, 1, 1);

        label_75 = new QLabel(groupBox_database2);
        label_75->setObjectName(QString::fromUtf8("label_75"));
        label_75->setWordWrap(true);

        gridLayout_11->addWidget(label_75, 3, 1, 1, 1);

        comboBox_dbJournalMode = new QComboBox(groupBox_database2);
        comboBox_dbJournalMode->setObjectName(QString::fromUtf8("comboBox_dbJournalMode"));

        gridLayout_11->addWidget(comboBox_dbJournalMode, 4, 0, 1, 1);

        label_77 = new QLabel(groupBox_database2);
        label_77->setObjectName(QString::fromUtf8("label_77"));
        label_77->setWordWrap(true);

        gridLayout_11->addWidget(label_77, 4, 1, 1, 1);

        label_74 = new QLabel(groupBox_database2);
        label_74->setObjectName(QString::fromUtf8("label_74"));
        label_74->setWordWrap(true);

        gridLayout_11->addWidget(label_74, 2, 1, 1, 1);

        label_89 = new QLabel(groupBox_database2);
        label_89->setObjectName(QString::fromUtf8("label_89"));
        label_89->setWordWrap(true);

        gridLayout_11->addWidget(label_89, 5, 1, 1, 1);

        label_120 = new QLabel(groupBox_database2);
        label_120->setObjectName(QString::fromUtf8("label_120"));
        label_120->setWordWrap(true);

        gridLayout_11->addWidget(label_120, 6, 1, 1, 1);

        comboBox_dbSynchronous = new QComboBox(groupBox_database2);
        comboBox_dbSynchronous->setObjectName(QString::fromUtf8("comboBox_dbSynchronous"));

        gridLayout_11->addWidget(comboBox_dbSynchronous, 5, 0, 1, 1);

        comboBox_dbTempStore = new QComboBox(groupBox_database2);
        comboBox_dbTempStore->setObjectName(QString::fromUtf8("comboBox_dbTempStore"));

        gridLayout_11->addWidget(comboBox_dbTempStore, 6, 0, 1, 1);

        label_keepRehearsed = new QLabel(groupBox_database2);
        label_keepRehearsed->setObjectName(QString::fromUtf8("label_keepRehearsed"));
        label_keepRehearsed->setWordWrap(true);

        gridLayout_11->addWidget(label_keepRehearsed, 1, 1, 1, 1);

        general_checkBox_keepNotLinkedNodes = new QCheckBox(groupBox_database2);
        general_checkBox_keepNotLinkedNodes->setObjectName(QString::fromUtf8("general_checkBox_keepNotLinkedNodes"));
        general_checkBox_keepNotLinkedNodes->setChecked(true);

        gridLayout_11->addWidget(general_checkBox_keepNotLinkedNodes, 1, 0, 1, 1);

        gridLayout_11->setColumnStretch(1, 1);

        verticalLayout_33->addWidget(groupBox_database2);

        verticalSpacer_12 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_33->addItem(verticalSpacer_12);

        stackedWidget->addWidget(page_24);
        page_9 = new QWidget();
        page_9->setObjectName(QString::fromUtf8("page_9"));
        verticalLayout_58 = new QVBoxLayout(page_9);
        verticalLayout_58->setObjectName(QString::fromUtf8("verticalLayout_58"));
        groupBox_signature_keypoint1 = new QGroupBox(page_9);
        groupBox_signature_keypoint1->setObjectName(QString::fromUtf8("groupBox_signature_keypoint1"));
        verticalLayout_12 = new QVBoxLayout(groupBox_signature_keypoint1);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        gridLayout_41 = new QGridLayout();
        gridLayout_41->setObjectName(QString::fromUtf8("gridLayout_41"));
        comboBox_detector_strategy = new QComboBox(groupBox_signature_keypoint1);
        comboBox_detector_strategy->setObjectName(QString::fromUtf8("comboBox_detector_strategy"));

        gridLayout_41->addWidget(comboBox_detector_strategy, 0, 0, 1, 1);

        label_117 = new QLabel(groupBox_signature_keypoint1);
        label_117->setObjectName(QString::fromUtf8("label_117"));
        label_117->setWordWrap(true);

        gridLayout_41->addWidget(label_117, 0, 1, 1, 1);

        surf_doubleSpinBox_maxDepth = new QDoubleSpinBox(groupBox_signature_keypoint1);
        surf_doubleSpinBox_maxDepth->setObjectName(QString::fromUtf8("surf_doubleSpinBox_maxDepth"));
        surf_doubleSpinBox_maxDepth->setDecimals(2);
        surf_doubleSpinBox_maxDepth->setMinimum(0);
        surf_doubleSpinBox_maxDepth->setMaximum(99);
        surf_doubleSpinBox_maxDepth->setSingleStep(1);
        surf_doubleSpinBox_maxDepth->setValue(0);

        gridLayout_41->addWidget(surf_doubleSpinBox_maxDepth, 1, 0, 1, 1);

        label_53 = new QLabel(groupBox_signature_keypoint1);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setWordWrap(true);

        gridLayout_41->addWidget(label_53, 1, 1, 1, 1);

        surf_spinBox_wordsPerImageTarget = new QSpinBox(groupBox_signature_keypoint1);
        surf_spinBox_wordsPerImageTarget->setObjectName(QString::fromUtf8("surf_spinBox_wordsPerImageTarget"));
        surf_spinBox_wordsPerImageTarget->setMinimum(-1);
        surf_spinBox_wordsPerImageTarget->setMaximum(2000);
        surf_spinBox_wordsPerImageTarget->setValue(150);

        gridLayout_41->addWidget(surf_spinBox_wordsPerImageTarget, 2, 0, 1, 1);

        label_22 = new QLabel(groupBox_signature_keypoint1);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setWordWrap(true);

        gridLayout_41->addWidget(label_22, 2, 1, 1, 1);

        surf_doubleSpinBox_ratioBadSign = new QDoubleSpinBox(groupBox_signature_keypoint1);
        surf_doubleSpinBox_ratioBadSign->setObjectName(QString::fromUtf8("surf_doubleSpinBox_ratioBadSign"));
        surf_doubleSpinBox_ratioBadSign->setDecimals(2);
        surf_doubleSpinBox_ratioBadSign->setMinimum(0);
        surf_doubleSpinBox_ratioBadSign->setMaximum(1);
        surf_doubleSpinBox_ratioBadSign->setSingleStep(0.05);
        surf_doubleSpinBox_ratioBadSign->setValue(0.25);

        gridLayout_41->addWidget(surf_doubleSpinBox_ratioBadSign, 3, 0, 1, 1);

        label_81 = new QLabel(groupBox_signature_keypoint1);
        label_81->setObjectName(QString::fromUtf8("label_81"));
        label_81->setWordWrap(true);

        gridLayout_41->addWidget(label_81, 3, 1, 1, 1);

        lineEdit_kp_roi = new QLineEdit(groupBox_signature_keypoint1);
        lineEdit_kp_roi->setObjectName(QString::fromUtf8("lineEdit_kp_roi"));
        lineEdit_kp_roi->setReadOnly(true);

        gridLayout_41->addWidget(lineEdit_kp_roi, 4, 0, 1, 1);

        label_101 = new QLabel(groupBox_signature_keypoint1);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setWordWrap(true);

        gridLayout_41->addWidget(label_101, 4, 1, 1, 1);

        doubleSpinBox_kp_roi0 = new QDoubleSpinBox(groupBox_signature_keypoint1);
        doubleSpinBox_kp_roi0->setObjectName(QString::fromUtf8("doubleSpinBox_kp_roi0"));
        doubleSpinBox_kp_roi0->setDecimals(0);

        gridLayout_41->addWidget(doubleSpinBox_kp_roi0, 5, 0, 1, 1);

        label_97 = new QLabel(groupBox_signature_keypoint1);
        label_97->setObjectName(QString::fromUtf8("label_97"));
        label_97->setWordWrap(true);

        gridLayout_41->addWidget(label_97, 5, 1, 1, 1);

        doubleSpinBox_kp_roi1 = new QDoubleSpinBox(groupBox_signature_keypoint1);
        doubleSpinBox_kp_roi1->setObjectName(QString::fromUtf8("doubleSpinBox_kp_roi1"));
        doubleSpinBox_kp_roi1->setDecimals(0);

        gridLayout_41->addWidget(doubleSpinBox_kp_roi1, 6, 0, 1, 1);

        label_98 = new QLabel(groupBox_signature_keypoint1);
        label_98->setObjectName(QString::fromUtf8("label_98"));
        label_98->setWordWrap(true);

        gridLayout_41->addWidget(label_98, 6, 1, 1, 1);

        doubleSpinBox_kp_roi2 = new QDoubleSpinBox(groupBox_signature_keypoint1);
        doubleSpinBox_kp_roi2->setObjectName(QString::fromUtf8("doubleSpinBox_kp_roi2"));
        doubleSpinBox_kp_roi2->setDecimals(0);

        gridLayout_41->addWidget(doubleSpinBox_kp_roi2, 7, 0, 1, 1);

        label_99 = new QLabel(groupBox_signature_keypoint1);
        label_99->setObjectName(QString::fromUtf8("label_99"));
        label_99->setWordWrap(true);

        gridLayout_41->addWidget(label_99, 7, 1, 1, 1);

        doubleSpinBox_kp_roi3 = new QDoubleSpinBox(groupBox_signature_keypoint1);
        doubleSpinBox_kp_roi3->setObjectName(QString::fromUtf8("doubleSpinBox_kp_roi3"));
        doubleSpinBox_kp_roi3->setDecimals(0);

        gridLayout_41->addWidget(doubleSpinBox_kp_roi3, 8, 0, 1, 1);

        label_100 = new QLabel(groupBox_signature_keypoint1);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setWordWrap(true);

        gridLayout_41->addWidget(label_100, 8, 1, 1, 1);

        gridLayout_41->setColumnStretch(1, 1);

        verticalLayout_12->addLayout(gridLayout_41);

        groupBox_dictionary_strategy2 = new QGroupBox(groupBox_signature_keypoint1);
        groupBox_dictionary_strategy2->setObjectName(QString::fromUtf8("groupBox_dictionary_strategy2"));
        gridLayout_18 = new QGridLayout(groupBox_dictionary_strategy2);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        toolButton_dictionaryPath = new QToolButton(groupBox_dictionary_strategy2);
        toolButton_dictionaryPath->setObjectName(QString::fromUtf8("toolButton_dictionaryPath"));

        gridLayout_18->addWidget(toolButton_dictionaryPath, 3, 1, 1, 1);

        label_78 = new QLabel(groupBox_dictionary_strategy2);
        label_78->setObjectName(QString::fromUtf8("label_78"));
        label_78->setWordWrap(true);

        gridLayout_18->addWidget(label_78, 4, 2, 1, 1);

        surf_doubleSpinBox_nndrRatio = new QDoubleSpinBox(groupBox_dictionary_strategy2);
        surf_doubleSpinBox_nndrRatio->setObjectName(QString::fromUtf8("surf_doubleSpinBox_nndrRatio"));
        surf_doubleSpinBox_nndrRatio->setDecimals(1);
        surf_doubleSpinBox_nndrRatio->setMinimum(0.1);
        surf_doubleSpinBox_nndrRatio->setMaximum(1);
        surf_doubleSpinBox_nndrRatio->setSingleStep(0.1);
        surf_doubleSpinBox_nndrRatio->setValue(0.7);

        gridLayout_18->addWidget(surf_doubleSpinBox_nndrRatio, 4, 0, 1, 1);

        lineEdit_dictionaryPath = new QLineEdit(groupBox_dictionary_strategy2);
        lineEdit_dictionaryPath->setObjectName(QString::fromUtf8("lineEdit_dictionaryPath"));

        gridLayout_18->addWidget(lineEdit_dictionaryPath, 3, 0, 1, 1);

        label_dictionaryPath = new QLabel(groupBox_dictionary_strategy2);
        label_dictionaryPath->setObjectName(QString::fromUtf8("label_dictionaryPath"));
        label_dictionaryPath->setWordWrap(true);

        gridLayout_18->addWidget(label_dictionaryPath, 3, 2, 1, 1);

        checkBox_dictionary_incremental = new QCheckBox(groupBox_dictionary_strategy2);
        checkBox_dictionary_incremental->setObjectName(QString::fromUtf8("checkBox_dictionary_incremental"));
        checkBox_dictionary_incremental->setChecked(true);

        gridLayout_18->addWidget(checkBox_dictionary_incremental, 1, 0, 1, 1);

        label_12 = new QLabel(groupBox_dictionary_strategy2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_18->addWidget(label_12, 5, 2, 1, 1);

        comboBox_dictionary_strategy = new QComboBox(groupBox_dictionary_strategy2);
        comboBox_dictionary_strategy->setObjectName(QString::fromUtf8("comboBox_dictionary_strategy"));
        comboBox_dictionary_strategy->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_18->addWidget(comboBox_dictionary_strategy, 5, 0, 1, 1);

        label_incrementalDict = new QLabel(groupBox_dictionary_strategy2);
        label_incrementalDict->setObjectName(QString::fromUtf8("label_incrementalDict"));
        label_incrementalDict->setWordWrap(true);

        gridLayout_18->addWidget(label_incrementalDict, 1, 2, 1, 1);

        label_94 = new QLabel(groupBox_dictionary_strategy2);
        label_94->setObjectName(QString::fromUtf8("label_94"));
        label_94->setWordWrap(true);

        gridLayout_18->addWidget(label_94, 2, 2, 1, 1);

        checkBox_kp_parallelized = new QCheckBox(groupBox_dictionary_strategy2);
        checkBox_kp_parallelized->setObjectName(QString::fromUtf8("checkBox_kp_parallelized"));
        checkBox_kp_parallelized->setChecked(true);

        gridLayout_18->addWidget(checkBox_kp_parallelized, 2, 0, 1, 1);

        checkBox_kp_newWordsComparedTogether = new QCheckBox(groupBox_dictionary_strategy2);
        checkBox_kp_newWordsComparedTogether->setObjectName(QString::fromUtf8("checkBox_kp_newWordsComparedTogether"));
        checkBox_kp_newWordsComparedTogether->setChecked(true);

        gridLayout_18->addWidget(checkBox_kp_newWordsComparedTogether, 6, 0, 1, 1);

        label_186 = new QLabel(groupBox_dictionary_strategy2);
        label_186->setObjectName(QString::fromUtf8("label_186"));
        label_186->setWordWrap(true);

        gridLayout_18->addWidget(label_186, 6, 2, 1, 1);

        gridLayout_18->setColumnStretch(2, 1);

        verticalLayout_12->addWidget(groupBox_dictionary_strategy2);

        groupBox_9 = new QGroupBox(groupBox_signature_keypoint1);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        verticalLayout_59 = new QVBoxLayout(groupBox_9);
        verticalLayout_59->setObjectName(QString::fromUtf8("verticalLayout_59"));
        label_35 = new QLabel(groupBox_9);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setWordWrap(true);

        verticalLayout_59->addWidget(label_35);

        gridLayout_32 = new QGridLayout();
        gridLayout_32->setObjectName(QString::fromUtf8("gridLayout_32"));
        subpix_winSize = new QSpinBox(groupBox_9);
        subpix_winSize->setObjectName(QString::fromUtf8("subpix_winSize"));
        subpix_winSize->setMinimum(0);
        subpix_winSize->setMaximum(999999);
        subpix_winSize->setSingleStep(1);
        subpix_winSize->setValue(5);

        gridLayout_32->addWidget(subpix_winSize, 0, 0, 1, 1);

        label_200 = new QLabel(groupBox_9);
        label_200->setObjectName(QString::fromUtf8("label_200"));
        label_200->setWordWrap(true);

        gridLayout_32->addWidget(label_200, 0, 1, 1, 1);

        subpix_iterations = new QSpinBox(groupBox_9);
        subpix_iterations->setObjectName(QString::fromUtf8("subpix_iterations"));
        subpix_iterations->setMinimum(0);
        subpix_iterations->setMaximum(999999);
        subpix_iterations->setSingleStep(1);
        subpix_iterations->setValue(20);

        gridLayout_32->addWidget(subpix_iterations, 1, 0, 1, 1);

        label_203 = new QLabel(groupBox_9);
        label_203->setObjectName(QString::fromUtf8("label_203"));
        label_203->setWordWrap(true);

        gridLayout_32->addWidget(label_203, 1, 1, 1, 1);

        subpix_eps = new QDoubleSpinBox(groupBox_9);
        subpix_eps->setObjectName(QString::fromUtf8("subpix_eps"));
        subpix_eps->setDecimals(3);
        subpix_eps->setMinimum(0.001);
        subpix_eps->setMaximum(0.1);
        subpix_eps->setSingleStep(0.01);
        subpix_eps->setValue(0.03);

        gridLayout_32->addWidget(subpix_eps, 2, 0, 1, 1);

        label_204 = new QLabel(groupBox_9);
        label_204->setObjectName(QString::fromUtf8("label_204"));
        label_204->setWordWrap(true);

        gridLayout_32->addWidget(label_204, 2, 1, 1, 1);

        gridLayout_32->setColumnStretch(1, 1);

        verticalLayout_59->addLayout(gridLayout_32);


        verticalLayout_12->addWidget(groupBox_9);


        verticalLayout_58->addWidget(groupBox_signature_keypoint1);

        verticalSpacer_15 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_58->addItem(verticalSpacer_15);

        stackedWidget->addWidget(page_9);
        page_13 = new QWidget();
        page_13->setObjectName(QString::fromUtf8("page_13"));
        verticalLayout_38 = new QVBoxLayout(page_13);
        verticalLayout_38->setObjectName(QString::fromUtf8("verticalLayout_38"));
        groupBox_detector_surf2 = new QGroupBox(page_13);
        groupBox_detector_surf2->setObjectName(QString::fromUtf8("groupBox_detector_surf2"));
        gridLayout_15 = new QGridLayout(groupBox_detector_surf2);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        surf_checkBox_upright = new QCheckBox(groupBox_detector_surf2);
        surf_checkBox_upright->setObjectName(QString::fromUtf8("surf_checkBox_upright"));
        surf_checkBox_upright->setEnabled(true);

        gridLayout_15->addWidget(surf_checkBox_upright, 4, 0, 1, 1);

        label_41 = new QLabel(groupBox_detector_surf2);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_15->addWidget(label_41, 2, 1, 1, 1);

        label_8 = new QLabel(groupBox_detector_surf2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setWordWrap(true);

        gridLayout_15->addWidget(label_8, 3, 1, 1, 1);

        label_surf_checkBox_gpuKeypointsRatio = new QLabel(groupBox_detector_surf2);
        label_surf_checkBox_gpuKeypointsRatio->setObjectName(QString::fromUtf8("label_surf_checkBox_gpuKeypointsRatio"));
        label_surf_checkBox_gpuKeypointsRatio->setEnabled(true);
        label_surf_checkBox_gpuKeypointsRatio->setWordWrap(true);

        gridLayout_15->addWidget(label_surf_checkBox_gpuKeypointsRatio, 6, 1, 1, 1);

        label_surf_checkBox_gpuVersion = new QLabel(groupBox_detector_surf2);
        label_surf_checkBox_gpuVersion->setObjectName(QString::fromUtf8("label_surf_checkBox_gpuVersion"));
        label_surf_checkBox_gpuVersion->setEnabled(true);
        label_surf_checkBox_gpuVersion->setWordWrap(true);

        gridLayout_15->addWidget(label_surf_checkBox_gpuVersion, 5, 1, 1, 1);

        surf_spinBox_octaves = new QSpinBox(groupBox_detector_surf2);
        surf_spinBox_octaves->setObjectName(QString::fromUtf8("surf_spinBox_octaves"));
        surf_spinBox_octaves->setMinimum(1);
        surf_spinBox_octaves->setMaximum(100000);
        surf_spinBox_octaves->setValue(4);

        gridLayout_15->addWidget(surf_spinBox_octaves, 1, 0, 1, 1);

        surf_checkBox_gpuVersion = new QCheckBox(groupBox_detector_surf2);
        surf_checkBox_gpuVersion->setObjectName(QString::fromUtf8("surf_checkBox_gpuVersion"));
        surf_checkBox_gpuVersion->setEnabled(true);

        gridLayout_15->addWidget(surf_checkBox_gpuVersion, 5, 0, 1, 1);

        label_49 = new QLabel(groupBox_detector_surf2);
        label_49->setObjectName(QString::fromUtf8("label_49"));

        gridLayout_15->addWidget(label_49, 4, 1, 1, 1);

        surf_doubleSpinBox_gpuKeypointsRatio = new QDoubleSpinBox(groupBox_detector_surf2);
        surf_doubleSpinBox_gpuKeypointsRatio->setObjectName(QString::fromUtf8("surf_doubleSpinBox_gpuKeypointsRatio"));
        surf_doubleSpinBox_gpuKeypointsRatio->setValue(0.01);

        gridLayout_15->addWidget(surf_doubleSpinBox_gpuKeypointsRatio, 6, 0, 1, 1);

        surf_doubleSpinBox_hessianThr = new QDoubleSpinBox(groupBox_detector_surf2);
        surf_doubleSpinBox_hessianThr->setObjectName(QString::fromUtf8("surf_doubleSpinBox_hessianThr"));
        surf_doubleSpinBox_hessianThr->setDecimals(0);
        surf_doubleSpinBox_hessianThr->setMaximum(50000);
        surf_doubleSpinBox_hessianThr->setValue(500);

        gridLayout_15->addWidget(surf_doubleSpinBox_hessianThr, 0, 0, 1, 1);

        checkBox_surfExtended = new QCheckBox(groupBox_detector_surf2);
        checkBox_surfExtended->setObjectName(QString::fromUtf8("checkBox_surfExtended"));

        gridLayout_15->addWidget(checkBox_surfExtended, 3, 0, 1, 1);

        surf_spinBox_octaveLayers = new QSpinBox(groupBox_detector_surf2);
        surf_spinBox_octaveLayers->setObjectName(QString::fromUtf8("surf_spinBox_octaveLayers"));
        surf_spinBox_octaveLayers->setMinimum(1);
        surf_spinBox_octaveLayers->setMaximum(100000);
        surf_spinBox_octaveLayers->setValue(2);

        gridLayout_15->addWidget(surf_spinBox_octaveLayers, 2, 0, 1, 1);

        label_26 = new QLabel(groupBox_detector_surf2);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_15->addWidget(label_26, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox_detector_surf2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_15->addWidget(label_4, 0, 1, 1, 1);

        gridLayout_15->setColumnStretch(1, 1);

        verticalLayout_38->addWidget(groupBox_detector_surf2);

        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_38->addItem(verticalSpacer_17);

        stackedWidget->addWidget(page_13);
        page_14 = new QWidget();
        page_14->setObjectName(QString::fromUtf8("page_14"));
        verticalLayout_39 = new QVBoxLayout(page_14);
        verticalLayout_39->setObjectName(QString::fromUtf8("verticalLayout_39"));
        groupBox_detector_sift2 = new QGroupBox(page_14);
        groupBox_detector_sift2->setObjectName(QString::fromUtf8("groupBox_detector_sift2"));
        gridLayout_19 = new QGridLayout(groupBox_detector_sift2);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        sift_spinBox_nFeatures = new QSpinBox(groupBox_detector_sift2);
        sift_spinBox_nFeatures->setObjectName(QString::fromUtf8("sift_spinBox_nFeatures"));

        gridLayout_19->addWidget(sift_spinBox_nFeatures, 0, 0, 1, 1);

        label_129 = new QLabel(groupBox_detector_sift2);
        label_129->setObjectName(QString::fromUtf8("label_129"));

        gridLayout_19->addWidget(label_129, 4, 1, 1, 1);

        sift_doubleSpinBox_contrastThr = new QDoubleSpinBox(groupBox_detector_sift2);
        sift_doubleSpinBox_contrastThr->setObjectName(QString::fromUtf8("sift_doubleSpinBox_contrastThr"));
        sift_doubleSpinBox_contrastThr->setDecimals(6);
        sift_doubleSpinBox_contrastThr->setMinimum(0);
        sift_doubleSpinBox_contrastThr->setMaximum(50000);
        sift_doubleSpinBox_contrastThr->setSingleStep(0.001);
        sift_doubleSpinBox_contrastThr->setValue(0.006667);

        gridLayout_19->addWidget(sift_doubleSpinBox_contrastThr, 2, 0, 1, 1);

        label_5 = new QLabel(groupBox_detector_sift2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_19->addWidget(label_5, 2, 1, 1, 1);

        label_30 = new QLabel(groupBox_detector_sift2);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_19->addWidget(label_30, 3, 1, 1, 1);

        sift_doubleSpinBox_edgeThr = new QDoubleSpinBox(groupBox_detector_sift2);
        sift_doubleSpinBox_edgeThr->setObjectName(QString::fromUtf8("sift_doubleSpinBox_edgeThr"));
        sift_doubleSpinBox_edgeThr->setSingleStep(0.1);
        sift_doubleSpinBox_edgeThr->setValue(10);

        gridLayout_19->addWidget(sift_doubleSpinBox_edgeThr, 3, 0, 1, 1);

        sift_doubleSpinBox_sigma = new QDoubleSpinBox(groupBox_detector_sift2);
        sift_doubleSpinBox_sigma->setObjectName(QString::fromUtf8("sift_doubleSpinBox_sigma"));
        sift_doubleSpinBox_sigma->setSingleStep(0.1);
        sift_doubleSpinBox_sigma->setValue(10);

        gridLayout_19->addWidget(sift_doubleSpinBox_sigma, 4, 0, 1, 1);

        sift_spinBox_nOctaveLayers = new QSpinBox(groupBox_detector_sift2);
        sift_spinBox_nOctaveLayers->setObjectName(QString::fromUtf8("sift_spinBox_nOctaveLayers"));

        gridLayout_19->addWidget(sift_spinBox_nOctaveLayers, 1, 0, 1, 1);

        label_130 = new QLabel(groupBox_detector_sift2);
        label_130->setObjectName(QString::fromUtf8("label_130"));

        gridLayout_19->addWidget(label_130, 1, 1, 1, 1);

        label_131 = new QLabel(groupBox_detector_sift2);
        label_131->setObjectName(QString::fromUtf8("label_131"));

        gridLayout_19->addWidget(label_131, 0, 1, 1, 1);

        gridLayout_19->setColumnStretch(1, 1);

        verticalLayout_39->addWidget(groupBox_detector_sift2);

        verticalSpacer_19 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_39->addItem(verticalSpacer_19);

        stackedWidget->addWidget(page_14);
        page_17 = new QWidget();
        page_17->setObjectName(QString::fromUtf8("page_17"));
        verticalLayout_14 = new QVBoxLayout(page_17);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        groupBox_detector_fast2 = new QGroupBox(page_17);
        groupBox_detector_fast2->setObjectName(QString::fromUtf8("groupBox_detector_fast2"));
        groupBox_detector_fast2->setCheckable(false);
        verticalLayout = new QVBoxLayout(groupBox_detector_fast2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        fastThreshold = new QSpinBox(groupBox_detector_fast2);
        fastThreshold->setObjectName(QString::fromUtf8("fastThreshold"));
        fastThreshold->setMinimum(1);
        fastThreshold->setMaximum(9999);
        fastThreshold->setValue(50);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, fastThreshold);

        label_56 = new QLabel(groupBox_detector_fast2);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setWordWrap(true);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, label_56);

        fastSuppressNonMax = new QCheckBox(groupBox_detector_fast2);
        fastSuppressNonMax->setObjectName(QString::fromUtf8("fastSuppressNonMax"));
        fastSuppressNonMax->setChecked(true);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, fastSuppressNonMax);

        label_69 = new QLabel(groupBox_detector_fast2);
        label_69->setObjectName(QString::fromUtf8("label_69"));
        label_69->setWordWrap(true);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, label_69);

        fastGpu = new QCheckBox(groupBox_detector_fast2);
        fastGpu->setObjectName(QString::fromUtf8("fastGpu"));
        fastGpu->setChecked(true);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, fastGpu);

        label_fastGPU = new QLabel(groupBox_detector_fast2);
        label_fastGPU->setObjectName(QString::fromUtf8("label_fastGPU"));
        label_fastGPU->setWordWrap(true);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, label_fastGPU);

        fastKeypointRatio = new QDoubleSpinBox(groupBox_detector_fast2);
        fastKeypointRatio->setObjectName(QString::fromUtf8("fastKeypointRatio"));
        fastKeypointRatio->setMaximum(1);
        fastKeypointRatio->setSingleStep(0.01);
        fastKeypointRatio->setValue(0.05);

        formLayout_2->setWidget(3, QFormLayout::LabelRole, fastKeypointRatio);

        label_fastGPUKptRatio = new QLabel(groupBox_detector_fast2);
        label_fastGPUKptRatio->setObjectName(QString::fromUtf8("label_fastGPUKptRatio"));
        label_fastGPUKptRatio->setWordWrap(true);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, label_fastGPUKptRatio);


        verticalLayout->addLayout(formLayout_2);

        verticalSpacer_16 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_16);


        verticalLayout_14->addWidget(groupBox_detector_fast2);

        stackedWidget->addWidget(page_17);
        page_15 = new QWidget();
        page_15->setObjectName(QString::fromUtf8("page_15"));
        verticalLayout_40 = new QVBoxLayout(page_15);
        verticalLayout_40->setObjectName(QString::fromUtf8("verticalLayout_40"));
        groupBox_detector_brief2 = new QGroupBox(page_15);
        groupBox_detector_brief2->setObjectName(QString::fromUtf8("groupBox_detector_brief2"));
        gridLayout_16 = new QGridLayout(groupBox_detector_brief2);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        briefBytes = new QSpinBox(groupBox_detector_brief2);
        briefBytes->setObjectName(QString::fromUtf8("briefBytes"));
        briefBytes->setMinimum(1);
        briefBytes->setMaximum(1000);
        briefBytes->setValue(32);

        gridLayout_16->addWidget(briefBytes, 0, 0, 1, 1);

        label_52 = new QLabel(groupBox_detector_brief2);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setWordWrap(true);

        gridLayout_16->addWidget(label_52, 0, 1, 1, 1);

        gridLayout_16->setColumnStretch(1, 1);

        verticalLayout_40->addWidget(groupBox_detector_brief2);

        verticalSpacer_22 = new QSpacerItem(20, 660, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_40->addItem(verticalSpacer_22);

        stackedWidget->addWidget(page_15);
        page_18 = new QWidget();
        page_18->setObjectName(QString::fromUtf8("page_18"));
        verticalLayout_35 = new QVBoxLayout(page_18);
        verticalLayout_35->setObjectName(QString::fromUtf8("verticalLayout_35"));
        groupBox_detector_orb2 = new QGroupBox(page_18);
        groupBox_detector_orb2->setObjectName(QString::fromUtf8("groupBox_detector_orb2"));
        gridLayout_14 = new QGridLayout(groupBox_detector_orb2);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        doubleSpinBox_ORBScaleFactor = new QDoubleSpinBox(groupBox_detector_orb2);
        doubleSpinBox_ORBScaleFactor->setObjectName(QString::fromUtf8("doubleSpinBox_ORBScaleFactor"));
        doubleSpinBox_ORBScaleFactor->setDecimals(1);
        doubleSpinBox_ORBScaleFactor->setValue(1.2);

        gridLayout_14->addWidget(doubleSpinBox_ORBScaleFactor, 0, 0, 1, 1);

        label_32 = new QLabel(groupBox_detector_orb2);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setWordWrap(true);

        gridLayout_14->addWidget(label_32, 0, 1, 1, 1);

        spinBox_ORBNLevels = new QSpinBox(groupBox_detector_orb2);
        spinBox_ORBNLevels->setObjectName(QString::fromUtf8("spinBox_ORBNLevels"));
        spinBox_ORBNLevels->setValue(8);

        gridLayout_14->addWidget(spinBox_ORBNLevels, 1, 0, 1, 1);

        label_43 = new QLabel(groupBox_detector_orb2);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setWordWrap(true);

        gridLayout_14->addWidget(label_43, 1, 1, 1, 1);

        spinBox_ORBEdgeThreshold = new QSpinBox(groupBox_detector_orb2);
        spinBox_ORBEdgeThreshold->setObjectName(QString::fromUtf8("spinBox_ORBEdgeThreshold"));
        spinBox_ORBEdgeThreshold->setValue(31);

        gridLayout_14->addWidget(spinBox_ORBEdgeThreshold, 2, 0, 1, 1);

        label_95 = new QLabel(groupBox_detector_orb2);
        label_95->setObjectName(QString::fromUtf8("label_95"));
        label_95->setWordWrap(true);

        gridLayout_14->addWidget(label_95, 2, 1, 1, 1);

        spinBox_ORBFirstLevel = new QSpinBox(groupBox_detector_orb2);
        spinBox_ORBFirstLevel->setObjectName(QString::fromUtf8("spinBox_ORBFirstLevel"));
        spinBox_ORBFirstLevel->setMaximum(0);

        gridLayout_14->addWidget(spinBox_ORBFirstLevel, 3, 0, 1, 1);

        label_127 = new QLabel(groupBox_detector_orb2);
        label_127->setObjectName(QString::fromUtf8("label_127"));
        label_127->setWordWrap(true);

        gridLayout_14->addWidget(label_127, 3, 1, 1, 1);

        spinBox_ORBWTA_K = new QSpinBox(groupBox_detector_orb2);
        spinBox_ORBWTA_K->setObjectName(QString::fromUtf8("spinBox_ORBWTA_K"));
        spinBox_ORBWTA_K->setValue(2);

        gridLayout_14->addWidget(spinBox_ORBWTA_K, 4, 0, 1, 1);

        label_128 = new QLabel(groupBox_detector_orb2);
        label_128->setObjectName(QString::fromUtf8("label_128"));
        label_128->setWordWrap(true);

        gridLayout_14->addWidget(label_128, 4, 1, 1, 1);

        spinBox_ORBScoreType = new QSpinBox(groupBox_detector_orb2);
        spinBox_ORBScoreType->setObjectName(QString::fromUtf8("spinBox_ORBScoreType"));

        gridLayout_14->addWidget(spinBox_ORBScoreType, 5, 0, 1, 1);

        label_140 = new QLabel(groupBox_detector_orb2);
        label_140->setObjectName(QString::fromUtf8("label_140"));
        label_140->setWordWrap(true);

        gridLayout_14->addWidget(label_140, 5, 1, 1, 1);

        spinBox_ORBPatchSize = new QSpinBox(groupBox_detector_orb2);
        spinBox_ORBPatchSize->setObjectName(QString::fromUtf8("spinBox_ORBPatchSize"));
        spinBox_ORBPatchSize->setValue(31);

        gridLayout_14->addWidget(spinBox_ORBPatchSize, 6, 0, 1, 1);

        label_147 = new QLabel(groupBox_detector_orb2);
        label_147->setObjectName(QString::fromUtf8("label_147"));
        label_147->setWordWrap(true);

        gridLayout_14->addWidget(label_147, 6, 1, 1, 1);

        checkBox_ORBGpu = new QCheckBox(groupBox_detector_orb2);
        checkBox_ORBGpu->setObjectName(QString::fromUtf8("checkBox_ORBGpu"));

        gridLayout_14->addWidget(checkBox_ORBGpu, 7, 0, 1, 1);

        label_orbGpu = new QLabel(groupBox_detector_orb2);
        label_orbGpu->setObjectName(QString::fromUtf8("label_orbGpu"));
        label_orbGpu->setWordWrap(true);

        gridLayout_14->addWidget(label_orbGpu, 7, 1, 1, 1);

        gridLayout_14->setColumnStretch(1, 1);

        verticalLayout_35->addWidget(groupBox_detector_orb2);

        verticalSpacer_18 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_35->addItem(verticalSpacer_18);

        stackedWidget->addWidget(page_18);
        page_11 = new QWidget();
        page_11->setObjectName(QString::fromUtf8("page_11"));
        verticalLayout_36 = new QVBoxLayout(page_11);
        verticalLayout_36->setObjectName(QString::fromUtf8("verticalLayout_36"));
        groupBox_detector_freak2 = new QGroupBox(page_11);
        groupBox_detector_freak2->setObjectName(QString::fromUtf8("groupBox_detector_freak2"));
        gridLayout_13 = new QGridLayout(groupBox_detector_freak2);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        checkBox_FREAKOrientationNormalized = new QCheckBox(groupBox_detector_freak2);
        checkBox_FREAKOrientationNormalized->setObjectName(QString::fromUtf8("checkBox_FREAKOrientationNormalized"));
        checkBox_FREAKOrientationNormalized->setChecked(true);

        gridLayout_13->addWidget(checkBox_FREAKOrientationNormalized, 0, 0, 1, 1);

        label_6 = new QLabel(groupBox_detector_freak2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setWordWrap(true);

        gridLayout_13->addWidget(label_6, 0, 1, 1, 1);

        checkBox_FREAKScaleNormalized = new QCheckBox(groupBox_detector_freak2);
        checkBox_FREAKScaleNormalized->setObjectName(QString::fromUtf8("checkBox_FREAKScaleNormalized"));
        checkBox_FREAKScaleNormalized->setChecked(true);

        gridLayout_13->addWidget(checkBox_FREAKScaleNormalized, 1, 0, 1, 1);

        label_9 = new QLabel(groupBox_detector_freak2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setWordWrap(true);

        gridLayout_13->addWidget(label_9, 1, 1, 1, 1);

        doubleSpinBox_FREAKPatternScale = new QDoubleSpinBox(groupBox_detector_freak2);
        doubleSpinBox_FREAKPatternScale->setObjectName(QString::fromUtf8("doubleSpinBox_FREAKPatternScale"));
        doubleSpinBox_FREAKPatternScale->setValue(22);

        gridLayout_13->addWidget(doubleSpinBox_FREAKPatternScale, 2, 0, 1, 1);

        label_13 = new QLabel(groupBox_detector_freak2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setWordWrap(true);

        gridLayout_13->addWidget(label_13, 2, 1, 1, 1);

        spinBox_FREAKNOctaves = new QSpinBox(groupBox_detector_freak2);
        spinBox_FREAKNOctaves->setObjectName(QString::fromUtf8("spinBox_FREAKNOctaves"));
        spinBox_FREAKNOctaves->setValue(4);

        gridLayout_13->addWidget(spinBox_FREAKNOctaves, 3, 0, 1, 1);

        label_23 = new QLabel(groupBox_detector_freak2);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setWordWrap(true);

        gridLayout_13->addWidget(label_23, 3, 1, 1, 1);

        gridLayout_13->setColumnStretch(1, 1);

        verticalLayout_36->addWidget(groupBox_detector_freak2);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_36->addItem(verticalSpacer_13);

        stackedWidget->addWidget(page_11);
        page_16 = new QWidget();
        page_16->setObjectName(QString::fromUtf8("page_16"));
        verticalLayout_9 = new QVBoxLayout(page_16);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        groupBox_detector_gftt2 = new QGroupBox(page_16);
        groupBox_detector_gftt2->setObjectName(QString::fromUtf8("groupBox_detector_gftt2"));
        gridLayout_21 = new QGridLayout(groupBox_detector_gftt2);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        doubleSpinBox_GFTT_qualityLevel = new QDoubleSpinBox(groupBox_detector_gftt2);
        doubleSpinBox_GFTT_qualityLevel->setObjectName(QString::fromUtf8("doubleSpinBox_GFTT_qualityLevel"));
        doubleSpinBox_GFTT_qualityLevel->setMaximum(1);
        doubleSpinBox_GFTT_qualityLevel->setSingleStep(0.01);
        doubleSpinBox_GFTT_qualityLevel->setValue(0.01);

        gridLayout_21->addWidget(doubleSpinBox_GFTT_qualityLevel, 0, 0, 1, 1);

        doubleSpinBox_GFTT_minDistance = new QDoubleSpinBox(groupBox_detector_gftt2);
        doubleSpinBox_GFTT_minDistance->setObjectName(QString::fromUtf8("doubleSpinBox_GFTT_minDistance"));
        doubleSpinBox_GFTT_minDistance->setDecimals(1);
        doubleSpinBox_GFTT_minDistance->setValue(1);

        gridLayout_21->addWidget(doubleSpinBox_GFTT_minDistance, 1, 0, 1, 1);

        checkBox_GFTT_useHarrisDetector = new QCheckBox(groupBox_detector_gftt2);
        checkBox_GFTT_useHarrisDetector->setObjectName(QString::fromUtf8("checkBox_GFTT_useHarrisDetector"));
        checkBox_GFTT_useHarrisDetector->setChecked(true);

        gridLayout_21->addWidget(checkBox_GFTT_useHarrisDetector, 3, 0, 1, 1);

        label_172 = new QLabel(groupBox_detector_gftt2);
        label_172->setObjectName(QString::fromUtf8("label_172"));
        label_172->setWordWrap(true);

        gridLayout_21->addWidget(label_172, 3, 1, 1, 1);

        doubleSpinBox_GFTT_k = new QDoubleSpinBox(groupBox_detector_gftt2);
        doubleSpinBox_GFTT_k->setObjectName(QString::fromUtf8("doubleSpinBox_GFTT_k"));
        doubleSpinBox_GFTT_k->setMaximum(1);
        doubleSpinBox_GFTT_k->setSingleStep(0.01);
        doubleSpinBox_GFTT_k->setValue(0.04);

        gridLayout_21->addWidget(doubleSpinBox_GFTT_k, 4, 0, 1, 1);

        label_173 = new QLabel(groupBox_detector_gftt2);
        label_173->setObjectName(QString::fromUtf8("label_173"));
        label_173->setWordWrap(true);

        gridLayout_21->addWidget(label_173, 4, 1, 1, 1);

        spinBox_GFTT_blockSize = new QSpinBox(groupBox_detector_gftt2);
        spinBox_GFTT_blockSize->setObjectName(QString::fromUtf8("spinBox_GFTT_blockSize"));
        spinBox_GFTT_blockSize->setValue(3);

        gridLayout_21->addWidget(spinBox_GFTT_blockSize, 2, 0, 1, 1);

        label_175 = new QLabel(groupBox_detector_gftt2);
        label_175->setObjectName(QString::fromUtf8("label_175"));
        label_175->setWordWrap(true);

        gridLayout_21->addWidget(label_175, 0, 1, 1, 1);

        label_176 = new QLabel(groupBox_detector_gftt2);
        label_176->setObjectName(QString::fromUtf8("label_176"));
        label_176->setWordWrap(true);

        gridLayout_21->addWidget(label_176, 1, 1, 1, 1);

        label_177 = new QLabel(groupBox_detector_gftt2);
        label_177->setObjectName(QString::fromUtf8("label_177"));
        label_177->setWordWrap(true);

        gridLayout_21->addWidget(label_177, 2, 1, 1, 1);

        gridLayout_21->setColumnStretch(1, 1);

        verticalLayout_9->addWidget(groupBox_detector_gftt2);

        verticalSpacer_25 = new QSpacerItem(20, 644, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_25);

        stackedWidget->addWidget(page_16);
        page_25 = new QWidget();
        page_25->setObjectName(QString::fromUtf8("page_25"));
        verticalLayout_47 = new QVBoxLayout(page_25);
        verticalLayout_47->setObjectName(QString::fromUtf8("verticalLayout_47"));
        groupBox_detector_brisk2 = new QGroupBox(page_25);
        groupBox_detector_brisk2->setObjectName(QString::fromUtf8("groupBox_detector_brisk2"));
        gridLayout_26 = new QGridLayout(groupBox_detector_brisk2);
        gridLayout_26->setObjectName(QString::fromUtf8("gridLayout_26"));
        label_191 = new QLabel(groupBox_detector_brisk2);
        label_191->setObjectName(QString::fromUtf8("label_191"));
        label_191->setWordWrap(true);

        gridLayout_26->addWidget(label_191, 0, 1, 1, 1);

        doubleSpinBox_BRISK_patterScale = new QDoubleSpinBox(groupBox_detector_brisk2);
        doubleSpinBox_BRISK_patterScale->setObjectName(QString::fromUtf8("doubleSpinBox_BRISK_patterScale"));
        doubleSpinBox_BRISK_patterScale->setMaximum(10);
        doubleSpinBox_BRISK_patterScale->setSingleStep(0.1);
        doubleSpinBox_BRISK_patterScale->setValue(1);

        gridLayout_26->addWidget(doubleSpinBox_BRISK_patterScale, 2, 0, 1, 1);

        label_188 = new QLabel(groupBox_detector_brisk2);
        label_188->setObjectName(QString::fromUtf8("label_188"));
        label_188->setWordWrap(true);

        gridLayout_26->addWidget(label_188, 1, 1, 1, 1);

        spinBox_BRISK_octaves = new QSpinBox(groupBox_detector_brisk2);
        spinBox_BRISK_octaves->setObjectName(QString::fromUtf8("spinBox_BRISK_octaves"));
        spinBox_BRISK_octaves->setMaximum(10);
        spinBox_BRISK_octaves->setValue(3);

        gridLayout_26->addWidget(spinBox_BRISK_octaves, 1, 0, 1, 1);

        label_189 = new QLabel(groupBox_detector_brisk2);
        label_189->setObjectName(QString::fromUtf8("label_189"));
        label_189->setWordWrap(true);

        gridLayout_26->addWidget(label_189, 2, 1, 1, 1);

        spinBox_BRISK_thresh = new QSpinBox(groupBox_detector_brisk2);
        spinBox_BRISK_thresh->setObjectName(QString::fromUtf8("spinBox_BRISK_thresh"));
        spinBox_BRISK_thresh->setMaximum(9999);
        spinBox_BRISK_thresh->setValue(30);

        gridLayout_26->addWidget(spinBox_BRISK_thresh, 0, 0, 1, 1);

        gridLayout_26->setColumnStretch(1, 1);

        verticalLayout_47->addWidget(groupBox_detector_brisk2);

        verticalSpacer_26 = new QSpacerItem(20, 742, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_47->addItem(verticalSpacer_26);

        stackedWidget->addWidget(page_25);
        page_19 = new QWidget();
        page_19->setObjectName(QString::fromUtf8("page_19"));
        verticalLayout_13 = new QVBoxLayout(page_19);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        groupBox_vh_strategy1 = new QGroupBox(page_19);
        groupBox_vh_strategy1->setObjectName(QString::fromUtf8("groupBox_vh_strategy1"));
        groupBox_vh_strategy1->setMinimumSize(QSize(0, 0));
        verticalLayout_3 = new QVBoxLayout(groupBox_vh_strategy1);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        comboBox_vh_strategy = new QComboBox(groupBox_vh_strategy1);
        comboBox_vh_strategy->setObjectName(QString::fromUtf8("comboBox_vh_strategy"));
        comboBox_vh_strategy->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_10->addWidget(comboBox_vh_strategy, 0, 0, 1, 1);

        label_50 = new QLabel(groupBox_vh_strategy1);
        label_50->setObjectName(QString::fromUtf8("label_50"));

        gridLayout_10->addWidget(label_50, 0, 1, 1, 1);

        gridLayout_10->setColumnStretch(1, 1);

        verticalLayout_3->addLayout(gridLayout_10);

        groupBox_vh_epipolar2 = new QGroupBox(groupBox_vh_strategy1);
        groupBox_vh_epipolar2->setObjectName(QString::fromUtf8("groupBox_vh_epipolar2"));
        gridLayout = new QGridLayout(groupBox_vh_epipolar2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        surf_spinBox_matchCountMinAccepted = new QSpinBox(groupBox_vh_epipolar2);
        surf_spinBox_matchCountMinAccepted->setObjectName(QString::fromUtf8("surf_spinBox_matchCountMinAccepted"));
        surf_spinBox_matchCountMinAccepted->setMinimum(8);
        surf_spinBox_matchCountMinAccepted->setMaximum(100000);
        surf_spinBox_matchCountMinAccepted->setValue(11);

        gridLayout->addWidget(surf_spinBox_matchCountMinAccepted, 0, 0, 1, 1);

        label_25 = new QLabel(groupBox_vh_epipolar2);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setWordWrap(true);

        gridLayout->addWidget(label_25, 0, 1, 1, 1);

        surf_doubleSpinBox_ransacParam1 = new QDoubleSpinBox(groupBox_vh_epipolar2);
        surf_doubleSpinBox_ransacParam1->setObjectName(QString::fromUtf8("surf_doubleSpinBox_ransacParam1"));
        surf_doubleSpinBox_ransacParam1->setDecimals(1);
        surf_doubleSpinBox_ransacParam1->setMaximum(10);
        surf_doubleSpinBox_ransacParam1->setSingleStep(0.1);
        surf_doubleSpinBox_ransacParam1->setValue(3);

        gridLayout->addWidget(surf_doubleSpinBox_ransacParam1, 1, 0, 1, 1);

        label_27 = new QLabel(groupBox_vh_epipolar2);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setWordWrap(true);

        gridLayout->addWidget(label_27, 1, 1, 1, 1);

        surf_doubleSpinBox_ransacParam2 = new QDoubleSpinBox(groupBox_vh_epipolar2);
        surf_doubleSpinBox_ransacParam2->setObjectName(QString::fromUtf8("surf_doubleSpinBox_ransacParam2"));
        surf_doubleSpinBox_ransacParam2->setDecimals(2);
        surf_doubleSpinBox_ransacParam2->setMaximum(0.99);
        surf_doubleSpinBox_ransacParam2->setSingleStep(0.01);
        surf_doubleSpinBox_ransacParam2->setValue(0.99);

        gridLayout->addWidget(surf_doubleSpinBox_ransacParam2, 2, 0, 1, 1);

        label_28 = new QLabel(groupBox_vh_epipolar2);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setWordWrap(true);

        gridLayout->addWidget(label_28, 2, 1, 1, 1);

        gridLayout->setColumnStretch(1, 10);

        verticalLayout_3->addWidget(groupBox_vh_epipolar2);


        verticalLayout_13->addWidget(groupBox_vh_strategy1);

        verticalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_3);

        stackedWidget->addWidget(page_19);
        page_10 = new QWidget();
        page_10->setObjectName(QString::fromUtf8("page_10"));
        verticalLayout_19 = new QVBoxLayout(page_10);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        groupBox_slam1 = new QGroupBox(page_10);
        groupBox_slam1->setObjectName(QString::fromUtf8("groupBox_slam1"));
        groupBox_slam1->setCheckable(false);
        verticalLayout_25 = new QVBoxLayout(groupBox_slam1);
        verticalLayout_25->setObjectName(QString::fromUtf8("verticalLayout_25"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        general_checkBox_activateRGBD = new QCheckBox(groupBox_slam1);
        general_checkBox_activateRGBD->setObjectName(QString::fromUtf8("general_checkBox_activateRGBD"));

        horizontalLayout_4->addWidget(general_checkBox_activateRGBD);

        label_activateRGBD = new QLabel(groupBox_slam1);
        label_activateRGBD->setObjectName(QString::fromUtf8("label_activateRGBD"));
        label_activateRGBD->setWordWrap(true);

        horizontalLayout_4->addWidget(label_activateRGBD);

        horizontalLayout_4->setStretch(1, 1);

        verticalLayout_25->addLayout(horizontalLayout_4);

        label_83 = new QLabel(groupBox_slam1);
        label_83->setObjectName(QString::fromUtf8("label_83"));
        label_83->setWordWrap(true);

        verticalLayout_25->addWidget(label_83);

        groupBox_slam_update = new QGroupBox(groupBox_slam1);
        groupBox_slam_update->setObjectName(QString::fromUtf8("groupBox_slam_update"));
        gridLayout_47 = new QGridLayout(groupBox_slam_update);
        gridLayout_47->setObjectName(QString::fromUtf8("gridLayout_47"));
        rgdb_linearUpdate = new QDoubleSpinBox(groupBox_slam_update);
        rgdb_linearUpdate->setObjectName(QString::fromUtf8("rgdb_linearUpdate"));
        rgdb_linearUpdate->setDecimals(3);
        rgdb_linearUpdate->setSingleStep(0.1);

        gridLayout_47->addWidget(rgdb_linearUpdate, 0, 0, 1, 1);

        label_153 = new QLabel(groupBox_slam_update);
        label_153->setObjectName(QString::fromUtf8("label_153"));
        label_153->setWordWrap(true);

        gridLayout_47->addWidget(label_153, 0, 1, 1, 1);

        rgdb_angularUpdate = new QDoubleSpinBox(groupBox_slam_update);
        rgdb_angularUpdate->setObjectName(QString::fromUtf8("rgdb_angularUpdate"));
        rgdb_angularUpdate->setDecimals(2);
        rgdb_angularUpdate->setMaximum(3.14);
        rgdb_angularUpdate->setSingleStep(0.1);

        gridLayout_47->addWidget(rgdb_angularUpdate, 1, 0, 1, 1);

        label_152 = new QLabel(groupBox_slam_update);
        label_152->setObjectName(QString::fromUtf8("label_152"));
        label_152->setWordWrap(true);

        gridLayout_47->addWidget(label_152, 1, 1, 1, 1);

        rgdb_newMapOdomChange = new QDoubleSpinBox(groupBox_slam_update);
        rgdb_newMapOdomChange->setObjectName(QString::fromUtf8("rgdb_newMapOdomChange"));
        rgdb_newMapOdomChange->setDecimals(1);
        rgdb_newMapOdomChange->setMaximum(99);
        rgdb_newMapOdomChange->setSingleStep(0.1);
        rgdb_newMapOdomChange->setValue(1);

        gridLayout_47->addWidget(rgdb_newMapOdomChange, 2, 0, 1, 1);

        label_163 = new QLabel(groupBox_slam_update);
        label_163->setObjectName(QString::fromUtf8("label_163"));
        label_163->setWordWrap(true);

        gridLayout_47->addWidget(label_163, 2, 1, 1, 1);

        odomScanHistory = new QCheckBox(groupBox_slam_update);
        odomScanHistory->setObjectName(QString::fromUtf8("odomScanHistory"));

        gridLayout_47->addWidget(odomScanHistory, 3, 0, 1, 1);

        label_scanMatching = new QLabel(groupBox_slam_update);
        label_scanMatching->setObjectName(QString::fromUtf8("label_scanMatching"));
        label_scanMatching->setWordWrap(true);

        gridLayout_47->addWidget(label_scanMatching, 3, 1, 1, 1);

        label_scanMatching_3 = new QLabel(groupBox_slam_update);
        label_scanMatching_3->setObjectName(QString::fromUtf8("label_scanMatching_3"));
        label_scanMatching_3->setWordWrap(true);

        gridLayout_47->addWidget(label_scanMatching_3, 5, 1, 1, 1);

        spinBox_maxLocalLocationsRetrieved = new QSpinBox(groupBox_slam_update);
        spinBox_maxLocalLocationsRetrieved->setObjectName(QString::fromUtf8("spinBox_maxLocalLocationsRetrieved"));

        gridLayout_47->addWidget(spinBox_maxLocalLocationsRetrieved, 5, 0, 1, 1);

        localDetection_radius = new QDoubleSpinBox(groupBox_slam_update);
        localDetection_radius->setObjectName(QString::fromUtf8("localDetection_radius"));
        localDetection_radius->setValue(1);

        gridLayout_47->addWidget(localDetection_radius, 7, 0, 1, 1);

        label_space2 = new QLabel(groupBox_slam_update);
        label_space2->setObjectName(QString::fromUtf8("label_space2"));
        label_space2->setWordWrap(true);

        gridLayout_47->addWidget(label_space2, 7, 1, 1, 1);

        label_scanMatching_5 = new QLabel(groupBox_slam_update);
        label_scanMatching_5->setObjectName(QString::fromUtf8("label_scanMatching_5"));
        label_scanMatching_5->setWordWrap(true);

        gridLayout_47->addWidget(label_scanMatching_5, 6, 1, 1, 1);

        rgdb_localImmunizationRatio = new QDoubleSpinBox(groupBox_slam_update);
        rgdb_localImmunizationRatio->setObjectName(QString::fromUtf8("rgdb_localImmunizationRatio"));
        rgdb_localImmunizationRatio->setDecimals(2);
        rgdb_localImmunizationRatio->setMaximum(1);
        rgdb_localImmunizationRatio->setSingleStep(0.1);
        rgdb_localImmunizationRatio->setValue(0.1);

        gridLayout_47->addWidget(rgdb_localImmunizationRatio, 6, 0, 1, 1);

        gridLayout_47->setColumnStretch(1, 1);

        verticalLayout_25->addWidget(groupBox_slam_update);

        groupBox_graphOptimization = new QGroupBox(groupBox_slam1);
        groupBox_graphOptimization->setObjectName(QString::fromUtf8("groupBox_graphOptimization"));
        gridLayout_49 = new QGridLayout(groupBox_graphOptimization);
        gridLayout_49->setObjectName(QString::fromUtf8("gridLayout_49"));
        graphOptimization_type = new QComboBox(groupBox_graphOptimization);
        graphOptimization_type->setObjectName(QString::fromUtf8("graphOptimization_type"));

        gridLayout_49->addWidget(graphOptimization_type, 0, 0, 1, 1);

        label_31 = new QLabel(groupBox_graphOptimization);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_49->addWidget(label_31, 1, 1, 1, 1);

        graphOptimization_iterations = new QSpinBox(groupBox_graphOptimization);
        graphOptimization_iterations->setObjectName(QString::fromUtf8("graphOptimization_iterations"));
        graphOptimization_iterations->setMinimum(0);
        graphOptimization_iterations->setMaximum(10000);
        graphOptimization_iterations->setValue(100);

        gridLayout_49->addWidget(graphOptimization_iterations, 1, 0, 1, 1);

        graphOptimization_covarianceIgnored = new QCheckBox(groupBox_graphOptimization);
        graphOptimization_covarianceIgnored->setObjectName(QString::fromUtf8("graphOptimization_covarianceIgnored"));

        gridLayout_49->addWidget(graphOptimization_covarianceIgnored, 2, 0, 1, 1);

        label_141 = new QLabel(groupBox_graphOptimization);
        label_141->setObjectName(QString::fromUtf8("label_141"));
        label_141->setWordWrap(true);

        gridLayout_49->addWidget(label_141, 2, 1, 1, 1);

        graphOptimization_fromGraphEnd = new QCheckBox(groupBox_graphOptimization);
        graphOptimization_fromGraphEnd->setObjectName(QString::fromUtf8("graphOptimization_fromGraphEnd"));

        gridLayout_49->addWidget(graphOptimization_fromGraphEnd, 5, 0, 1, 1);

        label_151 = new QLabel(groupBox_graphOptimization);
        label_151->setObjectName(QString::fromUtf8("label_151"));
        label_151->setWordWrap(true);

        gridLayout_49->addWidget(label_151, 5, 1, 1, 1);

        line = new QFrame(groupBox_graphOptimization);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_49->addWidget(line, 4, 1, 1, 1);

        graphOptimization_slam2d = new QCheckBox(groupBox_graphOptimization);
        graphOptimization_slam2d->setObjectName(QString::fromUtf8("graphOptimization_slam2d"));

        gridLayout_49->addWidget(graphOptimization_slam2d, 3, 0, 1, 1);

        label_164 = new QLabel(groupBox_graphOptimization);
        label_164->setObjectName(QString::fromUtf8("label_164"));
        label_164->setWordWrap(true);

        gridLayout_49->addWidget(label_164, 3, 1, 1, 1);

        label_45 = new QLabel(groupBox_graphOptimization);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout_49->addWidget(label_45, 0, 1, 1, 1);

        label_183 = new QLabel(groupBox_graphOptimization);
        label_183->setObjectName(QString::fromUtf8("label_183"));
        label_183->setWordWrap(true);

        gridLayout_49->addWidget(label_183, 7, 1, 1, 1);

        label_211 = new QLabel(groupBox_graphOptimization);
        label_211->setObjectName(QString::fromUtf8("label_211"));
        label_211->setWordWrap(true);

        gridLayout_49->addWidget(label_211, 6, 1, 1, 1);

        gridLayout_49->setColumnStretch(1, 1);

        verticalLayout_25->addWidget(groupBox_graphOptimization);

        groupBox_localDetection_time = new QGroupBox(groupBox_slam1);
        groupBox_localDetection_time->setObjectName(QString::fromUtf8("groupBox_localDetection_time"));
        groupBox_localDetection_time->setCheckable(true);
        verticalLayout_21 = new QVBoxLayout(groupBox_localDetection_time);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        label_54 = new QLabel(groupBox_localDetection_time);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setWordWrap(true);

        verticalLayout_21->addWidget(label_54);


        verticalLayout_25->addWidget(groupBox_localDetection_time);

        groupBox_localDetection_space = new QGroupBox(groupBox_slam1);
        groupBox_localDetection_space->setObjectName(QString::fromUtf8("groupBox_localDetection_space"));
        groupBox_localDetection_space->setCheckable(true);
        verticalLayout_48 = new QVBoxLayout(groupBox_localDetection_space);
        verticalLayout_48->setObjectName(QString::fromUtf8("verticalLayout_48"));
        label_space1_2 = new QLabel(groupBox_localDetection_space);
        label_space1_2->setObjectName(QString::fromUtf8("label_space1_2"));
        label_space1_2->setWordWrap(true);

        verticalLayout_48->addWidget(label_space1_2);

        gridLayout_50 = new QGridLayout();
        gridLayout_50->setObjectName(QString::fromUtf8("gridLayout_50"));
        localDetection_pathFilteringRadius = new QDoubleSpinBox(groupBox_localDetection_space);
        localDetection_pathFilteringRadius->setObjectName(QString::fromUtf8("localDetection_pathFilteringRadius"));
        localDetection_pathFilteringRadius->setDecimals(2);
        localDetection_pathFilteringRadius->setSingleStep(0.1);
        localDetection_pathFilteringRadius->setValue(1);

        gridLayout_50->addWidget(localDetection_pathFilteringRadius, 1, 0, 1, 1);

        localDetection_maxDiffID = new QSpinBox(groupBox_localDetection_space);
        localDetection_maxDiffID->setObjectName(QString::fromUtf8("localDetection_maxDiffID"));
        localDetection_maxDiffID->setMaximum(999);
        localDetection_maxDiffID->setValue(0);

        gridLayout_50->addWidget(localDetection_maxDiffID, 0, 0, 1, 1);

        label_space3_2 = new QLabel(groupBox_localDetection_space);
        label_space3_2->setObjectName(QString::fromUtf8("label_space3_2"));
        label_space3_2->setWordWrap(true);

        gridLayout_50->addWidget(label_space3_2, 0, 1, 1, 1);

        checkBox_localSpaceLinksKeptInWM = new QCheckBox(groupBox_localDetection_space);
        checkBox_localSpaceLinksKeptInWM->setObjectName(QString::fromUtf8("checkBox_localSpaceLinksKeptInWM"));

        gridLayout_50->addWidget(checkBox_localSpaceLinksKeptInWM, 3, 0, 1, 1);

        label_scanMatching_2 = new QLabel(groupBox_localDetection_space);
        label_scanMatching_2->setObjectName(QString::fromUtf8("label_scanMatching_2"));
        label_scanMatching_2->setWordWrap(true);

        gridLayout_50->addWidget(label_scanMatching_2, 3, 1, 1, 1);

        label_space3_3 = new QLabel(groupBox_localDetection_space);
        label_space3_3->setObjectName(QString::fromUtf8("label_space3_3"));
        label_space3_3->setWordWrap(true);

        gridLayout_50->addWidget(label_space3_3, 1, 1, 1, 1);

        label_scanMatching_4 = new QLabel(groupBox_localDetection_space);
        label_scanMatching_4->setObjectName(QString::fromUtf8("label_scanMatching_4"));
        label_scanMatching_4->setWordWrap(true);

        gridLayout_50->addWidget(label_scanMatching_4, 2, 1, 1, 1);

        checkBox_localSpacePathOdomPosesUsed = new QCheckBox(groupBox_localDetection_space);
        checkBox_localSpacePathOdomPosesUsed->setObjectName(QString::fromUtf8("checkBox_localSpacePathOdomPosesUsed"));

        gridLayout_50->addWidget(checkBox_localSpacePathOdomPosesUsed, 2, 0, 1, 1);

        gridLayout_50->setColumnStretch(1, 1);

        verticalLayout_48->addLayout(gridLayout_50);


        verticalLayout_25->addWidget(groupBox_localDetection_space);

        groupBox_12 = new QGroupBox(groupBox_slam1);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        verticalLayout_61 = new QVBoxLayout(groupBox_12);
        verticalLayout_61->setObjectName(QString::fromUtf8("verticalLayout_61"));
        label_space2_3 = new QLabel(groupBox_12);
        label_space2_3->setObjectName(QString::fromUtf8("label_space2_3"));
        label_space2_3->setWordWrap(true);

        verticalLayout_61->addWidget(label_space2_3);

        gridLayout_51 = new QGridLayout();
        gridLayout_51->setObjectName(QString::fromUtf8("gridLayout_51"));
        graphPlan_goalReachedRadius = new QDoubleSpinBox(groupBox_12);
        graphPlan_goalReachedRadius->setObjectName(QString::fromUtf8("graphPlan_goalReachedRadius"));
        graphPlan_goalReachedRadius->setValue(1);

        gridLayout_51->addWidget(graphPlan_goalReachedRadius, 0, 0, 1, 1);

        label_space2_2 = new QLabel(groupBox_12);
        label_space2_2->setObjectName(QString::fromUtf8("label_space2_2"));
        label_space2_2->setWordWrap(true);

        gridLayout_51->addWidget(label_space2_2, 0, 1, 1, 1);

        graphPlan_planWithNearNodesLinked = new QCheckBox(groupBox_12);
        graphPlan_planWithNearNodesLinked->setObjectName(QString::fromUtf8("graphPlan_planWithNearNodesLinked"));

        gridLayout_51->addWidget(graphPlan_planWithNearNodesLinked, 1, 0, 1, 1);

        label_space3_4 = new QLabel(groupBox_12);
        label_space3_4->setObjectName(QString::fromUtf8("label_space3_4"));
        label_space3_4->setWordWrap(true);

        gridLayout_51->addWidget(label_space3_4, 1, 1, 1, 1);

        label_space3_5 = new QLabel(groupBox_12);
        label_space3_5->setObjectName(QString::fromUtf8("label_space3_5"));
        label_space3_5->setWordWrap(true);

        gridLayout_51->addWidget(label_space3_5, 2, 1, 1, 1);

        graphPlan_goalsSavedInUserData = new QCheckBox(groupBox_12);
        graphPlan_goalsSavedInUserData->setObjectName(QString::fromUtf8("graphPlan_goalsSavedInUserData"));

        gridLayout_51->addWidget(graphPlan_goalsSavedInUserData, 2, 0, 1, 1);

        gridLayout_51->setColumnStretch(1, 1);

        verticalLayout_61->addLayout(gridLayout_51);


        verticalLayout_25->addWidget(groupBox_12);


        verticalLayout_19->addWidget(groupBox_slam1);

        verticalSpacer_11 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_19->addItem(verticalSpacer_11);

        stackedWidget->addWidget(page_10);
        page_21 = new QWidget();
        page_21->setObjectName(QString::fromUtf8("page_21"));
        verticalLayout_50 = new QVBoxLayout(page_21);
        verticalLayout_50->setObjectName(QString::fromUtf8("verticalLayout_50"));
        groupBox_visualTransform2 = new QGroupBox(page_21);
        groupBox_visualTransform2->setObjectName(QString::fromUtf8("groupBox_visualTransform2"));
        verticalLayout_41 = new QVBoxLayout(groupBox_visualTransform2);
        verticalLayout_41->setObjectName(QString::fromUtf8("verticalLayout_41"));
        label_16 = new QLabel(groupBox_visualTransform2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setWordWrap(true);

        verticalLayout_41->addWidget(label_16);

        gridLayout_23 = new QGridLayout();
        gridLayout_23->setObjectName(QString::fromUtf8("gridLayout_23"));
        loopClosure_bowMinInliers = new QSpinBox(groupBox_visualTransform2);
        loopClosure_bowMinInliers->setObjectName(QString::fromUtf8("loopClosure_bowMinInliers"));
        loopClosure_bowMinInliers->setMinimum(1);
        loopClosure_bowMinInliers->setMaximum(1000);
        loopClosure_bowMinInliers->setValue(10);

        gridLayout_23->addWidget(loopClosure_bowMinInliers, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_visualTransform2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_23->addWidget(label_2, 0, 1, 1, 1);

        loopClosure_bowInlierDistance = new QDoubleSpinBox(groupBox_visualTransform2);
        loopClosure_bowInlierDistance->setObjectName(QString::fromUtf8("loopClosure_bowInlierDistance"));
        loopClosure_bowInlierDistance->setDecimals(3);
        loopClosure_bowInlierDistance->setMinimum(0.001);
        loopClosure_bowInlierDistance->setSingleStep(0.01);
        loopClosure_bowInlierDistance->setValue(0.02);

        gridLayout_23->addWidget(loopClosure_bowInlierDistance, 1, 0, 1, 1);

        label_14 = new QLabel(groupBox_visualTransform2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_23->addWidget(label_14, 1, 1, 1, 1);

        loopClosure_bowIterations = new QSpinBox(groupBox_visualTransform2);
        loopClosure_bowIterations->setObjectName(QString::fromUtf8("loopClosure_bowIterations"));
        loopClosure_bowIterations->setMinimum(1);
        loopClosure_bowIterations->setMaximum(10000);
        loopClosure_bowIterations->setSingleStep(1);
        loopClosure_bowIterations->setValue(100);

        gridLayout_23->addWidget(loopClosure_bowIterations, 2, 0, 1, 1);

        label_15 = new QLabel(groupBox_visualTransform2);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_23->addWidget(label_15, 2, 1, 1, 1);

        loopClosure_bowMaxDepth = new QDoubleSpinBox(groupBox_visualTransform2);
        loopClosure_bowMaxDepth->setObjectName(QString::fromUtf8("loopClosure_bowMaxDepth"));
        loopClosure_bowMaxDepth->setMinimum(0);
        loopClosure_bowMaxDepth->setValue(5);

        gridLayout_23->addWidget(loopClosure_bowMaxDepth, 3, 0, 1, 1);

        label_146 = new QLabel(groupBox_visualTransform2);
        label_146->setObjectName(QString::fromUtf8("label_146"));
        label_146->setWordWrap(true);

        gridLayout_23->addWidget(label_146, 3, 1, 1, 1);

        loopClosure_bowForce2D = new QCheckBox(groupBox_visualTransform2);
        loopClosure_bowForce2D->setObjectName(QString::fromUtf8("loopClosure_bowForce2D"));

        gridLayout_23->addWidget(loopClosure_bowForce2D, 6, 0, 1, 1);

        label_184 = new QLabel(groupBox_visualTransform2);
        label_184->setObjectName(QString::fromUtf8("label_184"));
        label_184->setWordWrap(true);

        gridLayout_23->addWidget(label_184, 6, 1, 1, 1);

        globalDetection_icpType = new QComboBox(groupBox_visualTransform2);
        globalDetection_icpType->setObjectName(QString::fromUtf8("globalDetection_icpType"));
        globalDetection_icpType->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_23->addWidget(globalDetection_icpType, 7, 0, 1, 1);

        label_57 = new QLabel(groupBox_visualTransform2);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setWordWrap(true);

        gridLayout_23->addWidget(label_57, 7, 1, 1, 1);

        label_658 = new QLabel(groupBox_visualTransform2);
        label_658->setObjectName(QString::fromUtf8("label_658"));
        label_658->setWordWrap(true);

        gridLayout_23->addWidget(label_658, 4, 1, 1, 1);

        loopClosure_bowEpipolarGeometry = new QCheckBox(groupBox_visualTransform2);
        loopClosure_bowEpipolarGeometry->setObjectName(QString::fromUtf8("loopClosure_bowEpipolarGeometry"));

        gridLayout_23->addWidget(loopClosure_bowEpipolarGeometry, 4, 0, 1, 1);

        label_664 = new QLabel(groupBox_visualTransform2);
        label_664->setObjectName(QString::fromUtf8("label_664"));
        label_664->setWordWrap(true);

        gridLayout_23->addWidget(label_664, 5, 1, 1, 1);

        loopClosure_bowEpipolarGeometryVar = new QDoubleSpinBox(groupBox_visualTransform2);
        loopClosure_bowEpipolarGeometryVar->setObjectName(QString::fromUtf8("loopClosure_bowEpipolarGeometryVar"));
        loopClosure_bowEpipolarGeometryVar->setDecimals(3);
        loopClosure_bowEpipolarGeometryVar->setMinimum(0);
        loopClosure_bowEpipolarGeometryVar->setSingleStep(0.001);
        loopClosure_bowEpipolarGeometryVar->setValue(0.02);

        gridLayout_23->addWidget(loopClosure_bowEpipolarGeometryVar, 5, 0, 1, 1);

        gridLayout_23->setColumnStretch(1, 1);

        verticalLayout_41->addLayout(gridLayout_23);

        groupBox_reextract = new QGroupBox(groupBox_visualTransform2);
        groupBox_reextract->setObjectName(QString::fromUtf8("groupBox_reextract"));
        groupBox_reextract->setCheckable(true);
        groupBox_reextract->setChecked(true);
        verticalLayout_46 = new QVBoxLayout(groupBox_reextract);
        verticalLayout_46->setObjectName(QString::fromUtf8("verticalLayout_46"));
        label_182 = new QLabel(groupBox_reextract);
        label_182->setObjectName(QString::fromUtf8("label_182"));
        label_182->setWordWrap(true);

        verticalLayout_46->addWidget(label_182);

        label_171 = new QLabel(groupBox_reextract);
        label_171->setObjectName(QString::fromUtf8("label_171"));
        label_171->setWordWrap(true);

        verticalLayout_46->addWidget(label_171);

        label_174 = new QLabel(groupBox_reextract);
        label_174->setObjectName(QString::fromUtf8("label_174"));
        label_174->setWordWrap(true);

        verticalLayout_46->addWidget(label_174);

        gridLayout_25 = new QGridLayout();
        gridLayout_25->setObjectName(QString::fromUtf8("gridLayout_25"));
        reextract_nn = new QComboBox(groupBox_reextract);
        reextract_nn->setObjectName(QString::fromUtf8("reextract_nn"));
        reextract_nn->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_25->addWidget(reextract_nn, 3, 0, 1, 1);

        reextract_nndrRatio = new QDoubleSpinBox(groupBox_reextract);
        reextract_nndrRatio->setObjectName(QString::fromUtf8("reextract_nndrRatio"));
        reextract_nndrRatio->setDecimals(1);
        reextract_nndrRatio->setMinimum(0.1);
        reextract_nndrRatio->setMaximum(1);
        reextract_nndrRatio->setSingleStep(0.1);
        reextract_nndrRatio->setValue(0.7);

        gridLayout_25->addWidget(reextract_nndrRatio, 4, 0, 1, 1);

        reextract_maxFeatures = new QSpinBox(groupBox_reextract);
        reextract_maxFeatures->setObjectName(QString::fromUtf8("reextract_maxFeatures"));
        reextract_maxFeatures->setMaximum(999999);

        gridLayout_25->addWidget(reextract_maxFeatures, 2, 0, 1, 1);

        label_180 = new QLabel(groupBox_reextract);
        label_180->setObjectName(QString::fromUtf8("label_180"));
        label_180->setWordWrap(true);

        gridLayout_25->addWidget(label_180, 2, 1, 1, 1);

        label_179 = new QLabel(groupBox_reextract);
        label_179->setObjectName(QString::fromUtf8("label_179"));
        label_179->setWordWrap(true);

        gridLayout_25->addWidget(label_179, 3, 1, 1, 1);

        label_181 = new QLabel(groupBox_reextract);
        label_181->setObjectName(QString::fromUtf8("label_181"));
        label_181->setWordWrap(true);

        gridLayout_25->addWidget(label_181, 4, 1, 1, 1);

        reextract_type = new QComboBox(groupBox_reextract);
        reextract_type->setObjectName(QString::fromUtf8("reextract_type"));
        reextract_type->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_25->addWidget(reextract_type, 1, 0, 1, 1);

        label_178 = new QLabel(groupBox_reextract);
        label_178->setObjectName(QString::fromUtf8("label_178"));

        gridLayout_25->addWidget(label_178, 1, 1, 1, 1);

        gridLayout_25->setColumnStretch(1, 1);

        verticalLayout_46->addLayout(gridLayout_25);


        verticalLayout_41->addWidget(groupBox_reextract);


        verticalLayout_50->addWidget(groupBox_visualTransform2);

        verticalSpacer_20 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_50->addItem(verticalSpacer_20);

        stackedWidget->addWidget(page_21);
        page_27 = new QWidget();
        page_27->setObjectName(QString::fromUtf8("page_27"));
        verticalLayout_52 = new QVBoxLayout(page_27);
        verticalLayout_52->setObjectName(QString::fromUtf8("verticalLayout_52"));
        groupBox_icp2 = new QGroupBox(page_27);
        groupBox_icp2->setObjectName(QString::fromUtf8("groupBox_icp2"));
        verticalLayout_20 = new QVBoxLayout(groupBox_icp2);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        label_7 = new QLabel(groupBox_icp2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_20->addWidget(label_7);

        gridLayout_48 = new QGridLayout();
        gridLayout_48->setObjectName(QString::fromUtf8("gridLayout_48"));
        globalDetection_icpMaxTranslation = new QDoubleSpinBox(groupBox_icp2);
        globalDetection_icpMaxTranslation->setObjectName(QString::fromUtf8("globalDetection_icpMaxTranslation"));
        globalDetection_icpMaxTranslation->setDecimals(2);
        globalDetection_icpMaxTranslation->setMaximum(1);
        globalDetection_icpMaxTranslation->setSingleStep(0.01);
        globalDetection_icpMaxTranslation->setValue(0.2);

        gridLayout_48->addWidget(globalDetection_icpMaxTranslation, 0, 0, 1, 1);

        label_51 = new QLabel(groupBox_icp2);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setWordWrap(true);

        gridLayout_48->addWidget(label_51, 0, 1, 1, 1);

        label_55 = new QLabel(groupBox_icp2);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setWordWrap(true);

        gridLayout_48->addWidget(label_55, 1, 1, 1, 1);

        globalDetection_icpMaxRotation = new QDoubleSpinBox(groupBox_icp2);
        globalDetection_icpMaxRotation->setObjectName(QString::fromUtf8("globalDetection_icpMaxRotation"));
        globalDetection_icpMaxRotation->setDecimals(2);
        globalDetection_icpMaxRotation->setMaximum(3.14);
        globalDetection_icpMaxRotation->setSingleStep(0.01);
        globalDetection_icpMaxRotation->setValue(0.78);

        gridLayout_48->addWidget(globalDetection_icpMaxRotation, 1, 0, 1, 1);

        gridLayout_48->setColumnStretch(1, 1);

        verticalLayout_20->addLayout(gridLayout_48);

        groupBox_loopClosure_icp3 = new QGroupBox(groupBox_icp2);
        groupBox_loopClosure_icp3->setObjectName(QString::fromUtf8("groupBox_loopClosure_icp3"));
        gridLayout_53 = new QGridLayout(groupBox_loopClosure_icp3);
        gridLayout_53->setObjectName(QString::fromUtf8("gridLayout_53"));
        loopClosure_icpIterations = new QSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpIterations->setObjectName(QString::fromUtf8("loopClosure_icpIterations"));
        loopClosure_icpIterations->setMinimum(1);
        loopClosure_icpIterations->setMaximum(1000);
        loopClosure_icpIterations->setValue(10);

        gridLayout_53->addWidget(loopClosure_icpIterations, 5, 0, 1, 1);

        label_124 = new QLabel(groupBox_loopClosure_icp3);
        label_124->setObjectName(QString::fromUtf8("label_124"));
        label_124->setWordWrap(true);

        gridLayout_53->addWidget(label_124, 5, 1, 1, 1);

        loopClosure_icpSamples = new QSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpSamples->setObjectName(QString::fromUtf8("loopClosure_icpSamples"));
        loopClosure_icpSamples->setMinimum(0);
        loopClosure_icpSamples->setMaximum(999999);
        loopClosure_icpSamples->setSingleStep(100);
        loopClosure_icpSamples->setValue(5000);

        gridLayout_53->addWidget(loopClosure_icpSamples, 3, 0, 1, 1);

        loopClosure_icpDecimation = new QSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpDecimation->setObjectName(QString::fromUtf8("loopClosure_icpDecimation"));
        loopClosure_icpDecimation->setMinimum(1);
        loopClosure_icpDecimation->setMaximum(32);

        gridLayout_53->addWidget(loopClosure_icpDecimation, 0, 0, 1, 1);

        loopClosure_icpMaxCorrespondenceDistance = new QDoubleSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpMaxCorrespondenceDistance->setObjectName(QString::fromUtf8("loopClosure_icpMaxCorrespondenceDistance"));
        loopClosure_icpMaxCorrespondenceDistance->setDecimals(3);
        loopClosure_icpMaxCorrespondenceDistance->setMinimum(0.01);
        loopClosure_icpMaxCorrespondenceDistance->setSingleStep(0.01);
        loopClosure_icpMaxCorrespondenceDistance->setValue(0.1);

        gridLayout_53->addWidget(loopClosure_icpMaxCorrespondenceDistance, 4, 0, 1, 1);

        label_109 = new QLabel(groupBox_loopClosure_icp3);
        label_109->setObjectName(QString::fromUtf8("label_109"));
        label_109->setWordWrap(true);

        gridLayout_53->addWidget(label_109, 0, 1, 1, 1);

        loopClosure_icpMaxDepth = new QDoubleSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpMaxDepth->setObjectName(QString::fromUtf8("loopClosure_icpMaxDepth"));
        loopClosure_icpMaxDepth->setMinimum(0);
        loopClosure_icpMaxDepth->setValue(5);

        gridLayout_53->addWidget(loopClosure_icpMaxDepth, 1, 0, 1, 1);

        label_115 = new QLabel(groupBox_loopClosure_icp3);
        label_115->setObjectName(QString::fromUtf8("label_115"));
        label_115->setWordWrap(true);

        gridLayout_53->addWidget(label_115, 1, 1, 1, 1);

        loopClosure_icpVoxelSize = new QDoubleSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpVoxelSize->setObjectName(QString::fromUtf8("loopClosure_icpVoxelSize"));
        loopClosure_icpVoxelSize->setDecimals(3);
        loopClosure_icpVoxelSize->setMinimum(0);
        loopClosure_icpVoxelSize->setSingleStep(0.01);
        loopClosure_icpVoxelSize->setValue(0.005);

        gridLayout_53->addWidget(loopClosure_icpVoxelSize, 2, 0, 1, 1);

        label_125 = new QLabel(groupBox_loopClosure_icp3);
        label_125->setObjectName(QString::fromUtf8("label_125"));
        label_125->setWordWrap(true);

        gridLayout_53->addWidget(label_125, 2, 1, 1, 1);

        label_118 = new QLabel(groupBox_loopClosure_icp3);
        label_118->setObjectName(QString::fromUtf8("label_118"));
        label_118->setWordWrap(true);

        gridLayout_53->addWidget(label_118, 3, 1, 1, 1);

        label_122 = new QLabel(groupBox_loopClosure_icp3);
        label_122->setObjectName(QString::fromUtf8("label_122"));
        label_122->setWordWrap(true);

        gridLayout_53->addWidget(label_122, 4, 1, 1, 1);

        loopClosure_icpRatio = new QDoubleSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpRatio->setObjectName(QString::fromUtf8("loopClosure_icpRatio"));
        loopClosure_icpRatio->setMinimum(0);
        loopClosure_icpRatio->setMaximum(1);
        loopClosure_icpRatio->setSingleStep(0.01);
        loopClosure_icpRatio->setValue(0.7);

        gridLayout_53->addWidget(loopClosure_icpRatio, 6, 0, 1, 1);

        label_133 = new QLabel(groupBox_loopClosure_icp3);
        label_133->setObjectName(QString::fromUtf8("label_133"));
        label_133->setWordWrap(true);

        gridLayout_53->addWidget(label_133, 6, 1, 1, 1);

        loopClosure_icpPointToPlane = new QCheckBox(groupBox_loopClosure_icp3);
        loopClosure_icpPointToPlane->setObjectName(QString::fromUtf8("loopClosure_icpPointToPlane"));

        gridLayout_53->addWidget(loopClosure_icpPointToPlane, 7, 0, 1, 1);

        label_144 = new QLabel(groupBox_loopClosure_icp3);
        label_144->setObjectName(QString::fromUtf8("label_144"));
        label_144->setWordWrap(true);

        gridLayout_53->addWidget(label_144, 7, 1, 1, 1);

        loopClosure_icpPointToPlaneNormals = new QSpinBox(groupBox_loopClosure_icp3);
        loopClosure_icpPointToPlaneNormals->setObjectName(QString::fromUtf8("loopClosure_icpPointToPlaneNormals"));
        loopClosure_icpPointToPlaneNormals->setMinimum(1);
        loopClosure_icpPointToPlaneNormals->setMaximum(1000);
        loopClosure_icpPointToPlaneNormals->setValue(20);

        gridLayout_53->addWidget(loopClosure_icpPointToPlaneNormals, 8, 0, 1, 1);

        label_212 = new QLabel(groupBox_loopClosure_icp3);
        label_212->setObjectName(QString::fromUtf8("label_212"));
        label_212->setWordWrap(true);

        gridLayout_53->addWidget(label_212, 8, 1, 1, 1);

        gridLayout_53->setColumnStretch(1, 1);

        verticalLayout_20->addWidget(groupBox_loopClosure_icp3);

        groupBox_loopClosure_icp2 = new QGroupBox(groupBox_icp2);
        groupBox_loopClosure_icp2->setObjectName(QString::fromUtf8("groupBox_loopClosure_icp2"));
        gridLayout_52 = new QGridLayout(groupBox_loopClosure_icp2);
        gridLayout_52->setObjectName(QString::fromUtf8("gridLayout_52"));
        loopClosure_icp2MaxCorrespondenceDistance = new QDoubleSpinBox(groupBox_loopClosure_icp2);
        loopClosure_icp2MaxCorrespondenceDistance->setObjectName(QString::fromUtf8("loopClosure_icp2MaxCorrespondenceDistance"));
        loopClosure_icp2MaxCorrespondenceDistance->setDecimals(3);
        loopClosure_icp2MaxCorrespondenceDistance->setMinimum(0.01);
        loopClosure_icp2MaxCorrespondenceDistance->setSingleStep(0.01);
        loopClosure_icp2MaxCorrespondenceDistance->setValue(0.1);

        gridLayout_52->addWidget(loopClosure_icp2MaxCorrespondenceDistance, 0, 0, 1, 1);

        label_138 = new QLabel(groupBox_loopClosure_icp2);
        label_138->setObjectName(QString::fromUtf8("label_138"));
        label_138->setWordWrap(true);

        gridLayout_52->addWidget(label_138, 0, 1, 1, 1);

        loopClosure_icp2Iterations = new QSpinBox(groupBox_loopClosure_icp2);
        loopClosure_icp2Iterations->setObjectName(QString::fromUtf8("loopClosure_icp2Iterations"));
        loopClosure_icp2Iterations->setMinimum(1);
        loopClosure_icp2Iterations->setMaximum(1000);
        loopClosure_icp2Iterations->setValue(10);

        gridLayout_52->addWidget(loopClosure_icp2Iterations, 1, 0, 1, 1);

        label_139 = new QLabel(groupBox_loopClosure_icp2);
        label_139->setObjectName(QString::fromUtf8("label_139"));
        label_139->setWordWrap(true);

        gridLayout_52->addWidget(label_139, 1, 1, 1, 1);

        loopClosure_icp2Ratio = new QDoubleSpinBox(groupBox_loopClosure_icp2);
        loopClosure_icp2Ratio->setObjectName(QString::fromUtf8("loopClosure_icp2Ratio"));
        loopClosure_icp2Ratio->setMinimum(0);
        loopClosure_icp2Ratio->setMaximum(1);
        loopClosure_icp2Ratio->setSingleStep(0.01);
        loopClosure_icp2Ratio->setValue(0.7);

        gridLayout_52->addWidget(loopClosure_icp2Ratio, 2, 0, 1, 1);

        label_148 = new QLabel(groupBox_loopClosure_icp2);
        label_148->setObjectName(QString::fromUtf8("label_148"));
        label_148->setWordWrap(true);

        gridLayout_52->addWidget(label_148, 2, 1, 1, 1);

        loopClosure_icp2Voxel = new QDoubleSpinBox(groupBox_loopClosure_icp2);
        loopClosure_icp2Voxel->setObjectName(QString::fromUtf8("loopClosure_icp2Voxel"));
        loopClosure_icp2Voxel->setDecimals(3);
        loopClosure_icp2Voxel->setMinimum(0);
        loopClosure_icp2Voxel->setMaximum(0.1);
        loopClosure_icp2Voxel->setSingleStep(0.001);
        loopClosure_icp2Voxel->setValue(0);

        gridLayout_52->addWidget(loopClosure_icp2Voxel, 3, 0, 1, 1);

        label_150 = new QLabel(groupBox_loopClosure_icp2);
        label_150->setObjectName(QString::fromUtf8("label_150"));
        label_150->setWordWrap(true);

        gridLayout_52->addWidget(label_150, 3, 1, 1, 1);

        gridLayout_52->setColumnStretch(1, 1);

        verticalLayout_20->addWidget(groupBox_loopClosure_icp2);


        verticalLayout_52->addWidget(groupBox_icp2);

        verticalSpacer_21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_52->addItem(verticalSpacer_21);

        stackedWidget->addWidget(page_27);
        page_29 = new QWidget();
        page_29->setObjectName(QString::fromUtf8("page_29"));
        verticalLayout_57 = new QVBoxLayout(page_29);
        verticalLayout_57->setObjectName(QString::fromUtf8("verticalLayout_57"));
        groupBox_stereo2 = new QGroupBox(page_29);
        groupBox_stereo2->setObjectName(QString::fromUtf8("groupBox_stereo2"));
        verticalLayout_56 = new QVBoxLayout(groupBox_stereo2);
        verticalLayout_56->setObjectName(QString::fromUtf8("verticalLayout_56"));
        label_187 = new QLabel(groupBox_stereo2);
        label_187->setObjectName(QString::fromUtf8("label_187"));
        label_187->setWordWrap(true);

        verticalLayout_56->addWidget(label_187);

        groupBox_10 = new QGroupBox(groupBox_stereo2);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        gridLayout_33 = new QGridLayout(groupBox_10);
        gridLayout_33->setObjectName(QString::fromUtf8("gridLayout_33"));
        stereo_flow_winSize = new QSpinBox(groupBox_10);
        stereo_flow_winSize->setObjectName(QString::fromUtf8("stereo_flow_winSize"));
        stereo_flow_winSize->setMinimum(1);
        stereo_flow_winSize->setMaximum(999999);
        stereo_flow_winSize->setSingleStep(1);
        stereo_flow_winSize->setValue(21);

        gridLayout_33->addWidget(stereo_flow_winSize, 0, 0, 1, 1);

        label_205 = new QLabel(groupBox_10);
        label_205->setObjectName(QString::fromUtf8("label_205"));
        label_205->setWordWrap(true);

        gridLayout_33->addWidget(label_205, 0, 1, 1, 1);

        stereo_flow_iterations = new QSpinBox(groupBox_10);
        stereo_flow_iterations->setObjectName(QString::fromUtf8("stereo_flow_iterations"));
        stereo_flow_iterations->setMinimum(1);
        stereo_flow_iterations->setMaximum(999999);
        stereo_flow_iterations->setSingleStep(1);
        stereo_flow_iterations->setValue(30);

        gridLayout_33->addWidget(stereo_flow_iterations, 1, 0, 1, 1);

        label_206 = new QLabel(groupBox_10);
        label_206->setObjectName(QString::fromUtf8("label_206"));
        label_206->setWordWrap(true);

        gridLayout_33->addWidget(label_206, 1, 1, 1, 1);

        stereo_flow_eps = new QDoubleSpinBox(groupBox_10);
        stereo_flow_eps->setObjectName(QString::fromUtf8("stereo_flow_eps"));
        stereo_flow_eps->setDecimals(3);
        stereo_flow_eps->setMinimum(0.001);
        stereo_flow_eps->setMaximum(0.1);
        stereo_flow_eps->setSingleStep(0.01);
        stereo_flow_eps->setValue(0.01);

        gridLayout_33->addWidget(stereo_flow_eps, 2, 0, 1, 1);

        label_207 = new QLabel(groupBox_10);
        label_207->setObjectName(QString::fromUtf8("label_207"));
        label_207->setWordWrap(true);

        gridLayout_33->addWidget(label_207, 2, 1, 1, 1);

        label_208 = new QLabel(groupBox_10);
        label_208->setObjectName(QString::fromUtf8("label_208"));
        label_208->setWordWrap(true);

        gridLayout_33->addWidget(label_208, 3, 1, 1, 1);

        stereo_flow_maxLevel = new QSpinBox(groupBox_10);
        stereo_flow_maxLevel->setObjectName(QString::fromUtf8("stereo_flow_maxLevel"));
        stereo_flow_maxLevel->setMinimum(0);
        stereo_flow_maxLevel->setMaximum(999999);
        stereo_flow_maxLevel->setSingleStep(1);
        stereo_flow_maxLevel->setValue(3);

        gridLayout_33->addWidget(stereo_flow_maxLevel, 3, 0, 1, 1);

        gridLayout_33->setColumnStretch(1, 1);

        verticalLayout_56->addWidget(groupBox_10);

        gridLayout_35 = new QGridLayout();
        gridLayout_35->setObjectName(QString::fromUtf8("gridLayout_35"));
        stereo_maxSlope = new QDoubleSpinBox(groupBox_stereo2);
        stereo_maxSlope->setObjectName(QString::fromUtf8("stereo_maxSlope"));
        stereo_maxSlope->setDecimals(3);
        stereo_maxSlope->setMinimum(0.001);
        stereo_maxSlope->setMaximum(1);
        stereo_maxSlope->setSingleStep(0.01);
        stereo_maxSlope->setValue(0.1);

        gridLayout_35->addWidget(stereo_maxSlope, 0, 0, 1, 1);

        label_215 = new QLabel(groupBox_stereo2);
        label_215->setObjectName(QString::fromUtf8("label_215"));
        label_215->setWordWrap(true);

        gridLayout_35->addWidget(label_215, 0, 1, 1, 1);

        gridLayout_35->setColumnStretch(1, 1);

        verticalLayout_56->addLayout(gridLayout_35);


        verticalLayout_57->addWidget(groupBox_stereo2);

        verticalSpacer_29 = new QSpacerItem(20, 464, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_57->addItem(verticalSpacer_29);

        stackedWidget->addWidget(page_29);
        page_12 = new QWidget();
        page_12->setObjectName(QString::fromUtf8("page_12"));
        verticalLayout_32 = new QVBoxLayout(page_12);
        verticalLayout_32->setObjectName(QString::fromUtf8("verticalLayout_32"));
        groupBox_odometry1 = new QGroupBox(page_12);
        groupBox_odometry1->setObjectName(QString::fromUtf8("groupBox_odometry1"));
        verticalLayout_34 = new QVBoxLayout(groupBox_odometry1);
        verticalLayout_34->setObjectName(QString::fromUtf8("verticalLayout_34"));
        label_114 = new QLabel(groupBox_odometry1);
        label_114->setObjectName(QString::fromUtf8("label_114"));
        label_114->setWordWrap(true);

        verticalLayout_34->addWidget(label_114);

        gridLayout_27 = new QGridLayout();
        gridLayout_27->setObjectName(QString::fromUtf8("gridLayout_27"));
        label_160 = new QLabel(groupBox_odometry1);
        label_160->setObjectName(QString::fromUtf8("label_160"));
        label_160->setWordWrap(true);

        gridLayout_27->addWidget(label_160, 3, 1, 1, 1);

        label_47 = new QLabel(groupBox_odometry1);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setWordWrap(true);

        gridLayout_27->addWidget(label_47, 5, 1, 1, 1);

        odom_strategy = new QComboBox(groupBox_odometry1);
        odom_strategy->setObjectName(QString::fromUtf8("odom_strategy"));
        odom_strategy->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_27->addWidget(odom_strategy, 0, 0, 1, 1);

        odom_countdown = new QSpinBox(groupBox_odometry1);
        odom_countdown->setObjectName(QString::fromUtf8("odom_countdown"));
        odom_countdown->setMaximum(999999);

        gridLayout_27->addWidget(odom_countdown, 5, 0, 1, 1);

        odom_fillInfoData = new QCheckBox(groupBox_odometry1);
        odom_fillInfoData->setObjectName(QString::fromUtf8("odom_fillInfoData"));

        gridLayout_27->addWidget(odom_fillInfoData, 7, 0, 1, 1);

        odom_type = new QComboBox(groupBox_odometry1);
        odom_type->setObjectName(QString::fromUtf8("odom_type"));
        odom_type->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_27->addWidget(odom_type, 4, 0, 1, 1);

        label_44 = new QLabel(groupBox_odometry1);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setWordWrap(true);

        gridLayout_27->addWidget(label_44, 4, 1, 1, 1);

        label_103 = new QLabel(groupBox_odometry1);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setWordWrap(false);

        gridLayout_27->addWidget(label_103, 0, 1, 1, 1);

        odom_force2D = new QCheckBox(groupBox_odometry1);
        odom_force2D->setObjectName(QString::fromUtf8("odom_force2D"));

        gridLayout_27->addWidget(odom_force2D, 6, 0, 1, 1);

        label_196 = new QLabel(groupBox_odometry1);
        label_196->setObjectName(QString::fromUtf8("label_196"));
        label_196->setWordWrap(true);

        gridLayout_27->addWidget(label_196, 6, 1, 1, 1);

        label_221 = new QLabel(groupBox_odometry1);
        label_221->setObjectName(QString::fromUtf8("label_221"));
        label_221->setWordWrap(true);

        gridLayout_27->addWidget(label_221, 7, 1, 1, 1);

        pushButton_testOdometry = new QPushButton(groupBox_odometry1);
        pushButton_testOdometry->setObjectName(QString::fromUtf8("pushButton_testOdometry"));

        gridLayout_27->addWidget(pushButton_testOdometry, 8, 0, 1, 1);

        label_161 = new QLabel(groupBox_odometry1);
        label_161->setObjectName(QString::fromUtf8("label_161"));
        label_161->setWordWrap(true);

        gridLayout_27->addWidget(label_161, 2, 1, 1, 1);

        label_162 = new QLabel(groupBox_odometry1);
        label_162->setObjectName(QString::fromUtf8("label_162"));
        label_162->setWordWrap(true);

        gridLayout_27->addWidget(label_162, 1, 1, 1, 1);

        gridLayout_27->setColumnStretch(1, 1);

        verticalLayout_34->addLayout(gridLayout_27);

        groupBox_11 = new QGroupBox(groupBox_odometry1);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        gridLayout_37 = new QGridLayout(groupBox_11);
        gridLayout_37->setObjectName(QString::fromUtf8("gridLayout_37"));
        odom_minInliers = new QSpinBox(groupBox_11);
        odom_minInliers->setObjectName(QString::fromUtf8("odom_minInliers"));
        odom_minInliers->setMinimum(8);
        odom_minInliers->setMaximum(1000);
        odom_minInliers->setValue(10);

        gridLayout_37->addWidget(odom_minInliers, 0, 0, 1, 1);

        label_145 = new QLabel(groupBox_11);
        label_145->setObjectName(QString::fromUtf8("label_145"));
        label_145->setWordWrap(true);

        gridLayout_37->addWidget(label_145, 0, 1, 1, 1);

        odom_inlierDistance = new QDoubleSpinBox(groupBox_11);
        odom_inlierDistance->setObjectName(QString::fromUtf8("odom_inlierDistance"));
        odom_inlierDistance->setDecimals(3);
        odom_inlierDistance->setMinimum(0.001);
        odom_inlierDistance->setSingleStep(0.01);
        odom_inlierDistance->setValue(0.005);

        gridLayout_37->addWidget(odom_inlierDistance, 1, 0, 1, 1);

        label_149 = new QLabel(groupBox_11);
        label_149->setObjectName(QString::fromUtf8("label_149"));
        label_149->setWordWrap(true);

        gridLayout_37->addWidget(label_149, 1, 1, 1, 1);

        odom_iterations = new QSpinBox(groupBox_11);
        odom_iterations->setObjectName(QString::fromUtf8("odom_iterations"));
        odom_iterations->setMinimum(1);
        odom_iterations->setMaximum(10000);
        odom_iterations->setSingleStep(1);
        odom_iterations->setValue(100);

        gridLayout_37->addWidget(odom_iterations, 2, 0, 1, 1);

        label_36 = new QLabel(groupBox_11);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setWordWrap(true);

        gridLayout_37->addWidget(label_36, 2, 1, 1, 1);

        odom_refine_iterations = new QSpinBox(groupBox_11);
        odom_refine_iterations->setObjectName(QString::fromUtf8("odom_refine_iterations"));
        odom_refine_iterations->setMinimum(0);
        odom_refine_iterations->setMaximum(10000);
        odom_refine_iterations->setSingleStep(1);
        odom_refine_iterations->setValue(10);

        gridLayout_37->addWidget(odom_refine_iterations, 3, 0, 1, 1);

        label_111 = new QLabel(groupBox_11);
        label_111->setObjectName(QString::fromUtf8("label_111"));
        label_111->setWordWrap(true);

        gridLayout_37->addWidget(label_111, 3, 1, 1, 1);

        label_225 = new QLabel(groupBox_11);
        label_225->setObjectName(QString::fromUtf8("label_225"));
        label_225->setWordWrap(true);

        gridLayout_37->addWidget(label_225, 5, 1, 1, 1);

        odom_pnpReprojError = new QDoubleSpinBox(groupBox_11);
        odom_pnpReprojError->setObjectName(QString::fromUtf8("odom_pnpReprojError"));
        odom_pnpReprojError->setDecimals(1);
        odom_pnpReprojError->setMinimum(0.1);
        odom_pnpReprojError->setSingleStep(1);
        odom_pnpReprojError->setValue(8);

        gridLayout_37->addWidget(odom_pnpReprojError, 5, 0, 1, 1);

        odom_pnpEstimation = new QCheckBox(groupBox_11);
        odom_pnpEstimation->setObjectName(QString::fromUtf8("odom_pnpEstimation"));

        gridLayout_37->addWidget(odom_pnpEstimation, 4, 0, 1, 1);

        label_226 = new QLabel(groupBox_11);
        label_226->setObjectName(QString::fromUtf8("label_226"));
        label_226->setWordWrap(true);

        gridLayout_37->addWidget(label_226, 4, 1, 1, 1);

        label_227 = new QLabel(groupBox_11);
        label_227->setObjectName(QString::fromUtf8("label_227"));
        label_227->setWordWrap(true);

        gridLayout_37->addWidget(label_227, 6, 1, 1, 1);

        odom_pnpFlags = new QComboBox(groupBox_11);
        odom_pnpFlags->setObjectName(QString::fromUtf8("odom_pnpFlags"));

        gridLayout_37->addWidget(odom_pnpFlags, 6, 0, 1, 1);

        gridLayout_37->setColumnStretch(1, 1);

        verticalLayout_34->addWidget(groupBox_11);

        groupBox_8 = new QGroupBox(groupBox_odometry1);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_36 = new QGridLayout(groupBox_8);
        gridLayout_36->setObjectName(QString::fromUtf8("gridLayout_36"));
        odom_maxFeatures = new QSpinBox(groupBox_8);
        odom_maxFeatures->setObjectName(QString::fromUtf8("odom_maxFeatures"));
        odom_maxFeatures->setMaximum(999999);

        gridLayout_36->addWidget(odom_maxFeatures, 0, 0, 1, 1);

        label_123 = new QLabel(groupBox_8);
        label_123->setObjectName(QString::fromUtf8("label_123"));
        label_123->setWordWrap(true);

        gridLayout_36->addWidget(label_123, 2, 1, 1, 1);

        lineEdit_odom_roi = new QLineEdit(groupBox_8);
        lineEdit_odom_roi->setObjectName(QString::fromUtf8("lineEdit_odom_roi"));
        lineEdit_odom_roi->setReadOnly(false);

        gridLayout_36->addWidget(lineEdit_odom_roi, 2, 0, 1, 1);

        label_136 = new QLabel(groupBox_8);
        label_136->setObjectName(QString::fromUtf8("label_136"));
        label_136->setWordWrap(true);

        gridLayout_36->addWidget(label_136, 0, 1, 1, 1);

        label_104 = new QLabel(groupBox_8);
        label_104->setObjectName(QString::fromUtf8("label_104"));
        label_104->setWordWrap(true);

        gridLayout_36->addWidget(label_104, 1, 1, 1, 1);

        odom_maxDepth = new QDoubleSpinBox(groupBox_8);
        odom_maxDepth->setObjectName(QString::fromUtf8("odom_maxDepth"));
        odom_maxDepth->setDecimals(2);
        odom_maxDepth->setMinimum(0);
        odom_maxDepth->setSingleStep(1);
        odom_maxDepth->setValue(5);

        gridLayout_36->addWidget(odom_maxDepth, 1, 0, 1, 1);

        gridLayout_36->setColumnStretch(1, 1);

        verticalLayout_34->addWidget(groupBox_8);

        groupBox_7 = new QGroupBox(groupBox_odometry1);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        verticalLayout_60 = new QVBoxLayout(groupBox_7);
        verticalLayout_60->setObjectName(QString::fromUtf8("verticalLayout_60"));
        label_216 = new QLabel(groupBox_7);
        label_216->setObjectName(QString::fromUtf8("label_216"));
        label_216->setWordWrap(true);

        verticalLayout_60->addWidget(label_216);

        gridLayout_34 = new QGridLayout();
        gridLayout_34->setObjectName(QString::fromUtf8("gridLayout_34"));
        odom_subpix_winSize = new QSpinBox(groupBox_7);
        odom_subpix_winSize->setObjectName(QString::fromUtf8("odom_subpix_winSize"));
        odom_subpix_winSize->setMinimum(0);
        odom_subpix_winSize->setMaximum(999999);
        odom_subpix_winSize->setSingleStep(1);
        odom_subpix_winSize->setValue(5);

        gridLayout_34->addWidget(odom_subpix_winSize, 0, 0, 1, 1);

        label_197 = new QLabel(groupBox_7);
        label_197->setObjectName(QString::fromUtf8("label_197"));
        label_197->setWordWrap(true);

        gridLayout_34->addWidget(label_197, 0, 1, 1, 1);

        odom_subpix_iterations = new QSpinBox(groupBox_7);
        odom_subpix_iterations->setObjectName(QString::fromUtf8("odom_subpix_iterations"));
        odom_subpix_iterations->setMinimum(0);
        odom_subpix_iterations->setMaximum(999999);
        odom_subpix_iterations->setSingleStep(1);
        odom_subpix_iterations->setValue(20);

        gridLayout_34->addWidget(odom_subpix_iterations, 1, 0, 1, 1);

        label_198 = new QLabel(groupBox_7);
        label_198->setObjectName(QString::fromUtf8("label_198"));
        label_198->setWordWrap(true);

        gridLayout_34->addWidget(label_198, 1, 1, 1, 1);

        odom_subpix_eps = new QDoubleSpinBox(groupBox_7);
        odom_subpix_eps->setObjectName(QString::fromUtf8("odom_subpix_eps"));
        odom_subpix_eps->setDecimals(3);
        odom_subpix_eps->setMinimum(0.001);
        odom_subpix_eps->setMaximum(0.1);
        odom_subpix_eps->setSingleStep(0.01);
        odom_subpix_eps->setValue(0.03);

        gridLayout_34->addWidget(odom_subpix_eps, 2, 0, 1, 1);

        label_214 = new QLabel(groupBox_7);
        label_214->setObjectName(QString::fromUtf8("label_214"));
        label_214->setWordWrap(true);

        gridLayout_34->addWidget(label_214, 2, 1, 1, 1);

        gridLayout_34->setColumnStretch(1, 1);

        verticalLayout_60->addLayout(gridLayout_34);


        verticalLayout_34->addWidget(groupBox_7);


        verticalLayout_32->addWidget(groupBox_odometry1);

        verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_32->addItem(verticalSpacer_14);

        stackedWidget->addWidget(page_12);
        page_26 = new QWidget();
        page_26->setObjectName(QString::fromUtf8("page_26"));
        verticalLayout_54 = new QVBoxLayout(page_26);
        verticalLayout_54->setObjectName(QString::fromUtf8("verticalLayout_54"));
        groupBox_odometryBOW2 = new QGroupBox(page_26);
        groupBox_odometryBOW2->setObjectName(QString::fromUtf8("groupBox_odometryBOW2"));
        gridLayout_29 = new QGridLayout(groupBox_odometryBOW2);
        gridLayout_29->setObjectName(QString::fromUtf8("gridLayout_29"));
        odom_localHistory = new QSpinBox(groupBox_odometryBOW2);
        odom_localHistory->setObjectName(QString::fromUtf8("odom_localHistory"));
        odom_localHistory->setMinimum(0);
        odom_localHistory->setMaximum(999999);
        odom_localHistory->setSingleStep(1);
        odom_localHistory->setValue(0);

        gridLayout_29->addWidget(odom_localHistory, 0, 0, 1, 1);

        label_190 = new QLabel(groupBox_odometryBOW2);
        label_190->setObjectName(QString::fromUtf8("label_190"));
        label_190->setWordWrap(true);

        gridLayout_29->addWidget(label_190, 0, 1, 1, 1);

        odom_bin_nn = new QComboBox(groupBox_odometryBOW2);
        odom_bin_nn->setObjectName(QString::fromUtf8("odom_bin_nn"));
        odom_bin_nn->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_29->addWidget(odom_bin_nn, 1, 0, 1, 1);

        label_201 = new QLabel(groupBox_odometryBOW2);
        label_201->setObjectName(QString::fromUtf8("label_201"));
        label_201->setWordWrap(true);

        gridLayout_29->addWidget(label_201, 1, 1, 1, 1);

        odom_bin_nndrRatio = new QDoubleSpinBox(groupBox_odometryBOW2);
        odom_bin_nndrRatio->setObjectName(QString::fromUtf8("odom_bin_nndrRatio"));
        odom_bin_nndrRatio->setDecimals(1);
        odom_bin_nndrRatio->setMinimum(0.1);
        odom_bin_nndrRatio->setMaximum(1);
        odom_bin_nndrRatio->setSingleStep(0.1);
        odom_bin_nndrRatio->setValue(0.7);

        gridLayout_29->addWidget(odom_bin_nndrRatio, 2, 0, 1, 1);

        label_202 = new QLabel(groupBox_odometryBOW2);
        label_202->setObjectName(QString::fromUtf8("label_202"));
        label_202->setWordWrap(true);

        gridLayout_29->addWidget(label_202, 2, 1, 1, 1);

        gridLayout_29->setColumnStretch(1, 1);

        verticalLayout_54->addWidget(groupBox_odometryBOW2);

        verticalSpacer_27 = new QSpacerItem(20, 670, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_54->addItem(verticalSpacer_27);

        stackedWidget->addWidget(page_26);
        page_28 = new QWidget();
        page_28->setObjectName(QString::fromUtf8("page_28"));
        verticalLayout_55 = new QVBoxLayout(page_28);
        verticalLayout_55->setObjectName(QString::fromUtf8("verticalLayout_55"));
        groupBox_odometryFlow2 = new QGroupBox(page_28);
        groupBox_odometryFlow2->setObjectName(QString::fromUtf8("groupBox_odometryFlow2"));
        verticalLayout_53 = new QVBoxLayout(groupBox_odometryFlow2);
        verticalLayout_53->setObjectName(QString::fromUtf8("verticalLayout_53"));
        label_199 = new QLabel(groupBox_odometryFlow2);
        label_199->setObjectName(QString::fromUtf8("label_199"));
        label_199->setWordWrap(true);

        verticalLayout_53->addWidget(label_199);

        groupBox_4 = new QGroupBox(groupBox_odometryFlow2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_30 = new QGridLayout(groupBox_4);
        gridLayout_30->setObjectName(QString::fromUtf8("gridLayout_30"));
        odom_flow_winSize = new QSpinBox(groupBox_4);
        odom_flow_winSize->setObjectName(QString::fromUtf8("odom_flow_winSize"));
        odom_flow_winSize->setMinimum(1);
        odom_flow_winSize->setMaximum(999999);
        odom_flow_winSize->setSingleStep(1);
        odom_flow_winSize->setValue(21);

        gridLayout_30->addWidget(odom_flow_winSize, 0, 0, 1, 1);

        label_192 = new QLabel(groupBox_4);
        label_192->setObjectName(QString::fromUtf8("label_192"));
        label_192->setWordWrap(true);

        gridLayout_30->addWidget(label_192, 0, 1, 1, 1);

        odom_flow_iterations = new QSpinBox(groupBox_4);
        odom_flow_iterations->setObjectName(QString::fromUtf8("odom_flow_iterations"));
        odom_flow_iterations->setMinimum(1);
        odom_flow_iterations->setMaximum(999999);
        odom_flow_iterations->setSingleStep(1);
        odom_flow_iterations->setValue(30);

        gridLayout_30->addWidget(odom_flow_iterations, 1, 0, 1, 1);

        label_193 = new QLabel(groupBox_4);
        label_193->setObjectName(QString::fromUtf8("label_193"));
        label_193->setWordWrap(true);

        gridLayout_30->addWidget(label_193, 1, 1, 1, 1);

        label_194 = new QLabel(groupBox_4);
        label_194->setObjectName(QString::fromUtf8("label_194"));
        label_194->setWordWrap(true);

        gridLayout_30->addWidget(label_194, 2, 1, 1, 1);

        odom_flow_maxLevel = new QSpinBox(groupBox_4);
        odom_flow_maxLevel->setObjectName(QString::fromUtf8("odom_flow_maxLevel"));
        odom_flow_maxLevel->setMinimum(0);
        odom_flow_maxLevel->setMaximum(999999);
        odom_flow_maxLevel->setSingleStep(1);
        odom_flow_maxLevel->setValue(3);

        gridLayout_30->addWidget(odom_flow_maxLevel, 3, 0, 1, 1);

        label_195 = new QLabel(groupBox_4);
        label_195->setObjectName(QString::fromUtf8("label_195"));
        label_195->setWordWrap(true);

        gridLayout_30->addWidget(label_195, 3, 1, 1, 1);

        odom_flow_eps = new QDoubleSpinBox(groupBox_4);
        odom_flow_eps->setObjectName(QString::fromUtf8("odom_flow_eps"));
        odom_flow_eps->setDecimals(3);
        odom_flow_eps->setMinimum(0.001);
        odom_flow_eps->setMaximum(1);
        odom_flow_eps->setSingleStep(0.01);
        odom_flow_eps->setValue(0.01);

        gridLayout_30->addWidget(odom_flow_eps, 2, 0, 1, 1);

        gridLayout_30->setColumnStretch(1, 1);

        verticalLayout_53->addWidget(groupBox_4);


        verticalLayout_55->addWidget(groupBox_odometryFlow2);

        verticalSpacer_28 = new QSpacerItem(20, 518, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_55->addItem(verticalSpacer_28);

        stackedWidget->addWidget(page_28);
        page_30 = new QWidget();
        page_30->setObjectName(QString::fromUtf8("page_30"));
        verticalLayout_169 = new QVBoxLayout(page_30);
        verticalLayout_169->setObjectName(QString::fromUtf8("verticalLayout_169"));
        groupBox_odometryMono2 = new QGroupBox(page_30);
        groupBox_odometryMono2->setObjectName(QString::fromUtf8("groupBox_odometryMono2"));
        verticalLayout_168 = new QVBoxLayout(groupBox_odometryMono2);
        verticalLayout_168->setObjectName(QString::fromUtf8("verticalLayout_168"));
        label_661 = new QLabel(groupBox_odometryMono2);
        label_661->setObjectName(QString::fromUtf8("label_661"));
        label_661->setWordWrap(true);

        verticalLayout_168->addWidget(label_661);

        gridLayout_166 = new QGridLayout();
        gridLayout_166->setObjectName(QString::fromUtf8("gridLayout_166"));
        doubleSpinBox_minFlow = new QDoubleSpinBox(groupBox_odometryMono2);
        doubleSpinBox_minFlow->setObjectName(QString::fromUtf8("doubleSpinBox_minFlow"));
        doubleSpinBox_minFlow->setDecimals(0);
        doubleSpinBox_minFlow->setMaximum(1000);
        doubleSpinBox_minFlow->setSingleStep(10);
        doubleSpinBox_minFlow->setValue(100);

        gridLayout_166->addWidget(doubleSpinBox_minFlow, 0, 0, 1, 1);

        label_659 = new QLabel(groupBox_odometryMono2);
        label_659->setObjectName(QString::fromUtf8("label_659"));
        label_659->setWordWrap(true);

        gridLayout_166->addWidget(label_659, 0, 1, 1, 1);

        doubleSpinBox_minTranslation = new QDoubleSpinBox(groupBox_odometryMono2);
        doubleSpinBox_minTranslation->setObjectName(QString::fromUtf8("doubleSpinBox_minTranslation"));
        doubleSpinBox_minTranslation->setMinimum(0);
        doubleSpinBox_minTranslation->setSingleStep(0.01);
        doubleSpinBox_minTranslation->setValue(0.02);

        gridLayout_166->addWidget(doubleSpinBox_minTranslation, 2, 0, 1, 1);

        label_660 = new QLabel(groupBox_odometryMono2);
        label_660->setObjectName(QString::fromUtf8("label_660"));
        label_660->setWordWrap(true);

        gridLayout_166->addWidget(label_660, 2, 1, 1, 1);

        label_662 = new QLabel(groupBox_odometryMono2);
        label_662->setObjectName(QString::fromUtf8("label_662"));
        label_662->setWordWrap(true);

        gridLayout_166->addWidget(label_662, 1, 1, 1, 1);

        doubleSpinBox_minInitTranslation = new QDoubleSpinBox(groupBox_odometryMono2);
        doubleSpinBox_minInitTranslation->setObjectName(QString::fromUtf8("doubleSpinBox_minInitTranslation"));
        doubleSpinBox_minInitTranslation->setMinimum(0);
        doubleSpinBox_minInitTranslation->setSingleStep(0.01);
        doubleSpinBox_minInitTranslation->setValue(0.1);

        gridLayout_166->addWidget(doubleSpinBox_minInitTranslation, 1, 0, 1, 1);

        label_663 = new QLabel(groupBox_odometryMono2);
        label_663->setObjectName(QString::fromUtf8("label_663"));
        label_663->setWordWrap(true);

        gridLayout_166->addWidget(label_663, 3, 1, 1, 1);

        doubleSpinBox_maxVariance = new QDoubleSpinBox(groupBox_odometryMono2);
        doubleSpinBox_maxVariance->setObjectName(QString::fromUtf8("doubleSpinBox_maxVariance"));
        doubleSpinBox_maxVariance->setDecimals(3);
        doubleSpinBox_maxVariance->setMinimum(0);
        doubleSpinBox_maxVariance->setSingleStep(0.001);
        doubleSpinBox_maxVariance->setValue(0.01);

        gridLayout_166->addWidget(doubleSpinBox_maxVariance, 3, 0, 1, 1);

        gridLayout_166->setColumnStretch(1, 1);

        verticalLayout_168->addLayout(gridLayout_166);


        verticalLayout_169->addWidget(groupBox_odometryMono2);

        verticalSpacer_30 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_169->addItem(verticalSpacer_30);

        stackedWidget->addWidget(page_30);

        verticalLayout_24->addWidget(stackedWidget);


        verticalLayout_16->addLayout(verticalLayout_24);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_17->addWidget(scrollArea);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(12, -1, 12, -1);
        buttonBox_local = new QDialogButtonBox(preferencesDialog);
        buttonBox_local->setObjectName(QString::fromUtf8("buttonBox_local"));
        buttonBox_local->setOrientation(Qt::Horizontal);
        buttonBox_local->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::RestoreDefaults);

        horizontalLayout->addWidget(buttonBox_local);


        verticalLayout_17->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_17);

        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_26->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_basic = new QRadioButton(preferencesDialog);
        radioButton_basic->setObjectName(QString::fromUtf8("radioButton_basic"));
        radioButton_basic->setChecked(true);

        horizontalLayout_3->addWidget(radioButton_basic);

        radioButton_advanced = new QRadioButton(preferencesDialog);
        radioButton_advanced->setObjectName(QString::fromUtf8("radioButton_advanced"));

        horizontalLayout_3->addWidget(radioButton_advanced);

        buttonBox_global = new QDialogButtonBox(preferencesDialog);
        buttonBox_global->setObjectName(QString::fromUtf8("buttonBox_global"));
        buttonBox_global->setOrientation(Qt::Horizontal);
        buttonBox_global->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_3->addWidget(buttonBox_global);


        verticalLayout_26->addLayout(horizontalLayout_3);


        retranslateUi(preferencesDialog);

        stackedWidget->setCurrentIndex(19);
        comboBox_loggerLevel->setCurrentIndex(2);
        comboBox_loggerEventLevel->setCurrentIndex(3);
        comboBox_loggerPauseLevel->setCurrentIndex(3);
        comboBox_loggerType->setCurrentIndex(1);
        source_comboBox_image_type->setCurrentIndex(0);
        stackedWidget_image->setCurrentIndex(0);
        comboBox_dbSynchronous->setCurrentIndex(2);
        reextract_nn->setCurrentIndex(3);
        reextract_type->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(preferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *preferencesDialog)
    {
        preferencesDialog->setWindowTitle(QApplication::translate("preferencesDialog", "Preferences", 0, QApplication::UnicodeUTF8));
        groupBox_generalSettingsGui0->setTitle(QApplication::translate("preferencesDialog", "General settings (GUI)", 0, QApplication::UnicodeUTF8));
        general_checkBox_imagesKept->setText(QString());
        label_59->setText(QApplication::translate("preferencesDialog", "Insert new data received in the GUI cache. Used to show the loop closure image and the 3D Map.", 0, QApplication::UnicodeUTF8));
        checkBox_beep->setText(QString());
        label_88->setText(QApplication::translate("preferencesDialog", "Beep! on special events (finished processing the data set, an error has occured, ...).", 0, QApplication::UnicodeUTF8));
        label_102->setText(QApplication::translate("preferencesDialog", "Notify when a new global path is received.", 0, QApplication::UnicodeUTF8));
        checkBox_notifyWhenNewGlobalPathIsReceived->setText(QString());
        groupBox_5->setTitle(QApplication::translate("preferencesDialog", "Loop closure detection view", 0, QApplication::UnicodeUTF8));
        checkBox_imageRejectedShown->setText(QString());
        label_105->setText(QApplication::translate("preferencesDialog", "Show the image of the highest hypothesis.", 0, QApplication::UnicodeUTF8));
        checkBox_imageHighestHypShown->setText(QString());
        label_73->setText(QApplication::translate("preferencesDialog", "Show image when an hypothesis is rejected.", 0, QApplication::UnicodeUTF8));
        label_70->setText(QApplication::translate("preferencesDialog", "Vertical layout.", 0, QApplication::UnicodeUTF8));
        checkBox_verticalLayoutUsed->setText(QString());
        label_222->setText(QApplication::translate("preferencesDialog", "Show loop closure hypotheses on the graph view. Nodes are colorized from blue to red depending on the highest hypothesis (which is red). Posterior hypotheses should be published. ", 0, QApplication::UnicodeUTF8));
        checkBox_posteriorGraphView->setText(QString());
        groupBox->setTitle(QApplication::translate("preferencesDialog", "3D Map view", 0, QApplication::UnicodeUTF8));
        label_86->setText(QApplication::translate("preferencesDialog", "Odometry warning theshold:\n"
"Show a yellow background when the number of odometry inliers goes under this threshold. If 0, it is ignored. You can see the current odometry inliers count under Statistics view -> General -> Odom inliers.", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("preferencesDialog", "Save/load settings", 0, QApplication::UnicodeUTF8));
        pushButton_loadConfig->setText(QApplication::translate("preferencesDialog", "Load settings (*.ini) ...", 0, QApplication::UnicodeUTF8));
        pushButton_saveConfig->setText(QApplication::translate("preferencesDialog", "Save settings (*.ini) ...", 0, QApplication::UnicodeUTF8));
        pushButton_resetConfig->setText(QApplication::translate("preferencesDialog", "Reset all settings", 0, QApplication::UnicodeUTF8));
        groupBox_cloudRendering1->setTitle(QApplication::translate("preferencesDialog", "3D Rendering", 0, QApplication::UnicodeUTF8));
        groupBox_poseFiltering->setTitle(QApplication::translate("preferencesDialog", "Cloud filtering", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("preferencesDialog", "For visualization, superposed clouds are not shown. By comparing poses in the same area, only one cloud in a fixed radius and angle is kept.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_cloudFilterRadius->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("preferencesDialog", "Radius.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_cloudFilterAngle->setSuffix(QApplication::translate("preferencesDialog", " degrees", 0, QApplication::UnicodeUTF8));
        label_48->setText(QApplication::translate("preferencesDialog", "Angle.", 0, QApplication::UnicodeUTF8));
        groupBox_gridMap->setTitle(QApplication::translate("preferencesDialog", "Occupancy grid map", 0, QApplication::UnicodeUTF8));
        label_167->setText(QApplication::translate("preferencesDialog", "When the Graph view is visible or if the \"Show in 3D map view\" below is checked, the grid map is generated using the laser scans.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_map_opacity->setSuffix(QString());
        checkBox_map_shown->setText(QString());
        label_map_shown->setText(QApplication::translate("preferencesDialog", "Show in 3D map view.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_map_resolution->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_159->setText(QApplication::translate("preferencesDialog", "Resolution (cell size).", 0, QApplication::UnicodeUTF8));
        label_170->setText(QApplication::translate("preferencesDialog", "Opacity.", 0, QApplication::UnicodeUTF8));
        checkBox_map_occupancyFrom3DCloud->setText(QString());
        label_210->setText(QApplication::translate("preferencesDialog", "Occupancy from 3D cloud projection on the ground. Laser scans are ignored when activated.", 0, QApplication::UnicodeUTF8));
        label_224->setText(QApplication::translate("preferencesDialog", "Erode.", 0, QApplication::UnicodeUTF8));
        checkBox_map_erode->setText(QString());
        checkBox_showScans->setText(QString());
        label_108->setText(QApplication::translate("preferencesDialog", "3D cloud decimation (1-2-4-8-...).", 0, QApplication::UnicodeUTF8));
        label_154->setText(QApplication::translate("preferencesDialog", "Map", 0, QApplication::UnicodeUTF8));
        label_137->setText(QApplication::translate("preferencesDialog", "Odometry", 0, QApplication::UnicodeUTF8));
        checkBox_showClouds->setText(QString());
        checkBox_showOdomClouds->setText(QString());
        label_119->setText(QApplication::translate("preferencesDialog", "Show 3D clouds.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_voxelSize->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_voxelSize_odom->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_107->setText(QApplication::translate("preferencesDialog", "3D cloud voxel size.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_maxDepth->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_maxDepth_odom->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_132->setText(QApplication::translate("preferencesDialog", "3D cloud maximum depth (0 means no limit).", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_opacity->setSuffix(QString());
        doubleSpinBox_opacity_odom->setSuffix(QString());
        label_155->setText(QApplication::translate("preferencesDialog", "3D cloud opacity.", 0, QApplication::UnicodeUTF8));
        label_157->setText(QApplication::translate("preferencesDialog", "3D cloud point size (1..64).", 0, QApplication::UnicodeUTF8));
        checkBox_showOdomScans->setText(QString());
        label_110->setText(QApplication::translate("preferencesDialog", "Show 2D scans.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_opacity_scan->setSuffix(QString());
        checkBox_meshing->setText(QString());
        doubleSpinBox_opacity_odom_scan->setSuffix(QString());
        label_156->setText(QApplication::translate("preferencesDialog", "2D scan opacity.", 0, QApplication::UnicodeUTF8));
        label_158->setText(QApplication::translate("preferencesDialog", "2D scan point size (1..64).", 0, QApplication::UnicodeUTF8));
        label_168->setText(QApplication::translate("preferencesDialog", "Sphere radius that is to be used for determining the k-nearest neighbors used for triangulating (GP3). Guidelines: 4 times the voxel size, 0.025 for voxel=0.", 0, QApplication::UnicodeUTF8));
        label_85->setText(QApplication::translate("preferencesDialog", "Mesh smoothing using Moving Least Squares algorithm (MLS).", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_gp3Radius->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_mlsRadius->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_169->setText(QApplication::translate("preferencesDialog", "Online meshing using Greedy Projection Triangulation (GP3).", 0, QApplication::UnicodeUTF8));
        label_87->setText(QApplication::translate("preferencesDialog", "MLS search radius: Set the sphere radius that is to be used for determining the k-nearest neighbors used for fitting. Guidelines: 4 times the voxel size, 0.025 for voxel=0.", 0, QApplication::UnicodeUTF8));
        checkBox_mls->setText(QString());
        label_71->setText(QApplication::translate("preferencesDialog", "Set the number of k nearest neighbors to use for the normal estimation to create the mesh. Not used when mesh smoothing below is used.", 0, QApplication::UnicodeUTF8));
        label_213->setText(QApplication::translate("preferencesDialog", "Show graphs.", 0, QApplication::UnicodeUTF8));
        checkBox_showGraphs->setText(QString());
        groupBox_logging1->setTitle(QApplication::translate("preferencesDialog", "Logging", 0, QApplication::UnicodeUTF8));
        comboBox_loggerLevel->clear();
        comboBox_loggerLevel->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "DEBUG", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "INFO", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "WARNING", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ERROR", 0, QApplication::UnicodeUTF8)
        );
        label_60->setText(QApplication::translate("preferencesDialog", "Logger level.", 0, QApplication::UnicodeUTF8));
        comboBox_loggerEventLevel->clear();
        comboBox_loggerEventLevel->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "DEBUG", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "INFO", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "WARNING", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ERROR", 0, QApplication::UnicodeUTF8)
        );
        label_61->setText(QApplication::translate("preferencesDialog", "Logger event sent level (fatal messages are always sent).", 0, QApplication::UnicodeUTF8));
        comboBox_loggerPauseLevel->clear();
        comboBox_loggerPauseLevel->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "DEBUG", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "INFO", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "WARNING", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ERROR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FATAL", 0, QApplication::UnicodeUTF8)
        );
        label_62->setText(QApplication::translate("preferencesDialog", "Logger level on which the process will automatically pause, showing the log console. A level set under the logger event level will do nothing.", 0, QApplication::UnicodeUTF8));
        checkBox_logger_printTime->setText(QString());
        label_67->setText(QApplication::translate("preferencesDialog", "Logger print time.", 0, QApplication::UnicodeUTF8));
        comboBox_loggerType->clear();
        comboBox_loggerType->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "No log", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Console", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "File", 0, QApplication::UnicodeUTF8)
        );
        label_68->setText(QApplication::translate("preferencesDialog", "Logger type: \n"
"when using the file type, logs are saved in LogRtabmap.txt (located in the working directory).", 0, QApplication::UnicodeUTF8));
        groupBox_source0->setTitle(QApplication::translate("preferencesDialog", "Source", 0, QApplication::UnicodeUTF8));
        general_doubleSpinBox_imgRate->setSuffix(QApplication::translate("preferencesDialog", " Hz", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("preferencesDialog", "Input rate (0 means as fast as possible).", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("preferencesDialog", "Mirroring mode (flip image horizontally). It has no effect on database source.", 0, QApplication::UnicodeUTF8));
        source_mirroring->setText(QString());
        groupBox_sourceImage->setTitle(QApplication::translate("preferencesDialog", "Image source", 0, QApplication::UnicodeUTF8));
        source_comboBox_image_type->clear();
        source_comboBox_image_type->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "Usb camera", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Images", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Video file", 0, QApplication::UnicodeUTF8)
        );
        label_20->setText(QApplication::translate("preferencesDialog", "Source type. 0-Usb camera (Webcam), 1-Images (directory of images), 2-Video (AVI)", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("preferencesDialog", "Image width (set to 0 to use the default size of the source).", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("preferencesDialog", "Image height (set to 0 to use the default size of the source).", 0, QApplication::UnicodeUTF8));
        source_groupBox_usbDevice->setTitle(QApplication::translate("preferencesDialog", "Usb device", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("preferencesDialog", "Usb device.", 0, QApplication::UnicodeUTF8));
        source_groupBox_images->setTitle(QApplication::translate("preferencesDialog", "Images dataset", 0, QApplication::UnicodeUTF8));
        source_images_toolButton_selectSource->setText(QApplication::translate("preferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("preferencesDialog", "Start position (default 1, 0=start from the last).", 0, QApplication::UnicodeUTF8));
        source_images_refreshDir->setText(QString());
        label_34->setText(QApplication::translate("preferencesDialog", "Refresh the directory files list after each image loaded.", 0, QApplication::UnicodeUTF8));
        source_groupBox_video->setTitle(QApplication::translate("preferencesDialog", "Video (AVI)", 0, QApplication::UnicodeUTF8));
        source_video_toolButton_selectSource->setText(QApplication::translate("preferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        groupBox_sourceDatabase->setTitle(QApplication::translate("preferencesDialog", "Database source", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_dbViewer->setToolTip(QApplication::translate("preferencesDialog", "Open database viewer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_dbViewer->setText(QString());
        source_checkBox_ignoreOdometry->setText(QString());
        source_database_toolButton_selectSource->setText(QApplication::translate("preferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        label_58->setText(QApplication::translate("preferencesDialog", "Start position (index)", 0, QApplication::UnicodeUTF8));
        label_72->setText(QApplication::translate("preferencesDialog", "Ignore odometry saved in the database, so if RGB-D SLAM is activated, odometry will be recomputed.", 0, QApplication::UnicodeUTF8));
        label_80->setText(QApplication::translate("preferencesDialog", "Ignore goal delay.", 0, QApplication::UnicodeUTF8));
        source_checkBox_ignoreGoalDelay->setText(QString());
        label_90->setText(QApplication::translate("preferencesDialog", "Use database stamps as input rate.", 0, QApplication::UnicodeUTF8));
        source_checkBox_useDbStamps->setText(QString());
        groupBox_sourceOpenni->setTitle(QApplication::translate("preferencesDialog", "RGB-D camera", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("preferencesDialog", "Grabber for RGB-D devices (i.e., Primesense PSDK, Microsoft Kinect, Asus XTion Pro/Live).", 0, QApplication::UnicodeUTF8));
        comboBox_cameraRGBD->clear();
        comboBox_cameraRGBD->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "OpenNI-PCL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Freenect", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "OpenNI-CV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "OpenNI-CV-ASUS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "OpenNI2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Freenect2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "StereoDC1394", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "StereoFlyCapture2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "JdeRobotInterfaces", 0, QApplication::UnicodeUTF8)
        );
        pushButton_calibrate->setText(QApplication::translate("preferencesDialog", "Calibrate", 0, QApplication::UnicodeUTF8));
        label_46->setText(QApplication::translate("preferencesDialog", "On initialization, calibration files are loaded from the \"camera_info\" folder in the working directory.", 0, QApplication::UnicodeUTF8));
        pushButton_test_rgbd_camera->setText(QApplication::translate("preferencesDialog", "Test", 0, QApplication::UnicodeUTF8));
        label_228->setText(QApplication::translate("preferencesDialog", "Driver", 0, QApplication::UnicodeUTF8));
        groupBox_openni2->setTitle(QApplication::translate("preferencesDialog", "OpenNI 2", 0, QApplication::UnicodeUTF8));
        openni2_autoWhiteBalance->setText(QString());
        label_217->setText(QApplication::translate("preferencesDialog", "Auto white balance.", 0, QApplication::UnicodeUTF8));
        openni2_autoExposure->setText(QString());
        label_218->setText(QApplication::translate("preferencesDialog", "Auto exposure.", 0, QApplication::UnicodeUTF8));
        label_219->setText(QApplication::translate("preferencesDialog", "Exposure.", 0, QApplication::UnicodeUTF8));
        label_220->setText(QApplication::translate("preferencesDialog", "Gain.", 0, QApplication::UnicodeUTF8));
        openni2_mirroring->setText(QString());
        label_223->setText(QApplication::translate("preferencesDialog", "Mirroring.", 0, QApplication::UnicodeUTF8));
        groupBox_freenect2->setTitle(QApplication::translate("preferencesDialog", "Freenect2", 0, QApplication::UnicodeUTF8));
        label_230->setText(QApplication::translate("preferencesDialog", "Format.", 0, QApplication::UnicodeUTF8));
        comboBox_freenect2Format->clear();
        comboBox_freenect2Format->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "RGB+Depth SD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "RGB+Depth HD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "IR+Depth", 0, QApplication::UnicodeUTF8)
        );
        lineEdit_openniDevice->setText(QString());
        label_39->setText(QApplication::translate("preferencesDialog", "ID of the device, which might be a serial number, bus@address or the index of the device. If empty, the first device found is taken.", 0, QApplication::UnicodeUTF8));
        lineEdit_openniLocalTransform->setText(QApplication::translate("preferencesDialog", "0 0 0 -PI_2 0 -PI_2", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("preferencesDialog", "Local transform from /base_link to /camera_link. Format (6 values): x y z roll pitch yaw.", 0, QApplication::UnicodeUTF8));
        label_229->setText(QApplication::translate("preferencesDialog", "Only RGB images are published.", 0, QApplication::UnicodeUTF8));
        checkbox_rgbd_colorOnly->setText(QString());
        groupBox_rtabmap_basic0->setTitle(QApplication::translate("preferencesDialog", "RTAB-Map settings", 0, QApplication::UnicodeUTF8));
        general_doubleSpinBox_detectionRate_2->setSuffix(QApplication::translate("preferencesDialog", " Hz", 0, QApplication::UnicodeUTF8));
        general_checkBox_SLAM_mode_2->setText(QString());
        label_79->setText(QApplication::translate("preferencesDialog", "SLAM mode.\n"
" If set to false, global localization is performed, so without adding new data to the map.", 0, QApplication::UnicodeUTF8));
        general_checkBox_activateRGBD_2->setText(QString());
        label_activateRGBD_2->setText(QApplication::translate("preferencesDialog", "Activate metric RGB-D SLAM.\n"
"If set to false, classic RTAB-Map loop closure detection is done using only images and without any metric information.", 0, QApplication::UnicodeUTF8));
        general_checkBox_publishStats_2->setText(QString());
        label_publishStat->setText(QApplication::translate("preferencesDialog", "Publish statistics.", 0, QApplication::UnicodeUTF8));
        label_134->setText(QApplication::translate("preferencesDialog", "Detection rate (0 means inf). RTAB-Map will filter input images to satisfy this rate.", 0, QApplication::UnicodeUTF8));
        label_112->setText(QApplication::translate("preferencesDialog", "Images buffer size (0 means inf).", 0, QApplication::UnicodeUTF8));
        general_doubleSpinBox_timeThr_2->setSuffix(QApplication::translate("preferencesDialog", " ms", 0, QApplication::UnicodeUTF8));
        label_timeLimit_2->setText(QApplication::translate("preferencesDialog", "T_time : Max time (ms) allowed (0 means inf).", 0, QApplication::UnicodeUTF8));
        label_121->setText(QApplication::translate("preferencesDialog", "T_loop : Loop closure threshold.", 0, QApplication::UnicodeUTF8));
        label_similarity_2->setText(QApplication::translate("preferencesDialog", "T_similarity : Similarity threshold (for Rehearsal/Weight Update).", 0, QApplication::UnicodeUTF8));
        label_126->setText(QApplication::translate("preferencesDialog", "STM size : Short-term memory size.", 0, QApplication::UnicodeUTF8));
        groupBox_rtabmap_advanced0->setTitle(QApplication::translate("preferencesDialog", "RTAB-Map settings", 0, QApplication::UnicodeUTF8));
        general_checkBox_SLAM_mode->setText(QString());
        label_92->setText(QApplication::translate("preferencesDialog", "SLAM mode.\n"
" If set to false, global localization is performed, so without adding new data to the map.", 0, QApplication::UnicodeUTF8));
        general_doubleSpinBox_detectionRate->setSuffix(QApplication::translate("preferencesDialog", " Hz", 0, QApplication::UnicodeUTF8));
        label_135->setText(QApplication::translate("preferencesDialog", "Detection rate (0 means inf). RTAB-Map will filter input data to satisfy this rate. If you want to process all data, consider set \"Detection rate\" to 0 and \"Data buffer size\" to 0.", 0, QApplication::UnicodeUTF8));
        label_84->setText(QApplication::translate("preferencesDialog", "Data buffer size (0 means inf).", 0, QApplication::UnicodeUTF8));
        general_checkBox_startNewMapOnLoopClosure->setText(QString());
        label_185->setText(QApplication::translate("preferencesDialog", "Start a new map only if there is a global loop closure detected first with a previous map. If there is no map in memory, a new map is still created.", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("preferencesDialog", "Thresholds", 0, QApplication::UnicodeUTF8));
        general_doubleSpinBox_timeThr->setSuffix(QApplication::translate("preferencesDialog", " ms", 0, QApplication::UnicodeUTF8));
        label_timeLimit->setText(QApplication::translate("preferencesDialog", "T_time : Max time (ms) allowed (0 means inf).", 0, QApplication::UnicodeUTF8));
        label_maxWmSize->setText(QApplication::translate("preferencesDialog", "Maximum signatures allowed in Working Memory (0 means inf).", 0, QApplication::UnicodeUTF8));
        label_93->setText(QApplication::translate("preferencesDialog", "T_loop : Loop closure threshold. Setting to 0 means that the new place hypothesis is used as threshold.", 0, QApplication::UnicodeUTF8));
        label_96->setText(QApplication::translate("preferencesDialog", "T_ratio : The loop closure hypothesis must be over T_ratio x lastHypothesisValue.", 0, QApplication::UnicodeUTF8));
        groupBox_publishing->setTitle(QApplication::translate("preferencesDialog", "Publish statistics (stuff to be shown in the GUI)", 0, QApplication::UnicodeUTF8));
        general_checkBox_publishRawData->setText(QString());
        label_91->setText(QApplication::translate("preferencesDialog", "Publish signature data.", 0, QApplication::UnicodeUTF8));
        general_checkBox_publishPdf->setText(QString());
        label_106->setText(QApplication::translate("preferencesDialog", "Publish loop closure hypotheses (pdf).", 0, QApplication::UnicodeUTF8));
        general_checkBox_publishLikelihood->setText(QString());
        label_116->setText(QApplication::translate("preferencesDialog", "Publish loop closure likelihood.", 0, QApplication::UnicodeUTF8));
        groupBox_statistics->setTitle(QApplication::translate("preferencesDialog", "Log statistics (LogI.txt and LogF.txt) in the working directory.", 0, QApplication::UnicodeUTF8));
        general_checkBox_statisticLogsBufferedInRAM->setText(QString());
        label_143->setText(QApplication::translate("preferencesDialog", "Statistics logs buffered in RAM and written to hard drive on exit (otherwise logs are written at each iteration on hard drive).", 0, QApplication::UnicodeUTF8));
        general_checkBox_statisticLoggedHeaders->setText(QString());
        label_209->setText(QApplication::translate("preferencesDialog", "Add headers (column names) to log files.", 0, QApplication::UnicodeUTF8));
        label_workingDirectory->setText(QApplication::translate("preferencesDialog", "Working directory (where to put logs/dump stuff).", 0, QApplication::UnicodeUTF8));
        toolButton_workingDirectory->setText(QApplication::translate("preferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        groupBox_bayes1->setTitle(QApplication::translate("preferencesDialog", "Bayes filter", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("preferencesDialog", "Prediction probabilities for each loop closure event:", 0, QApplication::UnicodeUTF8));
        label_76->setText(QApplication::translate("preferencesDialog", "We can edit how will like the prediction used in the Bayes filter when there is a loop closure hypothesis. Format is [VP LC N1 N2 N3...]. LC is the loop closure event and N# are its neighbors. VP (virtual place) is the probability to be in a new place when a loop closure was found on the last iteration.", 0, QApplication::UnicodeUTF8));
        label_65->setText(QApplication::translate("preferencesDialog", "Prediction", 0, QApplication::UnicodeUTF8));
        label_66->setText(QApplication::translate("preferencesDialog", "Sum", 0, QApplication::UnicodeUTF8));
        label_prediction_sum->setText(QString());
        label_63->setText(QApplication::translate("preferencesDialog", "Preview", 0, QApplication::UnicodeUTF8));
        checkBox_bayes_fullPredictionUpdate->setText(QString());
        label_113->setText(QApplication::translate("preferencesDialog", "The VP here is the probability to be in a new place since no loop closure was found on the last iteration.", 0, QApplication::UnicodeUTF8));
        label_142->setText(QApplication::translate("preferencesDialog", "Regenerate all the prediction matrix on each iteration (otherwise only removed/added ids are updated).", 0, QApplication::UnicodeUTF8));
        label_82->setText(QApplication::translate("preferencesDialog", "Use the tf-idf method to compute the likelihood. Otherwise, images are compared with each other.", 0, QApplication::UnicodeUTF8));
        checkBox_kp_tfIdfLikelihoodUsed->setText(QString());
        groupBox_memory1->setTitle(QApplication::translate("preferencesDialog", "Memory", 0, QApplication::UnicodeUTF8));
        label_retrieved_4->setText(QApplication::translate("preferencesDialog", "Initialize the Woking Memory with all nodes from Long-Term memory, instead of only nodes of the last session. This may be useful in localization mode, where less processing time is required than in SLAM mode, so more nodes can be kept in Working Memory.", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("preferencesDialog", "STM size : Short-term memory size.", 0, QApplication::UnicodeUTF8));
        label_ratioRecent->setText(QApplication::translate("preferencesDialog", "T_recent : Ratio of locations after the last loop closure in WM that cannot be transferred.", 0, QApplication::UnicodeUTF8));
        label_retrieved->setText(QApplication::translate("preferencesDialog", "Maximum locations retrieved at the same time from LTM to WM.", 0, QApplication::UnicodeUTF8));
        general_checkBox_generateIds->setText(QString());
        label_retrieved_2->setText(QApplication::translate("preferencesDialog", "True=Generate location Ids, False=use input image ids.", 0, QApplication::UnicodeUTF8));
        general_checkBox_badSignaturesIgnored->setText(QString());
        label_retrieved_3->setText(QApplication::translate("preferencesDialog", "Bad signatures are ignored.", 0, QApplication::UnicodeUTF8));
        general_checkBox_initWMWithAllNodes->setText(QString());
        label_retrieved_5->setText(QApplication::translate("preferencesDialog", "Keep raw sensor data. Only useful to save loop closure computation time when features re-extraction is enabled. Disable to save RAM memory.", 0, QApplication::UnicodeUTF8));
        general_checkBox_keepRawData->setText(QString());
        label_retrieved_6->setText(QApplication::translate("preferencesDialog", "Image decimation. This feature can be used to save images in lower resolution (size/decimation).", 0, QApplication::UnicodeUTF8));
        label_retrieved_7->setText(QApplication::translate("preferencesDialog", "On transfer, signatures are sorted by weight->ID (i.e. the oldest of the lowest weighted signatures are transferred first). If false, the signatures are sorted by weight->Age (i.e. the oldest inserted in WM of the lowest weighted signatures are transferred first). Note that retrieval updates the age, not the ID.", 0, QApplication::UnicodeUTF8));
        general_checkBox_transferSortingByWeightId->setText(QString());
        label_retrieved_8->setText(QApplication::translate("preferencesDialog", "If > 0.0, voxelize laser scans when creating a location. This feature can be used to save laser scans already voxelized.", 0, QApplication::UnicodeUTF8));
        general_doubleSpinBox_laserScanVoxel->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        groupBox_rehearsal2->setTitle(QApplication::translate("preferencesDialog", "Rehearsal / Weight Update", 0, QApplication::UnicodeUTF8));
        general_checkBox_RehearsalIdUpdatedToNewOne->setText(QString());
        label_similarity->setText(QApplication::translate("preferencesDialog", "T_similarity : Similarity threshold. Values must be >=0.0 ans <=1.0.", 0, QApplication::UnicodeUTF8));
        label_rehearsalIdUpdate->setText(QApplication::translate("preferencesDialog", "On merging, update to new id.", 0, QApplication::UnicodeUTF8));
        label_rehearsalIdUpdate_2->setText(QApplication::translate("preferencesDialog", "Ignore Weight Update when the robot is moving.", 0, QApplication::UnicodeUTF8));
        rgdb_rehearsalWeightIgnoredWhileMoving->setText(QString());
        groupBox_database2->setTitle(QApplication::translate("preferencesDialog", "Database", 0, QApplication::UnicodeUTF8));
        general_checkBox_keepBinaryData->setText(QString());
        label_keepRawData->setText(QApplication::translate("preferencesDialog", "Keep sensor compressed data.", 0, QApplication::UnicodeUTF8));
        checkBox_dbInMemory->setText(QString());
        label_75->setText(QApplication::translate("preferencesDialog", "Sqlite3 cache size,\n"
"see Sqlite3 doc 'PRAGMA cache_size'.", 0, QApplication::UnicodeUTF8));
        comboBox_dbJournalMode->clear();
        comboBox_dbJournalMode->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "DELETE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "TRUNCATE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "PERSIST", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "MEMORY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "OFF", 0, QApplication::UnicodeUTF8)
        );
        label_77->setText(QApplication::translate("preferencesDialog", "Sqlite3 journal mode,\n"
"see Sqlite3 doc 'PRAGMA journal_mode'.", 0, QApplication::UnicodeUTF8));
        label_74->setText(QApplication::translate("preferencesDialog", "Using database in the memory instead of a file on the hard disk (this greatly improve database access performance but it requires more RAM memory).", 0, QApplication::UnicodeUTF8));
        label_89->setText(QApplication::translate("preferencesDialog", "Sqlite3 synchronous,\n"
"see Sqlite3 doc 'PRAGMA synchronous'.", 0, QApplication::UnicodeUTF8));
        label_120->setText(QApplication::translate("preferencesDialog", "Sqlite3 temp store,\n"
"see Sqlite3 doc 'PRAGMA temp_store'.", 0, QApplication::UnicodeUTF8));
        comboBox_dbSynchronous->clear();
        comboBox_dbSynchronous->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "OFF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "NORMAL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FULL", 0, QApplication::UnicodeUTF8)
        );
        comboBox_dbTempStore->clear();
        comboBox_dbTempStore->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "DEFAULT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FILE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "MEMORY", 0, QApplication::UnicodeUTF8)
        );
        label_keepRehearsed->setText(QApplication::translate("preferencesDialog", "Keep not linked nodes in db (rehearsed nodes and deleted nodes are saved).", 0, QApplication::UnicodeUTF8));
        general_checkBox_keepNotLinkedNodes->setText(QString());
        groupBox_signature_keypoint1->setTitle(QApplication::translate("preferencesDialog", "Visual word", 0, QApplication::UnicodeUTF8));
        comboBox_detector_strategy->clear();
        comboBox_detector_strategy->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "SURF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "SIFT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ORB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FAST+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FAST+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "GFTT+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "GFTT+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "BRISK", 0, QApplication::UnicodeUTF8)
        );
        label_117->setText(QApplication::translate("preferencesDialog", "Visual word type.", 0, QApplication::UnicodeUTF8));
        surf_doubleSpinBox_maxDepth->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_53->setToolTip(QApplication::translate("preferencesDialog", "0 means that the response (hessian) threshold \n"
"used for the detector will not be adapted. \n"
"Otherwise, the threshold is modified to \n"
"generate the number of words requested.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_53->setText(QApplication::translate("preferencesDialog", "Maximum words depth (0 means inf). Only used when a depth image is provided. Applied before \"Maximum words per image\".", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_22->setToolTip(QApplication::translate("preferencesDialog", "0 means that the response (hessian) threshold \n"
"used for the detector will not be adapted. \n"
"Otherwise, the threshold is modified to \n"
"generate the number of words requested.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_22->setText(QApplication::translate("preferencesDialog", "Maximum words per image (0=no maximum). Setting to -1 will disable features extraction, so disabling loop closure detection indirectly.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_81->setToolTip(QApplication::translate("preferencesDialog", "0 means that the response (hessian) threshold \n"
"used for the detector will not be adapted. \n"
"Otherwise, the threshold is modified to \n"
"generate the number of words requested.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_81->setText(QApplication::translate("preferencesDialog", "Bad signature ratio (less than Ratio x AverageWordsPerImage = bad).", 0, QApplication::UnicodeUTF8));
        label_101->setText(QApplication::translate("preferencesDialog", "ROI ratios [left, right, top, bottom] between 0 and 1.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_kp_roi0->setSuffix(QApplication::translate("preferencesDialog", " %", 0, QApplication::UnicodeUTF8));
        label_97->setText(QApplication::translate("preferencesDialog", "Left ROI ratio (0 = no change).", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_kp_roi1->setSuffix(QApplication::translate("preferencesDialog", " %", 0, QApplication::UnicodeUTF8));
        label_98->setText(QApplication::translate("preferencesDialog", "Right ROI ratio (0 = no change).", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_kp_roi2->setSuffix(QApplication::translate("preferencesDialog", " %", 0, QApplication::UnicodeUTF8));
        label_99->setText(QApplication::translate("preferencesDialog", "Top ROI ratio (0 = no change).", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_kp_roi3->setSuffix(QApplication::translate("preferencesDialog", " %", 0, QApplication::UnicodeUTF8));
        label_100->setText(QApplication::translate("preferencesDialog", "Bottom ROI ratio (0 = no change).", 0, QApplication::UnicodeUTF8));
        groupBox_dictionary_strategy2->setTitle(QApplication::translate("preferencesDialog", "Dictionary", 0, QApplication::UnicodeUTF8));
        toolButton_dictionaryPath->setText(QApplication::translate("preferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        label_78->setText(QApplication::translate("preferencesDialog", "NNDR ratio\n"
"(A matching pair is accepted, if its distance is closer than X times the distance of the second nearest neighbor)\n"
"Lower the ratio -> higher the precision.", 0, QApplication::UnicodeUTF8));
        label_dictionaryPath->setText(QApplication::translate("preferencesDialog", "Path to a pre-computed dictionary (when \"Use an incremental dictionary\" is not set).", 0, QApplication::UnicodeUTF8));
        checkBox_dictionary_incremental->setText(QString());
        label_12->setText(QApplication::translate("preferencesDialog", "Nearest neighbor strategy.", 0, QApplication::UnicodeUTF8));
        comboBox_dictionary_strategy->clear();
        comboBox_dictionary_strategy->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "FLANN Linear", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FLANN KdTree ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FLANN LSH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Brute Force", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Brute Force GPU", 0, QApplication::UnicodeUTF8)
        );
        label_incrementalDict->setText(QApplication::translate("preferencesDialog", "Use an incremental dictionary.\n"
"When set to false, no new words are added to dictionary, so no more updates are required after each detection, which greatly increases time performance at the cost of lower adaptation to new environments.", 0, QApplication::UnicodeUTF8));
        label_94->setText(QApplication::translate("preferencesDialog", "If the dictionary update and signature creation were parallelized.", 0, QApplication::UnicodeUTF8));
        checkBox_kp_parallelized->setText(QString());
        checkBox_kp_newWordsComparedTogether->setText(QString());
        label_186->setText(QApplication::translate("preferencesDialog", "When adding new words to dictionary, they are compared also with each other (to detect same words in the same signature).", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("preferencesDialog", "Sub pixel keypoints", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("preferencesDialog", "Refining corners to sub pixel. Only useful in RGB-D SLAM mode. Sub pixel corners may not be needed for features like SURF/SIFT, which are already sub pixel.", 0, QApplication::UnicodeUTF8));
        label_200->setText(QApplication::translate("preferencesDialog", "Window size.", 0, QApplication::UnicodeUTF8));
        label_203->setText(QApplication::translate("preferencesDialog", "Iterations. 0 disables sub pixel refining.", 0, QApplication::UnicodeUTF8));
        label_204->setText(QApplication::translate("preferencesDialog", "Epsilon.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_surf2->setTitle(QApplication::translate("preferencesDialog", "SURF", 0, QApplication::UnicodeUTF8));
        surf_checkBox_upright->setText(QString());
        label_41->setText(QApplication::translate("preferencesDialog", "Octave layers.", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("preferencesDialog", "Descriptor extended (true=128, false=64).", 0, QApplication::UnicodeUTF8));
        label_surf_checkBox_gpuKeypointsRatio->setText(QApplication::translate("preferencesDialog", "GPU keypoints ratio.", 0, QApplication::UnicodeUTF8));
        label_surf_checkBox_gpuVersion->setText(QApplication::translate("preferencesDialog", "GPU version.", 0, QApplication::UnicodeUTF8));
        surf_checkBox_gpuVersion->setText(QString());
        label_49->setText(QApplication::translate("preferencesDialog", "U-SURF used.", 0, QApplication::UnicodeUTF8));
        checkBox_surfExtended->setText(QString());
        label_26->setText(QApplication::translate("preferencesDialog", "Octaves.", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("preferencesDialog", "Hessian threshold.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_sift2->setTitle(QApplication::translate("preferencesDialog", "SIFT", 0, QApplication::UnicodeUTF8));
        label_129->setText(QApplication::translate("preferencesDialog", "Sigma.", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("preferencesDialog", "Contrast threshold.", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("preferencesDialog", "Edge threshold.", 0, QApplication::UnicodeUTF8));
        label_130->setText(QApplication::translate("preferencesDialog", "nOctaveLayers.", 0, QApplication::UnicodeUTF8));
        label_131->setText(QApplication::translate("preferencesDialog", "nFeatures.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_fast2->setTitle(QApplication::translate("preferencesDialog", "FAST", 0, QApplication::UnicodeUTF8));
        label_56->setText(QApplication::translate("preferencesDialog", "Threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.", 0, QApplication::UnicodeUTF8));
        fastSuppressNonMax->setText(QString());
        label_69->setText(QApplication::translate("preferencesDialog", "If true, non-maximum suppression is applied to detected corners (keypoints).", 0, QApplication::UnicodeUTF8));
        fastGpu->setText(QString());
        label_fastGPU->setText(QApplication::translate("preferencesDialog", "GPU-FAST: Use GPU version of FAST. This option is enabled only if OpenCV is built with CUDA and GPUs are detected.", 0, QApplication::UnicodeUTF8));
        label_fastGPUKptRatio->setText(QApplication::translate("preferencesDialog", "Used with FAST GPU.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_brief2->setTitle(QApplication::translate("preferencesDialog", "BRIEF", 0, QApplication::UnicodeUTF8));
        label_52->setText(QApplication::translate("preferencesDialog", "Bytes is a length of descriptor in bytes. It can be equal 16, 32 or 64 bytes.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_orb2->setTitle(QApplication::translate("preferencesDialog", "ORB", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("preferencesDialog", "Pyramid decimation ratio, greater than 1. scaleFactor==2 means the classical pyramid, where each next level has 4x less pixels than the previous, but such a big scale factor will degrade feature matching scores dramatically. On the other hand, too close to 1 scale factor will mean that to cover certain scale range you will need more pyramid levels and so the speed will suffer.", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("preferencesDialog", "The number of pyramid levels. The smallest level will have linear size equal to input_image_linear_size/pow(scaleFactor, nlevels).", 0, QApplication::UnicodeUTF8));
        label_95->setText(QApplication::translate("preferencesDialog", "This is size of the border where the features are not detected. It should roughly match the patchSize parameter.", 0, QApplication::UnicodeUTF8));
        label_127->setText(QApplication::translate("preferencesDialog", "It should be 0 in the current implementation.", 0, QApplication::UnicodeUTF8));
        label_128->setText(QApplication::translate("preferencesDialog", "WTA_K: The number of points that produce each element of the oriented BRIEF descriptor. The default value 2 means the BRIEF where we take a random point pair and compare their brightnesses, so we get 0/1 response. Other possible values are 3 and 4. For example, 3 means that we take 3 random points (of course, those point coordinates are random, but they are generated from the pre-defined seed, so each element of BRIEF descriptor is computed deterministically from the pixel rectangle), find point of maximum brightness and output index of the winner (0, 1 or 2). Such output will occupy 2 bits, and therefore it will need a special variant of Hamming distance, denoted as NORM_HAMMING2 (2 bits per bin). When WTA_K=4, we take 4 random points to compute each bin (that will also occupy 2 bits with possible values 0, 1, 2 or 3).", 0, QApplication::UnicodeUTF8));
        label_140->setText(QApplication::translate("preferencesDialog", "The default HARRIS_SCORE=0 means that Harris algorithm is used to rank features (the score is written to KeyPoint::score and is used to retain best nfeatures features); FAST_SCORE=1 is alternative value of the parameter that produces slightly less stable keypoints, but it is a little faster to compute.", 0, QApplication::UnicodeUTF8));
        label_147->setText(QApplication::translate("preferencesDialog", "size of the patch used by the oriented BRIEF descriptor. Of course, on smaller pyramid layers the perceived image area covered by a feature will be larger.", 0, QApplication::UnicodeUTF8));
        checkBox_ORBGpu->setText(QString());
        label_orbGpu->setText(QApplication::translate("preferencesDialog", "GPU-ORB: Use GPU version of ORB. This option is enabled only if OpenCV is built with CUDA and GPUs are detected.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_freak2->setTitle(QApplication::translate("preferencesDialog", "FREAK", 0, QApplication::UnicodeUTF8));
        checkBox_FREAKOrientationNormalized->setText(QString());
        label_6->setText(QApplication::translate("preferencesDialog", "Enable orientation normalization.", 0, QApplication::UnicodeUTF8));
        checkBox_FREAKScaleNormalized->setText(QString());
        label_9->setText(QApplication::translate("preferencesDialog", "Enable scale normalization.", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("preferencesDialog", "Scaling of the description pattern.", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("preferencesDialog", "Number of octaves covered by the detected keypoints.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_gftt2->setTitle(QApplication::translate("preferencesDialog", "GFTT", 0, QApplication::UnicodeUTF8));
        checkBox_GFTT_useHarrisDetector->setText(QString());
        label_172->setText(QApplication::translate("preferencesDialog", "Use Harris detector.", 0, QApplication::UnicodeUTF8));
        label_173->setText(QApplication::translate("preferencesDialog", "K.", 0, QApplication::UnicodeUTF8));
        label_175->setText(QApplication::translate("preferencesDialog", "Quality level.", 0, QApplication::UnicodeUTF8));
        label_176->setText(QApplication::translate("preferencesDialog", "Mininum distance.", 0, QApplication::UnicodeUTF8));
        label_177->setText(QApplication::translate("preferencesDialog", "Block size.", 0, QApplication::UnicodeUTF8));
        groupBox_detector_brisk2->setTitle(QApplication::translate("preferencesDialog", "BRISK", 0, QApplication::UnicodeUTF8));
        label_191->setText(QApplication::translate("preferencesDialog", "FAST/AGAST detection threshold score.", 0, QApplication::UnicodeUTF8));
        label_188->setText(QApplication::translate("preferencesDialog", "Detection octaves. Use 0 to do single scale.", 0, QApplication::UnicodeUTF8));
        label_189->setText(QApplication::translate("preferencesDialog", "Apply this scale to the pattern used for sampling the neighbourhood of a keypoint.", 0, QApplication::UnicodeUTF8));
        groupBox_vh_strategy1->setTitle(QApplication::translate("preferencesDialog", "Hypotheses verification", 0, QApplication::UnicodeUTF8));
        comboBox_vh_strategy->clear();
        comboBox_vh_strategy->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "No verification", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Epipolar constraints", 0, QApplication::UnicodeUTF8)
        );
        label_50->setText(QApplication::translate("preferencesDialog", "Hypotheses verification.", 0, QApplication::UnicodeUTF8));
        groupBox_vh_epipolar2->setTitle(QApplication::translate("preferencesDialog", "Epipolar constraints", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("preferencesDialog", "Minimum match count to accept a loop closure.", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("preferencesDialog", "Fundamental Matrix : Distance (pixels) for inliers.", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("preferencesDialog", "Fundamental Matrix : Ransac performance.", 0, QApplication::UnicodeUTF8));
        groupBox_slam1->setTitle(QApplication::translate("preferencesDialog", "RGB-D SLAM", 0, QApplication::UnicodeUTF8));
        general_checkBox_activateRGBD->setText(QString());
        label_activateRGBD->setText(QApplication::translate("preferencesDialog", "Activate metric RGB-D SLAM. This module enables 6d metric SLAM directly in RTAB-Map.\n"
"If set to false, classic RTAB-Map loop closure detection is done using only images and without any metric information.", 0, QApplication::UnicodeUTF8));
        label_83->setText(QApplication::translate("preferencesDialog", "Rigid transformations between nodes are saved on the neighbor links of the RTAB-Map's graph. On loop closures, a new constraint is added to the graph and TORO optimizes the graph. RGB-D images must be sent to work (see Source->RGB-D Camera).", 0, QApplication::UnicodeUTF8));
        groupBox_slam_update->setTitle(QApplication::translate("preferencesDialog", "Map update", 0, QApplication::UnicodeUTF8));
        rgdb_linearUpdate->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_153->setText(QApplication::translate("preferencesDialog", "Linear update: Minimum linear displacement to update the map. Note that Weight Update is done prior to this, so weights are still updated.", 0, QApplication::UnicodeUTF8));
        rgdb_angularUpdate->setSuffix(QApplication::translate("preferencesDialog", " rad", 0, QApplication::UnicodeUTF8));
        label_152->setText(QApplication::translate("preferencesDialog", "Angular update: Minimum angular displacement to update the map. Note that Weight Update is done prior to this, so weights are still updated.", 0, QApplication::UnicodeUTF8));
        rgdb_newMapOdomChange->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_163->setText(QApplication::translate("preferencesDialog", "Odometry change detected that triggers a new map (0 means whatever the odometry change, the detector will still link the new pose in the current map). Also by default, when an odometry with Identity transformation is detected, a new map is automatically created. ", 0, QApplication::UnicodeUTF8));
        odomScanHistory->setText(QString());
        label_scanMatching->setText(QApplication::translate("preferencesDialog", "Pose scan matching: odometry pose correction using laser scan matching. ICP 2D only is used here.", 0, QApplication::UnicodeUTF8));
        label_scanMatching_3->setText(QApplication::translate("preferencesDialog", "Maximum local locations retrieved (0=disabled) near the current pose in the local map or on the current planned path (those on the planned path have priority).", 0, QApplication::UnicodeUTF8));
        localDetection_radius->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_space2->setText(QApplication::translate("preferencesDialog", "Local radius for nodes selection in the local map. This parameter is used in some approaches below.", 0, QApplication::UnicodeUTF8));
        label_scanMatching_5->setText(QApplication::translate("preferencesDialog", "Ratio of working memory for which local nodes are immunized from transfer.", 0, QApplication::UnicodeUTF8));
        rgdb_localImmunizationRatio->setSuffix(QString());
        groupBox_graphOptimization->setTitle(QApplication::translate("preferencesDialog", "Graph optimization", 0, QApplication::UnicodeUTF8));
        graphOptimization_type->clear();
        graphOptimization_type->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "TORO", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "g2o", 0, QApplication::UnicodeUTF8)
        );
        label_31->setText(QApplication::translate("preferencesDialog", "Iterations.", 0, QApplication::UnicodeUTF8));
        graphOptimization_covarianceIgnored->setText(QString());
        label_141->setText(QApplication::translate("preferencesDialog", "Ignore constraints' variance. If checked, identity information matrix is used for each constraint. Otherwise, an information matrix is generated from the variance saved in the links (transitional and rotational variances).", 0, QApplication::UnicodeUTF8));
        graphOptimization_fromGraphEnd->setText(QString());
        label_151->setText(QApplication::translate("preferencesDialog", "Optimize graph from the newest node.", 0, QApplication::UnicodeUTF8));
        graphOptimization_slam2d->setText(QString());
        label_164->setText(QApplication::translate("preferencesDialog", "2d SLAM: use fast 3DoF (x, y, theta) optimization instead of 6DoF (x, y, z, roll, pitch, yaw) optimization.", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("preferencesDialog", "Graph optimization algorithm.", 0, QApplication::UnicodeUTF8));
        label_183->setText(QApplication::translate("preferencesDialog", "-If false, the graph is optimized from the oldest node of the current graph. It can be useful to preserve the map referential from the oldest node. An odometry correction between frames /map to /odom is computed. Warning: when some nodes are transferred, the first referential of the local map may change, resulting in momentary changes in robot/map position (which are annoying in teleoperation).", 0, QApplication::UnicodeUTF8));
        label_211->setText(QApplication::translate("preferencesDialog", "-If true, there is no odometry correction computed. All previous poses in the map are corrected instead, not the last one (which corresponds to latest odometry value). So, the transform between frames /map to /odom will be always Identity even on loop closures.", 0, QApplication::UnicodeUTF8));
        groupBox_localDetection_time->setTitle(QApplication::translate("preferencesDialog", "Local loop closure detection in time", 0, QApplication::UnicodeUTF8));
        label_54->setText(QApplication::translate("preferencesDialog", "Activate local detection over all locations in STM. The Bayes filter is not used here: If there are enough correspondences between the current image and others in STM, transformations are computed. This generates more constraints in the map's graph, so more time is required to optimize the graph.", 0, QApplication::UnicodeUTF8));
        groupBox_localDetection_space->setTitle(QApplication::translate("preferencesDialog", "Local loop closure detection in space", 0, QApplication::UnicodeUTF8));
        label_space1_2->setText(QApplication::translate("preferencesDialog", "Activate local detection over locations (in Working Memory) near in space. The locations are compared to those inside the local radius of the current estimated position of the robot (at a maximum distance of the path filtering radius below). If the laser scans are used, local locations are merged together and the current laser scan is compared to them. It is useful when the robot is visiting backward (camera not in the same direction) already visited locations.", 0, QApplication::UnicodeUTF8));
        localDetection_pathFilteringRadius->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_space3_2->setText(QApplication::translate("preferencesDialog", "Maximum graph depth between the current/last loop closure location and the local loop closure hypotheses. Set 0 to ignore.", 0, QApplication::UnicodeUTF8));
        checkBox_localSpaceLinksKeptInWM->setText(QString());
        label_scanMatching_2->setText(QApplication::translate("preferencesDialog", "If local space links are kept in WM.", 0, QApplication::UnicodeUTF8));
        label_space3_3->setText(QApplication::translate("preferencesDialog", "Path filtering radius to avoid merging laser scans which are close. 0 to ignore.", 0, QApplication::UnicodeUTF8));
        label_scanMatching_4->setText(QApplication::translate("preferencesDialog", "When comparing to a local path, merge the laser scans using the odometry poses instead of the ones in the optimized local graph.", 0, QApplication::UnicodeUTF8));
        checkBox_localSpacePathOdomPosesUsed->setText(QString());
        groupBox_12->setTitle(QApplication::translate("preferencesDialog", "Graph planning", 0, QApplication::UnicodeUTF8));
        label_space2_3->setText(QApplication::translate("preferencesDialog", " If the goal is too far (>local radius) from the graph, the plan is aborted. The next goal in the graph can't be farther than the local radius around the current position. Anticipated nodes on the path are retrieved in the local radius up to \"Maximum local locations retrieved\". A virtual link is added between the current location and the nearest one on the path up to local radius.", 0, QApplication::UnicodeUTF8));
        graphPlan_goalReachedRadius->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_space2_2->setText(QApplication::translate("preferencesDialog", "Goal reached radius.", 0, QApplication::UnicodeUTF8));
        graphPlan_planWithNearNodesLinked->setText(QString());
        label_space3_4->setText(QApplication::translate("preferencesDialog", "Add virtual links. Before planning in the graph, near nodes are linked together. The maximum distance is defined by \"Goal reached radius\" above. If \"Maximum ID difference\" below is set, only close nodes in time can be linked together.", 0, QApplication::UnicodeUTF8));
        label_space3_5->setText(QApplication::translate("preferencesDialog", "When a goal is received and processed with success, it is saved in user data of the location with this format: \"GOAL:#\".", 0, QApplication::UnicodeUTF8));
        graphPlan_goalsSavedInUserData->setText(QString());
        groupBox_visualTransform2->setTitle(QApplication::translate("preferencesDialog", "Loop closure constraint", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("preferencesDialog", "When a global loop closure is detected by the Bayes filter, a transform is computed between the current location and the old one. If a transform cannot be computed, the loop closure is rejected. A transformation is computed using the 3D visual word correspondences.", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("preferencesDialog", "Minimum visual word correspondences to compute geometry transform.", 0, QApplication::UnicodeUTF8));
        loopClosure_bowInlierDistance->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("preferencesDialog", "Maximum distance for visual word correspondences.", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("preferencesDialog", "Maximum iterations to compute the transform from visual words.", 0, QApplication::UnicodeUTF8));
        loopClosure_bowMaxDepth->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_146->setText(QApplication::translate("preferencesDialog", "Max feature depth. Note that parameter \"Visual Word\"->\"Max words depth\" is applied before this.", 0, QApplication::UnicodeUTF8));
        loopClosure_bowForce2D->setText(QString());
        label_184->setText(QApplication::translate("preferencesDialog", "Force 2D transform (3DoF: x,y and yaw).", 0, QApplication::UnicodeUTF8));
        globalDetection_icpType->clear();
        globalDetection_icpType->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "No ICP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ICP 3D (e.g. Kinect depth)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ICP 2D (e.g. Laser scan)", 0, QApplication::UnicodeUTF8)
        );
        label_57->setText(QApplication::translate("preferencesDialog", "When enabled, the visual transform is used as a guess for ICP estimation (3D or 2D). See \"ICP\" panel for parameters.", 0, QApplication::UnicodeUTF8));
        label_658->setText(QApplication::translate("preferencesDialog", "Use epipolar geometry to compute the loop closure transform. \"Maximum distance for visual word correspondences\" is not used in this mode.", 0, QApplication::UnicodeUTF8));
        loopClosure_bowEpipolarGeometry->setText(QString());
        label_664->setText(QApplication::translate("preferencesDialog", "Epipolar geometry maximum variance to accept the loop closure.", 0, QApplication::UnicodeUTF8));
        loopClosure_bowEpipolarGeometryVar->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        groupBox_reextract->setTitle(QApplication::translate("preferencesDialog", "Re-extract features on global loop closure", 0, QApplication::UnicodeUTF8));
        label_182->setText(QApplication::translate("preferencesDialog", "By activating the re-extraction of the features, the features from the two images will be re-extracted using the settings below, and matched directly instead of using the big vocabulary. This adds an overhead processing time but it will mostly produce more corresponding words between the images, so better transformation computed. We recommend to use binary features for fast extraction and matching.", 0, QApplication::UnicodeUTF8));
        label_171->setText(QApplication::translate("preferencesDialog", "If not activated, when a loop closure constraint must be computed, the words already extracted for the loop closure detector are used. These words are limited (see Visual Word->Words Per Image) and matched to the loop closure detection vocabulary. When the vocabulary is large, there maybe less corresponding words on a loop closure.", 0, QApplication::UnicodeUTF8));
        label_174->setText(QApplication::translate("preferencesDialog", "Epipolar geometry is ignored (if set above) by this option.", 0, QApplication::UnicodeUTF8));
        reextract_nn->clear();
        reextract_nn->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "FLANN Linear", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FLANN KdTree", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FLANN LSH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Brute Force", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Brute Force GPU", 0, QApplication::UnicodeUTF8)
        );
        label_180->setText(QApplication::translate("preferencesDialog", "Max features extracted from the images (0 means inf).", 0, QApplication::UnicodeUTF8));
        label_179->setText(QApplication::translate("preferencesDialog", "Nearest neighbor strategy. FLANN KdTree must be used only with SURF/SIFT. FLANN LSH must be used only with binary feature detector.", 0, QApplication::UnicodeUTF8));
        label_181->setText(QApplication::translate("preferencesDialog", "NNDR ratio\n"
"(A matching pair is accepted, if its distance is closer than X times the distance of the second nearest neighbor)\n"
"Lower the ratio -> higher the precision. 0 means disabled, matching the nearest.", 0, QApplication::UnicodeUTF8));
        reextract_type->clear();
        reextract_type->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "SURF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "SIFT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ORB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FAST+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FAST+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "GFTT+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "GFTT+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "BRISK", 0, QApplication::UnicodeUTF8)
        );
        label_178->setText(QApplication::translate("preferencesDialog", "Feature detector ", 0, QApplication::UnicodeUTF8));
        groupBox_icp2->setTitle(QApplication::translate("preferencesDialog", "ICP", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("preferencesDialog", "Iterative closest point (ICP) parameters.", 0, QApplication::UnicodeUTF8));
        globalDetection_icpMaxTranslation->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_51->setText(QApplication::translate("preferencesDialog", "Maximum ICP translation correction accepted (>0). A large translation difference between the visual transformation and ICP transformation results in wrong transformations in most cases.", 0, QApplication::UnicodeUTF8));
        label_55->setText(QApplication::translate("preferencesDialog", "Maximum ICP rotation correction accepted (>0). A large rotation difference between the visual transformation and ICP transformation results in wrong transformations in most cases.", 0, QApplication::UnicodeUTF8));
        globalDetection_icpMaxRotation->setSuffix(QApplication::translate("preferencesDialog", " rad", 0, QApplication::UnicodeUTF8));
        groupBox_loopClosure_icp3->setTitle(QApplication::translate("preferencesDialog", "ICP 3D (e.g. Kinect depth)", 0, QApplication::UnicodeUTF8));
        label_124->setText(QApplication::translate("preferencesDialog", "Max iterations.", 0, QApplication::UnicodeUTF8));
        label_109->setText(QApplication::translate("preferencesDialog", "Depth image decimation.", 0, QApplication::UnicodeUTF8));
        loopClosure_icpMaxDepth->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_115->setText(QApplication::translate("preferencesDialog", "Max cloud depth.", 0, QApplication::UnicodeUTF8));
        loopClosure_icpVoxelSize->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_125->setText(QApplication::translate("preferencesDialog", "Voxel size to be used for ICP computation. Set to 0 to disable voxel filtering.", 0, QApplication::UnicodeUTF8));
        label_118->setText(QApplication::translate("preferencesDialog", "Random samples to be used for ICP computation. Not used if Voxel size is null.", 0, QApplication::UnicodeUTF8));
        label_122->setText(QApplication::translate("preferencesDialog", "Max distance for point correspondences.", 0, QApplication::UnicodeUTF8));
        label_133->setText(QApplication::translate("preferencesDialog", "Ratio of matching correspondences to accept the transform.", 0, QApplication::UnicodeUTF8));
        loopClosure_icpPointToPlane->setText(QString());
        label_144->setText(QApplication::translate("preferencesDialog", "Point to plane ICP.", 0, QApplication::UnicodeUTF8));
        label_212->setText(QApplication::translate("preferencesDialog", "Number of neighbors to compute normals for point to plane.", 0, QApplication::UnicodeUTF8));
        groupBox_loopClosure_icp2->setTitle(QApplication::translate("preferencesDialog", "ICP 2D (e.g. Laser scan)", 0, QApplication::UnicodeUTF8));
        label_138->setText(QApplication::translate("preferencesDialog", "Max distance for point correspondences.", 0, QApplication::UnicodeUTF8));
        label_139->setText(QApplication::translate("preferencesDialog", "Max iterations.", 0, QApplication::UnicodeUTF8));
        label_148->setText(QApplication::translate("preferencesDialog", "Minimum ratio of correspondences on laser scan maximum size to accept the transform.", 0, QApplication::UnicodeUTF8));
        loopClosure_icp2Voxel->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_150->setText(QApplication::translate("preferencesDialog", "Voxel size. Set to 0 to disable voxel filtering.", 0, QApplication::UnicodeUTF8));
        groupBox_stereo2->setTitle(QApplication::translate("preferencesDialog", "Stereo", 0, QApplication::UnicodeUTF8));
        label_187->setText(QApplication::translate("preferencesDialog", "These parameters are used to find the 3D positions of visual words using disparity between the left and right images. Optical flow is used to find corresponding corners in the right image of words extracted from the left image.", 0, QApplication::UnicodeUTF8));
        groupBox_10->setTitle(QApplication::translate("preferencesDialog", "calcOpticalFlowPyrLK()", 0, QApplication::UnicodeUTF8));
        label_205->setText(QApplication::translate("preferencesDialog", "Window size.", 0, QApplication::UnicodeUTF8));
        label_206->setText(QApplication::translate("preferencesDialog", "Iterations.", 0, QApplication::UnicodeUTF8));
        label_207->setText(QApplication::translate("preferencesDialog", "Epsilon.", 0, QApplication::UnicodeUTF8));
        label_208->setText(QApplication::translate("preferencesDialog", "Max level.", 0, QApplication::UnicodeUTF8));
        label_215->setText(QApplication::translate("preferencesDialog", "The maximum slope for each stereo pairs. This will filter bad matches from calcOpticalFlowPyrLK().", 0, QApplication::UnicodeUTF8));
        groupBox_odometry1->setTitle(QApplication::translate("preferencesDialog", "Odometry", 0, QApplication::UnicodeUTF8));
        label_114->setText(QApplication::translate("preferencesDialog", "Odometry is computed as fast as possible up to \"Source->Input rate\" parameter. Odometry is not computed if already provided by the source.", 0, QApplication::UnicodeUTF8));
        label_160->setText(QApplication::translate("preferencesDialog", "3-Mono is for single camera motion estimation (MonoSLAM). On initialization, the camera must be translated on the side until a first transform can be computed.", 0, QApplication::UnicodeUTF8));
        label_47->setText(QApplication::translate("preferencesDialog", "Automatically reset odometry after X consecutive images on which odometry cannot be computed (value=0 disables auto-reset). When reset, the odometry starts from the last pose computed.", 0, QApplication::UnicodeUTF8));
        odom_strategy->clear();
        odom_strategy->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "BOW (bag-of-words)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Optical Flow", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Mono", 0, QApplication::UnicodeUTF8)
        );
        odom_fillInfoData->setText(QString());
        odom_type->clear();
        odom_type->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "SURF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "SIFT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "ORB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FAST+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FAST+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "GFTT+FREAK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "GFTT+BRIEF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "BRISK", 0, QApplication::UnicodeUTF8)
        );
        label_44->setText(QApplication::translate("preferencesDialog", "Feature detector. In BOW/Mono modes, the related descriptor is also used. In Optical flow mode, only the keypoint detector is used.", 0, QApplication::UnicodeUTF8));
        label_103->setText(QApplication::translate("preferencesDialog", "Odometry strategy:", 0, QApplication::UnicodeUTF8));
        odom_force2D->setText(QString());
        label_196->setText(QApplication::translate("preferencesDialog", "Force 2D transform (3DoF: x,y and yaw).", 0, QApplication::UnicodeUTF8));
        label_221->setText(QApplication::translate("preferencesDialog", "Fill info with data (inliers/outliers features to be shown in Odometry view).", 0, QApplication::UnicodeUTF8));
        pushButton_testOdometry->setText(QApplication::translate("preferencesDialog", "Test selected odometry", 0, QApplication::UnicodeUTF8));
        label_161->setText(QApplication::translate("preferencesDialog", "2-Optical flow estimate the location of 2D features from last frame to new frame, then computes RANSAC transformation with corresponding 3D features.", 0, QApplication::UnicodeUTF8));
        label_162->setText(QApplication::translate("preferencesDialog", "1-BOW matches features extracted from both frames using nearest neighbor with descriptors, then computes RANSAC transformation estimation with corresponding 3D features.", 0, QApplication::UnicodeUTF8));
        groupBox_11->setTitle(QApplication::translate("preferencesDialog", "Transformation estimation (RANSAC)", 0, QApplication::UnicodeUTF8));
        label_145->setText(QApplication::translate("preferencesDialog", "Minimum feature correspondences to compute geometry transform.", 0, QApplication::UnicodeUTF8));
        odom_inlierDistance->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_149->setText(QApplication::translate("preferencesDialog", "RANSAC: Maximum distance for 3D feature correspondences. Lower the value, higher the precision but higher the chance of RED screens (odometry lost).", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("preferencesDialog", "RANSAC: Maximum iterations to compute the transform from 3D features.", 0, QApplication::UnicodeUTF8));
        label_111->setText(QApplication::translate("preferencesDialog", "Refine iterations of the resulting transformation computed by RANSAC. 0 means no refining.", 0, QApplication::UnicodeUTF8));
        label_225->setText(QApplication::translate("preferencesDialog", "PnP reprojection error.", 0, QApplication::UnicodeUTF8));
        odom_pnpReprojError->setSuffix(QApplication::translate("preferencesDialog", " pix", 0, QApplication::UnicodeUTF8));
        odom_pnpEstimation->setText(QString());
        label_226->setText(QApplication::translate("preferencesDialog", "PnP RANSAC: Pose estimation from 2D to 3D correspondences instead of 3D to 3D correspondences. PnP uses \"Minimum feature correspondences\" and \"Maximum iterations\" above.", 0, QApplication::UnicodeUTF8));
        label_227->setText(QApplication::translate("preferencesDialog", "PnP flags.", 0, QApplication::UnicodeUTF8));
        odom_pnpFlags->clear();
        odom_pnpFlags->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "Iterative", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "EPNP", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "P3P", 0, QApplication::UnicodeUTF8)
        );
        groupBox_8->setTitle(QApplication::translate("preferencesDialog", "Features filtering", 0, QApplication::UnicodeUTF8));
        label_123->setText(QApplication::translate("preferencesDialog", "ROI ratios [left, right, top, bottom] between 0 and 1.", 0, QApplication::UnicodeUTF8));
        lineEdit_odom_roi->setText(QApplication::translate("preferencesDialog", "0.0 0.0 0.0 0.0", 0, QApplication::UnicodeUTF8));
        label_136->setText(QApplication::translate("preferencesDialog", "Max features extracted from the images (0 means inf).", 0, QApplication::UnicodeUTF8));
        label_104->setText(QApplication::translate("preferencesDialog", "Maximum feature depth.", 0, QApplication::UnicodeUTF8));
        odom_maxDepth->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("preferencesDialog", "Sub pixel corners", 0, QApplication::UnicodeUTF8));
        label_216->setText(QApplication::translate("preferencesDialog", "Refining corners to sub pixel. Sub pixel corners may not be needed for features like SURF/SIFT, which are already sub pixel.", 0, QApplication::UnicodeUTF8));
        label_197->setText(QApplication::translate("preferencesDialog", "Window size.", 0, QApplication::UnicodeUTF8));
        label_198->setText(QApplication::translate("preferencesDialog", "Iterations. 0 disables sub pixel refining.", 0, QApplication::UnicodeUTF8));
        label_214->setText(QApplication::translate("preferencesDialog", "Epsilon.", 0, QApplication::UnicodeUTF8));
        groupBox_odometryBOW2->setTitle(QApplication::translate("preferencesDialog", "BOW", 0, QApplication::UnicodeUTF8));
        label_190->setText(QApplication::translate("preferencesDialog", "Local history size: If > 0 (example 5000), the odometry will maintain a local map of X maximum words. This will decrease odometry drifting when the camera is not moving.", 0, QApplication::UnicodeUTF8));
        odom_bin_nn->clear();
        odom_bin_nn->insertItems(0, QStringList()
         << QApplication::translate("preferencesDialog", "FLANN Linear", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FLANN KdTree", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "FLANN LSH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Brute Force", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesDialog", "Brute Force GPU", 0, QApplication::UnicodeUTF8)
        );
        label_201->setText(QApplication::translate("preferencesDialog", "Nearest neighbor strategy. FLANN KdTree must be used only with SURF/SIFT. FLANN LSH must be used only with binary feature detector.", 0, QApplication::UnicodeUTF8));
        label_202->setText(QApplication::translate("preferencesDialog", "NNDR ratio\n"
"(A matching pair is accepted, if its distance is closer than X times the distance of the second nearest neighbor)\n"
"Lower the ratio -> higher the precision. 0 means disabled, matching the nearest.", 0, QApplication::UnicodeUTF8));
        groupBox_odometryFlow2->setTitle(QApplication::translate("preferencesDialog", "Optical Flow", 0, QApplication::UnicodeUTF8));
        label_199->setText(QApplication::translate("preferencesDialog", "The process is as follow:\n"
"  - Features from the last frame are estimated in the new frame using an optical flow approach (see cv::calcOpticalFlowPyrLK()).\n"
"  - 3D features from the new frame are extracted from the estimated positions.\n"
"  - Using RANSAC, a transformation is estimated between corresponding 3D features.\n"
"  - New features are extracted from the new frame to be used for the next time.\n"
"  - Optionally, the 2D position of the features can be refined for sub pixel precision (see cv::cornerSubPix()). ", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("preferencesDialog", "calcOpticalFlowPyrLK()", 0, QApplication::UnicodeUTF8));
        label_192->setText(QApplication::translate("preferencesDialog", "Window size.", 0, QApplication::UnicodeUTF8));
        label_193->setText(QApplication::translate("preferencesDialog", "Iterations.", 0, QApplication::UnicodeUTF8));
        label_194->setText(QApplication::translate("preferencesDialog", "Epsilon.", 0, QApplication::UnicodeUTF8));
        label_195->setText(QApplication::translate("preferencesDialog", "Max level.", 0, QApplication::UnicodeUTF8));
        groupBox_odometryMono2->setTitle(QApplication::translate("preferencesDialog", "Mono", 0, QApplication::UnicodeUTF8));
        label_661->setText(QApplication::translate("preferencesDialog", "Parameters from BOW and OpticalFlow are also used here. PnP parameters on Odometry panel are used too.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_minFlow->setSuffix(QApplication::translate("preferencesDialog", " pixels", 0, QApplication::UnicodeUTF8));
        label_659->setText(QApplication::translate("preferencesDialog", "Minimum optical flow required for the initialization step.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_minTranslation->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_660->setText(QApplication::translate("preferencesDialog", "Minimum translation to add new points to local map. On initialization, translation x 5 is used as the required minimum transformation.", 0, QApplication::UnicodeUTF8));
        label_662->setText(QApplication::translate("preferencesDialog", "Minimum translation required for the initialization step.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_minInitTranslation->setSuffix(QApplication::translate("preferencesDialog", " m", 0, QApplication::UnicodeUTF8));
        label_663->setText(QApplication::translate("preferencesDialog", "Maximum variance to add new points to local map.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_maxVariance->setSuffix(QString());
        radioButton_basic->setText(QApplication::translate("preferencesDialog", "Basic", 0, QApplication::UnicodeUTF8));
        radioButton_advanced->setText(QApplication::translate("preferencesDialog", "Advanced", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class preferencesDialog: public Ui_preferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
