#pragma once

struct Component_Position
{
    sf::Vector2f position;
};

struct Component_Velocity 
{
    sf::Vector2f velocity;
};


struct Component_Sprite 
{
    sf::Sprite sprite;
};

class MovementSystem 
{
public:
    static void update(entt::registry& registry, float deltaTime);
};

class SpriteUpdateSystem 
{
public:
    static void update(entt::registry& registry);
};

class EntityManager 
{
public:
    EntityManager();
    void createEntity(sf::Vector2f pos, const std::string& texture_path);
    void destroyEntity(entt::entity entity);
    void update(float delta_time);
    void draw(sf::RenderWindow& window);

private:
    entt::registry registry;
    sf::Texture    texture;
};
