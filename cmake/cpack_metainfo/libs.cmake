SET(CPACK_DEBIAN_SETUP_PACKAGE_DEPENDS "")
SET(CPACK_COMPONENT_SETUP_DESCRIPTION  
"Setup package for JdeRobot.  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_COLORSPACES_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_COLORSPACES_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CONFIG_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_CONFIG_DESCRIPTION  
"Library for read Yaml config files
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CONFIG-PYTHON_PACKAGE_DEPENDS "jderobot-setup, python-yaml")
SET(CPACK_COMPONENT_CONFIG-PYTHON_DESCRIPTION  
"Library for read Yaml config files 
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_COMM_PACKAGE_DEPENDS "jderobot-types, jderobot-config, jderobot-logger, jderobot-interfaces, jderobot-colorspaces, jderobot-setup")
SET(CPACK_COMPONENT_COMM_DESCRIPTION  
"Library for communications. You can use ROS or ICE 
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_COMM-PYTHON_PACKAGE_DEPENDS "jderobot-types-python,jderobot-config-python, jderobot-setup, jderobot-zeroc-ice-python")
SET(CPACK_COMPONENT_COMM-PYTHON_DESCRIPTION  
"Library for communications. You can use ROS or ICE 
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CVBLOB_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_CVBLOB_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_CVFAST_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_CVFAST_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_DEPTHLIB_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_DEPTHLIB_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_EASYICE_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_EASYICE_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_EASYICE-PYTHON_PACKAGE_DEPENDS "jderobot-setup, jderobot-zeroc-ice-python")
SET(CPACK_COMPONENT_EASYICE-PYTHON_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")


SET(CPACK_DEBIAN_FUZZYLIB_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_FUZZYLIB_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_GEOMETRY_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_GEOMETRY_DESCRIPTION  
"Library for  
 Home page https://jderobot.org") 

SET(CPACK_DEBIAN_HANDLERS_PACKAGE_DEPENDS "jderobot-logger, jderobot-colorspaces, jderobot-setup")
SET(CPACK_COMPONENT_HANDLERS_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_IMGANALYZE_PACKAGE_DEPENDS "jderobot-progeo, jderobot-cvfast, jderobot-setup")
SET(CPACK_COMPONENT_IMGANALYZE_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_INTERFACES_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_INTERFACES_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_LOGGER_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_LOGGER_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_NS_PACKAGE_DEPENDS "jderobot-logger, jderobot-interfaces, jderobot-setup")
SET(CPACK_COMPONENT_NS_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_PARALLELICE_PACKAGE_DEPENDS "jderobot-logger, jderobot-colorspaces, jderobot-setup")
SET(CPACK_COMPONENT_PARALLELICE_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_PARALLELICE-PYTHON_PACKAGE_DEPENDS "jderobot-setup, jderobot-zeroc-ice-python")
SET(CPACK_COMPONENT_PARALLELICE-PYTHON_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

#SET(CPACK_DEBIAN_PIONEER_PACKAGE_DEPENDS "jderobot-setup")
#SET(CPACK_COMPONENT_PIONEER_DESCRIPTION  
#"Library for  
# Home page https://jderobot.org")

SET(CPACK_DEBIAN_PROGEO_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_PROGEO_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_RESOURCELOCATOR_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_RESOURCELOCATOR_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_UTIL_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_UTIL_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")

SET(CPACK_DEBIAN_XMLPARSER_PACKAGE_DEPENDS "jderobot-setup")
SET(CPACK_COMPONENT_XMLPARSER_DESCRIPTION  
"Library for  
 Home page https://jderobot.org")
