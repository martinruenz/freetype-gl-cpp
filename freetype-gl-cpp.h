#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <fontconfig/fontconfig.h>

#ifdef WITH_EIGEN
#include <eigen3/Eigen/Dense>
#endif

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

#ifdef WITH_EIGEN
void eigen2mat4(const Eigen::Matrix4f& src, ftgl::mat4* dst);
#endif

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

#ifdef WITH_EIGEN
    void setPose(const Eigen::Matrix4f& pose);
#endif
    inline void setPose(const mat4& p){ pose = p; }
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

    markup_t createMarkup(std::string font_family,
                          float size,
                          const vec4& color=FreetypeGl::COLOR_WHITE,
                          bool bold=false,
                          bool underlined=false,
                          bool italic=false,
                          bool strikethrough=false,
                          bool overline=false) const;

    //FreetypeGlText createText(const std::string& text);
    FreetypeGlText createText(const std::string& text, markup_t* markup = NULL);

//    FreetypeGlText createText(const std::string& text,
//                              const vec4& color = FreetypeGl::COLOR_WHITE,
//                              float size = 32,
//                              bool bold = false,
//                              bool underlined = false);

    template <typename... markup_text>
    FreetypeGlText createText(const markup_text&... content);


    /**
     * @brief renderText directly (slow but easy to use)
     * @param text
     */
    void renderText(const std::string& text);

    void renderText(const FreetypeGlText& text) const;

    void updateTexture();

#ifdef WITH_EIGEN
    void setView(const Eigen::Matrix4f& v);
    void setProjection(const Eigen::Matrix4f& p);
#endif
    inline void setView(const ftgl::mat4& v){ view = v; }
    inline void setProjection(const ftgl::mat4& p){ projection = p; }

    constexpr static vec4 COLOR_BLACK  = {{0.0, 0.0, 0.0, 1.0}};
    constexpr static vec4 COLOR_WHITE  = {{1.0, 1.0, 1.0, 1.0}};
    constexpr static vec4 COLOR_YELLOW = {{1.0, 1.0, 0.0, 1.0}};
    constexpr static vec4 COLOR_GREY   = {{0.5, 0.5, 0.5, 1.0}};
    constexpr static vec4 COLOR_NONE   = {{1.0, 1.0, 1.0, 0.0}};
    constexpr static mat4 identity = {{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0}};

    mat4 view;
    mat4 projection;

private:

    GLuint compileShader(const char* source, const GLenum type);
    GLuint loadShader(char* frag, char* vert);

    void addLatin1Alphabet();

    GLuint text_shader = 0;
    font_manager_t* font_manager;
    markup_t default_markup;


};


}
