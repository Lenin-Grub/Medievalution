#include "AnimationEditor.hpp"

AnimationEditor::AnimationEditor(sf::RenderWindow& window)
    : window(window), sprite(), animator(sprite), selected_frame_index(-1), selected_tile_index(-1),
    is_playing(false), frame_size(32, 32), current_unit(""), current_texture_for_animation("")
{
    available_units = { "Spearman", "Archer", "Swordman" };
}

void AnimationEditor::update(const float& delta_time)
{
    if (is_playing) 
        animator.update(delta_time);
}

void AnimationEditor::draw()
{
    ImGui::Begin(SET_ICON_TEXT((Icon::INSTAGRAM), "Animation editor"));
    drawTopPanel();
    drawPlaybackControls();
    drawSpriteSheet();
    ImGui::End();
}

void AnimationEditor::drawTopPanel()
{
    if (ImGui::BeginTable("TopRow", 4))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); drawSpriteSelector();
        ImGui::TableNextColumn(); drawAnimationsPanel();
        ImGui::TableNextColumn(); drawFrameList();
        ImGui::TableNextColumn(); drawFrameControls();
        ImGui::EndTable();
    }
}

void AnimationEditor::drawSpriteSelector()
{
    ImGui::BeginChild("Sprite Preview", ImVec2(270, 310), true);
    const char* files[] = { "Spearman", "Archer", "Swordman" };
    static int selected = 0;

    if (ImGui::Combo("Select", &selected, files, IM_ARRAYSIZE(files)))
    {
        current_animation_name.clear();
        std::string filename = files[selected];
        current_unit = filename;
        animator.deleteAllAnimations();
        texture = ResourceLoader::instance().getTexture(filename + ".png");
        
        if (texture.getSize().x > 0)
            sprite.setTexture(texture);

        AnimationLoader::loadFromFile(filename, animator);
    }
    const sf::Sprite& animated_sprite = animator.getSprite();
    if (animated_sprite.getTexture() != nullptr)
        ImGui::Image(animated_sprite, ImVec2(256, 256));
    else
        ImGui::Text("Texture not loaded");
    ImGui::EndChild();
}


void AnimationEditor::loadUnitAnimations(const std::string& unit_name)
{
    current_animation_name.clear();
    selected_frame_index = -1;
    selected_tile_index = -1;
    is_playing = false;
    animator.stop();
    animator.deleteAllAnimations();

    texture = ResourceLoader::instance().getTexture(unit_name + ".png");
    
    if (texture.getSize().x > 0)
        sprite.setTexture(texture);
    else
        LOG_WARN("Failed to load texture for unit: {}", unit_name);

    if (!AnimationLoader::loadFromFile(unit_name, animator))
        LOG_INFO("Loaded animation for {}", unit_name);
    else
        LOG_WARN("Failed to load animations for unit: {}", unit_name);
}


void AnimationEditor::drawAnimationsPanel()
{
    ImGui::BeginChild("AnimationsPanel", ImVec2(0, 310), true);

    static char new_animation_name[64] = "";
    ImGui::InputText("New Animation", new_animation_name, IM_ARRAYSIZE(new_animation_name));

    static char new_animation_texture[128] = "";
    ImGui::InputText("Texture Name", new_animation_texture, IM_ARRAYSIZE(new_animation_texture));

    if (ImGui::Button("Add Animation") && new_animation_name[0])
    {
        animator.createNewAnimation(new_animation_name, new_animation_texture);
        current_animation_name = new_animation_name;
        new_animation_name[0] = '\0';
    }

    if (ImGui::Button("Save All"))
        AnimationLoader::saveToFile(current_unit, animator);

    for (const auto& [name, animation] : animator.getAllAnimations())
    {
        bool is_selected = (current_animation_name == name);
        std::string display_text = name;

        if (!animation.texture_name.empty())
            display_text;

        if (ImGui::Selectable(display_text.c_str(), is_selected))
        {
            current_animation_name = name;
            animator.setAnimation(current_animation_name, true);

            const std::string& anim_texture_name = animation.texture_name;
            if (!anim_texture_name.empty()) 
            {
                if (anim_texture_name != current_unit + ".png") 
                {
                    texture = ResourceLoader::instance().getTexture(anim_texture_name);
                    if (texture.getSize().x > 0)
                    {
                        sprite.setTexture(texture);
                        selected_tile_index = -1;
                    }
                    else 
                    {
                        LOG_WARN("Failed to load texture: {}", anim_texture_name);
                        texture = ResourceLoader::instance().getTexture(current_unit + ".png");

                        if (texture.getSize().x > 0)
                            sprite.setTexture(texture);
                    }
                }
            }
            else 
            {
                if (current_unit + ".png" != (texture.getSize().x > 0 ? "loaded" : "")) 
                {
                    texture = ResourceLoader::instance().getTexture(current_unit + ".png");

                    if (texture.getSize().x > 0) 
                    {
                        sprite.setTexture(texture);
                        selected_tile_index = -1;
                    }
                }
            }
            selected_frame_index = -1;
        }
    }
    ImGui::EndChild();
}

