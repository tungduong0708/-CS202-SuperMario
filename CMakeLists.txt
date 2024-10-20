cmake_minimum_required(VERSION 3.10)
project(SuperMario) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(external/raylib-master)
add_subdirectory(external/box2d-2.4.2)

include_directories(include())
include_directories(${CMAKE_SOURCE_DIR}/external/box2d-2.4.2/include)
include_directories(${CMAKE_SOURCE_DIR}/external/raylib-master/src)

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
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_link_libraries(${PROJECT_NAME} raylib box2d ${EXTRA_LIBS})

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(OUTPUT_DIR "${BIN_DIR}")

set_target_properties(SuperMario PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

add_custom_command(TARGET SuperMario POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${RESOURCE_DIR}"
    "${OUTPUT_DIR}/resources"
)
