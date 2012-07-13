# CHECK PCL
FIND_LIBRARY(with_pcl
    NAMES pcl_surface pcl_registration pcl_segmentation pcl_filters pcl_sample_consensus pcl_io pcl_octree pcl_features pcl_kdtree pcl_common
    PATHS /usr/lib)
IF(NOT with_pcl)
    MESSAGE ("*** PCL libraries not found. Are needed to KinectServer component")
endif()
IF(with_pcl)
    MESSAGE ("*** PCL OK")
endif()


include_directories(/usr/include)
link_directories(/usr/lib)
