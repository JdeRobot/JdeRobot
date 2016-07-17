# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_gui.ui'
#
# Created: Sun Oct 11 12:42:26 2015
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(400, 370)
        MainWindow.setMinimumSize(QtCore.QSize(400, 370))
        MainWindow.setMaximumSize(QtCore.QSize(400, 370))
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.verticalLayoutWidget = QtGui.QWidget(self.centralwidget)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(20, 30, 361, 301))
        self.verticalLayoutWidget.setObjectName(_fromUtf8("verticalLayoutWidget"))
        self.tlLayout = QtGui.QVBoxLayout(self.verticalLayoutWidget)
        self.tlLayout.setMargin(0)
        self.tlLayout.setObjectName(_fromUtf8("tlLayout"))
        self.XLabel = QtGui.QLabel(self.centralwidget)
        self.XLabel.setGeometry(QtCore.QRect(20, 340, 21, 21))
        self.XLabel.setObjectName(_fromUtf8("XLabel"))
        self.YLabel = QtGui.QLabel(self.centralwidget)
        self.YLabel.setGeometry(QtCore.QRect(130, 340, 21, 21))
        self.YLabel.setObjectName(_fromUtf8("YLabel"))
        self.XValue = QtGui.QLabel(self.centralwidget)
        self.XValue.setGeometry(QtCore.QRect(40, 340, 41, 21))
        self.XValue.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.XValue.setObjectName(_fromUtf8("XValue"))
        self.YValue = QtGui.QLabel(self.centralwidget)
        self.YValue.setGeometry(QtCore.QRect(150, 340, 41, 21))
        self.YValue.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.YValue.setObjectName(_fromUtf8("YValue"))
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "Basic Component py", None))
        self.XLabel.setText(_translate("MainWindow", "X:", None))
        self.YLabel.setText(_translate("MainWindow", "Y:", None))
        self.XValue.setText(_translate("MainWindow", "0", None))
        self.YValue.setText(_translate("MainWindow", "0", None))