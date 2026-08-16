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
    Game
    FreeType
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Utility)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Game)

########################################################################
# Copy assets next to the executable
########################################################################
set(ASSET_DIRS config fonts images)

foreach(ASSET_DIR ${ASSET_DIRS})
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${ASSET_DIR}
            $<TARGET_FILE_DIR:main>/${ASSET_DIR}
        COMMENT "Copying ${ASSET_DIR} to output directory"
    )
endforeach()
