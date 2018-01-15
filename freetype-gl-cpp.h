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

class FreetypeGl;

class FreetypeGlText {
public:
    template <typename... markup_text>
    explicit FreetypeGlText(const FreetypeGl* freetypeGL, const markup_text&... content);
    FreetypeGlText(FreetypeGlText&& other);
    FreetypeGlText(const FreetypeGlText& other) = delete;
    FreetypeGlText& operator=(const FreetypeGlText& other) = delete;

    virtual ~FreetypeGlText();

    inline const text_buffer_t* getTextBuffer() const { return text_buffer; }

    void render();

    ftgl::mat4 pose;

private:

    const FreetypeGl* manager;
    text_buffer_t* text_buffer;
};


class FreetypeGl {
public:
    FreetypeGl();
    FreetypeGl(const FreetypeGl& other) = delete;
    FreetypeGl& operator=(const FreetypeGl& other) = delete;
    ~FreetypeGl();

    FreetypeGlText createText(const std::string& text);

    template <typename... markup_text>
    FreetypeGlText createText(const markup_text&... content);


    /**
     * @brief renderText directly (slow but easy to use)
     * @param text
     */
    void renderText(const std::string& text);

    void renderText(const FreetypeGlText& text) const;
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
