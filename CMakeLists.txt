cmake_minimum_required(VERSION 3.26)
option(QOI_TESTING "Enable tests" ON)

cmake_policy(SET CMP0048 NEW)
project(qoi VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_subdirectory(src)

if(QOI_TESTING)
    include(cmake/doctest.cmake)
    add_subdirectory(test)
endif(QOI_TESTING)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/QoiConfigVersion.cmake VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)

install(EXPORT QoiConfig DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Qoi/ NAMESPACE Qoi::)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QoiConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Qoi )