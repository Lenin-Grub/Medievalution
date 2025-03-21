#pragma once
#include "../../Pathfinding/Node.h"

//enum class Components
//{
//    None,
//    Position,
//    Velocity,
//    Direction,
//    Control,
//    Sprite,
//    Animation,
//    Pathfindig,
//    Selectable
//};

namespace Components
{

    struct Position
    {
        sf::Vector2f position;
    };


    struct Velocity
    {
        sf::Vector2f velocity;
        float speed;
    };


    enum class Direction
    {
        None,
        Up,
        Down,
        Left,
        Right
    };

    struct Control
    {
        sf::Vector2f direction;
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
        size_t current_node_index;
    };

    struct Selectable
    {
        bool is_selected{ false };
    };
}