cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

FILE(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_executable(${PROJECT_NAME} ${SOURCES} resources.rc)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)



set(BUILD_SHARED_LIBS OFF)
set(EXTERNAL_DIR "D:/External")
include(FetchContent)



# SFML 2.6.0
find_package(SFML PATHS ${EXTERNAL_DIR}/sfml NO_DEFAULT_PATH)

if(SFML_FOUND)
    message(STATUS "Используется локальная версия SFML: ${SFML_VERSION}")
    add_subdirectory(${EXTERNAL_DIR}/sfml ${CMAKE_BINARY_DIR}/sfml-build)

    set(SFML_INCLUDE_DIR ${EXTERNAL_DIR}/sfml/include)
    set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system sfml-audio)
else()
    message(STATUS "SFML не найден локально, загружается из GitHub...")

    # Если SFML не найден — загружаем через FetchContent
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git 
        GIT_TAG        2.6.0
    )

    FetchContent_MakeAvailable(SFML)
endif()



# Spdlog
find_package(Spdlog 1.14.1 PATHS ${EXTERNAL_DIR}/spdlog NO_DEFAULT_PATH)

if(NOT Spdlog_FOUND)
    message(STATUS "Spdlog не найден локально, загружается из GitHub...")

    FetchContent_Declare(Spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.14.1
    )

    FetchContent_MakeAvailable(Spdlog)
else()
    message(STATUS "Используется локальная версия Spdlog: ${SFML_VERSION}")
endif()



# ImGui
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG docking
)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(ImGui-SFML)



# EnTT
find_package(Entt 3.14.0 PATHS ${EXTERNAL_DIR}/entt NO_DEFAULT_PATH)

if(NOT Entt_FOUND)
    message(STATUS "EnTT не найден локально, загружается из GitHub...")
    FetchContent_Declare(entt
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG v3.14.0
    )

    FetchContent_MakeAvailable(entt)
else()
    message(STATUS "Используется локальная версия EnTT: ${SFML_VERSION}")
endif()




add_subdirectory(${PROJECT_SOURCE_DIR}/ext_libs/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Core)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Engine)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Game)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Audio)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/Core
    ${CMAKE_SOURCE_DIR}/src/Engine
    ${CMAKE_SOURCE_DIR}/src/Game
    ${CMAKE_SOURCE_DIR}/src/Audio
)

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    spdlog
    ImGui-SFML::ImGui-SFML
    json
    EnTT::EnTT
    Core
    Audio
    Engine
    Game
)


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/config       $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/translation  $<TARGET_FILE_DIR:${PROJECT_NAME}>/translation
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders      $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ext_libs/openal32.dll  $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll

    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/config       ${CMAKE_CURRENT_BINARY_DIR}/config
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/translation  ${CMAKE_CURRENT_BINARY_DIR}/translation
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources    ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders      ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ext_libs/openal32.dll  ${CMAKE_CURRENT_BINARY_DIR}/openal32.dll
)

# Проверка наличия Doxygen
#find_package(Doxygen)

#if(DOXYGEN_FOUND)
#    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/docs)
    
    # Настройка конфигурации Doxygen
#    set(DOXYGEN_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${DOXYGEN_CONFIG} @ONLY)

    # Добавление команды для генерации документации
#    add_custom_target(doc ALL
#        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Generating API documentation with Doxygen"
#        VERBATIM)
#else()
#    message(WARNING "Doxygen is not found, documentation will not be generated.")
#endif()