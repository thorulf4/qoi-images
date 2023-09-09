add_library(encoder STATIC encoder.cpp)
target_link_libraries(encoder PUBLIC core)
target_include_directories(encoder PUBLIC "${CMAKE_SOURCE_DIR}/include/qoi/encoder")