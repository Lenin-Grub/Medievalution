#include "Animator.hpp"



#pragma region AnimationClip

void AnimationClip::addFrame(sf::IntRect rect)
{
    frames.push_back(rect);
    total_duration = static_cast<float>(frames.size()) * 0.1f;
}

bool AnimationClip::removeFrame(int index)
{
    if (index >= 0 && index < frames.size())
    {
        frames.erase(frames.begin() + index);
        total_duration = static_cast<float>(frames.size()) * 0.1f;
        return true;
    }
    return false;
}

void AnimationClip::clearFrames()
{
    frames.clear();
    total_duration = 0.0f;
}

void AnimationClip::flipHorizontally()
{
    for (auto& frame : frames)
    {
        frame.left += frame.width;
        frame.width *= -1;
    }
}

#pragma endregion



#pragma region Animator

Animator::Animator(sf::Sprite& sprite)
    : sprite(sprite)
{
}

void Animator::addAnimation(const std::string& name, const AnimationClip& clip)
{
    if (!clip.frames.empty()) 
        animations[name] = clip;
}

void Animator::setAnimation(const std::string& name, bool loop)
{
    if (animations.find(name) != animations.end()) 
    {
        animation_state.current_animation = name;
        animation_state.current_frame     = 0;
        animation_state.elapsed_time      = 0.0f;
        animation_state.is_looping        = loop;
        animation_state.is_playing        = true;

        if (!animation_state.is_playing)
            play();
    }
    else
    {
        LOG_WARN("Animation not found");
    }
}

void Animator::play()
{
    if (!animation_state.current_animation.empty() && !animations.empty())
        animation_state.is_playing = true;
}

void Animator::pause()
{
    animation_state.is_playing = false;
}

void Animator::stop()
{
    animation_state.is_playing    = false;
    animation_state.current_frame = 0;
}

void Animator::update(float dt)
{
    if (!animation_state.is_playing || animation_state.current_animation.empty())
        return;

    auto it = animations.find(animation_state.current_animation);
    if (it == animations.end())
        return;

    auto& clip = it->second;

    if (clip.frames.empty())
    {
        sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
        animation_state.is_playing = false;
        return;
    }

    animation_state.elapsed_time += dt * speed;

    if (animation_state.elapsed_time >= clip.total_duration)
    {
        animation_state.elapsed_time = 0.0f;

        if (animation_state.is_looping || animation_state.current_frame < static_cast<int>(clip.frames.size() - 1))
        {
            animation_state.current_frame = (animation_state.current_frame + 1) % static_cast<int>(clip.frames.size());
        }
        else
        {
            animation_state.is_playing = false;
            if (finished_callback)
                finished_callback(animation_state.current_animation);
        }

        if (animation_state.current_frame >= 0 &&
            animation_state.current_frame < static_cast<int>(clip.frames.size()))
        {
            sprite.setTextureRect(clip.frames[animation_state.current_frame]);
        }
        else
        {
            sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
        }
    }
}

const std::map<std::string, AnimationClip>& Animator::getAllAnimations() const
{
    return animations;
}

void Animator::onAnimationFinished(FinishedCallback callback)
{
    finished_callback = std::move(callback);
}

void Animator::setSprite(const sf::Sprite& sprite)
{
    this->sprite = sprite;
}

const sf::Sprite& Animator::getSprite() const
{
    return sprite;
}

void Animator::setAnimationSpeed(float speed) 
{
    this->speed = speed;
}

float Animator::getAnimationSpeed() const 
{
    return speed;
}

void Animator::setAnimationFrame(int frameIndex)
{
    if (!animation_state.current_animation.empty())
    {
        auto it = animations.find(animation_state.current_animation);
        if (it != animations.end() && frameIndex >= 0 && frameIndex < static_cast<int>(it->second.frames.size()))
        {
            animation_state.current_frame = frameIndex;
            sprite.setTextureRect(it->second.frames[frameIndex]);
        }
    }
}

#pragma endregion



#pragma region AnimationLoader

bool AnimationLoader::loadFromFile(const std::string& filename, Animator& animator)
{
    std::ifstream file("resources/Animations/" + filename + ".json");

    if (!file.is_open())
    {
        LOG_ERROR("Failed to open animation file: {}", filename);
        return false;
    }

    json jsonData;
    file >> jsonData;
    file.close();

    if (jsonData.contains("texture"))
        std::string textureName = jsonData["texture"];

    if (jsonData.contains("animations"))
    {
        for (auto& [animName, animJson] : jsonData["animations"].items())
        {
            AnimationClip clip;

            clip.total_duration = animJson.value("total_duration", 10.0f);

            if (animJson.contains("frames"))
            {
                for (const auto& frameJson : animJson["frames"])
                {
                    sf::IntRect rect(frameJson.value("left", 0),
                        frameJson.value("top", 0),
                        frameJson.value("width", 0),
                        frameJson.value("height", 0));

                    clip.frames.push_back(rect);
                }
            }
            animator.addAnimation(animName, clip);
        }
    }
    return true;
}

bool AnimationLoader::saveToFile(const std::string& filename, const Animator& animator)
{
    json jsonData;
    jsonData["texture"] = "unknown";

    json animationsJson;

    const auto& animationsMap = animator.getAllAnimations();

    for (const auto& [name, clip] : animationsMap)
    {
        json framesJson;
        for (const auto& frame : clip.frames)
        {
            framesJson.push_back({ {"left",   frame.left},
                                  {"top",    frame.top},
                                  {"width",  frame.width},
                                  {"height", frame.height} });
        }

        animationsJson[name]["total_duration"] = clip.total_duration;
        animationsJson[name]["frames"] = framesJson;
    }

    jsonData["animations"] = animationsJson;

    std::ofstream file(filename);

    if (!file.is_open())
    {
        LOG_ERROR("Failed to save animation file: {}", filename);
        return false;
    }

    file << jsonData.dump(4);
    file.close();

    return true;
}

#pragma endregion