#include "AnimationEditor.hpp"

AnimationEditor::AnimationEditor(sf::RenderWindow& window)
    : window(window), sprite(), animator(sprite), selected_frame_index(-1), selected_tile_index(-1), is_playing(false)
{
}

void AnimationEditor::update(const float& delta_time)
{
    animator.update(delta_time);
}

void AnimationEditor::gui()
{
    ImGui::Begin("Animation Editor");

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
    ImGui::BeginChild("SpritePreview", ImVec2(270, 310), true);

    const char* files[] = { "Spearman", "Archer" };
    static int selected = 0;

    if (ImGui::Combo("Select", &selected, files, IM_ARRAYSIZE(files)))
    {
        current_animation_name.clear();

        std::string filename = files[selected];

        animator.clearAllAnimations();

        texture = ResourceLoader::instance().getTexture(filename + ".png");
        sprite.setTexture(texture);

        AnimationLoader::loadFromFile(filename, animator);
    }

    const sf::Sprite& animatedSprite = animator.getSprite();
    if (animatedSprite.getTexture() != nullptr)
        ImGui::Image(animatedSprite, ImVec2(256, 256));
    else
        ImGui::Text("Texture not loaded");

    ImGui::EndChild();
}

void AnimationEditor::drawAnimationsPanel()
{
    ImGui::BeginChild("AnimationsPanel", ImVec2(0, 310), true);


    static char newAnimName[64] = "";
    ImGui::InputText("New Animation", newAnimName, IM_ARRAYSIZE(newAnimName));

    if (ImGui::Button("Add Animation") && newAnimName[0])
    {
        AnimationClip clip;
        animator.addAnimation(newAnimName, clip);
        current_animation_name = newAnimName;
        newAnimName[0] = '\0';
    }

    for (const auto& [name, _] : animator.getAllAnimations())
    {
        if (ImGui::Selectable(name.c_str(), current_animation_name == name))
        {
            current_animation_name = name;
            animator.setAnimation(current_animation_name, true);
        }
    }

    ImGui::EndChild();
}

void AnimationEditor::drawFrameList()
{
    ImGui::BeginChild("FrameList", ImVec2(0, 310), true);

    if (!current_animation_name.empty())
    {
        auto it = animator.getAllAnimations().find(current_animation_name);
        if (it != animator.getAllAnimations().end())
        {
            const auto& frames = it->second.frames;

            if (ImGui::BeginTable("FrameTable", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg))
            {
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
                    {
                        removeFrame(current_animation_name, i);
                    }

                    ImGui::PopID();
                }

                ImGui::EndTable();
            }
        }
    }

    ImGui::EndChild();
}

void AnimationEditor::drawFrameControls()
{
    ImGui::BeginChild("EditFrame", ImVec2(0, 350), true);

    if (!current_animation_name.empty())
    {
        const auto& animations = animator.getAllAnimations();
        auto it = animations.find(current_animation_name);
        if (it != animations.end())
        {
            auto& frames = const_cast<std::vector<Frame>&>(it->second.frames); // Ќе очень красиво, но работает в редакторе

            if (selected_frame_index >= 0 && selected_frame_index < static_cast<int>(frames.size()))
            {
                auto& frame = frames[selected_frame_index];

                ImGui::Text("Current Frame: %d", selected_frame_index);

                ImGui::Separator();

                ImGui::DragInt("Left", &frame.rect.left);
                ImGui::DragInt("Top", &frame.rect.top);
                ImGui::DragInt("Width", &frame.rect.width);
                ImGui::DragInt("Height", &frame.rect.height);

                ImGui::Separator();

                ImGui::DragFloat("Duration (s)", &frame.duration, 0.01f, 0.01f, 10.0f, "%.2f");
            }
            else
            {
                ImGui::Text("No valid frame selected.");
            }
        }
        else
        {
            ImGui::Text("Animation not found.");
        }
    }
    else
    {
        ImGui::Text("No animation selected.");
    }

    ImGui::EndChild();
}

