#include "../stdafx.h"
#include "Common.h"

class Input;

sf::Event    common::sfml_event; 
sf::View     common::view; 
sf::Clock    common::clock;
float        common::dtime; 

sf::Vector2i common::mouse_pos_screen;
sf::Vector2i common::mouse_pos_window;
sf::Vector2f common::mouse_pos_view;

Math         common::math;

Common::Common()
{
    system("chcp 1251");
    setlocale(LC_ALL, "ru");
}