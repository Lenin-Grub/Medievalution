#pragma once
#include "../../Pathfinding/Node.h"

namespace Components
{

    struct Position
    {
        sf::Vector2f position { 0.0f, 0.0f };
    };


    struct Velocity
    {
        sf::Vector2f velocity { 0.0f, 0.0f };
        float        speed = 1.0f;
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
        sf::Sprite  sprite;
        sf::IntRect int_rect;
        sf::Texture texture;
    };

    struct Animation
    {
        std::map<std::string, std::vector<sf::IntRect>> animations;
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