#include "InspectorDisplay.hpp"
#include <imgui.h>
#include <Resource/ResourceManager.hpp>
#include "SceneHierarhyDisplay.hpp"

InspectorDisplay::InspectorDisplay(Registry& registry)
    : registry(registry)
{
}

void InspectorDisplay::draw()
{
    ImGui::Begin("Inspector");

    uint32_t selectedId = SceneHierrarhyDisplay::getSelectedEntityId();

    if (selectedId == UINT32_MAX)
    {
        ImGui::Text("No object selected");
        ImGui::End();
        return;
    }

    entt::entity selected = entt::entity(selectedId);

    if (!registry.getRegistry().valid(selected)) {
        ImGui::Text("Selected entity is not valid");
        ImGui::End();
        return;
    }

    auto& id = registry.getRegistry().get<Components::Identification>(selected);
    ImGui::Text("Selected: %s", id.name.c_str());
    ImGui::Separator();

#pragma region Add Component button

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("component_popup");

    ImGui::SeparatorText("Coponents list");

#pragma endregion

#pragma region All Componets popup
    if (ImGui::BeginPopup("component_popup"))
    {
        bool added = false;


        if (!registry.getRegistry().all_of<Components::Sprite>(selected))
        {
            ImGui::SeparatorText("Render");

            if (ImGui::MenuItem("Sprite"))
            {
                auto& newSprite = registry.getRegistry().emplace<Components::Sprite>(selected);
                added = true;
            }
        }

        if (!registry.getRegistry().all_of<Components::Animation>(selected))
        {
            if (ImGui::MenuItem("Animation"))
            {
                if (registry.getRegistry().all_of<Components::Sprite>(selected))
                {
                    auto& spriteComp    = registry.getRegistry().get<Components::Sprite>(selected);
                    auto& animationComp = registry.getRegistry().emplace<Components::Animation>(selected, std::ref(spriteComp.sprite));

                    AnimationLoader loader;
                    std::string animFileName = "default_animations";
                    if (!spriteComp.texture_filename.empty()) 
                    {
                        size_t lastdot = spriteComp.texture_filename.find_last_of(".");
                        if (lastdot != std::string::npos) 
                        {
                            animFileName = spriteComp.texture_filename.substr(0, lastdot);
                        }
                        else 
                        {
                            animFileName = spriteComp.texture_filename;
                        }
                        LOG_INFO("Determined animation file name for entity {}: {}", static_cast<std::uint32_t>(selected), animFileName);
                    }

                    if (!loader.loadFromFile(animFileName, animationComp.animator)) 
                    {
                        LOG_WARN("Failed to load animations from file: resources/Animations/{}.json for entity {}", animFileName, static_cast<std::uint32_t>(selected));
                        // if load critical delete component
                        // registry.getRegistry().remove<Components::Animation>(selected);
                    }
                }
                else
                {
                    LOG_WARN("You need added Sprite Component first!");
                }
            }
        }

        if (!registry.getRegistry().all_of<Components::Control>(selected))
        {
            ImGui::SeparatorText("Movement");

            if (ImGui::MenuItem("Control"))
            {
                registry.getRegistry().emplace<Components::Control>(selected);
                added = true;
            }
        }

        if (!registry.getRegistry().all_of<Components::Velocity>(selected))
        {
            if (ImGui::MenuItem("Velocity"))
            {
                registry.getRegistry().emplace<Components::Velocity>(selected);
            }
        }

        if (!registry.getRegistry().all_of<Components::Pathfinding>(selected))
        {
            if (ImGui::MenuItem("Pathfinding"))
            {
                registry.getRegistry().emplace<Components::Pathfinding>(selected);
            }
        }

        if (!registry.getRegistry().all_of<Components::Selectable>(selected))
        {
            if (ImGui::MenuItem("Selectable"))
            {
                registry.getRegistry().emplace<Components::Selectable>(selected);
                added = true;
            }
        }

        if (!registry.getRegistry().all_of<Components::State>(selected))
        {
            if (ImGui::MenuItem("State"))
            {
                registry.getRegistry().emplace<Components::State>(selected);
            }
        }

        ImGui::EndPopup();
    }

#pragma endregion

#pragma region Select

    if (registry.getRegistry().any_of<Components::Selectable>(selected))
    {
        ImGui::Separator();
        //Select
        if (registry.getRegistry().all_of<Components::Selectable>(selected))
        {
            auto& selectable = registry.getRegistry().get<Components::Selectable>(selected);
            Components::Selectable new_selectable = selectable;

            if (ImGui::Checkbox("Select", &new_selectable.is_selected))
            {
                registry.getRegistry().replace<Components::Selectable>(selected, new_selectable);
            }
        }

        ImGui::Separator();
    }

#pragma endregion

#pragma region Transform colaps

    if (registry.getRegistry().any_of<Components::Position, Components::Control, Components::Velocity, Components::Selectable>(selected))
    {
        //Transform
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //Position
            if (registry.getRegistry().all_of<Components::Position>(selected))
            {
                auto& pos = registry.getRegistry().get<Components::Position>(selected);
                Components::Position newPos = pos;

                // X
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Button("X");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
                if (ImGui::InputFloat("##pos_x", reinterpret_cast<float*>(&newPos.position.x), 1.0f, 1.0f, "%.3f"))
                {
                    registry.getRegistry().replace<Components::Position>(selected, newPos);
                }

                ImGui::PopItemWidth();
                ImGui::SameLine();

                // Y
                ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
                ImGui::Button("Y");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
                if (ImGui::InputFloat("##pos_y", reinterpret_cast<float*>(&newPos.position.y), 1.0f, 1.0f, "%.3f"))
                {
                    registry.getRegistry().replace<Components::Position>(selected, newPos);
                }
                ImGui::PopItemWidth();
                ImGui::SameLine();
                ImGui::Text("Position");
            }

            //Scale
            if (registry.getRegistry().all_of<Components::Sprite>(selected))
            {
                auto& scale = registry.getRegistry().get<Components::Sprite>(selected);
                Components::Sprite new_scale = scale;

                // X
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Button("X");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
                if (ImGui::InputFloat("##scale_x", reinterpret_cast<float*>(&new_scale.scale.x), 1.0f, 1.0f, "%.3f"))
                {
                    registry.getRegistry().replace<Components::Sprite>(selected, scale);
                }

                ImGui::PopItemWidth();
                ImGui::SameLine();

                // Y
                ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
                ImGui::Button("Y");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
                if (ImGui::InputFloat("##scale_y", reinterpret_cast<float*>(&new_scale.scale.y), 1.0f, 1.0f, "%.3f"))
                {
                    registry.getRegistry().replace<Components::Sprite>(selected, scale);
                }
                ImGui::PopItemWidth();
                ImGui::SameLine();
                ImGui::Text("Scale");
            }

            if (registry.getRegistry().all_of<Components::Position>(selected))
            {
                auto& pos = registry.getRegistry().get<Components::Position>(selected);
                Components::Position newPos = pos;
                // Roatation
                if (ImGui::InputFloat("Roatation", reinterpret_cast<float*>(&newPos.rotation)))
                {
                    registry.getRegistry().replace<Components::Position>(selected, newPos);
                }
            }
        }
    }