void AnimationEditor::drawFrameList()
{
    ImGui::BeginChild("FrameList", ImVec2(0, 310), true);
    if (current_animation_name.empty())
    {
        ImGui::Text("No animation selected.");
        ImGui::EndChild();
        return;
    }
    auto it = animator.getAllAnimations().find(current_animation_name);
    if (it == animator.getAllAnimations().end())
    {
        ImGui::Text("Animation not found.");
        ImGui::EndChild();
        return;
    }
    const auto& frames = it->second.frames;
    if (frames.empty())
    {
        ImGui::Text("No frames available.");
        ImGui::EndChild();
        return;
    }
    if (!ImGui::BeginTable("FrameTable", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg))
    {
        ImGui::EndChild();
        return;
    }
    ImGui::TableSetupColumn("Frames", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableHeadersRow();
    for (size_t i = 0; i < frames.size(); ++i)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushID(static_cast<int>(i));
        std::string label = "Frame " + std::to_string(i);
        bool is_selected = (selected_frame_index == static_cast<int>(i));
        if (ImGui::Selectable(label.c_str(), is_selected))
        {
            selected_frame_index = static_cast<int>(i);
            sprite.setTextureRect(frames[i].rect);
            updateSelectedTileFromFrame();
        }
        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button(ICON::getStr(Icon::DUMP_FULL).c_str()))
            removeFrame(current_animation_name, i);
        ImGui::PopID();
    }
    ImGui::EndTable();
    ImGui::EndChild();
}

void AnimationEditor::drawFrameControls()
{
    ImGui::BeginChild("EditFrame", ImVec2(0, 350), true);
    if (current_animation_name.empty())
    {
        ImGui::Text("No animation selected.");
        ImGui::EndChild();
        return;
    }
    const auto& animations = animator.getAllAnimations();
    auto it = animations.find(current_animation_name);
    if (it == animations.end())
    {
        ImGui::Text("Animation not found.");
        ImGui::EndChild();
        return;
    }
    auto& frames = const_cast<std::vector<Frame>&>(it->second.frames);
    if (selected_frame_index < 0 || selected_frame_index >= static_cast<int>(frames.size()))
    {
        ImGui::Text("No valid frame selected.");
        ImGui::EndChild();
        return;
    }
    auto& frame = frames[selected_frame_index];
    ImGui::Text("Current Frame: %d", selected_frame_index);
    ImGui::Separator();
    ImGui::DragInt("Left", &frame.rect.left);
    ImGui::DragInt("Top", &frame.rect.top);
    ImGui::DragInt("Width", &frame.rect.width);
    ImGui::DragInt("Height", &frame.rect.height);
    ImGui::Separator();
    ImGui::DragFloat("Duration (s)", &frame.duration, 0.01f, 0.01f, 10.0f, "%.2f");
    ImGui::Checkbox("Flip", &frame.flip);
    ImGui::EndChild();
}

