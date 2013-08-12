from PyQt4 import QtCore
from PyQt4 import QtGui


class GUI(QtGui.QWidget):
    def __init__(self, sensor, parent=None):

        self.sensor = sensor

        QtGui.QWidget.__init__(self, parent)

        self.InitUI();
        
    def InitUI(self):

        self.labelImage = QtGui.QLabel();
                            
        mainLayout = QtGui.QGridLayout()
        
        mainLayout.addWidget(self.labelImage, 0, 0);

        self.setLayout(mainLayout)
        
        QtCore.QObject.connect(self, QtCore.SIGNAL("aa"), self.update)

        
        #self.setImage()
        
    def updateImage(self):
        img = self.sensor.getImage()
        image = QtGui.QImage(img.data, img.shape[1], img.shape[0], img.shape[1]*img.shape[2], QtGui.QImage.Format_RGB888);
        self.labelImage.setPixmap(QtGui.QPixmap.fromImage(image))
    
    def update(self):
        self.updateImage()
        
    def updateGUI_recieved_signal(self):
        self.emit(QtCore.SIGNAL("aa"), "lol")
