cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++14")


# Build freetype-gl library
set(freetype-gl_BUILD_DEMOS OFF)
set(freetype-gl_BUILD_MAKEFONT OFF)
add_subdirectory(external/freetype-gl)


# Build example
project (freetype-gl-cpp-example)

find_package(glfw3 3.0 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

message("GLFW3_VERSION ${GLFW3_VERSION} ${GLFW3_LIBRARY_DIR} external/freetype-gl")
include_directories(${GLFW3_LIBRARY_DIR} "/home/martin/Code/freetype-gl-cpp/external/freetype-gl")
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)

set(FREETYPE_GL_HEADERS
    external/freetype-gl/freetype-gl.h
    external/freetype-gl/font-manager.h)

#set(FREETYPE_GL_SOURCES
#    external/freetype-gl/font-manager.c)

add_executable(example example/example.cpp freetype-gl-cpp.h freetype-gl-cpp.cpp ${FREETYPE_GL_HEADERS} ${FREETYPE_GL_SOURCES})
target_link_libraries(example ${OPENGL_gl_LIBRARY} ${GLFW3_LIBRARY} freetype-gl ${FREETYPE_LIBRARIES})

