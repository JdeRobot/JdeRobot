import sys
sys.path.append("../../../interfaces/python/jderobot")
import ThreadGUI,ThreadControl
import Sensor
import GUI
from PyQt4 import QtCore
from PyQt4 import QtGui





if __name__ == '__main__': 
    #main() 
    sensor = Sensor.Sensor();      
    app = QtGui.QApplication(sys.argv)


    gui = GUI.GUI(sensor)
  
    gui.show()
    
    t1 = ThreadControl.MiThread(sensor)  
    t1.start()
    
    t2 = ThreadGUI.MiThread(gui)  
    t2.start()
    sys.exit(app.exec_())
