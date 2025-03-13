#pragma once
#include "../../Pathfinding/Node.h"

/// @struct Component_Position
/// @brief Represents the position component of an entity.
/// 
/// This structure holds the position of an entity in 2D space.
struct Component_Position
{
    sf::Vector2f position;
};

/// @struct Component_Velocity
/// @brief Represents the velocity component of an entity.
///
/// This structure holds the velocity of an entity in 2D space.
struct Component_Velocity
{
    sf::Vector2f velocity;
    float speed;
};

/// @enum Direction
/// @brief Represents the possible movement directions.
///
/// This enumeration defines the directions an entity can move.
enum class Component_Direction
{
    None,
    Up,
    Down,
    Left,
    Right
};

/// @struct Control
/// @brief Represents the control input for an entity.
///
/// This structure holds the direction of control input for an entity.
struct Component_Control
{
    sf::Vector2f direction;
};

/// @struct Component_Sprite
/// @brief Represents the sprite component of an entity.
///
/// This structure holds the sprite and its bounding rectangle for rendering.
struct Component_Sprite
{
    sf::Sprite  sprite;
    sf::IntRect int_rect;
    sf::Texture texture;
};

struct Component_Animation
{
    std::map<std::string, std::vector<sf::IntRect>> animations;
};

struct Component_Path 
{
    std::vector<Node*> path;
    size_t current_node_index;
};

struct Component_Selectable
{
    bool is_selected { false };
};