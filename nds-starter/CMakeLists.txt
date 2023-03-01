
cmake_minimum_required(VERSION 3.10)
project(nds-starter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NDSTOOL_NAME "NDS Starter")
set(NDSTOOL_SUBTITLE1 "Created by Melody")
set(NDSTOOL_SUBTITLE2 "www.melody.my.id")

set(PROJECT_INCLUDE
    "src")

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/**/*.cpp")

add_executable(nds-starter ${PROJECT_SOURCES})
target_include_directories(nds-starter PRIVATE ${PROJECT_INCLUDE})

nds_create_rom(nds-starter)
