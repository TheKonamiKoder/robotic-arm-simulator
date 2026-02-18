add_library(robot_editor robot_editor.cpp)
target_include_directories(robot_editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(robot_editor
    PUBLIC
        robot
        simulation_engine
        interpreter
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
)