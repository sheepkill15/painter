project(third_party)

target_sources(painter PRIVATE glad.c)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui/)

add_subdirectory(imgui-sfml)
target_link_libraries(painter ImGui-SFML::ImGui-SFML)