void AnimationEditor::drawPlaybackControls()
{
    ImGui::BeginGroup();

    if (!current_animation_name.empty())
    {
        auto it = animator.getAllAnimations().find(current_animation_name);
        if (it != animator.getAllAnimations().end() && !it->second.frames.empty())
        {
            const auto& clip = it->second;

            if (ImGui::Button(ICON::getStr(Icon::BEGIN).c_str()))
            {
                selected_frame_index = 0;
                sprite.setTextureRect(clip.frames[selected_frame_index].rect);
            }

            ImGui::SameLine();
            if (ImGui::Button(ICON::getStr(Icon::PREV).c_str()))
            {
                selected_frame_index = std::max(0, selected_frame_index - 1);
                sprite.setTextureRect(clip.frames[selected_frame_index].rect);
            }

            ImGui::SameLine();
            if (ImGui::Button(is_playing ? ICON::getStr(Icon::PAUSE).c_str() : ICON::getStr(Icon::PLAY).c_str()))
            {
                if (!current_animation_name.empty())
                {
                    if (animator.getCurrentAnimationName() != current_animation_name)
                    {
                        animator.setAnimation(current_animation_name, true);
                    }

                    is_playing = !is_playing;

                    if (is_playing)
                        animator.play();
                    else
                        animator.pause();
                }
            }

            ImGui::SameLine();
            if (ImGui::Button(ICON::getStr(Icon::NEXT).c_str()))
            {
                selected_frame_index = std::min(static_cast<int>(clip.frames.size()) - 1, selected_frame_index + 1);
                sprite.setTextureRect(clip.frames[selected_frame_index].rect);
            }

            ImGui::SameLine();
            if (ImGui::Button(ICON::getStr(Icon::END).c_str()))
            {
                selected_frame_index = static_cast<int>(clip.frames.size()) - 1;
                sprite.setTextureRect(clip.frames[selected_frame_index].rect);
            }

            ImGui::SameLine();
            ImGui::Separator();

            if (ImGui::Button(ICON::getStr(Icon::LOOP).c_str()))
            {
                is_looping != is_looping;
            }
        }
    }

    ImGui::EndGroup();
}

void AnimationEditor::drawSpriteSheet()
{
    ImGui::BeginChild("SpriteSheet", ImVec2(0, 0), true);

    if (texture.getSize().x > 0)
    {
        static int scaleValue = 96;
        const int cols = texture.getSize().x / tileSize;
        const int rows = texture.getSize().y / tileSize;

        ImGui::SliderInt("Scale", &scaleValue, 32, 128);
        ImTextureID texId = static_cast<ImTextureID>(texture.getNativeHandle());

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

                    const int index = row * cols + col;
                    const bool isSelected = (index == selected_tile_index);

                    if (isSelected)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 0, 0, 255));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
                    }

                    if (ImGui::ImageButton("##Tile", texId, ImVec2(scaleValue, scaleValue), uv0, uv1))
                    {
                        selected_tile_index = index;
                    }

                    if (ImGui::IsItemHovered())
                    {
                        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                        {
                            selected_tile_index = index;
                            ImGui::OpenPopup("AddFrameMenu");
                        }
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
        }
    }

    ImGui::EndChild();
}

void AnimationEditor::removeFrame(const std::string& animationName, size_t index)
{
    auto it = animator.getAllAnimations().find(animationName);
    if (it == animator.getAllAnimations().end())
        return;

    AnimationClip clip = it->second;
    if (index >= clip.frames.size())
        return;

    clip.frames.erase(clip.frames.begin() + index);
    selected_frame_index = std::max(-1, static_cast<int>(index) - 1);

    animator.addAnimation(animationName, clip);
}

void AnimationEditor::updateSelectedTileFromFrame()
{
    const int cols = texture.getSize().x / tileSize;
    if (cols <= 0) return;

    const sf::IntRect& rect = animator.getAllAnimations().at(current_animation_name).frames[selected_frame_index].rect;
    const int x = rect.left / tileSize;
    const int y = rect.top / tileSize;
    selected_tile_index = y * cols + x;
}


void AnimationEditor::addFrameFromSelectedTile()
{
    if (texture.getSize().x <= 0 || current_animation_name.empty())
        return;

    auto it = animator.getAllAnimations().find(current_animation_name);
    if (it == animator.getAllAnimations().end())
        return;

    const int cols = texture.getSize().x / tileSize;
    if (cols <= 0)
        return;

    const int x = selected_tile_index % cols;
    const int y = selected_tile_index / cols;

    sf::IntRect rect(x * tileSize, y * tileSize, tileSize, tileSize);

    Frame newFrame;
    newFrame.rect = rect;
    newFrame.duration = 1.0f;

    AnimationClip modifiedClip = it->second;
    modifiedClip.frames.push_back(newFrame);

    selected_frame_index = static_cast<int>(modifiedClip.frames.size() - 1);

    animator.addAnimation(current_animation_name, modifiedClip);
}