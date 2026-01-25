cmake_minimum_required(VERSION 3.16)
project(RobotSim LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 20)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(robot)
add_subdirectory(tests)

add_executable(robot_sim src/main.cpp)
target_compile_features(robot_sim PRIVATE cxx_std_17)
target_link_libraries(robot_sim 
    PRIVATE 
        robot 
        SFML::Graphics
)

add_custom_target(test
    COMMAND linear_actuator_tests
    COMMAND motor_tests
    COMMAND robot_tests
)