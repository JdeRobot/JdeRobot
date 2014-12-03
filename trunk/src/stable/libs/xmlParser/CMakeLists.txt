include_directories(${SLICE_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${INTERFACES_CPP_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${LIBS_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++


include_directories( ${LIBS_DIR}/)

add_library (xmlParser STATIC xmlReaderParser.cpp  xmlReaderParser.h  xmlWriterParser.cpp  xmlWriterParser.h)
TARGET_LINK_LIBRARIES(xmlParser ${Boost_LIBRARIES})

add_library (xmlParsershare SHARED xmlReaderParser.cpp  xmlReaderParser.h  xmlWriterParser.cpp  xmlWriterParser.h)
TARGET_LINK_LIBRARIES(xmlParsershare ${Boost_LIBRARIES} ${libxmlpp_LIBRARIES} )


set_target_properties(xmlParsershare PROPERTIES OUTPUT_NAME xmlParser)
