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

    uint32_t selectedId = SceneHierarchyDisplay::getSelectedEntityId();

    if (selectedId == NO_ENTITY_SELECTED)
    {
        ImGui::Text("No object selected");
        ImGui::End();
        return;
    }

    entt::entity selected = entt::entity(selectedId);
    auto& reg = registry.getRegistry();

    if (!reg.valid(selected))
    {
        LOG_WARN("Selected entity is not valid: {}", selectedId);
        ImGui::Text("Selected entity is not valid");
        ImGui::End();
        return;
    }

    drawHeader();
    drawAddComponentButton();
    
    ImGui::Separator();
    if (auto id = reg.try_get<Components::Identification>(selected))
    {
        ImGui::Text("Selected:"); 
        ImGui::SameLine(); 
        ImGui::Selectable(id->name.c_str());
        ImGui::Text("Group:   "); 
        ImGui::SameLine(); 
        ImGui::Selectable(id->group.c_str());
        ImGui::Text("ID: %i", selectedId); 
    }
    ImGui::Separator();

    drawSelectableComponent(selected);
    drawComponentListPopup(selected);
    drawTransformSection(selected);
    drawMovementSection(selected);
    drawSpriteSection(selected);
    drawIndividualComponents(selected);
    drawStateSection(selected);
    drawFormation(selected);

    ImGui::End();
}

void InspectorDisplay::drawHeader()
{
}

void InspectorDisplay::drawAddComponentButton()
{
    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("component_popup");
}

void InspectorDisplay::drawComponentListPopup(entt::entity selected)
{
    if (!ImGui::BeginPopup("component_popup"))
        return;

    auto& reg = registry.getRegistry();

    // Render components
    if (!reg.all_of<Components::Sprite>(selected))
    {
        ImGui::SeparatorText("Render");
        if (ImGui::MenuItem("Sprite"))
        {
            reg.emplace<Components::Sprite>(selected);
        }
    }

    // Animation component
    if (!reg.all_of<Components::Animation>(selected))
    {
        if (ImGui::MenuItem("Animation"))
        {
            if (reg.all_of<Components::Sprite>(selected))
            {
                auto& spriteComp = reg.get<Components::Sprite>(selected);
                auto& animationComp = reg.emplace<Components::Animation>(selected, std::ref(spriteComp.sprite));

                std::string animFileName = "default_animations";
                if (!spriteComp.texture_filename.empty())
                {
                    size_t lastdot = spriteComp.texture_filename.find_last_of(".");
                    animFileName = (lastdot != std::string::npos)
                        ? spriteComp.texture_filename.substr(0, lastdot)
                        : spriteComp.texture_filename;
                    LOG_INFO("Determined animation file name for entity {}: {}",
                        static_cast<std::uint32_t>(selected), animFileName);
                }

                AnimationLoader loader;
                if (!loader.loadFromFile(animFileName, animationComp.animator))
                {
                    LOG_WARN("Failed to load animations from file: resources/Animations/{}.json for entity {}",
                        animFileName, static_cast<std::uint32_t>(selected));
                }
            }
            else
            {
                LOG_WARN("You need to add Sprite Component first!");
            }
        }
    }

    // Movement components
    if (!reg.all_of<Components::Direction>(selected))
    {
        ImGui::SeparatorText("Movement");
        if (ImGui::MenuItem("Control"))
        {
            reg.emplace<Components::Direction>(selected);
        }
    }

    if (!reg.all_of<Components::Velocity>(selected))
    {
        if (ImGui::MenuItem("Velocity"))
        {
            reg.emplace<Components::Velocity>(selected);
        }
    }

    if (!reg.all_of<Components::Pathfinding>(selected))
    {
        if (ImGui::MenuItem("Pathfinding"))
        {
            reg.emplace<Components::Pathfinding>(selected);
        }
    }

    // Other components
    if (!reg.all_of<Components::Selectable>(selected))
    {
        if (ImGui::MenuItem("Selectable"))
        {
            reg.emplace<Components::Selectable>(selected);
        }
    }

    if (!reg.all_of<Components::State>(selected))
    {
        if (ImGui::MenuItem("State"))
        {
            reg.emplace<Components::State>(selected);
        }
    }

    if (!reg.all_of<Components::Formation>(selected))
    {
        if (ImGui::MenuItem("Formation"))
        {
            reg.emplace<Components::Formation>(selected);
        }
    }

    ImGui::EndPopup();
}

void InspectorDisplay::drawTransformSection(entt::entity selected)
{
    auto& reg = registry.getRegistry();

    if (!reg.any_of<Components::Position, Components::Direction, Components::Velocity, Components::Selectable>(selected))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (reg.all_of<Components::Position>(selected))
            drawPositionComponent(selected);

        if (reg.all_of<Components::Sprite>(selected))
            drawScaleComponent(selected);

        if (reg.all_of<Components::Position>(selected))
            drawRotationComponent(selected);

        ImGui::Dummy(ImVec2(0,50));
    }
}

