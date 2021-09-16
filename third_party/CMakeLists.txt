project(third_party)

target_sources(painter PRIVATE imgui/backends/imgui_impl_win32.cpp imgui/backends/imgui_impl_opengl3.cpp)
target_sources(painter PRIVATE imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp)

target_include_directories(painter PRIVATE imgui
        imgui/backends)

target_link_libraries(painter dwmapi)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui/)

add_subdirectory(imgui-sfml)
target_link_libraries(painter ImGui-SFML::ImGui-SFML)