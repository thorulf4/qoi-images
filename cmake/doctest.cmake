find_package(doctest 2.4.11 QUIET)
if(doctest_FOUND)
  get_target_property(DOCTEST_INCLUDE_DIR doctest::doctest INTERFACE_INCLUDE_DIRECTORIES)
  message(STATUS "Found doctest preinstalled: ${DOCTEST_INCLUDE_DIR}")
else(doctest_FOUND)
  message(STATUS "Failed to find doctest, going to make it from scratch.")
  include(FetchContent)
  set(DOCTEST_WITH_TESTS OFF CACHE BOOL "doctest tests")
  FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    UPDATE_DISCONNECTED ON
    USES_TERMINAL_DOWNLOAD ON
    USES_TERMINAL_CONFIGURE ON
    USES_TERMINAL_BUILD ON
    USES_TERMINAL_INSTALL ON
    FIND_PACKAGE_ARGS NAMES doctest
  )
  FetchContent_GetProperties(doctest)
  if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
    add_subdirectory(${doctest_SOURCE_DIR} ${doctest_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
  message(STATUS "Got doctest.")
endif(doctest_FOUND)