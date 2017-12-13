#include "freetype-gl-cpp.h"

FreetypeGL::FreetypeGL(bool add_default_font){
        font_manager = font_manager_new(512, 512, LCD_FILTERING_ON);
}

