cmake_minimum_required(VERSION 3.16)
project(RobotSim LANGUAGES CXX)

enable_testing()

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

set(CMAKE_CXX_STANDARD 20)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(robot)
add_subdirectory(interpreter)
add_subdirectory(robot_graphics)
add_subdirectory(robot_editor)
add_subdirectory(tests)

add_executable(robot_sim src/main.cpp)
target_compile_features(robot_sim PRIVATE cxx_std_17)
target_link_libraries(robot_sim 
    PRIVATE 
        robot
        robot_graphics
        robot_editor 
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
)