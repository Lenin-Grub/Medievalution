#include "Animator.hpp"


#pragma region AnimationClip

Animation::Animation(const std::string& name, const std::string& texture_name)
    : name(name)
    , texture_name(texture_name)
{
}

void Animation::addFrame(sf::IntRect rect, float duration)
{
    frames.push_back({ rect, duration });
}

bool Animation::removeFrame(int index)
{
    if (index >= 0 && index < frames.size())
    {
        frames.erase(frames.begin() + index);
        return true;
    }
    return false;
}

void Animation::clearAllFrames()
{
    frames.clear();
}

#pragma endregion



#pragma region Animator

Animator::Animator(sf::Sprite& sprite)
    : sprite(sprite)
{
}

void Animator::createNewAnimation(const std::string& name, const std::string& texture_name)
{
    if (name.empty())
    {
        LOG_WARN("Animation name is empty");
        return;
    }

    Animation anim(name, texture_name);
    animations.emplace(name, std::move(anim));
}

void Animator::pushAnimation(const std::string& name, const Animation& animation)
{
    if (name.empty())
    {
        LOG_WARN("Animation name is empty");
        return;
    }

    if (!animation.frames.empty())
    {
        animations[name] = animation;
    }
}

void Animator::deleteAllAnimations()
{
    animations.clear();
}

void Animator::setAnimation(const std::string& name, bool loop)
{
    auto it = animations.find(name);
    if (it == animations.end())
    {
        LOG_WARN("Animation not found: {}", name);
        return;
    }

    const Animation& anim = it->second;

    if (animation_state.current_animation == name)
    {
        animation_state.is_looping = loop;

        if (!animation_state.is_playing)
            play();
        return;
    }

    //LOG_DEBUG("Actually changing animation: {} -> {}",animation_state.current_animation, name);

    animation_state.current_animation = name;
    animation_state.current_frame = 0;
    animation_state.elapsed_time = 0.0f;
    animation_state.is_looping = loop;

    if (!anim.texture_name.empty() && animation_state.current_texture != anim.texture_name)
    {
        //LOG_INFO("Changing texture from {} to {}", animation_state.current_texture, anim.texture_name);

        sf::Texture& texture = ResourceLoader::instance().getTexture(anim.texture_name);
        sprite.setTexture(texture, true);
        animation_state.current_texture = anim.texture_name;
    }
    else if (!anim.texture_name.empty())
    {
        //LOG_DEBUG("Texture unchanged: {}", anim.texture_name);
    }

    if (!anim.frames.empty())
    {
        const auto& firstFrame = anim.frames[0];
        sprite.setTextureRect(firstFrame.rect);
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
    }

    if (!animation_state.is_playing)
        play();
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

    elapsed += delta_time;

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
        const auto& frame = clip.frames[current_index];

        if (frame.flip)
        {
            sf::IntRect flippedRect = frame.rect;
            flippedRect.left += flippedRect.width;
            flippedRect.width = -flippedRect.width;
            sprite.setTextureRect(flippedRect);
        }
        else
        {
            sprite.setTextureRect(frame.rect);
        }
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
    }
}

const std::map<std::string, Animation>& Animator::getAllAnimations() const
{
    return animations;
}

void Animator::setFlip(bool flip)
{
    animation_state.is_flip = flip;

    if (!animation_state.current_animation.empty())
    {
        auto it = animations.find(animation_state.current_animation);
        if (it != animations.end() && animation_state.current_frame < static_cast<int>(it->second.frames.size()))
        {
            const auto& frame = it->second.frames[animation_state.current_frame];
            if (frame.flip != animation_state.is_flip)
            {
                sf::IntRect currentRect = sprite.getTextureRect();
                currentRect.left += currentRect.width;
                currentRect.width = -currentRect.width;
                sprite.setTextureRect(currentRect);
            }
        }
    }
}

bool Animator::getFlip() const
{
    return animation_state.is_flip;
}

void Animator::applyFlip(const sf::IntRect& rect)
{
    if (animation_state.is_flip)
    {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f scale = sprite.getScale();
        sprite.setScale(-scale.x, scale.y);
        sprite.setPosition(position.x - rect.width * abs(scale.x), position.y);
    }
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

bool Animator::renameAnimation(const std::string& oldName, const std::string& newName)
{
    auto it = animations.find(oldName);
    if (it == animations.end())
        return false;

    if (animations.find(newName) != animations.end())
        return false;

    Animation anim = std::move(it->second);
    animations.erase(it);
    animations[newName] = std::move(anim);

    if (animation_state.current_animation == oldName)
        animation_state.current_animation = newName;

    return true;
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

    if (jsonData.contains("animations"))
    {
        for (auto& [animName, animJson] : jsonData["animations"].items())
        {
            std::string texture_name = animJson.value("texture", "");
            Animation clip(animName, texture_name);

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
            animator.pushAnimation(animName, clip);
        }
    }

    return true;
}

bool AnimationLoader::saveToFile(const std::string& filename, const Animator& animator)
{
    json jsonData;
    json animationsJson;

    const auto& animationsMap = animator.getAllAnimations();

    for (const auto& [name, clip] : animationsMap)
    {

        if (!clip.texture_name.empty())
            animationsJson[name]["texture"] = clip.texture_name;

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

    std::string full_filename = filename;
    if (filename.substr(filename.find_last_of(".") + 1) != "json") 
        full_filename += ".json";

    std::ofstream file(full_filename);

    if (!file.is_open())
    {
        LOG_ERROR("Failed to save animation file: {}", full_filename);
        return false;
    }

    try 
    {
        file << jsonData.dump(4);
    }
    catch (const std::exception& e) {
        LOG_ERROR("Failed to serialize JSON for file {}: {}", full_filename, e.what());
        file.close();
        return false;
    }
    file.close();

    LOG_INFO("Successfully saved animation file: {}", full_filename);
    return true;
}

#pragma endregion
