#include "freetype-gl-cpp.h"

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
}