#pragma endregion

#pragma region Movement colaps
    // Movement
    if (registry.getRegistry().any_of<Components::Control, Components::Velocity, Components::Selectable>(selected))
    {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        bool open_movement = ImGui::CollapsingHeader("Movement");

        if (open_movement)
        {

            ImGui::SameLine(ImGui::GetWindowWidth() - 50);
            if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveMovement")))
            {
                registry.getRegistry().remove<Components::Sprite>(selected);
            }

            // Movement
            if (registry.getRegistry().all_of<Components::Control>(selected))
            {
                auto& control = registry.getRegistry().get<Components::Control>(selected);
                Components::Control new_control = control;

                if (ImGui::InputFloat2("Direction", reinterpret_cast<float*>(&new_control.direction)))
                {
                    registry.getRegistry().replace<Components::Control>(selected, new_control);
                }
            }

            //Velocity & Speed
            if (registry.getRegistry().all_of<Components::Velocity>(selected))
            {
                auto& velocity = registry.getRegistry().get<Components::Velocity>(selected);
                Components::Velocity new_velocity = velocity;

                if (ImGui::InputFloat2("Velocity", reinterpret_cast<float*>(&new_velocity.velocity)))
                {
                    registry.getRegistry().replace<Components::Velocity>(selected, new_velocity);
                }


                if (ImGui::InputFloat("Speed", &new_velocity.speed))
                {
                    registry.getRegistry().replace<Components::Velocity>(selected, new_velocity);
                }
            }
        }
    }

#pragma endregion

