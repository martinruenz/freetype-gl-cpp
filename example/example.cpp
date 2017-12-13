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

    FreetypeGl text_renderer;
    std::cout << test << std::endl;

    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        text_renderer.renderText("test");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
