cmake_minimum_required(VERSION 3.16)

add_library(CLI "")

target_sources(CLI
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/CLI.cpp
)

target_include_directories(CLI
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
