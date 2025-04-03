cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME GameEngine)

project(${PROJECT_NAME})

add_subdirectory(Core)
add_subdirectory(Editor)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)