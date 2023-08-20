cmake_minimum_required(VERSION 3.16)

add_library(Utility "")

target_sources(Utility
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Shader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Texture.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ResourceManager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SpriteRenderer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TextRenderer.cpp
)

target_include_directories(Utility
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../glad
        ${CMAKE_CURRENT_LIST_DIR}/../stb_image
        ${CMAKE_CURRENT_LIST_DIR}/../lib/glm-0.9.9.8
)

target_link_libraries(Utility 
    glfw
    glad
    stb_image
    glm
    FreeType
)
