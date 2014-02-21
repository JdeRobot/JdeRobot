add_library (progeo STATIC progeo.c progeo.h)

add_library (progeoshare SHARED progeo.c progeo.h)

set_target_properties(progeoshare PROPERTIES OUTPUT_NAME progeo)

TARGET_LINK_LIBRARIES(progeoshare ${gsl_LIBRARIES} ${xerces_LIBRARIES} ${LIBXML2_LIBRARIES})
TARGET_LINK_LIBRARIES(progeo ${gsl_LIBRARIES} ${xerces_LIBRARIES} ${LIBXML2_LIBRARIES})
