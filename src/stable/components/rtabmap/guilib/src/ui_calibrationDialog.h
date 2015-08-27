/********************************************************************************
** Form generated from reading UI file 'calibrationDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATIONDIALOG_H
#define UI_CALIBRATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include "utilite/UImageView.h"

QT_BEGIN_NAMESPACE

class Ui_calibrationDialog
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_left;
    UImageView *image_view;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_right;
    UImageView *image_view_2;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_raw;
    QRadioButton *radioButton_rectified;
    QRadioButton *radioButton_stereoRectified;
    QFrame *line;
    QCheckBox *checkBox_mirror;
    QCheckBox *checkBox_showHorizontalLines;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QSpinBox *spinBox_boardHeight;
    QLabel *label_15;
    QLabel *label_12;
    QLabel *label_16;
    QSpinBox *spinBox_boardWidth;
    QDoubleSpinBox *doubleSpinBox_squareSize;
    QLabel *label_21;
    QLabel *label_20;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_4;
    QProgressBar *progressBar_skew;
    QProgressBar *progressBar_size;
    QLabel *label_2;
    QProgressBar *progressBar_count;
    QProgressBar *progressBar_x;
    QProgressBar *progressBar_y;
    QLabel *label_3;
    QLabel *label_5;
    QProgressBar *progressBar_x_2;
    QProgressBar *progressBar_y_2;
    QProgressBar *progressBar_size_2;
    QProgressBar *progressBar_skew_2;
    QLabel *label_19;
    QLabel *label_serial;
    QProgressBar *progressBar_count_2;
    QCheckBox *checkBox_switchImages;
    QPushButton *pushButton_calibrate;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_fx;
    QLabel *label_fy;
    QLineEdit *lineEdit_K;
    QLabel *label_cx;
    QLabel *label_11;
    QLabel *label_cy;
    QLabel *label_6;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_8;
    QLabel *label_17;
    QLineEdit *lineEdit_D_2;
    QLabel *label_7;
    QLineEdit *lineEdit_D;
    QLabel *label_14;
    QLabel *label_error;
    QLabel *label_fx_2;
    QLabel *label_cx_2;
    QLabel *label_fy_2;
    QLabel *label_cy_2;
    QLineEdit *lineEdit_K_2;
    QLabel *label_18;
    QLineEdit *lineEdit_R;
    QLineEdit *lineEdit_P;
    QLineEdit *lineEdit_R_2;
    QLineEdit *lineEdit_P_2;
    QLabel *label_baseline_name;
    QLabel *label_baseline;
    QLabel *label_error_2;
    QPushButton *pushButton_save;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_restart;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *calibrationDialog)
    {
        if (calibrationDialog->objectName().isEmpty())
            calibrationDialog->setObjectName(QString::fromUtf8("calibrationDialog"));
        calibrationDialog->resize(1314, 795);
        verticalLayout_5 = new QVBoxLayout(calibrationDialog);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_left = new QLabel(calibrationDialog);
        label_left->setObjectName(QString::fromUtf8("label_left"));
        label_left->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_left);

        image_view = new UImageView(calibrationDialog);
        image_view->setObjectName(QString::fromUtf8("image_view"));

        verticalLayout_2->addWidget(image_view);

        verticalLayout_2->setStretch(1, 1);

        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_right = new QLabel(calibrationDialog);
        label_right->setObjectName(QString::fromUtf8("label_right"));
        label_right->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_right);

        image_view_2 = new UImageView(calibrationDialog);
        image_view_2->setObjectName(QString::fromUtf8("image_view_2"));

        verticalLayout_3->addWidget(image_view_2);

        verticalLayout_3->setStretch(1, 1);

        horizontalLayout_3->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButton_raw = new QRadioButton(calibrationDialog);
        radioButton_raw->setObjectName(QString::fromUtf8("radioButton_raw"));

        horizontalLayout_2->addWidget(radioButton_raw);

        radioButton_rectified = new QRadioButton(calibrationDialog);
        radioButton_rectified->setObjectName(QString::fromUtf8("radioButton_rectified"));

        horizontalLayout_2->addWidget(radioButton_rectified);

        radioButton_stereoRectified = new QRadioButton(calibrationDialog);
        radioButton_stereoRectified->setObjectName(QString::fromUtf8("radioButton_stereoRectified"));

        horizontalLayout_2->addWidget(radioButton_stereoRectified);

        line = new QFrame(calibrationDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(line);

        checkBox_mirror = new QCheckBox(calibrationDialog);
        checkBox_mirror->setObjectName(QString::fromUtf8("checkBox_mirror"));

        horizontalLayout_2->addWidget(checkBox_mirror);

        checkBox_showHorizontalLines = new QCheckBox(calibrationDialog);
        checkBox_showHorizontalLines->setObjectName(QString::fromUtf8("checkBox_showHorizontalLines"));

        horizontalLayout_2->addWidget(checkBox_showHorizontalLines);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_4->addLayout(horizontalLayout_2);


        horizontalLayout_4->addLayout(verticalLayout_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(calibrationDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        spinBox_boardHeight = new QSpinBox(groupBox);
        spinBox_boardHeight->setObjectName(QString::fromUtf8("spinBox_boardHeight"));
        spinBox_boardHeight->setMinimum(2);
        spinBox_boardHeight->setValue(6);

        gridLayout_3->addWidget(spinBox_boardHeight, 1, 3, 1, 1);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 3, 0, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_3->addWidget(label_12, 1, 0, 1, 1);

        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_3->addWidget(label_16, 3, 3, 1, 1);

        spinBox_boardWidth = new QSpinBox(groupBox);
        spinBox_boardWidth->setObjectName(QString::fromUtf8("spinBox_boardWidth"));
        spinBox_boardWidth->setMinimum(2);
        spinBox_boardWidth->setValue(8);

        gridLayout_3->addWidget(spinBox_boardWidth, 1, 1, 1, 1);

        doubleSpinBox_squareSize = new QDoubleSpinBox(groupBox);
        doubleSpinBox_squareSize->setObjectName(QString::fromUtf8("doubleSpinBox_squareSize"));
        doubleSpinBox_squareSize->setDecimals(4);
        doubleSpinBox_squareSize->setMaximum(1);
        doubleSpinBox_squareSize->setValue(0.033);

        gridLayout_3->addWidget(doubleSpinBox_squareSize, 3, 1, 1, 1);

        label_21 = new QLabel(groupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_3->addWidget(label_21, 0, 0, 1, 1);

        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setOpenExternalLinks(true);

        gridLayout_3->addWidget(label_20, 0, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(calibrationDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 5, 0, 1, 1);

        progressBar_skew = new QProgressBar(groupBox_2);
        progressBar_skew->setObjectName(QString::fromUtf8("progressBar_skew"));
        progressBar_skew->setValue(24);

        gridLayout->addWidget(progressBar_skew, 5, 1, 1, 1);

        progressBar_size = new QProgressBar(groupBox_2);
        progressBar_size->setObjectName(QString::fromUtf8("progressBar_size"));
        progressBar_size->setValue(24);

        gridLayout->addWidget(progressBar_size, 4, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        progressBar_count = new QProgressBar(groupBox_2);
        progressBar_count->setObjectName(QString::fromUtf8("progressBar_count"));
        progressBar_count->setValue(24);

        gridLayout->addWidget(progressBar_count, 1, 1, 1, 1);

        progressBar_x = new QProgressBar(groupBox_2);
        progressBar_x->setObjectName(QString::fromUtf8("progressBar_x"));
        progressBar_x->setValue(24);

        gridLayout->addWidget(progressBar_x, 2, 1, 1, 1);

        progressBar_y = new QProgressBar(groupBox_2);
        progressBar_y->setObjectName(QString::fromUtf8("progressBar_y"));
        progressBar_y->setValue(24);

        gridLayout->addWidget(progressBar_y, 3, 1, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        progressBar_x_2 = new QProgressBar(groupBox_2);
        progressBar_x_2->setObjectName(QString::fromUtf8("progressBar_x_2"));
        progressBar_x_2->setValue(24);
        progressBar_x_2->setInvertedAppearance(true);
        progressBar_x_2->setTextDirection(QProgressBar::TopToBottom);

        gridLayout->addWidget(progressBar_x_2, 2, 2, 1, 1);

        progressBar_y_2 = new QProgressBar(groupBox_2);
        progressBar_y_2->setObjectName(QString::fromUtf8("progressBar_y_2"));
        progressBar_y_2->setValue(24);
        progressBar_y_2->setInvertedAppearance(true);

        gridLayout->addWidget(progressBar_y_2, 3, 2, 1, 1);

        progressBar_size_2 = new QProgressBar(groupBox_2);
        progressBar_size_2->setObjectName(QString::fromUtf8("progressBar_size_2"));
        progressBar_size_2->setValue(24);
        progressBar_size_2->setInvertedAppearance(true);

        gridLayout->addWidget(progressBar_size_2, 4, 2, 1, 1);

        progressBar_skew_2 = new QProgressBar(groupBox_2);
        progressBar_skew_2->setObjectName(QString::fromUtf8("progressBar_skew_2"));
        progressBar_skew_2->setValue(24);
        progressBar_skew_2->setInvertedAppearance(true);

        gridLayout->addWidget(progressBar_skew_2, 5, 2, 1, 1);

        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout->addWidget(label_19, 0, 0, 1, 1);

        label_serial = new QLabel(groupBox_2);
        label_serial->setObjectName(QString::fromUtf8("label_serial"));
        label_serial->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(label_serial, 0, 1, 1, 1);

        progressBar_count_2 = new QProgressBar(groupBox_2);
        progressBar_count_2->setObjectName(QString::fromUtf8("progressBar_count_2"));
        progressBar_count_2->setValue(24);
        progressBar_count_2->setInvertedAppearance(true);

        gridLayout->addWidget(progressBar_count_2, 1, 2, 1, 1);

        checkBox_switchImages = new QCheckBox(groupBox_2);
        checkBox_switchImages->setObjectName(QString::fromUtf8("checkBox_switchImages"));

        gridLayout->addWidget(checkBox_switchImages, 0, 2, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        pushButton_calibrate = new QPushButton(calibrationDialog);
        pushButton_calibrate->setObjectName(QString::fromUtf8("pushButton_calibrate"));

        verticalLayout->addWidget(pushButton_calibrate);

        groupBox_3 = new QGroupBox(calibrationDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_fx = new QLabel(groupBox_3);
        label_fx->setObjectName(QString::fromUtf8("label_fx"));
        label_fx->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_fx, 0, 1, 1, 1);

        label_fy = new QLabel(groupBox_3);
        label_fy->setObjectName(QString::fromUtf8("label_fy"));
        label_fy->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_fy, 1, 1, 1, 1);

        lineEdit_K = new QLineEdit(groupBox_3);
        lineEdit_K->setObjectName(QString::fromUtf8("lineEdit_K"));
        lineEdit_K->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_K, 5, 1, 1, 1);

        label_cx = new QLabel(groupBox_3);
        label_cx->setObjectName(QString::fromUtf8("label_cx"));
        label_cx->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_cx, 2, 1, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 3, 0, 1, 1);

        label_cy = new QLabel(groupBox_3);
        label_cy->setObjectName(QString::fromUtf8("label_cy"));
        label_cy->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_cy, 3, 1, 1, 1);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 5, 0, 1, 1);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 1, 0, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 2, 0, 1, 1);

        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);

        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_2->addWidget(label_17, 7, 0, 1, 1);

        lineEdit_D_2 = new QLineEdit(groupBox_3);
        lineEdit_D_2->setObjectName(QString::fromUtf8("lineEdit_D_2"));
        lineEdit_D_2->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_D_2, 6, 2, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 6, 0, 1, 1);

        lineEdit_D = new QLineEdit(groupBox_3);
        lineEdit_D->setObjectName(QString::fromUtf8("lineEdit_D"));
        lineEdit_D->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_D, 6, 1, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 9, 0, 1, 1);

        label_error = new QLabel(groupBox_3);
        label_error->setObjectName(QString::fromUtf8("label_error"));

        gridLayout_2->addWidget(label_error, 9, 1, 1, 1);

        label_fx_2 = new QLabel(groupBox_3);
        label_fx_2->setObjectName(QString::fromUtf8("label_fx_2"));
        label_fx_2->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_fx_2, 0, 2, 1, 1);

        label_cx_2 = new QLabel(groupBox_3);
        label_cx_2->setObjectName(QString::fromUtf8("label_cx_2"));
        label_cx_2->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_cx_2, 2, 2, 1, 1);

        label_fy_2 = new QLabel(groupBox_3);
        label_fy_2->setObjectName(QString::fromUtf8("label_fy_2"));
        label_fy_2->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_fy_2, 1, 2, 1, 1);

        label_cy_2 = new QLabel(groupBox_3);
        label_cy_2->setObjectName(QString::fromUtf8("label_cy_2"));
        label_cy_2->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_cy_2, 3, 2, 1, 1);

        lineEdit_K_2 = new QLineEdit(groupBox_3);
        lineEdit_K_2->setObjectName(QString::fromUtf8("lineEdit_K_2"));
        lineEdit_K_2->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_K_2, 5, 2, 1, 1);

        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_2->addWidget(label_18, 8, 0, 1, 1);

        lineEdit_R = new QLineEdit(groupBox_3);
        lineEdit_R->setObjectName(QString::fromUtf8("lineEdit_R"));
        lineEdit_R->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_R, 7, 1, 1, 1);

        lineEdit_P = new QLineEdit(groupBox_3);
        lineEdit_P->setObjectName(QString::fromUtf8("lineEdit_P"));
        lineEdit_P->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_P, 8, 1, 1, 1);

        lineEdit_R_2 = new QLineEdit(groupBox_3);
        lineEdit_R_2->setObjectName(QString::fromUtf8("lineEdit_R_2"));
        lineEdit_R_2->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_R_2, 7, 2, 1, 1);

        lineEdit_P_2 = new QLineEdit(groupBox_3);
        lineEdit_P_2->setObjectName(QString::fromUtf8("lineEdit_P_2"));
        lineEdit_P_2->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_P_2, 8, 2, 1, 1);

        label_baseline_name = new QLabel(groupBox_3);
        label_baseline_name->setObjectName(QString::fromUtf8("label_baseline_name"));

        gridLayout_2->addWidget(label_baseline_name, 4, 0, 1, 1);

        label_baseline = new QLabel(groupBox_3);
        label_baseline->setObjectName(QString::fromUtf8("label_baseline"));
        label_baseline->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(label_baseline, 4, 2, 1, 1);

        label_error_2 = new QLabel(groupBox_3);
        label_error_2->setObjectName(QString::fromUtf8("label_error_2"));

        gridLayout_2->addWidget(label_error_2, 9, 2, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        pushButton_save = new QPushButton(calibrationDialog);
        pushButton_save->setObjectName(QString::fromUtf8("pushButton_save"));

        verticalLayout->addWidget(pushButton_save);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_restart = new QPushButton(calibrationDialog);
        pushButton_restart->setObjectName(QString::fromUtf8("pushButton_restart"));

        horizontalLayout->addWidget(pushButton_restart);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_4->addLayout(verticalLayout);

        horizontalLayout_4->setStretch(0, 1);

        verticalLayout_5->addLayout(horizontalLayout_4);

        buttonBox = new QDialogButtonBox(calibrationDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout_5->addWidget(buttonBox);


        retranslateUi(calibrationDialog);

        QMetaObject::connectSlotsByName(calibrationDialog);
    } // setupUi

    void retranslateUi(QDialog *calibrationDialog)
    {
        calibrationDialog->setWindowTitle(QApplication::translate("calibrationDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_left->setText(QString());
        label_right->setText(QString());
        radioButton_raw->setText(QApplication::translate("calibrationDialog", "Raw", 0, QApplication::UnicodeUTF8));
        radioButton_rectified->setText(QApplication::translate("calibrationDialog", "Rectified", 0, QApplication::UnicodeUTF8));
        radioButton_stereoRectified->setText(QApplication::translate("calibrationDialog", "Stereo Rectified", 0, QApplication::UnicodeUTF8));
        checkBox_mirror->setText(QApplication::translate("calibrationDialog", "Mirror", 0, QApplication::UnicodeUTF8));
        checkBox_showHorizontalLines->setText(QApplication::translate("calibrationDialog", "Show horizontal lines", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("calibrationDialog", "Chessboard", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_15->setToolTip(QApplication::translate("calibrationDialog", "Number of inner squares on the board", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_15->setText(QApplication::translate("calibrationDialog", "Square size", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_12->setToolTip(QApplication::translate("calibrationDialog", "Number of inner squares on the board", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_12->setText(QApplication::translate("calibrationDialog", "Board size", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_16->setToolTip(QApplication::translate("calibrationDialog", "Number of inner squares on the board", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_16->setText(QString());
        doubleSpinBox_squareSize->setSuffix(QApplication::translate("calibrationDialog", " m", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_21->setToolTip(QApplication::translate("calibrationDialog", "Number of inner squares on the board", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_21->setText(QApplication::translate("calibrationDialog", "Download", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_20->setToolTip(QApplication::translate("calibrationDialog", "Number of inner squares on the board", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_20->setText(QApplication::translate("calibrationDialog", "<html><head/><body><p><a href=\"https://github.com/introlab/rtabmap/raw/master/bin/data/check-108.pdf\"><span style=\" text-decoration: underline; color:#0000ff;\">Sample8x6</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("calibrationDialog", "Progress", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("calibrationDialog", "X", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("calibrationDialog", "Skew", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("calibrationDialog", "Y", 0, QApplication::UnicodeUTF8));
        progressBar_count->setFormat(QApplication::translate("calibrationDialog", "%p", 0, QApplication::UnicodeUTF8));
        progressBar_x->setFormat(QApplication::translate("calibrationDialog", "%p%", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("calibrationDialog", "Size", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("calibrationDialog", "Count", 0, QApplication::UnicodeUTF8));
        progressBar_x_2->setFormat(QApplication::translate("calibrationDialog", "%p%", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("calibrationDialog", "Serial", 0, QApplication::UnicodeUTF8));
        label_serial->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        progressBar_count_2->setFormat(QApplication::translate("calibrationDialog", "%p", 0, QApplication::UnicodeUTF8));
        checkBox_switchImages->setText(QApplication::translate("calibrationDialog", "Switch images", 0, QApplication::UnicodeUTF8));
        pushButton_calibrate->setText(QApplication::translate("calibrationDialog", "Calibrate", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("calibrationDialog", "Camera intrinsic parameters", 0, QApplication::UnicodeUTF8));
        label_fx->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_fy->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_cx->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("calibrationDialog", "cy", 0, QApplication::UnicodeUTF8));
        label_cy->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_6->setToolTip(QApplication::translate("calibrationDialog", "Camera matrix", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("calibrationDialog", "K", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("calibrationDialog", "fy", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("calibrationDialog", "cx", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("calibrationDialog", "fx", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_17->setToolTip(QApplication::translate("calibrationDialog", "Distorsion coefficients", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_17->setText(QApplication::translate("calibrationDialog", "R", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_7->setToolTip(QApplication::translate("calibrationDialog", "Distorsion coefficients", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("calibrationDialog", "D", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_14->setToolTip(QApplication::translate("calibrationDialog", "Avg. reproduction error", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_14->setText(QApplication::translate("calibrationDialog", "Error", 0, QApplication::UnicodeUTF8));
        label_error->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_fx_2->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_cx_2->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_fy_2->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_cy_2->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_18->setToolTip(QApplication::translate("calibrationDialog", "Distorsion coefficients", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_18->setText(QApplication::translate("calibrationDialog", "P", 0, QApplication::UnicodeUTF8));
        label_baseline_name->setText(QApplication::translate("calibrationDialog", "baseline", 0, QApplication::UnicodeUTF8));
        label_baseline->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        label_error_2->setText(QApplication::translate("calibrationDialog", "0", 0, QApplication::UnicodeUTF8));
        pushButton_save->setText(QApplication::translate("calibrationDialog", "Export calibration (*.yaml)", 0, QApplication::UnicodeUTF8));
        pushButton_restart->setText(QApplication::translate("calibrationDialog", "Restart", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class calibrationDialog: public Ui_calibrationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATIONDIALOG_H
