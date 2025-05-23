#include "stdafx.h"
#include "Animation.hpp"

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

#pragma endregion

#pragma region AnimationEditor

AnimationEditor::AnimationEditor(sf::RenderWindow& window)
    : window(window),
    sprite(),
    animator(sprite)
{
}

void AnimationEditor::update(const float& delta_time)
{
    animator.update(delta_time);
}

void AnimationEditor::gui()
{
    ImGui::Begin("Animation Editor");

    if (ImGui::BeginTable("TopRow", 4 /*,ImGuiTableFlags_SizingStretchProp*/))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        spriteSelector();
        ImGui::TableNextColumn();

        animationsPanel();
        ImGui::TableNextColumn();

        frameSelector();
        ImGui::TableNextColumn();

        frameControls();
        ImGui::EndTable();
    }

    playbackButtons();
    spriteSheet();

    ImGui::End();
}

void AnimationEditor::spriteSelector()
{
    ImGui::BeginChild("SpritePreview", ImVec2(270, 310), true);

    const char* files[] = { "Spearman.png", "Archer.png" };
    static int selected = 0;

    if (ImGui::Combo("Select Sprite", &selected, files, IM_ARRAYSIZE(files)))
    {
        current_animation_name.clear();
        std::string filename = files[selected];
        texture = ResourceLoader::instance().getTexture(filename);
        sprite.setTexture(texture);

        AnimationLoader::loadFromFile("Spearman", animator);
    }

    if (texture.getSize().x > 0)
        ImGui::Image(sprite, ImVec2(256, 256));
    else
        ImGui::Text("Texture not loaded");

    ImGui::EndChild();
}


void AnimationEditor::animationsPanel()
{
    ImGui::BeginChild("AnimationsPanel", ImVec2(0, 310), true);

    for (const auto& [name, _] : animator.getAllAnimations())
    {
        if (ImGui::Selectable(name.c_str(), current_animation_name == name))
        {
            current_animation_name = name;
        }
    }

    static char newAnimName[64] = "";
    ImGui::InputText("New Animation", newAnimName, IM_ARRAYSIZE(newAnimName));

    if (ImGui::Button("Add Animation") && newAnimName[0])
    {
        AnimationClip clip;
        animator.addAnimation(newAnimName, clip);
        current_animation_name = newAnimName;
        newAnimName[0] = '\0';
    }

    ImGui::EndChild();
}


void AnimationEditor::frameControls()
{
    ImGui::BeginChild("EditFrame", ImVec2(0, 310), true);
    if (!current_animation_name.empty())
    {
        auto it = animator.getAllAnimations().find(current_animation_name);
        if (it != animator.getAllAnimations().end())
        {
            auto& frames = const_cast<std::vector<sf::IntRect>&>(it->second.frames);

            constexpr int tileSize = 64;

            int textureWidth  = texture.getSize().x;
            int textureHeight = texture.getSize().y;
            int cols          = textureWidth / tileSize;
            int rows          = textureHeight / tileSize;

            // === ѕровер€ем, чтобы индекс был в пределах диапазона ===
            if (selected_frame_index >= 0 && selected_frame_index < static_cast<int>(frames.size()))
            {
                auto& rect = frames[selected_frame_index];

                ImGui::Text   ("Current Frame: %d", selected_frame_index);
                ImGui::DragInt("Left",   &rect.left);
                ImGui::DragInt("Top",    &rect.top);
                ImGui::DragInt("Width",  &rect.width);
                ImGui::DragInt("Height", &rect.height);

                if (ImGui::Button("Delete"))
                {
                    frames.erase(frames.begin() + selected_frame_index);
                    selected_frame_index = std::min(selected_frame_index, static_cast<int>(frames.size()) - 1);
                    animator.addAnimation(current_animation_name, it->second);
                }
            }

            if (ImGui::Button("Add Frame"))
            {
                if (texture.getSize().x <= 0)
                    return;

                constexpr int tileSize = 64;
                int cols = texture.getSize().x / tileSize;
                if (cols <= 0) return;

                int x = selected_tile_index % cols;
                int y = selected_tile_index / cols;

                sf::IntRect defaultRect(
                    x * tileSize,
                    y * tileSize,
                    tileSize,
                    tileSize
                );

                frames.push_back(defaultRect);
                selected_frame_index = static_cast<int>(frames.size() - 1);
                animator.addAnimation(current_animation_name, it->second);
            }
        }
    }
    ImGui::EndChild();
}


