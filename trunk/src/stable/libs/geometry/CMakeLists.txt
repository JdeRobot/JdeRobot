
SET(SRC_FILES 
	math/matriz3x3.cpp  
	math/matriz4x4.cpp  
	math/plano.cpp  
	math/utils.cpp  
	math/vector2d.cpp 
	math/vector3.cpp  
	math/vector2H.cpp 
	math/vector3H.cpp 
	math/Line2D.cpp 
	math/Segment2D.cpp 
	math/Point2D.cpp 
	math/Point3D.cpp 
	math/Line3D.cpp 
	math/Plane3D.cpp 
	math/Segment3D.cpp 
	collada/colladaparser.cpp 
	collada/color.cpp 
	collada/malla.cpp 
	collada/material.cpp 
	collada/submalla.cpp 
	progeo/Progeo.cpp 
	math/segmento.cpp 
	math/recta.cpp) 

ADD_LIBRARY (geometryshare SHARED ${SRC_FILES})

SET_TARGET_PROPERTIES (geometryshare PROPERTIES OUTPUT_NAME geometry)

TARGET_LINK_LIBRARIES(geometryshare 
	${gsl_LIBRARIES}
	${Boost_LIBRARIES}
	${OpenCV_LIBRARIES}
	${LIBXML2_LIBRARIES}
        ${LIBTINYXML_LIBRARIES}
	${OPENGL_LIBRARIES} 
	${GLUT_LIBRARY}	
)

