/********************************************************************************
** Form generated from reading UI file 'exportCloudsDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTCLOUDSDIALOG_H
#define UI_EXPORTCLOUDSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ExportCloudsDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_regenerate;
    QGridLayout *gridLayout;
    QDoubleSpinBox *doubleSpinBox_voxelSize;
    QLabel *label_107;
    QSpinBox *spinBox_decimation;
    QLabel *label_108;
    QDoubleSpinBox *doubleSpinBox_maxDepth;
    QLabel *label_132;
    QGroupBox *groupBox_assemble;
    QGridLayout *gridLayout_2;
    QDoubleSpinBox *doubleSpinBox_voxelSize_assembled;
    QLabel *label_109;
    QCheckBox *checkBox_binary;
    QGroupBox *groupBox_mls;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *doubleSpinBox_mlsRadius;
    QLabel *label_87;
    QGroupBox *groupBox_gp3;
    QGridLayout *gridLayout_4;
    QSpinBox *spinBox_normalKSearch;
    QLabel *label_71;
    QDoubleSpinBox *doubleSpinBox_gp3Radius;
    QLabel *label_168;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExportCloudsDialog)
    {
        if (ExportCloudsDialog->objectName().isEmpty())
            ExportCloudsDialog->setObjectName(QString::fromUtf8("ExportCloudsDialog"));
        ExportCloudsDialog->resize(678, 579);
        verticalLayout_2 = new QVBoxLayout(ExportCloudsDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_regenerate = new QGroupBox(ExportCloudsDialog);
        groupBox_regenerate->setObjectName(QString::fromUtf8("groupBox_regenerate"));
        groupBox_regenerate->setCheckable(true);
        gridLayout = new QGridLayout(groupBox_regenerate);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        doubleSpinBox_voxelSize = new QDoubleSpinBox(groupBox_regenerate);
        doubleSpinBox_voxelSize->setObjectName(QString::fromUtf8("doubleSpinBox_voxelSize"));
        doubleSpinBox_voxelSize->setDecimals(3);
        doubleSpinBox_voxelSize->setMaximum(1);
        doubleSpinBox_voxelSize->setSingleStep(0.01);
        doubleSpinBox_voxelSize->setValue(0.005);

        gridLayout->addWidget(doubleSpinBox_voxelSize, 0, 0, 1, 1);

        label_107 = new QLabel(groupBox_regenerate);
        label_107->setObjectName(QString::fromUtf8("label_107"));
        label_107->setWordWrap(true);

        gridLayout->addWidget(label_107, 0, 1, 1, 1);

        spinBox_decimation = new QSpinBox(groupBox_regenerate);
        spinBox_decimation->setObjectName(QString::fromUtf8("spinBox_decimation"));
        spinBox_decimation->setMinimum(1);
        spinBox_decimation->setMaximum(32);
        spinBox_decimation->setValue(1);

        gridLayout->addWidget(spinBox_decimation, 1, 0, 1, 1);

        label_108 = new QLabel(groupBox_regenerate);
        label_108->setObjectName(QString::fromUtf8("label_108"));
        label_108->setWordWrap(true);

        gridLayout->addWidget(label_108, 1, 1, 1, 1);

        doubleSpinBox_maxDepth = new QDoubleSpinBox(groupBox_regenerate);
        doubleSpinBox_maxDepth->setObjectName(QString::fromUtf8("doubleSpinBox_maxDepth"));
        doubleSpinBox_maxDepth->setDecimals(1);
        doubleSpinBox_maxDepth->setMaximum(100);
        doubleSpinBox_maxDepth->setSingleStep(0.1);
        doubleSpinBox_maxDepth->setValue(4);

        gridLayout->addWidget(doubleSpinBox_maxDepth, 2, 0, 1, 1);

        label_132 = new QLabel(groupBox_regenerate);
        label_132->setObjectName(QString::fromUtf8("label_132"));
        label_132->setWordWrap(true);

        gridLayout->addWidget(label_132, 2, 1, 1, 1);

        gridLayout->setColumnStretch(1, 1);

        verticalLayout_2->addWidget(groupBox_regenerate);

        groupBox_assemble = new QGroupBox(ExportCloudsDialog);
        groupBox_assemble->setObjectName(QString::fromUtf8("groupBox_assemble"));
        groupBox_assemble->setCheckable(true);
        gridLayout_2 = new QGridLayout(groupBox_assemble);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        doubleSpinBox_voxelSize_assembled = new QDoubleSpinBox(groupBox_assemble);
        doubleSpinBox_voxelSize_assembled->setObjectName(QString::fromUtf8("doubleSpinBox_voxelSize_assembled"));
        doubleSpinBox_voxelSize_assembled->setDecimals(3);
        doubleSpinBox_voxelSize_assembled->setMaximum(1);
        doubleSpinBox_voxelSize_assembled->setSingleStep(0.01);
        doubleSpinBox_voxelSize_assembled->setValue(0.005);

        gridLayout_2->addWidget(doubleSpinBox_voxelSize_assembled, 0, 0, 1, 1);

        label_109 = new QLabel(groupBox_assemble);
        label_109->setObjectName(QString::fromUtf8("label_109"));
        label_109->setWordWrap(true);

        gridLayout_2->addWidget(label_109, 0, 1, 1, 1);

        gridLayout_2->setColumnStretch(1, 1);

        verticalLayout_2->addWidget(groupBox_assemble);

        checkBox_binary = new QCheckBox(ExportCloudsDialog);
        checkBox_binary->setObjectName(QString::fromUtf8("checkBox_binary"));
        checkBox_binary->setChecked(true);

        verticalLayout_2->addWidget(checkBox_binary);

        groupBox_mls = new QGroupBox(ExportCloudsDialog);
        groupBox_mls->setObjectName(QString::fromUtf8("groupBox_mls"));
        groupBox_mls->setCheckable(true);
        groupBox_mls->setChecked(false);
        verticalLayout = new QVBoxLayout(groupBox_mls);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox_mls);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        doubleSpinBox_mlsRadius = new QDoubleSpinBox(groupBox_mls);
        doubleSpinBox_mlsRadius->setObjectName(QString::fromUtf8("doubleSpinBox_mlsRadius"));
        doubleSpinBox_mlsRadius->setDecimals(3);
        doubleSpinBox_mlsRadius->setMaximum(1);
        doubleSpinBox_mlsRadius->setSingleStep(0.01);
        doubleSpinBox_mlsRadius->setValue(0.04);

        gridLayout_3->addWidget(doubleSpinBox_mlsRadius, 0, 0, 1, 1);

        label_87 = new QLabel(groupBox_mls);
        label_87->setObjectName(QString::fromUtf8("label_87"));
        label_87->setWordWrap(true);

        gridLayout_3->addWidget(label_87, 0, 1, 1, 1);

        gridLayout_3->setColumnStretch(1, 1);

        verticalLayout->addLayout(gridLayout_3);


        verticalLayout_2->addWidget(groupBox_mls);

        groupBox_gp3 = new QGroupBox(ExportCloudsDialog);
        groupBox_gp3->setObjectName(QString::fromUtf8("groupBox_gp3"));
        groupBox_gp3->setCheckable(true);
        groupBox_gp3->setChecked(false);
        gridLayout_4 = new QGridLayout(groupBox_gp3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        spinBox_normalKSearch = new QSpinBox(groupBox_gp3);
        spinBox_normalKSearch->setObjectName(QString::fromUtf8("spinBox_normalKSearch"));
        spinBox_normalKSearch->setMinimum(0);
        spinBox_normalKSearch->setValue(20);

        gridLayout_4->addWidget(spinBox_normalKSearch, 0, 0, 1, 1);

        label_71 = new QLabel(groupBox_gp3);
        label_71->setObjectName(QString::fromUtf8("label_71"));
        label_71->setWordWrap(true);

        gridLayout_4->addWidget(label_71, 0, 1, 1, 1);

        doubleSpinBox_gp3Radius = new QDoubleSpinBox(groupBox_gp3);
        doubleSpinBox_gp3Radius->setObjectName(QString::fromUtf8("doubleSpinBox_gp3Radius"));
        doubleSpinBox_gp3Radius->setDecimals(3);
        doubleSpinBox_gp3Radius->setMaximum(1);
        doubleSpinBox_gp3Radius->setSingleStep(0.01);
        doubleSpinBox_gp3Radius->setValue(0.04);

        gridLayout_4->addWidget(doubleSpinBox_gp3Radius, 1, 0, 1, 1);

        label_168 = new QLabel(groupBox_gp3);
        label_168->setObjectName(QString::fromUtf8("label_168"));
        label_168->setWordWrap(true);

        gridLayout_4->addWidget(label_168, 1, 1, 1, 1);

        gridLayout_4->setColumnStretch(1, 1);

        verticalLayout_2->addWidget(groupBox_gp3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(ExportCloudsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::RestoreDefaults|QDialogButtonBox::Save);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(ExportCloudsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExportCloudsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExportCloudsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ExportCloudsDialog);
    } // setupUi

    void retranslateUi(QDialog *ExportCloudsDialog)
    {
        ExportCloudsDialog->setWindowTitle(QApplication::translate("ExportCloudsDialog", "Export 3D clouds", 0, QApplication::UnicodeUTF8));
        groupBox_regenerate->setTitle(QApplication::translate("ExportCloudsDialog", "Regenerate clouds", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_voxelSize->setSuffix(QApplication::translate("ExportCloudsDialog", " m", 0, QApplication::UnicodeUTF8));
        label_107->setText(QApplication::translate("ExportCloudsDialog", "3D cloud voxel size.", 0, QApplication::UnicodeUTF8));
        label_108->setText(QApplication::translate("ExportCloudsDialog", "3D cloud decimation (1-2-4-8-...).", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_maxDepth->setSuffix(QApplication::translate("ExportCloudsDialog", " m", 0, QApplication::UnicodeUTF8));
        label_132->setText(QApplication::translate("ExportCloudsDialog", "3D cloud maximum depth (0 means no limit).", 0, QApplication::UnicodeUTF8));
        groupBox_assemble->setTitle(QApplication::translate("ExportCloudsDialog", "Assemble clouds to a single output cloud", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_voxelSize_assembled->setSuffix(QApplication::translate("ExportCloudsDialog", " m", 0, QApplication::UnicodeUTF8));
        label_109->setText(QApplication::translate("ExportCloudsDialog", "Voxel size.", 0, QApplication::UnicodeUTF8));
        checkBox_binary->setText(QApplication::translate("ExportCloudsDialog", "Binary file", 0, QApplication::UnicodeUTF8));
        groupBox_mls->setTitle(QApplication::translate("ExportCloudsDialog", "Mesh smoothing using Moving Least Squares algorithm (MLS)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ExportCloudsDialog", "WARNING: This adds significative time to process, though the clouds will be more smooth.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_mlsRadius->setSuffix(QApplication::translate("ExportCloudsDialog", " m", 0, QApplication::UnicodeUTF8));
        label_87->setText(QApplication::translate("ExportCloudsDialog", "MLS search radius: Set the sphere radius that is to be used for determining the k-nearest neighbors used for fitting. \n"
"Guidelines: 4 times the voxel size, 0.025 for voxel=0.", 0, QApplication::UnicodeUTF8));
        groupBox_gp3->setTitle(QApplication::translate("ExportCloudsDialog", "Meshing using Greedy Projection Triangulation algorithm (GP3)", 0, QApplication::UnicodeUTF8));
        label_71->setText(QApplication::translate("ExportCloudsDialog", "Set the number of k nearest neighbors to use for the normal estimation to create the mesh. Not used when mesh smoothing (MLS) above is used.", 0, QApplication::UnicodeUTF8));
        doubleSpinBox_gp3Radius->setSuffix(QApplication::translate("ExportCloudsDialog", " m", 0, QApplication::UnicodeUTF8));
        label_168->setText(QApplication::translate("ExportCloudsDialog", "Sphere radius that is to be used for determining the k-nearest neighbors used for triangulating (GP3). \n"
"Guidelines: 4 times the voxel size, 0.025 for voxel=0.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExportCloudsDialog: public Ui_ExportCloudsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTCLOUDSDIALOG_H
