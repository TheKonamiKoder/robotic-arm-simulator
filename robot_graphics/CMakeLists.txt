add_library(robot_graphics robot_graphics.cpp)
target_include_directories(robot_graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(robot_graphics
    PUBLIC
        robot
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
)