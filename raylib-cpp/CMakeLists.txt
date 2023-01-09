cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# note for future me... set project dlu sebelum find_package
set(PROJECT_NAME starter CACHE BOOL "")
set(PROJECT_VERSION 1.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif ()

if (${PLATFORM} STREQUAL "Android")
    set(ANDROID_OUTPUT "${CMAKE_BINARY_DIR}/apk")
    set(APP_LABEL "Raylib Android")
    set(APP_COMPANY_NAME "melody")
    set(APP_PRODUCT_NAME ${PROJECT_NAME})
    set(APP_PACKAGE "com.${APP_COMPANY_NAME}.${PROJECT_NAME}")
    set(APP_VERSION_CODE "1")
    set(APP_VERSION_NAME "1.0")
    set(APP_KEYSTORE_PASS "password")

    set(JAVA_HOME "/usr/lib/jvm/java-8-openjdk")
    set(ANDROID_PLATFORM "21")
    set(ANDROID_ABI "arm64-v8a")
    set(ANDROID_NDK "/opt/android-sdk/ndk/25.1.8937393")
    set(ANDROID_BUILD_TOOLS "/opt/android-sdk/build-tools/33.0.1")
    set(ANDROID_PLATFORM_PATH "/opt/android-sdk/platforms/android-${ANDROID_PLATFORM}")

    # set(CMAKE_TOOLCHAIN_FILE "${ANDROID_NDK}/build/cmake/android.toolchain.cmake")

    if(CMAKE_BUILD_TYPE MATCHES Release)
      set(ANDROID_APK_DEBUGGABLE "false")
    else()
      set(ANDROID_APK_DEBUGGABLE "true")
    endif()
endif()

##############################################################

message(STATUS "Downloading raylib library")
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# set(PLATFORM PLATFORM_ANDROID CACHE BOOL "")
FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/refs/tags/4.2.0.zip)
FetchContent_MakeAvailable(raylib)

message(STATUS "Downloading imgui library")
include(FetchContent)
FetchContent_Declare(imgui URL https://github.com/ocornut/imgui/archive/refs/tags/v1.89.1.zip)
FetchContent_MakeAvailable(imgui)
file(GLOB imgui_SOURCES "${imgui_SOURCE_DIR}/*.cpp")
if (${PLATFORM} STREQUAL "Android")
    set(imgui_SOURCES ${imgui_SOURCES}
        "${imgui_SOURCE_DIR}/backends/imgui_impl_android.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")
endif()

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
    raylib)

