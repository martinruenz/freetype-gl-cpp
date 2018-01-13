#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <fontconfig/fontconfig.h>

#include "freetype-gl.h"
#include "font-manager.h"
#include "vertex-buffer.h"
#include "text-buffer.h"
#include "markup.h"
#include "demos/shader.h"
#include "demos/mat4.h"
#include "demos/screenshot-util.h"

namespace ftgl {

constexpr char* shader_text_frag = (char*)
#include "generated/text.frag"
;

constexpr char* shader_text_vert = (char*)
#include "generated/text.vert"
;

struct FreetypeGlText {

};


class FreetypeGl {
public:
    FreetypeGl();
    FreetypeGl(const FreetypeGl& that) = delete;
    FreetypeGl& operator=(const FreetypeGl& other) = delete;
    ~FreetypeGl();

    /**
     * @brief renderText
     * @param text
     */
    void renderText(const std::string& text);

    void renderText(const FreetypeGlText& text);
    //text_buffer_t* buffer;

    void updateTexture();

    const vec4 COLOR_BLACK  = {{0.0, 0.0, 0.0, 1.0}};
    const vec4 COLOR_WHITE  = {{1.0, 1.0, 1.0, 1.0}};
    const vec4 COLOR_YELLOW = {{1.0, 1.0, 0.0, 1.0}};
    const vec4 COLOR_GREY   = {{0.5, 0.5, 0.5, 1.0}};
    const vec4 COLOR_NONE   = {{1.0, 1.0, 1.0, 0.0}};
    const mat4 identity = {{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0}};

    mat4 proj = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};

private:

    GLuint compileShader(const char* source, const GLenum type);
    GLuint loadShader(char* frag, char* vert);

    void addLatin1Alphabet();

    GLuint text_shader = 0;
    font_manager_t* font_manager;
    markup_t default_markup;


};


}