void AnimationEditor::drawPlaybackControls()
{
    ImGui::BeginGroup();
    if (current_animation_name.empty())
    {
        ImGui::EndGroup();
        return;
    }
    auto it = animator.getAllAnimations().find(current_animation_name);
    if (it == animator.getAllAnimations().end() || it->second.frames.empty())
    {
        ImGui::EndGroup();
        return;
    }
    const auto& clip = it->second;
    if (ImGui::Button(SET_ICON(Icon::BEGIN)))
    {
        selected_frame_index = 0;
        animator.setAnimationFrame(selected_frame_index);
    }
    ImGui::SameLine();
    if (ImGui::Button(SET_ICON(Icon::PREV)))
    {
        selected_frame_index = std::max(0, selected_frame_index - 1);
        animator.setAnimationFrame(selected_frame_index);
    }
    ImGui::SameLine();
    if (ImGui::Button(is_playing ? SET_ICON(Icon::PAUSE) : SET_ICON(Icon::PLAY)))
    {
        if (animator.getCurrentAnimationName() != current_animation_name)
            animator.setAnimation(current_animation_name, true);

        is_playing = !is_playing;
        is_playing ? animator.play() : animator.pause();
    }
    ImGui::SameLine();
    if (ImGui::Button(SET_ICON(Icon::NEXT)))
    {
        selected_frame_index = std::min(static_cast<int>(clip.frames.size()) - 1, selected_frame_index + 1);
        animator.setAnimationFrame(selected_frame_index);
    }
    ImGui::SameLine();
    if (ImGui::Button(SET_ICON(Icon::END)))
    {
        selected_frame_index = static_cast<int>(clip.frames.size()) - 1;
        animator.setAnimationFrame(selected_frame_index);
    }
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(50.0f, 0.0f));
    ImGui::SameLine();

    if (ImGui::Button(SET_ICON(Icon::LOOP)))
    {
        is_looping = !is_looping;
    }
    ImGui::SameLine();
    if (ImGui::Button(SET_ICON(Icon::FLIP_HORiZONTAL)))
    {
        if (!current_animation_name.empty())
        {
            auto anim_it = animator.getAllAnimations().find(current_animation_name);
            if (anim_it != animator.getAllAnimations().end() && !anim_it->second.frames.empty())
            {
                Animation modified_clip = anim_it->second;

                if (!modified_clip.frames.empty()) 
                {
                    bool new_flip_state = !modified_clip.frames[0].flip;
                    for (auto& frame : modified_clip.frames)
                    {
                        frame.flip = new_flip_state;
                    }
                }

                animator.pushAnimation(current_animation_name, modified_clip);

                if (selected_frame_index >= 0 && selected_frame_index < static_cast<int>(modified_clip.frames.size()))
                {
                    const auto& frame = modified_clip.frames[selected_frame_index];
                    if (frame.flip)
                    {
                        sf::IntRect flipped_rect = frame.rect;
                        flipped_rect.left += flipped_rect.width;
                        flipped_rect.width = -flipped_rect.width;
                        sprite.setTextureRect(flipped_rect);
                    }
                    else
                    {
                        sprite.setTextureRect(frame.rect);
                    }
                }
            }
        }
    }
    ImGui::EndGroup();
}

