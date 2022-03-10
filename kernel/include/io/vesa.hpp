#pragma once
#include <type.hpp>
#include <string_view.hpp>
namespace eospp::io {
//ARGB
enum Color : int{
    WHITE  = 0x00ffffff,
    BLACK  = 0x00000000,
    RED    = 0x00ff0000,
    BLUE   = 0x000000ff,
    YELLOW = 0x00ffff00,
    ORANGE = 0x00ff8000,
    GREEN  = 0x0000ff00,
    INDIGO = 0x0000ffff,
    PURPLE = 0x008000ff
};


class Vesa {
public:
    static bool Init();

    void  ColorPrint(Color font_color,Color bg_color,estd::string_view str);
    void  PutChar(Color font_color,Color bg_color,estd::uint8_t c);
private:
    int             *screen_;
    int             screen_width_;
    int             screen_height_;
    int             width_char_size_;
    int             height_char_size_;
    int             xpos_;
    int             ypos_;

};

void printf(const char* fmt,...);


}   // namespace eospp::io