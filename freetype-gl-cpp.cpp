#include "freetype-gl-cpp.h"

#include <cstring>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace ftgl {

FreetypeGl::FreetypeGl(){
    font_manager = font_manager_new(512, 512, LCD_FILTERING_ON);
    default_markup.family = (char*)"/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    //default_markup.family = (char*)"/usr/share/fonts/truetype/roboto/hinted/Roboto-Regular.ttf";
    default_markup.size   = 32.0;
    default_markup.bold   = 0;
    default_markup.italic = 0;
    default_markup.spacing= 0.0;
    default_markup.gamma  = 2.;
    default_markup.foreground_color   = COLOR_WHITE;
    default_markup.background_color   = COLOR_NONE;
    default_markup.outline            = 0;
    default_markup.outline_color      = COLOR_NONE;
    default_markup.underline          = 0;
    default_markup.underline_color    = COLOR_WHITE;
    default_markup.overline           = 0;
    default_markup.overline_color     = COLOR_WHITE;
    default_markup.strikethrough      = 0;
    default_markup.strikethrough_color= COLOR_WHITE;
    default_markup.font = font_manager_get_from_markup(font_manager, &default_markup);

    text_shader = loadShader(shader_text_frag, shader_text_vert);

    addLatin1Alphabet();
    updateTexture();
}

FreetypeGl::~FreetypeGl(){
    glDeleteTextures(1, &font_manager->atlas->id);
    font_manager_delete(font_manager);
    glDeleteProgram(text_shader);
}


void FreetypeGl::FreetypeGl::updateTexture(){

    glDeleteTextures(1, &font_manager->atlas->id);
    glGenTextures(1, &font_manager->atlas->id);
    glBindTexture( GL_TEXTURE_2D, font_manager->atlas->id );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, font_manager->atlas->width,
                  font_manager->atlas->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                  font_manager->atlas->data );
}


void FreetypeGl::renderText(const std::string &text){

    text_buffer_t* buffer = text_buffer_new( );
    vec2 pen = {{20,20}};
    text_buffer_printf(buffer, &pen, &default_markup, text.c_str(), NULL);
    //updateTexture();

    glColor4f(1.00,1.00,1.00,1.00);
    glUseProgram(text_shader);

    float left = 0;
    float right = 640;
    float bottom = -100;
    float top = 580;
    float zfar = 1;
    float znear = -1;
    proj.m00 = +2.0f/(right-left);
    proj.m30 = -(right+left)/(right-left);
    proj.m11 = +2.0f/(top-bottom);
    proj.m31 = -(top+bottom)/(top-bottom);
    proj.m22 = -2.0f/(zfar-znear);
    proj.m32 = -(zfar+znear)/(zfar-znear);
    proj.m33 = 1.0f;

    glUniformMatrix4fv( glGetUniformLocation( text_shader, "model" ),1, 0, identity.data);
    glUniformMatrix4fv( glGetUniformLocation( text_shader, "view" ), 1, 0, identity.data);
    glUniformMatrix4fv( glGetUniformLocation( text_shader, "projection" ), 1, 0, proj.data);
    glUniform1i( glGetUniformLocation( text_shader, "tex" ), 0 );
    glUniform3f( glGetUniformLocation( text_shader, "pixel" ),
                 1.0f/font_manager->atlas->width,
                 1.0f/font_manager->atlas->height,
                 (float)font_manager->atlas->depth );

    glEnable( GL_BLEND );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, font_manager->atlas->id );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBlendColor( 1, 1, 1, 1 );

    vertex_buffer_render( buffer->buffer, GL_TRIANGLES );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glBlendColor( 0, 0, 0, 0 );
    glUseProgram( 0 );

    text_buffer_delete( buffer );
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

void FreetypeGl::addLatin1Alphabet(){
    const char a[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    for(int i=0; i<strlen(a); i++)
        texture_font_load_glyph(default_markup.font, a+i);
}

}
