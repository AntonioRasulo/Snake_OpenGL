cmake_minimum_required(VERSION 3.16)

########################################################################
# Project Configuration
########################################################################
project(DUMMY_PROJECT LANGUAGES C CXX VERSION 1.0.0)

message(STATUS "CMAKE_SYSTEM: " ${CMAKE_SYSTEM})
message(STATUS "CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
message(STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

########################################################################
# PROJECT LIBS
########################################################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.8)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glm-0.9.9.8)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeTypeAmalgam-master)

########################################################################
# PROJECT EXE
########################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
