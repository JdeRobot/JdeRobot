#include_directories(/usr/include/gearbox)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library (bgfgsegmentation STATIC bgfg_codebook.cpp bgfg_common.cpp bgfg_exp.cpp bgfg_fixed.cpp bgfg_mean.cpp bgfg_mode.cpp bgfgsegmentation.h bgmodelfactory.cpp bgmodelfactory.h)

add_library (bgfgsegmentationhsare SHARED bgfg_codebook.cpp bgfg_common.cpp bgfg_exp.cpp bgfg_fixed.cpp bgfg_mean.cpp bgfg_mode.cpp bgfgsegmentation.h bgmodelfactory.cpp bgmodelfactory.h)

set_target_properties(bgfgsegmentationhsare PROPERTIES OUTPUT_NAME bgfgsegmentation)