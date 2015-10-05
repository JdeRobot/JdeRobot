file(GLOB ice_files "*.ice")
foreach(ice_file ${ice_files})
    get_filename_component(ice_name ${ice_file} NAME_WE)

    set(cpp_file ${ice_name}.cpp)
    set(header_file ${ice_name}.h)

    add_custom_command(OUTPUT ${cpp_file} ${header_file}
        COMMAND slice2cpp -I${CMAKE_CURRENT_SOURCE_DIR} ${ice_file}
        DEPENDS ${ice_file}
    )

    list(APPEND SOURCE_FILES ${cpp_file} ${header_file})
endforeach(ice_file)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(jderobot_interfaces SHARED ${SOURCE_FILES})
