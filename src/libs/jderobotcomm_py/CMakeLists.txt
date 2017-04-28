cmake_minimum_required(VERSION 2.8)

#macro configure files

macro(configure_jderobotcomm_py in)
  file(GLOB_RECURSE files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/jderobotComm/ *.${in})

  foreach(file ${files})
    string(REGEX REPLACE "\\.${in}$" "" f "${file}")

    configure_file_python(jderobotComm/${file} jderobotComm/${f})

  endforeach(file ${files})
  
endmacro()

set (roscpp_FOUND FALSE)

if(roscpp_FOUND)
    configure_jderobotcomm_py(ros.in)
ELSE()
    configure_jderobotcomm_py(only-ice.in)
endif()

add_custom_target(jderobotcomm_py ALL)
copy_to_binary_python(jderobotcomm_py jderobotComm)

install_python(jderobotComm core)