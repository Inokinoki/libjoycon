set(test_libraries joycon)

add_executable(test_packet_encode test_packet_encode.c)
add_test("Test packet encode" test_packet_encode)
target_link_libraries(test_packet_encode ${test_libraries})

add_executable(test_spi_encode test_spi_encode.c)
add_test("Test SPI encode" test_spi_encode)
target_link_libraries(test_spi_encode ${test_libraries})

# TODO: Add hidapi for integration test
# TODO: Add OpenGL detection for visualization integration test
# TODO: Move the integration tests to examples
