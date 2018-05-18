#!/usr/bin/python2
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

import sys
import config
from gui.threadGUI import ThreadGUI
import comm
from sensors.cameraFilter import CameraFilter
from gui.gui import MainWindow
from PyQt5.QtWidgets import QApplication
from PyQt5 import QtGui
import qdarkstyle




import signal

signal.signal(signal.SIGINT, signal.SIG_DFL)

if __name__ == '__main__':
    cfg = config.load(sys.argv[1])
    try:
        img_path=sys.argv[2]
    except IndexError:
        img_path=None

    #starting comm
    jdrc= comm.init(cfg, 'ColorTuner')

    cameraCli = jdrc.getCameraClient("ColorTuner.Camera")
    camera = CameraFilter(cameraCli)
    
    app = QApplication(sys.argv)
    frame = MainWindow(img_path)
    frame.setCamera(camera)
    frame.show()
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())

    t2 = ThreadGUI(frame)  
    t2.daemon=True
    t2.start()
    
    sys.exit(app.exec_()) 