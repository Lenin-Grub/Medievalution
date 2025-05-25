#include "AnimationEditor.hpp"



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

    const sf::Sprite& animatedSprite = animator.getSprite();
    if (animatedSprite.getTexture() != nullptr)
        ImGui::Image(animatedSprite, ImVec2(256, 256));
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

            int textureWidth = texture.getSize().x;
            int textureHeight = texture.getSize().y;
            int cols = textureWidth / tileSize;
            int rows = textureHeight / tileSize;

            if (selected_frame_index >= 0 && selected_frame_index < static_cast<int>(frames.size()))
            {
                auto& rect = frames[selected_frame_index];

                ImGui::Text("Current Frame: %d", selected_frame_index);
                ImGui::DragInt("Left", &rect.left);
                ImGui::DragInt("Top", &rect.top);
                ImGui::DragInt("Width", &rect.width);
                ImGui::DragInt("Height", &rect.height);
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

                    ImGui::TableSetColumnIndex(1);
                    if (ImGui::SmallButton(ICON::getStr(Icon::DUMP_FULL).c_str()))
                    {
                        auto& framesNonConst = const_cast<std::vector<sf::IntRect>&>(frames);
                        framesNonConst.erase(framesNonConst.begin() + i);
                        selected_frame_index = std::max(-1, static_cast<int>(i) - 1);
                        animator.addAnimation(current_animation_name, it->second);
                    }

                    ImGui::PopID();
                }

                ImGui::EndTable();
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
        static int value = 96;
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
                    {
                        selected_tile_index = current_index;
                    }

                    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                    {
                        ImGui::OpenPopup("AddFrameMenu");
                        selected_tile_index = current_index;
                    }

                    if (ImGui::BeginPopup("AddFrameMenu"))
                    {
                        if (ImGui::MenuItem("Add Frame"))
                        {
                            addFrameFromSelectedTile();
                        }
                        ImGui::EndPopup();
                    }

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

        if (show_context_menu && context_menu_tile_index.has_value())
        {
            if (ImGui::BeginPopupContextVoid("TileContextMenu"))
            {
                if (ImGui::MenuItem("Add Frame"))
                {
                    selected_tile_index = context_menu_tile_index.value();

                    // Добавляем новый кадр
                    if (!current_animation_name.empty())
                    {
                        auto it = animator.getAllAnimations().find(current_animation_name);
                        if (it != animator.getAllAnimations().end())
                        {
                            auto& frames = const_cast<std::vector<sf::IntRect>&>(it->second.frames);
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
                ImGui::EndPopup();
            }
            show_context_menu = false;
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

void AnimationEditor::addFrameFromSelectedTile()
{
    if (texture.getSize().x <= 0 || current_animation_name.empty())
        return;

    constexpr int tileSize = 64;
    auto it = animator.getAllAnimations().find(current_animation_name);
    if (it == animator.getAllAnimations().end())
        return;

    int cols = texture.getSize().x / tileSize;
    if (cols <= 0)
        return;

    int x = selected_tile_index % cols;
    int y = selected_tile_index / cols;

    sf::IntRect rect(x * tileSize, y * tileSize, tileSize, tileSize);
    auto& frames = const_cast<std::vector<sf::IntRect>&>(it->second.frames);
    frames.push_back(rect);
    selected_frame_index = static_cast<int>(frames.size() - 1);
    animator.addAnimation(current_animation_name, it->second);
}

#pragma endregion