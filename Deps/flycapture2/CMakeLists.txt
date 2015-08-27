FIND_PACKAGE(FlyCapture2)

IF(FlyCapture2_FOUND)
MESSAGE(STATUS "  With FlyCapture2/Triclops  = YES")
ELSE()
MESSAGE(STATUS "  With FlyCapture2/Triclops  = NO (Point Grey SDK not found)")
ENDIF()