void AnimationEditor::frameSelector()
{
    ImGui::BeginChild("FrameList", ImVec2(0, 310), true);
    if (!current_animation_name.empty())
    {
        auto it = animator.getAllAnimations().find(current_animation_name);
        if (it != animator.getAllAnimations().end())
        {
            const auto& frames = it->second.frames;
            for (size_t i = 0; i < frames.size(); ++i)
            {
                std::string label = "Frame " + std::to_string(i);
                if (ImGui::Selectable(label.c_str(), selected_frame_index == i))
                {
                    selected_frame_index = static_cast<int>(i);
                    sprite.setTextureRect(frames[i]);

                    constexpr int tileSize = 64;
                    int cols = texture.getSize().x / tileSize;
                    if (cols > 0)
                    {
                        int col = frames[i].left / tileSize;
                        int row = frames[i].top / tileSize;
                        selected_tile_index = row * cols + col;
                    }
                }
            }
        }
    }
    ImGui::EndChild();
}


void AnimationEditor::spriteSheet()
{
    ImGui::BeginChild("SpriteSheet", ImVec2(0, 0), true);
    if (texture.getSize().x > 0)
    {
        static int value   = 96;
        const int minValue = 32, maxValue = 128;

        ImVec2 scale_factor = ImVec2(value, value);
        constexpr int tileSize = 64;
        int cols = texture.getSize().x / tileSize;
        int rows = texture.getSize().y / tileSize;

        ImGui::SliderInt("Scale", &value, minValue, maxValue);

        ImTextureID texId = (ImTextureID)(intptr_t)texture.getNativeHandle();

        if (ImGui::BeginTable("Tiles", cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
        {
            for (int row = 0; row < rows; ++row)
            {
                ImGui::TableNextRow();
                for (int col = 0; col < cols; ++col)
                {
                    ImGui::TableNextColumn();
                    ImGui::PushID(row * cols + col);

                    ImVec2 uv0(col / (float)cols, row / (float)rows);
                    ImVec2 uv1((col + 1) / (float)cols, (row + 1) / (float)rows);

                    int current_index = row * cols + col;
                    bool is_current = (selected_tile_index == current_index);

                    if (is_current)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 0, 0, 255));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
                    }

                    if (ImGui::ImageButton("Tile", texId, scale_factor, uv0, uv1))
                        selected_tile_index = current_index;

                    if (is_current)
                    {
                        ImGui::PopStyleColor();
                        ImGui::PopStyleVar();
                    }

                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
    }
    ImGui::EndChild();
}

void AnimationEditor::playbackButtons()
{
    ImGui::BeginGroup();

    if (!current_animation_name.empty())
    {
        auto it = animator.getAllAnimations().find(current_animation_name);
        if (it != animator.getAllAnimations().end())
        {
            const auto& clip = it->second;

            if (clip.frames.empty())
                return;

            if (ImGui::Button((ICON::getStr(Icon::BEGIN) + "##Begin").c_str()))
            {
                selected_frame_index = 0;
                sprite.setTextureRect(clip.frames[selected_frame_index]);
            }

            ImGui::SameLine(0.0f, 5.0f);

            if (ImGui::Button((ICON::getStr(Icon::PREV) + "##Prev").c_str()))
            {
                selected_frame_index = std::max(0, selected_frame_index - 1);
                sprite.setTextureRect(clip.frames[selected_frame_index]);
            }

            ImGui::SameLine(0.0f, 5.0f);

            if (ImGui::Button(is_playing ? (ICON::getStr(Icon::PAUSE) + "##Pause").c_str() : (ICON::getStr(Icon::PLAY) + "##Play").c_str()))
            {
                is_playing = !is_playing;
                if (is_playing)
                    animator.play();
                else
                    animator.pause();
            }

            ImGui::SameLine(0.0f, 5.0f);

            if (ImGui::Button((ICON::getStr(Icon::NEXT) + "##Next").c_str()))
            {
                selected_frame_index = std::min(static_cast<int>(clip.frames.size()) - 1, selected_frame_index + 1);
                sprite.setTextureRect(clip.frames[selected_frame_index]);
            }

            ImGui::SameLine(0.0f, 5.0f);

            if (ImGui::Button((ICON::getStr(Icon::END) + "##End").c_str()))
            {
                selected_frame_index = static_cast<int>(clip.frames.size()) - 1;
                sprite.setTextureRect(clip.frames[selected_frame_index]);
            }
        }
    }

    ImGui::EndGroup();
}

#pragma endregion