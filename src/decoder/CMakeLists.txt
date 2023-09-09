add_library(decoder decoder.cpp)
target_link_libraries(decoder PUBLIC core)
target_include_directories(decoder PUBLIC "${CMAKE_SOURCE_DIR}/include/qoi/decoder")