#include "../freetype-gl-cpp.h"
#include <GLFW/glfw3.h>
#include <iostream>

const float window_width = 1024;
const float window_height = 768;

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(window_width, window_height, "freetype-gl-cpp example", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) throw std::runtime_error("Unable to initialise glew.");
    glClearColor(0.40,0.40,0.45,1.00);

    // Create FreetypeGl instance and provide camera view / projection matrix
    FreetypeGl text_renderer(true);
    text_renderer.setProjectionOrtho(-window_width/2, window_width/2, -window_height/2, window_height/2, -1,1);

    // Create static texts
    FreetypeGlText text = text_renderer.createText(std::string("Static text (faster)"));
    Markup markup = text_renderer.createMarkup("DejaVu Sans", 32, FreetypeGl::COLOR_RED);
    FreetypeGlText text2 = text_renderer.createText(std::string("Red static text"), markup);
    FreetypeGlText text3 = text_renderer.createText(std::string("Another red text"), markup);
    text3.setPosition(-50,-50,0.5);

    text.setScalingFactor(2);

    ftgl::mat4 rot;
    float angle = 0;
    while (!glfwWindowShouldClose(window)){
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Render text
        text_renderer.renderText("Direct render (slower, but super simple)");
        text_renderer.renderText(text);
        text_renderer.renderText(text2);
        text_renderer.renderText(text3);

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
