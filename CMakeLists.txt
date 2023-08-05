cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Medievalution)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/source/*.cpp)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(ext_libs/spdlog)
include_directories (ext_libs/spdlog/include)


include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
 SFML GIT_REPOSITORY https://github.com/SFML/SFML.git
 GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(SFML)


target_link_libraries(${PROJECT_NAME}
	sfml-graphics
	sfml-window
	sfml-system
	sfml-audio
	sfml-network
	spdlog
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/ )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/translation $<TARGET_FILE_DIR:${PROJECT_NAME}>/translation
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}/ext_libs/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll
)