add_executable(test_decoder test_decoder.cpp)
target_link_libraries(test_decoder doctest::doctest qoi)
add_test(NAME decoder COMMAND test_decoder)

add_executable(test_encoder test_encoder.cpp)
target_link_libraries(test_encoder doctest::doctest qoi)
add_test(NAME encoder COMMAND test_encoder)

add_executable(test_integration test_integration.cpp)
target_link_libraries(test_integration doctest::doctest qoi)
add_test(NAME integration COMMAND test_integration)