cmake_minimum_required(VERSION 3.16)

add_library(Game "")

target_sources(Game
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Game.cpp
)

target_include_directories(Game
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../Utility
        ${CMAKE_CURRENT_LIST_DIR}/../../lib/glm-0.9.9.8
)

target_link_libraries(Game
    glfw
)
