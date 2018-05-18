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

from PyQt5.QtCore import pyqtSignal, Qt,QRect
from PyQt5.QtWidgets import QWidget, QLabel, QRadioButton, QGridLayout, QVBoxLayout, QHBoxLayout, QSpacerItem, QSizePolicy, QSlider, QTextEdit, QLineEdit,QCheckBox
from PyQt5.QtGui import QIntValidator
from PyQt5 import QtWidgets
from filters.hsvFilter import HSVMAX, HSVMIN
from filters.rgbFilter import RGBMAX, RGBMIN
from filters.yuvFilter import YUVMAX, YUVMIN
from hsvWidget import HsvWidget
WIDTH = 1340
HEIGTH = 200

class ControlWidget(QWidget):
    updatehsv_disc=pyqtSignal()
    
    def __init__(self,winParent):      
        super(ControlWidget, self).__init__()
        self.winParent=winParent

        self.rgbdwn = list(RGBMIN)
        self.rgbup = list(RGBMAX)
        self.hsvdwn = list(HSVMIN)
        self.hsvup = list(HSVMAX)
        self.yuvdwn = list(YUVMIN)
        self.yuvup = list(YUVMAX)
        self.hsvWidget=HsvWidget(self)
        self.updatehsv_disc.connect(self.updatecolordisc)
        self.initUI()
        
    def initUI(self):

        #self.setMinimumSize(WIDTH,HEIGTH)
        #self.setMaximumSize(WIDTH,HEIGTH)

        '''Radio buttons for Original/RGB/HSV/YUV images'''
        self.origButton = QRadioButton("Original")
        self.rgbButton = QRadioButton("RGB")
        self.hsvButton = QRadioButton("HSV")
        self.yuvButton = QRadioButton("YUV")
        '''HSV status scheck'''
        self.hsvCheck = QCheckBox('HSV Color Disc')
        self.hsvCheck.setObjectName("hsvCheck")
        self.hsvCheck.stateChanged.connect(self.showHSVWidget)

        '''Signals for toggled radio buttons'''
        self.origButton.toggled.connect(lambda:self.origButtonState())       
        self.rgbButton.toggled.connect(lambda:self.rgbButtonState())
        self.hsvButton.toggled.connect(lambda:self.hsvButtonState())
        self.yuvButton.toggled.connect(lambda:self.yuvButtonState())

        self.origButton.setChecked(True)

        '''Main layout of the widget will contain several vertical layouts'''
        self.gLayout = QGridLayout(self)
        self.gLayout.setObjectName("gLayout")

        ''' Vertical Layout for radio buttons '''
        self.radio1Layout = QHBoxLayout()
        self.radio2Layout = QVBoxLayout()
        self.radio3Layout = QVBoxLayout()
        self.radioLayout = QVBoxLayout()
        self.radioLayout.setObjectName("radioLayout")
        self.radio1Layout.addWidget(self.origButton)
        self.radio1Layout.addWidget(self.rgbButton)
        self.radio1Layout.addWidget(self.hsvButton)
        self.radio1Layout.addWidget(self.yuvButton)
        self.radio1Layout.addWidget(self.hsvCheck)
        #self.radio1Layout.addLayout(self.radio2Layout)
        #self.radio1Layout.addLayout(self.radio3Layout)
        self.radioLayout.addLayout(self.radio1Layout)
        self.vSpacer = QSpacerItem(10, 10, QSizePolicy.Ignored, QSizePolicy.Ignored);
        self.radioLayout.addItem(self.vSpacer)
        
        hmin,smin,vmin = HSVMIN
        hmax,smax,vmax = HSVMAX
        ''' Vertical Layout for HMIN Slider''' 
        self.hminLayout = QVBoxLayout()
        self.hminLayout.setObjectName("hminLayout")
        self.hminLabel = QLabel("HMin")
        self.hminValue = QLineEdit(str(hmin),self)
        self.hminValue.setValidator(QIntValidator(hmin, hmax, self));
        self.hminValue.setFixedWidth(40)
        self.hminValue.setFixedHeight(27)
        self.hminValue.setAlignment(Qt.AlignCenter);
        self.hminSlider = QSlider(Qt.Vertical)
        self.hminSlider.setMinimum(hmin)
        self.hminSlider.setMaximum(hmax)
        self.hminSlider.setValue(hmin)
        self.hminLayout.addWidget(self.hminLabel, Qt.AlignCenter)
        self.hminLayout.addWidget(self.hminValue,Qt.AlignCenter)
        self.hminLayout.addWidget(self.hminSlider)
        

        ''' Vertical Layout for HMAX Slider''' 
        self.hmaxLayout = QVBoxLayout()
        self.hmaxLayout.setObjectName("hmaxLayout")
        self.hmaxLabel = QLabel("HMax")
        self.hmaxValue = QLineEdit(str(hmax),self)
        self.hmaxValue.setValidator(QIntValidator(hmin, hmax, self));
        self.hmaxValue.setFixedWidth(40)
        self.hmaxValue.setFixedHeight(27)
        self.hmaxValue.setAlignment(Qt.AlignCenter);
        self.hmaxSlider = QSlider(Qt.Vertical)
        self.hmaxSlider.setMinimum(hmin)
        self.hmaxSlider.setMaximum(hmax)
        self.hmaxSlider.setValue(hmax)
        self.hmaxLayout.addWidget(self.hmaxLabel)
        self.hmaxLayout.addWidget(self.hmaxValue)
        self.hmaxLayout.addWidget(self.hmaxSlider)

        ''' Vertical Layout for SMIN Slider'''
        self.sminLayout = QVBoxLayout()
        self.sminLayout.setObjectName("sminLayout")
        self.sminLabel = QLabel("SMin")
        self.sminValue = QLineEdit(str(smin),self)
        self.sminValue.setValidator(QIntValidator(smin, smax, self));
        self.sminValue.setFixedWidth(40)
        self.sminValue.setFixedHeight(27)
        self.sminValue.setAlignment(Qt.AlignCenter);
        self.sminSlider = QSlider(Qt.Vertical)
        self.sminSlider.setMinimum(smin)
        self.sminSlider.setMaximum(smax)
        self.sminSlider.setValue(smin)
        self.sminLayout.addWidget(self.sminLabel)
        self.sminLayout.addWidget(self.sminValue)
        self.sminLayout.addWidget(self.sminSlider)

        ''' Vertical Layout for SMAX Slider'''
        self.smaxLayout = QVBoxLayout()
        self.smaxLayout.setObjectName("smaxLayout")
        self.smaxLabel = QLabel("SMax")
        self.smaxValue = QLineEdit(str(smax),self)
        self.smaxValue.setValidator(QIntValidator(smin, smax, self));
        self.smaxValue.setFixedWidth(40)
        self.smaxValue.setFixedHeight(27)
        self.smaxValue.setAlignment(Qt.AlignCenter);
        self.smaxSlider = QSlider(Qt.Vertical)
        self.smaxSlider.setMinimum(smin)
        self.smaxSlider.setMaximum(smax)
        self.smaxSlider.setValue(smax)
        self.smaxLayout.addWidget(self.smaxLabel)
        self.smaxLayout.addWidget(self.smaxValue)
        self.smaxLayout.addWidget(self.smaxSlider)

        ''' Vertical Layout for VMIN Slider'''
        self.vminLayout = QVBoxLayout()
        self.vminLayout.setObjectName("vminLayout")
        self.vminLabel = QLabel("VMin")
        self.vminValue = QLineEdit(str(vmin),self)
        self.vminValue.setValidator(QIntValidator(vmin, vmax, self));
        self.vminValue.setFixedWidth(40)
        self.vminValue.setFixedHeight(27)
        self.vminValue.setAlignment(Qt.AlignCenter);
        self.vminSlider = QSlider(Qt.Vertical)
        self.vminSlider.setMinimum(vmin)
        self.vminSlider.setMaximum(vmax)
        self.vminSlider.setValue(vmin)
        self.vminLayout.addWidget(self.vminLabel)
        self.vminLayout.addWidget(self.vminValue)
        self.vminLayout.addWidget(self.vminSlider)

        ''' Vertical Layout for VMAX Slider'''
        self.vmaxLayout = QVBoxLayout()
        self.vmaxLayout.setObjectName("vmaxLayout")
        self.vmaxLabel = QLabel("VMax")
        self.vmaxValue = QLineEdit(str(vmax),self)
        self.vmaxValue.setValidator(QIntValidator(vmin, vmax, self));
        self.vmaxValue.setFixedWidth(40)
        self.vmaxValue.setFixedHeight(27)
        self.vmaxValue.setAlignment(Qt.AlignCenter);
        self.vmaxSlider = QSlider(Qt.Vertical)
        self.vmaxSlider.setMinimum(vmin)
        self.vmaxSlider.setMaximum(vmax)
        self.vmaxSlider.setValue(vmax)
        self.vmaxLayout.addWidget(self.vmaxLabel)
        self.vmaxLayout.addWidget(self.vmaxValue)
        self.vmaxLayout.addWidget(self.vmaxSlider)
    
        '''Adding all the vertical layouts to the main horizontal layout'''
        self.gLayout.addLayout(self.radioLayout,1,0,1,6,Qt.AlignCenter)
        self.gLayout.addLayout(self.hminLayout,2,0,Qt.AlignCenter)
        self.gLayout.addLayout(self.hmaxLayout,2,1,Qt.AlignCenter)
        self.gLayout.addLayout(self.sminLayout,2,2,Qt.AlignCenter)
        self.gLayout.addLayout(self.smaxLayout,2,3,Qt.AlignCenter)
        self.gLayout.addLayout(self.vminLayout,2,4,Qt.AlignCenter)
        self.gLayout.addLayout(self.vmaxLayout,2,5,Qt.AlignCenter)
        self.setLayout(self.gLayout)


        '''Signals for sliders value changes'''
        self.hminSlider.valueChanged.connect(self.changeHmin)     
        self.hmaxSlider.valueChanged.connect(self.changeHmax)
        self.sminSlider.valueChanged.connect(self.changeSmin)     
        self.smaxSlider.valueChanged.connect(self.changeSmax)
        self.vminSlider.valueChanged.connect(self.changeVmin)     
        self.vmaxSlider.valueChanged.connect(self.changeVmax)

        self.hminValue.textChanged.connect(self.changeHmin2)
        self.hmaxValue.textChanged.connect(self.changeHmax2)
        self.sminValue.textChanged.connect(self.changeSmin2)
        self.smaxValue.textChanged.connect(self.changeSmax2)
        self.vminValue.textChanged.connect(self.changeVmin2)
        self.vmaxValue.textChanged.connect(self.changeVmax2)
        
        

    #Show filtered image. Don't manually disable radio button, API does it for you!
    '''Methods for showing images depending on the current checked radio button'''
    def origButtonState(self):
        if self.origButton.isChecked():
            self.winParent.setFilterName('Orig')
    def updatecolordisc(self):
        self.hsvWidget.hsvUpdate.emit()      
    def showHSVWidget(self,state):
        if state == Qt.Checked:
            self.hsvWidget.show()
        else:
            self.hsvWidget.close()
    def closeHSVWidget(self):
        self.hsvCheck.setChecked(False)
    def rgbButtonState(self):
        if self.rgbButton.isChecked():
            self.winParent.setFilterName('RGB')

            rmin,gmin,bmin = RGBMIN
            rmax,gmax,bmax = RGBMAX
            rd, gd, bd = self.rgbdwn
            ru, gu, bu = self.rgbup
            self.hminLabel.setText('Rmin')
            self.hminValue.setText(str(rd))
            self.hminValue.setValidator(QIntValidator(rmin, rmax, self));
            self.hminSlider.setMinimum(rmin)
            self.hminSlider.setMaximum(rmax)
            self.hminSlider.setValue(rd)

            self.hmaxLabel.setText("RMax")
            self.hmaxValue.setText(str(ru))
            self.hmaxValue.setValidator(QIntValidator(rmin, rmax, self));
            self.hmaxSlider.setMinimum(rmin)
            self.hmaxSlider.setMaximum(rmax)
            self.hmaxSlider.setValue(ru)

            self.sminLabel.setText("GMin")
            self.sminValue.setText(str(gd))
            self.sminValue.setValidator(QIntValidator(gmin, gmax, self));
            self.sminSlider.setMinimum(gmin)
            self.sminSlider.setMaximum(gmax)
            self.sminSlider.setValue(gd)

            self.smaxLabel.setText("GMax")
            self.smaxValue.setText(str(gu))
            self.smaxValue.setValidator(QIntValidator(gmin, gmax, self));
            self.smaxSlider.setMinimum(gmin)
            self.smaxSlider.setMaximum(gmax)
            self.smaxSlider.setValue(gu)

            self.vminLabel.setText("BMin")
            self.vminValue.setText(str(bd))
            self.vminValue.setValidator(QIntValidator(bmin, bmax, self));
            self.vminSlider.setMinimum(bmin)
            self.vminSlider.setMaximum(bmax)
            self.vminSlider.setValue(bd)

            self.vmaxLabel.setText("BMax")
            self.vmaxValue.setText(str(bu))
            self.vmaxValue.setValidator(QIntValidator(bmin, bmax, self));
            self.vmaxSlider.setMinimum(bmin)
            self.vmaxSlider.setMaximum(bmax)
            self.vmaxSlider.setValue(bu)

            

    def hsvButtonState(self):
        if self.hsvButton.isChecked():
            self.winParent.setFilterName('HSV')

            hmin,smin,vmin = HSVMIN
            hmax,smax,vmax = HSVMAX

            hd, sd, vd = self.hsvdwn
            hu, su, vu = self.hsvup
            
            self.hminLabel.setText("HMin")
            self.hminValue.setText(str(hd))
            self.hminValue.setValidator(QIntValidator(hmin, hmax, self));
            self.hminSlider.setMinimum(hmin)
            self.hminSlider.setMaximum(hmax)
            self.hminSlider.setValue(hd)

            self.hmaxLabel.setText("HMax")
            self.hmaxValue.setText(str(hu))
            self.hmaxValue.setValidator(QIntValidator(hmin, hmax, self));
            self.hmaxSlider.setMinimum(hmin)
            self.hmaxSlider.setMaximum(hmax)
            self.hmaxSlider.setValue(hu)

            self.sminLabel.setText("SMin")
            self.sminValue.setText(str(sd))
            self.sminValue.setValidator(QIntValidator(smin, smax, self));
            self.sminSlider.setMinimum(smin)
            self.sminSlider.setMaximum(smax)
            self.sminSlider.setValue(sd)

            self.smaxLabel.setText("SMax")
            self.smaxValue.setText(str(su))
            self.smaxValue.setValidator(QIntValidator(smin, smax, self)); 
            self.smaxSlider.setMinimum(smin)
            self.smaxSlider.setMaximum(smax)
            self.smaxSlider.setValue(su)

            self.vminLabel.setText("VMin")
            self.vminValue.setText(str(vd))
            self.vminValue.setValidator(QIntValidator(vmin, vmax, self));
            self.vminSlider.setMinimum(vmin)
            self.vminSlider.setMaximum(vmax)
            self.vminSlider.setValue(vd)

            self.vmaxLabel.setText("VMax")
            self.vmaxValue.setText(str(vu))
            self.vmaxValue.setValidator(QIntValidator(vmin, vmax, self));
            self.vmaxSlider.setMinimum(vmin)
            self.vmaxSlider.setMaximum(vmax)
            self.vmaxSlider.setValue(vu)
            

    def yuvButtonState(self):
        if self.yuvButton.isChecked():  
            self.winParent.setFilterName('YUV')

            ymin,umin,vmin = YUVMIN
            ymax,umax,vmax = YUVMAX

            yd, ud, vd = self.yuvdwn
            yu, uu, vu = self.yuvup
            
            self.hminLabel.setText("YMin")
            self.hminValue.setText(str(yd))
            self.hminValue.setValidator(QIntValidator(ymin, ymax, self));
            self.hminSlider.setMinimum(ymin)
            self.hminSlider.setMaximum(ymax)
            self.hminSlider.setValue(yd)

            self.hmaxLabel.setText("YMax")
            self.hmaxValue.setText(str(yu))
            self.hmaxValue.setValidator(QIntValidator(ymin, ymax, self));
            self.hmaxSlider.setMinimum(ymin)
            self.hmaxSlider.setMaximum(ymax)
            self.hmaxSlider.setValue(yu)

            self.sminLabel.setText("UMin")
            self.sminValue.setText(str(ud))
            self.sminValue.setValidator(QIntValidator(umin, umax, self));
            self.sminSlider.setMinimum(umin)
            self.sminSlider.setMaximum(umax)
            self.sminSlider.setValue(ud)

            self.smaxLabel.setText("UMax")
            self.smaxValue.setText(str(uu))
            self.smaxValue.setValidator(QIntValidator(umin, umax, self));
            self.smaxSlider.setMinimum(umin)
            self.smaxSlider.setMaximum(umax)
            self.smaxSlider.setValue(uu)

            self.vminLabel.setText("VMin")
            self.vminValue.setText(str(vd))
            self.vminValue.setValidator(QIntValidator(vmin, vmax, self));
            self.vminSlider.setMinimum(vmin)
            self.vminSlider.setMaximum(vmax)
            self.vminSlider.setValue(vd)

            self.vmaxLabel.setText("VMax")
            self.vmaxValue.setText(str(vu))
            self.vmaxValue.setValidator(QIntValidator(vmin, vmax, self));
            self.vmaxSlider.setMinimum(vmin)
            self.vmaxSlider.setMaximum(vmax)
            self.vmaxSlider.setValue(vu)


    '''Methods to get the slider value and update value labels'''
    def changeHmin(self):
        value = self.hminSlider.value()
        if self.hsvButton.isChecked():
            self.hsvdwn[0] = value
        elif self.rgbButton.isChecked():
            self.rgbdwn[0] = value
        elif self.yuvButton.isChecked():
            value = self.hminSlider.value()
            self.yuvdwn[0] = value
        self.hminValue.setText(str(value))
        self.setMIN()
        self.updatecolordisc()

    def changeHmin2(self):
        v = self.hminValue.text()
        if v == None or v == "":
            value = 0
        else:
            value = int(v)
        if self.hsvButton.isChecked():
            self.hsvdwn[0] = value
        elif self.rgbButton.isChecked():
            self.rgbdwn[0] = value
        elif self.yuvButton.isChecked():
            value = self.hminSlider.value()
            self.yuvdwn[0] = value
        self.hminSlider.setValue(value)
        self.setMIN()    
        self.updatecolordisc()

    def changeHmax(self):
        value = self.hmaxSlider.value()
        if self.hsvButton.isChecked():
            self.hsvup[0] = value
        elif self.rgbButton.isChecked():
            self.rgbup[0] = value
        elif self.yuvButton.isChecked():
            self.yuvup[0] = value

        self.hmaxValue.setText(str(value))
        self.setMAX()
        self.updatecolordisc()

    def changeHmax2(self):
        v = self.hmaxValue.text()
        if v == None or v == "":
            value = 0
        else:
            value = int(v)
        if self.hsvButton.isChecked():
            self.hsvup[0] = value
        elif self.rgbButton.isChecked():
            self.rgbup[0] = value
        elif self.yuvButton.isChecked():
            value = self.hmaxSlider.value()
            self.yuvup[0] = value
        self.hmaxSlider.setValue(value)
        self.setMAX()
        self.updatecolordisc()


    def changeSmin(self):
        value = self.sminSlider.value()
        if self.hsvButton.isChecked():
            self.hsvdwn[1] = value
        elif self.rgbButton.isChecked():
            self.rgbdwn[1] = value
        elif self.yuvButton.isChecked():
            self.yuvdwn[1] = value

        self.sminValue.setText(str(value))
        self.setMIN()
        self.updatecolordisc()


    def changeSmin2(self):
        v = self.sminValue.text()
        if v == None or v == "":
            value = 0
        else:
            value = int(v)
        if self.hsvButton.isChecked():
            self.hsvdwn[1] = value
        elif self.rgbButton.isChecked():
            self.rgbdwn[1] = value
        elif self.yuvButton.isChecked():
            value = self.hminSlider.value()
            self.yuvdwn[1] = value
        self.sminSlider.setValue(value)
        self.setMIN()
        self.updatecolordisc()


    def changeSmax(self):
        value = self.smaxSlider.value()
        if self.hsvButton.isChecked():
            self.hsvup[1] = value
        elif self.rgbButton.isChecked():
            self.rgbup[1] = value
        elif self.yuvButton.isChecked():
            self.yuvup[1] = value

        self.smaxValue.setText(str(value))
        self.setMAX()
        self.updatecolordisc()


    def changeSmax2(self):
        v = self.smaxValue.text()
        if v == None or v == "":
            value = 0
        else:
            value = int(v)
        if self.hsvButton.isChecked():
            self.hsvup[1] = value
        elif self.rgbButton.isChecked():
            self.rgbup[1] = value
        elif self.yuvButton.isChecked():
            value = self.hmaxSlider.value()
            self.yuvup[1] = value
        self.smaxSlider.setValue(value)
        self.setMAX()
        self.updatecolordisc()


    def changeVmin(self):
        value = self.vminSlider.value()
        if self.hsvButton.isChecked():
            self.hsvdwn[2] = value
        elif self.rgbButton.isChecked():
            self.rgbdwn[2] = value
        elif self.yuvButton.isChecked():
            self.yuvdwn[2] = value

        self.vminValue.setText(str(value))
        self.setMIN()

    def changeVmin2(self):
        v = self.vminValue.text()
        if v == None or v == "":
            value = 0
        else:
            value = int(v)
        if self.hsvButton.isChecked():
            self.hsvdwn[2] = value
        elif self.rgbButton.isChecked():
            self.rgbdwn[2] = value
        elif self.yuvButton.isChecked():
            value = self.hminSlider.value()
            self.yuvdwn[2] = value
        self.vminSlider.setValue(value)
        self.setMIN()

    def changeVmax(self):
        value = self.vmaxSlider.value()
        if self.hsvButton.isChecked():
            self.hsvup[2] = value
        elif self.rgbButton.isChecked():
            self.rgbup[2] = value
        elif self.yuvButton.isChecked():
            self.yuvup[2] = value

        self.vmaxValue.setText(str(value))
        self.setMAX()

    def changeVmax2(self):
        v = self.vmaxValue.text()
        if v == None or v == "":
            value = 0
        else:
            value = int(v)
        if self.hsvButton.isChecked():
            self.hsvup[2] = value
        elif self.rgbButton.isChecked():
            self.rgbup[2] = value
        elif self.yuvButton.isChecked():
            value = self.hmaxSlider.value()
            self.yuvup[2] = value
        self.vmaxSlider.setValue(value)
        self.setMAX()

    def setMAX (self):

        filt = self.winParent.getFilterName()
        if self.hsvButton.isChecked():
            h, s, v = self.hsvup
            self.winParent.getCamera().getFilter(filt).setUpLimit(h,s,v)
        elif self.rgbButton.isChecked():
            h, s, v = self.rgbup
            self.winParent.getCamera().getFilter(filt).setUpLimit(h,s,v)
        elif self.yuvButton.isChecked():
            h, s, v = self.yuvup
            self.winParent.getCamera().getFilter(filt).setUpLimit(h,s,v)


    def setMIN (self):
        filt = self.winParent.getFilterName()
        if self.hsvButton.isChecked():
            h, s, v = self.hsvdwn
            self.winParent.getCamera().getFilter(filt).setDownLimit(h,s,v)
        elif self.rgbButton.isChecked():
            h, s, v = self.rgbdwn
            self.winParent.getCamera().getFilter(filt).setDownLimit(h,s,v)
        elif self.yuvButton.isChecked():
            h, s, v = self.yuvdwn
            self.winParent.getCamera().getFilter(filt).setDownLimit(h,s,v)
    '''Method for updating slider values'''
    def updateSlider(self,hue_min,hue_max,sat_min,sat_max):
        if self.hsvButton.isChecked():
            self.hsvdwn[0]=hue_min
            self.hsvup[0]=hue_max
            self.hsvdwn[1]=sat_min
            self.hsvup[1]=sat_max
            self.hminValue.setText(str(hue_min))
            self.hminSlider.setValue(hue_min)
            self.hmaxValue.setText(str(hue_max))
            self.hmaxSlider.setValue(hue_max)
            self.sminValue.setText(str(sat_min))
            self.sminSlider.setValue(sat_min)
            self.smaxValue.setText(str(sat_max))
            self.smaxSlider.setValue(sat_max)
            self.setMAX()
            self.setMIN()

    '''Close event, for finalize the program'''
    def closeEvent(self, event):
        self.winParent.closeimagesWidget()