cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
project(Engine)

# Сборка как статическая библиотека
add_library(${PROJECT_NAME} STATIC)

# Сборка всех файлов
file(GLOB_RECURSE EDITOR_HEADERS ${PROJECT_SOURCE_DIR}/*.hpp ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.inl)
file(GLOB_RECURSE EDITOR_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

# Добавляем все файлы в проект (включая заголовки)
target_sources(${PROJECT_NAME} PRIVATE
    ${EDITOR_HEADERS}
    ${EDITOR_SOURCES}
)

# Указываем, что заголовки доступны извне (если нужно)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/Core
)

# Группировка файлов в Solution Explorer (Visual Studio)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${EDITOR_HEADERS} ${EDITOR_SOURCES})

# Использование стандартов C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Подключение зависимостей
target_link_libraries(${PROJECT_NAME} PUBLIC 
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML::ImGui-SFML
    EnTT::EnTT
    json
    spdlog
    Core
)