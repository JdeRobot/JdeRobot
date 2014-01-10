# CHECK PCL
FIND_PATH( pcl_INCLUDE_DIR NAMES pcl/point_cloud.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/pcl-1.5/ include/pcl-1.6/ include/pcl-1.7/ )

#MESSAGE("PCL LIBRARY FOUND IN ${pcl_INCLUDE_DIR}")
FIND_LIBRARY(with_pcl
    NAMES pcl_surface pcl_registration pcl_segmentation pcl_filters pcl_sample_consensus pcl_io pcl_octree pcl_features pcl_kdtree pcl_common
    PATHS /usr/lib)

MESSAGE(" with_pcl PCL LIBRARY FOUND IN ${with_pcl}")

IF( pcl_INCLUDE_DIR )
    include_directories (${pcl_INCLUDE_DIR})
    FIND_LIBRARY( pcl_LIBRARY1 NAMES pcl_surface PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY2 NAMES pcl_registration PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY3 NAMES pcl_segmentation PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY4 NAMES pcl_filters PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY5 NAMES pcl_sample_consensus PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY6 NAMES pcl_io PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY7 NAMES pcl_octree PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY8 NAMES pcl_features PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY9 NAMES pcl_kdtree PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY10 NAMES pcl_common PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY11 NAMES pcl_visualization PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)

MESSAGE(" pcl_LIBRARY11 PCL LIBRARY FOUND IN ${pcl_LIBRARY11S}")

    SET (pcl_LIBRARIES ${pcl_LIBRARY1} ${pcl_LIBRARY2} ${pcl_LIBRARY3} ${pcl_LIBRARY4} ${pcl_LIBRARY5} ${pcl_LIBRARY6} ${pcl_LIBRARY7} 
                       ${pcl_LIBRARY8} ${pcl_LIBRARY9} ${pcl_LIBRARY10} ${pcl_LIBRARY11})


    IF( pcl_LIBRARIES )
		MESSAGE("PCL LIBRARY FOUND IN ${pcl_LIBRARIES}")
    ENDIF( pcl_LIBRARIES )
ENDIF(pcl_INCLUDE_DIR)



IF(NOT with_pcl)
    MESSAGE ("*** PCL libraries not found. Are needed to KinectServer component")
endif()
IF(with_pcl)
    MESSAGE ("*** PCL OK")
    SET(DEPS "${DEPS} libpcl-visualization-1.7, libpcl-surface-1.7, libpcl-registration-1.7, libpcl-segmentation-1.7, libpcl-filters-1.7, libpcl-sample-consensus-1.7, libpcl-io-1.7, libpcl-octree-1.7, libpcl-features-1.7, libpcl-kdtree-1.7, libpcl-common-1.7,")
endif()
