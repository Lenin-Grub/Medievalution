cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
project(Medievalution)

if(DEFINED ENV{External})
    set(EXTERNAL_DIR $ENV{External})
    string(REPLACE "\\" "/" EXTERNAL_DIR ${EXTERNAL_DIR})
    message(STATUS "Using External directory from environment: ${EXTERNAL_DIR}")
else()
    set(EXTERNAL_DIR "D:/External")
    message(STATUS "Using default External directory: ${EXTERNAL_DIR}")
endif()

file(MAKE_DIRECTORY ${EXTERNAL_DIR})

message(STATUS "External libraries directory: ${EXTERNAL_DIR}")

function(download_repo NAME REPO TAG)
    set(REPO_DIR "${EXTERNAL_DIR}/${NAME}")
    
    if(NOT EXISTS ${REPO_DIR})
        message(STATUS "Downloading ${NAME} from ${REPO}")
        message(STATUS "Working directory: ${EXTERNAL_DIR}")
        
        execute_process(
            COMMAND git clone ${REPO} ${NAME}
            WORKING_DIRECTORY ${EXTERNAL_DIR}
            RESULT_VARIABLE GIT_RESULT
            OUTPUT_VARIABLE GIT_OUTPUT
            ERROR_VARIABLE GIT_ERROR
        )
        
        if(GIT_RESULT EQUAL 0)
            execute_process(COMMAND git checkout ${TAG} WORKING_DIRECTORY ${REPO_DIR} RESULT_VARIABLE GIT_RESULT )
        else()
            message(STATUS "Git output: ${GIT_OUTPUT}")
            message(STATUS "Git error: ${GIT_ERROR}")
        endif()
        
        if(NOT GIT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to download ${NAME}")
        endif()
    else()
        message(STATUS "${NAME} already exists at ${REPO_DIR}")
    endif()
endfunction()

download_repo(sfml https://github.com/SFML/SFML.git 2.6.x)
download_repo(spdlog https://github.com/gabime/spdlog.git v1.x)
download_repo(entt https://github.com/skypjack/entt.git master)
download_repo(imgui https://github.com/ocornut/imgui.git v1.89.9-docking)
download_repo(imgui-sfml https://github.com/SFML/imgui-sfml.git 2.6.x)

set(LIB_OUTPUT_DIR ${CMAKE_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${LIB_OUTPUT_DIR})

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

set(SFML_SOURCE_DIR "${EXTERNAL_DIR}/sfml")
if(EXISTS ${SFML_SOURCE_DIR} AND IS_DIRECTORY ${SFML_SOURCE_DIR})
    add_subdirectory(${SFML_SOURCE_DIR} ${CMAKE_BINARY_DIR}/sfml_build)
endif()

set(SPDLOG_SOURCE_DIR "${EXTERNAL_DIR}/spdlog")
if(EXISTS ${SPDLOG_SOURCE_DIR} AND IS_DIRECTORY ${SPDLOG_SOURCE_DIR})
    add_subdirectory(${SPDLOG_SOURCE_DIR} ${CMAKE_BINARY_DIR}/spdlog_build)
endif()

set(ENTT_SOURCE_DIR "${EXTERNAL_DIR}/entt")
if(EXISTS ${ENTT_SOURCE_DIR} AND IS_DIRECTORY ${ENTT_SOURCE_DIR})
    add_subdirectory(${ENTT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/entt_build)
endif()

set(IMGUI_SOURCE_DIR "${EXTERNAL_DIR}/imgui")
if(EXISTS ${IMGUI_SOURCE_DIR} AND IS_DIRECTORY ${IMGUI_SOURCE_DIR})
    set(IMGUI_SOURCES
        ${IMGUI_SOURCE_DIR}/imgui.cpp
        ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
        ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
        ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
    )
    
    set(IMGUI_FILES_EXIST TRUE)
    foreach(SRC_FILE ${IMGUI_SOURCES})
        if(NOT EXISTS ${SRC_FILE})
            message(STATUS "Missing ImGui source file: ${SRC_FILE}")
            set(IMGUI_FILES_EXIST FALSE)
        endif()
    endforeach()
    
    if(IMGUI_FILES_EXIST)
        add_library(imgui STATIC ${IMGUI_SOURCES})
        
        set_target_properties(imgui PROPERTIES
            OUTPUT_NAME "imgui"
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
            RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
        )
        
        target_include_directories(imgui PUBLIC ${IMGUI_SOURCE_DIR})
    else()
        message(FATAL_ERROR "Required ImGui source files are missing")
    endif()
endif()

set(IMGUI_SFML_SOURCE_DIR "${EXTERNAL_DIR}/imgui-sfml")
if(EXISTS ${IMGUI_SFML_SOURCE_DIR} AND IS_DIRECTORY ${IMGUI_SFML_SOURCE_DIR})
    message(STATUS "Creating ImGui-SFML library")
    
    file(GLOB IMGUI_SFML_SOURCES 
        ${IMGUI_SFML_SOURCE_DIR}/*.cpp
    )
    
    if(IMGUI_SFML_SOURCES)
        message(STATUS "ImGui-SFML sources: ${IMGUI_SFML_SOURCES}")
        
        add_library(imgui-sfml STATIC ${IMGUI_SFML_SOURCES})
        
        set_target_properties(imgui-sfml PROPERTIES
            OUTPUT_NAME "imgui-sfml"
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
            RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
        )
        
        target_include_directories(imgui-sfml PUBLIC ${IMGUI_SFML_SOURCE_DIR} ${IMGUI_SOURCE_DIR} )
        
        target_link_libraries(imgui-sfml PUBLIC
            sfml-graphics
            sfml-window
            sfml-system
            imgui
        )
        
        target_compile_definitions(imgui-sfml PUBLIC
            IMGUI_USER_CONFIG="imconfig-SFML.h"
        )
        
        add_library(ImGui-SFML::ImGui-SFML ALIAS imgui-sfml)
        
        message(STATUS "ImGui-SFML library created successfully")
    else()
        message(FATAL_ERROR "No ImGui-SFML source files found")
    endif()
endif()

set(JSON_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext_libs/json)
if(EXISTS ${JSON_SOURCE_DIR} AND IS_DIRECTORY ${JSON_SOURCE_DIR})
    add_library(nlohmann_json INTERFACE)
    target_include_directories(nlohmann_json INTERFACE ${JSON_SOURCE_DIR}/include ${JSON_SOURCE_DIR} )
    add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
endif()

add_subdirectory(src/Core)
add_subdirectory(src/Engine)
add_subdirectory(src/Audio)
add_subdirectory(src/Editor)
add_subdirectory(src/Game)

add_custom_target(copy_resources ALL COMMENT "Copying resources..." )

function(copy_directory_if_exists SOURCE_DIR TARGET_DIR)
    if(EXISTS ${SOURCE_DIR} AND IS_DIRECTORY ${SOURCE_DIR})
        add_custom_command(TARGET copy_resources POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${TARGET_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR} ${TARGET_DIR}
            COMMENT "Copying ${SOURCE_DIR} to ${TARGET_DIR}"
        )
    endif()
endfunction()

copy_directory_if_exists(${CMAKE_SOURCE_DIR}/config ${CMAKE_BINARY_DIR}/config)
copy_directory_if_exists(${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)
copy_directory_if_exists(${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders)
copy_directory_if_exists(${CMAKE_SOURCE_DIR}/translation ${CMAKE_BINARY_DIR}/translation)

if(EXISTS ${CMAKE_SOURCE_DIR}/ext_libs/openal32.dll)
    add_custom_command(TARGET copy_resources POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/ext_libs/openal32.dll 
        ${CMAKE_BINARY_DIR}/openal32.dll
        COMMENT "Copying OpenAL library"
    )
endif()

add_dependencies(Editor copy_resources)
add_dependencies(Medievalution copy_resources)

set_target_properties( Core Audio Engine PROPERTIES FOLDER "02 Engine" )
#set_target_properties( imgui imgui-sfml spdlog copy_resources sfml-system sfml-window sfml-graphics sfml-audio sfml-main sfml-network PROPERTIES FOLDER "01 Libraries" )