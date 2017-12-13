#include "freetype-gl-cpp.h"

#include <cstring>
#include <vector>
#include <stdexcept>

namespace ftgl {

FreetypeGl::FreetypeGl(){
    font_manager = font_manager_new(512, 512, LCD_FILTERING_ON);
    default_markup = {
        family  : (char*)"/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        size    : 24.0,
        bold    : 0,
        italic  : 0,
        spacing : 0.0,
        gamma   : 2.,
        foreground_color    : COLOR_WHITE,
        background_color    : COLOR_NONE,
        outline             : 0,
        outline_color       : COLOR_NONE,
        underline           : 0,
        underline_color     : COLOR_WHITE,
        overline            : 0,
        overline_color      : COLOR_WHITE,
        strikethrough       : 0,
        strikethrough_color : COLOR_WHITE,
        font : 0
    };

    default_markup.font = font_manager_get_from_markup(font_manager, &default_markup);
    glGenTextures(1, &font_manager->atlas->id);
    glBindTexture( GL_TEXTURE_2D, font_manager->atlas->id );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, font_manager->atlas->width,
                  font_manager->atlas->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                  font_manager->atlas->data );

    loadShader(shader_text_frag, shader_text_vert);
}

GLuint FreetypeGl::compileShader(const char* source, const GLenum type){
    GLint gl_compile_status;
    GLuint handle = glCreateShader(type);
    glShaderSource(handle, 1, &source, 0);
    glCompileShader(handle);
    glGetShaderiv( handle, GL_COMPILE_STATUS, &gl_compile_status);
    if(gl_compile_status == GL_FALSE){
        GLint log_length = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> log(log_length);
        glGetShaderInfoLog(handle, log_length, &log_length, &log[0]);
        glDeleteShader(handle);
        throw std::runtime_error((char*)&log[0]);
    }
    return handle;
}


GLuint FreetypeGl::loadShader(char *frag, char *vert){
    GLuint handle = glCreateProgram( );
    GLint link_status;
    if(strlen(vert)){
        GLuint vert_shader = compileShader(vert, GL_VERTEX_SHADER);
        glAttachShader(handle, vert_shader);
        glDeleteShader(vert_shader);
    }
    if(strlen(frag)){
        GLuint frag_shader = compileShader(frag, GL_FRAGMENT_SHADER);
        glAttachShader(handle, frag_shader);
        glDeleteShader(frag_shader);
    }

    glLinkProgram( handle );

    glGetProgramiv( handle, GL_LINK_STATUS, &link_status );
    if (link_status == GL_FALSE){
        GLint log_length = 0;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> log(log_length);
        glGetProgramInfoLog(handle, log_length, &log_length, &log[0]);
        glDeleteProgram(handle);
        throw std::runtime_error((char*)&log[0]);
    }

    return handle;
}

}
