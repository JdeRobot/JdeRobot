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

from PyQt5.QtCore import pyqtSignal, QPoint, Qt
from PyQt5.QtWidgets import QWidget, QLabel, QToolTip
from PyQt5.QtGui import QImage, QPixmap, QPainter, QColor, QCursor, QPalette

class MyLabel(QLabel):

    
    imageUpdate=pyqtSignal()
    
    def __init__(self,winParent, cross=False):      
        QLabel.__init__(self, winParent)
        #self.setFixedSize(self.IMAGE_COLS_MAX, self.IMAGE_ROWS_MAX)
        #self.scale = 1
        self.setMouseTracking(True)

        self.mouse_x = 0
        self.mouse_y = 0
        self.mouse_pos = None
        self.setCursor(Qt.BlankCursor)
        self.cross=cross
        self.img = None

        #img = self.pixmap().toImage()


    '''def wheelEvent(self, event):
        delta = event.angleDelta()/120
        print delta
        if (delta.y() > 0):
            self.scale += 0.1
        else:
            self.scale -= 0.1
        self.resize(self.pixmap().size()*self.scale'''

    def mouseMoveEvent(self, event):
        if not self.cross:
            mouse_x = event.pos().x()
            mouse_y = event.pos().y()
            self.mouse_pos = event.pos()
            if self.img != None:
                rgb = QColor(self.img.pixel(self.mouse_pos))
                text = "XY[" + str(mouse_x) + "," + str(mouse_y)+ "] RGB(" + str(rgb.red()) + "," + str(rgb.green()) + "," + str(rgb.blue()) + ")"
                QToolTip.showText(QPoint(QCursor.pos()), text)
            self.repaint()

    def getMousePos(self):
        return self.mouse_pos

    def setPixmap(self, pixmap):
        super(MyLabel, self).setPixmap(pixmap)
        self.img = pixmap.toImage()

    def paintEvent(self, event):

        super(MyLabel, self).paintEvent(event)
        painter = QPainter(self)
        if not self.cross:
            if self.mouse_pos != None:
                painter.drawLine(QPoint(self.mouse_pos.x(), 0) , QPoint(self.mouse_pos.x(), self.height()))
                painter.drawLine(QPoint(0,self.mouse_pos.y()) , QPoint(self.width(),self.mouse_pos.y()))
        else :
            self.paintCross(painter)

    def paintCross(self,painter):
        painter.drawLine(QPoint(self.width()/2,0), QPoint(self.width()/2,self.height()))
        painter.drawLine(QPoint(0,self.height()/2), QPoint(self.width(),self.height()/2))






        