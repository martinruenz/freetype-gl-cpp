cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++14")

function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} ${content})
endfunction(make_includable)

# Build freetype-gl library
set(freetype-gl_BUILD_DEMOS OFF)
set(freetype-gl_BUILD_MAKEFONT OFF)
set(freetype-gl_BUILD_TESTS OFF)
set(freetype-gl_BUILD_APIDOC OFF)
add_subdirectory(external/freetype-gl)

# Build freetype-gl-cpp library
#configure_file(external/freetype-gl/shaders/text.vert ../generated/test.vert)
make_includable(external/freetype-gl/shaders/text.vert generated/text.vert)
make_includable(external/freetype-gl/shaders/text.frag generated/text.frag)

# Build example
project (freetype-gl-cpp-example)

find_package(glfw3 3.0 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

message("GLFW3_VERSION ${GLFW3_VERSION} ${GLFW3_LIBRARY_DIR}")
include_directories(${GLFW3_LIBRARY_DIR} external/freetype-gl)
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)

#set(FREETYPE_GL_HEADERS
#    external/freetype-gl/freetype-gl.h
#    external/freetype-gl/font-manager.h)

#set(FREETYPE_GL_SOURCES
#    external/freetype-gl/font-manager.c)

add_executable(example example/example.cpp freetype-gl-cpp.h freetype-gl-cpp.cpp)
target_link_libraries(example ${OPENGL_gl_LIBRARY} ${GLFW3_LIBRARY} freetype-gl ${FREETYPE_LIBRARIES})

