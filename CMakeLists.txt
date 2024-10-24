cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Medievalution)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Сбор всех заголовочных файлов и исходных файлов
FILE(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.inl)
FILE(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Определение макроса для отладочного режима
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} resources.rc)

# Указание директории заголовочных файлов
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Группировка файлов в Solution Explorer
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

# Установка директории выходных файтов
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

# Отключение создания общих библиотек
set(BUILD_SHARED_LIBS OFF)

# Установка пути к внешним библиотекам
set(EXTERNAL_DIR "D:/External")

include(FetchContent)

# SFML
find_package(SFML 2.6.0 PATHS ${EXTERNAL_DIR}/sfml NO_DEFAULT_PATH)

if(NOT SFML_FOUND)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 2.6.0
    )
    FetchContent_MakeAvailable(SFML)
    message("SFML not found and it was downloaded")
endif()

# Spdlog
find_package(Spdlog 1.14.1 PATHS ${EXTERNAL_DIR}/spdlog NO_DEFAULT_PATH)

if(NOT Spdlog_FOUND)
    FetchContent_Declare(Spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.14.1
    )
    FetchContent_MakeAvailable(Spdlog)
    message("Spdlog not found and it was downloaded")
endif()

# ImGui
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.90.2
)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(ImGui-SFML)

# Включение внешних библиотек
add_subdirectory(${PROJECT_SOURCE_DIR}/ext_libs/json)

# Подключение библиотек к проекту
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    spdlog
    ImGui-SFML::ImGui-SFML
    json
)

# Установка стандарта C++
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Предкомпиляция заголовочных файлов
target_precompile_headers(${PROJECT_NAME} PRIVATE src/stdafx.h)

# Копирование ресурсов в директорию выходных файтов
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