set(PROJECT_DEFINITION
    RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resource/")

##############################################################

if (${PLATFORM} STREQUAL "Android")
    set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
    add_library(native_app_glue STATIC "${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 ")
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    set_property(TARGET ${PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${ANDROID_OUTPUT}/lib/${ANDROID_ABI})

    set(PROJECT_INCLUDE ${PROJECT_INCLUDE} "${ANDROID_NDK}/sources/android/native_app_glue")
    set(PROJECT_LIB ${PROJECT_LIB} native_app_glue log android EGL GLESv3 OpenSLES)
    set(PROJECT_DEFINITION ${PROJECT_DEFINITION} IMGUI_IMPL_OPENGL_ES2 "ANDROID")
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

# add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARY})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_DEFINITION})

if (${PLATFORM} STREQUAL "Android")
    # create keystore
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/key.keystore)
        execute_process(COMMAND keytool -genkeypair -validity 1000 -dname "CN=${APP_COMPANY_NAME},O=Android,C=ES" -keystore ${CMAKE_SOURCE_DIR}/key.keystore -storepass ${APP_KEYSTORE_PASS} -keypass ${APP_KEYSTORE_PASS} -alias ${PROJECT_NAME}Key -keyalg RSA)
    endif()

    # generate .xml
    file(GLOB_RECURSE RES_FILES RELATIVE "${CMAKE_SOURCE_DIR}/android" "android/res/**.xml" "android/*.xml")
    foreach(FILE IN LISTS RES_FILES)
        configure_file(${CMAKE_SOURCE_DIR}/android/${FILE} ${ANDROID_OUTPUT}/config_bak/${FILE})
    endforeach()

    #configure NativeLoader.java
    configure_file(
        "${CMAKE_SOURCE_DIR}/android/NativeLoader.java"
        "${ANDROID_OUTPUT}/config_bak/NativeLoader.java")

    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        # prepare directory
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}

        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ANDROID_OUTPUT}/bin
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ANDROID_OUTPUT}/lib
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ANDROID_OUTPUT}/res
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ANDROID_OUTPUT}/src
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${ANDROID_OUTPUT}/assets

        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/bin
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/src
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/src/com
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/src/com/${APP_COMPANY_NAME}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/src/com/${APP_COMPANY_NAME}/${APP_PRODUCT_NAME}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/lib/${ANDROID_ABI}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/res
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ANDROID_OUTPUT}/obj)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # copy resource
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/android/res" "res"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/resource" "assets/resource"

        # copy config_bak
        COMMAND ${CMAKE_COMMAND} -E copy_directory "config_bak" "."
        COMMAND ${CMAKE_COMMAND} -E remove_directory "config_bak"

        COMMAND ${CMAKE_COMMAND} -E copy "${ANDROID_OUTPUT}/NativeLoader.java" "${ANDROID_OUTPUT}/src/com/${APP_COMPANY_NAME}/${APP_PRODUCT_NAME}/NativeLoader.java"

        # generate R.java
        COMMAND ${ANDROID_BUILD_TOOLS}/aapt ARGS package -f -m
            -S "res"
            -J "src"
            -M "AndroidManifest.xml"
            -I "${ANDROID_PLATFORM_PATH}/android.jar"


        # compile java to class
        COMMAND javac ARGS -verbose -source 1.7 -target 1.7
           -d "${ANDROID_OUTPUT}/obj"
           # -bootclasspath "${JAVA_HOME}/jre/lib/rt.jar"
           -classpath "${ANDROID_PLATFORM_PATH}/android.jar"
           -sourcepath "${ANDROID_OUTPUT}/src"
                       "${ANDROID_OUTPUT}/src/com/${APP_COMPANY_NAME}/${APP_PRODUCT_NAME}/R.java"
                       "${ANDROID_OUTPUT}/src/com/${APP_COMPANY_NAME}/${APP_PRODUCT_NAME}/NativeLoader.java"


        COMMAND echo ARGS "=====R======"

        # compile .class to .dex
        COMMAND ${ANDROID_BUILD_TOOLS}/d8 ARGS
            --output "${ANDROID_OUTPUT}/bin"
            --classpath "${ANDROID_PLATFORM_PATH}/android.jar"
            "${ANDROID_OUTPUT}/obj/com/${APP_COMPANY_NAME}/${APP_PRODUCT_NAME}/*.class"

        COMMAND echo ARGS "============"
        # pack apk
        COMMAND ${ANDROID_BUILD_TOOLS}/aapt ARGS package -f
            -M "AndroidManifest.xml"
            -S "res"
            -A "${ANDROID_OUTPUT}/assets"
            -I "${ANDROID_PLATFORM_PATH}/android.jar"
            -F ${ANDROID_OUTPUT}/bin/${PROJECT_NAME}.unsigned.apk
            ${ANDROID_OUTPUT}/bin

        COMMAND ${ANDROID_BUILD_TOOLS}/aapt ARGS add ${ANDROID_OUTPUT}/bin/${PROJECT_NAME}.unsigned.apk "lib/${ANDROID_ABI}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"

        # sign apk
        COMMAND jarsigner ARGS
            -keystore ${CMAKE_SOURCE_DIR}/key.keystore
            -storepass ${APP_KEYSTORE_PASS}
            -keypass ${APP_KEYSTORE_PASS}
            -signedjar ${ANDROID_OUTPUT}/bin/${PROJECT_NAME}.signed.apk
            ${ANDROID_OUTPUT}/bin/${PROJECT_NAME}.unsigned.apk ${PROJECT_NAME}Key

        COMMAND ${ANDROID_BUILD_TOOLS}/zipalign ARGS -f 4
            ${ANDROID_OUTPUT}/bin/${PROJECT_NAME}.signed.apk
            ${PROJECT_NAME}.apk

        WORKING_DIRECTORY ${ANDROID_OUTPUT})
endif()
