project(Engine)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE ENGINE_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.inl)
file(GLOB_RECURSE ENGINE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/src/Core
    ${EXTERNAL_DIR}/imgui
    ${EXTERNAL_DIR}/imgui-sfml
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    imgui
    imgui-sfml
    EnTT::EnTT
    nlohmann_json
    spdlog
    Core
)