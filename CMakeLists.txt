cmake_minimum_required(VERSION 3.20)
project(painter)

set(CMAKE_CXX_STANDARD 17)
add_executable(painter main.cpp)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
target_link_libraries(painter sfml-graphics sfml-window sfml-system)

add_custom_command(TARGET painter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:painter>/res/)