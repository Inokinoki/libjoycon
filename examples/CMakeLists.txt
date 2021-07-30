find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(HIDAPI hidapi-hidraw)
endif()

if(HIDAPI_FOUND)
    # Add HIDAPI
    add_executable(joycon_helloworld joycon_helloworld.c)
    include_directories(joycon_helloworld ${HIDAPI_INCLUDE_DIRS})
    target_link_libraries(joycon_helloworld joycon ${HIDAPI_LIBRARIES})

    add_executable(joycon_getcolor joycon_getcolor.c)
    include_directories(joycon_getcolor ${HIDAPI_INCLUDE_DIRS})
    target_link_libraries(joycon_getcolor joycon ${HIDAPI_LIBRARIES})

    # TODO: Add OpenGL detection
else()
    message(HIDAPI not found, examples will not be built)
endif()