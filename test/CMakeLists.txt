add_executable(test_decoder test_decoder.cpp)
target_link_libraries(test_decoder doctest::doctest decoder)
add_test(NAME decoder COMMAND test_decoder)