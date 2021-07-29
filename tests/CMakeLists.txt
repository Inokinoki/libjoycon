set(test_libraries joycon)

add_executable(test_packet_encode test_packet_encode.c)
add_test("Test packet encode" test_packet_encode)
target_link_libraries(test_packet_encode ${test_libraries})
