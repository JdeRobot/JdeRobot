/********************************************************************************
** Form generated from reading UI file 'consoleWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSOLEWIDGET_H
#define UI_CONSOLEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_consoleWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QPushButton *pushButton_clear;

    void setupUi(QWidget *consoleWidget)
    {
        if (consoleWidget->objectName().isEmpty())
            consoleWidget->setObjectName(QString::fromUtf8("consoleWidget"));
        consoleWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(consoleWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(consoleWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setLineWrapMode(QTextEdit::NoWrap);
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);

        pushButton_clear = new QPushButton(consoleWidget);
        pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));

        verticalLayout->addWidget(pushButton_clear);


        retranslateUi(consoleWidget);

        QMetaObject::connectSlotsByName(consoleWidget);
    } // setupUi

    void retranslateUi(QWidget *consoleWidget)
    {
        consoleWidget->setWindowTitle(QApplication::translate("consoleWidget", "consoleWidget", 0, QApplication::UnicodeUTF8));
        pushButton_clear->setText(QApplication::translate("consoleWidget", "Clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class consoleWidget: public Ui_consoleWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLEWIDGET_H
