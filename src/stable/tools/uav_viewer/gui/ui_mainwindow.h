/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Aug 1 20:12:34 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "qwt_thermo.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *view_tab;
    QGroupBox *vel_groupbox;
    QLabel *velXLabel;
    QLabel *velYLabel;
    QLabel *velZLabel;
    QGroupBox *data_groupbox;
    QwtThermo *battery;
    QLabel *batteryLabel;
    QLabel *yawLabel;
    QLabel *altdLabel;
    QLabel *pitchLabel;
    QLabel *rollLabel;
    QLabel *pitchValue;
    QLabel *rollValue;
    QLabel *altdValue;
    QLabel *yawValue;
    QLabel *yawG;
    QLabel *pitchG;
    QLabel *rollG;
    QLabel *altdM;
    QLabel *imageLabel;
    QGroupBox *controlBox;
    QPushButton *takeoff_button;
    QPushButton *land_button;
    QPushButton *reset_button;
    QPushButton *angZL;
    QPushButton *linZD;
    QPushButton *angZR;
    QPushButton *linZU;
    QPushButton *linXF;
    QPushButton *linYR;
    QPushButton *linYL;
    QPushButton *linXB;
    QPushButton *toggleCam;
    QLabel *imageVLabel;
    QLabel *statusLabel;
    QLabel *statusValue;
    QLabel *imageLLabel;
    QWidget *gps_tab;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1240, 620);
        MainWindow->setMinimumSize(QSize(1240, 620));
        MainWindow->setMaximumSize(QSize(1240, 620));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 0, 1221, 611));
        view_tab = new QWidget();
        view_tab->setObjectName(QString::fromUtf8("view_tab"));
        vel_groupbox = new QGroupBox(view_tab);
        vel_groupbox->setObjectName(QString::fromUtf8("vel_groupbox"));
        vel_groupbox->setGeometry(QRect(720, 330, 521, 221));
        velXLabel = new QLabel(vel_groupbox);
        velXLabel->setObjectName(QString::fromUtf8("velXLabel"));
        velXLabel->setGeometry(QRect(60, 190, 51, 21));
        velYLabel = new QLabel(vel_groupbox);
        velYLabel->setObjectName(QString::fromUtf8("velYLabel"));
        velYLabel->setGeometry(QRect(220, 190, 51, 21));
        velZLabel = new QLabel(vel_groupbox);
        velZLabel->setObjectName(QString::fromUtf8("velZLabel"));
        velZLabel->setGeometry(QRect(370, 190, 51, 21));
        data_groupbox = new QGroupBox(view_tab);
        data_groupbox->setObjectName(QString::fromUtf8("data_groupbox"));
        data_groupbox->setGeometry(QRect(710, 10, 521, 311));
        battery = new QwtThermo(data_groupbox);
        battery->setObjectName(QString::fromUtf8("battery"));
        battery->setGeometry(QRect(430, 30, 56, 241));
        battery->setMaxValue(100);
        batteryLabel = new QLabel(data_groupbox);
        batteryLabel->setObjectName(QString::fromUtf8("batteryLabel"));
        batteryLabel->setGeometry(QRect(440, 290, 51, 21));
        yawLabel = new QLabel(data_groupbox);
        yawLabel->setObjectName(QString::fromUtf8("yawLabel"));
        yawLabel->setGeometry(QRect(260, 130, 41, 21));
        altdLabel = new QLabel(data_groupbox);
        altdLabel->setObjectName(QString::fromUtf8("altdLabel"));
        altdLabel->setGeometry(QRect(260, 290, 41, 21));
        pitchLabel = new QLabel(data_groupbox);
        pitchLabel->setObjectName(QString::fromUtf8("pitchLabel"));
        pitchLabel->setGeometry(QRect(70, 250, 41, 21));
        rollLabel = new QLabel(data_groupbox);
        rollLabel->setObjectName(QString::fromUtf8("rollLabel"));
        rollLabel->setGeometry(QRect(70, 270, 41, 21));
        pitchValue = new QLabel(data_groupbox);
        pitchValue->setObjectName(QString::fromUtf8("pitchValue"));
        pitchValue->setGeometry(QRect(110, 250, 65, 21));
        pitchValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        rollValue = new QLabel(data_groupbox);
        rollValue->setObjectName(QString::fromUtf8("rollValue"));
        rollValue->setGeometry(QRect(110, 270, 65, 21));
        rollValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        altdValue = new QLabel(data_groupbox);
        altdValue->setObjectName(QString::fromUtf8("altdValue"));
        altdValue->setGeometry(QRect(300, 290, 65, 21));
        altdValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yawValue = new QLabel(data_groupbox);
        yawValue->setObjectName(QString::fromUtf8("yawValue"));
        yawValue->setGeometry(QRect(300, 130, 65, 21));
        yawValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yawG = new QLabel(data_groupbox);
        yawG->setObjectName(QString::fromUtf8("yawG"));
        yawG->setGeometry(QRect(370, 130, 16, 21));
        pitchG = new QLabel(data_groupbox);
        pitchG->setObjectName(QString::fromUtf8("pitchG"));
        pitchG->setGeometry(QRect(180, 250, 16, 21));
        rollG = new QLabel(data_groupbox);
        rollG->setObjectName(QString::fromUtf8("rollG"));
        rollG->setGeometry(QRect(180, 270, 16, 21));
        altdM = new QLabel(data_groupbox);
        altdM->setObjectName(QString::fromUtf8("altdM"));
        altdM->setGeometry(QRect(370, 290, 16, 21));
        imageLabel = new QLabel(view_tab);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(120, 100, 320, 240));
        imageLabel->setMinimumSize(QSize(320, 240));
        imageLabel->setMaximumSize(QSize(320, 240));
        controlBox = new QGroupBox(view_tab);
        controlBox->setObjectName(QString::fromUtf8("controlBox"));
        controlBox->setGeometry(QRect(160, 380, 391, 211));
        takeoff_button = new QPushButton(controlBox);
        takeoff_button->setObjectName(QString::fromUtf8("takeoff_button"));
        takeoff_button->setGeometry(QRect(10, 40, 95, 31));
        land_button = new QPushButton(controlBox);
        land_button->setObjectName(QString::fromUtf8("land_button"));
        land_button->setGeometry(QRect(140, 40, 95, 31));
        reset_button = new QPushButton(controlBox);
        reset_button->setObjectName(QString::fromUtf8("reset_button"));
        reset_button->setGeometry(QRect(260, 40, 95, 31));
        angZL = new QPushButton(controlBox);
        angZL->setObjectName(QString::fromUtf8("angZL"));
        angZL->setGeometry(QRect(10, 140, 41, 31));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../buttons/rotate_left.png"), QSize(), QIcon::Normal, QIcon::Off);
        angZL->setIcon(icon);
        linZD = new QPushButton(controlBox);
        linZD->setObjectName(QString::fromUtf8("linZD"));
        linZD->setGeometry(QRect(60, 140, 41, 31));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../buttons/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        linZD->setIcon(icon1);
        angZR = new QPushButton(controlBox);
        angZR->setObjectName(QString::fromUtf8("angZR"));
        angZR->setGeometry(QRect(110, 140, 41, 31));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../buttons/rotate_right.png"), QSize(), QIcon::Normal, QIcon::Off);
        angZR->setIcon(icon2);
        linZU = new QPushButton(controlBox);
        linZU->setObjectName(QString::fromUtf8("linZU"));
        linZU->setGeometry(QRect(60, 100, 41, 31));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("../buttons/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        linZU->setIcon(icon3);
        linXF = new QPushButton(controlBox);
        linXF->setObjectName(QString::fromUtf8("linXF"));
        linXF->setGeometry(QRect(260, 100, 41, 31));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("../buttons/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        linXF->setIcon(icon4);
        linYR = new QPushButton(controlBox);
        linYR->setObjectName(QString::fromUtf8("linYR"));
        linYR->setGeometry(QRect(310, 140, 41, 31));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("../buttons/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        linYR->setIcon(icon5);
        linYL = new QPushButton(controlBox);
        linYL->setObjectName(QString::fromUtf8("linYL"));
        linYL->setGeometry(QRect(210, 140, 41, 31));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("../buttons/left.png"), QSize(), QIcon::Normal, QIcon::Off);
        linYL->setIcon(icon6);
        linXB = new QPushButton(controlBox);
        linXB->setObjectName(QString::fromUtf8("linXB"));
        linXB->setGeometry(QRect(260, 140, 41, 31));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("../buttons/backward.png"), QSize(), QIcon::Normal, QIcon::Off);
        linXB->setIcon(icon7);
        toggleCam = new QPushButton(controlBox);
        toggleCam->setObjectName(QString::fromUtf8("toggleCam"));
        toggleCam->setGeometry(QRect(160, 100, 41, 31));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8("../buttons/toggle_cam.png"), QSize(), QIcon::Normal, QIcon::Off);
        toggleCam->setIcon(icon8);
        imageVLabel = new QLabel(view_tab);
        imageVLabel->setObjectName(QString::fromUtf8("imageVLabel"));
        imageVLabel->setGeometry(QRect(180, 140, 200, 180));
        statusLabel = new QLabel(view_tab);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(20, 10, 51, 21));
        statusValue = new QLabel(view_tab);
        statusValue->setObjectName(QString::fromUtf8("statusValue"));
        statusValue->setGeometry(QRect(90, 10, 65, 21));
        imageLLabel = new QLabel(view_tab);
        imageLLabel->setObjectName(QString::fromUtf8("imageLLabel"));
        imageLLabel->setGeometry(QRect(20, 40, 640, 320));
        tabWidget->addTab(view_tab, QString());
        gps_tab = new QWidget();
        gps_tab->setObjectName(QString::fromUtf8("gps_tab"));
        tabWidget->addTab(gps_tab, QString());
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "uav_viewer", 0, QApplication::UnicodeUTF8));
        vel_groupbox->setTitle(QApplication::translate("MainWindow", "Velocities", 0, QApplication::UnicodeUTF8));
        velXLabel->setText(QApplication::translate("MainWindow", "Lineal X", 0, QApplication::UnicodeUTF8));
        velYLabel->setText(QApplication::translate("MainWindow", "Lineal Y", 0, QApplication::UnicodeUTF8));
        velZLabel->setText(QApplication::translate("MainWindow", "Lineal Z", 0, QApplication::UnicodeUTF8));
        data_groupbox->setTitle(QApplication::translate("MainWindow", "Data", 0, QApplication::UnicodeUTF8));
        batteryLabel->setText(QApplication::translate("MainWindow", "Battery", 0, QApplication::UnicodeUTF8));
        yawLabel->setText(QApplication::translate("MainWindow", "Yaw:", 0, QApplication::UnicodeUTF8));
        altdLabel->setText(QApplication::translate("MainWindow", "Altd.:", 0, QApplication::UnicodeUTF8));
        pitchLabel->setText(QApplication::translate("MainWindow", "Pitch:", 0, QApplication::UnicodeUTF8));
        rollLabel->setText(QApplication::translate("MainWindow", "Roll:", 0, QApplication::UnicodeUTF8));
        pitchValue->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        rollValue->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        altdValue->setText(QApplication::translate("MainWindow", "10", 0, QApplication::UnicodeUTF8));
        yawValue->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        yawG->setText(QApplication::translate("MainWindow", "\302\272", 0, QApplication::UnicodeUTF8));
        pitchG->setText(QApplication::translate("MainWindow", "\302\272", 0, QApplication::UnicodeUTF8));
        rollG->setText(QApplication::translate("MainWindow", "\302\272", 0, QApplication::UnicodeUTF8));
        altdM->setText(QApplication::translate("MainWindow", "m", 0, QApplication::UnicodeUTF8));
        imageLabel->setText(QString());
        controlBox->setTitle(QApplication::translate("MainWindow", "Control", 0, QApplication::UnicodeUTF8));
        takeoff_button->setText(QApplication::translate("MainWindow", "take off", 0, QApplication::UnicodeUTF8));
        land_button->setText(QApplication::translate("MainWindow", "land", 0, QApplication::UnicodeUTF8));
        reset_button->setText(QApplication::translate("MainWindow", "reset", 0, QApplication::UnicodeUTF8));
        angZL->setText(QString());
        linZD->setText(QString());
        angZR->setText(QString());
        linZU->setText(QString());
        linXF->setText(QString());
        linYR->setText(QString());
        linYL->setText(QString());
        linXB->setText(QString());
        toggleCam->setText(QString());
        imageVLabel->setText(QString());
        statusLabel->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        statusValue->setText(QString());
        imageLLabel->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(view_tab), QApplication::translate("MainWindow", "Viewer", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(gps_tab), QApplication::translate("MainWindow", "GPS", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
