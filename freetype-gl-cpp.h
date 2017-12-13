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

class FreetypeGL {
public:
    FreetypeGL(bool add_default_font=true);
    FreetypeGL(const FreetypeGL& that) = delete;
    FreetypeGL& operator=(const FreetypeGL& other) = delete;

    void renderText(const std::string& text){

    }
private:
    font_manager_t* font_manager;
};

//}
