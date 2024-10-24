cmake_minimum_required(VERSION 3.16)
project(json)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)

# Указание директории заголовочных файлов
add_library(${PROJECT_NAME} json.cpp)

# Указание директории заголовочных файлов
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})