void InspectorDisplay::drawPositionComponent(entt::entity selected)
{
    auto& pos = registry.getRegistry().get<Components::Position>(selected);

    // X coordinate
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(150 / 255.0f, 78 / 255.0f, 76 / 255.0f, 1.0f));
    ImGui::Button("X");
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    float x = pos.position.x;
    if (ImGui::InputFloat("##pos_x", &x, 1.0f, 1.0f, "%.3f"))
        pos.position.x = x;

    ImGui::PopItemWidth();
    ImGui::SameLine();

    // Y coordinate
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(78 / 255.0f, 153 / 255.0f, 77 / 255.0f, 1.0f));
    ImGui::Button("Y");
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    float y = pos.position.y;
    if (ImGui::InputFloat("##pos_y", &y, 1.0f, 1.0f, "%.3f"))
        pos.position.y = y;

    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("Position");
}

void InspectorDisplay::drawScaleComponent(entt::entity selected)
{
    auto& sprite = registry.getRegistry().get<Components::Sprite>(selected);

    // X scale
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(150 / 255.0f, 78 / 255.0f, 76 / 255.0f, 1.0f));
    ImGui::Button("X");
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    float x = sprite.scale.x;
    if (ImGui::InputFloat("##scale_x", &x, 1.0f, 1.0f, "%.3f"))
    {
        sprite.scale.x = x;
        sprite.sprite.setScale(sprite.scale);
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    // Y scale
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(78 / 255.0f, 153 / 255.0f, 77 / 255.0f, 1.0f));
    ImGui::Button("Y");
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    float y = sprite.scale.y;
    if (ImGui::InputFloat("##scale_y", &y, 1.0f, 1.0f, "%.3f"))
    {
        sprite.scale.y = y;
        sprite.sprite.setScale(sprite.scale);
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("Scale");
}

void InspectorDisplay::drawRotationComponent(entt::entity selected)
{
    auto& pos = registry.getRegistry().get<Components::Position>(selected);
    float rotation = pos.angle;

    if (ImGui::InputFloat("Rotation", &rotation))
    {
        pos.angle = rotation;
    }
}

void InspectorDisplay::drawMovementSection(entt::entity selected)
{
    auto& reg = registry.getRegistry();

    if (!reg.any_of<Components::Direction, Components::Velocity, Components::Selectable>(selected))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    bool open_movement = ImGui::CollapsingHeader("Movement");

    if (open_movement)
    {
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveMovement")))
        {
            if (reg.all_of<Components::Sprite>(selected))
                reg.remove<Components::Sprite>(selected);
        }

        if (reg.all_of<Components::Direction>(selected))
            drawControlComponent(selected);

        if (reg.all_of<Components::Velocity>(selected))
            drawVelocityComponent(selected);
    }
}

void InspectorDisplay::drawControlComponent(entt::entity selected)
{
    auto& control = registry.getRegistry().get<Components::Direction>(selected);

    float dir[2] = { control.direction.x, control.direction.y };
    if (ImGui::InputFloat2("Direction", dir))
    {
        control.direction.x = dir[0];
        control.direction.y = dir[1];
    }
}

void InspectorDisplay::drawVelocityComponent(entt::entity selected)
{
    auto& velocity = registry.getRegistry().get<Components::Velocity>(selected);

    float vel[2] = { velocity.velocity.x, velocity.velocity.y };
    if (ImGui::InputFloat2("Velocity", vel))
    {
        velocity.velocity.x = vel[0];
        velocity.velocity.y = vel[1];
    }

    ImGui::InputFloat("Speed", &velocity.speed);
}

void InspectorDisplay::drawSpriteSection(entt::entity selected)
{
    auto& reg = registry.getRegistry();

    if (!reg.all_of<Components::Sprite>(selected))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    bool open_sprite = ImGui::CollapsingHeader("Sprite");

    ImGui::SameLine(ImGui::GetWindowWidth() - 50);
    if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveSprite")))
        reg.remove<Components::Sprite>(selected);

    if (open_sprite)
    {
        auto& spriteComponent = reg.get<Components::Sprite>(selected);

        const char* textureItems[] = { "Spearman.png", "Archer.png", "Swordman.png" };
        static int currentTextureItem = 0;
        std::string currentTextureName = spriteComponent.texture.getSize().x > 0 ? textureItems[currentTextureItem] : "Select Texture";

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

        ImGui::Dummy(ImVec2(0,50));
    }
}

