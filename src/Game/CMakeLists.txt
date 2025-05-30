cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

add_executable(${PROJECT_NAME} ${SOURCES} resources_game.rc)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")

file(GLOB_RECURSE EDITOR_HEADERS ${PROJECT_SOURCE_DIR}/*.hpp ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.inl)
file(GLOB_RECURSE EDITOR_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${EDITOR_HEADERS} ${EDITOR_SOURCES})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${EDITOR_HEADERS} ${EDITOR_SOURCES})

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

set(BUILD_SHARED_LIBS OFF)

target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${SPDLOG_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${ENTT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Audio)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    Engine
    Audio
	spdlog
    ImGui-SFML::ImGui-SFML
    EnTT::EnTT
    json
)


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/config       $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/translation  $<TARGET_FILE_DIR:${PROJECT_NAME}>/translation
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders      $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ext_libs/openal32.dll  $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll
)