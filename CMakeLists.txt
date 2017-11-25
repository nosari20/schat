cmake_minimum_required(VERSION 2.8)

project(schat)
include_directories(${SOURCE_DIR}/lib)

file(GLOB PROJECT_LIB
    "lib/*/*.h"
    "lib/*/*.cpp"
    "lib/*.h"
    "lib/*.cpp"
)

file(GLOB PROJECT_INCLUDE
    "include/*/*.h"
    "include/*/*.cpp"
    "include/*.h"
    "include/*.cpp"
)


add_executable(${PROJECT_NAME} "main.cpp" ${PROJECT_LIB} ${PROJECT_INCLUDE})

TARGET_LINK_LIBRARIES( ${PROJECT_NAME} pthread)
