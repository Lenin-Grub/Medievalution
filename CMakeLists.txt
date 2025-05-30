cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

include(FetchContent)

set(EXTERNAL_DIR "D:/External")

set(SFML_SOURCE_DIR "${EXTERNAL_DIR}/sfml")
if(NOT EXISTS ${SFML_SOURCE_DIR})
    message(STATUS "SFML not found at ${SFML_SOURCE_DIR}, It will be dowload form github")
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git 
        GIT_TAG        2.6.0
    )
    FetchContent_MakeAvailable(sfml)
    set(SFML_SOURCE_DIR ${SFML_SOURCE_DIR})
endif()


set(SPDLOG_SOURCE_DIR "${EXTERNAL_DIR}/spdlog")
if(NOT EXISTS ${SPDLOG_SOURCE_DIR})
    message(STATUS "spdlog not found at ${SPDLOG_SOURCE_DIR}, It will be dowload form github")
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git 
        GIT_TAG        v1.x
    )
    FetchContent_MakeAvailable(spdlog)
    set(SPDLOG_SOURCE_DIR ${SPDLOG_SOURCE_DIR})
endif()


set(ENTT_SOURCE_DIR "${EXTERNAL_DIR}/entt")
if(NOT EXISTS ${ENTT_SOURCE_DIR})
    message(STATUS "EnTT not found at ${ENTT_SOURCE_DIR}, It will be dowload form github")
    FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git 
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(entt)
    set(ENTT_SOURCE_DIR ${ENTT_SOURCE_DIR})
endif()

set(IMGUI_SOURCE_DIR "${EXTERNAL_DIR}/imgui")
if(NOT EXISTS ${IMGUI_SOURCE_DIR})
    message(STATUS "Dear ImGui not found at ${IMGUI_SOURCE_DIR}, It will be dowload form github")
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git 
        GIT_TAG        docking
    )
    FetchContent_MakeAvailable(imgui)
endif()
set(IMGUI_SOURCE_DIR ${IMGUI_SOURCE_DIR})

set(IMGUI_SFML_SOURCE_DIR "${EXTERNAL_DIR}/imgui-sfml")
if(NOT EXISTS ${IMGUI_SFML_SOURCE_DIR})
    message(STATUS "ImGui not found at ${IMGUI_SFML_SOURCE_DIR}, It will be dowload form github")
    FetchContent_Declare(
        ImGui-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG        2.6.x
    )
    FetchContent_MakeAvailable(ImGui-SFML)
    set(IMGUI_SFML_SOURCE_DIR ${IMGUI_SFML_SOURCE_DIR})
endif()

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(${SFML_SOURCE_DIR} 	${CMAKE_BINARY_DIR}/sfml_build)
add_subdirectory(${SPDLOG_SOURCE_DIR} 	${CMAKE_BINARY_DIR}/spdlog_build)
add_subdirectory(${ENTT_SOURCE_DIR} 	${CMAKE_BINARY_DIR}/entt_build)

set(IMGUI_DIR ${IMGUI_SOURCE_DIR})
add_subdirectory(${IMGUI_SFML_SOURCE_DIR} ${CMAKE_BINARY_DIR}/imgui_build)

add_subdirectory(${PROJECT_SOURCE_DIR}/ext_libs/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Core)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Engine)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Game)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Audio)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Editor)