cmake_minimum_required(VERSION 2.26.4)
option(QOI_TESTING "Enable tests" ON)

project(qoi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_subdirectory(src)

if(QOI_TESTING)
    include(cmake/doctest.cmake)
    add_subdirectory(test)
endif(QOI_TESTING)