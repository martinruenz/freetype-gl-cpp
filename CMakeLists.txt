cmake_minimum_required (VERSION 2.6)

include(FeatureSummary)

find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.0)
find_package(GLEW)
find_package(Eigen3)
find_package(FontConfig)

set(CMAKE_CXX_FLAGS "-std=c++14")

function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

# Build freetype-gl library
option(freetype-gl_USE_VAO "Use a VAO to render a vertex_buffer instance (required for forward compatible OpenGL 3.0 contexts)" OFF)
option(freetype-gl_BUILD_DEMOS "Build the freetype-gl example programs" OFF)
option(freetype-gl_BUILD_APIDOC "Build the freetype-gl API documentation" OFF)
option(freetype-gl_BUILD_HARFBUZZ "Build the freetype-gl harfbuzz support (experimental)" OFF)
option(freetype-gl_BUILD_MAKEFONT "Build the makefont tool" OFF)
option(freetype-gl_BUILD_TESTS "Build the tests" OFF)
add_subdirectory(external/freetype-gl)

# Build freetype-gl-cpp library
#configure_file(external/freetype-gl/shaders/text.vert ../generated/test.vert)
make_includable(external/freetype-gl/shaders/text.vert generated/text.vert)
make_includable(external/freetype-gl/shaders/text.frag generated/text.frag)

# Handle features
feature_summary(WHAT ALL)

#message("====")
#get_property(.. GLOBAL PACKAGES_FOUND)
#message()
if(EIGEN3_FOUND)
    add_definitions(-DWITH_EIGEN)
endif()

if(FONTCONFIG_FOUND)
    add_definitions(-DWITH_FONTCONFIG)
endif()


# Build example
project (freetype-gl-cpp-example)

message("GLFW3_VERSION ${GLFW3_VERSION} ${GLFW3_LIBRARY_DIR}")
include_directories(${GLFW3_LIBRARY_DIR} ${GLEW_INCLUDE_PATH} ${FONTCONFIG_INCLUDE_DIR} external/freetype-gl)
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)

#set(FREETYPE_GL_HEADERS
#    external/freetype-gl/freetype-gl.h
#    external/freetype-gl/font-manager.h)

#set(FREETYPE_GL_SOURCES
#    external/freetype-gl/font-manager.c)

add_executable(example example/example.cpp freetype-gl-cpp.h freetype-gl-cpp.cpp external/freetype-gl/demos/mat4.h external/freetype-gl/demos/mat4.c)
target_link_libraries(example freetype-gl ${OPENGL_gl_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${FREETYPE_LIBRARIES} ${FONTCONFIG_LIBRARY})

