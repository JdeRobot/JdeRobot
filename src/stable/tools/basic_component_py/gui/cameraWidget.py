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


class CameraWidget(QtGui. QWidget):

    imageUpdate = QtCore.pyqtSignal()

    def __init__(self, winParent):
        super(CameraWidget, self).__init__()
        self.winParent = winParent
        self.imageUpdate.connect(self.updateImage)
        self.initUI()

    def initUI(self):

        self.setWindowTitle("Camera")

        self.imgLabel = QtGui.QLabel(self)
        self.imgLabel.show()

    def updateImage(self):

        img = self.winParent.getCamera().getImage()
        if img is not None:
            image = QtGui.QImage(img.data, img.shape[1], img.shape[0],
                                 img.shape[1] * img.shape[2], QtGui.QImage.Format_RGB888)

            size = QtCore.QSize(img.shape[1], img.shape[0])
            self.resize(size)
            self.imgLabel.resize(size)
            self.imgLabel.setPixmap(QtGui.QPixmap.fromImage(image))