cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# note for future me... set project dlu sebelum find_package
set(PROJECT_NAME starter CACHE BOOL "")
set(PROJECT_VERSION 1.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

##############################################################

find_package(raylib REQUIRED)

message(STATUS "Downloading imgui library")
include(FetchContent)
FetchContent_Declare(imgui URL https://github.com/ocornut/imgui/archive/refs/tags/v1.89.1.zip)
FetchContent_MakeAvailable(imgui)
file(GLOB imgui_SOURCES "${imgui_SOURCE_DIR}/*.cpp")

message(STATUS "Downloading rlImGui library")
include(FetchContent)
FetchContent_Declare(rlimgui URL https://github.com/raylib-extras/rlImGui/archive/refs/heads/main.zip)
FetchContent_MakeAvailable(rlimgui)
file(GLOB rlImGui_SOURCES ${rlimgui_SOURCE_DIR}/*.cpp)

##############################################################
# yes... im using glob... dont judge me....

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/**/*.cpp"
    ${imgui_SOURCES}
    ${rlImGui_SOURCES})

set(PROJECT_INCLUDE
    "src"
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${rlimgui_SOURCE_DIR})

set(PROJECT_LIBRARY
    ${raylib_LIBRARY})

set(PROJECT_DEFINITION "")

##############################################################

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
