include(FindPkgConfig)
find_package(Boost COMPONENTS system filesystem REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# ubuntu

SET(DEPS "${DEPS} libboost-system1.46.1, libboost-filesystem1.46.1,")
