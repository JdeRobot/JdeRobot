/********************************************************************************
** Form generated from reading UI file 'postProcessingDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POSTPROCESSINGDIALOG_H
#define UI_POSTPROCESSINGDIALOG_H

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

class Ui_PostProcessingDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *detectMoreLoopClosures;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QDoubleSpinBox *clusterAngle;
    QDoubleSpinBox *clusterRadius;
    QLabel *label_3;
    QLabel *clusterAngle_label;
    QLabel *label_5;
    QCheckBox *reextractFeatures;
    QLabel *label_6;
    QSpinBox *iterations;
    QLabel *label;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QCheckBox *refineNeighborLinks;
    QCheckBox *refineLoopClosureLinks;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PostProcessingDialog)
    {
        if (PostProcessingDialog->objectName().isEmpty())
            PostProcessingDialog->setObjectName(QString::fromUtf8("PostProcessingDialog"));
        PostProcessingDialog->resize(486, 425);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/RTAB-Map.ico"), QSize(), QIcon::Normal, QIcon::Off);
        PostProcessingDialog->setWindowIcon(icon);
        verticalLayout_3 = new QVBoxLayout(PostProcessingDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        detectMoreLoopClosures = new QGroupBox(PostProcessingDialog);
        detectMoreLoopClosures->setObjectName(QString::fromUtf8("detectMoreLoopClosures"));
        detectMoreLoopClosures->setCheckable(true);
        verticalLayout = new QVBoxLayout(detectMoreLoopClosures);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        clusterAngle = new QDoubleSpinBox(detectMoreLoopClosures);
        clusterAngle->setObjectName(QString::fromUtf8("clusterAngle"));
        clusterAngle->setDecimals(0);
        clusterAngle->setMinimum(0);
        clusterAngle->setMaximum(180);
        clusterAngle->setValue(30);

        gridLayout->addWidget(clusterAngle, 1, 0, 1, 1);

        clusterRadius = new QDoubleSpinBox(detectMoreLoopClosures);
        clusterRadius->setObjectName(QString::fromUtf8("clusterRadius"));
        clusterRadius->setMinimum(0.01);
        clusterRadius->setMaximum(99);
        clusterRadius->setSingleStep(0.01);
        clusterRadius->setValue(0.3);

        gridLayout->addWidget(clusterRadius, 0, 0, 1, 1);

        label_3 = new QLabel(detectMoreLoopClosures);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        gridLayout->addWidget(label_3, 0, 1, 1, 1);

        clusterAngle_label = new QLabel(detectMoreLoopClosures);
        clusterAngle_label->setObjectName(QString::fromUtf8("clusterAngle_label"));
        clusterAngle_label->setWordWrap(true);

        gridLayout->addWidget(clusterAngle_label, 1, 1, 1, 1);

        label_5 = new QLabel(detectMoreLoopClosures);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setWordWrap(true);

        gridLayout->addWidget(label_5, 3, 1, 1, 1);

        reextractFeatures = new QCheckBox(detectMoreLoopClosures);
        reextractFeatures->setObjectName(QString::fromUtf8("reextractFeatures"));
        reextractFeatures->setChecked(false);

        gridLayout->addWidget(reextractFeatures, 3, 0, 1, 1);

        label_6 = new QLabel(detectMoreLoopClosures);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setWordWrap(true);

        gridLayout->addWidget(label_6, 2, 1, 1, 1);

        iterations = new QSpinBox(detectMoreLoopClosures);
        iterations->setObjectName(QString::fromUtf8("iterations"));
        iterations->setMinimum(1);
        iterations->setMaximum(100);
        iterations->setValue(1);

        gridLayout->addWidget(iterations, 2, 0, 1, 1);

        gridLayout->setColumnStretch(1, 1);

        verticalLayout->addLayout(gridLayout);

        label = new QLabel(detectMoreLoopClosures);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);


        verticalLayout_3->addWidget(detectMoreLoopClosures);

        groupBox_3 = new QGroupBox(PostProcessingDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        verticalLayout_2->addWidget(label_2);

        refineNeighborLinks = new QCheckBox(groupBox_3);
        refineNeighborLinks->setObjectName(QString::fromUtf8("refineNeighborLinks"));
        refineNeighborLinks->setChecked(false);

        verticalLayout_2->addWidget(refineNeighborLinks);

        refineLoopClosureLinks = new QCheckBox(groupBox_3);
        refineLoopClosureLinks->setObjectName(QString::fromUtf8("refineLoopClosureLinks"));
        refineLoopClosureLinks->setChecked(false);

        verticalLayout_2->addWidget(refineLoopClosureLinks);


        verticalLayout_3->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(PostProcessingDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::RestoreDefaults);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(PostProcessingDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PostProcessingDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PostProcessingDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PostProcessingDialog);
    } // setupUi

    void retranslateUi(QDialog *PostProcessingDialog)
    {
        PostProcessingDialog->setWindowTitle(QApplication::translate("PostProcessingDialog", "Post-Processing", 0, QApplication::UnicodeUTF8));
        detectMoreLoopClosures->setTitle(QApplication::translate("PostProcessingDialog", "Detect more loop closures", 0, QApplication::UnicodeUTF8));
        clusterAngle->setSuffix(QApplication::translate("PostProcessingDialog", " degrees", 0, QApplication::UnicodeUTF8));
        clusterRadius->setSuffix(QApplication::translate("PostProcessingDialog", " m", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PostProcessingDialog", "Cluster radius", 0, QApplication::UnicodeUTF8));
        clusterAngle_label->setText(QApplication::translate("PostProcessingDialog", "Cluster angle", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("PostProcessingDialog", "Re-extract features", 0, QApplication::UnicodeUTF8));
        reextractFeatures->setText(QString());
        label_6->setText(QApplication::translate("PostProcessingDialog", "Iterations", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PostProcessingDialog", "See \"RTAB-Map settings/RGB-D SLAM/Loop closure constraint\" to tune the parameters used.", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("PostProcessingDialog", "Refine links with ICP 3D", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PostProcessingDialog", "See \"RTAB-Map settings/RGB-D SLAM/ICP\" to tune the parameters used.", 0, QApplication::UnicodeUTF8));
        refineNeighborLinks->setText(QApplication::translate("PostProcessingDialog", "Refine neighbor links", 0, QApplication::UnicodeUTF8));
        refineLoopClosureLinks->setText(QApplication::translate("PostProcessingDialog", "Refine loop closure links", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PostProcessingDialog: public Ui_PostProcessingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POSTPROCESSINGDIALOG_H
