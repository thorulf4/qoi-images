add_library(core INTERFACE)
target_include_directories(core INTERFACE  "${CMAKE_SOURCE_DIR}/include/qoi/core/")

add_subdirectory(decoder)
add_subdirectory(encoder)