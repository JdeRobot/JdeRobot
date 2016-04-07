# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'additionalSubautWind.ui'
#
# Created: Sun Jan 31 12:31:50 2016
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

class Ui_SubautomataWindow(object):
    def setupUi(self, SubautomataWindow):
        SubautomataWindow.setObjectName(_fromUtf8("SubautomataWindow"))
        SubautomataWindow.resize(579, 455)
        self.verticalLayout = QtGui.QVBoxLayout(SubautomataWindow)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.title = QtGui.QLabel(SubautomataWindow)
        self.title.setObjectName(_fromUtf8("title"))
        self.verticalLayout.addWidget(self.title)
        self.schema = QtGui.QGraphicsView(SubautomataWindow)
        self.schema.setObjectName(_fromUtf8("schema"))
        self.verticalLayout.addWidget(self.schema)

        self.retranslateUi(SubautomataWindow)
        QtCore.QMetaObject.connectSlotsByName(SubautomataWindow)

    def retranslateUi(self, SubautomataWindow):
        SubautomataWindow.setWindowTitle(_translate("SubautomataWindow", "Form", None))
        self.title.setText(_translate("SubautomataWindow", "Default Title", None))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    SubautomataWindow = QtGui.QWidget()
    ui = Ui_SubautomataWindow()
    ui.setupUi(SubautomataWindow)
    SubautomataWindow.show()
    sys.exit(app.exec_())

