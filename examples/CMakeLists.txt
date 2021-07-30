set(test_libraries joycon)

add_executable(joycon_helloworld joycon_helloworld.c)
target_link_libraries(joycon_helloworld joycon)

# TODO: Add hidapi
# TODO: Add OpenGL detection