void AnimationEditor::drawSpriteSheet()
{
    ImGui::BeginChild("SpriteSheet", ImVec2(0, 0), true);
    if (texture.getSize().x <= 0)
    {
        ImGui::Text("No texture loaded");
        ImGui::EndChild();
        return;
    }
    const int frameWidth = frame_size.x;
    const int frameHeight = frame_size.y;
    const int cols = (frameWidth > 0) ? texture.getSize().x / frameWidth : 0;
    const int rows = (frameHeight > 0) ? texture.getSize().y / frameHeight : 0;
    if (cols <= 0 || rows <= 0 || frameWidth <= 0 || frameHeight <= 0)
    {
        ImGui::Text("Invalid frame size or texture dimensions");
        ImGui::EndChild();
        return;
    }
    static int scale_value = 96;
    ImGui::SliderInt("Scale", &scale_value, 32, 256);
    int tmp_frame_size[2] = { frame_size.x, frame_size.y };
    if (ImGui::SliderInt2("Frame Size", tmp_frame_size, 16, 256))
    {
        if (tmp_frame_size[0] > 0 && tmp_frame_size[1] > 0)
        {
            frame_size.x = tmp_frame_size[0];
            frame_size.y = tmp_frame_size[1];
            selected_tile_index = -1;
            selected_frame_index = -1;
            sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
        }
    }

    ImTextureID texId = reinterpret_cast<ImTextureID>(texture.getNativeHandle());

    if (!ImGui::BeginTable("Tiles", cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
    {
        ImGui::EndChild();
        return;
    }
    for (int row = 0; row < rows; ++row)
    {
        ImGui::TableNextRow();
        for (int col = 0; col < cols; ++col)
        {
            ImGui::TableNextColumn();
            ImGui::PushID(row * cols + col);
            ImVec2 uv0((float)(col * frameWidth) / (float)texture.getSize().x, (float)(row * frameHeight) / (float)texture.getSize().y);
            ImVec2 uv1((float)((col + 1) * frameWidth) / (float)texture.getSize().x, (float)((row + 1) * frameHeight) / (float)texture.getSize().y);
            const int index = row * cols + col;
            const bool isSelected = (index == selected_tile_index);
            if (isSelected)
            {
                ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 0, 0, 255));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
            }
            if (ImGui::ImageButton("##Tile", texId, ImVec2(scale_value, scale_value), uv0, uv1))
            {
                selected_tile_index = index;
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            {
                selected_tile_index = index;
                ImGui::OpenPopup("AddFrameMenu");
            }
            if (ImGui::BeginPopup("AddFrameMenu"))
            {
                if (ImGui::MenuItem("Add Frame"))
                {
                    addFrameFromSelectedTile();
                }
                ImGui::EndPopup();
            }
            if (isSelected)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
            }
            ImGui::PopID();
        }
    }
    ImGui::EndTable();
    ImGui::EndChild();
}

void AnimationEditor::removeFrame(const std::string& animationName, size_t index)
{
    auto it = animator.getAllAnimations().find(animationName);
    if (it == animator.getAllAnimations().end())
        return;
    Animation clip = it->second;
    if (index >= clip.frames.size())
        return;
    clip.frames.erase(clip.frames.begin() + index);

    if (selected_frame_index >= static_cast<int>(clip.frames.size())) 
    {
        selected_frame_index = static_cast<int>(clip.frames.size()) - 1;
    }
    animator.pushAnimation(animationName, clip);
}

void AnimationEditor::updateSelectedTileFromFrame()
{
    if (current_animation_name.empty() || selected_frame_index < 0) return;
    const auto& animations = animator.getAllAnimations();
    auto anim_it = animations.find(current_animation_name);
    if (anim_it == animations.end() || anim_it->second.frames.empty()) return;
    if (selected_frame_index >= static_cast<int>(anim_it->second.frames.size())) return;
    const sf::IntRect& rect = anim_it->second.frames[selected_frame_index].rect;
    if (frame_size.x <= 0 || frame_size.y <= 0) return;
    const int cols = texture.getSize().x / frame_size.x;
    if (cols <= 0) return;
    const int x = rect.left / frame_size.x;
    const int y = rect.top / frame_size.y;
    selected_tile_index = y * cols + x;
}

void AnimationEditor::addFrameFromSelectedTile()
{
    if (texture.getSize().x <= 0 || current_animation_name.empty())
        return;
    auto it = animator.getAllAnimations().find(current_animation_name);
    if (it == animator.getAllAnimations().end())
        return;
    const int cols = (frame_size.x > 0) ? texture.getSize().x / frame_size.x : 0;
    if (cols <= 0 || frame_size.x <= 0 || frame_size.y <= 0)
        return;
    if (selected_tile_index < 0) return;
    const int x = selected_tile_index % cols;
    const int y = selected_tile_index / cols;
    sf::IntRect rect(x * frame_size.x, y * frame_size.y, frame_size.x, frame_size.y);
    Frame newFrame;
    newFrame.rect = rect;
    newFrame.duration = 1.0f;
    newFrame.flip = false;
    Animation modifiedClip = it->second;
    modifiedClip.frames.push_back(newFrame);
    selected_frame_index = static_cast<int>(modifiedClip.frames.size() - 1);
    animator.pushAnimation(current_animation_name, modifiedClip);
    sprite.setTextureRect(rect);
}