/********************************************************************************
** Form generated from reading UI file 'aboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_aboutDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_10;
    QFrame *frame;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QLabel *label_9;
    QGridLayout *gridLayout;
    QLabel *label_16;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label_HomePage_2;
    QLabel *label_8;
    QLabel *label_version;
    QLabel *label_HomePage_3;
    QLabel *label_7;
    QLabel *label_opencv_version;
    QLabel *label_11;
    QLabel *label_pcl_version;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_freenect;
    QLabel *label_openni2;
    QLabel *label_g2o;
    QLabel *label_15;
    QLabel *label_17;
    QLabel *label_freenect2;
    QLabel *label_dc1394;
    QLabel *label_flycapture2;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_3;
    QLabel *label_5;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *aboutDialog)
    {
        if (aboutDialog->objectName().isEmpty())
            aboutDialog->setObjectName(QString::fromUtf8("aboutDialog"));
        aboutDialog->resize(824, 507);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(aboutDialog->sizePolicy().hasHeightForWidth());
        aboutDialog->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(aboutDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_10 = new QLabel(aboutDialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMaximumSize(QSize(100, 100));
        label_10->setPixmap(QPixmap(QString::fromUtf8(":/images/RTAB-Map.png")));
        label_10->setScaledContents(true);
        label_10->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_10);

        frame = new QFrame(aboutDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_4->addWidget(label);

        label_9 = new QLabel(frame);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_9->setWordWrap(true);

        verticalLayout_4->addWidget(label_9);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_16 = new QLabel(frame);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout->addWidget(label_16, 13, 0, 1, 1);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 7, 0, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_HomePage_2 = new QLabel(frame);
        label_HomePage_2->setObjectName(QString::fromUtf8("label_HomePage_2"));
        label_HomePage_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_HomePage_2->setOpenExternalLinks(true);

        gridLayout->addWidget(label_HomePage_2, 3, 1, 1, 1);

        label_8 = new QLabel(frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 1, 1, 1, 1);

        label_version = new QLabel(frame);
        label_version->setObjectName(QString::fromUtf8("label_version"));
        label_version->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_version, 7, 1, 1, 1);

        label_HomePage_3 = new QLabel(frame);
        label_HomePage_3->setObjectName(QString::fromUtf8("label_HomePage_3"));
        label_HomePage_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_HomePage_3->setOpenExternalLinks(true);

        gridLayout->addWidget(label_HomePage_3, 4, 1, 1, 1);

        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 8, 0, 1, 1);

        label_opencv_version = new QLabel(frame);
        label_opencv_version->setObjectName(QString::fromUtf8("label_opencv_version"));
        label_opencv_version->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_opencv_version, 8, 1, 1, 1);

        label_11 = new QLabel(frame);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 9, 0, 1, 1);

        label_pcl_version = new QLabel(frame);
        label_pcl_version->setObjectName(QString::fromUtf8("label_pcl_version"));
        label_pcl_version->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_pcl_version, 9, 1, 1, 1);

        label_12 = new QLabel(frame);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 10, 0, 1, 1);

        label_13 = new QLabel(frame);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 11, 0, 1, 1);

        label_14 = new QLabel(frame);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 15, 0, 1, 1);

        label_freenect = new QLabel(frame);
        label_freenect->setObjectName(QString::fromUtf8("label_freenect"));
        label_freenect->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_freenect, 10, 1, 1, 1);

        label_openni2 = new QLabel(frame);
        label_openni2->setObjectName(QString::fromUtf8("label_openni2"));
        label_openni2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_openni2, 11, 1, 1, 1);

        label_g2o = new QLabel(frame);
        label_g2o->setObjectName(QString::fromUtf8("label_g2o"));
        label_g2o->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_g2o, 15, 1, 1, 1);

        label_15 = new QLabel(frame);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 12, 0, 1, 1);

        label_17 = new QLabel(frame);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout->addWidget(label_17, 14, 0, 1, 1);

        label_freenect2 = new QLabel(frame);
        label_freenect2->setObjectName(QString::fromUtf8("label_freenect2"));
        label_freenect2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_freenect2, 12, 1, 1, 1);

        label_dc1394 = new QLabel(frame);
        label_dc1394->setObjectName(QString::fromUtf8("label_dc1394"));
        label_dc1394->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_dc1394, 13, 1, 1, 1);

        label_flycapture2 = new QLabel(frame);
        label_flycapture2->setObjectName(QString::fromUtf8("label_flycapture2"));
        label_flycapture2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_flycapture2, 14, 1, 1, 1);

        gridLayout->setColumnStretch(1, 1);

        verticalLayout_4->addLayout(gridLayout);

        verticalSpacer_3 = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/images/IntRoLabSmall.png")));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_3);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_5);


        horizontalLayout->addWidget(frame);


        verticalLayout_3->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(aboutDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(aboutDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), aboutDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), aboutDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(aboutDialog);
    } // setupUi

    void retranslateUi(QDialog *aboutDialog)
    {
        aboutDialog->setWindowTitle(QApplication::translate("aboutDialog", "About RTAB-Map", 0, QApplication::UnicodeUTF8));
        label_10->setText(QString());
        label->setText(QApplication::translate("aboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:14pt; font-weight:600;\">RTAB-Map : Real-Time Appearance-Based Mapping</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("aboutDialog", "RTAB-Map is an appearance-based loop closure detector for autonomous robot. This interface is used to control or to monitor the core process. ", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("aboutDialog", "With stereo dc1394 :", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("aboutDialog", "Version :", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("aboutDialog", "Links :", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("aboutDialog", "Author :", 0, QApplication::UnicodeUTF8));
        label_HomePage_2->setText(QApplication::translate("aboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://github.com/introlab/rtabmap\"><span style=\" text-decoration: underline; color:#0000ff;\">Source page</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("aboutDialog", "Mathieu Labb\303\251, matlabbe@gmail.com", 0, QApplication::UnicodeUTF8));
        label_version->setText(QString());
        label_HomePage_3->setText(QApplication::translate("aboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://introlab.github.io/rtabmap\"><span style=\" text-decoration: underline; color:#0000ff;\">Home page</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("aboutDialog", "OpenCV version :", 0, QApplication::UnicodeUTF8));
        label_opencv_version->setText(QString());
        label_11->setText(QApplication::translate("aboutDialog", "PCL version :", 0, QApplication::UnicodeUTF8));
        label_pcl_version->setText(QString());
        label_12->setText(QApplication::translate("aboutDialog", "With Freenect :", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("aboutDialog", "With OpenNI2 :", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("aboutDialog", "With g2o :", 0, QApplication::UnicodeUTF8));
        label_freenect->setText(QString());
        label_openni2->setText(QString());
        label_g2o->setText(QString());
        label_15->setText(QApplication::translate("aboutDialog", "With Freenect2 :", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("aboutDialog", "With stereo FlyCapture2 :", 0, QApplication::UnicodeUTF8));
        label_freenect2->setText(QString());
        label_dc1394->setText(QString());
        label_flycapture2->setText(QString());
        label_3->setText(QString());
        label_5->setText(QApplication::translate("aboutDialog", "Copyright (C) 2010-2015 IntRoLab - Universit\303\251 de Sherbrooke", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class aboutDialog: public Ui_aboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
