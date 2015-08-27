/********************************************************************************
** Form generated from reading UI file 'exportDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTDIALOG_H
#define UI_EXPORTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ExportDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *spinBox_ignored;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QDoubleSpinBox *doubleSpinBox_framerate;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *spinBox_session;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkBox_rgb;
    QCheckBox *checkBox_depth;
    QCheckBox *checkBox_depth2d;
    QCheckBox *checkBox_odom;
    QCheckBox *checkBox_userData;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit_path;
    QToolButton *toolButton_path;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExportDialog)
    {
        if (ExportDialog->objectName().isEmpty())
            ExportDialog->setObjectName(QString::fromUtf8("ExportDialog"));
        ExportDialog->resize(382, 331);
        verticalLayout = new QVBoxLayout(ExportDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        spinBox_ignored = new QSpinBox(ExportDialog);
        spinBox_ignored->setObjectName(QString::fromUtf8("spinBox_ignored"));

        horizontalLayout_2->addWidget(spinBox_ignored);

        label = new QLabel(ExportDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        doubleSpinBox_framerate = new QDoubleSpinBox(ExportDialog);
        doubleSpinBox_framerate->setObjectName(QString::fromUtf8("doubleSpinBox_framerate"));
        doubleSpinBox_framerate->setDecimals(1);

        horizontalLayout_4->addWidget(doubleSpinBox_framerate);

        label_3 = new QLabel(ExportDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        spinBox_session = new QSpinBox(ExportDialog);
        spinBox_session->setObjectName(QString::fromUtf8("spinBox_session"));
        spinBox_session->setMinimum(-1);
        spinBox_session->setMaximum(999);
        spinBox_session->setValue(-1);

        horizontalLayout_3->addWidget(spinBox_session);

        label_2 = new QLabel(ExportDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        checkBox_rgb = new QCheckBox(ExportDialog);
        checkBox_rgb->setObjectName(QString::fromUtf8("checkBox_rgb"));
        checkBox_rgb->setChecked(true);

        verticalLayout->addWidget(checkBox_rgb);

        checkBox_depth = new QCheckBox(ExportDialog);
        checkBox_depth->setObjectName(QString::fromUtf8("checkBox_depth"));
        checkBox_depth->setChecked(true);

        verticalLayout->addWidget(checkBox_depth);

        checkBox_depth2d = new QCheckBox(ExportDialog);
        checkBox_depth2d->setObjectName(QString::fromUtf8("checkBox_depth2d"));
        checkBox_depth2d->setChecked(true);

        verticalLayout->addWidget(checkBox_depth2d);

        checkBox_odom = new QCheckBox(ExportDialog);
        checkBox_odom->setObjectName(QString::fromUtf8("checkBox_odom"));
        checkBox_odom->setChecked(true);

        verticalLayout->addWidget(checkBox_odom);

        checkBox_userData = new QCheckBox(ExportDialog);
        checkBox_userData->setObjectName(QString::fromUtf8("checkBox_userData"));
        checkBox_userData->setChecked(false);

        verticalLayout->addWidget(checkBox_userData);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit_path = new QLineEdit(ExportDialog);
        lineEdit_path->setObjectName(QString::fromUtf8("lineEdit_path"));
        lineEdit_path->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit_path);

        toolButton_path = new QToolButton(ExportDialog);
        toolButton_path->setObjectName(QString::fromUtf8("toolButton_path"));

        horizontalLayout->addWidget(toolButton_path);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(ExportDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::RestoreDefaults);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ExportDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExportDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExportDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ExportDialog);
    } // setupUi

    void retranslateUi(QDialog *ExportDialog)
    {
        ExportDialog->setWindowTitle(QApplication::translate("ExportDialog", "Export...", 0, QApplication::UnicodeUTF8));
        spinBox_ignored->setSuffix(QString());
        label->setText(QApplication::translate("ExportDialog", "Frames ignored", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_framerate->setSuffix(QApplication::translate("ExportDialog", " Hz", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ExportDialog", "Target frame rate", 0, QApplication::UnicodeUTF8));
        spinBox_session->setSuffix(QString());
        label_2->setText(QApplication::translate("ExportDialog", "Mapping session (-1 = all)", 0, QApplication::UnicodeUTF8));
        checkBox_rgb->setText(QApplication::translate("ExportDialog", "RGB images", 0, QApplication::UnicodeUTF8));
        checkBox_depth->setText(QApplication::translate("ExportDialog", "Depth images", 0, QApplication::UnicodeUTF8));
        checkBox_depth2d->setText(QApplication::translate("ExportDialog", "Depth 2D (laser scans)", 0, QApplication::UnicodeUTF8));
        checkBox_odom->setText(QApplication::translate("ExportDialog", "Odometry", 0, QApplication::UnicodeUTF8));
        checkBox_userData->setText(QApplication::translate("ExportDialog", "User data (planned goals)", 0, QApplication::UnicodeUTF8));
        toolButton_path->setText(QApplication::translate("ExportDialog", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExportDialog: public Ui_ExportDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTDIALOG_H
