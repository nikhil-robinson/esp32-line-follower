include($ENV{IDF_PATH}/tools/cmake/version.cmake)

idf_component_register(SRCS src/ir_control.cpp src/motor_control.cpp src/pid_control.cpp
                       INCLUDE_DIRS "include"
                       REQUIRES "driver")