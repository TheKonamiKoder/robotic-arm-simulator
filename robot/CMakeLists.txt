add_library(robot
    coordinate.cpp
    linear_actuator.cpp
    motor.cpp
    robot_component.cpp
    robot.cpp
    segment.cpp
)

target_include_directories(robot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})