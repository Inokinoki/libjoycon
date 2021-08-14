set(test_libraries joycon)

add_executable(test_packet_encode test_packet_encode.c)
add_test("Test packet encode" test_packet_encode)
target_link_libraries(test_packet_encode ${test_libraries})

add_executable(test_packet_ir_encode test_packet_ir_encode.c)
add_test("Test IR packet encode" test_ir_packet_encode)
target_link_libraries(test_packet_ir_encode ${test_libraries})

add_executable(test_packet_light_encode test_packet_light_encode.c)
add_test("Test light packet encode" test_packet_light_encode)
target_link_libraries(test_packet_light_encode ${test_libraries})

add_executable(test_spi_encode test_spi_encode.c)
add_test("Test SPI encode" test_spi_encode)
target_link_libraries(test_spi_encode ${test_libraries})

add_executable(test_rumble_encode test_rumble_encode.c)
add_test("Test rumble encode" test_rumble_encode)
target_link_libraries(test_rumble_encode ${test_libraries})
