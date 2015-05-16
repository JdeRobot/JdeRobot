#
#  Copyright (C) 1997-2015 JDE Developers Team
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see http://www.gnu.org/licenses/.
#  Authors :
#       Alberto Martin Florido <almartinflorido@gmail.com>
#

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(660, 370)
        MainWindow.setMinimumSize(QtCore.QSize(660, 370))
        MainWindow.setMaximumSize(QtCore.QSize(660, 370))
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.takeoffButton = QtGui.QPushButton(self.centralwidget)
        self.takeoffButton.setGeometry(QtCore.QRect(470, 30, 161, 41))
        self.takeoffButton.setObjectName(_fromUtf8("takeoffButton"))
        self.altdSlider = QtGui.QSlider(self.centralwidget)
        self.altdSlider.setGeometry(QtCore.QRect(400, 30, 19, 311))
        self.altdSlider.setMaximum(100)
        self.altdSlider.setProperty("value", 49)
        self.altdSlider.setOrientation(QtCore.Qt.Vertical)
        self.altdSlider.setObjectName(_fromUtf8("altdSlider"))
        self.playButton = QtGui.QPushButton(self.centralwidget)
        self.playButton.setGeometry(QtCore.QRect(470, 80, 71, 51))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8("resources/play.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.playButton.setIcon(icon)
        self.playButton.setObjectName(_fromUtf8("playButton"))
        self.stopButton = QtGui.QPushButton(self.centralwidget)
        self.stopButton.setGeometry(QtCore.QRect(560, 80, 71, 51))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8("resources/stop.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.stopButton.setIcon(icon1)
        self.stopButton.setObjectName(_fromUtf8("stopButton"))
        self.windowsLabel = QtGui.QLabel(self.centralwidget)
        self.windowsLabel.setGeometry(QtCore.QRect(540, 190, 71, 21))
        self.windowsLabel.setObjectName(_fromUtf8("windowsLabel"))
        self.cameraCheck = QtGui.QCheckBox(self.centralwidget)
        self.cameraCheck.setGeometry(QtCore.QRect(540, 220, 94, 26))
        self.cameraCheck.setObjectName(_fromUtf8("cameraCheck"))
        self.sensorsCheck = QtGui.QCheckBox(self.centralwidget)
        self.sensorsCheck.setGeometry(QtCore.QRect(540, 250, 94, 26))
        self.sensorsCheck.setObjectName(_fromUtf8("sensorsCheck"))
        self.worldCheck = QtGui.QCheckBox(self.centralwidget)
        self.worldCheck.setGeometry(QtCore.QRect(540, 280, 94, 26))
        self.worldCheck.setObjectName(_fromUtf8("worldCheck"))
        self.altdLabel = QtGui.QLabel(self.centralwidget)
        self.altdLabel.setGeometry(QtCore.QRect(390, 340, 51, 21))
        self.altdLabel.setObjectName(_fromUtf8("altdLabel"))
        self.verticalLayoutWidget = QtGui.QWidget(self.centralwidget)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(20, 30, 361, 301))
        self.verticalLayoutWidget.setObjectName(_fromUtf8("verticalLayoutWidget"))
        self.tlLayout = QtGui.QVBoxLayout(self.verticalLayoutWidget)
        self.tlLayout.setMargin(0)
        self.tlLayout.setObjectName(_fromUtf8("tlLayout"))
        self.rotationDial = QtGui.QDial(self.centralwidget)
        self.rotationDial.setGeometry(QtCore.QRect(440, 220, 50, 64))
        self.rotationDial.setMaximum(100)
        self.rotationDial.setProperty("value", 49)
        self.rotationDial.setObjectName(_fromUtf8("rotationDial"))
        self.rotationLabel = QtGui.QLabel(self.centralwidget)
        self.rotationLabel.setGeometry(QtCore.QRect(440, 280, 65, 21))
        self.rotationLabel.setObjectName(_fromUtf8("rotationLabel"))
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
        self.altdValue = QtGui.QLabel(self.centralwidget)
        self.altdValue.setGeometry(QtCore.QRect(390, 10, 41, 21))
        self.altdValue.setAlignment(QtCore.Qt.AlignCenter)
        self.altdValue.setObjectName(_fromUtf8("altdValue"))
        self.rotValue = QtGui.QLabel(self.centralwidget)
        self.rotValue.setGeometry(QtCore.QRect(445, 200, 41, 21))
        self.rotValue.setAlignment(QtCore.Qt.AlignCenter)
        self.rotValue.setObjectName(_fromUtf8("rotValue"))
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "Introrob py", None, QtGui.QApplication.UnicodeUTF8))
        self.takeoffButton.setText(QtGui.QApplication.translate("MainWindow", "Take off", None, QtGui.QApplication.UnicodeUTF8))
        self.playButton.setText(QtGui.QApplication.translate("MainWindow", "Play", None, QtGui.QApplication.UnicodeUTF8))
        self.stopButton.setText(QtGui.QApplication.translate("MainWindow", "Stop", None, QtGui.QApplication.UnicodeUTF8))
        self.windowsLabel.setText(QtGui.QApplication.translate("MainWindow", "Windows:", None, QtGui.QApplication.UnicodeUTF8))
        self.cameraCheck.setText(QtGui.QApplication.translate("MainWindow", "Camera", None, QtGui.QApplication.UnicodeUTF8))
        self.sensorsCheck.setText(QtGui.QApplication.translate("MainWindow", "Sensors", None, QtGui.QApplication.UnicodeUTF8))
        self.worldCheck.setText(QtGui.QApplication.translate("MainWindow", "3D world", None, QtGui.QApplication.UnicodeUTF8))
        self.altdLabel.setText(QtGui.QApplication.translate("MainWindow", "Altitude", None, QtGui.QApplication.UnicodeUTF8))
        self.rotationLabel.setText(QtGui.QApplication.translate("MainWindow", "Rotation", None, QtGui.QApplication.UnicodeUTF8))
        self.XLabel.setText(QtGui.QApplication.translate("MainWindow", "X:", None, QtGui.QApplication.UnicodeUTF8))
        self.YLabel.setText(QtGui.QApplication.translate("MainWindow", "Y:", None, QtGui.QApplication.UnicodeUTF8))
        self.XValue.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.YValue.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.altdValue.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.rotValue.setText(QtGui.QApplication.translate("MainWindow", "0", None, QtGui.QApplication.UnicodeUTF8))

