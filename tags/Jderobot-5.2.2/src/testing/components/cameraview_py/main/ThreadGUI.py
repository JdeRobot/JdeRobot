import threading, time
from datetime import datetime

from PyQt4 import QtGui
from PyQt4 import QtCore

time_cycle = 50;

class MiThread(threading.Thread):  
    def __init__(self, gui):
        self.gui = gui  
        threading.Thread.__init__(self)  
 
    def run(self):  

        while(True):
            
            start_time = datetime.now()
            
            self.gui.updateGUI_recieved_signal()

            finish_Time = datetime.now()
            
            dt = finish_Time - start_time
            ms = (dt.days * 24 * 60 * 60 + dt.seconds) * 1000 + dt.microseconds / 1000.0
            
            if(ms < time_cycle):
                time.sleep((time_cycle-ms) / 1000.0);
                   
          
        