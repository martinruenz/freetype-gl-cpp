#include "freetype-gl-cpp.h"

#include <cstring>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <fontconfig/fontconfig.h>

namespace ftgl {

constexpr vec4 FreetypeGl::COLOR_BLACK;
constexpr vec4 FreetypeGl::COLOR_WHITE;
constexpr vec4 FreetypeGl::COLOR_RED;
constexpr vec4 FreetypeGl::COLOR_GREEN;
constexpr vec4 FreetypeGl::COLOR_BLUE;
constexpr vec4 FreetypeGl::COLOR_YELLOW;
constexpr vec4 FreetypeGl::COLOR_GREY;
constexpr vec4 FreetypeGl::COLOR_NONE;
constexpr mat4 FreetypeGl::identity;

Markup::Markup(){
    description.font = NULL;
    manager = NULL;
}

Markup::Markup(const std::string &font_family,
               float size,
               const vec4 &color,
               bool bold,
               bool underlined,
               bool italic,
               bool strikethrough,
               bool overline,
               FreetypeGl* freetype_gl){
    //font_file = FreetypeGl::findFont(font_family);
    manager = freetype_gl;
    description.family = strdup((char*)FreetypeGl::findFont(font_family).c_str());
    description.size   = size;
    description.bold   = bold;
    description.italic = italic;
    description.spacing= 0.0;
    description.gamma  = 2.;
    description.foreground_color   = color;
    description.background_color   = FreetypeGl::COLOR_NONE;
    description.outline            = 0;
    description.outline_color      = FreetypeGl::COLOR_NONE;
    description.underline          = underlined;
    description.underline_color    = color;
    description.overline           = overline;
    description.overline_color     = color;
    description.strikethrough      = strikethrough;
    description.strikethrough_color= color;
    description.font = font_manager_get_from_markup(manager->font_manager, &description);
}

Markup::Markup(Markup &&other){
    description = other.description;
    other.description.font = NULL;
    other.description.family = NULL;
    other.manager = NULL;
}

Markup& Markup::operator =(Markup &&other){
    description = other.description;
    other.description.font = NULL;
    other.description.family = NULL;
    other.manager = NULL;
}

Markup::~Markup(){
    if(description.family != NULL)
        free(description.family);

    if(manager != NULL && description.font != NULL)
        font_manager_delete_font(manager->font_manager, description.font);
    //FIXME Remove glyphs as well!
}

//markup_text
//template <typename... markup_text>
//FreetypeGlText::FreetypeGlText(const FreetypeGl* freetypeGL, const markup_text&... content)
//    : manager(freetypeGL)
//{
//    text_buffer = text_buffer_new();
//    vec2 pen = {{0,0}};

//    text_buffer_printf(text_buffer, &pen, content...);
//    mat4_set_identity(&pose);
//}

FreetypeGlText::FreetypeGlText(FreetypeGlText&& other){
    manager = other.manager;
    text_buffer = other.text_buffer;
    pose = other.pose;
    other.manager = NULL;
    other.text_buffer = NULL;

}

FreetypeGlText::~FreetypeGlText(){
    text_buffer_delete(text_buffer);
}

void FreetypeGlText::render(){
    manager->renderText(*this);
}

#ifdef WITH_EIGEN
void FreetypeGlText::setPose(const Eigen::Matrix4f &pose){
    eigen2mat4(pose, &this->pose);
}
#endif

FreetypeGl::FreetypeGl(){
    font_manager = font_manager_new(1024, 1024, LCD_FILTERING_ON);

#ifdef WITH_FONTCONFIG
    default_markup = std::move(createMarkup(findFont("DejaVu Sans"), 32));
#else
    default_markup = createMarkup("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);
#endif

    text_shader = loadShader(shader_text_frag, shader_text_vert);

    addLatin1Alphabet();
    updateTexture();

    mat4_set_identity(&view);
    mat4_set_identity(&projection);

    float left = 0;
    float right = 640;
    float bottom = -100;
    float top = 580;
    float zfar = 1;
    float znear = -1;
    view.m00 = +2.0f/(right-left);
    view.m30 = -(right+left)/(right-left);
    view.m11 = +2.0f/(top-bottom);
    view.m31 = -(top+bottom)/(top-bottom);
    view.m22 = -2.0f/(zfar-znear);
    view.m32 = -(zfar+znear)/(zfar-znear);
    view.m33 = 1.0f;
}

FreetypeGl::~FreetypeGl(){
    glDeleteTextures(1, &font_manager->atlas->id);
    font_manager_delete(font_manager);
    glDeleteProgram(text_shader);
}

Markup FreetypeGl::createMarkup(const std::string& font_family,
                                  float size,
                                  const vec4 &color,
                                  bool bold,
                                  bool underlined,
                                  bool italic,
                                  bool strikethrough,
                                  bool overline) {
    return Markup(font_family, size, color, bold, underlined, italic, strikethrough, overline, this);
}


std::string FreetypeGl::findFont(const std::string &search_pattern) {

    // If the search pattern is a path already, return this path
#if (defined(_WIN32) || defined(_WIN64))
    if(search_pattern.find('\\') != std::string::npos) return search_pattern;
#else
    if(search_pattern.find('/') != std::string::npos) return search_pattern;
#endif

    // Otherwise (if the pattern is not a path), use fontconfig to search for font based on pattern
#ifndef WITH_FONTCONFIG
    throw std::runtime_error("Finding font " + search_pattern + " failed: Pattern was not a path and fontconfig could not be used to find font.");
#else
    auto throwError = [&search_pattern]() {
        throw std::runtime_error("FreetypeGL error -- could not find font: " + search_pattern);
    };

    std::string filename;
    FcInit();
    FcPattern *pattern = FcNameParse((const FcChar8*)search_pattern.c_str());
    FcConfigSubstitute( 0, pattern, FcMatchPattern );
    FcDefaultSubstitute( pattern );
    FcResult font_result;
    FcPattern *match = FcFontMatch(0, pattern, &font_result);
    FcPatternDestroy(pattern);
    if (!match) throwError();

    FcValue value;
    FcResult result = FcPatternGet( match, FC_FILE, 0, &value );
    if (result) throwError();

    filename = (char *)(value.u.s);
    FcPatternDestroy( match );
    return filename;
#endif
}

//FreetypeGlText FreetypeGl::createText(const std::string& text){
//    return FreetypeGlText(this, &this->default_markup, text.c_str(), NULL);
//}

FreetypeGlText FreetypeGl::createText(const std::string& text, markup_t* markup){
    if(markup == NULL) return FreetypeGlText(this, &this->default_markup.description, text.c_str(), NULL);
    return FreetypeGlText(this, markup, text.c_str(), NULL);
}

//FreetypeGlText FreetypeGl::createText(const std::string &text, const vec4 &color, bool bold, bool underlined){


//}

//template<typename... markup_text>
//FreetypeGlText FreetypeGl::createText(const markup_text&... content){
//    return FreetypeGlText(this, content...);
//}


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
    text_buffer_printf(buffer, &pen, &default_markup.description, text.c_str(), NULL);
    //updateTexture();

    glColor4f(1.00,1.00,1.00,1.00);
    glUseProgram(text_shader);

    glUniformMatrix4fv( glGetUniformLocation( text_shader, "model" ),1, 0, identity.data);
    glUniformMatrix4fv( glGetUniformLocation( text_shader, "view" ), 1, 0, view.data);
    glUniformMatrix4fv( glGetUniformLocation( text_shader, "projection" ), 1, 0, projection.data);
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

void FreetypeGl::renderText(const FreetypeGlText& text) const {

    glColor4f(1.00,1.00,1.00,1.00);
    glUseProgram(text_shader);

    glUniformMatrix4fv( glGetUniformLocation( text_shader, "model" ), 1, 0, text.pose.data);
    glUniformMatrix4fv( glGetUniformLocation( text_shader, "view" ), 1, 0, view.data);
    glUniformMatrix4fv( glGetUniformLocation( text_shader, "projection" ), 1, 0, projection.data);
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

    vertex_buffer_render( text.getTextBuffer()->buffer, GL_TRIANGLES );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glBlendColor( 0, 0, 0, 0 );
    glUseProgram( 0 );
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
        texture_font_load_glyph(default_markup.description.font, a+i);
}

// variadic template / va_list

#ifdef WITH_EIGEN
void FreetypeGl::setView(const Eigen::Matrix4f& v){
    eigen2mat4(v, &view);
}

void FreetypeGl::setProjection(const Eigen::Matrix4f& p){
    eigen2mat4(p, &projection);
}

void eigen2mat4(const Eigen::Matrix4f& src, mat4 *dst){
    memcpy(dst,
           (src.Flags & Eigen::RowMajorBit) ? src.data() : src.transpose().data(),
           16 * sizeof(float));
}
#endif


}