#pragma region Sprite Renderer
    // Sprite Renderer
    if (registry.getRegistry().all_of<Components::Sprite>(selected))
    {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        bool open_sprite = ImGui::CollapsingHeader("Sprite");

        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveSprite")))
        {
            registry.getRegistry().remove<Components::Sprite>(selected);
        }

        if (open_sprite)
        {
            auto& spriteComponent = registry.getRegistry().get<Components::Sprite>(selected);

            const char* textureItems[] = { "Spearman.png", "Archer.png", "Swordman.png" };
            static int currentTextureItem = 0;
            std::string currentTextureName = spriteComponent.texture.getSize().x > 0 ?
                textureItems[0] : "Select Texture";

            if (ImGui::BeginCombo("Texture", currentTextureName.c_str()))
            {
                for (int n = 0; n < IM_ARRAYSIZE(textureItems); n++)
                {
                    bool is_selected = (currentTextureItem == n);
                    if (ImGui::Selectable(textureItems[n], is_selected))
                    {
                        currentTextureItem = n;
                        spriteComponent.texture_filename = textureItems[n];
                        LOG_INFO("Set texture filename to: {}", spriteComponent.texture_filename);

                        sf::Texture newTexture = ResourceLoader::instance().getTexture(spriteComponent.texture_filename);
                        spriteComponent.texture = newTexture;
                        spriteComponent.sprite.setTexture(spriteComponent.texture, true);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Texture IntRect
            sf::IntRect& intRect = spriteComponent.int_rect;
            int rectValues[4] = { intRect.left, intRect.top, intRect.width, intRect.height };

            if (ImGui::InputInt4("Texture Rect (L,T,W,H)", rectValues))
            {
                intRect.left = rectValues[0];
                intRect.top = rectValues[1];
                intRect.width = rectValues[2];
                intRect.height = rectValues[3];
                spriteComponent.sprite.setTextureRect(intRect);
            }

            // Color setup
            sf::Color currentColor = spriteComponent.sprite.getColor();
            float color[4] = {
                currentColor.r / 255.0f,
                currentColor.g / 255.0f,
                currentColor.b / 255.0f,
                currentColor.a / 255.0f
            };

            if (ImGui::ColorEdit4("Color", color))
            {
                sf::Color newColor(
                    static_cast<sf::Uint8>(color[0] * 255.0f),
                    static_cast<sf::Uint8>(color[1] * 255.0f),
                    static_cast<sf::Uint8>(color[2] * 255.0f),
                    static_cast<sf::Uint8>(color[3] * 255.0f)
                );
                spriteComponent.sprite.setColor(newColor);
            }
        }
    }

#pragma endregion

#pragma region Control

    // Control
    if (registry.getRegistry().all_of<Components::Control>(selected) &&
        !registry.getRegistry().any_of<Components::Position, Components::Velocity, Components::Selectable>(selected))
    {
        ImGui::Text("Control");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveControl")))
        {
            registry.getRegistry().remove<Components::Control>(selected);
        }
    }

#pragma endregion

#pragma region Selectable

    // Selectable
    if (registry.getRegistry().all_of<Components::Selectable>(selected) &&
        !registry.getRegistry().any_of<Components::Position, Components::Control, Components::Velocity>(selected))
    {
        ImGui::Text("Selectable");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveSelectable")))
        {
            registry.getRegistry().remove<Components::Selectable>(selected);
        }
    }

#pragma endregion

#pragma region Velocity

    // Velocity
    if (registry.getRegistry().all_of<Components::Velocity>(selected) &&
        !registry.getRegistry().any_of<Components::Position, Components::Control, Components::Selectable>(selected))
    {
        ImGui::Text("Velocity");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);

        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveVelocity")))
        {
            registry.getRegistry().remove<Components::Velocity>(selected);
        }
    }

#pragma endregion

#pragma region Pathfinding
    // Pathfinding
    if (registry.getRegistry().all_of<Components::Pathfinding>(selected))
    {
        ImGui::Text("Pathfinding");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemovePathfinding")))
        {
            registry.getRegistry().remove<Components::Pathfinding>(selected);
        }
    }

#pragma endregion

#pragma region Animation

    // Animation
    if (registry.getRegistry().all_of<Components::Animation>(selected))
    {
        ImGui::Text("Animation");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);

        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveAnimation")))
        {
            registry.getRegistry().remove<Components::Animation>(selected);
        }
    }

#pragma endregion

#pragma region State
    // State
    if (registry.getRegistry().any_of<Components::State>(selected))
    {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (registry.getRegistry().all_of<Components::State>(selected))
            {
                auto& state = registry.getRegistry().get<Components::State>(selected);
                Components::State new_state = state;

                const char* stateNames[] = { "NONE", "Idle", "Move", "Attack", "Dead", "Shoot", "Hit" };
                int currentStateIndex = static_cast<int>(new_state.state);
                if (ImGui::Combo("Character State", &currentStateIndex, stateNames, IM_ARRAYSIZE(stateNames)))
                {
                    new_state.state = static_cast<Components::CharacterState>(currentStateIndex);
                    if (new_state.state != state.state)
                    {
                        registry.getRegistry().replace<Components::State>(selected, new_state);
                    }
                }
            }
        }
    }

#pragma endregion

    ImGui::End();
}


void InspectorDisplay::update(const float& delta_time)
{
}
