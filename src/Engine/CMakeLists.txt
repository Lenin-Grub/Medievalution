cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Engine)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE EDITOR_HEADERS ${PROJECT_SOURCE_DIR}/*.hpp ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.inl)
file(GLOB_RECURSE EDITOR_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${EDITOR_HEADERS} ${EDITOR_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src/Core)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${EDITOR_HEADERS} ${EDITOR_SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    sfml-graphics
    sfml-window
    sfml-system
	sfml-audio
    ImGui-SFML::ImGui-SFML
    EnTT::EnTT
    json
    spdlog
    Core
)