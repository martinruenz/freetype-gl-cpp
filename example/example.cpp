#include "../freetype-gl-cpp.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(640, 480, "GLyphs", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) throw std::runtime_error("Unable to initialise glew.");

    FreetypeGl text_renderer;

    glClearColor(0.40,0.40,0.45,1.00);
    while (!glfwWindowShouldClose(window)){
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        text_renderer.renderText("test asdfasdf asdf asdf asdf asdf asdf asdf asdf ");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
