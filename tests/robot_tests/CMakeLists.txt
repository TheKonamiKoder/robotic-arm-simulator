add_executable(linear_actuator_tests linear_actuator_tests.cpp)
target_link_libraries(linear_actuator_tests PRIVATE test_utils)

add_executable(motor_tests motor_tests.cpp)
target_link_libraries(motor_tests PRIVATE test_utils)

set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/robot_test_files")
add_executable(robot_tests robot_tests.cpp)
target_compile_definitions(robot_tests PRIVATE TEST_DATA_DIR="${TEST_DATA_DIR}")
target_link_libraries(robot_tests PRIVATE test_utils)