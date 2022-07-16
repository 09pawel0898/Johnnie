cmake_minimum_required(VERSION 3.2)

# Project

set(PROJECT_NAME Project)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Targets

set(APP_NAME Johnnie)
set(ENGINE_NAME Engine)

# Dependencies

set(DEPENDENCIES_DIR_NAME Dependencies)
add_subdirectory(Dependencies)

# Targets Subdirectories

add_subdirectory(${ENGINE_NAME})
add_subdirectory(${APP_NAME})