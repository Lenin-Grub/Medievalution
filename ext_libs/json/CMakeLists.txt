cmake_minimum_required(VERSION 3.16)
project(JSON)

# Указание директории заголовочных файлов
add_library(JSON INTERFACE)

# Указание директории заголовочных файлов
target_include_directories(JSON INTERFACE ${PROJECT_SOURCE_DIR})