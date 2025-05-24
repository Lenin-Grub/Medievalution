#pragma once
#include "stdafx.h"
#include "Input/Input.h"
#include "Resource/ResourceManager.hpp"
#include "Localisation/Localisation.hpp"
#include "Math/Math.h"

class Common
{
public:
    static sf::Event    sfml_event;
    static sf::View     view;
    static sf::Clock    clock;
    static float        dtime;
    static sf::Vector2i mouse_pos_screen;
    static sf::Vector2i mouse_pos_window;
    static sf::Vector2i mouse_pos_grid;
    static sf::Vector2f mouse_pos_view;
    static Math         math;

    Common();
};

typedef Common common;