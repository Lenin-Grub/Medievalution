#pragma once
#include "../Animation/Animation.hpp"

// компоненты
struct Component_Position
{
    sf::Vector2f position;
};

struct Component_Velocity 
{
    sf::Vector2f velocity;
};

struct Control 
{
    bool up;
    bool down;
    bool left;
    bool right;
};

struct Component_Sprite 
{
    sf::Sprite sprite;
    sf::IntRect int_rect;
};

// системы
class MovementSystem 
{
public:
    static void update(entt::registry& registry, float deltaTime);
};

class SpriteUpdateSystem 
{
public:
    static void update(entt::registry& registry, Animator animator);
};


class HandleInputSystem
{
public:
    static void handleInput(entt::registry& registry);
};


class ControlSystem
{
public:
    static void controlSystem(entt::registry& registry);

};


// менеджер компонентов
class EntityManager 
{
public:
    EntityManager();
    void createEntity(sf::Vector2f pos, const sf::Sprite& sprite);
    void destroyEntity(entt::entity entity);

    void addComponent();
    void deleteComponent();
    void checkComponent(); // приватная проверка есть ли компонент чтобы не добавлять его



    void update(float delta_time, Animator animator);

    // не должно быть в сущности
    void draw(sf::RenderWindow& window);

private:
    entt::registry registry;
    sf::Texture    texture;
};
