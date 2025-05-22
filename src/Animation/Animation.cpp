#include "stdafx.h"
#include "Animation.hpp"

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
    animation_state.elapsed_time += dt;

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

        sprite.setTextureRect(clip.frames[animation_state.current_frame]);
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


bool AnimationLoader::loadFromFile(const std::string& filename, Animator& animator)
{
    std::ifstream file("resources/Animations/" + filename + ".json");

    if (!file.is_open())
    {
        LOG_ERROR("Failed to open animation file: {}", filename);
        return false;
    }

    nlohmann::json jsonData;
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

const std::map<std::string, AnimationClip>& Animator::getAllAnimations() const
{
    return animations;
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
            framesJson.push_back({{"left",   frame.left},
                                  {"top",    frame.top},
                                  {"width",  frame.width},
                                  {"height", frame.height}});
        }

        animationsJson[name]["total_duration"] = clip.total_duration;
        animationsJson[name]["frames"]         = framesJson;
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


//
//Animator::Animator(sf::Sprite& sprite)
//    : sprite        (sprite)
//    , played        (true)
//    , current_frame (0)
//    , frame_time    (0.0f)
//    , current_time  (0.0f)
//    , frame_size    {64,64}
//{
//}
//
//bool Animator::init(const std::string& name)
//{
//    try
//    {
//        sprite.setTexture(ResourceLoader::instance().getTexture(name));
//        sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
//        LOG_INFO("Animation {} loaded", name);
//        return true;
//    }
//    catch (const std::exception& e)
//    {
//        LOG_ERROR("fail init animations: {} !", name);
//        return false;
//    }
//
//}
//
//void Animator::addFrame(sf::IntRect rect)
//{
//    frames.push_back(rect);
//}
//
//void Animator::removeFrame(const int& id)
//{
//    if (!frames.empty())
//    {
//        frames.erase(frames.begin() + id);
//        if (current_frame > 0)
//            current_frame--;
//    }
//    else 
//       LOG_ERROR("You can't remove this frame. Frames are empty!");
//}
//
//void Animator::removeAllFrames()
//{
//    frames.clear();
//    current_frame = 0;
//}
//
//void Animator::addAnimation(const std::string& name)
//{
//    if (frames.empty())
//    {
//        LOG_ERROR("You can`t add animation. Frames are empty");
//        return;
//    }
//    animations.emplace(name, frames);
//    LOG_INFO("Size animation {}", animations.size());
//}
//
//void Animator::removeAnimation(const std::string& name)
//{
//    if (animations.empty())
//    {
//        LOG_ERROR("You can`t remove animation: \"{}\" not founded", name);
//        return;
//    }
//    animations.erase(name);
//}
//
//void Animator::setFrameTime(float time)
//{
//    frame_time = time;
//}
//
//void Animator::setCurrentFrame(int frame)
//{
//    if (!frames.empty()) 
//        current_frame = frame;
//}
//
//void Animator::update(float deltaTime)
//{
//    if (frames.empty())
//        return;
//
//    current_time += deltaTime;
//    if (!played)
//        sprite.setTextureRect(frames.at(current_frame));
//    else
//    {
//        if (current_time >= frame_time)
//        {
//            current_frame = (current_frame + 1) % frames.size();
//            sprite.setTextureRect(frames.at(current_frame));
//            current_time = 0.0f;
//        }
//    }
//}
//
//void Animator::play(bool play)
//{
//    played = play;
//}
//
//void Animator::pause()
//{
//    played = false;
//}
//
//void Animator::loop(bool loop)
//{
//
//}
//
//const int Animator::getCurrentFrame() const
//{
//    if (!frames.empty() && current_frame < frames.size()) 
//        return current_frame;
//    else 
//        return 0;
//}
//
//const int Animator::getNextFrame() const
//{
//    if (!frames.empty() && current_frame < frames.size()-1)
//        return current_frame + 1;
//}
//
//int Animator::getPrevFrame() const
//{
//    if (!frames.empty() && current_frame <= 0)
//        return 0;
//       
//    if (!frames.empty() && current_frame > 0)
//        return current_frame - 1;
//}
//
//const int Animator::getLastFrame()
//{
//    if (!frames.empty())
//        return current_frame = frames.size()-1;
//}
//
//const int Animator::getFirstFrame()
//{
//    if (!frames.empty() && current_frame < frames.size())
//        return current_frame = 0;
//}
//
//sf::Vector2i Animator::getFrameSize() const
//{
//    return sf::Vector2i();
//}
//
//void Animator::setFrameSize(const sf::Vector2i size)
//{
//}
//
//const float Animator::getCurrentTime() const
//{
//    return current_time;
//}
//
//const float Animator::getFrameTime() const
//{
//    return frame_time;
//}
//
//const bool Animator::isPlayed() const
//{
//    return played;
//}
//
//const std::vector<sf::IntRect>& Animator::getFrames() const
//{
//    return frames;
//}
//
//const sf::IntRect& Animator::getCurrentFrameRect() const
//{
//    if (frames.empty())
//        return sf::IntRect(0, 0, 64, 64);
//
//    return frames.at(getCurrentFrame());
//}
//
//std::map<std::string, std::vector<sf::IntRect>> Animator::getAnimations() const
//{
//    return animations;
//}
//
//bool Animator::saveAnimation(const std::string& path, const std::string& name) const 
//{
//    auto it = animations.find(name);
//    if (it != animations.end()) 
//    {
//        nlohmann::json j;
//
//        std::ifstream file("resources/Animations/" + path + ".json");
//        if (file.is_open()) 
//        {
//            file >> j;
//            file.close();
//            return false;
//        }
//
//        bool animationExists = false;
//        for (const auto& animation : j["animations"]) 
//        {
//            if (animation["name"] == name)
//            {
//                animationExists = true;
//                break;
//            }
//        }
//
//        if (!animationExists) 
//        {
//            nlohmann::json newAnimation;
//            newAnimation["name"]   = name;
//            newAnimation["frames"] = nlohmann::json::array();
//
//            for (const auto& frame : it->second) 
//            {
//                newAnimation["frames"].push_back({ {"left"  , frame.left   }, 
//                                                   {"top"   , frame.top    }, 
//                                                   {"width" , frame.width  }, 
//                                                   {"height", frame.height } });
//            }
//
//            j["animations"].push_back(newAnimation);
//        }
//
//        j["texture"] = path + ".png";
//
//
//        std::ofstream outFile("../resources/Animations/" + path + ".json");
//        if (outFile.is_open()) 
//        {
//            outFile << j.dump(4);
//            outFile.close();
//            return true;
//        }
//        else 
//        {
//            LOG_ERROR("Failed to open file to save animation: {}", path + ".json");
//            return false;
//        }
//    }
//    else 
//    {
//        LOG_ERROR("Animation not found: {}", name);
//        return false;
//    }
//}
//
//bool Animator::loadAllAnimations(const std::string& path) 
//{
//    std::ifstream file("resources/Animations/" + path + ".json");
//
//    if (file.is_open()) 
//    {
//        nlohmann::json j;
//        file >> j;
//        file.close();
//
//        animations.clear();
//
//        std::string textureName = j["texture"];
//
//        sprite.setTexture(ResourceLoader::instance().getTexture(textureName));
//
//        for (const auto& animation : j["animations"]) 
//        {
//            std::string animationName = animation["name"];
//            std::vector<sf::IntRect> loadedFrames;
//            for (const auto& frame : animation["frames"]) 
//            {
//                loadedFrames.emplace_back(frame["left"], frame["top"], frame["width"], frame["height"]);
//            }
//            animations[animationName] = loadedFrames;
//        }
//
//        return true;
//    }
//    else 
//    {
//        LOG_ERROR("Failed to open file for loading animations: {}", path);
//    }
//    return false;
//}
//
//std::vector<sf::IntRect> Animator::findAnimation(const std::string& name) const 
//{
//    auto it = animations.find(name);
//    if (it != animations.end()) 
//    {
//        return it->second;
//    }
//    else 
//    {
//        LOG_ERROR("Animation not found: {}", name);
//        return {};
//    }
//}
//
//std::vector<std::string> Animator::getAnimationNames() const
//{
//    std::vector<std::string> names;
//    for (const auto& pair : animations)
//    {
//        names.push_back(pair.first);
//    }
//    return names;
//}
//
//void Animator::flipFramesHorizontally()
//{
//    if (frames.empty())
//    {
//        LOG_ERROR("Cannot flip frames. Frames are empty!");
//        return;
//    }
//
//    const sf::Texture* texture = sprite.getTexture();
//    if (!texture)
//    {
//        LOG_ERROR("Cannot flip frames. Texture is not set!");
//        return;
//    }
//
//    sf::Vector2u textureSize = texture->getSize();
//
//    for (auto& frame : frames)
//    {
//        frame.left = textureSize.x - frame.left - frame.width;
//    }
//
//    if (!frames.empty())
//    {
//        sprite.setTextureRect(frames.at(current_frame));
//    }
//
//    LOG_INFO("All frames flipped horizontally using IntRect.");
//}