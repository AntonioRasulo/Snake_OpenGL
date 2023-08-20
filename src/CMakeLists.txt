cmake_minimum_required(VERSION 3.16)

########################################################################
# PROJECT EXE
########################################################################

add_executable(main
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_include_directories(main
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(main
    glfw
    glad
    glm
    Utility
    stb_image
    CLI
    Game
    FreeType
    #${FREETYPE_LIBRARIES}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Utility)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CLI)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Game)

#target_include_directories(main PRIVATE ${FREETYPE_INCLUDE_DIRS})
