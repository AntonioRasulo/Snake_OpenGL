cmake_minimum_required(VERSION 3.16)

add_library(FreeType "")

target_sources(FreeType
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/FreeTypeAmalgam.c
)

target_include_directories(FreeType
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
