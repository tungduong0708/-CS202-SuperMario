cmake_minimum_required(VERSION 3.10)
project(SuperMario) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(FetchContent)

message(STATUS "Fetching raylib...")
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) 
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.zip
)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
message(STATUS "Fetching Box2D...")
FetchContent_Declare(
    box2d
    URL https://github.com/erincatto/box2d/archive/refs/tags/v2.4.2.zip
)

message(STATUS "Making raylib and Box2D available...")
FetchContent_MakeAvailable(raylib box2d)

if(WIN32)
    message(STATUS "Configuring for Windows...")
    set(RAYLIB_LIBRARIES raylib)
    set(EXTRA_LIBS opengl32 gdi32 winmm)
elseif(APPLE)
    message(STATUS "Configuring for macOS...")
    set(RAYLIB_LIBRARIES raylib)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
    find_library(CORE_VIDEO_LIBRARY CoreVideo)
    find_library(OPENGL_LIBRARY OpenGL)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${CORE_FOUNDATION_LIBRARY} ${CORE_VIDEO_LIBRARY} ${OPENGL_LIBRARY})
else()
    message(STATUS "Configuring for other platforms...")
    set(RAYLIB_LIBRARIES raylib)
    set(EXTRA_LIBS GL m pthread)
endif()

file(GLOB SOURCES src/*.cpp)
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(OUTPUT_DIR "${BIN_DIR}")

add_executable(${PROJECT_NAME} ${SOURCES}) 
target_link_libraries(${PROJECT_NAME} raylib box2d ${EXTRA_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

set_target_properties(SuperMario PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

add_custom_command(TARGET SuperMario POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_DIR}/resources"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCE_DIR}" "${OUTPUT_DIR}/resources"
)