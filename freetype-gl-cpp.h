#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <fontconfig/fontconfig.h>

#ifdef WITH_EIGEN
#include <eigen3/Eigen/Dense>
#endif

#include "freetype-gl/freetype-gl.h"
#include "freetype-gl/font-manager.h"
#include "freetype-gl/vertex-buffer.h"
#include "freetype-gl/text-buffer.h"
#include "freetype-gl/markup.h"
#include "freetype-gl/demos/mat4.h"

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

class Markup {
friend class FreetypeGl;
public:
    Markup();
    Markup(const std::string& font_family,
           float size,
           const vec4 &color,
           bool bold,
           bool underlined,
           bool italic,
           bool strikethrough,
           bool overline,
           FreetypeGl* freetype_gl);

    Markup(Markup&& other);
    Markup(const Markup& other) = delete;
    Markup& operator=(Markup&& other);
    Markup& operator=(const Markup& other) = delete;
    virtual ~Markup();

    markup_t description;
private:
    FreetypeGl* manager;
};

class FreetypeGlText {
public:
    template <typename... markup_text>
    explicit FreetypeGlText(const FreetypeGl* freetypeGL, const markup_text&... content) : manager(freetypeGL)
    {
        text_buffer = text_buffer_new();
        vec2 pen = {{0,0}};

        text_buffer_printf(text_buffer, &pen, content...);
        mat4_set_identity(&pose);
    }

    FreetypeGlText(FreetypeGlText&& other);
    FreetypeGlText(const FreetypeGlText& other) = delete;
    FreetypeGlText& operator=(const FreetypeGlText& other) = delete;

    virtual ~FreetypeGlText();

    inline const text_buffer_t* getTextBuffer() const { return text_buffer; }

    void render();

#ifdef WITH_EIGEN
    void setPose(const Eigen::Matrix4f& pose);
    void setPosition(const Eigen::Vector3f& position);
#endif
    inline void setPose(const mat4& p){ pose = p; }
    inline void setPosition(float x, float y, float z) { pose.m30 = x; pose.m31 = y; pose.m32 = z;}
    ftgl::mat4 pose;

private:

    const FreetypeGl* manager;
    text_buffer_t* text_buffer;
};


class FreetypeGl {
friend class Markup;
public:
    FreetypeGl(bool initialise = false);
    FreetypeGl(const FreetypeGl& other) = delete;
    FreetypeGl& operator=(const FreetypeGl& other) = delete;
    ~FreetypeGl();

    void init();

    Markup createMarkup(const std::string& font_family,
                          float size,
                          const vec4& color=FreetypeGl::COLOR_WHITE,
                          bool bold=false,
                          bool underlined=false,
                          bool italic=false,
                          bool strikethrough=false,
                          bool overline=false);

    static std::string findFont(const std::string& search_pattern);

    FreetypeGlText createText(const std::string& text, const Markup& markup);
    FreetypeGlText createText(const std::string& text, const markup_t* markup = NULL);

    /**
     * @brief renderText Renders text directly (slow but easy to use)
     * @param text
     */
    void renderText(const std::string& text);

    void renderText(const FreetypeGlText& text, bool call_pre_post=true) const;

    void updateTexture();

    void preRender() const;
    void postRender() const;

#ifdef WITH_EIGEN
    void setView(const Eigen::Matrix4f& v);
    void setProjection(const Eigen::Matrix4f& p);
#endif
    inline void setView(const ftgl::mat4& v){ view = v; }
    inline void setProjection(const ftgl::mat4& p){ projection = p; }
    inline void setView(const float* v){ memcpy(&view, v, 16 * sizeof(float)); }
    inline void setProjection(const float* p){ memcpy(&projection, p, 16 * sizeof(float)); }
    inline void setProjectionOrtho(float left,   float right,
                                   float bottom, float top,
                                   float znear,  float zfar){
        mat4_set_orthographic(&projection, left, right, bottom, top, znear, zfar);
    }

    constexpr static vec4 COLOR_BLACK  = {{0.0, 0.0, 0.0, 1.0}};
    constexpr static vec4 COLOR_WHITE  = {{1.0, 1.0, 1.0, 1.0}};
    constexpr static vec4 COLOR_RED = {{1.0, 0.0, 0.0, 1.0}};
    constexpr static vec4 COLOR_GREEN = {{0.0, 1.0, 0.0, 1.0}};
    constexpr static vec4 COLOR_BLUE = {{0.0, 0.0, 1.0, 1.0}};
    constexpr static vec4 COLOR_YELLOW = {{1.0, 1.0, 0.0, 1.0}};
    constexpr static vec4 COLOR_GREY   = {{0.5, 0.5, 0.5, 1.0}};
    constexpr static vec4 COLOR_NONE   = {{1.0, 1.0, 1.0, 0.0}};
    constexpr static mat4 identity = {{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0}};

    mat4 view;
    mat4 projection;

private:

    //TODO Make more secure and public again
//    template <typename... markup_text>
//    FreetypeGlText createTextImpl(const markup_text&... content){
//        return FreetypeGlText(this, content..., NULL);
//    }

    GLuint compileShader(const char* source, const GLenum type);
    GLuint loadShader(char* frag, char* vert);

    void addLatin1Alphabet();

    GLuint text_shader = 0;
    font_manager_t* font_manager;
    Markup default_markup;


};


}
