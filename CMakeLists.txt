cmake_minimum_required (VERSION 3.1)
project (gameoflife)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")
#source groups split files into the correct hierarchy in VS
source_group(src
    REGULAR_EXPRESSION ${CMAKE_SOURCE_DIR}/src/*)
source_group(src\\glh
    REGULAR_EXPRESSION ${CMAKE_SOURCE_DIR}/src/glh/*)

add_executable(gameoflife ${SOURCE_FILES})

target_include_directories(gameoflife
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${SDL2_INCLUDE_DIR}
    PRIVATE ${GLEW_INCLUDE_DIRS})

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #g++ options
    target_compile_options(gameoflife
        PRIVATE -Wall
        PRIVATE -Wextra
        PRIVATE -pedantic)
endif()

target_link_libraries(gameoflife
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARY})
