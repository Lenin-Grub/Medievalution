#pragma once
#include <Pathfinding/Node.h>
#include <Animation/Animator/Animator.hpp>

namespace Components
{

    struct Identification
    {
        std::string   name  { "Game Object" };
        std::string   group { " " };
        std::int32_t  id    { -1 };
    };

    struct Position
    {
        sf::Vector2f position { 0.0f, 0.0f };
        float        angle    { 0.0f };
    };

    struct Velocity
    {
        sf::Vector2f velocity { 0.0f, 0.0f };
        float        speed    { 1.0f };
    };

    struct Direction
    {
        sf::Vector2f direction { 0.0f, 0.0f };
    };

    struct Sprite
    {
        std::string        texture_filename{ "" };
        sf::Image          imgae;
        sf::Texture        texture;
        sf::Sprite         sprite;
        sf::IntRect        int_rect;
        sf::RectangleShape shape { sf::Vector2f(1.0f, 1.0f) };
        sf::Vector2f       scale{ 1.0f, 1.0f };
    };

    struct Pathfinding 
    {
        std::vector<Node*> path;
        Node*              start_node         = nullptr;
        Node*              end_node           = nullptr;
        size_t             current_node_index = 0;
        bool               is_moving          = false;
    };

    struct Selectable
    {
        bool is_selected = false;
    };

    struct Animation
    {
        Animator animator;

        explicit Animation(sf::Sprite& sprite)
            : animator(sprite) 
        {
        }
    };

    enum class CharacterState
    {
        NONE = 0,
        Idle,
        Walk,
        Run,
        Attack,
        Dead,
        Shoot,
        Hit,
        Taunt,
    };

    struct State
    {
        CharacterState state = CharacterState::NONE;
    };

    enum class FormationType
    {
        //NONE = 0,
        Line,
        Square,
        Circle,
    };

    struct Formation
    {
        uint32_t squad_id = 0;
        uint32_t formation_index = 0;
        sf::Vector2f formation_offset;
        bool is_leader = false;
        FormationType formation_type = FormationType::Line;
    };
}