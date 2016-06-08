#
#  Copyright (C) 1997-2016 JDE Developers Team
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
#       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
#


from PyQt4 import QtGui, QtCore
from gui.ui_gui import Ui_MainWindow
from gui.teleopWidget import TeleopWidget
from gui.cameraWidget import CameraWidget
from gui.communicator import Communicator


class MainWindow(QtGui.QMainWindow, Ui_MainWindow):

    updGUI = QtCore.pyqtSignal()

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.teleop = TeleopWidget(self)
        self.tlLayout.addWidget(self.teleop)
        self.teleop.setVisible(True)

        self.updGUI.connect(self.updateGUI)

        self.cameraWidget = CameraWidget(self)

        self.cameraCommunicator = Communicator()
        self.trackingCommunicator = Communicator()

    def setCamera(self, camera):
        self.camera = camera
        self.cameraWidget.show()

    def getCamera(self):
        return self.camera

    def setMotors(self, motors):
        self.motors = motors

    def getMotors(self):
        return self.motors

    def updateGUI(self):
        self.cameraWidget.imageUpdate.emit()

    def setXYValues(self, newX, newY):
        self.XValue.setText(str(newX))
        self.YValue.setText(str(newY))
        self.motors.setV(-newY)
        self.motors.setW(-newX)
        self.motors.sendVelocities()

