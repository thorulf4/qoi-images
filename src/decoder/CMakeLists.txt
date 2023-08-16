add_library(decoder decoder.cpp)
target_include_directories(decoder PUBLIC "${CMAKE_SOURCE_DIR}/include/qoi/decoder")