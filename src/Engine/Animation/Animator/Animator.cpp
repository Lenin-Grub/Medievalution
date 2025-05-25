#include "Animator.hpp"


#pragma region AnimationClip

void AnimationClip::addFrame(sf::IntRect rect, float duration)
{
    frames.push_back({ rect, duration });
}

bool AnimationClip::removeFrame(int index)
{
    if (index >= 0 && index < frames.size())
    {
        frames.erase(frames.begin() + index);
        return true;
    }
    return false;
}

void AnimationClip::clearAllFrames()
{
    frames.clear();
}

void AnimationClip::flipHorizontally()
{
    for (auto& frame : frames)
    {
        frame.rect.left += frame.rect.width;
        frame.rect.width *= -1;
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
    {
        animations[name] = clip;
    }
}

void Animator::clearAllAnimations()
{
    animations.clear();
}

void Animator::setAnimation(const std::string& name, bool loop)
{
    if (animations.find(name) != animations.end()) 
    {
        animation_state.current_animation = name;
        animation_state.current_frame     = 0;
        animation_state.elapsed_time      = 0.0f;
        animation_state.is_looping        = loop;

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

void Animator::update(float delta_time)
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

    int current_index    = animation_state.current_frame;
    float elapsed        = animation_state.elapsed_time;
    float frame_duration = clip.frames[current_index].duration;

    elapsed += delta_time * speed;

    if (elapsed >= frame_duration)
    {
        elapsed = 0.0f;
        ++current_index;

        if (current_index >= static_cast<int>(clip.frames.size()))
        {
            if (animation_state.is_looping)
                current_index = 0;
            else
            {
                animation_state.is_playing = false;
                current_index = static_cast<int>(clip.frames.size()) - 1;

                if (finished_callback)
                    finished_callback(animation_state.current_animation);
            }
        }

        animation_state.current_frame = current_index;
    }

    animation_state.elapsed_time = elapsed;

    if (current_index >= 0 && current_index < static_cast<int>(clip.frames.size()))
    {
        sprite.setTextureRect(clip.frames[current_index].rect);
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
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
            animation_state.elapsed_time = 0.0f;
            sprite.setTextureRect(it->second.frames[frameIndex].rect);
        }
    }
}

const std::string& Animator::getCurrentAnimationName() const
{
    return animation_state.current_animation;
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
    {
        std::string textureName = jsonData["texture"];
    }

    if (jsonData.contains("animations"))
    {
        for (auto& [animName, animJson] : jsonData["animations"].items())
        {
            AnimationClip clip;

            if (animJson.contains("frames"))
            {
                for (const auto& frameJson : animJson["frames"])
                {
                    sf::IntRect rect(
                        frameJson.value("left", 0),
                        frameJson.value("top", 0),
                        frameJson.value("width", 0),
                        frameJson.value("height", 0)
                    );
                    float duration = frameJson.value("duration", 1.0f);

                    clip.addFrame(rect, duration);
                }
            }

            bool loop = animJson.value("loop", true);
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
            framesJson.push_back({
                {"left",     frame.rect.left},
                {"top",      frame.rect.top},
                {"width",    frame.rect.width},
                {"height",   frame.rect.height},
                {"duration", frame.duration}
                });
        }

        animationsJson[name]["frames"] = framesJson;
        animationsJson[name]["loop"] = true;
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