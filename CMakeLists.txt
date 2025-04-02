cmake_minimum_required(VERSION 3.12)

project(GameEngine)

set(PROJECT_NAME GameEngine)

add_subdirectory(Core)
add_subdirectory(Editor)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)