void InspectorDisplay::drawIndividualComponents(entt::entity selected)
{
    auto& reg = registry.getRegistry();

    // Control component
    if (reg.all_of<Components::Direction>(selected) &&
        !reg.any_of<Components::Position, Components::Velocity, Components::Selectable>(selected))
    {
        ImGui::Text("Control");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveControl")))
        {
            reg.remove<Components::Direction>(selected);
        }
    }

    // Selectable component
    if (reg.all_of<Components::Selectable>(selected) &&
        !reg.any_of<Components::Position, Components::Direction, Components::Velocity>(selected))
    {
        drawSelectableComponent(selected);
    }

    // Velocity component
    if (reg.all_of<Components::Velocity>(selected) &&
        !reg.any_of<Components::Position, Components::Direction, Components::Selectable>(selected))
    {
        drawVelocityComponent(selected);
    }

    // Pathfinding component
    if (reg.all_of<Components::Pathfinding>(selected))
    {
        drawPathComponent(selected);
    }

    // Animation component
    if (reg.all_of<Components::Animation>(selected))
    {
        drawAnimationComponent(selected);
    }
}

void InspectorDisplay::drawSelectableComponent(entt::entity selected)
{
    if (registry.getRegistry().any_of<Components::Selectable>(selected))
    {
        ImGui::Separator();

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
}

void InspectorDisplay::drawPathComponent(entt::entity selected)
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Pathfinding", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto& pathfinding = registry.getRegistry().get<Components::Pathfinding>(selected);

        ImGui::Text("Pathfinding");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemovePathfinding")))
        {
            registry.getRegistry().remove<Components::Pathfinding>(selected);
        }

        ImGui::Checkbox("Is Moving", &pathfinding.is_moving);

        ImGui::Separator();

        if (pathfinding.start_node)
        {
            ImGui::Text("Start Node: %.1f x %.1f", pathfinding.start_node->position.x, pathfinding.start_node->position.y); 
        }
        else
        {
            ImGui::Text("Start Node: None");
        }

        if (pathfinding.end_node)
        {
            ImGui::Text("End Node: %.1f x %.1f", pathfinding.end_node->position.x, pathfinding.end_node->position.y); 
        }
        else
        {
            ImGui::Text("End Node: None");
        }

        if (pathfinding.current_node_index)
        {
            ImGui::Text("Current Node: %i", pathfinding.current_node_index); 
        }

        ImGui::Dummy(ImVec2(0,50));
    }
}

void InspectorDisplay::drawAnimationComponent(entt::entity selected)
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto& animation = registry.getRegistry().get<Components::Animation>(selected);

        ImGui::Text("Animation");
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::SmallButton(SET_ICON_TEXT((Icon::DUMP_FULL), "##RemoveAnimation")))
        {
            registry.getRegistry().remove<Components::Animation>(selected);
        }

        ImGui::Separator();
        ImGui::Dummy(ImVec2(0,50));
    }
}

void InspectorDisplay::drawStateSection(entt::entity selected)
{
    if (!registry.getRegistry().any_of<Components::State>(selected))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (registry.getRegistry().all_of<Components::State>(selected))
        {
            auto& state = registry.getRegistry().get<Components::State>(selected);

            const char* stateNames[] = { "NONE", "Idle", "Move", "Attack", "Dead", "Shoot", "Hit" };
            int currentStateIndex = static_cast<int>(state.state);

            if (ImGui::Combo("Character State", &currentStateIndex, stateNames, IM_ARRAYSIZE(stateNames)))
            {
                auto newState = state.state;
                state.state = static_cast<Components::CharacterState>(currentStateIndex);
            }
        }
        ImGui::Dummy(ImVec2(0,50));
    }
}

void InspectorDisplay::drawFormation(entt::entity selected)
{
    if (!registry.getRegistry().any_of<Components::Formation>(selected))
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Formation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto& formation = registry.getRegistry().get<Components::Formation>(selected);
        Components::Formation new_formation = formation;

        ImGui::Checkbox("Leader", &new_formation.is_leader);

        const char* formation_names[] = { /*"NONE", */"Line", "Square", "Circle" };
        int current_formation_id = static_cast<int>(new_formation.formation_type);

        if (ImGui::Combo("Formation State", &current_formation_id, formation_names, IM_ARRAYSIZE(formation_names)))
        {
            auto newState = new_formation.formation_type;
            new_formation.formation_type = static_cast<Components::FormationType>(current_formation_id);
        }

        ImGui::Separator();

        // Squad ID
        int squadId = static_cast<int>(new_formation.squad_id);
        if (ImGui::InputInt("Squad ID", &squadId))
        {
            new_formation.squad_id = static_cast<uint32_t>(squadId);
        }

        // Formation Index
        int formationIndex = static_cast<int>(new_formation.formation_index);
        if (ImGui::InputInt("Formation Index", &formationIndex))
        {
            new_formation.formation_index = static_cast<uint32_t>(formationIndex);
        }

        // Formation Offset
        float offset[2] = { new_formation.formation_offset.x, new_formation.formation_offset.y };
        if (ImGui::InputFloat2("Formation Offset", offset))
        {
            new_formation.formation_offset.x = offset[0];
            new_formation.formation_offset.y = offset[1];
        }

        ImGui::Dummy(ImVec2(0,50));
    }
}

void InspectorDisplay::update(const float& delta_time)
{
}
