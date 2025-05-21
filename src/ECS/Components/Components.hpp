#pragma once
#include "../../Pathfinding/Node.h"
#include "../../Animation/Animation.hpp"

namespace Components
{

    struct Identification
    {
        std::string   name  { "Game Object" };
        std::string   group { "" };
        std::int32_t  id    { -1 };
    };

    struct Position
    {
        sf::Vector2f position { 0.0f, 0.0f };
    };


    struct Velocity
    {
        sf::Vector2f velocity { 0.0f, 0.0f };
        float        speed    { 1.0f };
    };


    enum class Direction
    {
        None = 0,
        Up,
        Down,
        Left,
        Right
    };

    struct Control
    {
        sf::Vector2f direction { 0.0f, 0.0f };
    };

    struct Sprite
    {
        sf::Image          imgae;
        sf::Texture        texture;
        sf::Sprite         sprite;
        sf::IntRect        int_rect;
        sf::RectangleShape shape { sf::Vector2f(0.f, 0.f) };
    };

    struct Pathfinding 
    {
        std::vector<Node*> path;
        Node*              start_node         = nullptr;
        Node*              end_node           = nullptr;
        size_t             current_node_index = 0;
    };

    struct Selectable
    {
        bool is_selected = false;
    };
}