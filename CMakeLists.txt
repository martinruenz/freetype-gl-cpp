cmake_minimum_required (VERSION 2.6)

#set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(${GLFW3_LIBRARY_DIR})

# Build example
project (freetype-gl-cpp-example)

find_package(glfw3 3.0 REQUIRED)
find_package(OpenGL REQUIRED)

message("GLFW3_VERSION ${GLFW3_VERSION} ${GLFW3_LIBRARY_DIR}")

add_executable(example example/example.cpp freetype-gl-cpp.h)
target_link_libraries(example ${OPENGL_gl_LIBRARY} ${GLFW3_LIBRARY})

