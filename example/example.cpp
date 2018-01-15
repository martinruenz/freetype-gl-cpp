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
    FreetypeGlText text = text_renderer.createText(std::string("test asdf"));

    ftgl::mat4 rot;
    mat4_set_rotation(&rot, 10.6, 0, 0, 1);
    while (!glfwWindowShouldClose(window)){
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //text_renderer.renderText("test asdfasdf asdf asdf asdf asdf asdf asdf asdf ");
        text_renderer.renderText(text);
        text.setPose(rot);

//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        glFrustum(0.0f, 640, 480, 0.0f, 0.0f, 192.0f);

//        glBegin(GL_QUAD_STRIP);
//        glColor3f(1.f, 0.f, 0.f);

//        glVertex3f(-1.f, -1.f,  1.0f);
//        glVertex3f(1.f, -1.f,   2.0f);
//        glVertex3f(-1.f, 1.f,   1.0f);
//        glVertex3f(1.f, 1.f,    1.0f);
//        glEnd();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
