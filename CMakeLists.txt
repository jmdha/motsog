cmake_minimum_required(VERSION 3.14.0)

add_definitions(
        -D_PROJECT_NAME="jarlo"
        -D_PROJECT_AUTHOR="jamadaha"
        -D_PROJECT_VERSION="dev"
)

project(
    ${_PROJECT_NAME}
    VERSION ${_PROJECT_VERSION}
    LANGUAGES C
)

add_compile_options(-march=native -flto)

FILE(GLOB_RECURSE sources "src/*.c" "src/*.h")
# Links math.h
link_libraries(m)

include(bin/CMakeLists.txt)
include(test/CMakeLists.txt)
