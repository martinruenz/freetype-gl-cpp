cmake_minimum_required (VERSION 3.0.2)

project (freetype-gl-cpp)
include(FeatureSummary)
include(CMakePackageConfigHelpers)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/external/freetype-gl/cmake/Modules/")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.0)
find_package(GLEW)
find_package(Eigen3)
find_package(FontConfig)

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

# Embed required freetype-gl shaders
make_includable(external/freetype-gl/shaders/text.vert generated/text.vert)
make_includable(external/freetype-gl/shaders/text.frag generated/text.frag)

# Handle features
feature_summary(WHAT ALL)
if(EIGEN3_FOUND)
    add_definitions(-DWITH_EIGEN)
    #set(WITH_EIGEN 1)
endif()
if(FONTCONFIG_FOUND)
    add_definitions(-DWITH_FONTCONFIG)
    #set(WITH_FONTCONFIG 1)
endif()

set(SOURCES freetype-gl-cpp.cpp)
set(HEADERS freetype-gl-cpp.h)
set(SOURCES_FREETYPEGL_ADDITIONAL external/freetype-gl/demos/mat4.c)

# Build freetype-gl-cpp
include_directories(${FONTCONFIG_INCLUDE_DIR} external/ external/freetype-gl external/freetype-gl/demos)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${SOURCES_FREETYPEGL_ADDITIONAL})
target_link_libraries(${PROJECT_NAME} PUBLIC freetype-gl PRIVATE ${FREETYPE_LIBRARIES} ${FONTCONFIG_LIBRARY})

# Install freetype-gl-cpp
set(INCLUDE_INSTALL_DIR include/)
set(LIB_INSTALL_DIR lib/)
install(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR}${PROJECT_NAME})
file(GLOB HEADERS_FREETYPEGL external/freetype-gl/*.h)
file(GLOB HEADERS_FREETYPEGL_DEMOS external/freetype-gl/demos/*.h)
file(GLOB GENERATED generated/*)
install(FILES ${HEADERS_FREETYPEGL} DESTINATION ${INCLUDE_INSTALL_DIR}${PROJECT_NAME}/freetype-gl)
install(FILES ${HEADERS_FREETYPEGL_DEMOS} DESTINATION ${INCLUDE_INSTALL_DIR}${PROJECT_NAME}/freetype-gl/demos)
install(FILES ${GENERATED} DESTINATION ${INCLUDE_INSTALL_DIR}${PROJECT_NAME}/generated)
install(FILES ${HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}${PROJECT_NAME})

# Generate config file
set(CMAKECONFIG_INSTALL_DIR "share/cmake/${PROJECT_NAME}" CACHE STRING "install path for cmake config file")
configure_package_config_file(Config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                              INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
                              PATH_VARS INCLUDE_INSTALL_DIR)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ConfigVersion.cmake
                                 VERSION 0.0.1
                                 COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake ${CMAKE_CURRENT_BINARY_DIR}/ConfigVersion.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

# Build example
project (freetype-gl-cpp-example)
include_directories(${GLFW3_LIBRARY_DIR} ${GLEW_INCLUDE_PATH})
add_executable(example example/example.cpp)
target_link_libraries(example freetype-gl-cpp ${OPENGL_gl_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY})
