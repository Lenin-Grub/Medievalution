#pragma once

#include <iostream>
#include <fstream>

#include <Common/Common.h>
#include <Resource/ResourceManager.hpp>


#pragma region AnimationClip

struct Frame 
{
    sf::IntRect rect;
    float       duration = 1.0f;
    bool        flip     = false;
    sf::Vector2f pivot = { 0.5f, 0.5f };
};

struct Animation 
{
    Animation() = default;
    Animation(const std::string& name, const std::string& texture_name = "");

    void addFrame(sf::IntRect rect, float duration = 1.0f);

    bool removeFrame(int index);

    void clearAllFrames();

    std::vector<Frame> frames;
    std::string        texture_name;
    std::string        name;
};

#pragma endregion

#pragma region Animator

class Animator 
{
public:
    using FinishedCallback = std::function<void(const std::string& animation_name)>;

    explicit Animator(sf::Sprite& sprite);

    void createNewAnimation(const std::string& name, const std::string& texture_name = "");

    void pushAnimation(const std::string& name, const Animation& animation);

    void deleteAllAnimations();

    void setAnimation(const std::string& name, bool loop = true);

    void play();

    void pause();

    void stop();

    void update(float dt);

    void onAnimationFinished(FinishedCallback callback);

    void setSprite(const sf::Sprite& sprite);

    const sf::Sprite& getSprite() const;

    void setAnimationFrame(int frameIndex);

    const std::string& getCurrentAnimationName() const;

    bool renameAnimation(const std::string& oldName, const std::string& newName);

    const std::map<std::string, Animation>& getAllAnimations() const;

    void setFlip(bool flip);

    bool getFlip() const;

private:
    struct AnimationState
    {
        std::string current_animation = "NONE";
        std::string current_texture   = "NONE";
        int         current_frame     = 0;
        float       elapsed_time      = 0.0f;
        bool        is_playing        = false;
        bool        is_looping        = true;
        bool        is_flip           = false;
    };

    sf::Sprite&                          sprite;
    std::map<std::string, Animation>     animations;
    AnimationState                       animation_state;
    FinishedCallback                     finished_callback;

    void applyFlip(const sf::IntRect& rect);
};

#pragma endregion

#pragma region AnimationLoader

class AnimationLoader 
{
public:
    static bool loadFromFile(const std::string& filename,       Animator& animator);
    static bool saveToFile  (const std::string& filename, const Animator& animator);
};

#pragma endregion
