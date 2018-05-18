SET(CPACK_DEBIAN_BASIC-COMPONENT_PACKAGE_DEPENDS "jderobot-config, jderobot-comm")
SET(CPACK_COMPONENT_BASIC-COMPONENT_DESCRIPTION  
"Example of tool C++ 
 Manual Page http://jderobot.org/
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_BASIC-COMPONENT-PYTHON_PACKAGE_DEPENDS "jderobot-config-python, jderobot-comm-python, python-matplotlib, python-pyqt5, python-pip, python-numpy, python-pyqt5.qtsvg")
SET(CPACK_COMPONENT_BASIC-COMPONENT-PYTHON_DESCRIPTION  
"Example of tool Python
 Manual Page http://jderobot.org/
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CAMERACALIBRATOR_PACKAGE_DEPENDS "jderobot-easyice,  jderobot-interfaces, jderobot-util, jderobot-parallelice, jderobot-colorspaces")
SET(CPACK_COMPONENT_CAMERACALIBRATOR_DESCRIPTION  
"Calibrator for extrinsics and intrinsics parameters of RGB cameras. 
 Manual page http://jderobot.org/index.php/Tools#CameraCalibrator
 Home page https://jderobot.org")

 SET(CPACK_DEBIAN_COLORTUNER-PYTHON_PACKAGE_DEPENDS "python-matplotlib, python-pyqt5, python-pip, python-numpy, python-pyqt5.qtsvg, jderobot-comm-python, jderobot-easyice-python")
SET(CPACK_COMPONENT_COLORTUNER-PYTHON_DESCRIPTION  
"Tool for color filters and pixel treatment from multiple visual sources.
 Manual page http://jderobot.org/index.php/Tools#ColorTuner_.28Python.29
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CAMVIZ_PACKAGE_DEPENDS "jderobot-config, jderobot-comm, jderobot-util, jderobot-interfaces, jderobot-resourcelocator, jderobot-colorspaces")
SET(CPACK_DEBIAN_CAMVIZ_PACKAGE_REPLACES "jderobot-cameraview")
SET(CPACK_COMPONENT_CAMVIZ_DESCRIPTION  
"Generic viewer for cameras 
 Manual Page http://jderobot.org/index.php/Tools#CamViz
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CAMVIZ-PYTHON_PACKAGE_DEPENDS "jderobot-easyice, jderobot-comm, jderobot-util, jderobot-interfaces, jderobot-resourcelocator, jderobot-colorspaces")
SET(CPACK_DEBIAN_CAMVIZ-PYTHON_PACKAGE_REPLACES "jderobot-cameraview-python")
SET(CPACK_COMPONENT_CAMVIZ-PYTHON_DESCRIPTION  
"Generic viewer for cameras in python
 Manual Page http://jderobot.org/index.php/Tools#CamViz
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_GIRAFFECLIENT_PACKAGE_DEPENDS "jderobot-easyice, jderobot-progeo, jderobot-util, jderobot-interfaces, jderobot-colorspaces")
SET(CPACK_COMPONENT_GIRAFFECLIENT_DESCRIPTION  
"Teleoperator for robotic necks and arms [deprecated] 
 Manual Page 
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CARVIZ_PACKAGE_DEPENDS "jderobot-config, jderobot-comm")
SET(CPACK_DEBIAN_CARVIZ_PACKAGE_REPLACES "jderobot-kobukiviewer")
SET(CPACK_COMPONENT_CARVIZ_DESCRIPTION  
"Teleoperator for vehicle-type robots, such as kobuki, pioneer, cars, etc. 
 Manual page http://jderobot.org/index.php/Tools#carViz
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_NAMINGSERVICE_PACKAGE_DEPENDS "jderobot-easyice")
SET(CPACK_COMPONENT_NAMINGSERVICE_DESCRIPTION  
" 
 Manual Page 
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_PANTILTTELEOP-PYTHON_PACKAGE_DEPENDS "python-matplotlib, python-pyqt5, python-pip, python-numpy, python-pyqt5.qtsvg, jderobot-comm-python, jderobot-config-python")
SET(CPACK_COMPONENT_PANTILTTELEOP-PYTHON_DESCRIPTION  
"Teleoperator for IP cameras that allow movement (i.e. Sony EVI camera)
 Manual Page http://jderobot.org/index.php/Tools#NavigatorCamera
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_NAVIGATORCAMERA_PACKAGE_DEPENDS "jderobot-easyice, jderobot-parallelice, jderobot-util, jderobot-interfaces, jderobot-resourcelocator, jderobot-colorspaces")
SET(CPACK_COMPONENT_NAVIGATORCAMERA_DESCRIPTION  
"Teleoperator for simulated flying cameras.
 Manual Page http://jderobot.org/index.php/Tools#NavigatorCamera
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_OPENCVDEMO_PACKAGE_DEPENDS "jderobot-easyice, jderobot-parallelice, jderobot-util, jderobot-interfaces, jderobot-resourcelocator, jderobot-colorspaces")
SET(CPACK_COMPONENT_OPENCVDEMO_DESCRIPTION  
"An example of a tool made for image treatment using OpenCV.
 Manual Page http://jderobot.org/index.php/Tools#NavigatorCamera
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_RECORDER2_PACKAGE_DEPENDS "jderobot-easyice, jderobot-util, jderobot-interfaces, jderobot-ns, jderobot-colorspaces")
SET(CPACK_COMPONENT_RECORDER2_DESCRIPTION  
"Tool for record logs of robots sensors and actuators.
 Manual Page http://jderobot.org/index.php/Tools#Recorder
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_REPLAYCONTROLLER_PACKAGE_DEPENDS "jderobot-easyice, jderobot-viewer, jderobot-util, jderobot-interfaces, jderobot-logger, jderobot-ns, jderobot-colorspaces")
SET(CPACK_COMPONENT_REPLAYCONTROLLER_DESCRIPTION  
"Tool for controlling the recorded logs reproduction (play, pause, stop, ...) 
 Manual Page http://jderobot.org/index.php/Tools#Replayer
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_REPLAYER2_PACKAGE_DEPENDS "jderobot-easyice, jderobot-resourcelocator, jderobot-util, jderobot-interfaces, jderobot-logger, jderobot-ns, jderobot-colorspaces")
SET(CPACK_COMPONENT_REPLAYER2_DESCRIPTION  
"Tool for replaying recorded logs with recorder2. 
 Manual Page http://jderobot.org/index.php/Tools#Replayer
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_RGBDCALIBRATOR_PACKAGE_DEPENDS "jderobot-easyice, jderobot-progeo, jderobot-geometry, jderobot-interfaces, jderobot-resourcelocator, jderobot-cvblob, jderobot-colorspaces")
SET(CPACK_COMPONENT_RGBDCALIBRATOR_DESCRIPTION  
"Calibrator for RGB cameras.
 Manual Page 
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_RGBDMANUALCALIBRATOR_PACKAGE_DEPENDS "jderobot-easyice, jderobot-progeo, jderobot-util, jderobot-interfaces, jderobot-resourcelocator, jderobot-parallelice, jderobot-colorspaces")
SET(CPACK_COMPONENT_RGBDMANUALCALIBRATOR_DESCRIPTION  
"Manual calibrator for RGBD cameras.
 Manual Page 
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_RGBDVIZ_PACKAGE_DEPENDS "jderobot-easyice, jderobot-geometry, jderobot-util, jderobot-interfaces, jderobot-parallelice, jderobot-progeo, jderobot-resourcelocator, jderobot-colorspaces")
SET(CPACK_DEBIAN_RGBDVIZ_PACKAGE_REPLACES "jderobot-rgbdviewer")
SET(CPACK_COMPONENT_RGBDVIZ_DESCRIPTION  
"Generic viewer for RGBD cameras. Included an openGL viewer for pointcloud representation. 
 Manual Page http://jderobot.org/index.php/Tools#RGBDViz
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_SCRACTCH2JDEROBOT_PACKAGE_DEPENDS "python-parse, python-termcolor, python-pyqt5, python-pip, python-numpy, python-pyqt5.qtsvg, jderobot-parallelice-python, jderobot-comm-python, kurt-jderobot")
SET(CPACK_COMPONENT_SCRATCH2JDEROBOT_DESCRIPTION  
"Tool for programing robots using scracth language and then translate it to python. 
 Manual Page http://jderobot.org/
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_UAV-VIEWER_PACKAGE_DEPENDS "jderobot-easyice, jderobot-util, jderobot-interfaces, jderobot-colorspaces")
SET(CPACK_COMPONENT_UAV-VIEWER_DESCRIPTION  
"Teleoperator for UAV drones real and simulated (Parrot ArDrone, 3DR SoloDrone, ...) 
 Manual Page http://jderobot.org/index.php/Tools#UAV_Viewer
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_UAV-VIEWER-PYTHON_PACKAGE_DEPENDS "python-matplotlib, python-pyqt5, python-pip, python-numpy, python-pyqt5.qtsvg, jderobot-parallelice-python, jderobot-easyice-python, qfi")
SET(CPACK_COMPONENT_UAV-VIEWER-PYTHON_DESCRIPTION  
"Teleoperator for UAV drones real and simulated (Parrot ArDrone, 3DR SoloDrone, ...) for python
 Manual Page http://jderobot.org/index.php/Tools#UAV_Viewer
 Home page https://jderobot.org")

 SET(CPACK_DEBIAN_VISUALSTATES-PYTHON_PACKAGE_DEPENDS "python-matplotlib, python-sysv-ipc, python-pyqt5, python-pyqt5.qsci, python-pip, python-numpy, python-pyqt5.qtsvg, jderobot-comm-python, jderobot-config-python, jderobot-comm, jderobot-config")
SET(CPACK_DEBIAN_VISUALSTATES-PYTHON_DESCRIPTION  
"VisualStates is a tool for the programming of robot behaviors using hierarchy finite state machines (C++ or python)
 Manual Page http://jderobot.org/VisualStates
 Home page http://jderobot.org/VisualStates")
