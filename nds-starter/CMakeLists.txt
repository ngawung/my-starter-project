cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME "nds-starter")

set(NDSTOOL_NAME ${PROJECT_NAME} CACHE BOOL "")
set(NDSTOOL_SUBTITLE1 "Created by Melody" CACHE BOOL "")
set(NDSTOOL_SUBTITLE2 "www.melody.my.id" CACHE BOOL "")
set(NDSTOOL_NITROFS "resource" CACHE BOOL "")

# add_subdirectory(arm9)

set(NDSTOOL_ARM7 "${CMAKE_BINARY_DIR}/arm7/arm7.elf" CACHE BOOL "")
# set(NDSTOOL_ARM9 arm9 CACHE BOOL "")

project(${PROJECT_NAME})

set(PROJECT_INCLUDE
    "src")

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/**/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
# target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_LIB})

nds_create_rom(${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/arm7
    COMMAND ${DEVKITPRO}/portlibs/nds/bin/arm-none-eabi-cmake -S ${CMAKE_SOURCE_DIR}/arm7 ${CMAKE_BINARY_DIR}/arm7
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/arm7
)