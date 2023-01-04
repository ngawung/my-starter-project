cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

##############################################################

message(STATUS "Downloading raylib library")
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
# set(PLATFORM PLATFORM_ANDROID CACHE BOOL "")
FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/refs/tags/4.2.0.zip)
FetchContent_MakeAvailable(raylib)

##############################################################
# yes... im using glob... dont judge me....

set(PROJECT_NAME starter CACHE BOOL "")
set(PROJECT_VERSION 1.0)

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/**/*.cpp")

set(PROJECT_INCLUDE
    "src")

set(PROJECT_LIBRARY
    raylib)

set(PROJECT_DEFINITION "")

##############################################################

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARY})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_DEFINITION})

# copy/symlink resource folder

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resource resource)
endif (WIN32)

if (UNIX)
    add_custom_command(
         TARGET ${PROJECT_NAME}
         PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
         create_symlink ${CMAKE_SOURCE_DIR}/resource resource)
endif (UNIX)
