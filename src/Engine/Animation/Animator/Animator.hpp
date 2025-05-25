#pragma once

#include <iostream>
#include <fstream>

#include <json.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <Common/Common.h>
#include <Common/IconText.h>
#include <Resource/ResourceManager.hpp>


#pragma region AnimationClip

struct AnimationClip 
{
    std::vector<sf::IntRect> frames;
    float total_duration = 0.1f;

    void addFrame(sf::IntRect rect);
    bool removeFrame(int index);
    void clearFrames();
    void flipHorizontally();
};

#pragma endregion

#pragma region Animator

class Animator 
{
public:
    using FinishedCallback = std::function<void(const std::string& animation_name)>;

    Animator() = default;
    explicit Animator(sf::Sprite& sprite);

    void addAnimation(const std::string& name, const AnimationClip& clip);
    void setAnimation(const std::string& name, bool loop = true);
    void play();
    void pause();
    void stop();

    void update(float dt);

    void onAnimationFinished(FinishedCallback callback);
    void setSprite(const sf::Sprite& sprite);
    const sf::Sprite& getSprite() const;

    void setAnimationSpeed(float speed);
    float getAnimationSpeed() const;

    void setAnimationFrame(int frameIndex);

    const std::map<std::string, AnimationClip>& getAllAnimations() const;

private:
    struct AnimationState 
    {
        std::string current_animation = "NONE";
        int         current_frame     = 0;
        float       elapsed_time      = 0.0f;
        bool        is_playing        = false;
        bool        is_looping        = true;
    };

    sf::Sprite&                          sprite;
    std::map<std::string, AnimationClip> animations;
    AnimationState                       animation_state;
    FinishedCallback                     finished_callback;
    float                                speed = 1.0f;
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
