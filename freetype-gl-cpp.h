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

//const std::string R"
//namespace ftgl {

//constexpr char* test = R"foo(
//#include "/home/martin/Code/freetype-gl-cpp/external/freetype-gl/shaders/text.frag"
//)foo";

constexpr char *test =
#include "generated/text.frag"
;

struct FreetypeGlText {

};


class FreetypeGl {
public:
    FreetypeGl();
    FreetypeGl(const FreetypeGl& that) = delete;
    FreetypeGl& operator=(const FreetypeGl& other) = delete;

    /**
     * @brief renderText
     * @param text
     */
    void renderText(const std::string& text){

    }

    void renderText(const FreetypeGlText& text){

    }

    const vec4 COLOR_BLACK  = {{0.0, 0.0, 0.0, 1.0}};
    const vec4 COLOR_WHITE  = {{1.0, 1.0, 1.0, 1.0}};
    const vec4 COLOR_YELLOW = {{1.0, 1.0, 0.0, 1.0}};
    const vec4 COLOR_GREY   = {{0.5, 0.5, 0.5, 1.0}};
    const vec4 COLOR_NONE   = {{1.0, 1.0, 1.0, 0.0}};

private:
    font_manager_t* font_manager;
    markup_t default_markup;
};


//}
