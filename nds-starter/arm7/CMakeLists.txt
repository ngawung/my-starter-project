cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# armv5te arm9
# armv4t  arm7
set(CMAKE_SYSTEM_PROCESSOR "armv4t")

project(arm7)

set(PROJECT_INCLUDE
    "src")

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/**/*.cpp")

add_executable(arm7 ${PROJECT_SOURCES})
target_include_directories(arm7 PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(arm7 PUBLIC dswifi7 mm7)
