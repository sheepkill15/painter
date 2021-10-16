cmake_minimum_required(VERSION 3.20)
project(painter)

set(CMAKE_CXX_STANDARD 20)
add_executable(painter src/main.cpp src/canvas/canvas.cpp src/canvas/canvas.h src/brush/brush.h src/brush/pencilbrush.cpp src/brush/pencilbrush.h src/brush/brush.cpp src/utils/utils.cpp src/utils/utils.h src/shapes/lineshape.cpp src/shapes/lineshape.h src/brush/linebrush.cpp src/brush/linebrush.h)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
target_link_libraries(painter sfml-graphics sfml-window sfml-system)

target_include_directories(painter PRIVATE src)
target_include_directories(painter PRIVATE include)

target_compile_options(painter PRIVATE -Wall -Wextra)

add_custom_command(TARGET painter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:painter>/res/)

add_subdirectory(third_party)