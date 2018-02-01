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


//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();

//    glViewport(0, 0, 640, 480);

    glClearColor(0.40,0.40,0.45,1.00);
    FreetypeGlText text = text_renderer.createText(std::string("Static text (faster)"));

    markup_t markup = text_renderer.createMarkup("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32, FreetypeGl::COLOR_RED);
    FreetypeGlText text2 = text_renderer.createText(&markup, (char*)"Red static text");

    ftgl::mat4 rot;
    float angle = 0;
    while (!glfwWindowShouldClose(window)){
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Render text
        text_renderer.renderText("Direct render (slower, but super simple)");
        text_renderer.renderText(text);
        text_renderer.renderText(text2);

        // Animate static text
        mat4_set_rotation(&rot, angle, 0, 0, 1);
        angle += 0.1;
        rot.m03 = fmod(0.001 * angle, 0.01);
        text.setPose(rot);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
