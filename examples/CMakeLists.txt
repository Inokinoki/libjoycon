find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(HIDAPI hidapi-hidraw)
    pkg_check_modules(GL gl)
    pkg_check_modules(GLU glu)
    pkg_check_modules(GLUT glut)
endif()

if(HIDAPI_FOUND)
    # Add HIDAPI
    add_executable(joycon_helloworld joycon_helloworld.c)
    include_directories(joycon_helloworld ${HIDAPI_INCLUDE_DIRS})
    target_link_libraries(joycon_helloworld joycon ${HIDAPI_LIBRARIES})

    add_executable(joycon_getcolor joycon_getcolor.c)
    include_directories(joycon_getcolor ${HIDAPI_INCLUDE_DIRS})
    target_link_libraries(joycon_getcolor joycon ${HIDAPI_LIBRARIES})

    add_executable(joycon_playmxbc joycon_playmxbc.c)
    include_directories(joycon_playmxbc ${HIDAPI_INCLUDE_DIRS})
    target_link_libraries(joycon_playmxbc joycon ${HIDAPI_LIBRARIES})

    # Add OpenGL detection
    if(GL_FOUND AND GLUT_FOUND AND GLU_FOUND)
        message("Build IMU Mapping example based on OpenGL")
        add_executable(joycon_imumapping joycon_imumapping.c joycon_imumapping_opengl.c)
        include_directories(joycon_imumapping ${HIDAPI_INCLUDE_DIRS} ${GL_INCLUDE_DIRS} ${GLU_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
        target_link_libraries(joycon_imumapping joycon ${HIDAPI_LIBRARIES} ${GL_LIBRARIES} ${GLU_LIBRARIES} ${GLUT_LIBRARIES})
    else()
        message("Not building IMU Mapping example due to missing of OpenGL/GLUT/GLU")
    endif()

    add_executable(joycon_playerlight joycon_playerlight.c)
    include_directories(joycon_playerlight ${HIDAPI_INCLUDE_DIRS})
    target_link_libraries(joycon_playerlight joycon ${HIDAPI_LIBRARIES})
else()
    message("HIDAPI not found, examples will not be built")
endif()