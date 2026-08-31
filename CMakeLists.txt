cmake_minimum_required(VERSION 3.5)
project(patience)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glad EXCLUDE_FROM_ALL)
add_subdirectory(vendor/json EXCLUDE_FROM_ALL)
add_subdirectory(vendor/freetype EXCLUDE_FROM_ALL)

add_executable(patience WIN32)

set_target_properties(patience PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
)

target_sources(patience
  PRIVATE
    src/main.cpp
    src/game_window.cpp

    src/game/game.cpp
    src/game/game_hud.cpp
    src/game/entity_pool.cpp
    src/game/entities.cpp
    src/game/systems.cpp
    src/game/map.cpp
    src/game/player.cpp
    src/game/sign.cpp
    src/game/slime.cpp

    src/renderer/renderer.cpp
    src/renderer/shader.cpp
    src/renderer/font_atlas.cpp
    src/renderer/sprite_atlas.cpp
    src/renderer/vertex_array.cpp
    src/renderer/vertex_buffer.cpp
)

target_link_libraries(patience 
  PRIVATE 
    SDL3::SDL3
    glad
    glm::glm-header-only
    nlohmann_json::nlohmann_json
    freetype
)

target_include_directories(patience 
  PRIVATE 
    vendor/stb_image/
)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets"
)
add_dependencies(${PROJECT_NAME} copy_assets)
