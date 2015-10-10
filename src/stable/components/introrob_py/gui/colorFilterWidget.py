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
from PyQt4 import QtGui,QtCore,uic
from sensors.colorFilterValues import ColorFilterValues
from gui.colorFilter import Ui_Form

class ColorFilterWidget(QtGui.QWidget, Ui_Form):
    IMAGE_COLS_MAX=320
    IMAGE_ROWS_MAX=240

    imageUpdate=QtCore.pyqtSignal()
    
    def __init__(self,winParent):      
        super(ColorFilterWidget, self).__init__()
	self.setupUi(self)
        self.winParent=winParent
        self.imageUpdate.connect(self.updateImage)

        self.filterValues = ColorFilterValues()

        self.resetButton.clicked[bool].connect(self.resetValues)
        self.trackButton.clicked[bool].connect(self.track)
        self.trackObject = False

        self.hminSlider.valueChanged[int].connect(self.hminValueChange)
        self.hmaxSlider.valueChanged[int].connect(self.hmaxValueChange)
        self.sminSlider.valueChanged[int].connect(self.sminValueChange)
        self.smaxSlider.valueChanged[int].connect(self.smaxValueChange)
        self.vminSlider.valueChanged[int].connect(self.vminValueChange)
        self.vmaxSlider.valueChanged[int].connect(self.vmaxValueChange)

    def updateImage(self):
        self.setInputImage()
        self.setThresoldImage()
        self.winParent.getSensor().setColorFilterValues(self.filterValues)

    def setInputImage(self):

        if self.trackObject:
            img = self.winParent.getSensor().getTrackImage()
        else:
            img = self.winParent.getSensor().getImage()

        if img is not None:
            image = QtGui.QImage(img.data, img.shape[1], img.shape[0], img.shape[1]*img.shape[2], QtGui.QImage.Format_RGB888);
            self.inputImage.setPixmap(QtGui.QPixmap.fromImage(image))


    def setThresoldImage(self):
        img = self.winParent.getSensor().getThresoldImage()
        if img is not None:
            image = QtGui.QImage(img.data, img.shape[1], img.shape[0], img.shape[1], QtGui.QImage.Format_Indexed8)
            self.outputFilterImage.setPixmap(QtGui.QPixmap.fromImage(image))

    def track(self):
        if(self.trackObject==False):
            self.trackButton.setText("Untrack")
            self.trackObject=True
        else:
            self.trackButton.setText("Track")
            self.trackObject=False

    def getFilterValues(self):
        return self.filterValues

    def setFilterValues(self,values):
        self.filterValues=values

    def resetValues(self):
        self.filterValues.setHMin(0)
        self.hminSlider.setValue(self.filterValues.getHMin())
        self.filterValues.setHMax(0)
        self.hmaxSlider.setValue(self.filterValues.getHMax())
        self.filterValues.setSMin(0)
        self.sminSlider.setValue(self.filterValues.getSMin())
        self.filterValues.setSMax(0)
        self.smaxSlider.setValue(self.filterValues.getSMax())
        self.filterValues.setVMin(0)
        self.vminSlider.setValue(self.filterValues.getVMin())
        self.filterValues.setVMax(0)
        self.vmaxSlider.setValue(self.filterValues.getVMax())

    def closeEvent(self, event):
        self.winParent.closeColorFilterWidget()

    def hminValueChange(self, value):
        self.filterValues.setHMin(value)
        self.hminValue.setNum(self.filterValues.getHMin())

    def hmaxValueChange(self, value):
        self.filterValues.setHMax(value)
        self.hmaxValue.setNum(self.filterValues.getHMax())

    def sminValueChange(self, value):
        self.filterValues.setSMin(value)
        self.sminValue.setNum(self.filterValues.getSMin())

    def smaxValueChange(self, value):
        self.filterValues.setSMax(value)
        self.smaxValue.setNum(self.filterValues.getSMax())

    def vminValueChange(self, value):
        self.filterValues.setVMin(value)
        self.vminValue.setNum(self.filterValues.getVMin())

    def vmaxValueChange(self, value):
        self.filterValues.setVMax(value)
        self.vmaxValue.setNum(self.filterValues.getVMax())
        
